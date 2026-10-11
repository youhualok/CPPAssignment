#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <cctype>
#include <limits>

using namespace std;

struct Order {
    string item;
    int quantity = 0;
    double total_price = 0.0;
    double price = 0.0;
};

struct CustomerAccount {
    string username = "";
    string name = "";
    string phone = "";
    string email = "";
    string password = "";
    bool isAdmin = false;
    bool isRegistered = false;
};

const int MAX_ORDERS = 100;
const int MAX_TOTAL_ORDERS = 1000;
const int MAX_ACCOUNTS = 50;
const int MAX_BOOKS = 100;
const int UI_WIDTH = 83; // Standardized display width across all screens

// Dynamic Store Inventory Data
string ids[MAX_BOOKS] = { "B001", "B002", "B003", "B004", "B005", "B006", "B007", "B008", "B009", "B010" };
string items[MAX_BOOKS] = {
    "The Great Gatsby", "To Kill a Mockingbird", "1984", "The Hobbit", "Harry Potter",
    "The Catcher in the Rye", "Pride and Prejudice", "The Lord of the Rings", "The Alchemist", "Atomic Habits"
};
double prices[MAX_BOOKS] = { 45.00, 40.00, 35.00, 50.00, 60.00, 38.00, 42.00, 70.00, 30.00, 55.00 };
int stock[MAX_BOOKS] = { 15, 10, 20, 8, 12, 15, 10, 5, 25, 18 };
int activeBookCount = 10;

// Cross-platform screen clearing
void ClearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Clear input stream on invalid user entries
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Helper to pause screen safely
void pauseScreen() {
    cout << "\n  Press Enter to continue...";
    clearInputBuffer();
    cin.get();
}

// Validation Helpers
bool isValidPIN(const string& pin) {
    if (pin.length() != 6) return false;
    for (char c : pin) {
        if (!isdigit(c)) return false;
    }
    return true;
}

bool isValidPhoneNumber(const string& phone) {
    if (phone.length() < 10) return false;
    for (char c : phone) {
        if (!isdigit(c)) return false;
    }
    return true;
}

bool isValidPassword(const string& password) {
    return password.length() >= 4;
}

// ASCII Logo for BOOKHAVEN (Unchanged)
void displaylogo() {
    ClearScreen();
    cout << R"(
=================================================================================
 ____    ____    ____   _  __  _   _    _  __   ___ _____  _   _ 
|  _ \  / __ \  / __ \ | |/ / | |_| |  / \ \ \ /  /| ____|| \ | |
| |_) || |  | || |  | || ' /  |  _  | / _ \ \ V  / |  _|  |  \| |
|  _ < | |__| || |__| || . \  | | | |/ ___ \ \  /  | |___ | |\  |
|____/  \____/  \____/ |_|\_\ |_| |_/_/   \_\ \/   |_____||_| \_|
=================================================================================
                 Your One-Stop Bookstore for All Bestsellers!
=================================================================================

+---------------------------------------------------------------------------------+
|                       [>] PRESS ENTER TO START SYSTEM                           |
+---------------------------------------------------------------------------------+
)";
    cin.get(); // Wait for user key press
}

void displaymenu(int loggedInIndex, const CustomerAccount accounts[]) {
    cout << "+---------------------------------------------------------------------------------+\n";
    cout << "|                                MAIN SYSTEM MENU                                 |\n";
    cout << "+---------------------------------------------------------------------------------+\n";
    if (loggedInIndex != -1) {
        string roleTag = accounts[loggedInIndex].isAdmin ? "ADMIN" : "CUSTOMER";
        string statusText = "Account Status: LOGGED IN [" + roleTag + "] (Welcome, " + accounts[loggedInIndex].name + "!)";
        if (statusText.length() > 78) statusText = statusText.substr(0, 75) + "...";
        cout << "|  " << left << setw(78) << statusText << "|\n";
    } else {
        cout << "|  Account Status: NOT LOGGED IN (Login or Register required to proceed)          |\n";
    }
    cout << "+---------------------------------------------------------------------------------+\n";
    cout << "|  [1] Login to Account                                                           |\n";
    cout << "|  [2] Register New Customer Account                                              |\n";
    cout << "|  [3] View Account Details & Profile                                             |\n";
    cout << "|  [4] Order Books & Checkout                                                     |\n";
    cout << "|  [5] Manage Inventory (ADMIN ONLY)                                              |\n";
    cout << "|  [6] View Daily Sales & Revenue Report                                          |\n";
    cout << "|  [7] Logout Account                                                             |\n";
    cout << "|  [8] Exit System                                                                |\n";
    cout << "+---------------------------------------------------------------------------------+\n";
}

