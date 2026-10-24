#include <iostream>
using namespace std;

// Virtual Functions
/*
These are functions initialized in the Base class and overridden in any derived class
*/


// Base Class
class Instrument
{
    public:
        virtual void Play()
        {
            cout << "Instrument Playing..." << endl;
        }

        void Stop()
        {
            cout << "Instrument Stops Playing." << endl;
        }
};

// Derived Class 
class Guitar:public Instrument
{
    public:
        void Play()
        {
            cout << "Guitar Playing..." << endl;
        }

        void Stop()
        {
            cout << "Guitar Stops Playing." << endl;
        }
};

// Another Derived Class
class Violin:public Instrument
{
    public:
        void Play()
        {
            cout << "Violin Playing..." << endl;
        }

        void Stop()
        {
            cout << "Violin Stops Playing." << endl;
        }
};

class Piano:public Instrument
{
    public:
        void Stop()
        {
            cout << "Violin Stops Playing." << endl;
        }
};

int main()
{
    // Initializing an object of the Base Class
    Instrument i1;
    // Initializing an object of the Derived Class
    Instrument *i2 = new Guitar();
    Instrument *i3 = new Violin();

    // Instrument *i4; This gives 'segmentation fault'
    // i4->Play();
    // To run the Play() method for the Base Class
    i1.Play();
    // To run the Play() method for the Derived Class
    i2->Play();
    i3->Play();

    // Also, if you do not do 'virtual void Play()' in the base class then it will always return the value
    // of the function in the base class, i.e then i1.Stop() and i2->Stop() will do the same thing
    i1.Stop();
    // i1->Stop(); Is inherently wrong cz i1 is not initialized as a pointer
    i2->Stop();
    i3->Stop();
    /*
    We Get:
        Instrument Playing...
        Guitar Playing...
        Violin Playing...
        Instrument Stops Playing.
        Instrument Stops Playing.
        Instrument Stops Playing.
    
    Because there is not 'virtual' written in the Base Class Stop function
    */

    // Now since the Piano Derived Class doesn't have a method for Play(), if you do this
    Instrument *i5 = new Piano();
    i5->Play();
    // You won't get an error but instead, it will say "Instrument Playing..."

    return 0;
}