#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_USERS 100
#define MAX_ADMINS 100
#define MAX_FLIGHTS 50
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
    struct Booking *bookedTickets;  // Dynamic array to store booked tickets
    int bookedTicketsCount;  // Number of booked tickets
};

// Updated struct Flight definition
struct Flight {
    char flightNumber[10];
    char departureTime[20];
    char arrivalTime[20];
    char departureDate[20];  // New: Departure date
    char arrivalDate[20];    // New: Arrival date
    float ticketPriceInfant;
    float ticketPriceChild;
    float ticketPriceAdult;
    char source[50];
    char destination[50];
    int seatsAvailable;
};

struct Booking {
    struct User user;
    struct Flight flight;
    int flightIndex;
    int passengerType;
};

// Function prototypes for admin utilities
void enterFlightDetails(struct Flight flights[], int *numFlights);
void editFlightDetails(struct Flight flights[], int numFlights);
void viewAllFlightDetails(struct Flight flights[], int numFlights);

// Function prototypes
void signUp(struct Database *db);
void bookTickets(struct Database *db, struct Flight flights[], int numFlights);
void adminUtilities(struct Database *db, struct Flight flights[], int *numFlights);
void displayDetails(const struct Person *person);
void freePerson(struct Person *person);
void freeDatabase(struct Database *db);
void saveDataToFile(struct Database *db, const char *filename);
void loadDataFromFile(struct Database *db, const char *filename);
void viewBookedTickets(struct Database *db, struct Flight flights[], int numFlights);

int main() {
    struct Database db = { .count = 0 };
    struct Flight flights[MAX_FLIGHTS];
    int numFlights = 0;

    // Load user data from file
    //loadDataFromFile(&db, FILENAME_USER);
    // Load admin data from file
    //loadDataFromFile(&db, FILENAME_ADMIN);

    int choice;

    do {
        printf("\n\n\t\t ********************************************************************\n");
        printf("\n\n\t\t <----------- Sky High Flight Ticket Reservation System ------------>\n");
        printf("\n\n\t\t ********************************************************************\n");
        printf("\n\t\t 1. Sign Up\n");
        printf("\n\t\t 2. Admin Utilities\n");
        printf("\n\t\t 3. Book Tickets\n");
        printf("\n\t\t 4. View Booked Tickets\n");  // New option
        printf("\n\t\t 5. Exit\n");
        printf("\n\t\t Enter your choice: ");
        scanf("%d", &choice);
        printf("\t\t ____________________________________________________________________\n");

        switch (choice) {
            case 1:
                signUp(&db);
                // Ask the user to press any button to return to the main interface
                printf("\n\t\t Press enter key to return to the main interface...\n");
                printf("\n\t\t ___________________________________________ \n");
                getchar();  // Consume the newline character from the previous input
                getchar();  // Wait for a key press
                system("clear");
                break;
            case 2:
                adminUtilities(&db, flights, &numFlights);
                // Ask the user to press any button to return to the main interface
                printf("\n\t\t Press enter key to return to the main interface...\n");
                printf("\n\t\t ___________________________________________ \n");
                getchar();  // Consume the newline character from the previous input
                getchar();  // Wait for a key press
                system("clear");
                break;
            case 3:
                bookTickets(&db, flights, numFlights);
                system("clear");
                break;
            case 4:
                viewBookedTickets(&db, flights, numFlights);
                // Ask the user to press any button to return to the main interface
                printf("\n\t\t Press enter key to return to the main interface...\n");
                printf("\n\t\t ___________________________________________ \n");
                getchar();  // Consume the newline character from the previous input
                getchar();  // Wait for a key press
                system("clear");
                break;
            case 5:
                system("clear");
                printf("\n\t\t Exiting the program.\n");
                // Simulate a delay for a more realistic experience
                usleep(4000000);
                break;
            default:
                printf("\n\t\t Invalid choice. Please try again.\n");
        }
    } while (choice != 5);

    // Save user data to file before exiting
    //saveDataToFile(&db, FILENAME_USER);
    // Save admin data to file before exiting
    //saveDataToFile(&db, FILENAME_ADMIN);

    freeDatabase(&db);
    return 0;
}


