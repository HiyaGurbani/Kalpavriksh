#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define NAME_SIZE 51

#define MIN_INITIAL_PRODUCTS 1
#define MAX_INITIAL_PRODUCTS 100

#define MIN_ID 1
#define MAX_ID 10000

#define MIN_PRICE 0
#define MAX_PRICE 100000

#define MIN_QUANTITY 0
#define MAX_QUANTITY 1000000 

typedef struct {
    unsigned int id;
    char name[NAME_SIZE];
    float price;
    unsigned int quantity;
} Product;

void getInitialNumberOfProducts(unsigned int *initialProducts) {
    printf("Enter initial number of products: ");
    while (true)
    {
        if (scanf("%u", initialProducts) != 1) 
        {
            printf("Invalid Number! Kindly enter again: ");
            while (getchar() != '\n');
            continue;
        }
        if (*initialProducts < MIN_INITIAL_PRODUCTS || *initialProducts > MAX_INITIAL_PRODUCTS)
        {
            printf("Initial Number of products must be in range 1-100! Kindly Enter again: ");
            continue;
        }
        break;
    }
}

void getProductID(unsigned int *id) {
    while (true) {      
        if (scanf ("%u", id) != 1)
        {
            printf("Invalid ID! Kindly enter again: ");
            while (getchar() != '\n');
            continue;
        }
        if (*id < MIN_ID || *id > MAX_ID)
        {
            printf("ID must be in range 1-10000! Kindly enter again: ");
            continue;
        }
        break;
    }
}

int stringLength(char *string) {
    int length = 0;
    while (string[length] != '\0')
    {
        length++;
    }
    return length;
}

void getProductName(char *name) {
    while (getchar() != '\n');

    while (true)
    {   
        if (fgets(name, NAME_SIZE, stdin) == NULL) {
            printf("Error reading input. Try again.\n");
            continue;
        }
        name[strcspn(name, "\n")] = '\0';
        if (stringLength(name) == 0)
        {
            printf("Name cannot be empty. Kindly enter again: ");
            continue;
        }
        if (stringLength(name) >= NAME_SIZE - 1)
        {
            while (getchar() != '\n');
            printf("Name too long! Only first %d characters will be considered.\n", NAME_SIZE - 1);      
        }
        break;
    }
}

void getProductPrice(float *price) {
    while (true) 
    {
        if (scanf ("%f", price) != 1)
        {
            printf("Invalid Price! Kindly enter again. ");
            while (getchar() != '\n');
            continue;
        }
        if (*price < MIN_PRICE || *price > MAX_PRICE)
        {
            printf("Price must be in range 0-100000! Kindly enter again. ");
            continue;
        }
        break;
    }
}

void getProductQuantity(unsigned int *quantity) {
    while (true)
    {
        if (scanf ("%u", quantity) != 1)
        {
            printf("Invalid Quantity! Kindly enter again. ");
            while (getchar() != '\n');
            continue;
        }
        if (*quantity < MIN_QUANTITY || *quantity > MAX_QUANTITY)
        {
            printf("Quantity must be in range 0-1000000! Kindly enter again. ");
            continue;
        }
        break;
    }
}

int findProductIndex(Product *products, const unsigned int totalProducts, const unsigned int id) {
    for (unsigned int productIndex = 0; productIndex < totalProducts; productIndex++)
    {
        if ((products + productIndex)->id == id)
        {
            return productIndex;
        }
    }
    return -1;
}

void getProductDetails(Product *product, Product *products, const unsigned int totalProducts) {
    printf("\nProduct ID: ");
    while (true)
    {    
        getProductID(&product->id);

        if (findProductIndex(products, totalProducts, product->id) != -1)
        {
            printf("ID already exists. Kindly enter a unique ID: ");
        }
        else 
        {
            break;
        }
    }

    printf("Product Name: ");
    getProductName(product->name);

    printf("Product Price: ");
    getProductPrice(&product->price);

    printf("Product Quantity: ");
    getProductQuantity(&product->quantity);
}

