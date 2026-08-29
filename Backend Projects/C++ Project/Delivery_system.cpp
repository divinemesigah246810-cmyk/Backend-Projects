/*
    ========================================
      DELIVERY MANAGEMENT SYSTEM (Console)
    ========================================
    Features:
      - Customer: Register / View / Search / Update
      - Delivery: Create / View / Search / Update / Track / Delete
      - Auto-generated Delivery IDs (DEL-1001, DEL-1002, ...)
      - Delivery status workflow

    Compile:
      g++ -std=c++17 delivery_management_system.cpp -o delivery
*/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <iomanip>

using namespace std;

// ---------------------------------------------------------
// Data Models
// ---------------------------------------------------------

struct Customer {
    int id;
    string name;
    string phone;
    string address;
};

enum class DeliveryStatus {
    Pending,
    PickedUp,
    InTransit,
    OutForDelivery,
    Delivered,
    Cancelled
};

string statusToString(DeliveryStatus status) {
    switch (status) {
        case DeliveryStatus::Pending:        return "Pending";
        case DeliveryStatus::PickedUp:       return "Picked Up";
        case DeliveryStatus::InTransit:      return "In Transit";
        case DeliveryStatus::OutForDelivery: return "Out for Delivery";
        case DeliveryStatus::Delivered:      return "Delivered";
        case DeliveryStatus::Cancelled:      return "Cancelled";
    }
    return "Unknown";
}

struct Delivery {
    string deliveryId;
    string senderName;
    string receiverName;
    string pickupLocation;
    string deliveryLocation;
    string packageType;
    double packageWeight; // kg
    double deliveryFee;   // GH cedis
    string deliveryDate;
    DeliveryStatus status;
};

// ---------------------------------------------------------
// Global Storage
// ---------------------------------------------------------

vector<Customer> customers;
vector<Delivery> deliveries;

int nextCustomerId = 1;
int nextDeliveryNumber = 1001; // produces DEL-1001, DEL-1002, ...

// ---------------------------------------------------------
// Utility / Input Helpers
// ---------------------------------------------------------

void pause() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string readLine(const string &prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

double readDouble(const string &prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            clearInputBuffer();
            return value;
        }
        cout << "Invalid number. Please try again.\n";
        clearInputBuffer();
    }
}

int readInt(const string &prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            clearInputBuffer();
            return value;
        }
        cout << "Invalid input. Please enter a number.\n";
        clearInputBuffer();
    }
}

string generateDeliveryId() {
    string id = "DEL-" + to_string(nextDeliveryNumber);
    nextDeliveryNumber++;
    return id;
}

void printLine(char ch = '-', int count = 40) {
    cout << string(count, ch) << "\n";
}

// ---------------------------------------------------------
// Customer Management
// ---------------------------------------------------------

void registerCustomer() {
    cout << "\n---------- REGISTER CUSTOMER ----------\n";
    Customer c;
    c.id = nextCustomerId++;
    c.name = readLine("Customer Name: ");
    c.phone = readLine("Phone Number: ");
    c.address = readLine("Address: ");

    customers.push_back(c);

    cout << "\nCustomer registered successfully!\n";
    cout << "Customer ID: " << c.id << "\n";
}

void printCustomer(const Customer &c) {
    printLine();
    cout << "Customer ID : " << c.id << "\n";
    cout << "Name        : " << c.name << "\n";
    cout << "Phone       : " << c.phone << "\n";
    cout << "Address     : " << c.address << "\n";
    printLine();
}

void viewCustomers() {
    cout << "\n---------- ALL CUSTOMERS ----------\n";
    if (customers.empty()) {
        cout << "No customers registered yet.\n";
        return;
    }
    for (const auto &c : customers) {
        printCustomer(c);
    }
}

Customer* findCustomerById(int id) {
    for (auto &c : customers) {
        if (c.id == id) return &c;
    }
    return nullptr;
}

void searchCustomer() {
    cout << "\n---------- SEARCH CUSTOMER ----------\n";
    if (customers.empty()) {
        cout << "No customers registered yet.\n";
        return;
    }
    string keyword = readLine("Enter name or ID to search: ");

    bool found = false;
    for (const auto &c : customers) {
        if (to_string(c.id) == keyword ||
            c.name.find(keyword) != string::npos) {
            printCustomer(c);
            found = true;
        }
    }
    if (!found) {
        cout << "No matching customer found.\n";
    }
}

void updateCustomer() {
    cout << "\n---------- UPDATE CUSTOMER ----------\n";
    if (customers.empty()) {
        cout << "No customers registered yet.\n";
        return;
    }
    int id = readInt("Enter Customer ID to update: ");
    Customer *c = findCustomerById(id);

    if (!c) {
        cout << "Customer not found.\n";
        return;
    }

    printCustomer(*c);
    cout << "Leave a field blank to keep it unchanged.\n";

    string input;

    input = readLine("New Name [" + c->name + "]: ");
    if (!input.empty()) c->name = input;

    input = readLine("New Phone [" + c->phone + "]: ");
    if (!input.empty()) c->phone = input;

    input = readLine("New Address [" + c->address + "]: ");
    if (!input.empty()) c->address = input;

    cout << "\nCustomer updated successfully!\n";
    printCustomer(*c);
}

