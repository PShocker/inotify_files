#include <sys/sendfile.h>
#include <linux/fs.h>
#include <fcntl.h>
#include <unistd.h>
#include <libgen.h>
#include "xwrap.hpp"
#include "common.h"
#include <sys/stat.h>

int getattr(const char *path, file_attr *a)
{
    if (xlstat(path, &a->st) == -1)
        return -1;
    return 0;
}

void cp_afc(const char *src, const char *dest)
{
    file_attr a;
    if (getattr(src, &a) == 0)
    {
        // unlink(dest);
        if (S_ISREG(a.st.st_mode))
        {
            int sfd = xopen(src, O_RDONLY | O_CLOEXEC);
            int dfd = xopen(dest, O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC, 0);
            xsendfile(dfd, sfd, nullptr, a.st.st_size);
            close(sfd);
            close(dfd);
        }
        else if (S_ISLNK(a.st.st_mode))
        {
            char buf[4096];
            xreadlink(src, buf, sizeof(buf));
            xsymlink(buf, dest);
        }

        setattr(dest, &a);
    }
}
