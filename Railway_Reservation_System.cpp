#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Train
{
    int number, distance;
    string name, source, destination, type;
    int sleeper, ac, general;
};

struct Booking
{
    int pnr, trainNo, seats, age[10];
    string name[10], gender[10], travelClass, status;
    double fare;
};

Train trains[50];
Booking bookings[100];

int trainCount = 0, bookingCount = 0, nextPNR = 1001;

// Save train details
void saveTrains()
{
    ofstream file("trains.txt");

    for (int i = 0; i < trainCount; i++)
    {
        file << trains[i].number << " "
             << trains[i].name << " "
             << trains[i].source << " "
             << trains[i].destination << " "
             << trains[i].distance << " "
             << trains[i].type << " "
             << trains[i].sleeper << " "
             << trains[i].ac << " "
             << trains[i].general << endl;
    }

    file.close();
}

// Load train details
void loadTrains()
{
    ifstream file("trains.txt");

    while (trainCount < 50 &&
           file >> trains[trainCount].number
                >> trains[trainCount].name
                >> trains[trainCount].source
                >> trains[trainCount].destination
                >> trains[trainCount].distance
                >> trains[trainCount].type
                >> trains[trainCount].sleeper
                >> trains[trainCount].ac
                >> trains[trainCount].general)
    {
        trainCount++;
    }

    file.close();
}

// Save bookings
void saveBookings()
{
    ofstream file("bookings.txt");

    file << nextPNR << endl;

    for (int i = 0; i < bookingCount; i++)
    {
        file << bookings[i].pnr << " "
             << bookings[i].trainNo << " "
             << bookings[i].seats << " "
             << bookings[i].travelClass << " "
             << bookings[i].fare << " "
             << bookings[i].status << endl;

        for (int j = 0; j < bookings[i].seats; j++)
        {
            file << bookings[i].name[j] << " "
                 << bookings[i].age[j] << " "
                 << bookings[i].gender[j] << endl;
        }
    }

    file.close();
}

// Load bookings
void loadBookings()
{
    ifstream file("bookings.txt");

    if (!(file >> nextPNR))
    {
        nextPNR = 1001;
        return;
    }

    while (bookingCount < 100)
    {
        Booking b;

        if (!(file >> b.pnr >> b.trainNo >> b.seats
                  >> b.travelClass >> b.fare >> b.status))
            break;

        if (b.seats < 1 || b.seats > 10)
            break;

        bool valid = true;

        for (int j = 0; j < b.seats; j++)
        {
            if (!(file >> b.name[j] >> b.age[j] >> b.gender[j]))
            {
                valid = false;
                break;
            }
        }

        if (!valid)
            break;

        bookings[bookingCount++] = b;
    }

    file.close();
}

// Add a train
void addTrain()
{
    if (trainCount >= 50)
    {
        cout << "Train limit reached!\n";
        return;
    }

    Train t;

    cout << "Enter train number: ";
    cin >> t.number;

    for (int i = 0; i < trainCount; i++)
    {
        if (trains[i].number == t.number)
        {
            cout << "Train number already exists!\n";
            return;
        }
    }

    cout << "Enter train name (one word): ";
    cin >> t.name;

    cout << "Enter source: ";
    cin >> t.source;

    cout << "Enter destination: ";
    cin >> t.destination;

    cout << "Enter distance in km: ";
    cin >> t.distance;

    cout << "Enter type (Express/Superfast): ";
    cin >> t.type;

    cout << "Enter Sleeper seats: ";
    cin >> t.sleeper;

    cout << "Enter AC seats: ";
    cin >> t.ac;

    cout << "Enter General seats: ";
    cin >> t.general;

    if (t.distance <= 0 || t.sleeper < 0 ||
        t.ac < 0 || t.general < 0)
    {
        cout << "Invalid distance or seat count!\n";
        return;
    }

    trains[trainCount++] = t;
    saveTrains();

    cout << "Train added successfully!\n";
}

// Display all trains
void showTrains()
{
    if (trainCount == 0)
    {
        cout << "No trains available.\n";
        return;
    }

    cout << "\n========== TRAIN LIST ==========\n";

    for (int i = 0; i < trainCount; i++)
    {
        cout << "\nTrain Number: " << trains[i].number;
        cout << "\nTrain Name: " << trains[i].name;
        cout << "\nRoute: " << trains[i].source
             << " -> " << trains[i].destination;
        cout << "\nDistance: " << trains[i].distance << " km";
        cout << "\nType: " << trains[i].type;
        cout << "\nSleeper Seats: " << trains[i].sleeper;
        cout << "\nAC Seats: " << trains[i].ac;
        cout << "\nGeneral Seats: " << trains[i].general;
        cout << "\n-------------------------------\n";
    }
}

