#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

typedef struct {
    int id;
    char name[30];
    float salary;
} Employee;

#define FILENAME "employees.dat"

void write_records() {
    int fd = open(FILENAME, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd < 0) { perror("open"); exit(1); }

    Employee emps[3] = {
        {1, "Alice", 50000},
        {2, "Bob", 55000},
        {3, "Charlie", 60000}
    };

    for (int i = 0; i < 3; i++) {
        write(fd, &emps[i], sizeof(Employee));
    }
    close(fd);
    printf("3 employee records written.\n");
}

void update_record(int emp_id, float new_salary) {
    int fd = open(FILENAME, O_RDWR);
    if (fd < 0) { perror("open"); exit(1); }

    Employee e;
    while (read(fd, &e, sizeof(Employee)) == sizeof(Employee)) {
        if (e.id == emp_id) {
            e.salary = new_salary;
            // Move file pointer back to the start of this record
            lseek(fd, -sizeof(Employee), SEEK_CUR);
            write(fd, &e, sizeof(Employee));
            printf("Updated record ID %d to new salary %.2f\n", emp_id, new_salary);
            break;
        }
    }
    close(fd);
}

void read_record_at(int index) {
    int fd = open(FILENAME, O_RDONLY);
    if (fd < 0) { perror("open"); exit(1); }

    Employee e;
    // Jump directly to the desired record without reading previous ones
    lseek(fd, index * sizeof(Employee), SEEK_SET);
    if (read(fd, &e, sizeof(Employee)) == sizeof(Employee)) {
        printf("Record %d -> ID:%d Name:%s Salary:%.2f\n", index, e.id, e.name, e.salary);
    } else {
        printf("No record at index %d\n", index);
    }
    close(fd);
}

int main() {
    printf("===== Writing records =====\n");
    write_records();

    printf("===== Reading records BEFORE update =====\n");
    read_record_at(0);
    read_record_at(1);
    read_record_at(2);

    printf("===== Updating record ID 2 (Bob) =====\n");
    update_record(2, 65000);

    printf("===== Reading records AFTER update =====\n");
    read_record_at(0);
    read_record_at(1);
    read_record_at(2);

    return 0;
}