// ---------------------------------------------------------
// Delivery Management
// ---------------------------------------------------------

void createDelivery() {
    cout << "\n---------- CREATE DELIVERY ----------\n";
    Delivery d;

    d.senderName       = readLine("Sender Name: ");
    d.receiverName     = readLine("Receiver Name: ");
    d.pickupLocation   = readLine("Pickup Location: ");
    d.deliveryLocation = readLine("Delivery Location: ");
    d.packageType      = readLine("Package Type: ");
    d.packageWeight    = readDouble("Package Weight (kg): ");
    d.deliveryFee      = readDouble("Delivery Fee (GHC): ");
    d.deliveryDate     = readLine("Delivery Date (e.g. 2026-08-26): ");

    d.deliveryId = generateDeliveryId();
    d.status = DeliveryStatus::Pending;

    deliveries.push_back(d);

    cout << "\nDelivery created successfully!\n\n";
    cout << "Delivery ID: " << d.deliveryId << "\n";
    cout << "Status: " << statusToString(d.status) << "\n";
}

void printDeliveryShort(const Delivery &d) {
    printLine();
    cout << "Delivery ID : " << d.deliveryId << "\n";
    cout << "Sender      : " << d.senderName << "\n";
    cout << "Receiver    : " << d.receiverName << "\n";
    cout << "From        : " << d.pickupLocation << "\n";
    cout << "To          : " << d.deliveryLocation << "\n";
    cout << "Status      : " << statusToString(d.status) << "\n";
    printLine();
}

void printDeliveryFull(const Delivery &d) {
    printLine('=', 40);
    cout << "Delivery ID     : " << d.deliveryId << "\n";
    cout << "Sender          : " << d.senderName << "\n";
    cout << "Receiver        : " << d.receiverName << "\n";
    cout << "Pickup Location : " << d.pickupLocation << "\n";
    cout << "Delivery To     : " << d.deliveryLocation << "\n";
    cout << "Package Type    : " << d.packageType << "\n";
    cout << "Package Weight  : " << fixed << setprecision(1) << d.packageWeight << " kg\n";
    cout << "Delivery Fee    : GHC " << fixed << setprecision(2) << d.deliveryFee << "\n";
    cout << "Delivery Date   : " << d.deliveryDate << "\n";
    cout << "Status          : " << statusToString(d.status) << "\n";
    printLine('=', 40);
}

void viewDeliveries() {
    cout << "\n---------- ALL DELIVERIES ----------\n";
    if (deliveries.empty()) {
        cout << "No deliveries recorded yet.\n";
        return;
    }
    for (const auto &d : deliveries) {
        printDeliveryShort(d);
    }
}

Delivery* findDeliveryById(const string &id) {
    for (auto &d : deliveries) {
        if (d.deliveryId == id) return &d;
    }
    return nullptr;
}

void searchDelivery() {
    cout << "\n---------- SEARCH DELIVERY ----------\n";
    if (deliveries.empty()) {
        cout << "No deliveries recorded yet.\n";
        return;
    }
    string keyword = readLine("Enter Delivery ID, Sender, or Receiver name: ");

    bool found = false;
    for (const auto &d : deliveries) {
        if (d.deliveryId == keyword ||
            d.senderName.find(keyword) != string::npos ||
            d.receiverName.find(keyword) != string::npos) {
            printDeliveryFull(d);
            found = true;
        }
    }
    if (!found) {
        cout << "No matching delivery found.\n";
    }
}

DeliveryStatus chooseStatus() {
    cout << "\nSelect new status:\n";
    cout << "1. Pending\n";
    cout << "2. Picked Up\n";
    cout << "3. In Transit\n";
    cout << "4. Out for Delivery\n";
    cout << "5. Delivered\n";
    cout << "6. Cancelled\n";

    int choice = readInt("Enter choice: ");
    switch (choice) {
        case 1: return DeliveryStatus::Pending;
        case 2: return DeliveryStatus::PickedUp;
        case 3: return DeliveryStatus::InTransit;
        case 4: return DeliveryStatus::OutForDelivery;
        case 5: return DeliveryStatus::Delivered;
        case 6: return DeliveryStatus::Cancelled;
        default:
            cout << "Invalid choice, status left unchanged.\n";
            return DeliveryStatus::Pending; // fallback, handled by caller check
    }
}