bool loginCustomer(const CustomerAccount accounts[], int accountCount, int& loggedInIndex) {
    ClearScreen();
    cout << "+---------------------------------------------------------------------------------+\n";
    cout << "|                             CUSTOMER ACCOUNT LOGIN                              |\n";
    cout << "+---------------------------------------------------------------------------------+\n\n";

    if (loggedInIndex != -1) {
        cout << "  [i] You are currently logged in as: " << accounts[loggedInIndex].username << "\n";
        cout << "      Please logout (Option [7]) first if you wish to switch accounts.\n\n";
        pauseScreen();
        return true;
    }

    string inputUser, inputPass;
    cout << "  Enter Username : ";
    cin >> inputUser;
    cout << "  Enter Password : ";
    cin >> inputPass;

    for (int i = 0; i < accountCount; ++i) {
        if (accounts[i].username == inputUser && accounts[i].password == inputPass) {
            loggedInIndex = i;
            string roleStr = accounts[i].isAdmin ? "ADMINISTRATOR" : "CUSTOMER";
            cout << "\n+---------------------------------------------------------------------------------+\n";
            cout << "|                         LOGIN SUCCESSFUL! WELCOME BACK                         |\n";
            cout << "+---------------------------------------------------------------------------------+\n";
            cout << "| Account  : " << left << setw(68) << accounts[i].username << " |\n";
            cout << "| Name     : " << left << setw(68) << accounts[i].name << " |\n";
            cout << "| Role     : " << left << setw(68) << roleStr << " |\n";
            cout << "+---------------------------------------------------------------------------------+\n";
            pauseScreen();
            return true;
        }
    }

    cout << "\n  [!] Invalid Username or Password! Please try again.\n";
    pauseScreen();
    return false;
}

void registerCustomer(CustomerAccount accounts[], int& accountCount, int& loggedInIndex) {
    ClearScreen();
    cout << "+---------------------------------------------------------------------------------+\n";
    cout << "|                          CUSTOMER ACCOUNT REGISTRATION                          |\n";
    cout << "+---------------------------------------------------------------------------------+\n\n";

    if (accountCount >= MAX_ACCOUNTS) {
        cout << "  [!] Account database full! Cannot register new users.\n";
        pauseScreen();
        return;
    }

    CustomerAccount newAccount;
    clearInputBuffer();

    while (true) {
        cout << "  Enter Desired Username   : ";
        cin >> newAccount.username;

        bool duplicate = false;
        for (int i = 0; i < accountCount; ++i) {
            if (accounts[i].username == newAccount.username) {
                duplicate = true;
                break;
            }
        }
        if (duplicate) {
            cout << "  [!] Username already exists! Please choose another username.\n\n";
        } else if (newAccount.username.empty()) {
            cout << "  [!] Username cannot be empty.\n\n";
        } else {
            break;
        }
    }

    clearInputBuffer();
    cout << "  Enter Full Name          : ";
    getline(cin, newAccount.name);

    while (newAccount.name.empty()) {
        cout << "  [!] Name cannot be empty! Enter Full Name: ";
        getline(cin, newAccount.name);
    }

    do {
        cout << "  Enter Phone (min 10 digits): ";
        cin >> newAccount.phone;
        if (!isValidPhoneNumber(newAccount.phone)) {
            cout << "  [!] Invalid phone number. Must contain at least 10 numerical digits.\n";
        }
    } while (!isValidPhoneNumber(newAccount.phone));

    cout << "  Enter Email Address      : ";
    cin >> newAccount.email;

    string confirmPass;
    do {
        cout << "  Create Password (min 4 chars): ";
        cin >> newAccount.password;
        if (!isValidPassword(newAccount.password)) {
            cout << "  [!] Password too short! Must be at least 4 characters long.\n";
            continue;
        }

        cout << "  Confirm Password             : ";
        cin >> confirmPass;

        if (newAccount.password != confirmPass) {
            cout << "  [!] Passwords do not match! Please try again.\n\n";
        }
    } while (!isValidPassword(newAccount.password) || newAccount.password != confirmPass);

    newAccount.isAdmin = false;
    newAccount.isRegistered = true;
    accounts[accountCount] = newAccount;
    loggedInIndex = accountCount;
    accountCount++;

    cout << "\n+---------------------------------------------------------------------------------+\n";
    cout << "|                        REGISTRATION COMPLETED SUCCESSFULLY                      |\n";
    cout << "+---------------------------------------------------------------------------------+\n";
    cout << "| Username: " << left << setw(69) << newAccount.username << " |\n";
    cout << "| Name    : " << left << setw(69) << newAccount.name << " |\n";
    cout << "| Role    : CUSTOMER                                                              |\n";
    cout << "+---------------------------------------------------------------------------------+\n";
    cout << "|  [✔] Account created and logged in! You can now proceed to place orders.        |\n";
    cout << "+---------------------------------------------------------------------------------+\n";
    pauseScreen();
}