Product* getInitialProductsDetails(const unsigned int totalProducts) {
    Product *initialProducts = (Product *) calloc (totalProducts, sizeof(Product));

    if (initialProducts == NULL) 
    {
        printf("Error Allocating Memory");
        return NULL;
    }

    for (int productIndex = 0; productIndex < totalProducts; productIndex++)
    {
        printf("\nEnter details for product %d: ", productIndex + 1);
        getProductDetails(initialProducts + productIndex, initialProducts, productIndex);
    }

    return initialProducts;
}

bool addNewProduct(Product **products, unsigned int *totalProducts) {
    Product *temporary = realloc (*products, (*totalProducts + 1) * sizeof(Product));
    if (temporary == NULL) 
    {
        return false;
    }
    *products = temporary;

    printf("\nEnter new product details: ");
    getProductDetails(*products + *totalProducts, *products, *totalProducts);
    printf("Product added successfully!\n");
    (*totalProducts)++;

    return true;
}

void viewProduct(Product *product) {
    printf("Product ID: %3u | Product Name: %5s | Price: %3.2f | Quantity: %3u\n",
                product->id, product->name, product->price, product->quantity);
}

void viewProducts(Product *products, const unsigned int totalProducts){
    if (totalProducts == 0)
    {
        printf("No Products Exist. Add new product first.\n");
        return;
    }
    
    printf("\n=========PRODUCT LIST=========\n");
    for (unsigned int productIndex = 0; productIndex < totalProducts; productIndex++)
    {
        viewProduct(products + productIndex);
    }
}

void updateProductQuantity(Product *products, const unsigned int totalProducts){
    unsigned int currentId;
    
    printf("\nEnter Product ID to update quantity: ");
    getProductID(&currentId);

    int productIndex = findProductIndex(products, totalProducts, currentId);
    if (productIndex != -1)
    {
        printf("Enter new Quantity: ");
        getProductQuantity(&(*(products + productIndex)).quantity);
        printf("Quantity updated successfully!\n");
    } else
    {
        printf("The entered Product ID doesn't exist.\n");
    }
}

int searchProductById(Product *products, const unsigned int totalProducts) {
    unsigned int currentId;

    printf("\nEnter Product ID to search: ");
    getProductID(&currentId);
    
    int productIndex = findProductIndex(products, totalProducts, currentId);
    if (productIndex != -1)
    {
        return productIndex;
    } 

    return -1;
}

bool findSubstring(char *string, char *subString) {
    if (*subString == '\0') 
    {
        return true;
    }

    while (*string != '\0') 
    {
        const char *currentString = string;
        const char *currentSubString = subString;

        while (*currentString != '\0' && *currentSubString != '\0' && 
               tolower((unsigned char) *currentString) == tolower((unsigned char) *currentSubString))
        {
            currentString++;
            currentSubString++;
        }

        if (*currentSubString == '\0')
        {
            return true;
        }

        string++;
    }
    return false;
}
 
int* searchProductByName(Product *products, const unsigned int totalProducts, int *foundCount) {
    char currentName[NAME_SIZE];
    printf("\nEnter name to search (partial allowed): ");
    getProductName(currentName);

    int* foundProductsIndex = malloc (totalProducts * sizeof(int));
    if (foundProductsIndex == NULL)
    {
        *foundCount = -1;
        return NULL;
    }

    *foundCount = 0;

    for (unsigned int productIndex = 0; productIndex < totalProducts; productIndex++)
    {
        if (findSubstring((products + productIndex)->name, currentName))
        {
            *(foundProductsIndex + *foundCount) = productIndex;
            (*foundCount)++;
        }
    }

    if (*foundCount == 0)
    {
        printf("Product Not found.\n");
        free (foundProductsIndex);
        return NULL;
    }

    return foundProductsIndex;
}

