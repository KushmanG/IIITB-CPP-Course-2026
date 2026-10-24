// This Lab is essentially just about making methods for classes and initializing structs

/**************************************  NO EDIT ZONE *******************************/

#include <iostream>
#include <string>

using namespace std;

#define LIMIT 10
#define GRID_SIZE 5

enum class CISTA_CHOICE {
    LOGIN = 1,
    REGISTER,
    EXIT
};

enum class USER_CHOICE {
    CREATE_POST = 1,
    SCROLL_DOWN,
    DELETE_POST,
    LOGOUT
};

struct Photo {
    int seed;
    int content[GRID_SIZE][GRID_SIZE];
    string timestamp;
    string shot_at;

    void show() {
        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                if (content[i][j]) {
                    cout << "*";
                } else {
                    cout << " ";
                }
            }
        }
        cout << " shot at: " << shot_at << endl;
    }
};

struct Post {
    Photo* photo;
    string caption;

    void show() {
        cout << caption << endl;
        if (photo != nullptr) {
            photo->show();
        }
    }
};

/*
 * Zero-pad a non-negative number to at least two digits.
 *
 * 0   -> "00"
 * 7   -> "07"
 * 10  -> "10"
 * 123 -> "123"
 *
 * Negative numbers are returned normally rather than producing
 * unusual values such as "0-1".
 */
string pad2(int n) {
    if (n < 0) {
        return to_string(n);
    }

    if (n < 10) {
        return "0" + to_string(n);
    }

    return to_string(n);
}

Photo* createRandomPhoto(int seed) {
    srand(seed);

    Photo* photo = new Photo;
    photo->seed = seed;

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            photo->content[i][j] = rand() % 2;
        }
    }

    int day = 1 + rand() % 28;
    int month = 1 + rand() % 12;
    int year = 2026 + rand() % 7;
    int hour = rand() % 24;
    int minute = rand() % 60;
    int second = rand() % 60;

    photo->timestamp =
        pad2(day) + "-" +
        pad2(month) + "-" +
        to_string(year) + ":" +
        pad2(hour) + ":" +
        pad2(minute) + ":" +
        pad2(second);

    string locations[5] = {
        "Bangalore",
        "Mumbai",
        "Delhi",
        "Chennai",
        "Kolkata"
    };

    photo->shot_at = locations[rand() % 5];

    return photo;
}

/************************************** END OF NO EDIT ZONE *******************************/

const Photo* imgs[1000];
int totalImgs = 0;

void registerExternalPhoto(const Photo* photo)
{
    for (int i = 0; i < totalImgs; i++)
    {
        if (imgs[i] == photo) return;
    }

    imgs[totalImgs] = photo;
    totalImgs++;
}

void clearExternalPhotos()
{
    for (int i = 0; i < totalImgs; i++) delete imgs[i];
    totalImgs = 0;
}

class User {
    string username;
    bool loggedIn = false;

    Post* posts[LIMIT];
    int postsDone = 0;


    void freePost(Post* post) {
        delete post->photo;
    }

    void doCompaction(int index) {
        for (int i = index; i < postsDone-1; i++) posts[i] = posts[i+1];
    }


public:
    User(const string& username)
    {
        this->username = username;
    }

    string getUsername() const {
        return this->username;
    }

    void logout() {
        loggedIn = false;
    }

    void login() {
        loggedIn = true;
        cout << "Welcome, " << username << "!\n";
    }

    bool isLoggedIn() const {
        return loggedIn;
    }

    bool createPost(const Photo* photo, const string& caption) {
        if (loggedIn == false || photo == nullptr || postsDone >= LIMIT) return false;
        if (totalImgs >= 1000) 
        {
            cout << "No space is left in the device" << endl;
            return false;
        }

        Photo* photo_copy = new Photo(*photo);
        Post* post = new Post{photo_copy, caption};

        registerExternalPhoto(photo);

        posts[postsDone] = post;
        postsDone++;
        return true;
    }

    int getPostsCount() const {
        return postsDone;
    }

    bool showPosts() const {
        if (postsDone == 0) return false;

        for (int i = 0; i < postsDone; i++)
        {
            cout << "[" << i << "]\t" << endl;
            posts[i]->show();
        }
        return true;
    }

    bool deletePost(int index) {
        if (loggedIn == false || index < 0 || index >= postsDone) return false;

        Post* post = posts[index];
        freePost(post);
        delete posts[index];
        doCompaction(index);
        postsDone--;
        return true;
    }
/*
    void clearPosts() {
        while (postsDone > 0) deletePost(0); // I delete post[0] because we shift everything leftwards
    }
*/

