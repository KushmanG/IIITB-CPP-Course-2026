#ifndef CLASSES_H
#define CLASSES_H
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <iomanip>
#include <map>

using namespace std;

enum class PaperStatus {
    In_progress,
    Submitted,
    Under_Review,
    Accepted,
    Rejected
};

extern const vector<string> paperStatus;

class Paper {
    static int uniq;
    int paperID;
    string title;
    PaperStatus status;
    vector<string> authors;
public:
    Paper() = default;
    Paper(const string& t);
    Paper(const string& t, const string& a);
    Paper(const string& t, PaperStatus s);
    Paper(const string& t, PaperStatus s, vector<string>& a);
    void addAuthor(string author);
    double averageScore() const;
    void displayPaper() const;
    int getID() const;
    vector<string> getAuthors() const;
    string getStatus() const;
    void setStatus(int s);
};

class ConferencePaper : public Paper {
    vector<int> reviewScores;
public:
    ConferencePaper() = default;
    ConferencePaper(const string& t, const string& n);
    void addReviewScore(int score);
    double averageScore() const;
    void display() const;
};

class Reviewer {
    string name;
    /* changed Paper to ConferencePaper because it does not make sense for a reviewer to
       review a non-submitted paper */
    // also used pointer to be able to change its value properly
    vector<ConferencePaper*> assignedPapers;
public:
    Reviewer() = default;
    Reviewer(const string& n);
    Reviewer(const string& n, vector<ConferencePaper*>& papers);
    void assignPaper(ConferencePaper& paper);
    void reviewPaper(int score, int paperID);
    void display() const;
    string getName() const;
};

class Conference {
    string name;
    vector<ConferencePaper*> papers;
    vector<Reviewer*> reviewers;
public:
    Conference() = default;
    Conference(const string& n);
    Conference(const string& n, vector<ConferencePaper*>& p);
    Conference(const string& n, vector<ConferencePaper*>& p, vector<Reviewer*> &r);
    void submitPaper(ConferencePaper& paper);
    void addPaper(ConferencePaper& paper);
    void addReviewer(Reviewer& reviewer);
    void assignPaperToReviewer(int paperID, string reviewerName);
    void printAllPapers() const;
    void decideAcceptance();
    void printAllReviewers() const;
    void display() const;
    ConferencePaper* findPaper(int id);
    Reviewer* findReviewer(string name);
};

#endif // CLASSES_H