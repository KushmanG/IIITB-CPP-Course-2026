// Task 1 --> Created TomStory.cpp

// Task 3 --> Included the iostream library and used namespace std
#include <iostream>
using namespace std;

#include <string> //Task 4 sub-part --> Using string library

// Task 2 --> Copy the Brain class
class Brain
{
    bool good = true;
    string status = "";

    public:
        void whatsup()
        {
            if (status != "")
            {
                cout << "Got an Idea Sir ji. to trouble others, Ma'am ji!" << endl;
                return;
            }
            cout << "Getting Bored doing C++" << endl;
        }

        bool isWicked()
        {
            return !good;
        }

        void think()
        {
            status = "Gibberish";
            good = false;
        }

        void rethink()
        {
            status = "";
            good = true;
        }
};

// Task 4 --> Create the Spectacle class and initialize the private attribute shopName
class Spectacles
{
    string shopName = " E-City Gift Shop";

    // Task 5 --> Create a public method called show()
    public:
        void show()
        {
            cout << "Got from" << shopName << "." << endl;
        };
};

// Task 6 --> Create a class "Trap" with private boolean attribute called "placed" initialised to False
class Trap
{
    bool placed = false;
    // Task 7 --> Create a public method called "placed" that prints notification and changes the private flag "placed" to True
    public:
        void place()
        {
            cout << "Trap has been placed." << endl;
            placed = true;
        };
};

//Task 8 --> Create class called "Cat" with the given private attributes
class Cat
{
    string myName;
    Brain myBrain;
    bool sleeping = false;
    bool wearingSpectacles = false;

    Spectacles* spectacles = nullptr;

    // Task 9
    void whatsup()
    {
        myBrain.whatsup();
    }
    public:
        void setName(string name)
        {
            myName = name;
        }

        void wearSpectacles(Spectacles* sp)
        {
            if (sleeping)
            {
                cout << myName << " is sleeping and does not need spectacles." << endl;
            }
            spectacles = sp;
            wearingSpectacles = true;
        }

        void removeSpectacles()
        {
            wearingSpectacles = false;
        }

        bool canSee()
        {
            if (sleeping)
            {
                cout << myName << " is sleeping and cannot see." << endl;
                return false;
            }

            if (spectacles != nullptr && wearingSpectacles)
            {
                cout << myName << " can see clearly." << endl;
                return true;
            }

            else
            {
                cout << myName << " cannot see without spectacles." << endl;
                return false;
            }
        }

        void sleep()
        {
            sleeping = true;
        }

        void wakeUp()
        {
            sleeping = false;
            if (spectacles != nullptr)
            {
                wearingSpectacles = true;
            }
        }

        void torublePeopleUsingTrap(Trap* trap)
        {
            myBrain.think();
            trap->place();
        }

        void patpat()
        {
            myBrain.rethink();
            if (myBrain.isWicked() == false)
            {
                cout << myName << " is a good cat!" << endl;
            }
        }

        void show()
        {
            cout << endl;
            if (spectacles != nullptr)
            {
                spectacles->show();
            }
            canSee();
            whatsup();
            cout << endl;
        };

};

int main()
{
    Cat tom;
    tom.setName("Tom");
    tom.show();

    Spectacles spectacles;
    tom.wearSpectacles(&spectacles);
    tom.show();

    tom.sleep();
    tom.show();

    tom.wakeUp();
    tom.show();

    Trap stolenTrap;
    tom.torublePeopleUsingTrap(&stolenTrap);
    tom.show();

    tom.patpat();
    tom.show();

    return 0;
}