void bookTickets(struct Database *db, struct Flight flights[], int numFlights) {
    if (db->count == 0) {
        printf("\n\t\t No registered users found. Please sign up before booking tickets.\n");
        return;
    }


    char inputUsername[20];
    char inputPassword[20];

    // Authenticating the user
    printf("\n\t\t Enter your username: ");
    scanf("%s", inputUsername);

    printf("\n\t\t Enter your password: ");
    scanf("%s", inputPassword);

    int userIndex = -1;
    for (int i = 0; i < db->count; ++i) {
        if (strcmp(db->users[i]->person.username, inputUsername) == 0 &&
            strcmp(db->users[i]->person.password, inputPassword) == 0) {
            userIndex = i;
            break;
        }
    }

    if (userIndex == -1) {
        printf("\n\t\t Login failed. Invalid username or password.\n");
        return;
    }

    // User authenticated, proceed with ticket booking
    printf("\n\t\t Enter the source: ");
    char source[50];
    scanf("%s", source);

    printf("\n\t\t Enter the destination: ");
    char destination[50];
    scanf("%s", destination);

    // Display available flights based on the entered information
    printf("\n\t\t Available Flights:\n");
    int availableFlights = 0; // Keep track of available flights
    // printf("numFlights value = %d",numFlights);
    for (int i = 0; i < numFlights; ++i) {
    if (strcmp(flights[i].source, source) == 0 &&
        strcmp(flights[i].destination, destination) == 0) {
        printf("\n\t\t %d. Flight Number: %s\n", i+1, flights[i].flightNumber);
        printf("\n\t\t    Source: %s\n", flights[i].source);
        printf("\n\t\t    Destination: %s\n", flights[i].destination);
        printf("\n\t\t    Departure Date: %s\n", flights[i].departureDate);
        printf("\n\t\t    Departure Time: %s\n", flights[i].departureTime);
        printf("\n\t\t    Arrival Date: %s\n", flights[i].arrivalDate);
        printf("\n\t\t    Arrival Time: %s\n", flights[i].arrivalTime);
        printf("\n\t\t    Ticket Price for Infant: %.2f\n", flights[i].ticketPriceInfant);
        printf("\n\t\t    Ticket Price for Child: %.2f\n", flights[i].ticketPriceChild);
        printf("\n\t\t    Ticket Price for Adult: %.2f\n", flights[i].ticketPriceAdult);
        printf("\n\t\t    Seats Available: %d\n", flights[i].seatsAvailable);
        printf("\n\t\t ------------------------------------\n");
        availableFlights++; // Increment the count of available flights
    }
}

if (availableFlights == 0) {
    printf("\n\t\t Sorry, no flights available for the specified route. Please try again later.\n");
    return;
}

    // Ask the user to select a flight
    int selectedFlight;
    do {
        printf("\n\t\t Enter the serial number of the flight you want to book: ");
        scanf("%d", &selectedFlight);

        if (selectedFlight < 1 || selectedFlight > numFlights) {
            printf("\n\t\t Invalid choice. Please enter a valid flight number.\n");
        }
    } while (selectedFlight < 1 || selectedFlight > numFlights);

    // Display selected flight details
    printf("\n\t\t Selected Flight Details:\n");
    printf("\t\t Flight Number: %s\n", flights[selectedFlight - 1].flightNumber);
    printf("\t\t Source: %s\n", flights[selectedFlight - 1].source);
    printf("\t\t Destination: %s\n", flights[selectedFlight - 1].destination);
    printf("\t\t Departure Date: %s\n", flights[selectedFlight - 1].departureDate);
    printf("\t\t Departure Time: %s\n", flights[selectedFlight - 1].departureTime);
    printf("\t\t Arrival Date: %s\n", flights[selectedFlight - 1].arrivalDate);
    printf("\t\t Arrival Time: %s\n", flights[selectedFlight - 1].arrivalTime);
    printf("\t\t Ticket Price for Infant: %.2f\n", flights[selectedFlight - 1].ticketPriceInfant);
    printf("\t\t Ticket Price for Child: %.2f\n", flights[selectedFlight - 1].ticketPriceChild);
    printf("\t\t Ticket Price for Adult: %.2f\n", flights[selectedFlight - 1].ticketPriceAdult);

    // Check for seat availability
    if (flights[selectedFlight - 1].seatsAvailable <= 0) {
        printf("\n\t\t Sorry, no seats available for the selected flight. Please try again later.\n");
        return;
    }

    // Ask the user to select passenger type
    int passengerType;
    do {
        printf("\n\t\t Select Passenger Type:\n");
        printf("\n\t\t 1. Infant (below 5 years)\n");
        printf("\n\t\t 2. Child (between 6 and 18 years)\n");
        printf("\n\t\t 3. Adult (above 18 years)\n");
        printf("\n\t\t Enter your choice: ");
        scanf("%d", &passengerType);

        if (passengerType < 1 || passengerType > 3) {
            printf("\n\t\t Invalid choice. Please enter a valid option.\n");
        }
    } while (passengerType < 1 || passengerType > 3);

    // Display ticket fare based on passenger type
    float ticketFare;
    switch (passengerType) {
        case 1:
            ticketFare = flights[selectedFlight - 1].ticketPriceInfant;
            break;
        case 2:
            ticketFare = flights[selectedFlight - 1].ticketPriceChild;
            break;
        case 3:
            ticketFare = flights[selectedFlight - 1].ticketPriceAdult;
            break;
    }

    printf("\n\t\t Ticket Fare: %.2f\n\n\t\t Please proceed to payment.\n", ticketFare);

    // Book the ticket
    printf("\n\t\t Please wait while we process your request...\n");

    printf("\n\t\t Making the reservation...\n");
    usleep(2000000); // Simulate a delay for a more realistic experience

    printf("\n\t\t Booking the ticket...\n");
    usleep(2000000);  // Simulate a delay for a more realistic experience

    // Update seat availability
    flights[selectedFlight - 1].seatsAvailable--;

    // Display booking confirmation and details
    printf("\n\t\t Ticket booked successfully!\n");

    // Store booking details in an array of structures
    //     struct Booking {
//     struct User user;
//     struct Flight flight;
//     int flightIndex;
//     int passengerType;
// };
    struct Booking booking;
    booking.user = *db->users[userIndex];  // Assume userIndex is the index of the logged-in user
    booking.flight = flights[selectedFlight - 1];
    booking.passengerType = passengerType; 

    // Increase the size of the bookedTickets array
    db->bookedTicketsCount++;
    db->bookedTickets = realloc(db->bookedTickets, db->bookedTicketsCount * sizeof(struct Booking));

// Check if the reallocation was successful
if (db->bookedTickets == NULL) {
    printf("\n\t\t Memory allocation failed. Exiting.\n");
    exit(EXIT_FAILURE);
}

// Add the new booking
db->bookedTickets[db->bookedTicketsCount - 1] = booking;
    //printf("The flight index = "+(db->bookedTickets[db->bookedTicketsCount - 1]).flightIndex);
    // Display booking details
    printf("\n\t\t ___________________________________________ \n");
    printf("\n\n\t\t Booking Details:\n");
    printf("\t\t Passenger Name: %s\n", booking.user.person.name);

    if (passengerType==1){
        printf("\t\t Passenger Type: I\n");
    }
    else if (passengerType==2){
        printf("\t\t Passenger Type: C\n");
    }
    else {
        printf("\t\t Passenger Type: A\n");
    }

    printf("\n\t\t Flight Number: %s\n", booking.flight.flightNumber);
    printf("\t\t Source: %s\n", booking.flight.source);
    printf("\t\t Destination: %s\n", booking.flight.destination);
    printf("\t\t Departure Date: %s\n", booking.flight.departureDate);
    printf("\t\t Departure Time: %s\n", booking.flight.departureTime);
    printf("\t\t Arrival Date: %s\n", booking.flight.arrivalDate);
    printf("\t\t Arrival Time: %s\n", booking.flight.arrivalTime);
    printf("\t\t Ticket Price: %.2f\n", ticketFare);

    // Ask the user to press any button to return to the main interface
    printf("\n\t\t Press enter key to return to the main interface...\n");
    printf("\n\t\t ___________________________________________ \n");
    getchar();  // Consume the newline character from the previous input
    getchar();  // Wait for a key press
}

