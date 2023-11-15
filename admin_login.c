#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h> // for usleep

#define MAX_ADMINS 100
#define FILENAME "admindata.txt"

struct Admin {
    char *name;
    char *phone;
    char *email;
    char *username;
    char *password;
    char *employeeID; // New field for employee ID
};

struct AdminDatabase {
    struct Admin *admins[MAX_ADMINS];
    int count;
};

// Function prototypes
void adminSignUp(struct AdminDatabase *adb);
void adminLogin(struct AdminDatabase *adb);
void displayAdminDetails(const struct Admin *admin);
void freeAdmin(struct Admin *admin);
void freeAdminDatabase(struct AdminDatabase *adb);
void saveDataToFile(struct AdminDatabase *adb);
void loadDataFromFile(struct AdminDatabase *adb);

int main() {
    struct AdminDatabase adb = {.count = 0};

    // Load admin data from file
    loadDataFromFile(&adb);

    int choice;

    do {
        printf("\nAdmin Authentication Interface\n");
        printf("1. Admin Sign Up\n");
        printf("2. Admin Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                adminSignUp(&adb);
                break;
            case 2:
                adminLogin(&adb);
                break;
            case 3:
                printf("Exiting the program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 3);

    // Save admin data to file before exiting
    saveDataToFile(&adb);

    freeAdminDatabase(&adb);
    return 0;
}

// Function to sign up a new admin
void adminSignUp(struct AdminDatabase *adb) {
    if (adb->count == MAX_ADMINS) {
        printf("Admin database is full. Cannot sign up new admins.\n");
        return;
    }

    struct Admin *newAdmin = (struct Admin *)malloc(sizeof(struct Admin));
    if (!newAdmin) {
        printf("Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    printf("Enter your name: ");
    newAdmin->name = (char *)malloc(50 * sizeof(char));
    scanf(" %49[^\n]", newAdmin->name);

    printf("Enter your phone number: ");
    newAdmin->phone = (char *)malloc(15 * sizeof(char));
    scanf(" %14[^\n]", newAdmin->phone);

    printf("Enter your email: ");
    newAdmin->email = (char *)malloc(50 * sizeof(char));
    scanf(" %49[^\n]", newAdmin->email);

    printf("Enter your desired username: ");
    newAdmin->username = (char *)malloc(20 * sizeof(char));
    scanf(" %19[^\n]", newAdmin->username);

    printf("Enter your password: ");
    newAdmin->password = (char *)malloc(20 * sizeof(char));
    scanf(" %19[^\n]", newAdmin->password);

    printf("Enter your unique employee ID: ");
    newAdmin->employeeID = (char *)malloc(20 * sizeof(char));
    scanf(" %19[^\n]", newAdmin->employeeID);

    // TODO: Implement password hashing (e.g., using a library like bcrypt)

    adb->admins[adb->count++] = newAdmin;

    printf("Admin signup successful!\n\n");
}

// Function to log in an existing admin
void adminLogin(struct AdminDatabase *adb) {
    char inputUsername[20];
    char inputPassword[20];
    char inputEmployeeID[20];

    printf("Enter your username: ");
    scanf("%s", inputUsername);

    printf("Enter your password: ");
    scanf("%s", inputPassword);

    printf("Enter your employee ID: ");
    scanf("%s", inputEmployeeID);

    // TODO: Implement password and employee ID verification using hashed passwords

    for (int i = 0; i < adb->count; ++i) {
        if (strcmp(adb->admins[i]->username, inputUsername) == 0 &&
            strcmp(adb->admins[i]->password, inputPassword) == 0 &&
            strcmp(adb->admins[i]->employeeID, inputEmployeeID) == 0) {
            // Display admin details (excluding password)
            printf("\nLogin successful!\n");
            displayAdminDetails(adb->admins[i]);
            return;
        }
    }

    printf("Login failed. Invalid username, password, or employee ID.\n\n");
}

// Function to display admin details
void displayAdminDetails(const struct Admin *admin) {
    printf("\nAdmin details:\n");
    printf("Name: %s\n", admin->name);
    printf("Phone: %s\n", admin->phone);
    printf("Email: %s\n", admin->email);
    printf("Username: %s\n", admin->username);

    // Simulate a delay for a more realistic experience
    usleep(1000000);

    printf("\nRedirecting to main menu...\n");
}

// Function to save admin data to a file
void saveDataToFile(struct AdminDatabase *adb) {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < adb->count; ++i) {
        fprintf(file, "%s|%s|%s|%s|%s|%s\n",
                adb->admins[i]->name, adb->admins[i]->phone, adb->admins[i]->email,
                adb->admins[i]->username, adb->admins[i]->password, adb->admins[i]->employeeID);
    }

    fclose(file);
}

// Function to load admin data from a file
void loadDataFromFile(struct AdminDatabase *adb) {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("No existing admin data file found.\n");
        return;
    }

    char name[50], phone[15], email[50], username[20], password[20], employeeID[20];
    while (fscanf(file, "%49[^|]|%14[^|]|%49[^|]|%19[^|]|%19[^|]|%19[^\n]",
                   name, phone, email, username, password, employeeID) != EOF) {
        if (adb->count < MAX_ADMINS) {
            struct Admin *loadedAdmin = (struct Admin *)malloc(sizeof(struct Admin));
            if (!loadedAdmin) {
                printf("Memory allocation error while loading admin data.\n");
                exit(EXIT_FAILURE);
            }

            loadedAdmin->name = strdup(name);
            loadedAdmin->phone = strdup(phone);
            loadedAdmin->email = strdup(email);
            loadedAdmin->username = strdup(username);
            loadedAdmin->password = strdup(password);
            loadedAdmin->employeeID = strdup(employeeID);

            adb->admins[adb->count++] = loadedAdmin;
        } else {
            printf("Admin database is full. Some data may not be loaded.\n");
            break;
        }
    }

    fclose(file);
}

// Function to free memory allocated for an admin
void freeAdmin(struct Admin *admin) {
    free(admin->name);
    free(admin->phone);
    free(admin->email);
    free(admin->username);
    free(admin->password);
    free(admin->employeeID);
    free(admin);
}

// Function to free memory allocated for the entire admin database
void freeAdminDatabase(struct AdminDatabase *adb) {
    for (int i = 0; i < adb->count; ++i) {
        freeAdmin(adb->admins[i]);
    }
}
