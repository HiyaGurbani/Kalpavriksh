#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

#define LINE_SIZE 200
#define NAME_SIZE 100
#define USER_FILE "users.txt"

struct User {
    int id;
    char name[NAME_SIZE];
    int age;
};

//Parses line of file and fills User Struct
bool parseUserLine(const char *line, struct User *user){
    char tempLine[LINE_SIZE];
    strncpy(tempLine, line, LINE_SIZE);
    tempLine[LINE_SIZE - 1] = '\0';

    char *lastSpace = strrchr(tempLine, ' ');
    if (lastSpace == NULL){
        return false;
    }

    *lastSpace = '\0';
    user->age = atoi(lastSpace+1);

    if (sscanf(tempLine, "%d %[^\n]", &user->id, user->name) != 2){
        return false;
    }

    return true;
}

//Operation 1 : Create a New User and checking the unique ID constraint
void createUser(){
    struct User user, parsedUser;

    FILE *usersFile;
    char line[LINE_SIZE];
    bool idExists = false;

    printf("Enter ID: ");
    scanf("%d", &user.id);
    getchar();

    usersFile = fopen(USER_FILE, "r");
    if (usersFile != NULL){
        while (fgets(line, sizeof(line), usersFile)){
            if (parseUserLine(line , &parsedUser) && parsedUser.id == user.id){
                idExists = true;
                break;
            }
        }
        fclose(usersFile);
    }

    if (idExists){
        printf("Error: User with ID %d already exists!\n", user.id);
        return;
    }

    usersFile = fopen(USER_FILE, "a");
    if (usersFile == NULL){
        printf("Error creating file\n");
        return;
    }

    printf("Enter Name: ");
    fgets(user.name, sizeof(user.name), stdin);
    user.name[strcspn(user.name, "\n")] = '\0';

    printf("Enter Age: ");
    scanf("%d", &user.age);

    fprintf(usersFile, "%d %s %d\n", user.id, user.name, user.age);
    fclose(usersFile);

    printf("User Created Successfully\n");
}

//Operation 2 : Display all users
void displayUsers(){
    FILE *usersFile;
    char line[LINE_SIZE];
    
    usersFile = fopen(USER_FILE, "r");
    if (usersFile == NULL){
        printf("Error Opening File. Try creating a User!\n");
        return;
    }

    if (fgets(line, sizeof(line), usersFile) == NULL){
        printf("There are no users. Kindly create a user!");
        fclose(usersFile);
        return;
    }

    printf("Users' List\n");
    printf("%s", line);
    while(fgets(line, sizeof(line), usersFile)){
        printf("%s",line);
    }

    fclose(usersFile);
}

//Operation 3 : Update user by ID
void updateUser(){
    FILE *usersFile, *tempFile;
    char line[LINE_SIZE];
    int id;
    bool userExists = false;
    struct User parsedUser;

    printf("Enter the ID of the user: ");
    scanf("%d", &id);

    usersFile = fopen(USER_FILE, "r");
    if (usersFile == NULL){
        printf("Error Opening File. Try creating a User!\n");
        return;
    }

    tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL){
        printf("Error Opening Temp File!\n");
        fclose(usersFile);
        return;
    }

    while(fgets(line, sizeof(line), usersFile)){
        if (parseUserLine(line, &parsedUser)){
            if (parsedUser.id == id){
                userExists = true;
                printf("Enter new name: ");
                getchar();
                fgets(parsedUser.name, sizeof(parsedUser.name), stdin);
                parsedUser.name[strcspn(parsedUser.name, "\n")] = '\0';

                printf("Enter new age: ");
                scanf("%d", &parsedUser.age);
            }
            fprintf(tempFile, "%d %s %d\n", parsedUser.id, parsedUser.name, parsedUser.age);
        }
    }

    fclose(usersFile);
    fclose(tempFile);

    if (userExists){
        remove(USER_FILE);
        rename("temp.txt", USER_FILE);
        printf("User updated successfully!\n");
    } else {
        remove("temp.txt");
        printf("User with ID %d not found.\n", id);
    }
}

//Operation 4 : Delete user by id 
void deleteUser(){
    FILE *usersFile, *tempFile;
    char line[LINE_SIZE];
    int id;
    bool userExists = false;
    struct User parsedUser;

    printf("Enter the ID of the user to delete: ");
    scanf("%d", &id);

    usersFile = fopen(USER_FILE, "r");
    if (usersFile == NULL){
        printf("Error Opening File. Try creating a user!\n");
        return;
    }

    tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL){
        printf("Error Opening Temp File!\n");
        fclose(usersFile);
        return;
    }

    while(fgets(line, sizeof(line), usersFile)){
        if (parseUserLine(line, &parsedUser)){
            if (parsedUser.id == id){
                userExists = true;
            } else {
                fprintf(tempFile, "%d %s %d\n", parsedUser.id, parsedUser.name, parsedUser.age);
            }
        }
    }

    fclose(usersFile);
    fclose(tempFile);

    if (userExists){
        remove(USER_FILE);
        rename("temp.txt", USER_FILE);
        printf("User deleted successfully!\n");
    } else {
        remove("temp.txt");
        printf("User with ID %d not found.\n", id);
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