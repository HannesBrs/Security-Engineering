#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <time.h>

void print_file_info(const char *filename) {
    struct stat file_stat;
    if (lstat(filename, &file_stat) == -1) {
        perror("lstat");
        return;
    }

    printf("File: %s\n", filename);

    printf("Type: ");
    switch (file_stat.st_mode & S_IFMT) {
        case S_IFSOCK:
            printf("socket\n");
            break;
        case S_IFLNK:
            printf("symbolic link\n");
            break;
        case S_IFREG:
            printf("regular file\n");
            break;
        case S_IFBLK:
            printf("block device\n");
            break;
        case S_IFDIR:
            printf("directory\n");
            break;
        case S_IFCHR:
            printf("character device\n");
            break;
        case S_IFIFO:
            printf("FIFO\n");
            break;
        default:
            printf("Unknown\n");
            break;
    }

    printf("User ID: %u\n", file_stat.st_uid);

    struct passwd *pw = getpwuid(file_stat.st_uid);
    if (pw != NULL) {
        printf("User Name: %s\n", pw->pw_name);
    } else {
        printf("User Name: Unknown\n");
    }


    printf("Group ID: %u\n", file_stat.st_gid);


    printf("Access Bits: %o\n", file_stat.st_mode & 07777);


    char timeOutputString[50];
    struct tm *timeData;

    timeData = localtime(&file_stat.st_atime);
    strftime(timeOutputString, 50, "%c", timeData);
    printf("Last Access: %s\n", timeOutputString);

    timeData = localtime(&file_stat.st_ctime);
    strftime(timeOutputString, 50, "%c", timeData);
    printf("Last Inode Change: %s\n", timeOutputString);

    timeData = localtime(&file_stat.st_mtime);
    strftime(timeOutputString, 50, "%c", timeData);
    printf("Last File Change: %s\n", timeOutputString);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("You have to provide at least one filename \n");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; ++i) {
        print_file_info(argv[i]);
    }

    return 0;
}
