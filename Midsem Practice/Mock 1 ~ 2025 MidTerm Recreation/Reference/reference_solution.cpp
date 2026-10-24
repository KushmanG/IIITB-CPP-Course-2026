// Reference solution — Conference Paper Review System (reconstructed 2025 midsem)
// Do NOT open before attempting the mock.
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <iomanip>
using namespace std;

/*================================= NO EDIT ZONE =================================*/
enum class PaperStatus {
    In_progress,
    Submitted,
    Under_Review,
    Accepted,
    Rejected
};

const vector<string> paperStatus = {"In progress", "Submitted", "Under review", "Accepted", "Rejected"};
/*============================== END OF NO EDIT ZONE =============================*/

/* ---------------- 1. Paper ---------------- */
class Paper {
    static int uniq;
    int paperID;
    string title;
    PaperStatus status;
    vector<string> authors;
public:
    Paper(const string& title) : paperID(++uniq), title(title), status(PaperStatus::In_progress) {}
    Paper(const string& title, const string& author)
        : paperID(++uniq), title(title), status(PaperStatus::In_progress) {
        authors.push_back(author);
    }
    virtual ~Paper() {}

    int getID() const { return paperID; }
    string getTitle() const { return title; }
    vector<string> getAuthors() const { return authors; }
    bool hasAuthor(const string& name) const {
        for (const string& a : authors)
            if (a == name) return true;
        return false;
    }
    PaperStatus getStatusCode() const { return status; }
    string getStatus() const { return paperStatus[static_cast<int>(status)]; }
    void setStatus(PaperStatus s) { status = s; }

    void addAuthor(const string& author) {
        if (status != PaperStatus::In_progress) throw paperID;
        if (hasAuthor(author)) return;
        authors.push_back(author);
    }
    virtual double averageScore() const { return -1.0; }
    void displayPaper() const {
        cout << paperID << " " << title << ": ";
        for (const string& a : authors) cout << a << " ";
    }
};
int Paper::uniq = 0;

/* ---------------- 2. ConferencePaper ---------------- */
class ConferencePaper : public Paper {
    vector<int> reviewScores;
public:
    ConferencePaper(const string& title, const string& author) : Paper(title, author) {}
    void addReviewScore(int score) { reviewScores.push_back(score); }
    int getReviewCount() const { return reviewScores.size(); }
    double averageScore() const override {
        if (reviewScores.empty()) return 0.0;
        double sum = 0;
        for (int s : reviewScores) sum += s;
        return sum / reviewScores.size();
    }
    void display() const {
        displayPaper();
        cout << fixed << setprecision(2) << averageScore() << " " << getStatus() << "\n";
    }
};

/* ---------------- 3. Reviewer ---------------- */
class Reviewer {
    string name;
    vector<ConferencePaper*> assignedPapers;
public:
    Reviewer(const string& name) : name(name) {}
    string getName() const { return name; }
    void assignPaper(ConferencePaper* paper) {
        if (isAssigned(paper->getID())) return;
        assignedPapers.push_back(paper);
    }
    bool isAssigned(int paperID) const {
        for (ConferencePaper* p : assignedPapers)
            if (p->getID() == paperID) return true;
        return false;
    }
    void reviewPaper(int score, int paperID) {
        if (score < 1 || score > 10) throw score;
        for (ConferencePaper* p : assignedPapers) {
            if (p->getID() == paperID) {
                if (p->getStatusCode() != PaperStatus::Under_Review) throw paperID;
                p->addReviewScore(score);
                return;
            }
        }
        throw paperID;
    }
    void display() const { cout << name << "\n"; }
};

/* ---------------- 4. Conference ---------------- */
class Conference {
    string name;
    vector<ConferencePaper*> papers;
    vector<Reviewer*> reviewers;
public:
    Conference() {}
    Conference(const string& name) : name(name) {}

    void addPaper(ConferencePaper* paper) {
        for (ConferencePaper* p : papers)
            if (p->getID() == paper->getID()) return;
        papers.push_back(paper);
    }
    void addReviewer(Reviewer* reviewer) {
        for (Reviewer* r : reviewers)
            if (r->getName() == reviewer->getName()) throw invalid_argument(reviewer->getName());
        reviewers.push_back(reviewer);
    }
    ConferencePaper* findPaper(int id) {
        for (ConferencePaper* p : papers)
            if (p->getID() == id) return p;
        throw id;
    }
    Reviewer* findReviewer(const string& name) {
        for (Reviewer* r : reviewers)
            if (r->getName() == name) return r;
        throw invalid_argument(name);
    }
    void submitPaper(int paperID) {
        ConferencePaper* p = findPaper(paperID);
        if (p->getStatusCode() != PaperStatus::In_progress) throw paperID;
        p->setStatus(PaperStatus::Submitted);
    }
    void assignPaperToReviewer(int paperID, const string& reviewerName) {
        ConferencePaper* p = findPaper(paperID);
        Reviewer* r = findReviewer(reviewerName);
        PaperStatus s = p->getStatusCode();
        if (s != PaperStatus::Submitted && s != PaperStatus::Under_Review) throw paperID;
        if (p->hasAuthor(reviewerName)) throw paperID;
        r->assignPaper(p);
        p->setStatus(PaperStatus::Under_Review);
    }
    void decideAcceptance() {
        for (ConferencePaper* p : papers) {
            if (p->getStatusCode() != PaperStatus::Under_Review) continue;
            if (p->getReviewCount() == 0) continue;
            if (p->averageScore() >= 7.0) p->setStatus(PaperStatus::Accepted);
            else p->setStatus(PaperStatus::Rejected);
        }
    }
    void printAllPapers() const {
        for (int i = (int)papers.size() - 1; i >= 0; i--) papers[i]->display();
    }
    void printAllReviewers() const {
        for (Reviewer* r : reviewers) r->display();
    }
    void display() const {
        printAllReviewers();
        printAllPapers();
    }
};

int main() {
    return 0;
}
