
#include <stdio.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <string.h>

#include "files.hpp"

int main(int argc, char *argv[])
{
    char dirName[] = "/data/user/0/com.xiaoyou.ToramOnline.aligames.uc/";

    printf("监控位置 : %s\n", dirName);

    int fd = inotify_init();
    if (fd < 0)
    {
        printf("inotify_init err.\n");
        return 0;
    }
    int wd = inotify_add_watch(fd, dirName, IN_ALL_EVENTS);
    if (wd < 0)
    {
        printf("inotify_add_watch err.\n");
        close(fd);
        return 0;
    }

    const int buflen = sizeof(struct inotify_event) * 0x100;
    char buf[buflen] = {0};
    fd_set readfds;

    while (1)
    {

        FD_ZERO(&readfds);

        FD_SET(fd, &readfds);

        int iRet = select(fd + 1, &readfds, 0, 0, 0); // 此处阻塞

        // printf("iRet的返回值:%d\n", iRet);
        if (-1 == iRet)
            break;

        if (iRet)
        {

            memset(buf, 0, buflen);
            int len = read(fd, buf, buflen);

            int i = 0;

            while (i < len)
            {

                struct inotify_event *event = (struct inotify_event *)&buf[i];

                // printf("event mask:%d\n", event->mask);

                if ((event->mask & IN_CREATE))
                {
                    printf("IN_CREATE\n");
                    printf("%s\n", event->name);
                    char path[PATH_MAX];
                    sprintf(path, "/data/user/0/com.xiaoyou.ToramOnline.aligames.uc/%s", event->name);
                    cp_afc(path, event->name);
                }
                if ((event->mask & IN_MODIFY))
                {
                    printf("IN_MODIFY\n");
                    printf("%s\n", event->name);
                    char path[PATH_MAX];
                    sprintf(path, "/data/user/0/com.xiaoyou.ToramOnline.aligames.uc/%s", event->name);
                    cp_afc(path, event->name);
                }
                i += sizeof(struct inotify_event) + event->len;
            }
        }
    }

    inotify_rm_watch(fd, wd); //移出监控
    close(fd);

    return 0;
}