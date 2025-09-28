//CRUD Operations on File

#include<stdio.h>

int main(){
    int choice;
    printf("List of Operations: \n");
    printf("1. Create User \n2. Read User \n3. Update User \n4. Delete User \n");
    printf("Enter the number of operation you want to perform: ");
    scanf("%d", &choice);

    if (choice==1) printf("Create User chosen");
    else if (choice==2) printf("Read User chosen");
    else if (choice==3) printf("Update User chosen");
    else if (choice==4) printf("Delete User chosen");
    else printf("Wrong Choice Selected");

    return 0;
}