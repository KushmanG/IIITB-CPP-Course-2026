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
    //Wallet& operator+=(double amount);
    //Wallet& operator-=(double amount);
    //double getBalance() const;
};


class Account {
    string phone;
    string name;
    string upiId;
    Wallet wallet;
    static const double MAX_ADD_LIMIT;
    static const double MAX_TXN_LIMIT;
public:
    //Account(string phone, string name, string upiId);
    //const string& getPhone() const;
    const string& getName() const {return name;}
    //const string& getUpiId() const;
    //void addMoney(double amount, Logger& logger);
    //void sendMoney(Account& receiver, double amount, Logger& logger);
    //double showBalance() const;
};


class UPISystem {
    Logger logger{"./upi_log.txt"};

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
public:

    //void registerUser(const string& phone, const string& name);
    //Account* login(const string& identifier);

    bool doUserAction(Account *currentUser, int choice) {

            switch (static_cast<UserMenuCmd>(choice)) {
                case UserMenuCmd::SEND_MONEY: {
                    string receiverIdentifier;
                    double amount;
                    cout << "Receiver phone number or UPI ID: ";
                    cout << "Amount: ";
                    break;
                }

                case UserMenuCmd::ADD_MONEY: {
                    double amount;
                    cout << "Amount to add: ";
                    break;
                }

                case UserMenuCmd::CHECK_BALANCE:
                    break;

                case UserMenuCmd::LOGOUT:
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
            cout << "Name: ";
            break;
        }

        case MainMenuCmd::LOGIN: {
            string identifier;
            cout << "Enter phone number or name: ";
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