void viewBookedTickets(struct Database *db, struct Flight flights[], int numFlights) {
    printf("\n\t\t <-------- View Booked Tickets -------->\n");

    int userType;
    printf("\n\t\t Are you a user or an admin?\n");
    printf("\t\t 1. User\n");
    printf("\t\t 2. Admin\n");
    printf("\t\t Enter your choice: ");
    scanf("%d", &userType);
    printf("\n\t\t -------------------------------------\n");

    if (userType == 1) {
        // User login to view booked tickets
        char inputUsername[20];
        char inputPassword[20];

        printf("\t\t Enter your username: ");
        scanf("%s", inputUsername);

        printf("\n\t\t Enter your password: ");
        scanf("%s", inputPassword);
        printf("\n\t\t -------------------------------------\n");

        int userIndex = -1;
        for (int i = 0; i < db->count; ++i) {
            if (strcmp(db->users[i]->person.username, inputUsername) == 0 &&
                strcmp(db->users[i]->person.password, inputPassword) == 0) {
                userIndex = i;
                break;
            }
        }

        if (userIndex == -1) {
            printf("\n\t\t Login failed. Invalid username or password.\n");
            return;
        }

        // Display booked tickets for the logged-in user
        printf("\n\t\t Booked Tickets for User: %s\n", db->users[userIndex]->person.username);
        printf("\n\t\t -------------------------------------\n");

        // Check if the user has booked any tickets

        int cnt=0; 
        for (int i=0;i<db->bookedTicketsCount;i++) {
    
            if(strcmp((db->bookedTickets[i].user.person.username),inputUsername)!=0) continue;
            cnt++;
            printf("\n\t\t Booked Ticket Details:\n");
            printf("\n\t\t Passenger Type: ");
        switch (db->bookedTickets[i].passengerType) {
            case 1:
                printf("\n\t\t Infant\n");
                break;
            case 2:
                printf("\t\t Child\n");
                break;
            case 3:
                printf("\t\t Adult\n");
                break;
            default:
                printf("\n\t\t Unknown\n");
        }
            printf("\n\t\t Flight Number: %s\n", db->bookedTickets[i].flight.flightNumber);
            printf("\n\t\t Source: %s\n", db->bookedTickets[i].flight.source);
            printf("\n\t\t Destination: %s\n", db->bookedTickets[i].flight.destination);
            printf("\n\t\t Departure Date: %s\n", db->bookedTickets[i].flight.departureDate);
            printf("\n\t\t Departure Time: %s\n", db->bookedTickets[i].flight.departureTime);
            printf("\n\t\t Arrival Date: %s\n", db->bookedTickets[i].flight.arrivalDate);
            printf("\n\t\t Arrival Time: %s\n", db->bookedTickets[i].flight.arrivalTime);
            printf("\n\t\t _______________________________________ \n");
        } if(cnt==0) {
            printf("\n\t\t No booked tickets found for this user.\n");
        }
    } else if (userType == 2) {
        // Admin login to view all booked tickets
        char inputUsername[20];
        char inputPassword[20];
        char inputEmployeeID[20];

        printf("\n\t\t Enter your username: ");
        scanf("%s", inputUsername);

        printf("\n\t\t Enter your password: ");
        scanf("%s", inputPassword);

        printf("\n\t\t Enter your employee ID: ");
        scanf("%s", inputEmployeeID);
        printf("\n\t\t _____________________________________________ \n");

        int adminIndex = -1;
        for (int i = 0; i < db->count; ++i) {
            if (strcmp(db->admins[i]->person.username, inputUsername) == 0 &&
                strcmp(db->admins[i]->person.password, inputPassword) == 0 &&
                strcmp(db->admins[i]->employeeID, inputEmployeeID) == 0) {
                adminIndex = i;
                break;
            }
        }

        if (adminIndex == -1) {
            printf("\n\t\t Login failed. Invalid username, password, or employee ID.\n");
            return;
        }

        // Display all booked tickets for the admin
        printf("\n\t\t Displaying all booked Tickets for Admin: %s\n", db->admins[adminIndex]->person.username);
        printf("\n\t\t _____________________________________________ \n");

        for (int i = 0; i < db->count; ++i) {
            if (db->bookedTickets[i].user.person.username != NULL) {
                printf("\n\t\t User: %s\n", db->bookedTickets[i].user.person.username);
                printf("\n\t\t _________________________________________ \n");
                printf("\n\t\t Booked Ticket Details:\n");
                printf("\n\t\t _________________________________________ \n");
                printf("\n\t\t Passenger Type: ");
        switch (db->bookedTickets[i].passengerType) {
            case 1:
                printf("\t\t Infant\n");
                break;
            case 2:
                printf("\t\t Child\n");
                break;
            case 3:
                printf("\t\t Adult\n");
                break;
            default:
                printf("\t\t Unknown\n");
        }
                printf("\n\t\t Flight Number: %s\n", db->bookedTickets[i].flight.flightNumber);
                printf("\n\t\t Source: %s\n", db->bookedTickets[i].flight.source);
                printf("\n\t\t Destination: %s\n", db->bookedTickets[i].flight.destination);
                printf("\n\t\t Departure Date: %s\n", db->bookedTickets[i].flight.departureDate);
                printf("\n\t\t Departure Time: %s\n", db->bookedTickets[i].flight.departureTime);
                printf("\n\t\t Arrival Date: %s\n", db->bookedTickets[i].flight.arrivalDate);
                printf("\n\t\t Arrival Time: %s\n", db->bookedTickets[i].flight.arrivalTime);
                printf("\n\t\t __________________________________________ \n");
            }
        }
    } else {
        printf("\n\t\t Invalid choice. Please try again.\n");
    }
}


