#include <iostream>
#include <string>
using namespace std;
#define MAX_SCREENS 10

class Screen
{
    string contents;
    unsigned int height;
    unsigned int width;
    int cursor;

    public:
        friend class Window_mgr;
        // Constructors
        // Default
        Screen() : contents(20*50, ' '), height(20), width(50), cursor(0) {};

        // Value constructor
        Screen(unsigned int h, unsigned int w, char fillChar)
            : contents(h*w, fillChar), height(h), width(w), cursor(0) 
            {};

        // Copy Constructor
        Screen(const Screen& other)
        {
            this->contents = other.contents;
            this->height = other.height;
            this->width = other.width;
            this->cursor = other.cursor;
        }

        // Copy assignment (Rule of Three: we declared a copy ctor)
        Screen& operator=(const Screen& other) = default;

        // Member functions
        char get() const { return contents[cursor]; }
        char get(unsigned int r, unsigned int c) const { return contents[r * width + c]; }
        Screen& set(char ch) 
        { 
            contents[cursor] = ch;
            return *this;
        }
        Screen& set(unsigned int r, unsigned int c, char ch) 
        { 
            if (r >= this->height || c >= this->width) return *this;
            contents[r*width + c] = ch; 
            return *this;
        }
        Screen& move(unsigned int r, unsigned int c)
        {
            cursor = r * width + c;
            return *this;
        }
        Screen& display(std::ostream &os)
        {
            for (unsigned int i = 0; i < height; i++)
            {
                for (unsigned int j = 0; j < width; j++)
                {
                    os << contents[i*width+j];
                }
                os << "\n";
            }
            return *this;
        }
        const Screen& display(std::ostream &os) const
        {
            for (unsigned int i = 0; i < height; i++)
            {
                for (unsigned int j = 0; j < width; j++)
                {
                    os << contents[i*width+j];
                }
                os << "\n";
            }
            return *this;
        }
};

class Window_mgr 
{
    Screen screens[MAX_SCREENS];
    int screenCount;

    public:
        // Default Constructor
        Window_mgr(): screenCount(1) { screens[0] = Screen(); }

        int addScreen(const Screen &s)
        {
            if (screenCount >= MAX_SCREENS) return -1;
            screens[screenCount] = s;
            return screenCount++;
        }

        int addScreen(const Screen *s)
        {
            if (screenCount >= MAX_SCREENS) return -1;
            screens[screenCount] = *s;
            return screenCount++;
        }

        void clear (int index)
        {
            unsigned int h = screens[index].height;
            unsigned int w = screens[index].width;
            screens[index].contents =  string(h*w, ' ');
        }

        // Lets the driver see a screen that the manager owns
        void displayScreen(int index, ostream& os)
        {
            screens[index].display(os);
        }
};

int main()
{
    Screen s1(10, 20, '#');   
    Screen s2;                
    Screen s3(5, 5, '*');     
    Screen s4(s3);           

    cout << "Initial Displays" << endl;
    s1.display(cout);
    s3.display(cout);
    s4.display(cout);

    // Chained calls
    s1.move(2, 3).set('@').move(5, 10).set('@');
    s3.move(2, 2).set('X');

    // out-of-range set 
    s3.set(99, 99, '?');

    cout << "Displays after Edit 1" << endl;
    s1.display(cout);
    s3.display(cout);

    // get() reads the character under the cursor / at a location
    cout << "s3.get() at cursor (2,2) = " << s3.get() << "\n";
    cout << "s3.get(0,0) = " << s3.get(0, 0) << "\n";

    Window_mgr win;
    int i1 = win.addScreen(s1);
    int i3 = win.addScreen(&s3);
    cout << "s1 stored at index " << i1 << ", s3 stored at index " << i3 << "\n";

    // Clear one screen 
    cout << "\n Screen " << i3 << " before clear \n";
    win.displayScreen(i3, cout);
    win.clear(i3);
    cout << "Screen" << i3 << " after clear\n";
    win.displayScreen(i3, cout);

    // Chained call: move -> set -> display in one statement
    cout << "\n Chained move/set/display on s4 \n";
    s4.move(0, 0).set('+').display(cout);

    return 0;
}