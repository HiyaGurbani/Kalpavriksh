#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define NAME_SIZE 50

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
            printf ("Invalid Number! Kindly enter again: ");
            while (getchar() != '\n');
            continue;
        }
        if (*initialProducts < MIN_INITIAL_PRODUCTS || *initialProducts > MAX_INITIAL_PRODUCTS)
        {
            printf ("Initial Number of products must be in range 1-100! Kindly Enter again: ");
            continue;
        }
        break;
    }
}

void getProductID (unsigned int *id) {
    while (true) {      
        if (scanf ("%u", id) != 1)
        {
            printf ("Invalid ID! Kindly enter again: ");
            while (getchar() != '\n');
            continue;
        }
        if (*id < MIN_ID || *id > MAX_ID)
        {
            printf ("ID must be in range 1-10000! Kindly enter again: ");
            continue;
        }
        break;
    }
}

int nameLength(char *name) {
    int length = 0;
    while (name[length] != '\0')
    {
        length++;
    }
    return length;
}

void getProductName (char *name) {
    int character;
    while ((character = getchar()) != '\n' && character != EOF);

    while (true)
    {   
        if (fgets(name, NAME_SIZE, stdin) == NULL) {
            printf("Error reading input. Try again.\n");
            continue;
        }
        name[strcspn(name, "\n")] = '\0';

        if (nameLength(name) == 0)
        {
            printf("Name cannot be empty. Kindly enter again: ");
            continue;
        }

        if (nameLength(name) >= NAME_SIZE - 1)
        {
            int character;
            while ((character = getchar()) != '\n' && character != EOF);
            printf("Name too long! Only first %d characters will be considered.\n", NAME_SIZE );      
        }

        break;
    }
}

void getProductPrice (float *price) {
    while (true) 
    {
        if (scanf ("%f", price) != 1)
        {
            printf ("Invalid Price! Kindly enter again. ");
            while (getchar() != '\n');
            continue;
        }
        if (*price < MIN_PRICE || *price > MAX_PRICE)
        {
            printf ("Price must be in range 0-100000! Kindly enter again. ");
            continue;
        }
        break;
    }
}

void getProductQuantity (unsigned int *quantity) {
    while (true)
    {
        if (scanf ("%u", quantity) != 1)
        {
            printf ("Invalid Quantity! Kindly enter again. ");
            while (getchar() != '\n');
            continue;
        }
        if (*quantity < MIN_QUANTITY || *quantity > MAX_QUANTITY)
        {
            printf ("Quantity must be in range 0-1000000! Kindly enter again. ");
            continue;
        }
        break;
    }
}

int findProductIndex (Product *products, const unsigned int totalProducts, const unsigned int id) {
    for (unsigned int productIndex = 0; productIndex < totalProducts; productIndex++)
    {
        if (products[productIndex].id == id)
        {
            return productIndex;
        }
    }
    return -1;
}

void getProductDetails (Product *product, Product *products, const unsigned int totalProducts) {
    printf ("\nProduct ID: ");
    while (true)
    {    
        getProductID(&product->id);

        if (findProductIndex(products, totalProducts, product->id) != -1)
        {
            printf ("ID already exists. Kindly enter a unique ID: ");
        }
        else 
        {
            break;
        }
    }

    printf ("Product Name: ");
    getProductName(product->name);

    printf ("Product Price: ");
    getProductPrice(&product->price);

    printf ("Product Quantity: ");
    getProductQuantity(&product->quantity);
}

