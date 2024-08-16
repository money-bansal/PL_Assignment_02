#include <bits/stdc++.h>
using namespace std;

// Date class to handle booking dates
class Date {
public:
    Date(int day, int month, int year) : day(day), month(month), year(year) {}

    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }

    // Display the date in YYYY-MM-DD format
    string toString() const {
        ostringstream oss;
        oss << setw(4) << setfill('0') << year << '-'
            << setw(2) << setfill('0') << month << '-'
            << setw(2) << setfill('0') << day;
        return oss.str();
    }

    // Simple comparison to check if one date is before another
    bool isBefore(const Date& other) const {
        return tie(year, month, day) < tie(other.year, other.month, other.day);
    }

    // Check if this date is on or after the other date
    bool isOnOrAfter(const Date& other) const {
        return tie(year, month, day) >= tie(other.year, other.month, other.day);
    }

    // Check if this date is after the other date
    bool isAfter(const Date& other) const {
        return tie(year, month, day) > tie(other.year, other.month, other.day);
    }

    // Check if this date is before or equal to the other date
    bool isBeforeOrEqual(const Date& other) const {
        return tie(year, month, day) <= tie(other.year, other.month, other.day);
    }

    // Check if two date ranges overlap
    static bool doDatesOverlap(const Date& start1, const Date& end1, const Date& start2, const Date& end2) {
        return start1.isBeforeOrEqual(end2) && start2.isBeforeOrEqual(end1);
    }

private:
    int day, month, year;
};

// Exception classes
class RoomNotFoundException : public runtime_error {
public:
    RoomNotFoundException(const string& msg) : runtime_error(msg) {}
};

class RoomAlreadyBookedException : public runtime_error {
public:
    RoomAlreadyBookedException(const string& msg) : runtime_error(msg) {}
};

class RoomNotBookedException : public runtime_error {
public:
    RoomNotBookedException(const string& msg) : runtime_error(msg) {}
};

// Base class for Room
class Room {
public:
    Room(int id, double price) : id(id), price(price), booked(false) {}

    virtual ~Room() = default;

    int getId() const { return id; }
    double getPrice() const { return price; }
    bool isBooked() const { return booked; }
    virtual string getType() const = 0; // Pure virtual function

    void book() {
        if (booked) throw RoomAlreadyBookedException("Room already booked.");
        booked = true;
    }

    void cancel() {
        if (!booked) throw RoomNotBookedException("Room was not booked.");
        booked = false;
    }

private:
    int id;
    double price;
    bool booked;
};

// Derived classes for specific types of rooms
class SingleRoom : public Room {
public:
    SingleRoom(int id, double price) : Room(id, price) {}

    string getType() const override {
        return "Single";
    }
};

class DoubleRoom : public Room {
public:
    DoubleRoom(int id, double price) : Room(id, price) {}

    string getType() const override {
        return "Double";
    }
};

class SuiteRoom : public Room {
public:
    SuiteRoom(int id, double price) : Room(id, price) {}

    string getType() const override {
        return "Suite";
    }
};

// Class for Customer
class Customer {
public:
    Customer(int id, const string& name) : id(id), name(name) {}

    int getId() const { return id; }
    string getName() const { return name; }

private:
    int id;
    string name;
};

// Class for Booking
class Booking {
public:
    Booking(shared_ptr<Room> room, shared_ptr<Customer> customer, Date startDate, Date endDate) 
        : room(room), customer(customer), startDate(startDate), endDate(endDate) {}

    shared_ptr<Room> getRoom() const { return room; }
    shared_ptr<Customer> getCustomer() const { return customer; }
    Date getStartDate() const { return startDate; }
    Date getEndDate() const { return endDate; }

private:
    shared_ptr<Room> room;
    shared_ptr<Customer> customer;
    Date startDate;
    Date endDate;
};

// Class for Hotel
class Hotel {
public:
    void addRoom(shared_ptr<Room> room) {
        rooms.push_back(room);
    }

    void addCustomer(shared_ptr<Customer> customer) {
        customers.push_back(customer);
    }

    shared_ptr<Room> findRoom(int id) const {
        for (auto& room : rooms) {
            if (room->getId() == id) {
                return room;
            }
        }
        throw RoomNotFoundException("Room not found");
    }

    shared_ptr<Customer> findCustomer(int id) const {
        for (auto& customer : customers) {
            if (customer->getId() == id) {
                return customer;
            }
        }
        throw runtime_error("Customer not found");
    }

