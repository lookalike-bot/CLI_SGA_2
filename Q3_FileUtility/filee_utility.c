#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define RECORD_SIZE 50
#define FILENAME "employees.dat"

void write_record(int fd, int index, const char *name, int id) {
    char buffer[RECORD_SIZE];
    memset(buffer, 0, RECORD_SIZE);
    snprintf(buffer, RECORD_SIZE, "ID:%d,Name:%s", id, name);

    off_t offset = index * RECORD_SIZE;
    lseek(fd, offset, SEEK_SET);
    write(fd, buffer, RECORD_SIZE);
}

void read_record(int fd, int index) {
    char buffer[RECORD_SIZE];
    memset(buffer, 0, RECORD_SIZE);

    off_t offset = index * RECORD_SIZE;
    lseek(fd, offset, SEEK_SET);
    ssize_t bytes = read(fd, buffer, RECORD_SIZE);

    if (bytes > 0) {
        printf("Record %d: %s\n", index, buffer);
    } else {
        printf("Record %d: (empty or not found)\n", index);
    }
}

int main() {
    int fd = open(FILENAME, O_RDWR | O_CREAT, 0644);
    if (fd < 0) {
        perror("open failed");
        exit(1);
    }

    // Write 3 employee records
    write_record(fd, 0, "Alice", 101);
    write_record(fd, 1, "Bob", 102);
    write_record(fd, 2, "Charlie", 103);

    printf("--- Initial Records ---\n");
    read_record(fd, 0);
    read_record(fd, 1);
    read_record(fd, 2);

    // Update record at index 1 without touching others
    write_record(fd, 1, "Robert", 102);

    printf("--- After Updating Record 1 ---\n");
    read_record(fd, 0);
    read_record(fd, 1);
    read_record(fd, 2);

    close(fd);
    return 0;
}