int* searchProductByPriceRange(Product *products, const unsigned int totalProducts, int *foundCount) {
    float minPrice, maxPrice;

    printf("\nEnter minimum price: ");
    getProductPrice(&minPrice);
    printf("Enter maximum price: ");
    getProductPrice(&maxPrice);

    if (maxPrice < minPrice)
    {
        printf("Error: Maximum Price Range must be greater than Minimum Price Range.\n");
        *foundCount = 0;
        return NULL;
    }

    int* foundProductsIndex = malloc (totalProducts * sizeof(int));
    if (foundProductsIndex == NULL)
    {
        *foundCount = -1;
        return NULL;
    }

    *foundCount = 0;

    for (unsigned int productIndex = 0; productIndex < totalProducts; productIndex++)
    {
        if ((products + productIndex)->price >= minPrice && (products + productIndex)->price <= maxPrice)
        {
            *(foundProductsIndex + *foundCount) = productIndex;
            (*foundCount)++;
        }
    }

    if (*foundCount == 0)
    {
        printf("No Product in price range!\n");
        free (foundProductsIndex);
        return NULL;
    }

    return foundProductsIndex;
}

Product* deleteProduct(Product *products, unsigned int *totalProducts) {
    unsigned int currentId;
    printf("\nEnter Product ID to delete: ");
    getProductID(&currentId);

    int productIndex = findProductIndex(products, *totalProducts, currentId);
    if (productIndex != -1)
    {
        for (unsigned int shiftIndex = productIndex; shiftIndex < *totalProducts - 1; shiftIndex++)
        {
            *(products + shiftIndex) = *(products + (shiftIndex + 1));
        }
            (*totalProducts)--;
        printf("Product Deleted Successfully.\n");
    } else
    {
        printf("Product Not Found.\n");
        return products;
    }

    if (*totalProducts == 0)
    {
        free (products);
        products = NULL;
        return NULL;
    } 

    //Attempting to shrink the memory 
    Product *temporary = realloc (products, (*totalProducts) * sizeof(Product));
    if (temporary == NULL)
    {
        printf("Warning: Memory reallocation failed. Array size not reduced, but deletion successful.\n");
        return products;
    }

    return temporary;  
}

int main () {
    unsigned int totalProducts;
    getInitialNumberOfProducts(&totalProducts);

    Product *products = getInitialProductsDetails(totalProducts);
    if (products == NULL)
    {
        printf("Error Allocating Memory.");
        return 1;
    }

    unsigned int choice;
    while (true)
    {
        printf("\n========= INVENTORY MENU =========\n");
        printf("1. Add New Product\n"
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
                if (!addNewProduct(&products, &totalProducts))
                {
                    printf("Memory allocation failed. Please try again.\n");
                }
                break;
            
            case 2:
                viewProducts(products, totalProducts);
                break;
            
            case 3:
                updateProductQuantity(products, totalProducts);
                break;

            case 4: {
                int productIndex = searchProductById(products, totalProducts);
                if (productIndex == -1) 
                {
                    printf("Product Not Found!\n");
                }
                else 
                {    
                    printf("Product Found: ");
                    viewProduct(products + productIndex);
                }
                break;
            }

            case 5: {
                int foundCount;
                int *foundProductsIndex = searchProductByName(products, totalProducts, &foundCount);

                if (foundProductsIndex == NULL && foundCount == -1)
                {
                    printf("Memory allocation failed.\n");
                    return 1;
                }
                else if (foundProductsIndex != NULL)
                {
                    printf("\nProducts Found:\n");
                    for (int productIndex = 0; productIndex < foundCount; productIndex++) {
                        viewProduct(products + *(foundProductsIndex + productIndex));
                    }
                    free (foundProductsIndex);
                }

                break;
            }

            case 6: {
                int foundCount = 0;
                int *foundProductsIndex = searchProductByPriceRange(products, totalProducts, &foundCount);

                if (foundProductsIndex == NULL && foundCount == -1)
                {
                    printf("Memory allocation failed.\n");
                    return 1;
                }
                else if (foundProductsIndex != NULL)
                {
                    printf("\nProducts in price range: \n");
                    for (int productIndex = 0; productIndex < foundCount; productIndex++) {
                        viewProduct(products + *(foundProductsIndex + productIndex));
                    }
                    free (foundProductsIndex);
                }
                
                break;
            }

            case 7:
                products = deleteProduct(products, &totalProducts);
                break;

            case 8:
                free (products);
                products = NULL;
                printf("\nMemory released successfully. Exiting program...\n");
                return 0;

            default:
                printf("Invalid Choice! Please choose again.\n");
        }
    }
    return 0;
}
