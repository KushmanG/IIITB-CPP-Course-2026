// Command-driven driver (last year's main), used by local_test8 and by run_driver.cpp.
// No change required.
void runDriver(istream& in) {
    Conference conference("ICPC");
    int no;
    while (in >> no) {
        try {
            if (no == 0) {
                string name; in >> name;
                conference.addReviewer(new Reviewer(name));
                cout << "0 OK\n";
            } else if (no == 1) {
                string title, author; in >> title >> author;
                ConferencePaper* cp = new ConferencePaper(title, author);
                conference.addPaper(cp);
                cp->display();
                cout << "1 OK\n";
            } else if (no == 2) {
                int id; string author; in >> id >> author;
                ConferencePaper* p = conference.findPaper(id);
                p->addAuthor(author);
                p->display();
                cout << "2 OK\n";
            } else if (no == 3) {
                int id; in >> id;
                conference.submitPaper(id);
                cout << "3 OK\n";
            } else if (no == 4) {
                int id; string reviewer; in >> id >> reviewer;
                conference.assignPaperToReviewer(id, reviewer);
                cout << "4 OK\n";
            } else if (no == 5) {
                string reviewer; int id, score; in >> reviewer >> id >> score;
                conference.findReviewer(reviewer)->reviewPaper(score, id);
                cout << "5 OK\n";
            } else if (no == 6) {
                conference.decideAcceptance();
                cout << "6 OK\n";
            } else if (no == 7) {
                conference.display();
                cout << "7 OK\n";
            } else {
                cout << no << " OK\n";
                break;
            }
        } catch (int x) {
            cout << x << " " << no << " NOK\n";
        } catch (invalid_argument& e) {
            cout << e.what() << " " << no << " NOK\n";
        }
    }
}
