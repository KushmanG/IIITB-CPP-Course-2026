#include "Classes.h"

int main() {
    Conference conference;
    do {
        int no;
        cin >> no;
        if (no == 0) {
            try {
                string reviewerName;
                cin >> reviewerName;
                Reviewer *reviewer = new Reviewer(reviewerName);
                conference.addReviewer(*reviewer);
                cout << "0 OK\n";
            }
            catch(invalid_argument& e) {
                cerr << e.what() << " 0 NOK\n";
                break;
            }
        }
        else if (no == 1) {
            try {
                string title, author;
                cin >> title >> author;
                ConferencePaper* cp = new ConferencePaper(title, author);
                conference.addPaper(*cp);
                cp->display();
                cout << "1 OK\n";
            }
            catch(invalid_argument& e) {
                cerr << e.what() << " 1 NOK\n";
                break;
            }
        }
        else if (no == 2) {
            try {
                int id;
                string author;
                cin >> id >> author;
                ConferencePaper* toAdd = conference.findPaper(id);
                toAdd->addAuthor(author);
                toAdd->display();
                cout << "2 OK\n";
            }
            catch(int i) {
                cerr << i << " 2 NOK\n";
                break;
            }
        }
        else if (no == 3) {
            try {
                int id;
                cin >> id;
                ConferencePaper* toSubmit = conference.findPaper(id);
                conference.submitPaper(*toSubmit);
                cout << "3 OK\n";
            }
            catch(int i) {
                cerr << i << " 3 NOK\n";
                break;
            }
        }
        else if (no == 4) {
            try {
                int id;
                string reviewer;
                cin >> id >> reviewer;
                conference.assignPaperToReviewer(id, reviewer);
                cout << "4 OK\n";
            }
            catch(int i) {
                cerr << i << " 4 NOK\n";
                break;
            }
        }
        else if (no == 5) {
            try {
                string reviewer;
                int id, score;
                cin >> reviewer >> id >> score;
                Reviewer* found = conference.findReviewer(reviewer);
                found->reviewPaper(score, id);
                cout << "5 OK\n";
            }
            catch(int i) {
                cerr << i << " 5 NOK\n";
                break;
            }
        }
        else if (no == 6) {
            try {
                conference.decideAcceptance();
                cout << "6 OK\n";
            }
            catch(invalid_argument& e) {
                cerr << e.what() << " 6 NOK\n";
                break;
            }
        }
        else if (no == 7) {
            try {
                conference.display();
                cout << "7 OK\n";
            }
            catch(invalid_argument& e) {
                cerr << e.what() << " 7 NOK\n";
                break;
            }
        }
        else {
            cout << no << " OK\n";
            break;
        }
    } while (true);
    return 0;
}