void viewAccountDetails(int loggedInIndex, const CustomerAccount accounts[]) {
    ClearScreen();
    cout << "+---------------------------------------------------------------------------------+\n";
    cout << "|                            CUSTOMER ACCOUNT PROFILE                             |\n";
    cout << "+---------------------------------------------------------------------------------+\n\n";

    if (loggedInIndex == -1) {
        cout << "  [!] Access Denied! No user is currently logged in.\n";
        cout << "      Please select Option [1] to Login or Option [2] to Register first.\n\n";
        pauseScreen();
        return;
    }

    const CustomerAccount& activeAccount = accounts[loggedInIndex];
    string maskedPass(activeAccount.password.length(), '*');
    string roleStr = activeAccount.isAdmin ? "ADMINISTRATOR" : "CUSTOMER";

    ClearScreen();
    cout << "+---------------------------------------------------------------------------------+\n";
    cout << "|                            CUSTOMER ACCOUNT PROFILE                             |\n";
    cout << "+---------------------------------------------------------------------------------+\n";
    cout << "| Username  : " << left << setw(67) << activeAccount.username << " |\n";
    cout << "| Full Name : " << left << setw(67) << activeAccount.name << " |\n";
    cout << "| Phone No. : " << left << setw(67) << activeAccount.phone << " |\n";
    cout << "| Email Addr: " << left << setw(67) << activeAccount.email << " |\n";
    cout << "| Role      : " << left << setw(67) << roleStr << " |\n";
    cout << "| Password  : " << left << setw(67) << maskedPass << " |\n";
    cout << "+---------------------------------------------------------------------------------+\n";
    cout << "| Account Status: ACTIVE & LOGGED IN                                              |\n";
    cout << "+---------------------------------------------------------------------------------+\n";
    pauseScreen();
}

void displayOrderMenu() {
    cout << "+---------------------------------------------------------------------------------+\n";
    cout << "|                               BOOKSTORE INVENTORY                               |\n";
    cout << "+-------+-------------------------------------------------+---------------+-------+\n";
    cout << "| ID    | BOOK TITLE                                      | PRICE (RM)    | STOCK |\n";
    cout << "+-------+-------------------------------------------------+---------------+-------+\n";
    for (int i = 0; i < activeBookCount; i++) {
        string stockDisplay = (stock[i] == 0) ? "OUT" : to_string(stock[i]);
        cout << "| " << left << setw(5) << ids[i]
             << " | " << setw(47) << items[i]
             << " | RM " << setw(8) << fixed << setprecision(2) << right << prices[i] << "   "
             << " | " << setw(5) << right << stockDisplay << " |\n";
    }
    cout << "+-------+-------------------------------------------------+---------------+-------+\n";
}

void addBook() {
    ClearScreen();
    cout << "+---------------------------------------------------------------------------------+\n";
    cout << "|                              ADD NEW BOOK TITLE                                 |\n";
    cout << "+---------------------------------------------------------------------------------+\n\n";

    if (activeBookCount >= MAX_BOOKS) {
        cout << "  [!] Inventory capacity full! Cannot add more books.\n";
        pauseScreen();
        return;
    }

    string newID, newTitle;
    double newPrice;
    int newStock;

    cout << "  Enter New Book ID (e.g. B011) : ";
    cin >> newID;

    for (int i = 0; i < activeBookCount; ++i) {
        if (ids[i] == newID) {
            cout << "  [!] Duplicate Book ID! ID \"" << newID << "\" already exists.\n";
            pauseScreen();
            return;
        }
    }

    clearInputBuffer();
    cout << "  Enter Book Title              : ";
    getline(cin, newTitle);

    while (newTitle.empty()) {
        cout << "  [!] Title cannot be empty! Enter Book Title: ";
        getline(cin, newTitle);
    }

    cout << "  Enter Unit Price (RM)         : ";
    while (!(cin >> newPrice) || newPrice <= 0) {
        cout << "  [!] Invalid price! Enter a positive amount: ";
        clearInputBuffer();
    }

    cout << "  Enter Initial Stock Quantity  : ";
    while (!(cin >> newStock) || newStock < 0) {
        cout << "  [!] Invalid stock! Enter a non-negative whole number: ";
        clearInputBuffer();
    }

    ids[activeBookCount] = newID;
    items[activeBookCount] = newTitle;
    prices[activeBookCount] = newPrice;
    stock[activeBookCount] = newStock;
    activeBookCount++;

    cout << "\n  [✔] Book \"" << newTitle << "\" (ID: " << newID << ") added successfully!\n";
    pauseScreen();
}