// Update train details
void updateTrain()
{
    int no;

    cout << "Enter train number to update: ";
    cin >> no;

    for (int i = 0; i < trainCount; i++)
    {
        if (trains[i].number == no)
        {
            cout << "Enter new train name: ";
            cin >> trains[i].name;

            cout << "Enter new source: ";
            cin >> trains[i].source;

            cout << "Enter new destination: ";
            cin >> trains[i].destination;

            cout << "Enter new distance: ";
            cin >> trains[i].distance;

            cout << "Enter train type: ";
            cin >> trains[i].type;

            if (trains[i].distance <= 0)
            {
                cout << "Invalid distance!\n";
                return;
            }

            saveTrains();
            cout << "Train updated successfully!\n";
            return;
        }
    }

    cout << "Train not found!\n";
}

// Delete a train
void deleteTrain()
{
    int no;

    cout << "Enter train number to delete: ";
    cin >> no;

    for (int i = 0; i < trainCount; i++)
    {
        if (trains[i].number == no)
        {
            for (int j = 0; j < bookingCount; j++)
            {
                if (bookings[j].trainNo == no &&
                    bookings[j].status == "Booked")
                {
                    cout << "Cancel active bookings first!\n";
                    return;
                }
            }

            for (int j = i; j < trainCount - 1; j++)
                trains[j] = trains[j + 1];

            trainCount--;
            saveTrains();

            cout << "Train deleted successfully!\n";
            return;
        }
    }

    cout << "Train not found!\n";
}

// Search trains
void searchTrain()
{
    string from, to;

    cout << "Enter source: ";
    cin >> from;

    cout << "Enter destination: ";
    cin >> to;

    bool found = false;

    for (int i = 0; i < trainCount; i++)
    {
        if (trains[i].source == from &&
            trains[i].destination == to)
        {
            cout << "\nTrain Number: " << trains[i].number;
            cout << "\nTrain Name: " << trains[i].name;
            cout << "\nSleeper Seats: " << trains[i].sleeper;
            cout << "\nAC Seats: " << trains[i].ac;
            cout << "\nGeneral Seats: " << trains[i].general;
            cout << "\n";

            found = true;
        }
    }

    if (!found)
        cout << "No trains found for this route.\n";
}

// Book a ticket
void bookTicket()
{
    if (bookingCount >= 100)
    {
        cout << "Booking limit reached!\n";
        return;
    }

    int no, choice;
    cout << "Enter train number: ";
    cin >> no;

    int index = -1;

    for (int i = 0; i < trainCount; i++)
    {
        if (trains[i].number == no)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        cout << "Train not found!\n";
        return;
    }

    cout << "1. Sleeper\n2. AC\n3. General\n";
    cout << "Select class: ";
    cin >> choice;

    int *availableSeats;

    if (choice == 1)
        availableSeats = &trains[index].sleeper;
    else if (choice == 2)
        availableSeats = &trains[index].ac;
    else if (choice == 3)
        availableSeats = &trains[index].general;
    else
    {
        cout << "Invalid class!\n";
        return;
    }

    Booking b;

    cout << "Enter number of passengers (1-10): ";
    cin >> b.seats;

    if (b.seats < 1 || b.seats > 10 ||
        b.seats > *availableSeats)
    {
        cout << "Invalid passenger count or insufficient seats!\n";
        return;
    }

    b.pnr = nextPNR++;
    b.trainNo = no;
    b.status = "Booked";

    if (choice == 1)
    {
        b.travelClass = "Sleeper";
    }
    else if (choice == 2)
    {
        b.travelClass = "AC";
    }
    else
    {
        b.travelClass = "General";
    }

    double rate;

    if (choice == 1)
        rate = 1.5;
    else if (choice == 2)
        rate = 3.0;
    else
        rate = 0.8;

    if (trains[index].type == "Superfast")
        rate += 0.5;

    b.fare = trains[index].distance * rate * b.seats;

    cout << "\nEnter passenger details:\n";

    for (int i = 0; i < b.seats; i++)
    {
        cout << "\nPassenger " << i + 1 << " name (one word): ";
        cin >> b.name[i];

        cout << "Age: ";
        cin >> b.age[i];

        cout << "Gender (M/F/O): ";
        cin >> b.gender[i];
    }

    *availableSeats -= b.seats;

    bookings[bookingCount++] = b;

    saveTrains();
    saveBookings();

    cout << "\n========== TICKET BOOKED ==========\n";
    cout << "PNR Number: " << b.pnr;
    cout << "\nTrain: " << trains[index].name;
    cout << "\nRoute: " << trains[index].source
         << " -> " << trains[index].destination;
    cout << "\nClass: " << b.travelClass;
    cout << "\nPassengers: " << b.seats;
    cout << "\nTotal Fare: Rs. " << b.fare;
    cout << "\nStatus: " << b.status;
    cout << "\n===================================\n";
}

