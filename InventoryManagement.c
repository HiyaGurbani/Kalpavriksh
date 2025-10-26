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

bool addNewProduct(Product **products, unsigned int *size) {
    Product *temporary = realloc (*products, (*size+1) * sizeof(Product));
    if (temporary == NULL) 
    {
        return false;
    }
    *products = temporary;
    (*size)++;

    printf("\nEnter new product details: ");
    printf ("\nProduct ID: ");
    scanf ("%u", &(*products)[*size-1].id);

    printf ("Product Name: ");
    while (getchar() != '\n');
    fgets ((*products)[*size-1].name, sizeof((*products)[*size-1].name), stdin);
    (*products)[*size-1].name[strcspn((*products)[*size-1].name, "\n")] = '\0';

    printf ("Product Price: ");
    scanf ("%f", &(*products)[*size-1].price);

    printf ("Product Quantity: ");
    scanf ("%u", &(*products)[*size-1].quantity);

    printf("Product added successfully!\n");

    return true;
}

void viewProducts (Product *products, const unsigned int size){
    if (size == 0)
    {
        printf ("No Products Exist. Add new product first.");
    }
    printf("\n=========PRODUCT LIST=========\n");
    for (unsigned int index = 0; index < size; index++)
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

    for (unsigned int index = 0; index < size; index++)
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
    
    for (unsigned int index = 0; index < size; index++)
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

//Helper Function
bool compareName (char *name1, char *name2) {
    while (*name2 != '\0')
    {   
        if (*name1 != *name2)
        {
            return false;
        } 
        name2++;
        name1++;
    }
    return true;
}
 
void searchProductByName (Product *products, const unsigned int size) {
    char currentName[NAME_SIZE];
    printf ("\nEnter name to search (partial allowed): ");
    fgets (currentName, sizeof(currentName), stdin);
    currentName[strcspn(currentName, "\n")] = '\0';

    bool isNameExists = false;

    for (int index = 0; index < size; index++)
    {
        if (compareName(products[index].name, currentName))
        {
            if (!isNameExists)
            {
                printf("\nProducts Found: \n");
                isNameExists = true;
            }
            printf("Product ID: %u | Product Name: %s | Price: %.2f | Quantity: %u\n",
                products[index].id, products[index].name, products[index].price, products[index].quantity);
        }
    }

    if (!isNameExists)
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

    for (unsigned int index = 0; index < size; index++)
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

Product* deleteProduct(Product *products, unsigned int *size) {
    unsigned int currentId;
    printf ("\nEnter Product ID to delete: ");
    scanf ("%u", &currentId);

    bool isIdExists = false;

    for (unsigned int index = 0; index < *size; index++)
    {
        if (products[index].id == currentId)
        {
            isIdExists = true;
            for (unsigned int shiftIndex = index; shiftIndex < *size - 1; shiftIndex)
            {
                products[shiftIndex] = products[shiftIndex + 1];
            }
            (*size)--;
            printf ("Product Deleted Successfully.\n");
            break;
        }
    }

    if (!isIdExists)
    {
        printf("Product Not Found.\n");
        return products;
    }

    if (*size == 0)
    {
        free (products);
        products = NULL;
        return NULL;
    } 

    //Attempting to shrink the memory 
    Product *temporary = realloc (products, (*size) * sizeof(Product));
    if (temporary == NULL)
    {
        printf("Warning: Memory reallocation failed. Array size not reduced, but deletion successful.\n");
        return products;
    }

    return temporary;  
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
                if (!addNewProduct(&products, &size))
                {
                    printf("Memory allocation failed. Please try again.\n");
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
                searchProductByName(products, size);
                break;

            case 6:
                searchProductByRange(products, size);
                break;

            case 7:
                // if (!deleteProduct(&products, &size))
                // {
                //     printf("Warning: Memory reallocation failed. Array size not reduced, but deletion successful.\n");
                // }
                products = deleteProduct(products, &size);
                break;

            case 8:
                free (products);
                products = NULL;
                printf ("Memory released successfully. Exiting program...");
                break;

            default:
                printf("Invalid Choice! Please choose again.");
        }
    }
}