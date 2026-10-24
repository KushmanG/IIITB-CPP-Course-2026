// Reference solution — 2026 Lab Quiz (UPI Wallet). Open only after attempting.
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>
using namespace std;

/*================================= NO EDIT ZONE **********************************/
class UPIException : public runtime_error {
public:
    UPIException(const string& message)
        : runtime_error(message) {}
};

class Logger {
    ofstream logFile;

public:
    Logger(const string& filename): logFile(filename, ios::out | ios::app) {
        if (!logFile.is_open()) {
            throw runtime_error("Unable to open log file.");
        }
    }

    void log(const string& actor, const string& message) {
        logFile << "[" << actor << "]: " << message << '\n';
        logFile.flush();
        if (!logFile) {
            throw runtime_error("Failed to write to log file.");

        }
    }
};

enum class UserMenuCmd {
    SEND_MONEY = 1,
    ADD_MONEY,
    CHECK_BALANCE,
    LOGOUT
};


enum class MainMenuCmd {
    REGISTER = 1,
    LOGIN,
    EXIT
};


const vector<pair<MainMenuCmd, string>> mainMenu = {
    {MainMenuCmd::REGISTER, "Register"},
    {MainMenuCmd::LOGIN, "Login"},
    {MainMenuCmd::EXIT, "Exit"}
};

const vector<pair<UserMenuCmd, string>> userMenu = {
    {UserMenuCmd::SEND_MONEY, "Send Money"},
    {UserMenuCmd::ADD_MONEY, "Add Money"},
    {UserMenuCmd::CHECK_BALANCE, "Check Balance"},
    {UserMenuCmd::LOGOUT, "Logout"}
};

template <typename Command>
void printMenu(const string& title, const vector<pair<Command, string>>& items) {
    cout << "\n===== " << title << " =====\n";

    for (const auto& item : items) {
        cout << static_cast<int>(item.first)<< ". " << item.second << '\n';
    }

    cout << "> ";
}

/*================================= END OF NO EDIT ZONE **********************************/

class Wallet {
    double balance;
public:
    Wallet() : balance(0.0) {}
    Wallet& operator+=(double amount) {
        if (amount <= 0) throw UPIException("Invalid amount.");
        balance += amount;
        return *this;
    }
    Wallet& operator-=(double amount) {
        if (amount <= 0) throw UPIException("Invalid amount.");
        if (amount > balance) throw UPIException("Balance not sufficient!");
        balance -= amount;
        return *this;
    }
    double getBalance() const { return balance; }
};


class Account {
    string phone;
    string name;
    string upiId;
    Wallet wallet;
    static const double MAX_ADD_LIMIT;
    static const double MAX_TXN_LIMIT;
public:
    Account(string phone, string name, string upiId) : phone(phone), name(name), upiId(upiId) {}
    const string& getPhone() const { return phone; }
    const string& getName() const {return name;}
    const string& getUpiId() const { return upiId; }

    void addMoney(double amount, Logger& logger) {
        try {
            if (amount > MAX_ADD_LIMIT) throw UPIException("Add-money limit exceeded (max Rs.20000).");
            wallet += amount;
            logger.log(upiId, "Added Rs." + to_string(amount) + " to " + name + "'s wallet. Balance: Rs." + to_string(wallet.getBalance()));
        } catch (UPIException& e) {
            logger.log(upiId, string("Add-money failed: ") + e.what());
            cout << e.what() << "\n";
        }
    }

    void sendMoney(Account& receiver, double amount, Logger& logger) {
        try {
            if (amount > MAX_TXN_LIMIT) throw UPIException("Transaction limit exceeded (max Rs.10000).");
            if (receiver.upiId == upiId) throw UPIException("Cannot send money to self.");
            wallet -= amount;
            logger.log(upiId, "Rs." + to_string(amount) + " debited from " + upiId);
            receiver.addMoney(amount, logger);
        } catch (UPIException& e) {
            logger.log(upiId, string("Send money failed: ") + e.what());
            cout << e.what() << "\n";
        }
    }

    double showBalance() const {
        cout << name << "'s balance: Rs." << wallet.getBalance() << "\n";
        return wallet.getBalance();
    }
};
const double Account::MAX_ADD_LIMIT = 20000;
const double Account::MAX_TXN_LIMIT = 10000;


class UPISystem {
    Logger logger{"./upi_log.txt"};
    vector<Account*> accounts;