void adminUtilities(struct Database *db, struct Flight flights[], int *numFlights) {
    if (db->count > 0) {
        char inputUsername[20];
        char inputPassword[20];
        char inputEmployeeID[20];

        printf("\n\t\t Enter your username: ");
        scanf("%s", inputUsername);

        printf("\n\t\t Enter your password: ");
        scanf("%s", inputPassword);

        printf("\n\t\t Enter your employee ID: ");
        scanf("%s", inputEmployeeID);

        int adminIndex = -1;
        for (int i = 0; i < db->count; ++i) {
            if (strcmp(db->admins[i]->person.username, inputUsername) == 0 &&
                strcmp(db->admins[i]->person.password, inputPassword) == 0 &&
                strcmp(db->admins[i]->employeeID, inputEmployeeID) == 0) {
                adminIndex = i;
                break;
            }
        }

        if (adminIndex != -1) {
            int adminChoice;
            do {
                printf("\n\t\t Admin Utilities\n");
                printf("\n\t\t 1. Enter Flight Details\n");
                printf("\n\t\t 2. Edit Flight Details\n");
                printf("\n\t\t 3. View All Flight Details\n");
                printf("\n\t\t 4. Back to Main Menu\n");
                printf("\n\t\t Enter your choice: ");
                scanf("%d", &adminChoice);
                printf("\n\t\t ___________________________________________\n");

                switch (adminChoice) {
                    case 1:
                        enterFlightDetails(flights, numFlights);
                        break;
                    case 2:
                        editFlightDetails(flights, *numFlights);
                        break;
                    case 3:
                        viewAllFlightDetails(flights, *numFlights);
                        break;
                    case 4:
                        printf("\n\t\t Returning to the main menu.\n");
                        printf("\n\t\t _________________________________ \n");
                        break;
                    default:
                        printf("\n\t\t Invalid choice. Please enter a valid option.\n");
                }
            } while (adminChoice != 4);
        } else {
            printf("\n\t\t Login failed. Invalid username, password, or employee ID.\n");
        }
    } else {
        printf("\n\t\t No admins exist. Admin utilities cannot be accessed.\n");
    }
}

