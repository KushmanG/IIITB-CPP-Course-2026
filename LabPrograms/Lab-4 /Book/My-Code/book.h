#ifndef CPP_BOOK_H
#define CPP_BOOK_H

#include <iostream>
#include <string>
using namespace std;
#define MAX_BOOKS 10

namespace Encyclopedia{
    class Book
    {
        private:
            int pages;
            string title;

            static Book *books[MAX_BOOKS];
            static int bookCount;
        public:
        // Constructor
        Book();
        Book(const string& title, int pages);

        // Destructor
        ~Book();

        // Getters
        string getTitle() const;
        int getPages() const;
        static int getBooksCount();
        static Book *getAllBooks();

        // show() function
        void show();

        
        // Overload == operator so that equality of two books can be checked.
        bool operator==(const Book &b) const;
        
        // Overload + operator so that two Book objects can be added. The title of the sum book will be the concatenation of their
        // title, in the operand order.
        Book& operator+(const Book &b) const;
        
        friend ostream& operator<<(ostream& os, Book& b);
        
        static Book *findByTitle(const string &title);

        static bool deleteAllBooks();

        static void showAllBooks();



    };
}


#endif // CPP_BOOK_H