void editBook() {
    ClearScreen();
    cout << "+---------------------------------------------------------------------------------+\n";
    cout << "|                             EDIT EXISTING BOOK                                  |\n";
    cout << "+---------------------------------------------------------------------------------+\n\n";

    string editID;
    cout << "  Enter Book ID to Edit: ";
    cin >> editID;

    int idx = -1;
    for (int i = 0; i < activeBookCount; ++i) {
        if (ids[i] == editID) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        cout << "  [!] Book ID \"" << editID << "\" not found in inventory!\n";
        pauseScreen();
        return;
    }

    cout << "\n  Currently Editing: [" << ids[idx] << "] " << items[idx] 
         << " (RM " << fixed << setprecision(2) << prices[idx] << " | Stock: " << stock[idx] << ")\n\n";

    cout << "  Select Field to Edit:\n";
    cout << "  [1] Update Title\n";
    cout << "  [2] Update Unit Price\n";
    cout << "  [3] Update Stock Quantity\n";
    cout << "  Enter Choice (1-3): ";

    int choice;
    if (cin >> choice) {
        if (choice == 1) {
            clearInputBuffer();
            cout << "  Enter New Title: ";
            getline(cin, items[idx]);
            cout << "  [✔] Title updated successfully!\n";
        } else if (choice == 2) {
            cout << "  Enter New Price (RM): ";
            while (!(cin >> prices[idx]) || prices[idx] <= 0) {
                cout << "  [!] Invalid price! Enter again: ";
                clearInputBuffer();
            }
            cout << "  [✔] Price updated successfully!\n";
        } else if (choice == 3) {
            cout << "  Enter New Stock Quantity: ";
            while (!(cin >> stock[idx]) || stock[idx] < 0) {
                cout << "  [!] Invalid stock! Enter again: ";
                clearInputBuffer();
            }
            cout << "  [✔] Stock updated successfully!\n";
        } else {
            cout << "  [!] Invalid choice! No changes made.\n";
        }
    } else {
        clearInputBuffer();
        cout << "  [!] Invalid input!\n";
    }
    pauseScreen();
}

void deleteBook() {
    ClearScreen();
    cout << "+---------------------------------------------------------------------------------+\n";
    cout << "|                            DELETE BOOK FROM INVENTORY                           |\n";
    cout << "+---------------------------------------------------------------------------------+\n\n";

    string delID;
    cout << "  Enter Book ID to Delete: ";
    cin >> delID;

    int idx = -1;
    for (int i = 0; i < activeBookCount; ++i) {
        if (ids[i] == delID) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        cout << "  [!] Book ID \"" << delID << "\" not found in inventory!\n";
        pauseScreen();
        return;
    }

    char confirm;
    cout << "  Are you sure you want to delete \"" << items[idx] << "\" (ID: " << ids[idx] << ")? (Y/N): ";
    cin >> confirm;

    if (toupper(confirm) == 'Y') {
        string removedTitle = items[idx];
        for (int i = idx; i < activeBookCount - 1; ++i) {
            ids[i] = ids[i + 1];
            items[i] = items[i + 1];
            prices[i] = prices[i + 1];
            stock[i] = stock[i + 1];
        }
        activeBookCount--;
        cout << "  [✔] Book \"" << removedTitle << "\" successfully removed from inventory!\n";
    } else {
        cout << "  [!] Deletion cancelled.\n";
    }
    pauseScreen();
}

void manageInventory(int loggedInIndex, const CustomerAccount accounts[]) {
    if (loggedInIndex == -1 || !accounts[loggedInIndex].isAdmin) {
        ClearScreen();
        cout << "+---------------------------------------------------------------------------------+\n";
        cout << "| [!] ACCESS DENIED: Administrator privileges required for this action!           |\n";
        cout << "+---------------------------------------------------------------------------------+\n";
        pauseScreen();
        return;
    }

    while (true) {
        ClearScreen();
        cout << "+---------------------------------------------------------------------------------+\n";
        cout << "|                          ADMIN INVENTORY MANAGEMENT                             |\n";
        cout << "+---------------------------------------------------------------------------------+\n";
        cout << "|  [1] View Full Inventory & Stock Status                                         |\n";
        cout << "|  [2] Add New Book Title                                                         |\n";
        cout << "|  [3] Edit Existing Book Details (Title / Price / Stock)                         |\n";
        cout << "|  [4] Delete Book Title from Inventory                                           |\n";
        cout << "|  [5] Return to Main Menu                                                        |\n";
        cout << "+---------------------------------------------------------------------------------+\n";

        int subChoice;
        cout << "\n  Enter your choice (1-5): ";
        if (!(cin >> subChoice)) {
            clearInputBuffer();
            continue;
        }

        switch (subChoice) {
        case 1:
            ClearScreen();
            displayOrderMenu();
            pauseScreen();
            break;
        case 2:
            addBook();
            break;
        case 3:
            editBook();
            break;
        case 4:
            deleteBook();
            break;
        case 5:
            return;
        default:
            cout << "  [!] Invalid choice. Select 1 to 5.\n";
            pauseScreen();
            break;
        }
    }
}