// Function to sign up a new user or admin
void signUp(struct Database *db) {
    if (db->count == MAX_USERS + MAX_ADMINS) {
        printf("\n\t\t Database is full. Cannot sign up new users or admins.\n");
        return;
    }

    int userType;
    printf("\n\t\t Choose user type:\n");
    printf("\n\t\t 1. User\n");
    printf("\t\t 2. Admin\n");
    printf("\n\t\t Enter your choice: ");
    scanf("%d", &userType);
    printf("\t\t -------------------------------------------\n");

    if (userType != 1 && userType != 2) {
        printf("Invalid choice. Please try again.\n");
        return;
    }

    char inputUsername[20];
    char inputPassword[20];

    printf("\n\t\t Enter your desired username: ");
    scanf(" %19[^\n]", inputUsername);

    // Check if the username already exists
    for (int i = 0; i < db->count; ++i) {
        if ((userType == 1 && strcmp(db->users[i]->person.username, inputUsername) == 0) ||
            (userType == 2 && strcmp(db->admins[i]->person.username, inputUsername) == 0)) {
            printf("Username already exists. Please try again with a different username.\n");
            return;
        }
    }

    printf("\n\t\t Enter your desired password: ");
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
            printf("\n\t\t Memory allocation error.\n");
            exit(EXIT_FAILURE);
        }

        printf("\n\t\t Enter your name: ");
        newUser->person.name = (char *)malloc(50 * sizeof(char));
        scanf(" %49[^\n]", newUser->person.name);

        printf("\n\t\t Enter your phone number: ");
        newUser->person.phone = (char *)malloc(15 * sizeof(char));
        scanf(" %14[^\n]", newUser->person.phone);

        printf("\n\t\t Enter your email: ");
        newUser->person.email = (char *)malloc(50 * sizeof(char));
        scanf(" %49[^\n]", newUser->person.email);

        newUser->person.username = strdup(inputUsername);
        newUser->person.password = strdup(inputPassword);

        db->users[db->count++] = newUser;

        printf("\n\t\t ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
        printf("\n\t\t Sign up successful!\n\n");
        printf("\t\t ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\n");
    } else if (userType == 2 && db->count < MAX_ADMINS) {
        struct Admin *newAdmin = (struct Admin *)malloc(sizeof(struct Admin));
        if (!newAdmin) {
            printf("\n\t\t Memory allocation error.\n");
            exit(EXIT_FAILURE);
        }

        printf("\n\t\t Enter your name: ");
        newAdmin->person.name = (char *)malloc(50 * sizeof(char));
        scanf(" %49[^\n]", newAdmin->person.name);

        printf("\n\t\t Enter your phone number: ");
        newAdmin->person.phone = (char *)malloc(15 * sizeof(char));
        scanf(" %14[^\n]", newAdmin->person.phone);

        printf("\n\t\t Enter your email: ");
        newAdmin->person.email = (char *)malloc(50 * sizeof(char));
        scanf(" %49[^\n]", newAdmin->person.email);

        newAdmin->person.username = strdup(inputUsername);
        newAdmin->person.password = strdup(inputPassword);

        printf("\n\t\t Enter your unique employee ID: ");
        newAdmin->employeeID = (char *)malloc(20 * sizeof(char));
        scanf(" %19[^\n]", newAdmin->employeeID);

        db->admins[db->count++] = newAdmin;

        printf("\n\t\t ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
        printf("\n\t\t Admin signup successful!\n\n");
        printf("\t\t ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\n");
    } else {
        printf("\n\t\t Database is full. Cannot sign up new users or admins.\n");
    }
}