    static string generateUpiId(const string& name) {
        string upiId;

        for (unsigned char character : name) {
            if (!isspace(character)) {
                upiId += static_cast<char>(tolower(character));
            }
        }

        upiId += "@ok.iiitb";
        return upiId;
    }

    Account* findAccount(const string& key) {
        for (Account* a : accounts)
            if (a->getPhone() == key || a->getUpiId() == key) return a;
        throw UPIException("[" + key + "] Receiver not registered on UPI.");
    }
public:

    void registerUser(const string& phone, const string& name) {
        try {
            string upiId = generateUpiId(name);
            for (Account* a : accounts)
                if (a->getPhone() == phone) throw UPIException("Phone number already registered.");
            for (Account* a : accounts)
                if (a->getUpiId() == upiId) throw UPIException("Generated UPI ID already in use. Try a different name.");
            accounts.push_back(new Account(phone, name, upiId));
            string msg = "Registered " + name + " | Phone: " + phone + " | UPI ID: " + upiId;
            logger.log("UPISystem", msg);
            cout << msg << "\n";
        } catch (UPIException& e) {
            logger.log("UPISystem", string("Registration failed: ") + e.what());
            cout << e.what() << "\n";
        }
    }

    Account* login(const string& identifier) {
        try {
            for (Account* a : accounts) {
                if (a->getPhone() == identifier || a->getName() == identifier) {
                    logger.log(a->getUpiId(), "Login successful.");
                    return a;
                }
            }
            throw UPIException("No such user " + identifier + ". Check phone number or name.");
        } catch (UPIException& e) {
            logger.log("UPISystem", string("Login failed: ") + e.what());
            return nullptr;
        }
    }

    bool doUserAction(Account *&currentUser, int choice) {

            switch (static_cast<UserMenuCmd>(choice)) {
                case UserMenuCmd::SEND_MONEY: {
                    string receiverIdentifier;
                    double amount;
                    cout << "Receiver phone number or UPI ID: ";
                    cin >> receiverIdentifier;
                    cout << "Amount: ";
                    cin >> amount;
                    try {
                        Account* receiver = findAccount(receiverIdentifier);
                        currentUser->sendMoney(*receiver, amount, logger);
                    } catch (UPIException& e) {
                        logger.log(currentUser->getUpiId(), string("Send money failed: ") + e.what());
                        cout << e.what() << "\n";
                    }
                    break;
                }

                case UserMenuCmd::ADD_MONEY: {
                    double amount;
                    cout << "Amount to add: ";
                    cin >> amount;
                    currentUser->addMoney(amount, logger);
                    break;
                }

                case UserMenuCmd::CHECK_BALANCE:
                    currentUser->showBalance();
                    break;

                case UserMenuCmd::LOGOUT:
                    cout << "Logged out.\n";
                    currentUser = nullptr;
                    return false;

                default:
                    cout << "Invalid choice.\n";
            }
        return true;
    }
};

bool doAppAction(UPISystem& upi, Account *&currentUser, int choice) {
    switch (static_cast<MainMenuCmd>(choice)) {
        case MainMenuCmd::REGISTER: {
            string phone;
            string name;
            cout << "Phone number: ";
            cin >> phone;
            cout << "Name: ";
            cin >> name;
            upi.registerUser(phone, name);
            break;
        }

        case MainMenuCmd::LOGIN: {
            string identifier;
            cout << "Enter phone number or name: ";
            cin >> identifier;
            currentUser = upi.login(identifier);
            if (currentUser) cout << "Welcome " << currentUser->getName() << "\n";
            break;
        }

        case MainMenuCmd::EXIT:
            cout << "Exiting app.\n";
            return false;

        default:
            cout << "Invalid choice.\n";
    }
    return true;
}

/*================================= NO EDIT ZONE **********************************/
void doApplicationJob() {
    UPISystem upi;

    // nullptr means no user is currently logged in.
    Account* currentUser = nullptr;

    while (true) {
        if (currentUser == nullptr) {
            printMenu("UPI App", mainMenu);

            int choice;
            cin >> choice;

            if (!doAppAction(upi, currentUser, choice)) break;
        } else {
            printMenu("Logged in as " + currentUser->getName(),userMenu);

            int choice;
            cin >> choice;

            upi.doUserAction(currentUser, choice);
        }
    }
}

int main() {
    try {
        doApplicationJob();
    } catch (const exception& exception) {
        cout << "Application error: " << exception.what() << '\n';
        return 1;
    }
    return 0;
}
/*================================= END OF NO EDIT ZONE **********************************/
