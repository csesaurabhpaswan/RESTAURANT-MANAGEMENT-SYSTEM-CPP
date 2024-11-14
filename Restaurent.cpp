#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <ctime>

using namespace std;

class MenuItem {
public:
    string name;
    double price;

    MenuItem(string n, double p) : name(n), price(p) {}
};

class Menu {
private:
    vector<MenuItem> items;

public:
    void addMenuItem(const string &name, double price) {
        items.emplace_back(name, price);
        cout << "Added: " << name << " - $" << fixed << setprecision(2) << price << endl;
    }

    void removeMenuItem(const string &name) {
        for (auto it = items.begin(); it != items.end(); ++it) {
            if (it->name == name) {
                items.erase(it);
                cout << "Removed: " << name << endl;
                return;
            }
        }
        cout << "Item not found: " << name << endl;
    }

    void viewMenu() {
        cout << "Menu Items:\n";
        for (const auto &item : items) {
            cout << "- " << item.name << ": $" << fixed << setprecision(2) << item.price << endl;
        }
    }

    double getItemPrice(const string &name) {
        for (const auto &item : items) {
            if (item.name == name) {
                return item.price;
            }
        }
        return 0.0; // Item not found
    }

    void saveToFile(const string &filename) {
        ofstream outFile(filename);
        for (const auto &item : items) {
            outFile << item.name << "," << item.price << endl;
        }
        outFile.close();
    }

    void loadFromFile(const string &filename) {
        ifstream inFile(filename);
        string line;
        while (getline(inFile, line)) {
            size_t commaPos = line.find(',');
            string name = line.substr(0, commaPos);
            double price = stod(line.substr(commaPos + 1));
            addMenuItem(name, price);
        }
        inFile.close();
    }
};

class Table {
public:
    int number;
    bool booked;

    Table(int num) : number(num), booked(false) {}

    void book() {
        booked = true;
        cout << "Table " << number << " is now booked." << endl;
    }

    void unbook() {
        booked = false;
        cout << "Table " << number << " is unbooked." << endl;
    }
};

class Order {
private:
    vector<string> orderedItems;
    double total;
    int tableNumber; // Table number for the order

public:
    Order(int tableNum) : total(0.0), tableNumber(tableNum) {}

    void addItem(const string &item, double price) {
        orderedItems.push_back(item);
        total += price;
    }

    void viewOrder() const {
        cout << "Table " << tableNumber << " Order:\n";
        for (const auto &item : orderedItems) {
            cout << "- " << item << endl;
        }
    }

    double getTotal() const {
        return total;
    }

    int getTableNumber() const {
        return tableNumber;
    }
};


class Restaurant {
private:
    Menu menu;
    vector<Table> tables;
    vector<Order> orders;
    double totalSales;

    void logDailyIncome(double amount) {
        ofstream outFile("daily_income.txt", ios::app);
        time_t now = time(0);
        tm *ltm = localtime(&now);
        outFile << ltm->tm_year + 1900 << "-"
                << ltm->tm_mon + 1 << "-"
                << ltm->tm_mday << ": $"
                << fixed << setprecision(2) << amount << endl;
        outFile.close();
    }

public:
    Restaurant(int numTables) : totalSales(0.0) {
        for (int i = 1; i <= numTables; ++i) {
            tables.emplace_back(i);
        }
    }

    void addMenuItem(const string &name, double price) {
        menu.addMenuItem(name, price);
    }

    void removeMenuItem(const string &name) {
        menu.removeMenuItem(name);
    }

    void viewMenu() {
        menu.viewMenu();
    }

   void manageTable(int tableNumber, bool book) {
    if (tableNumber < 1 || tableNumber > tables.size()) {
        cout << "Invalid table number!" << endl;
        return;
    }

    Table &table = tables[tableNumber - 1];
    if (book) {
        if (table.booked) {
            cout << "Table " << tableNumber << " is already booked!" << endl;
        } else {
            table.book();
        }
    } else {
        if (!table.booked) {
            cout << "Table " << tableNumber << " is already unbooked!" << endl;
        } else {
            table.unbook();
        }
    }
}


    void createOrder(int tableNumber) {
    if (tableNumber < 1 || tableNumber > tables.size() || !tables[tableNumber - 1].booked) {
        cout << "Table is not booked or invalid!" << endl;
        return;
    }

    Order newOrder(tableNumber);  // Pass the table number to the Order
    string itemName;
    cout << "Enter items for the order (type 'done' to finish):" << endl;

    while (true) {
        cin >> itemName;
        if (itemName == "done") break;
        double price = menu.getItemPrice(itemName);
        if (price > 0) {
            newOrder.addItem(itemName, price);
        } else {
            cout << "Item not found: " << itemName << endl;
        }
    }

    orders.push_back(newOrder);
    totalSales += newOrder.getTotal();
    logDailyIncome(newOrder.getTotal()); // Log the daily income
    cout << "Order created for Table " << tableNumber << "!" << endl;
}