// Function to enter flight details
void enterFlightDetails(struct Flight flights[], int *numFlights) {
    if (*numFlights == MAX_FLIGHTS) {
        printf("\n\t\t Maximum number of flights reached. Cannot add more flights.\n");
        return;
    }

    printf("\n\t\t Enter Flight Details:\n");
    printf("\n\t\t __________________________________________ \n");
    printf("\n\t\t Flight Number: ");
    scanf("%s", flights[*numFlights].flightNumber);

    // Check if the flight number already exists
    for (int i = 0; i < *numFlights; i++) {
        if (strcmp(flights[i].flightNumber, flights[*numFlights].flightNumber) == 0) {
            printf("\n\t\t Flight with the given number already exists. Cannot add duplicate flights.\n");
            return;
        }
    }

    printf("\n\t\t Departure Time: ");
    scanf("%s", flights[*numFlights].departureTime);
    printf("\n\t\t Arrival Time: ");
    scanf("%s", flights[*numFlights].arrivalTime);

    // Input for departure and arrival dates
    printf("\n\t\t Departure Date: ");
    scanf("%s", flights[*numFlights].departureDate);
    printf("\n\t\t Arrival Date: ");
    scanf("%s", flights[*numFlights].arrivalDate);

    printf("\n\t\t Ticket Price for Infant: ");
    scanf("%f", &flights[*numFlights].ticketPriceInfant);
    printf("\n\t\t Ticket Price for Child: ");
    scanf("%f", &flights[*numFlights].ticketPriceChild);
    printf("\n\t\t Ticket Price for Adult: ");
    scanf("%f", &flights[*numFlights].ticketPriceAdult);
    printf("\n\t\t Source: ");
    scanf("%s", flights[*numFlights].source);
    printf("\n\t\t Destination: ");
    scanf("%s", flights[*numFlights].destination);

    // Input validation for seats available
    do {
        printf("\n\t\t Number of Seats Available: ");
        scanf("%d", &flights[*numFlights].seatsAvailable);
        if (flights[*numFlights].seatsAvailable < 0) {
            printf("\n\t\t Please enter a non-negative number of seats.\n");
        }
    } while (flights[*numFlights].seatsAvailable < 0);

    printf("\n\t\t Flight Details added successfully.\n");
    printf("\n\t\t _____________________________________________________\n");
    (*numFlights)++;
}