// View ticket
void viewTicket()
{
    int pnr;

    cout << "Enter PNR number: ";
    cin >> pnr;

    for (int i = 0; i < bookingCount; i++)
    {
        if (bookings[i].pnr == pnr)
        {
            cout << "\n========== E-TICKET ==========\n";
            cout << "PNR: " << bookings[i].pnr;
            cout << "\nTrain Number: " << bookings[i].trainNo;
            cout << "\nClass: " << bookings[i].travelClass;
            cout << "\nPassengers: " << bookings[i].seats;
            cout << "\nTotal Fare: Rs. " << bookings[i].fare;
            cout << "\nStatus: " << bookings[i].status;

            cout << "\n\nPassenger Details:\n";

            for (int j = 0; j < bookings[i].seats; j++)
            {
                cout << j + 1 << ". "
                     << bookings[i].name[j]
                     << ", Age: " << bookings[i].age[j]
                     << ", Gender: " << bookings[i].gender[j]
                     << endl;
            }

            cout << "==============================\n";
            return;
        }
    }

    cout << "Ticket not found!\n";
}

// Cancel ticket
void cancelTicket()
{
    int pnr;

    cout << "Enter PNR number to cancel: ";
    cin >> pnr;

    for (int i = 0; i < bookingCount; i++)
    {
        if (bookings[i].pnr == pnr)
        {
            if (bookings[i].status == "Cancelled")
            {
                cout << "Ticket already cancelled!\n";
                return;
            }

            for (int j = 0; j < trainCount; j++)
            {
                if (trains[j].number == bookings[i].trainNo)
                {
                    if (bookings[i].travelClass == "Sleeper")
                        trains[j].sleeper += bookings[i].seats;
                    else if (bookings[i].travelClass == "AC")
                        trains[j].ac += bookings[i].seats;
                    else
                        trains[j].general += bookings[i].seats;

                    break;
                }
            }

            double refund = bookings[i].fare * 0.80;

            bookings[i].status = "Cancelled";

            saveTrains();
            saveBookings();

            cout << "Ticket cancelled successfully!\n";
            cout << "Cancellation charge: Rs. "
                 << bookings[i].fare - refund;
            cout << "\nRefund amount: Rs. " << refund << endl;

            return;
        }
    }

    cout << "PNR not found!\n";
}

int main()
{
    loadTrains();
    loadBookings();

    int choice;

    do
    {
        cout << "\n====================================\n";
        cout << "    RAILWAY RESERVATION SYSTEM\n";
        cout << "====================================\n";
        cout << "1. Add Train (Admin)\n";
        cout << "2. View All Trains\n";
        cout << "3. Update Train (Admin)\n";
        cout << "4. Delete Train (Admin)\n";
        cout << "5. Search Train\n";
        cout << "6. Book Ticket\n";
        cout << "7. Cancel Ticket\n";
        cout << "8. View E-Ticket\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";

        cin >> choice;

        switch (choice)
        {
            case 1:
                addTrain();
                break;

            case 2:
                showTrains();
                break;

            case 3:
                updateTrain();
                break;

            case 4:
                deleteTrain();
                break;

            case 5:
                searchTrain();
                break;

            case 6:
                bookTicket();
                break;

            case 7:
                cancelTicket();
                break;

            case 8:
                viewTicket();
                break;

            case 9:
                cout << "Thank you for using Railway Reservation System!\n";
                break;

            default:
                cout << "Invalid choice! Try again.\n";
        }

    } while (choice != 9);

    return 0;
}
