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

void getProductID (int *id) {
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
int lengthName(char *name) {
    int length = 0;
    while (name[length] != '\0')
    {
        length++;
    }
    return length;
}

void getProductName (char *name) {
    while (true)
    {   
        int character;
        while ((character = getchar()) != '\n' && character != EOF);

        if (fgets(name, NAME_SIZE, stdin) == NULL) {
            printf("Error reading input. Try again.\n");
            continue;
        }
        name[strcspn(name, "\n")] = '\0';

        if (lengthName(name) == 0)
        {
            printf("Name cannot be empty. Kindly enter again: ");
            continue;
        }

        if (lengthName(name) >= NAME_SIZE - 1)
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

void getProductQuantity (int *quantity) {
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

void getProductDetails (Product *product) {
    printf ("\nProduct ID: ");
    getProductID(&product->id);

    printf ("Product Name: ");
    getProductName(product->name);

    printf ("Product Price: ");
    getProductPrice(&product->price);

    printf ("Product Quantity: ");
    getProductQuantity(&product->quantity);
}

Product* getProductsDetails(const unsigned int totalProducts) {
    Product *initialProducts = (Product *) malloc (totalProducts * sizeof(Product));

    if (initialProducts == NULL) 
    {
        printf("Error Allocating Memory");
        return NULL;
    }

    for (int index = 0; index < totalProducts; index++)
    {
        printf ("\nEnter details for product %d: ", index + 1);
        getProductDetails(&initialProducts[index]);
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
    (*totalProducts)++;

    printf("\nEnter new product details: ");
    getProductDetails(products[*totalProducts-1]);
    printf("Product added successfully!\n");

    return true;
}

void viewProduct (Product *product) {
    printf("Product ID: %u | Product Name: %s | Price: %.2f | Quantity: %u\n",
                product->id, product->name, product->price, product->quantity);
}

void viewProducts (Product *products, const unsigned int totalProducts){
    if (totalProducts == 0)
    {
        printf ("No Products Exist. Add new product first.");
    }
    printf("\n=========PRODUCT LIST=========\n");
    for (unsigned int index = 0; index < totalProducts; index++)
    {
        viewProduct(&products[index]);
    }
}

void updateQuantity (Product *products, const unsigned int totalProducts){
    unsigned int currentId;
    bool isIdExists = false;
    printf ("\nEnter Product ID to update quantity: ");
    getProductID(&currentId);

    for (unsigned int index = 0; index < totalProducts; index++)
    {
        if (products[index].id == currentId)
        {
            printf ("Enter new Quantity: ");
            getProductQuantity(&products[index].quantity);
            printf("Quantity updated successfully!\n");
            isIdExists = true;
        }
    }

    if (!isIdExists)
    {
        printf ("The entered Product ID doesn't exist.\n");
    }
}

void searchProductById (Product *products, const unsigned int totalProducts) {
    unsigned int currentId;
    bool isIdExists = false;
    printf ("\nEnter Product ID to search: ");
    getProductID(&currentId);
    
    for (unsigned int index = 0; index < totalProducts; index++)
    {
        if (products[index].id == currentId)
        {
            printf("Product Found: ");
            viewProduct(&products[index]);
            isIdExists = true;
        }
    }

    if (!isIdExists)
    {
        printf ("Product Not Found.\n");
    }
}

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
 
void searchProductByName (Product *products, const unsigned int totalProducts) {
    char currentName[NAME_SIZE];
    printf ("\nEnter name to search (partial allowed): ");
    getProductName(currentName);

    bool isNameExists = false;

    for (int index = 0; index < totalProducts; index++)
    {
        if (compareName(products[index].name, currentName))
        {
            if (!isNameExists)
            {
                printf("\nProducts Found: \n");
                isNameExists = true;
            }
            viewProduct(&products[index]);
        }
    }

    if (!isNameExists)
    {
        printf ("Product Not Found.\n");
    }
}

void searchProductByRange (Product *products, const unsigned int totalProducts) {
    unsigned int minPrice, maxPrice;
    bool isProductExists = false;

    printf ("\nEnter minimun price: ");
    getProductPrice(&minPrice);
    printf("Enter maximum price: ");
    getProductPrice(&maxPrice);

    if (maxPrice < minPrice)
    {
        printf ("Error: Minimum Price Range must be greater than Maximum Price Range.");
        return;
    }

    for (unsigned int index = 0; index < totalProducts; index++)
    {
        if (products[index].price >= minPrice && products[index].price <= maxPrice)
        {
            if (!isProductExists)
            {
                printf("\nProducts in price range: \n");
                isProductExists = true;
            }
            viewProduct(&products[index]);
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

    bool isIdExists = false;

    for (unsigned int index = 0; index < *totalProducts; index++)
    {
        if (products[index].id == currentId)
        {
            isIdExists = true;
            for (unsigned int shiftIndex = index; shiftIndex < *totalProducts - 1; shiftIndex)
            {
                products[shiftIndex] = products[shiftIndex + 1];
            }
            (*totalProducts)--;
            printf ("Product Deleted Successfully.\n");
            break;
        }
    }

    if (!isIdExists)
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
                printf ("Memory released successfully. Exiting program...");
                break;

            default:
                printf("Invalid Choice! Please choose again.");
        }
    }
}