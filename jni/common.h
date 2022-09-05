#pragma once
#include <sys/sendfile.h>
#include <linux/fs.h>
#include <fcntl.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/stat.h>


struct file_attr {
    struct stat st;
    char con[128];
};