bool processOrder(Order orders[], int& itemCount, double& Subtotal) {
    string id;
    int lineNo = 1;
    bool hasOrders = false;

    cout << "\n+---------------------------------------------------------------------------------+\n";
    cout << "| QUICK CONTROLS:  Type 'D' to Finish & Checkout  |  Type 'E' to Cancel Order     |\n";
    cout << "+---------------------------------------------------------------------------------+\n\n";

    while (true) {
        if (itemCount >= MAX_ORDERS) {
            cout << "  [!] Maximum item limit reached for this order session.\n";
            break;
        }

        cout << "  Item #" << lineNo << " -> Enter Book ID: ";
        cin >> id;

        if (id == "E" || id == "e") {
            cout << "\n  [!] Order cancelled by user.\n";
            pauseScreen();
            return false;
        }

        if (id == "D" || id == "d") {
            if (!hasOrders) {
                cout << "  [!] Cart is empty! Please add at least one book or enter 'E' to cancel.\n\n";
                continue;
            }
            return true;
        }

        bool found = false;
        for (int j = 0; j < activeBookCount; j++) {
            if (id == ids[j]) {
                found = true;

                if (stock[j] <= 0) {
                    cout << "         [!] OUT OF STOCK! \"" << items[j] << "\" is currently unavailable.\n\n";
                    break;
                }

                int alreadyInCart = 0;
                for (int k = 0; k < itemCount; ++k) {
                    if (orders[k].item == items[j]) {
                        alreadyInCart += orders[k].quantity;
                    }
                }

                int availableStock = stock[j] - alreadyInCart;
                if (availableStock <= 0) {
                    cout << "         [!] Stock limit reached! You already have " << alreadyInCart 
                         << " unit(s) of this title in your cart (Max available: " << stock[j] << ").\n\n";
                    break;
                }

                int qty;
                cout << "         Enter Quantity for \"" << items[j] << "\" (Stock: " << availableStock << "): ";
                if (!(cin >> qty) || qty <= 0) {
                    cout << "         [!] Invalid quantity. Please enter a positive whole number.\n\n";
                    clearInputBuffer();
                    break;
                }

                if (qty > availableStock) {
                    cout << "         [!] Insufficient stock! Only " << availableStock 
                         << " unit(s) remaining for \"" << items[j] << "\".\n\n";
                    break;
                }

                orders[itemCount].item = items[j];
                orders[itemCount].price = prices[j];
                orders[itemCount].quantity = qty;
                orders[itemCount].total_price = orders[itemCount].price * qty;

                Subtotal += orders[itemCount].total_price;

                hasOrders = true;
                itemCount++;
                lineNo++;
                cout << "  [✔] Added: " << qty << "x \"" << items[j] 
                     << "\" (RM " << fixed << setprecision(2) << orders[itemCount - 1].total_price << ")\n";
                cout << "      ---> Current Cart Subtotal: RM " << fixed << setprecision(2) << Subtotal << "\n\n";
                break;
            }
        }
        if (!found) {
            cout << "  [!] Invalid Book ID! Please check the inventory table above.\n\n";
        }
    }
    return hasOrders;
}

void deductStock(const Order orders[], int itemCount) {
    for (int i = 0; i < itemCount; ++i) {
        for (int j = 0; j < activeBookCount; ++j) {
            if (orders[i].item == items[j]) {
                stock[j] -= orders[i].quantity;
                if (stock[j] < 0) stock[j] = 0;
                break;
            }
        }
    }
}

