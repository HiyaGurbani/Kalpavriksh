//CRUD Operations on File

#include<stdio.h>
#include<string.h>

struct User {
    int id;
    char name[100];
    int age;
};

void createUser(){
    struct User u;

    FILE *fp;
    fp = fopen("users.txt", "a");
    if (fp == NULL){
        printf("Error creating file\n");
        return;
    }

    printf("Enter ID: ");
    scanf("%d", &u.id);
    getchar();

    printf("Enter Name: ");
    fgets(u.name, sizeof(u.name), stdin);
    //Used to remove extra line with fgets
    u.name[strcspn(u.name, "\n")] = '\0';

    printf("Enter Age: ");
    scanf("%d", &u.age);

    fprintf(fp, "%d %s %d\n", u.id, u.name, u.age);

    fclose(fp);
    printf("User Created Successfully\n");
}

int main(){
    int choice;

    printf("List of Operations: \n");
    printf("1. Create User \n2. Read User \n3. Update User \n4. Delete User \n");
    printf("Enter the number of operation you want to perform: ");
    scanf("%d", &choice);

    if (choice==1){
        printf("Create User chosen\n");
        createUser();
    }

    else if (choice==2) printf("Read User chosen");
    else if (choice==3) printf("Update User chosen");
    else if (choice==4) printf("Delete User chosen");
    else printf("Wrong Choice Selected");

    return 0;
}