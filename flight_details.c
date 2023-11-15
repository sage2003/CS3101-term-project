#include <stdio.h>
#include <string.h>

#define MAX_FLIGHTS 50

struct Flight {
    char flightNumber[10];
    char departureTime[20];
    char arrivalTime[20];
    float ticketPriceInfant;
    float ticketPriceChild;
    float ticketPriceAdult;
    char source[50];
    char destination[50];
    int seatsAvailable;  // New field for the number of seats available
};

void enterFlightDetails(struct Flight flights[], int *numFlights) {
    if (*numFlights == MAX_FLIGHTS) {
        printf("Maximum number of flights reached. Cannot add more flights.\n");
        return;
    }

    printf("Enter Flight Details:\n");
    printf("Flight Number: ");
    scanf("%s", flights[*numFlights].flightNumber);

    // Check if the flight number already exists
    for (int i = 0; i < *numFlights; i++) {
        if (strcmp(flights[i].flightNumber, flights[*numFlights].flightNumber) == 0) {
            printf("Flight with the given number already exists. Cannot add duplicate flights.\n");
            return;
        }
    }

    printf("Departure Time: ");
    scanf("%s", flights[*numFlights].departureTime);
    printf("Arrival Time: ");
    scanf("%s", flights[*numFlights].arrivalTime);
    printf("Ticket Price for Infant: ");
    scanf("%f", &flights[*numFlights].ticketPriceInfant);
    printf("Ticket Price for Child: ");
    scanf("%f", &flights[*numFlights].ticketPriceChild);
    printf("Ticket Price for Adult: ");
    scanf("%f", &flights[*numFlights].ticketPriceAdult);
    printf("Source: ");
    scanf("%s", flights[*numFlights].source);
    printf("Destination: ");
    scanf("%s", flights[*numFlights].destination);

    // Input validation for seats available
    do {
        printf("Number of Seats Available: ");
        scanf("%d", &flights[*numFlights].seatsAvailable);
        if (flights[*numFlights].seatsAvailable < 0) {
            printf("Please enter a non-negative number of seats.\n");
        }
    } while (flights[*numFlights].seatsAvailable < 0);

    printf("Flight Details added successfully.\n");
    (*numFlights)++;
}

void editFlightDetails(struct Flight flights[], int numFlights) {
    char editFlightNumber[10];
    printf("Enter the Flight Number to edit details: ");
    scanf("%s", editFlightNumber);

    for (int i = 0; i < numFlights; i++) {
        if (strcmp(flights[i].flightNumber, editFlightNumber) == 0) {
            printf("Flight Details found. Enter new details:\n");
            printf("Departure Time: ");
            scanf("%s", flights[i].departureTime);
            printf("Arrival Time: ");
            scanf("%s", flights[i].arrivalTime);
            printf("Ticket Price for Infant: ");
            scanf("%f", &flights[i].ticketPriceInfant);
            printf("Ticket Price for Child: ");
            scanf("%f", &flights[i].ticketPriceChild);
            printf("Ticket Price for Adult: ");
            scanf("%f", &flights[i].ticketPriceAdult);
            printf("Source: ");
            scanf("%s", flights[i].source);
            printf("Destination: ");
            scanf("%s", flights[i].destination);

            // Input validation for seats available during editing
            do {
                printf("Number of Seats Available: ");
                scanf("%d", &flights[i].seatsAvailable);
                if (flights[i].seatsAvailable < 0) {
                    printf("Please enter a non-negative number of seats.\n");
                }
            } while (flights[i].seatsAvailable < 0);

            printf("Flight Details updated successfully.\n");
            return;
        }
    }

    printf("Flight with the given number not found.\n");
}

void viewAllFlightDetails(struct Flight flights[], int numFlights) {
    if (numFlights == 0) {
        printf("No flights available.\n");
        return;
    }

    printf("Flight Details:\n");
    for (int i = 0; i < numFlights; i++) {
        printf("Flight Number: %s\n", flights[i].flightNumber);
        printf("Departure Time: %s\n", flights[i].departureTime);
        printf("Arrival Time: %s\n", flights[i].arrivalTime);
        printf("Ticket Price for Infant: %.2f\n", flights[i].ticketPriceInfant);
        printf("Ticket Price for Child: %.2f\n", flights[i].ticketPriceChild);
        printf("Ticket Price for Adult: %.2f\n", flights[i].ticketPriceAdult);
        printf("Source: %s\n", flights[i].source);
        printf("Destination: %s\n", flights[i].destination);
        printf("Seats Available: %d\n", flights[i].seatsAvailable);
        printf("-------------------------------\n");
    }
}

int main() {
    struct Flight flights[MAX_FLIGHTS];
    int numFlights = 0;
    int choice;

    do {
        printf("\nFlight Reservation System\n");
        printf("1. Enter Flight Details\n");
        printf("2. Edit Flight Details\n");
        printf("3. View All Flight Details\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                enterFlightDetails(flights, &numFlights);
                break;
            case 2:
                editFlightDetails(flights, numFlights);
                break;
            case 3:
                viewAllFlightDetails(flights, numFlights);
                break;
            case 4:
                printf("Exiting the program. Thank you!\n");
                break;
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != 4);

    return 0;
}
