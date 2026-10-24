// This is the code I submitted in the exam
// Final Score: 48
// Avg: 24.7 Median: 23
// Using Local Tester I got 3/11 tho
// I think Vishnu made a LocalTester to cheeck your score without any domjudge credentials so check that out if you wanna test you program


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
    Wallet() : balance(0.0) {};
    Wallet& operator+=(double amount) 
    {
        if (amount <= 0) 
        {
            throw UPIException("Invalid amount.");
            return *this;
        }
        balance += amount;
        return *this;
    }
    Wallet& operator-=(double amount)
    {
        if (amount <= 0) 
        {
            throw UPIException("Invalid amount.");
            return *this;
        }
        if (amount > balance) 
        {
            throw UPIException("Balance not sufficient!");
            return *this;
        }
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
    Account(string phone, string name, string upiId)
    {
        this->phone = phone;
        this->name = name;
        this->upiId = upiId;
    }
    const string& getPhone() const { return phone; }
    const string& getName() const {return name;}
    const string& getUpiId() const { return upiId; }
    
    void addMoney(double amount, Logger& logger)
    {
        string actor = upiId;
        if (amount > MAX_ADD_LIMIT) throw UPIException("Add-money limit exceeded (max Rs.20000).");
        wallet.operator+=(amount);
        string message = "Added Rs." + to_string(amount) + "to" + name + "'s wallet. Balance: Rs." + to_string(wallet.getBalance());
        logger.log(actor, message);
    }
    
    void sendMoney(Account& receiver, double amount, Logger& logger)
    {
        string actor = upiId;
        if (amount > MAX_TXN_LIMIT) throw UPIException("Transaction limit exceeded (max Rs.10000).");
        else if (receiver.upiId == this->upiId) throw UPIException("Cannot send money to self.");
        wallet.operator-=(amount);
        string message = "Rs." + to_string(amount) + "debited from" + upiId;
        logger.log(actor, message);
    }

    double showBalance() const { cout << name << "'s balance: Rs." << wallet.getBalance() << endl; }
};

const double Account::MAX_ADD_LIMIT = 20000;
const double Account::MAX_TXN_LIMIT = 10000;

class UPISystem {
    vector<Account*> accounts;
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

    Account* findUser(string s) {
        for (size_t i = 0; i < accounts.size(); i++)
        {
            if (s == accounts[i]->getUpiId()) return accounts[i];
            else if (s == accounts[i]->getPhone()) return accounts[i];
        }   
        throw UPIException("[" + s + "]" + " Receiver not registered on UPI.");
        return nullptr;
    }

    bool userExists(string s)
    {
        bool flag = false;
        for (size_t i = 0; i < accounts.size(); i++)
        {
            if (s == accounts[i]->getPhone()) flag = true;
        }  
        return flag;
    }

     bool idExists(string s)
    {
        bool flag = false;
        string id = generateUpiId(s);
        for (size_t i = 0; i < accounts.size(); i++)
        {
            if (id == accounts[i]->getUpiId()) flag = true;
        }
        return flag;
    }
public:

    void registerUser(const string& phone, const string& name)
    {
        if (userExists(phone) == true) throw UPIException("Phone number already registered.");
        else if (idExists(name) == true) throw UPIException("Generated UPI ID already in use. Try a different name.");
        string upiId = generateUpiId(name);
        Account* account = new Account(phone, name, upiId);
        accounts.push_back(account);
        string actor = "UPISystem";
        string message = "Registered " + name + " | Phone: " + phone + " | UPI ID: "  + upiId;
        logger.log(actor, message);

    }
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
