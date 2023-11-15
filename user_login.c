#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h> // for usleep

#define MAX_USERS 100
#define FILENAME "userdata.txt"

struct User {
    char *name;
    char *phone;
    char *email;
    char *username;
    char *password;
};

struct Database {
    struct User *users[MAX_USERS];
    int count;
};

// Function prototypes
void signUp(struct Database *db);
void login(struct Database *db);
void displayUserDetails(const struct User *user);
void freeUser(struct User *user);
void freeDatabase(struct Database *db);
void saveDataToFile(struct Database *db);
void loadDataFromFile(struct Database *db);

int main() {
    struct Database db = { .count = 0 };

    // Load user data from file
    loadDataFromFile(&db);

    int choice;

    do {
        printf("\nUser Authentication Interface\n");
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
    saveDataToFile(&db);

    freeDatabase(&db);
    return 0;
}

// Function to sign up a new user
void signUp(struct Database *db) {
    if (db->count == MAX_USERS) {
        printf("User database is full. Cannot sign up new users.\n");
        return;
    }

    struct User *newUser = (struct User *)malloc(sizeof(struct User));
    if (!newUser) {
        printf("Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    printf("Enter your name: ");
    newUser->name = (char *)malloc(50 * sizeof(char));
    scanf(" %49[^\n]", newUser->name);

    printf("Enter your phone number: ");
    newUser->phone = (char *)malloc(15 * sizeof(char));
    scanf(" %14[^\n]", newUser->phone);

    printf("Enter your email: ");
    newUser->email = (char *)malloc(50 * sizeof(char));
    scanf(" %49[^\n]", newUser->email);

    printf("Enter your desired username: ");
    newUser->username = (char *)malloc(20 * sizeof(char));
    scanf(" %19[^\n]", newUser->username);

    printf("Enter your password: ");
    newUser->password = (char *)malloc(20 * sizeof(char));
    scanf(" %19[^\n]", newUser->password);

    // TODO: Implement password hashing (e.g., using a library like bcrypt)

    db->users[db->count++] = newUser;

    printf("Sign up successful!\n\n");
}

// Function to log in an existing user
void login(struct Database *db) {
    char inputUsername[20];
    char inputPassword[20];

    printf("Enter your username: ");
    scanf("%s", inputUsername);

    printf("Enter your password: ");
    scanf("%s", inputPassword);

    // TODO: Implement password verification using hashed passwords

    for (int i = 0; i < db->count; ++i) {
        if (strcmp(db->users[i]->username, inputUsername) == 0 &&
            strcmp(db->users[i]->password, inputPassword) == 0) {
            // Display user details (excluding password)
            printf("\nLogin successful!\n");
            displayUserDetails(db->users[i]);
            return;
        }
    }

    printf("Login failed. Invalid username or password.\n\n");
}

// Function to display user details
void displayUserDetails(const struct User *user) {
    printf("\nUser details:\n");
    printf("Name: %s\n", user->name);
    printf("Phone: %s\n", user->phone);
    printf("Email: %s\n", user->email);
    printf("Username: %s\n", user->username);

    // Simulate a delay for a more realistic experience
    usleep(1000000);

    printf("\nRedirecting to main menu...\n");
}

// Function to save user data to a file
void saveDataToFile(struct Database *db) {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < db->count; ++i) {
        fprintf(file, "%s|%s|%s|%s|%s\n",
            db->users[i]->name, db->users[i]->phone, db->users[i]->email,
            db->users[i]->username, db->users[i]->password);
    }

    fclose(file);
}

// Function to load user data from a file
void loadDataFromFile(struct Database *db) {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("No existing user data file found.\n");
        return;
    }

    char name[50], phone[15], email[50], username[20], password[20];
    while (fscanf(file, "%49[^|]|%14[^|]|%49[^|]|%19[^|]|%19[^\n]",
           name, phone, email, username, password) != EOF) {
        if (db->count < MAX_USERS) {
            struct User *loadedUser = (struct User *)malloc(sizeof(struct User));
            if (!loadedUser) {
                printf("Memory allocation error while loading user data.\n");
                exit(EXIT_FAILURE);
            }

            loadedUser->name = strdup(name);
            loadedUser->phone = strdup(phone);
            loadedUser->email = strdup(email);
            loadedUser->username = strdup(username);
            loadedUser->password = strdup(password);

            db->users[db->count++] = loadedUser;
        } else {
            printf("User database is full. Some data may not be loaded.\n");
            break;
        }
    }

    fclose(file);
}

// Function to free memory allocated for a user
void freeUser(struct User *user) {
    free(user->name);
    free(user->phone);
    free(user->email);
    free(user->username);
    free(user->password);
    free(user);
}

// Function to free memory allocated for the entire user database
void freeDatabase(struct Database *db) {
    for (int i = 0; i < db->count; ++i) {
        freeUser(db->users[i]);
    }
}
