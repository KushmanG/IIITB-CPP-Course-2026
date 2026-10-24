#include <iostream>
#include <vector>
#include <unordered_set>
using namespace std;

// Everything in a class in cpp is private by default so you always gotta do the public
// private --> Only usable inside the class itself and it's friends
// public --> Usable everywhere given the class object is made
// protected --> Usable in the class itself, it's friends and derived classes

class Employee
{
    public: // Access modifiers
        int Age;
        string Name;
        string Company;

        // Class Methods are used to describe actions/behaviours of objects
        // Example, an employee could introduce himself
        void introduce()
        {
            cout << "Hello, my name is " << Name << ", " << Age << " years old, and I work for " << Company << endl;
        }

        /* 
        Constructors: 
        Have the same name as the class
        Have no return value
        Must be in public

        We use em to initialize Class objects much easily
        */
        Employee(string name, string company, int age)
        {
            Name = name;
            Company = company;
            Age = age;
        }
};

// Inheritance
// Here the derived class Devloper inherits all the public properties of the parent class Employee
/*
Hierarchies:

- Chains are fine, Hexagon : Polygon : Shape. Dispatch finds the most-derived override, and intermediate levels can 
  skip overriding entirely.

- Virtualness is inherited and can't be undone. override asks the compiler to check you; final stops further 
  overriding.

- Depth gets expensive to maintain — 2–3 levels is usually the limit.


Access:

- Default is private for class, public for struct.

- The specifier is a ceiling on the base's members, never a grant. Base private members stay inaccessible either way
  that's what protected is for.

- Only public inheritance preserves is-a. Private inheritance kills the derived→base conversion, so polymorphism 
  stops working from outside.
*/

class Developer:public Employee 
{
    private:
        string FavLang;
    public: 
        // Derived classes don't inherit Constructors
        Developer(string name, string company, int age, string favLang)
            : Employee(name, company, age)
        {
            favLang = FavLang;
        }
};



/*
Encapsulation:
In encapsulation we keep all the variables private and the class itself decides what can be done to them. Instead of just handing out 
setters and getters for every variable, we write functions for the actual stuff we want to do, like withdraw(500) instead of getBalance 
and setBalance, cz that way the object checks its own rules and the outside code doesn't have to. The constructor sets it up in a valid 
state and every function keeps it that way. Point is nobody outside knows how we're storing anything, so we can change the variables 
later and nothing else breaks. Setters and getters are just one way to do this, useful cz they let us put constraints, but they're 
not the whole thing
*/
class Account
{
    private:
        string Username;
        int balance;
    
    public:
        static int account_number;
        inline static unordered_set<string> Names; // We are using inline static to keep a global unordered set for all class objects
        // If we don't use inline static then each Account will have it's own unordered set so uniqueness is nowhere forced
        void setName()
        {
            int l = Names.size();
            int L = Names.size();
            bool first_time = true;
            while (L <= l)
            {
                if (first_time) cout << "Hello New User" << endl;
                string name;
                cout << " Please Enter Your Username: ";
                cin >> name;
                cout << endl;
                if (!Names.count(name)) 
                {
                    Username = name;
                    Names.insert(Username);
                    L++;
                    cout << "Welcome to KayGee Bank " << name << "\n" << endl;
                }
                else 
                {
                    cout << "\nUsername is already taken, please choose a different one\n" << endl;
                    first_time = false;
                }
            }
        }
        void withdraw(int amount)
        {
            if (balance > amount) balance = balance - amount;
            else cout << "Insufficient Funds, Current Bank Balance = " << balance << ", You cannot withdraw more than Bank Balance." << endl;
        }

        void deposit(int amount)
        {
            balance = balance + amount;
            cout << "Added " << amount << " to your bank account, New Balance = " << balance << endl;
        }

        Account()
        {
            setName();
            balance = 0;
            account_number++;
        }
};


int Account::account_number= 0;


int main()
{
    // CLASSES AND CONSTRUCTORS:
    Employee e1("Kushman", "IIIT-B", 18); // Class object
    /*
    e1.Name = "Kushman"
    e1.Company = "IIIT-B"
    e1.Age = 18

    Without constructor
    */
    e1.introduce(); 

    Employee e2("Vishnu", "IIIT-B", 19);
    e2.introduce();

    Developer d("Avaneesh", "IIIT-B", 20, "C++");
    // Now typing every single employee name is pretty.. annoying, idw do that, so we use 
    // constructors

    // ENCAPSULATION:
    Account a1;
    Account a2;
    Account a3;
    Account a4;
      

}