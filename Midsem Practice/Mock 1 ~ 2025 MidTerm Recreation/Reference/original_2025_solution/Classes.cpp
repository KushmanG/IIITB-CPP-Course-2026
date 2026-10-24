#include "Classes.h"

int Paper::uniq = 0;
vector<string> paperStatus = {"In progress", "Submitted", "Under review", "Accepted", "Rejected"};

Paper::Paper(const string& t) : title(t) {
    paperID = ++uniq;
}
Paper::Paper(const string& t, const string& a) : title(t) {
    paperID = ++uniq;
    authors.push_back(a);
}
Paper::Paper(const string& t, PaperStatus s) : title(t), status(s) {
    paperID = ++uniq;
}
Paper::Paper(const string& t, PaperStatus s, vector<string>& a) : title(t), status(s), authors(a) {
    paperID = ++uniq;
}
void Paper::addAuthor(string author) {
    if (getStatus() != "Submitted")
        authors.push_back(author);
    else
        throw paperID;
}
double Paper::averageScore() const {
    return -1.0;
}
void Paper::displayPaper() const {
    cout << paperID << " " << title << ": ";
    for (size_t i = 0; i < authors.size(); i++) {
        cout << authors[i] << " ";
    }
}
int Paper::getID() const {
    return paperID;
}
vector<string> Paper::getAuthors() const {
    return authors;
}
string Paper::getStatus() const {
    int stat = static_cast<int>(status);
    if (stat >= 0 && stat <= 4)
        return paperStatus[stat];
    return "INVALID";
}
void Paper::setStatus(int s) {
    status = static_cast<PaperStatus>(s);
}

ConferencePaper::ConferencePaper(const string& t, const string& n) : Paper(t, n) {}
void ConferencePaper::addReviewScore(int score) {
    reviewScores.push_back(score);
}
double ConferencePaper::averageScore() const {
    double average = 0, size = reviewScores.size();
    if (size == 0) return 0;
    for (auto it: reviewScores)
        average += it;
    average /= size;
    return average;
}
void ConferencePaper::display() const {
    string status = getStatus();
    displayPaper();
    cout << fixed << setprecision(2) << averageScore() << " " << status << endl;
}

Reviewer::Reviewer(const string& n) : name(n) {}
Reviewer::Reviewer(const string& n, vector<ConferencePaper*>& papers) : name(n), assignedPapers(papers) {}
void Reviewer::assignPaper(ConferencePaper& paper) {
    bool found = false;
    for (auto &it: assignedPapers)
        if (it->getID() == paper.getID())
            found = true;
    if (!found)
        assignedPapers.push_back(&paper);
}
void Reviewer::reviewPaper(int score, int paperID) {
    if (score < 1 || score > 10)
        throw score;
    bool found = false;
    for (auto &it: assignedPapers) {
        if (it->getID() == paperID) {
            found = true;
            if (it->getStatus() != "Submitted")
                throw paperID;
            it->addReviewScore(score);
        }
    }
    if (!found)
        throw paperID;
}
void Reviewer::display() const {
    cout << name << endl;
}
string Reviewer::getName() const {
    return name;
}

Conference::Conference(const string& n) : name(n) {}
Conference::Conference(const string& n, vector<ConferencePaper*>& p) : name(n), papers(p) {}
Conference::Conference(const string& n, vector<ConferencePaper*>& p, vector<Reviewer*> &r) : name(n), papers(p), reviewers(r) {}
void Conference::submitPaper(ConferencePaper& paper) {
    bool found = false;
    for (auto &it: papers)
        if (it->getID() == paper.getID())
            found = true;
    if (!found)
        papers.push_back(&paper);
    paper.setStatus(1);
}
void Conference::addPaper(ConferencePaper& paper) {
    bool found = false;
    for (auto &it: papers)
        if (it->getID() == paper.getID())
            found = true;
    if (!found)
        papers.push_back(&paper);
}
void Conference::addReviewer(Reviewer& reviewer) {
    for (auto &it: reviewers)
        if (it->getName() == reviewer.getName())
            throw invalid_argument(reviewer.getName());
    reviewers.push_back(&reviewer);
}
void Conference::assignPaperToReviewer(int paperID, string reviewerName) {
    bool ok = false;
    for (auto &it: papers) {
        if (it->getID() == paperID) {
            ok = true;
            if (it->getStatus() != "Submitted")
                throw paperID;
            vector<string> authors = it->getAuthors();
            for (auto &i: authors)
                if (i == reviewerName)
                    throw paperID;
            for (auto &i: reviewers) {
                i->assignPaper(*it);
            }
        }
    }
    if (!ok)
        throw paperID;
}
void Conference::printAllPapers() const {
    int size = papers.size();
    for (int i = size-1; i >= 0; i--)
        papers[i]->display();
}
void Conference::decideAcceptance() {
    for (auto &it: papers) {
        if (it->averageScore() < 7.0)
            it->setStatus(4);
        else
            it->setStatus(3);
    }
}
void Conference::printAllReviewers() const {
    for (auto &it: reviewers)
        it->display();
}
void Conference::display() const {
    printAllReviewers();
    printAllPapers();
}
ConferencePaper* Conference::findPaper(int id) {
    ConferencePaper* found = nullptr;
    for (auto &it: papers)
        if (it->getID() == id)
            return it;
    if (!found)
        throw id;
    return nullptr;
}
Reviewer* Conference::findReviewer(string name) {
    Reviewer* found = nullptr;
    for (auto &it: reviewers)
        if (it->getName() == name)
            return it;
    if (!found)
        throw name;
    return nullptr;
}