// Function to edit flight details
void editFlightDetails(struct Flight flights[], int numFlights) {
    char editFlightNumber[10];
    printf("\n\t\t Enter the Flight Number to edit details: ");
    scanf("%s", editFlightNumber);

    for (int i = 0; i < numFlights; i++) {
        if (strcmp(flights[i].flightNumber, editFlightNumber) == 0) {
            printf("\n\t\t Flight Details found. Enter new details:\n");
            printf("\n\t\t Departure Time: ");
            scanf("%s", flights[i].departureTime);
            printf("\n\t\t Arrival Time: ");
            scanf("%s", flights[i].arrivalTime);

            // Input for departure and arrival dates
            printf("\n\t\t Departure Date: ");
            scanf("%s", flights[i].departureDate);
            printf("\n\t\t Arrival Date: ");
            scanf("%s", flights[i].arrivalDate);

            printf("\n\t\t Ticket Price for Infant: ");
            scanf("%f", &flights[i].ticketPriceInfant);
            printf("\n\t\t Ticket Price for Child: ");
            scanf("%f", &flights[i].ticketPriceChild);
            printf("\n\t\t Ticket Price for Adult: ");
            scanf("%f", &flights[i].ticketPriceAdult);
            printf("\n\t\t Source: ");
            scanf("%s", flights[i].source);
            printf("\n\t\t Destination: ");
            scanf("%s", flights[i].destination);

            // Input validation for seats available during editing
            do {
                printf("\n\t\t Number of Seats Available: ");
                scanf("%d", &flights[i].seatsAvailable);
                if (flights[i].seatsAvailable < 0) {
                    printf("\n\t\t Please enter a non-negative number of seats.\n");
                }
            } while (flights[i].seatsAvailable < 0);

            printf("\n\t\t Flight Details updated successfully.\n");
            printf("\n\t\t ____________________________________ \n");
            return;
        }
    }

    printf("\n\t\t Flight with the given number not found.\n");
    printf("\n\t\t ________________________________________\n");
}

