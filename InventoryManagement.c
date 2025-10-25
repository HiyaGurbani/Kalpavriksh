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
        return NULL;
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

Product* addNewProduct(Product *products, unsigned int *size) {
    products = realloc (products, (++(*size)) * sizeof(Product));
    if (products == NULL) 
    {
        (*size)--;
        printf("Memory allocation failed!\n");
        return NULL;
    }

    printf("\nEnter new product details: ");
    printf ("\nProduct ID: ");
    scanf ("%u", &products[*size-1].id);

    printf ("Product Name: ");
    while (getchar() != '\n');
    fgets (products[*size-1].name, sizeof(products[*size-1].name), stdin);
    products[*size-1].name[strcspn(products[*size-1].name, "\n")] = '\0';

    printf ("Product Price: ");
    scanf ("%f", &products[*size-1].price);

    printf ("Product Quantity: ");
    scanf ("%u", &products[*size-1].quantity);

    printf("Product added successfully!\n");

    return products;
}

void viewProducts (Product *products, const unsigned int size){
    printf("\n=========PRODUCT LIST=========\n");
    for (int index = 0; index < size; index++)
    {
        printf("Product ID: %u | Product Name: %s | Price: %.2f | Quantity: %u\n",
                products[index].id, products[index].name, products[index].price, products[index].quantity);
    }
}

void updateQuantity (Product *products, const unsigned int size){
    unsigned int currentId;
    bool isIdExists = false;
    printf ("\nEnter Product ID to update quantity: ");
    scanf ("%u", &currentId);

    for (int index = 0; index < size; index++)
    {
        if (products[index].id == currentId)
        {
            printf ("Enter new Quantity: ");
            scanf ("%u", &products[index].quantity);
            printf("Quantity updated successfully!\n");
            isIdExists = true;
        }
    }

    if (!isIdExists)
    {
        printf ("The entered Product ID doesn't exist.\n");
    }
}

void searchProductById (Product *products, const unsigned int size) {
    unsigned int currentId;
    bool isIdExists = false;
    printf ("\nEnter Product ID to search: ");
    scanf ("%u", &currentId);
    
    for (int index = 0; index < size; index++)
    {
        if (products[index].id == currentId)
        {
            printf("Product Found: Product ID: %u | Product Name: %s | Price: %.2f | Quantity: %u\n",
                products[index].id, products[index].name, products[index].price, products[index].quantity);
            isIdExists = true;
        }
    }

    if (!isIdExists)
    {
        printf ("Product Not Found.\n");
    }
}

void searchProductByRange (Product *products, const unsigned int size) {
    unsigned int minPrice, maxPrice;
    bool isProductExists = false;

    printf ("\nEnter minimun price: ");
    scanf ("%u", &minPrice);
    printf("Enter maximum price: ");
    scanf ("%u", &maxPrice);

    for (int index = 0; index < size; index++)
    {
        if (products[index].price >= minPrice && products[index].price <= maxPrice)
        {
            if (!isProductExists)
            {
                printf("\nProducts in price range: \n");
                isProductExists = true;
            }
            printf("Product ID: %u | Product Name: %s | Price: %.2f | Quantity: %u\n",
                products[index].id, products[index].name, products[index].price, products[index].quantity);
            
        }
    }

    if (!isProductExists)
    {
        printf ("No Product in Price Range.\n");
    }

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
                "8. Exit\n");

        printf("Enter your choice: ");
        scanf("%u", &choice);

        switch (choice)
        {
            case 1:
                products = addNewProduct(products, &size);
                if (products == NULL)
                {
                    printf("Error in memory allocation.");
                }
                break;
            
            case 2:
                viewProducts(products, size);
                break;
            
            case 3:
                updateQuantity(products, size);
                break;

            case 4:
                searchProductById(products, size);
                break;

            case 5:
                break;

            case 6:
                searchProductByRange(products, size);
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