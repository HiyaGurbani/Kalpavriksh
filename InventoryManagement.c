#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define NAME_SIZE 50

typedef struct {
    unsigned int id;
    char name[NAME_SIZE];
    float price;
    unsigned int quantity;
} Product;

Product* getProductsDetails(const unsigned int initialNumber) {
    Product *initialProducts = (Product *) malloc (initialNumber * sizeof(Product));

    if (initialProducts == NULL) 
    {
        printf("Error Allocating Memory");
        return initialProducts;
    }

    for (int index = 0; index < initialNumber; index++)
    {
        printf ("\nEnter details for product %d: ", index + 1);

        printf ("\nProduct ID: ");
        scanf ("%u", &initialProducts[index].id);

        printf ("Product Name: ");
        while (getchar() != '\n');
        fgets (initialProducts[index].name, sizeof(initialProducts[index].name), stdin);
        initialProducts[index].name[strcspn(initialProducts[index].name, "\n")] = '\0';

        printf ("Product Price: ");
        scanf ("%f", &initialProducts[index].price);

        printf ("Product Quantity: ");
        scanf ("%u", &initialProducts[index].quantity);
    }

    return initialProducts;
}

void addNewProduct(Product *products, unsigned int *size) {
    products = realloc (products, (++(*size)) * sizeof(Product));
    if (products == NULL) 
    {
        (*size)--;
        printf("Memory allocation failed!\n");
        return products;
    }

    printf("Enter new product details: ");
    printf ("\nProduct ID: ");
    scanf ("%u", &products[*size].id);

    printf ("Product Name: ");
    while (getchar() != '\n');
    fgets (products[*size].name, sizeof(products[*size].name), stdin);
    products[*size].name[strcspn(products[*size].name, "\n")] = '\0';

    printf ("Product Price: ");
    scanf ("%f", &products[*size].price);

    printf ("Product Quantity: ");
    scanf ("%u", &products[*size].quantity);

}

int main () {
    unsigned int initialNumber;
    printf("Enter initial number of products: ");
    scanf ("%u", &initialNumber);

    Product *products = getProductsDetails(initialNumber);
    if (products == NULL)
    {
        printf("Error Allocating Memory.");
        return 0;
    }

    unsigned int size = initialNumber;

    unsigned int choice;
    while (true)
    {
        printf ("\n========= INVENTORY MENU =========\n");
        printf ("1. Add New Product\n"
                "2. View All Products\n"
                "3. Update Quantity\n"
                "4. Search Product By ID\n"
                "5. Search Product by Name\n"
                "6. Search Product by Price Range\n"
                "7. Delete Product\n"
                "8. Exit");

        printf("Enter your choice: ");
        scanf("%u", &choice);

        switch (choice)
        {
            case 1:
                addNewProduct(products, &size);
                break;
            
            case 2:
                break;
            
            case 3:
                break;

            case 4:
                break;

            case 5:
                break;

            case 6:
                break;

            case 7:
                break;

            case 8:
                break;

            default:
                printf("Invalid Choice! Please choose again.");
        }
    }
}