void displayInvoice(const Order orders[], int itemCount, double Subtotal, double Discount, double S_Charge, double Tax, double Total_Pay, int customer, const CustomerAccount& account) {
    ClearScreen();
    cout << "+---------------------------------------------------------------------------------+\n";
    cout << "|                               BOOKHAVEN BOOKSTORE                               |\n";
    cout << "|                             PRE-PAYMENT TAX INVOICE                             |\n";
    cout << "|                          Branch: BH001 - Kuala Selangor                         |\n";
    cout << "+---------------------------------------------------------------------------------+\n";
    cout << "| Invoice #: BH-" << left << setw(10) << customer 
         << setw(55) << right << "Status: UNPAID" << " |\n";
    cout << "| Customer : " << left << setw(67) << account.name << " |\n";
    cout << "| Username : " << left << setw(67) << account.username << " |\n";
    cout << "| Phone    : " << left << setw(67) << account.phone << " |\n";
    cout << "+--------------------------------------------+-------+-------------+--------------+\n";
    cout << "| BOOK TITLE                                 | QTY   | UNIT PRICE  | TOTAL (RM)   |\n";
    cout << "+--------------------------------------------+-------+-------------+--------------+\n";
    for (int j = 0; j < itemCount; ++j) {
        cout << "| " << left  << setw(42) << orders[j].item
             << " | " << right << setw(5)  << orders[j].quantity
             << " | RM " << setw(8) << fixed << setprecision(2) << orders[j].price
             << " | RM " << setw(9) << fixed << setprecision(2) << orders[j].total_price
             << " |\n";
    }
    cout << "+--------------------------------------------+-------+-------------+--------------+\n";
    cout << "| Subtotal:                                                         | RM " << right << setw(9) << fixed << setprecision(2) << Subtotal << " |\n";
    if (Discount > 0) {
        cout << "| Promotional Discount (10% off >= RM150):                          | -RM" << right << setw(8) << fixed << setprecision(2) << Discount << "  |\n";
    }
    cout << "| Service Charge (10%):                                             | RM " << right << setw(9) << fixed << setprecision(2) << S_Charge << " |\n";
    cout << "| Government SST (6%):                                              | RM " << right << setw(9) << fixed << setprecision(2) << Tax << " |\n";
    cout << "+-------------------------------------------------------------------+--------------+\n";
    cout << "| TOTAL AMOUNT DUE:                                                 | RM " << right << setw(9) << fixed << setprecision(2) << Total_Pay << " |\n";
    cout << "+-------------------------------------------------------------------+--------------+\n";
}

void displayReceipt(const Order orders[], int itemCount, double Subtotal, double Discount, double S_Charge, double Tax, double Total_Pay, double Pay_Amt, double change, const string& typepay, int customer, const CustomerAccount& account) {
    ClearScreen();
    cout << "+---------------------------------------------------------------------------------+\n";
    cout << "|                               BOOKHAVEN BOOKSTORE                               |\n";
    cout << "|                             OFFICIAL PAYMENT RECEIPT                            |\n";
    cout << "|                          Branch: BH001 - Kuala Selangor                         |\n";
    cout << "+---------------------------------------------------------------------------------+\n";
    cout << "| Receipt #: BH-" << left << setw(10) << customer 
         << setw(55) << right << "Status: PAID" << " |\n";
    cout << "| Customer : " << left << setw(67) << account.name << " |\n";
    cout << "| Username : " << left << setw(67) << account.username << " |\n";
    cout << "| Phone    : " << left << setw(67) << account.phone << " |\n";
    cout << "+--------------------------------------------+-------+-------------+--------------+\n";
    cout << "| BOOK TITLE                                 | QTY   | UNIT PRICE  | TOTAL (RM)   |\n";
    cout << "+--------------------------------------------+-------+-------------+--------------+\n";
    for (int j = 0; j < itemCount; ++j) {
        cout << "| " << left  << setw(42) << orders[j].item
             << " | " << right << setw(5)  << orders[j].quantity
             << " | RM " << setw(8) << fixed << setprecision(2) << orders[j].price
             << " | RM " << setw(9) << fixed << setprecision(2) << orders[j].total_price
             << " |\n";
    }
    cout << "+--------------------------------------------+-------+-------------+--------------+\n";
    cout << "| Subtotal:                                                         | RM " << right << setw(9) << fixed << setprecision(2) << Subtotal << " |\n";
    if (Discount > 0) {
        cout << "| Discount Applied:                                                 | -RM" << right << setw(8) << fixed << setprecision(2) << Discount << "  |\n";
    }
    cout << "| Service Charge (10%):                                             | RM " << right << setw(9) << fixed << setprecision(2) << S_Charge << " |\n";
    cout << "| SST Tax (6%):                                                     | RM " << right << setw(9) << fixed << setprecision(2) << Tax << " |\n";
    cout << "| Total Amount:                                                     | RM " << right << setw(9) << fixed << setprecision(2) << Total_Pay << " |\n";
    cout << "+-------------------------------------------------------------------+--------------+\n";
    
    string paymentInfo = "Payment Method (" + typepay + ")";
    cout << "| " << left << setw(65) << paymentInfo << "| RM " << right << setw(9) << fixed << setprecision(2) << Pay_Amt << " |\n";
    cout << "| Change Returned:                                                  | RM " << right << setw(9) << fixed << setprecision(2) << change << " |\n";
    cout << "+-------------------------------------------------------------------+--------------+\n";
    cout << "|                        Thank you for shopping at BOOKHAVEN!                      |\n";
    cout << "+---------------------------------------------------------------------------------+\n";
}