// Function for viewing all flight details
void viewAllFlightDetails(struct Flight flights[], int numFlights) {
    if (numFlights == 0) {
        printf("\n\t\t No flights available.\n");
        return;
    }

    printf("\n\t\t Flight Details:\n");
    printf("\n\t\t ______________________________ \n");
    for (int i = 0; i < numFlights; i++) {
        printf("\n\t\t Flight Number: %s\n", flights[i].flightNumber);
        printf("\n\t\t Departure Date: %s\n", flights[i].departureDate);
        printf("\n\t\t Arrival Date: %s\n", flights[i].arrivalDate);
        printf("\n\t\t Departure Time: %s\n", flights[i].departureTime);
        printf("\n\t\t Arrival Time: %s\n", flights[i].arrivalTime);
        printf("\n\t\t Ticket Price for Infant: %.2f\n", flights[i].ticketPriceInfant);
        printf("\n\t\t Ticket Price for Child: %.2f\n", flights[i].ticketPriceChild);
        printf("\n\t\t Ticket Price for Adult: %.2f\n", flights[i].ticketPriceAdult);
        printf("\n\t\t Source: %s\n", flights[i].source);
        printf("\n\t\t Destination: %s\n", flights[i].destination);
        printf("\n\t\t Seats Available: %d\n", flights[i].seatsAvailable);
        printf("\n\t\t -------------------------------\n");
    }
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

// Function to save user or admin data to a file (not working)
/*void saveDataToFile(struct Database *db, const char *filename) {
    FILE *file = fopen(filename, "a+");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < db->count; ++i) {
        if (i < MAX_USERS) {
            fprintf(file, "%s|%s|%s|%s|%s|%d\n",
                    db->users[i]->person.name, db->users[i]->person.phone, db->users[i]->person.email,
                    db->users[i]->person.username, db->users[i]->person.password, 1);
        } else {
            fprintf(file, "%s|%s|%s|%s|%s|%s|%d\n",
                    db->admins[i - MAX_USERS]->person.name, db->admins[i - MAX_USERS]->person.phone,
                    db->admins[i - MAX_USERS]->person.email, db->admins[i - MAX_USERS]->person.username,
                    db->admins[i - MAX_USERS]->person.password, db->admins[i - MAX_USERS]->employeeID, 2);
        }
    }

    fclose(file);
}*/


// Function to load user or admin data from a file (not working)
/*void loadDataFromFile(struct Database *db, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No existing data file found for %s.\n", filename);
        return;
    }

    char name[50], phone[15], email[50], username[20], password[20], employeeID[20];
    int userType;

    while (fscanf(file, "%49[^|]|%14[^|]|%49[^|]|%19[^|]|%19[^|]|%19[^|]|%d", name, phone, email, username, password, employeeID, &userType) != EOF) {
        if (db->count < MAX_USERS + MAX_ADMINS) {
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
            printf("Database is full. Some data may not be loaded.\n");
            break;
        }
    }

    fclose(file);
}*/


// Function to display details
void displayDetails(const struct Person *person) {
    printf("\n\t\t Details:\n");
    printf("\n\t\t Name: %s\n", person->name);
    printf("\n\t\t Phone: %s\n", person->phone);
    printf("\n\t\t Email: %s\n", person->email);
    printf("\n\t\t Username: %s\n", person->username);

    // Simulate a delay for a more realistic experience
    usleep(2000000);

    printf("\n\t\t Redirecting to main menu...\n");
}

