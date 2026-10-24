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

// paperStatus[static_cast<int>(s)] gives the display string of status s
const vector<string> paperStatus = {"In progress", "Submitted", "Under review", "Accepted", "Rejected"};
/*============================== END OF NO EDIT ZONE =============================*/


/* Write the classes below, IN THE ORDER of the question (Section 4 -> Section 7). */
class Paper {
    private:
        static int uniq;
        int paperID;
        string title;
        PaperStatus status = PaperStatus::In_progress;
        vector<string> authors;
    
    public:
        // Constructors/Destructors
        Paper(const string& title) 
        {
            this->paperID = ++uniq;
            this->title = title;
        }

        Paper(const string& title, const string& author)
        {
            this->paperID = ++uniq;
            this->title = title;
            this->authors.push_back(author);
        }

        virtual ~Paper() {}

        // Getters
        int getID() const { return this->paperID; }
        string getTitle() const { return this->title; }
        vector<string> getAuthors() const 
        {
            vector<string> authors_copy;
            for (size_t i = 0; i < authors.size(); i++) { authors_copy.push_back(authors[i]); }
            return authors_copy;
        }
        PaperStatus getStatusCode() const { return status; }
        string getStatus() const { return paperStatus[static_cast<int>(status)]; }
        
        // Setters
        void setStatus(PaperStatus s) { this->status = s; }
        bool hasAuthor(const string& author) const
        {
            for (size_t i = 0; i < authors.size(); i++) { if (authors[i] == author) return true; }
            return false;
        }

        void addAuthor(const string& author) 
        { 
            if (status != PaperStatus::In_progress) throw paperID;
            if (hasAuthor(author)) return;
            this->authors.push_back(author); 
        }

        // Misc Methods
        virtual double averageScore() const { return -1.0; }


        void displayPaper() const
        {
            cout << paperID << " " << title << ": ";
            for (size_t i = 0; i < authors.size(); i++) { cout << authors[i] << " "; }
        }
};

// Initializing the static variable to 0
int Paper::uniq = 0;

class ConferencePaper : public Paper {
    private:
        vector<int> reviewScores;
    
    public:
        ConferencePaper(const string& title, const string& author) : Paper(title, author) {};

        void addReviewScore(int score) { this->reviewScores.push_back(score); }

        int getReviewCount() const { return reviewScores.size(); }

        double averageScore() const
        {
            if (reviewScores.empty()) return 0.0;
            double sum = 0;
            for (size_t i = 0; i < reviewScores.size(); i++) { sum += reviewScores[i]; }
            double avg = sum/reviewScores.size();
            return avg;
        }

        void display() const
        {
            displayPaper();
            cout << fixed << setprecision(2) << this->averageScore() << " " << this->getStatus() << endl;
        }

};

class Reviewer {
    string name;
    vector<ConferencePaper*> assignedPapers;

    public:
        // Constructors
        Reviewer(const string& name) { this->name = name; }

        // Getters
        string getName() const { return name; }

        // Misc
        bool isAssigned(int paperID) const
        {
            for (size_t i = 0; i < assignedPapers.size(); i++) if (assignedPapers[i]->getID() == paperID) return true;
            return false;
        }

        void assignPaper(ConferencePaper* paper) 
        {
            if (isAssigned(paper->getID())) return;
            assignedPapers.push_back(paper);
        }


        void reviewPaper(int score, int paperID)
        {
            if (score < 1 || score > 10) { throw score; return; }
            if (!isAssigned(paperID)) { throw paperID; return; }

            ConferencePaper* paper;
            for (size_t i = 0; i < assignedPapers.size(); i++) if (assignedPapers[i]->getID() == paperID) paper = assignedPapers[i];
            if (paper->getStatus() != "Under review") {throw paperID; return; }
            
            paper->addReviewScore(score);
        }

        void display() const { cout << name << endl; }
};

class Conference {
    string name;
    vector<ConferencePaper*> papers;
    vector<Reviewer*> reviewers;

    public:
        // Constructors
        Conference() = default;
        Conference(const string& name) { this-> name = name; }

        void addPaper(ConferencePaper* paper) 
        {
            for (size_t i = 0; i < papers.size(); i++) if (papers[i] == paper) return;
            papers.push_back(paper);
        }
        void addReviewer(Reviewer* reviewer) 
        {
            for (size_t i = 0; i < reviewers.size(); i++) if (reviewers[i]->getName() == reviewer->getName()) throw invalid_argument(reviewer->getName());
            reviewers.push_back(reviewer);
        }

        ConferencePaper* findPaper(int id) 
        {
            for (size_t i = 0; i < papers.size(); i++) if (papers[i]->getID() == id) return papers[i];
            throw id;
        }
        Reviewer* findReviewer(const string& name) 
        {
            for (size_t i = 0; i < reviewers.size(); i++) if (reviewers[i]->getName() == name) return reviewers[i];
            throw invalid_argument(name);
        }

        void submitPaper(int paperID) 
        {
            ConferencePaper* paper = findPaper(paperID);
            if (paper->getStatus() != "In progress") throw paperID;
            paper->setStatus(PaperStatus::Submitted);
        }
        void assignPaperToReviewer(int paperID, const string& reviewerName) 
        {
            ConferencePaper* paper = findPaper(paperID);
            Reviewer* reviewer = findReviewer(reviewerName);
            if (paper->getStatus() != "Submitted" && paper->getStatus() != "Under review") { throw paperID; return; }
            if (paper->hasAuthor(reviewerName)) { throw paperID; return; }

            reviewer->assignPaper(paper);
            paper->setStatus(PaperStatus::Under_Review);
        }
        void decideAcceptance() 
        {
            for (size_t i = 0; i < papers.size(); i++)
            {
                if (papers[i]->getReviewCount() >= 1 && papers[i]->getStatus() == "Under review")
                {
                    if (papers[i]->averageScore() >= 7.0) papers[i]->setStatus(PaperStatus::Accepted);
                    else papers[i]->setStatus(PaperStatus::Rejected);
                }
            }
        }

        void printAllReviewers() const { for(size_t i = 0; i < reviewers.size(); i++) reviewers[i]->display(); }
        void printAllPapers() const { for(int i = (int)papers.size()-1; i >= 0; i--) papers[i]->display(); }
        void display() const { printAllReviewers(); printAllPapers(); }
};


/* main is not evaluated. Use it for your own testing if you like. */
int main() {
    return 0;
}
