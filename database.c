#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "students.dat"


typedef struct {
    int id;
    char name[50];
    int age;
    float marks;
} Student;


void addStudent();
void displayStudents();
void searchStudent();
void deleteStudent();
void updateStudent();

int main() {
    int choice;

    while (1) {
        printf("\n=== Student Database (Binary File) ===\n");
        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student by ID\n");
        printf("4. Delete Student by ID\n");
        printf("5. Update Student by ID\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: deleteStudent(); break;
            case 5: updateStudent(); break;
            case 6: exit(0);
            default: printf("Invalid choice!\n");
        }
    }

    return 0;
}

void addStudent() {
    FILE *fp = fopen(FILENAME, "ab");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    Student s;
    printf("Enter ID: ");
    scanf("%d", &s.id);
    printf("Enter Name: ");
    getchar();
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = '\0';
    printf("Enter Age: ");
    scanf("%d", &s.age);
    printf("Enter Marks: ");
    scanf("%f", &s.marks);

    fwrite(&s, sizeof(Student), 1, fp);
    fclose(fp);

    printf("Student added successfully!\n");
}

void displayStudents() {
    FILE *fp = fopen(FILENAME, "rb");
    if (!fp) {
        printf("No records found!\n");
        return;
    }

    Student s;
    printf("\n--- Student Records ---\n");
    while (fread(&s, sizeof(Student), 1, fp)) {
        printf("ID: %d, Name: %s, Age: %d, Marks: %.2f\n",
               s.id, s.name, s.age, s.marks);
    }

    fclose(fp);
}

void searchStudent() {
    FILE *fp = fopen(FILENAME, "rb");
    if (!fp) {
        printf("No records found!\n");
        return;
    }

    int id, found = 0;
    Student s;

    printf("Enter Student ID to search: ");
    scanf("%d", &id);

    while (fread(&s, sizeof(Student), 1, fp)) {
        if (s.id == id) {
            printf("Record Found!\n");
            printf("ID: %d, Name: %s, Age: %d, Marks: %.2f\n",
                   s.id, s.name, s.age, s.marks);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Student with ID %d not found.\n", id);
    }

    fclose(fp);
}

void deleteStudent() {
    FILE *fp = fopen(FILENAME, "rb");
    if (!fp) {
        printf("No records found!\n");
        return;
    }

    FILE *temp = fopen("temp.dat", "wb");
    if (!temp) {
        printf("Error creating temp file!\n");
        fclose(fp);
        return;
    }

    int id, found = 0;
    Student s;

    printf("Enter Student ID to delete: ");
    scanf("%d", &id);

    while (fread(&s, sizeof(Student), 1, fp)) {
        if (s.id == id) {
            found = 1;
            continue; 
        }
        fwrite(&s, sizeof(Student), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove(FILENAME);
    rename("temp.dat", FILENAME);

    if (found) {
        printf("Student with ID %d deleted successfully.\n", id);
    } else {
        printf("Student with ID %d not found.\n", id);
    }
}

void updateStudent() {
    FILE *fp = fopen(FILENAME, "rb+");
    if (!fp) {
        printf("No records found!\n");
        return;
    }

    int id, found = 0;
    Student s;

    printf("Enter Student ID to update: ");
    scanf("%d", &id);

    while (fread(&s, sizeof(Student), 1, fp)) {
        if (s.id == id) {
            printf("Current Record: ID=%d, Name=%s, Age=%d, Marks=%.2f\n",
                   s.id, s.name, s.age, s.marks);

            printf("Enter new Name: ");
            getchar();
            fgets(s.name, sizeof(s.name), stdin);
            s.name[strcspn(s.name, "\n")] = '\0';
            printf("Enter new Age: ");
            scanf("%d", &s.age);
            printf("Enter new Marks: ");
            scanf("%f", &s.marks);

            fseek(fp, -sizeof(Student), SEEK_CUR); 
            fwrite(&s, sizeof(Student), 1, fp);

            printf("Student record updated successfully!\n");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Student with ID %d not found.\n", id);
    }

    fclose(fp);
}