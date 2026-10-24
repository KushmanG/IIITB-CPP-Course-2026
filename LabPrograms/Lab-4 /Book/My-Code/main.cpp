#include <iostream>
#include <string>
#include "book.h"
using namespace Encyclopedia;

enum class Action
{
    CREATE_BOOK,
    SHOW_BOOK,
    ADD_BOOKS,
    DELETE_ALL_BOOKS,
    EXIT,
    COUNT
};

const char* book_cmds[] = {
    "CREATE_BOOK", 
    "SHOW_BOOK", 
    "ADD_BOOKS", 
    "DELETE ALL BOOKS", 
    "EXIT"
};

// Operator<< uses array instead of switch
ostream& operator<<(ostream& os, const Action& choice) {
    int idx = static_cast<int>(choice);
    if (idx >= 0 && idx < static_cast<int>(Action::COUNT))
        os << book_cmds[idx];
    else
        os << "UNKNOWN";
    return os;
}

Action printMenu(istream& in=cin, ostream& out=cout) 
{
    int choice;
    out << "===== Book Actions Menu =====\n";
    for (int i = 0; i < sizeof(book_cmds)/sizeof(book_cmds[0]); i++)
    {
        out << i << ". " << book_cmds[i] << endl;
    }
    out << "==============================\n";
    out << "Enter Choice: ";
    in >> choice;
    out << endl;

    out << book_cmds[choice] << endl;

    Action action = static_cast<Action>(choice);
    return action;
}

Book*  getBookOrNull(const string& title) {
    Book* b = Book::findByTitle(title);
    if (b == nullptr) cout << title << " does not exists!" << endl;
    return b;
}

void doAction(Action actionItem, istream& in = cin, ostream& out = cout) 
{
    string title, title1, title2;
    int pages;
    switch(actionItem)
    {
        case Action::CREATE_BOOK:
        {
            out << "Enter title: ";
            in >> title;
            out << endl;

            out << "Enter pages: ";
            in >> pages;
            out << endl;

            Book* book1 = new Book(title, pages);
            break;
        }

        case Action::SHOW_BOOK:
        {
            out << "Enter title: ";
            in >> title;

            Book* book2 = getBookOrNull(title);
            if (book2 != nullptr) book2->show();
            break;
        }

        case Action::ADD_BOOKS:
        {
            out << "Enter title 1: ";
            in >> title1;
            out << endl;

            out << "Enter title 2: ";
            in >> title2;
            out << endl;

            Book* book3 = getBookOrNull(title1);
            Book* book4 = getBookOrNull(title2);
            if (book3 != nullptr && book4 != nullptr) *book3 = book3->operator+(*book4);
            break;
        }

        case Action::DELETE_ALL_BOOKS:
        {
            Book::deleteAllBooks();
            break;
        }

        default:
        {
            out << "Invalid command." << endl;
            break;
        }
    }
}

void runEventLoop(istream& in=cin, ostream& out=cout) 
{
    while (true)
    {
        Book::showAllBooks();
        Action actionItem = printMenu();
        if (actionItem == Action::EXIT)
        {
            Book::deleteAllBooks();
            out << "Exit" << endl;
            return;
        }
        doAction(actionItem);
    }
}

int main() 
{
    runEventLoop();
}