    void bookRoom(int roomId, int customerId, Date startDate, Date endDate) {
        shared_ptr<Room> room = findRoom(roomId);
        shared_ptr<Customer> customer = findCustomer(customerId);

        if (room->isBooked()) {
            // Check for overlapping bookings
            for (const auto& booking : bookings) {
                if (booking->getRoom()->getId() == roomId &&
                    Date::doDatesOverlap(startDate, endDate, booking->getStartDate(), booking->getEndDate())) {
                    throw RoomAlreadyBookedException("Room is booked for the given date range.");
                }
            }
        }

        room->book();
        bookings.push_back(make_shared<Booking>(room, customer, startDate, endDate));
        cout << "Room booked successfully from " << startDate.toString() << " to " << endDate.toString() << ".\n";
    }

    void cancelBooking(int roomId) {
        auto it = find_if(bookings.begin(), bookings.end(), [roomId](const shared_ptr<Booking>& booking) {
            return booking->getRoom()->getId() == roomId;
        });

        if (it == bookings.end()) {
            throw RoomNotBookedException("Room was not booked");
        }

        (*it)->getRoom()->cancel();
        bookings.erase(it);
        cout << "Booking cancelled successfully.\n";
    }

    void listRooms() const {
        cout << "Rooms in the hotel:\n";
        for (const auto& room : rooms) {
            cout << "ID: " << room->getId() << ", Type: " << room->getType() 
                 << ", Price: $" << room->getPrice() 
                 << ", Booked: " << (room->isBooked() ? "Yes" : "No") << endl;
        }
    }

    void listCustomers() const {
        cout << "Customers in the hotel:\n";
        for (const auto& customer : customers) {
            cout << "ID: " << customer->getId() << ", Name: " << customer->getName() << endl;
        }
    }

private:
    vector<shared_ptr<Room>> rooms;
    vector<shared_ptr<Customer>> customers;
    vector<shared_ptr<Booking>> bookings;
};

// Main function
int main() {
    Hotel hotel;
    int choice;

    do {
        cout << "\nHotel Booking System\n";
        cout << "1. Add Room\n";
        cout << "2. Add Customer\n";
        cout << "3. Book Room\n";
        cout << "4. Cancel Booking\n";
        cout << "5. List Rooms\n";
        cout << "6. List Customers\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        try {
            switch (choice) {
                case 1: {
                    int id, type;
                    double price;
                    cout << "Enter room ID: ";
                    cin >> id;
                    cout << "Enter room price: ";
                    cin >> price;
                    cout << "Enter room type (1=Single, 2=Double, 3=Suite): ";
                    cin >> type;
                    shared_ptr<Room> room;
                    switch (type) {
                        case 1:
                            room = make_shared<SingleRoom>(id, price);
                            break;
                        case 2:
                            room = make_shared<DoubleRoom>(id, price);
                            break;
                        case 3:
                            room = make_shared<SuiteRoom>(id, price);
                            break;
                        default:
                            throw runtime_error("Invalid room type");
                    }
                    hotel.addRoom(room);
                    cout << "Room added successfully.\n";
                    break;
                }
                case 2: {
                    int id;
                    string name;
                    cout << "Enter customer ID: ";
                    cin >> id;
                    cin.ignore(); // ignore newline character
                    cout << "Enter customer name: ";
                    getline(cin, name);
                    hotel.addCustomer(make_shared<Customer>(id, name));
                    cout << "Customer added successfully.\n";
                    break;
                }
                case 3: {
                    int roomId, customerId;
                    int startDay, startMonth, startYear;
                    int endDay, endMonth, endYear;
                    cout << "Enter room ID to book: ";
                    cin >> roomId;
                    cout << "Enter customer ID: ";
                    cin >> customerId;
                    cout << "Enter start date (day month year): ";
                    cin >> startDay >> startMonth >> startYear;
                    cout << "Enter end date (day month year): ";
                    cin >> endDay >> endMonth >> endYear;
                    Date startDate(startDay, startMonth, startYear);
                    Date endDate(endDay, endMonth, endYear);
                    hotel.bookRoom(roomId, customerId, startDate, endDate);
                    break;
                }
                case 4: {
                    int roomId;
                    cout << "Enter room ID to cancel booking: ";
                    cin >> roomId;
                    hotel.cancelBooking(roomId);
                    break;
                }
                case 5: {
                    hotel.listRooms();
                    break;
                }
                case 6: {
                    hotel.listCustomers();
                    break;
                }
                case 7:
                    cout << "Exiting...\n";
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    } while (choice != 7);

    return 0;
}
