#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_USERS 100
#define MAX_ADMINS 100
#define FILENAME_USER "userdata.txt"
#define FILENAME_ADMIN "admindata.txt"

struct Person {
    char *name;
    char *phone;
    char *email;
    char *username;
    char *password;
};

struct User {
    struct Person person;
};

struct Admin {
    struct Person person;
    char *employeeID;
};

struct Database {
    union {
        struct User *users[MAX_USERS];
        struct Admin *admins[MAX_ADMINS];
    };
    int count;
};

// Function prototypes
void signUp(struct Database *db);
void login(struct Database *db);
void displayDetails(const struct Person *person);
void freePerson(struct Person *person);
void freeDatabase(struct Database *db);
void saveDataToFile(struct Database *db, const char *filename);
void loadDataFromFile(struct Database *db, const char *filename);

int main() {
    struct Database db = { .count = 0 };

    // Load user data from file
    loadDataFromFile(&db, FILENAME_USER);
    // Load admin data from file
    loadDataFromFile(&db, FILENAME_ADMIN);

    int choice;

    do {
        printf("\nAuthentication Interface\n");
        printf("1. Sign Up\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                signUp(&db);
                break;
            case 2:
                login(&db);
                break;
            case 3:
                printf("Exiting the program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 3);

    // Save user data to file before exiting
    saveDataToFile(&db, FILENAME_USER);
    // Save admin data to file before exiting
    saveDataToFile(&db, FILENAME_ADMIN);

    freeDatabase(&db);
    return 0;
}

// Function to sign up a new user or admin
// Function to sign up a new user or admin
void signUp(struct Database *db) {
    if (db->count == MAX_USERS + MAX_ADMINS) {
        printf("Database is full. Cannot sign up new users or admins.\n");
        return;
    }

    int userType;
    printf("Choose user type:\n");
    printf("1. User\n");
    printf("2. Admin\n");
    printf("Enter your choice: ");
    scanf("%d", &userType);

    if (userType != 1 && userType != 2) {
        printf("Invalid choice. Please try again.\n");
        return;
    }

    char inputUsername[20];
    char inputPassword[20];

    printf("Enter your desired username: ");
    scanf(" %19[^\n]", inputUsername);

    // Check if the username already exists
    for (int i = 0; i < db->count; ++i) {
        if ((userType == 1 && strcmp(db->users[i]->person.username, inputUsername) == 0) ||
            (userType == 2 && strcmp(db->admins[i]->person.username, inputUsername) == 0)) {
            printf("Username already exists. Please try again with a different username.\n");
            return;
        }
    }

    printf("Enter your desired password: ");
    scanf(" %19[^\n]", inputPassword);

    // Check if the password already exists
    for (int i = 0; i < db->count; ++i) {
        if ((userType == 1 && strcmp(db->users[i]->person.password, inputPassword) == 0) ||
            (userType == 2 && strcmp(db->admins[i]->person.password, inputPassword) == 0)) {
            printf("Password already exists. Please try again with a different password.\n");
            return;
        }
    }

    if (userType == 1 && db->count < MAX_USERS) {
        struct User *newUser = (struct User *)malloc(sizeof(struct User));
        if (!newUser) {
            printf("Memory allocation error.\n");
            exit(EXIT_FAILURE);
        }

        printf("Enter your name: ");
        newUser->person.name = (char *)malloc(50 * sizeof(char));
        scanf(" %49[^\n]", newUser->person.name);

        printf("Enter your phone number: ");
        newUser->person.phone = (char *)malloc(15 * sizeof(char));
        scanf(" %14[^\n]", newUser->person.phone);

        printf("Enter your email: ");
        newUser->person.email = (char *)malloc(50 * sizeof(char));
        scanf(" %49[^\n]", newUser->person.email);

        newUser->person.username = strdup(inputUsername);
        newUser->person.password = strdup(inputPassword);

        db->users[db->count++] = newUser;

        printf("Sign up successful!\n\n");
    } else if (userType == 2 && db->count < MAX_ADMINS) {
        struct Admin *newAdmin = (struct Admin *)malloc(sizeof(struct Admin));
        if (!newAdmin) {
            printf("Memory allocation error.\n");
            exit(EXIT_FAILURE);
        }

        printf("Enter your name: ");
        newAdmin->person.name = (char *)malloc(50 * sizeof(char));
        scanf(" %49[^\n]", newAdmin->person.name);

        printf("Enter your phone number: ");
        newAdmin->person.phone = (char *)malloc(15 * sizeof(char));
        scanf(" %14[^\n]", newAdmin->person.phone);

        printf("Enter your email: ");
        newAdmin->person.email = (char *)malloc(50 * sizeof(char));
        scanf(" %49[^\n]", newAdmin->person.email);

        newAdmin->person.username = strdup(inputUsername);
        newAdmin->person.password = strdup(inputPassword);

        printf("Enter your unique employee ID: ");
        newAdmin->employeeID = (char *)malloc(20 * sizeof(char));
        scanf(" %19[^\n]", newAdmin->employeeID);

        db->admins[db->count++] = newAdmin;

        printf("Admin signup successful!\n\n");
    } else {
        printf("Database is full. Cannot sign up new users or admins.\n");
    }
}

// Function to log in an existing user or admin
void login(struct Database *db) {
    int userType;
    printf("Choose user type:\n");
    printf("1. User\n");
    printf("2. Admin\n");
    printf("Enter your choice: ");
    scanf("%d", &userType);

    if (userType != 1 && userType != 2) {
        printf("Invalid choice. Please try again.\n");
        return;
    }

    char inputUsername[20];
    char inputPassword[20];
    char inputEmployeeID[20];

    printf("Enter your username: ");
    scanf("%s", inputUsername);

    printf("Enter your password: ");
    scanf("%s", inputPassword);

    // Check if the username and password match
    for (int i = 0; i < db->count; ++i) {
        if (userType == 1 && strcmp(db->users[i]->person.username, inputUsername) == 0 &&
            strcmp(db->users[i]->person.password, inputPassword) == 0) {
            // Display user details (excluding password)
            printf("\nLogin successful!\n");
            displayDetails(&db->users[i]->person);
            return;
        } else if (userType == 2 && strcmp(db->admins[i]->person.username, inputUsername) == 0 &&
                   strcmp(db->admins[i]->person.password, inputPassword) == 0) {
            // Display admin details (excluding password)
            printf("\nLogin successful!\n");
            displayDetails(&db->admins[i]->person);
            return;
        }
    }

    printf("Login failed. Invalid username or password.\n\n");
}


// Function to free memory allocated for a person
void freePerson(struct Person *person) {
    free(person->name);
    free(person->phone);
    free(person->email);
    free(person->username);
    free(person->password);
}

// Function to free memory allocated for the entire database
void freeDatabase(struct Database *db) {
    for (int i = 0; i < db->count; ++i) {
        if (i < MAX_USERS) {
            freePerson(&db->users[i]->person);
            free(db->users[i]);
        } else {
            freePerson(&db->admins[i - MAX_USERS]->person);
            free(db->admins[i - MAX_USERS]->employeeID);
            free(db->admins[i - MAX_USERS]);
        }
    }
}

// Function to save user or admin data to a file
void saveDataToFile(struct Database *db, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < db->count; ++i) {
        if (i < MAX_USERS) {
            fprintf(file, "%s|%s|%s|%s|%s\n",
                    db->users[i]->person.name, db->users[i]->person.phone, db->users[i]->person.email,
                    db->users[i]->person.username, db->users[i]->person.password);
        } else {
            fprintf(file, "%s|%s|%s|%s|%s|%s\n",
                    db->admins[i - MAX_USERS]->person.name, db->admins[i - MAX_USERS]->person.phone,
                    db->admins[i - MAX_USERS]->person.email, db->admins[i - MAX_USERS]->person.username,
                    db->admins[i - MAX_USERS]->person.password, db->admins[i - MAX_USERS]->employeeID);
        }
    }

    fclose(file);
}

// Function to load user or admin data from a file
void loadDataFromFile(struct Database *db, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No existing data file found for %s.\n", filename);
        return;
    }

    char name[50], phone[15], email[50], username[20], password[20], employeeID[20];
    int userType;

    while (fscanf(file, "%49[^|]|%14[^|]|%49[^|]|%19[^|]|%19[^|]|%19[^\n]",
                   name, phone, email, username, password, employeeID) != EOF) {
        if (db->count < MAX_USERS + MAX_ADMINS) {
            if (fscanf(file, "%d", &userType) == 1) {
                if (userType == 1) {
                    struct User *loadedUser = (struct User *)malloc(sizeof(struct User));
                    if (!loadedUser) {
                        printf("Memory allocation error while loading user data.\n");
                        exit(EXIT_FAILURE);
                    }

                    loadedUser->person.name = strdup(name);
                    loadedUser->person.phone = strdup(phone);
                    loadedUser->person.email = strdup(email);
                    loadedUser->person.username = strdup(username);
                    loadedUser->person.password = strdup(password);

                    db->users[db->count++] = loadedUser;
                } else if (userType == 2) {
                    struct Admin *loadedAdmin = (struct Admin *)malloc(sizeof(struct Admin));
                    if (!loadedAdmin) {
                        printf("Memory allocation error while loading admin data.\n");
                        exit(EXIT_FAILURE);
                    }

                    loadedAdmin->person.name = strdup(name);
                    loadedAdmin->person.phone = strdup(phone);
                    loadedAdmin->person.email = strdup(email);
                    loadedAdmin->person.username = strdup(username);
                    loadedAdmin->person.password = strdup(password);
                    loadedAdmin->employeeID = strdup(employeeID);

                    db->admins[db->count++ - MAX_USERS] = loadedAdmin;
                } else {
                    printf("Invalid user type in file %s.\n", filename);
                }
            } else {
                printf("Error reading user type from file %s.\n", filename);
            }
        } else {
            printf("Database is full. Some data may not be loaded.\n");
            break;
        }
    }

    fclose(file);
}

// Function to display details
void displayDetails(const struct Person *person) {
    printf("\nDetails:\n");
    printf("Name: %s\n", person->name);
    printf("Phone: %s\n", person->phone);
    printf("Email: %s\n", person->email);
    printf("Username: %s\n", person->username);

    // Simulate a delay for a more realistic experience
    usleep(1000000);

    printf("\nRedirecting to main menu...\n");
}