   void viewOrders() const {
    if (orders.empty()) {
        cout << "No orders have been placed yet.\n";
        return;
    }

    cout << "Orders:\n";
    for (const auto &order : orders) {
        cout << "Table " << order.getTableNumber() << ":\n";
        order.viewOrder();
        cout << "Total: $" << fixed << setprecision(2) << order.getTotal() << endl;
    }
}


    void viewTotalSales() {
        cout << "Total Sales: $" << fixed << setprecision(2) << totalSales << endl;
    }

    void saveMenuToFile(const string &filename) {
        menu.saveToFile(filename);
    }

    void loadMenuFromFile(const string &filename) {
        menu.loadFromFile(filename);
    }
};

int main() {
    Restaurant restaurant(10);                // Create restaurant with 10 tables
     system("cls");
    cout << "---------------------------------------\n";
    cout << "      Restaurant Management System     \n";
    cout << "---------------------------------------\n\n";

    int option1, option2;
    do {
        cout << "1. Admin\n";
        cout << "2. User\n";
        cout << "3. Exit\n";
        cout << "\nEnter an Option: ";
        cin >> option1;
        cin.ignore(); // Clear the newline left in the buffer

        if (option1 == 1) {
            cout << "---------------------------------------\n";
            cout << "            ADMIN DASHBOARD             \n";
            cout << "---------------------------------------\n";
            string username, password;

            cout << "Enter Username: ";
            getline(cin, username); // Read full line for username
            cout << "Enter Password: ";
            getline(cin, password); // Read full line for password


            system("cls");
            if (username == "admin" && password == "admin") {
                do {
                    
                    cout << "1. Add Menu Item\n";
                    cout << "2. Remove Menu Item\n";
                    cout << "3. View Orders\n";
                    cout << "4. View Total Sales\n";
                    cout << "5. Save Menu\n";
                    cout << "6. Exit\n";

                    cout << "Choose an option: ";
                    cin >> option2;
                    cin.ignore(); // Clear the newline left in the buffer

                    switch (option2) {
                    case 1: {
                        system("cls");

                        restaurant.loadMenuFromFile("menu.txt");
                        cout<<"\n\n";
                        string name;
                        double price;
                        cout << "Enter item name: ";
                        cin >> name;
                        cout << "Enter item price: ";
                        cin >> price;
                        restaurant.addMenuItem(name, price);
                        cin.get();
                        cout<<"\n\n";
                        break;
                    }
                    case 2: {
                        system("cls");
                        string name;
                        cout << "Enter item name to remove: ";
                        cin >> name;
                        restaurant.removeMenuItem(name);
                        cin.get();
                         cout<<"\n\n";
                        break;
                    }
                    case 3:{
                        system("cls");
                        restaurant.viewOrders();
                        cin.get();
                        cout<<"\n\n";
                        break;}
                    case 4:{
                        restaurant.viewTotalSales();
                        cin.get();
                         cout<<"\n\n";
                        break;}
                    case 5:{
                        system("cls");
                        restaurant.saveMenuToFile("menu.txt");
                        cout << "Menu saved to file." << endl;
                        cin.get();
                         cout<<"\n\n";
                        break;}
                    case 6:
                        cout << "Exiting the system." << endl;
                        break;
                    default:
                        cout << "Invalid option!\n";
                        break;
                    }

                } while (option2 != 6);
            } else {
                cout << "Invalid Username or Password!\n";
            }
        } else if (option1 == 2) {
            cout << "---------------------------------------\n";
            cout << "             USER DASHBOARD           \n";
            cout << "---------------------------------------\n";

            do {
                cout << "1. Manage Table \n";
                cout << "2. View Menu\n";
                cout << "3. Create Order\n";
                cout << "4. Exit\n";

                cout << "Choose an Option: ";
                cin >> option2;
                cin.ignore(); // Clear the newline left in the buffer

                switch (option2) {
                case 1: {
                    system("cls");
                    int tableNumber;
                    cout << "Enter table number: ";
                    cin >> tableNumber;
                    cin.ignore(); // Clear the newline left in the buffer
                    char action;
                    cout << "Book (b) or Unbook (u)? ";
                    cin >> action;
                    restaurant.manageTable(tableNumber, action == 'b');
                     cout<<"\n\n";
                    break;
                }
                case 2:
                system("cls");
                    restaurant.viewMenu();
                    cin.get();
                     cout<<"\n\n";
                    break;
                case 3: {

                    system("cls");
                    int tableNumber;
                    cout << "Enter table number for order: ";
                    cin >> tableNumber;
                    restaurant.createOrder(tableNumber);
                    cin.get();
                     cout<<"\n\n";
                    break;
                }
                case 4:
                    cout << "Exiting the system." << endl;
                    break;
                default:
                    cout << "Invalid option!\n";
                    break;
                }

            } while (option2 != 4);
        }

    } while (option1 != 3);

    return 0;
}