void updateDelivery() {
    cout << "\n---------- UPDATE DELIVERY ----------\n";
    if (deliveries.empty()) {
        cout << "No deliveries recorded yet.\n";
        return;
    }
    string id = readLine("Enter Delivery ID to update: ");
    Delivery *d = findDeliveryById(id);

    if (!d) {
        cout << "Delivery not found.\n";
        return;
    }

    printDeliveryFull(*d);
    cout << "\nWhat would you like to update?\n";
    cout << "1. Package / Location Details\n";
    cout << "2. Delivery Fee\n";
    cout << "3. Delivery Status\n";
    cout << "4. Cancel (go back)\n";

    int choice = readInt("Enter choice: ");
    string input;

    switch (choice) {
        case 1:
            input = readLine("New Pickup Location [" + d->pickupLocation + "]: ");
            if (!input.empty()) d->pickupLocation = input;

            input = readLine("New Delivery Location [" + d->deliveryLocation + "]: ");
            if (!input.empty()) d->deliveryLocation = input;

            input = readLine("New Package Type [" + d->packageType + "]: ");
            if (!input.empty()) d->packageType = input;

            cout << "Details updated.\n";
            break;

        case 2:
            d->deliveryFee = readDouble("New Delivery Fee (GHC): ");
            cout << "Delivery fee updated.\n";
            break;

        case 3:
            d->status = chooseStatus();
            cout << "Status updated to: " << statusToString(d->status) << "\n";
            break;

        case 4:
            cout << "Update cancelled.\n";
            return;

        default:
            cout << "Invalid choice.\n";
            return;
    }

    cout << "\nDelivery updated successfully!\n";
    printDeliveryFull(*d);
}

void trackDelivery() {
    cout << "\n---------- TRACK DELIVERY ----------\n";
    if (deliveries.empty()) {
        cout << "No deliveries recorded yet.\n";
        return;
    }
    string id = readLine("Enter Delivery ID: ");
    Delivery *d = findDeliveryById(id);

    if (!d) {
        cout << "Delivery ID not found.\n";
        return;
    }

    printDeliveryShort(*d);
}

void deleteDelivery() {
    cout << "\n---------- DELETE DELIVERY ----------\n";
    if (deliveries.empty()) {
        cout << "No deliveries recorded yet.\n";
        return;
    }
    string id = readLine("Enter Delivery ID to delete: ");

    auto it = find_if(deliveries.begin(), deliveries.end(),
                       [&](const Delivery &d) { return d.deliveryId == id; });

    if (it == deliveries.end()) {
        cout << "Delivery not found.\n";
        return;
    }

    cout << "Are you sure you want to delete this delivery?\n";
    printDeliveryFull(*it);
    string confirm = readLine("Type 'yes' to confirm: ");

    if (confirm == "yes" || confirm == "Yes" || confirm == "YES") {
        deliveries.erase(it);
        cout << "Delivery deleted successfully.\n";
    } else {
        cout << "Deletion cancelled.\n";
    }
}

// ---------------------------------------------------------
// Menus
// ---------------------------------------------------------

void customerMenu() {
    int choice;
    do {
        cout << "\n========================================\n";
        cout << "         CUSTOMER MANAGEMENT\n";
        cout << "========================================\n";
        cout << "1. Register Customer\n";
        cout << "2. View Customers\n";
        cout << "3. Search Customer\n";
        cout << "4. Update Customer\n";
        cout << "5. Back to Main Menu\n";

        choice = readInt("Enter choice: ");

        switch (choice) {
            case 1: registerCustomer(); pause(); break;
            case 2: viewCustomers(); pause(); break;
            case 3: searchCustomer(); pause(); break;
            case 4: updateCustomer(); pause(); break;
            case 5: break;
            default: cout << "Invalid choice.\n"; pause(); break;
        }
    } while (choice != 5);
}

void deliveryMenu() {
    int choice;
    do {
        cout << "\n========================================\n";
        cout << "       DELIVERY MANAGEMENT SYSTEM\n";
        cout << "========================================\n";
        cout << "1. Create Delivery\n";
        cout << "2. View Deliveries\n";
        cout << "3. Search Delivery\n";
        cout << "4. Update Delivery\n";
        cout << "5. Track Delivery\n";
        cout << "6. Delete Delivery\n";
        cout << "7. Back to Main Menu\n";

        choice = readInt("Enter choice: ");

        switch (choice) {
            case 1: createDelivery(); pause(); break;
            case 2: viewDeliveries(); pause(); break;
            case 3: searchDelivery(); pause(); break;
            case 4: updateDelivery(); pause(); break;
            case 5: trackDelivery(); pause(); break;
            case 6: deleteDelivery(); pause(); break;
            case 7: break;
            default: cout << "Invalid choice.\n"; pause(); break;
        }
    } while (choice != 7);
}

void mainMenu() {
    int choice;
    do {
        cout << "\n========================================\n";
        cout << "     DELIVERY COMPANY MAIN MENU\n";
        cout << "========================================\n";
        cout << "1. Customer Management\n";
        cout << "2. Delivery Management\n";
        cout << "3. Exit\n";

        choice = readInt("Enter choice: ");

        switch (choice) {
            case 1: customerMenu(); break;
            case 2: deliveryMenu(); break;
            case 3: cout << "\nGoodbye!\n"; break;
            default: cout << "Invalid choice.\n"; pause(); break;
        }
    } while (choice != 3);
}

int main() {
    mainMenu();
    return 0;
}