void resetOrders(Order orders[], int& orderCount) {
    for (int i = 0; i < orderCount; ++i) {
        orders[i].item = "";
        orders[i].quantity = 0;
        orders[i].total_price = 0.0;
        orders[i].price = 0.0;
    }
    orderCount = 0;
}

void report(const Order orders[], int totalOrderCount) {
    double totalSales = 0.0;
    int totalQuantities = 0;
    ClearScreen();

    int quantities[MAX_BOOKS] = { 0 };
    double totalPrices[MAX_BOOKS] = { 0.0 };

    for (int i = 0; i < totalOrderCount; ++i) {
        for (int j = 0; j < activeBookCount; ++j) {
            if (orders[i].item == items[j]) {
                quantities[j] += orders[i].quantity;
                totalPrices[j] += orders[i].total_price;
                break;
            }
        }
    }

    cout << "+---------------------------------------------------------------------------------+\n";
    cout << "|                           BOOKHAVEN DAILY SALES REPORT                          |\n";
    cout << "+-----+------------------------------------+------------+----------+--------------+\n";
    cout << "| #   | BOOK TITLE                         | UNIT PRICE | QTY SOLD | REVENUE (RM) |\n";
    cout << "+-----+------------------------------------+------------+----------+--------------+\n";

    for (int j = 0; j < activeBookCount; j++) {
        cout << "| " << right << setw(3) << j + 1 << " | "
             << left  << setw(34) << items[j] << " | RM "
             << right << setw(7) << fixed << setprecision(2) << prices[j] << " | "
             << setw(8) << quantities[j] << " | RM "
             << setw(9) << fixed << setprecision(2) << totalPrices[j] << " |\n";

        totalSales += totalPrices[j];
        totalQuantities += quantities[j];
    }

    cout << "+-----+------------------------------------+------------+----------+--------------+\n";
    cout << "| TOTAL SUMMARY (ALL BOOKS COMBINED)                    | "
         << right << setw(8) << totalQuantities << " | RM "
         << setw(9) << fixed << setprecision(2) << totalSales << " |\n";
    cout << "+-------------------------------------------------------+----------+--------------+\n\n";
    pauseScreen();
}