    ~User()
    {
        while (postsDone > 0) deletePost(0);
    }
};

class Cistagram {
    User* allUsers[LIMIT];
    int userCount = 0;

public:
    int getUserCount() const {
        return userCount;
    }

    User* findUserByName(const string& username) const {
        for (int i = 0; i < userCount; i++)
        {
            if (allUsers[i]->getUsername() == username) return allUsers[i];
        }
        cout << "No such user found" << endl;
        return nullptr;
    }

    User* registerUser(const string& username) {
        if (userCount >= LIMIT) 
        {
            cout << "Registration could not be done since maximum user count reached." << endl;
            return nullptr;
        }

        else if (findUserByName(username) != nullptr)
        {
            cout << "Registration could not be done because Username is already taken." << endl;
            return nullptr;
        }

        User* user = new User(username);
        allUsers[userCount] = user;
        userCount++;

        cout << "Registered successfully. You can now log in.\n";
        return user;
    }

    User* loginUser(const string& username) const {
        User* user = findUserByName(username);

        if (user == nullptr) return nullptr;
        else
        {
            user->login();
            return user;
        }
    }

    void runLoggedInMenu(User* currentUser) const {
        if (currentUser == nullptr) return;

        int choice;
        USER_CHOICE choice_enum;

        while (currentUser->isLoggedIn()) {
            cout << "\n--- Logged in as "
                 << currentUser->getUsername()
                 << " ---\n";

            cout << "1. Create a post\n";
            cout << "2. Scroll my posts\n";
            cout << "3. Delete a post\n";
            cout << "4. Logout\n";
            cout << "Choice: ";

            cin >> choice;
            choice_enum = static_cast<USER_CHOICE>(choice);

            if (choice_enum == USER_CHOICE::CREATE_POST) {
                int seed;
                string caption;

                cout << "Enter a random seed: ";
                cin >> seed;

                cin.ignore();

                cout << "Enter a caption (without any whitespace): ";
                cin >> caption;

                Photo* photo = createRandomPhoto(seed);
                bool flag = currentUser->createPost(photo, caption);
                if (!flag) 
                {
                    delete photo;
                    cout << "Post couldn't be created." << endl;
                } 
                else cout << "Post Created!" << endl;
                

            } else if (choice_enum == USER_CHOICE::SCROLL_DOWN) {
                cout << "Your posts:\n";
                currentUser->showPosts();

            } else if (choice_enum == USER_CHOICE::DELETE_POST) {
                int index;
                currentUser->showPosts();
                cout << "Enter the index of the post to delete: ";
                cin >> index;
                bool flag = currentUser->deletePost(index);
                if (flag == false) cout << "Invalid Index, Post couldn't be deleted." << endl;
                else cout << "Post deleted." << endl;

            } else if (choice_enum == USER_CHOICE::LOGOUT) {
                currentUser->logout();
                cout << "Logged out.\n";

            } else {
                cout << "Invalid choice.\n";
                cin.ignore();
            }
        }
    }

    ~Cistagram()
    {
        while (userCount > 0) 
        {
            delete allUsers[userCount-1];
            userCount--;
        }
        clearExternalPhotos();   
    }
};

/********************************** DO NOT EDIT THE MAIN ******************************************/

int main() {
    int cista_choice;
    CISTA_CHOICE choice;
    bool exitApp = false;

    Cistagram app;

    while (!exitApp) {
        cout << "\n===== Welcome =====\n";
        cout << "1. Login\n";
        cout << "2. Register\n";
        cout << "3. Exit\n";
        cout << "Choice: ";

        cin >> cista_choice;
        choice = static_cast<CISTA_CHOICE>(cista_choice);

        if (choice == CISTA_CHOICE::LOGIN) {
            string username;

            cout << "Username: ";
            cin >> username;

            User* currentUser = app.loginUser(username);

            if (currentUser != nullptr) {
                app.runLoggedInMenu(currentUser);
            }

        } else if (choice == CISTA_CHOICE::REGISTER) {
            string username;

            cout << "Choose a username: ";
            cin >> username;

            app.registerUser(username);

        } else if (choice == CISTA_CHOICE::EXIT) {
            exitApp = true;

        } else {
            cout << "Invalid choice.\n";
            cin.ignore();
        }
    }

    cout << "Goodbye!\n";

    return 0;
}