Product* getProductsDetails(const unsigned int totalProducts) {
    Product *initialProducts = (Product *) calloc (totalProducts, sizeof(Product));

    if (initialProducts == NULL) 
    {
        printf("Error Allocating Memory");
        return NULL;
    }

    for (int productIndex = 0; productIndex < totalProducts; productIndex++)
    {
        printf ("\nEnter details for product %d: ", productIndex + 1);
        getProductDetails(&initialProducts[productIndex], initialProducts, productIndex);
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
    getProductDetails(&(*products)[*totalProducts], *products, *totalProducts);
    printf("Product added successfully!\n");
    (*totalProducts)++;

    return true;
}

void viewProduct (Product *product) {
    printf("Product ID: %3u | Product Name: %5s | Price: %3.2f | Quantity: %3u\n",
                product->id, product->name, product->price, product->quantity);
}

void viewProducts (Product *products, const unsigned int totalProducts){
    if (totalProducts == 0)
    {
        printf ("No Products Exist. Add new product first.");
        return;
    }
    
    printf("\n=========PRODUCT LIST=========\n");
    for (unsigned int productIndex = 0; productIndex < totalProducts; productIndex++)
    {
        viewProduct(&products[productIndex]);
    }
}

void updateQuantity (Product *products, const unsigned int totalProducts){
    unsigned int currentId;
    
    printf ("\nEnter Product ID to update quantity: ");
    getProductID(&currentId);

    int productIndex = findProductIndex(products, totalProducts, currentId);
    if (productIndex != -1)
    {
        printf ("Enter new Quantity: ");
        getProductQuantity(&products[productIndex].quantity);
        printf("Quantity updated successfully!\n");
    } else
    {
        printf ("The entered Product ID doesn't exist.\n");
    }
}

void searchProductById (Product *products, const unsigned int totalProducts) {
    unsigned int currentId;

    printf ("\nEnter Product ID to search: ");
    getProductID(&currentId);
    
    int productIndex = findProductIndex(products, totalProducts, currentId);
    if (productIndex != -1)
    {
        printf("Product Found: ");
        viewProduct(&products[productIndex]);
    } else
    {
        printf ("Product Not Found.\n");
    }
}

bool compareName (char *fullName, char *prefix) {
    while (*prefix != '\0')
    {   
        if (tolower((unsigned char)*fullName) != tolower((unsigned char)*prefix))
        {
            return false;
        } 
        prefix++;
        fullName++;
    }
    return true;
}
 
void searchProductByName (Product *products, const unsigned int totalProducts) {
    char currentName[NAME_SIZE];
    printf ("\nEnter name to search (partial allowed): ");
    getProductName(currentName);

    bool isNameExists = false;

    for (unsigned int productIndex = 0; productIndex < totalProducts; productIndex++)
    {
        if (compareName(products[productIndex].name, currentName))
        {
            if (!isNameExists)
            {
                printf("\nProducts Found: \n");
                isNameExists = true;
            }
            viewProduct(&products[productIndex]);
        }
    }

    if (!isNameExists)
    {
        printf ("Product Not Found.\n");
    }
}

void searchProductByRange (Product *products, const unsigned int totalProducts) {
    float minPrice, maxPrice;
    bool isProductExists = false;

    printf ("\nEnter minimum price: ");
    getProductPrice(&minPrice);
    printf("Enter maximum price: ");
    getProductPrice(&maxPrice);

    if (maxPrice < minPrice)
    {
        printf ("Error: Maximum Price Range must be greater than Minimum Price Range.\n");
        return;
    }

    for (unsigned int productIndex = 0; productIndex < totalProducts; productIndex++)
    {
        if (products[productIndex].price >= minPrice && products[productIndex].price <= maxPrice)
        {
            if (!isProductExists)
            {
                printf("\nProducts in price range: \n");
                isProductExists = true;
            }
            viewProduct(&products[productIndex]);
        }
    }

    if (!isProductExists)
    {
        printf ("No Product in Price Range.\n");
    }
}

Product* deleteProduct(Product *products, unsigned int *totalProducts) {
    unsigned int currentId;
    printf ("\nEnter Product ID to delete: ");
    getProductID(&currentId);

    int productIndex = findProductIndex(products, *totalProducts, currentId);
    if (productIndex != -1)
    {
        for (unsigned int shiftIndex = productIndex; shiftIndex < *totalProducts - 1; shiftIndex++)
        {
            products[shiftIndex] = products[shiftIndex + 1];
        }
            (*totalProducts)--;
        printf ("Product Deleted Successfully.\n");
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

    Product *products = getProductsDetails(totalProducts);
    if (products == NULL)
    {
        printf("Error Allocating Memory.");
        return 1;
    }

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
                if (!addNewProduct(&products, &totalProducts))
                {
                    printf("Memory allocation failed. Please try again.\n");
                }
                break;
            
            case 2:
                viewProducts(products, totalProducts);
                break;
            
            case 3:
                updateQuantity(products, totalProducts);
                break;

            case 4:
                searchProductById(products, totalProducts);
                break;

            case 5:
                searchProductByName(products, totalProducts);
                break;

            case 6:
                searchProductByRange(products, totalProducts);
                break;

            case 7:
                products = deleteProduct(products, &totalProducts);
                break;

            case 8:
                free (products);
                products = NULL;
                printf ("\nMemory released successfully. Exiting program...\n");
                return 0;

            default:
                printf("Invalid Choice! Please choose again.");
        }
    }
    return 0;
}