int main() {
    Order current_orders[MAX_ORDERS];
    int currentOrderCount = 0;
    Order total_orders[MAX_TOTAL_ORDERS];
    int totalOrderCount = 0;

    // Accounts Database initialization
    CustomerAccount accounts[MAX_ACCOUNTS];
    int accountCount = 1;

    // Pre-loaded Default Admin Account (youhua / 123456)
    accounts[0].username = "youhua";
    accounts[0].name = "Youhua";
    accounts[0].phone = "0123456789";
    accounts[0].email = "youhua@bookhaven.com";
    accounts[0].password = "123456";
    accounts[0].isAdmin = true;
    accounts[0].isRegistered = true;

    int loggedInIndex = -1; // -1 indicates no active login session
    int customerCounter = 1000;

    // STEP 1: Display Splash Screen Logo ONCE on program start
    displaylogo();

    // STEP 2: Main System Loop
    while (true) {
        ClearScreen();
        displaymenu(loggedInIndex, accounts);

        int choice;
        cout << "\n  Enter your choice (1-8): ";
        if (!(cin >> choice)) {
            cout << "  [!] Invalid input! Please enter a number between 1 and 8.\n";
            clearInputBuffer();
            pauseScreen();
            continue;
        }

        switch (choice) {
        case 1:
            loginCustomer(accounts, accountCount, loggedInIndex);
            break;

        case 2:
            registerCustomer(accounts, accountCount, loggedInIndex);
            break;

        case 3:
            viewAccountDetails(loggedInIndex, accounts);
            break;

        case 4: {
            if (loggedInIndex == -1) {
                cout << "\n+---------------------------------------------------------------------------------+\n";
                cout << "| [!] ACCESS DENIED: You must login or register an account before ordering!      |\n";
                cout << "|     Select Option [1] to Login or Option [2] to Register on the Main Menu.     |\n";
                cout << "+---------------------------------------------------------------------------------+\n";
                pauseScreen();
                break;
            }

            const CustomerAccount& activeUser = accounts[loggedInIndex];
            bool keepOrdering = true;

            while (keepOrdering) {
                resetOrders(current_orders, currentOrderCount);
                double Subtotal = 0.0, Discount = 0.0, S_Charge = 0.0, Tax = 0.0, Total_Pay = 0.0, Pay_Amt = 0.0, change = 0.0;
                
                ClearScreen();
                displayOrderMenu();
                int currentCustomerID = customerCounter + 1;
                cout << "\n  [::] CUSTOMER SESSION ID: #" << currentCustomerID << " | " << activeUser.name << " (@" << activeUser.username << ")\n";

                if (!processOrder(current_orders, currentOrderCount, Subtotal)) {
                    break;
                }

                customerCounter++;

                if (Subtotal >= 150.0) {
                    Discount = 0.10 * Subtotal;
                    cout << "\n  [★] PROMO APPLIED: 10% Discount for purchases over RM150.00!\n";
                }

                double taxableSubtotal = Subtotal - Discount;
                S_Charge = 0.10 * taxableSubtotal;
                Tax = 0.06 * taxableSubtotal;
                Total_Pay = taxableSubtotal + S_Charge + Tax;

                displayInvoice(current_orders, currentOrderCount, Subtotal, Discount, S_Charge, Tax, Total_Pay, customerCounter, activeUser);

                int Paytype = 0;
                string typepay;
                string pin;

                cout << "\n+---------------------------------------------------------------------------------+\n";
                cout << "| PAYMENT OPTIONS: [1] E-Wallet   [2] Credit Card   [3] Debit Card   [4] Cash     |\n";
                cout << "+---------------------------------------------------------------------------------+\n";

                while (true) {
                    cout << "  Select Payment Type (1-4): ";
                    if (cin >> Paytype && (Paytype >= 1 && Paytype <= 4)) {
                        break;
                    }
                    cout << "  [!] Invalid choice! Please select 1, 2, 3, or 4.\n";
                    clearInputBuffer();
                }

                switch (Paytype) {
                case 1:
                    typepay = "E-Wallet";
                    cout << "\n  [E-Wallet Selected]\n";
                    cout << "  [✔] Verified Registered Mobile: " << activeUser.phone << "\n";
                    Pay_Amt = Total_Pay;
                    cout << "  [✔] E-Wallet Payment Successful!\n";
                    pauseScreen();
                    break;

                case 2:
                case 3:
                    typepay = (Paytype == 2) ? "Credit Card" : "Debit Card";
                    cout << "\n  [" << typepay << " Selected]\n";
                    do {
                        cout << "  Enter 6-Digit PIN: ";
                        cin >> pin;
                        if (!isValidPIN(pin)) {
                            cout << "  [!] Invalid PIN. Must be exactly 6 numerical digits.\n";
                        }
                    } while (!isValidPIN(pin));
                    Pay_Amt = Total_Pay;
                    cout << "  [✔] PIN Verified. Payment Processing Successful!\n";
                    pauseScreen();
                    break;

                case 4:
                    typepay = "Cash";
                    cout << "\n  [Cash Payment Selected]\n";
                    cout << "  Enter Amount Received (RM): ";
                    while (!(cin >> Pay_Amt) || Pay_Amt < Total_Pay) {
                        if (cin.fail()) {
                            cout << "  [!] Invalid numerical input. Enter amount again: ";
                            clearInputBuffer();
                        } else {
                            cout << "  [!] Insufficient amount! Minimum required is RM " 
                                 << fixed << setprecision(2) << Total_Pay << ". Enter again: ";
                        }
                    }
                    break;
                }

                change = Pay_Amt - Total_Pay;
                
                deductStock(current_orders, currentOrderCount);

                displayReceipt(current_orders, currentOrderCount, Subtotal, Discount, S_Charge, Tax, Total_Pay, Pay_Amt, change, typepay, customerCounter, activeUser);

                for (int i = 0; i < currentOrderCount; ++i) {
                    if (totalOrderCount < MAX_TOTAL_ORDERS) {
                        total_orders[totalOrderCount++] = current_orders[i];
                    } else {
                        cout << "  [!] Warning: Daily sales memory is full.\n";
                        break;
                    }
                }

                char nextChoice;
                cout << "\n  Enter 'N' for Next Customer, or 'M' for Main Menu: ";
                cin >> nextChoice;
                if (toupper(nextChoice) == 'M') {
                    keepOrdering = false;
                }
            }
            break;
        }

        case 5:
            manageInventory(loggedInIndex, accounts);
            break;

        case 6:
            report(total_orders, totalOrderCount);
            break;

        case 7:
            if (loggedInIndex != -1) {
                cout << "\n  [✔] User @" << accounts[loggedInIndex].username << " logged out successfully.\n";
                loggedInIndex = -1;
            } else {
                cout << "\n  [!] No active user session to log out.\n";
            }
            pauseScreen();
            break;

        case 8:
            cout << "\n  Thank you for using BOOKHAVEN BookShop System. Goodbye!\n\n";
            return 0;

        default:
            cout << "  [!] Invalid option! Please choose between 1 and 8.\n";
            pauseScreen();
            break;
        }
    }

    return 0;
}
//g++ asg.cpp -o asg.exe
//.\asg.exe