#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

#define NAME_SIZE 100
#define USER_FILE "users.dat"

typedef struct{
    int id;
    char name[NAME_SIZE];
    int age;
} User;

void createUser(){
    User user, tempUser;
    bool isIdExists = false;

    printf("Enter ID: ");
    scanf("%d", &user.id);
    getchar();

    //Checking for Unique User Validation
    FILE *usersFile = fopen(USER_FILE, "rb");
    if (usersFile){
        while (fread(&tempUser, sizeof(User), 1, usersFile) == 1){
            if (tempUser.id == user.id){
                isIdExists = true;
                break;
            }
        }
        fclose(usersFile);
    }

    if (isIdExists){
        printf("Error: User with ID %d already exists!\n", user.id);
        return;
    }

    usersFile = fopen(USER_FILE, "ab");
    if (!usersFile){
        printf("Error creating file\n");
        return;
    }

    printf("Enter Name: ");
    fgets(user.name, sizeof(user.name), stdin);
    user.name[strcspn(user.name, "\n")] = '\0';

    printf("Enter Age: ");
    scanf("%d", &user.age);

    fwrite(&user, sizeof(User), 1, usersFile);
    fclose(usersFile);

    printf("User Created Successfully\n");
}

void displayUsers(){
    FILE *usersFile = fopen(USER_FILE, "rb");
    if (!usersFile){
        printf("Error Opening File. Try creating a User!\n");
        return;
    }

    //Checking if file is empty
    fseek(usersFile, 0, SEEK_END);
    long fileSize = ftell(usersFile);
    if (fileSize == 0){
        printf("No Users Available. Create a New User!\n");
        fclose(usersFile);
        return;
    }
    rewind(usersFile);

    User user;

    printf("Users' List\n");
    printf("Id\tName\tAge\n");
    while (fread(&user, sizeof(User), 1, usersFile) == 1){
        printf("%d\t%s\t%d\n", user.id, user.name, user.age);
    }

    fclose(usersFile);
}

void updateUser(){
    User user;
    int targetId;
    bool isUserExists = false;

    printf("Enter the ID of the user: ");
    scanf("%d", &targetId);
    getchar();

    FILE *usersFile = fopen(USER_FILE, "rb");
    FILE *tempFile = fopen("temp.dat", "wb");
    if (!usersFile || !tempFile){
        printf("Error Opening File!\n");
        if (usersFile){
            fclose(usersFile);
        }
        if (tempFile){
            fclose(tempFile);
        }
        return;
    }

    while (fread(&user, sizeof(User), 1, usersFile) == 1){
        if (user.id == targetId){
            isUserExists = true;

            printf("Enter new Name: ");
            fgets(user.name, sizeof(user.name), stdin);
            user.name[strcspn(user.name, "\n")] = '\0';

            printf("Enter new Age: ");
            scanf("%d", &user.age);
            getchar();
        }
        fwrite(&user, sizeof(User), 1, tempFile);
    }

    fclose(usersFile);
    fclose(tempFile);

    if (isUserExists){
        remove(USER_FILE);
        rename("temp.dat", USER_FILE);
        printf("User updated successfully!\n");
    } else {
        remove("temp.dat");
        printf("User with ID %d not found.\n", targetId);
    }
}

void deleteUser(){
    User user;
    int targetId;
    bool isUserExists = false;

    printf("Enter the ID of the user to delete: ");
    scanf("%d", &targetId);

    FILE *usersFile = fopen(USER_FILE, "rb");
    FILE *tempFile = fopen("temp.dat", "wb");
    if (!usersFile || !tempFile){
        printf("Error Opening File!\n");
        if (usersFile){
            fclose(usersFile);
        }
        if (tempFile){
            fclose(tempFile);
        }
        return;
    }

    while(fread(&user, sizeof(User), 1, usersFile) == 1){
        if (user.id == targetId){
            isUserExists = true;
        } else {
            fwrite(&user, sizeof(User), 1, tempFile);
        }
    }

    fclose(usersFile);
    fclose(tempFile);

    if (isUserExists){
        remove(USER_FILE);
        rename("temp.dat", USER_FILE);
        printf("User deleted successfully!\n");
    } else {
        remove("temp.dat");
        printf("User with ID %d not found.\n", targetId);
    }
}


int main(){
    int choice;

    while(1){
        printf("\nList of Operations: \n");
        printf("1. Create User \n2. Display Users \n3. Update User \n4. Delete User \n5. Exit\n");
        printf("Enter the number of operation you want to perform: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Create User chosen\n");
                createUser();
                break;

            case 2:
                printf("Display Users chosen\n");
                displayUsers();
                break;

            case 3:
                printf("Update User chosen\n");
                updateUser();
                break;

            case 4:
                printf("Delete User chosen\n");
                deleteUser();
                break;

            case 5:
                printf("Exiting the program\n");
                return 0;

            default:
                printf("Invalid Choice! Please Choose Again.\n");
                break;       
        }
    }

    return 0;
}