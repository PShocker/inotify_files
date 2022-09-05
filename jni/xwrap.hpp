#include <sys/sendfile.h>
#include <linux/fs.h>
#include <fcntl.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/types.h>
#include <dirent.h>
#include "common.h"


int xopen(const char *pathname, int flags, mode_t mode) {
    int fd = open(pathname, flags, mode);
    if (fd < 0) {
        printf("open: %s", pathname);
    }
    return fd;
}

int xopen(const char *pathname, int flags) {
    int fd = open(pathname, flags);
    if (fd < 0) {
        printf("open: %s", pathname);
    }
    return fd;
}

ssize_t xsendfile(int out_fd, int in_fd, off_t *offset, size_t count) {
    ssize_t ret = sendfile(out_fd, in_fd, offset, count);
    if (ret < 0) {
        printf("sendfile");
    }
    return ret;
}

ssize_t xreadlink(const char *pathname, char *buf, size_t bufsiz) {
    ssize_t ret = readlink(pathname, buf, bufsiz);
    if (ret < 0) {
        printf("readlink %s", pathname);
    } else {
        buf[ret] = '\0';
    }
    return ret;
}

int xsymlink(const char *target, const char *linkpath) {
    int ret = symlink(target, linkpath);
    if (ret < 0) {
        printf("symlink %s->%s", target, linkpath);
    }
    return ret;
}

int setattr(const char *path, file_attr *a) {
    if (chmod(path, a->st.st_mode & 0777) < 0)
        return -1;
    if (chown(path, a->st.st_uid, a->st.st_gid) < 0)
        return -1;
    return 0;
}

int xlstat(const char *pathname, struct stat *buf) {
    int ret = lstat(pathname, buf);
    if (ret < 0) {
        printf("lstat %s\n", pathname);
    }
    return ret;
}