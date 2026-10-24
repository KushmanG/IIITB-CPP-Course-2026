#include <iostream>
#include <string>
#include "book.h"
using namespace Encyclopedia; // Must be written after imports else it shows object declaration as "ambiguous"

// Re-Initializing the array and counter
Book* Book::books[MAX_BOOKS];
int Book::bookCount = 0;

// Constructor
Book::Book()
{
    this->pages = 0;
    this->title = "";
    books[bookCount] = this;
    bookCount++;
}

Book::Book(const string& title, int pages)
{
    this->title = title;
    this->pages = pages;
    books[bookCount] = this;
    bookCount++;
}


// Destructor
Book::~Book()
{
    int thisIndex;
    for (int i = 0; i < bookCount; i++)
    {
        if (books[i] == this) 
        {
            thisIndex = i;
            break;
        }
    }

    books[thisIndex] = nullptr;
    for (int i = thisIndex; i < bookCount-1; i++) books[i] = books[i+1];
    bookCount--;
}

// Getters
string Book::getTitle() const
{
    return title;
}

int Book::getPages() const
{
    return pages;
}

int Book::getBooksCount()
{
    return bookCount;
}
Book* Book::getAllBooks()
{
    return *books;
}

// show() function
void Book::show()
{
    std::cout << this->title << " " << this->pages << std::endl;
}

// Overload == operator so that equality of two books can be checked.
bool Book::operator==(const Book &b) const
{
    if (this->title == b.title && this->pages == b.pages) return true;
    else return false;
}

// Overload + operator so that two Book objects can be added. The title of the sum book will be the concatenation of their
// title, in the operand order.
Book& Book::operator+(const Book &b) const
{
    string newTitle = this->title + b.title;
    int newPages = this->pages + b.pages;

    Book* book = new Book(newTitle, newPages);
    return *book;
}
namespace Encyclopedia
{
    ostream& operator<<(ostream& os, Book& b) 
    {
        os << b.title << " " << b.pages;
        return os;
    }
}

Book* Book::findByTitle(const string &title)
{
    for (int i = 0; i < bookCount; i++)
    {
        if (title == books[i]->title) return books[i];
    }
    return nullptr;
}

bool Book::deleteAllBooks()
{
    if (bookCount == 0) return false;
    while (bookCount > 0) delete books[0];
    bookCount = 0;
    return true;
}

void Book::showAllBooks()
{
    cout << "========== BOOKS REPO ============" << endl;
    for (int i = 0; i < bookCount; i++)
    {
        cout << "[" << i << "]" << endl;
        cout << books[i]->title << " " << books[i]->pages << endl;
    }
    cout << "=========== BOOKS REPO END =============" << endl;
}

