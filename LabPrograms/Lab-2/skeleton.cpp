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

class User {
    string username;
    bool loggedIn = false;

    Post* posts[LIMIT];
    int postsDone = 0;

    void freePost(Post* post) {
        // should free the photo-memory of the post object.
    }

    void doCompaction(int index) {
        // post at index is not deleted, so move back the remaining posts one position left.
    }

public:
    void setUsername(string name) {
        // write setter statement
    }

    string getUsername() {
        // write getter statement and delete the dummy return statement below
        return "";
    }

    void logout() {
        // set the corresponding flag properly
    }

    void login() {
        // set the corresponding flag properly
        cout << "Welcome, " << username << "!\n";
    }

    bool isLoggedIn() {
        // write the correct return statement and delete the dummy statement below
        return false;
    }

    bool createPost(Photo* photo, string caption) {

        /* if the
         * user is not logged in, or
         * Photo* is nullptr, or
         * number of posts done already has reached LIMIT,
         * no post should be created.
         * implement this by proper flag checking */

        /* create a new Post object using dynamic memory
         * Assign photo and the caption.
         * Insert the Post object in its member data structure attribute.
         * Update post count.
         */

        return true;
    }

    int getPostsCount() {
        // return the proper count and delete the dummy statement below
        return 0;
    }

    bool showPosts() {
        // if no posts, return false

        /* otherwise, iterate through the posts in creation order ()
         * and use the following cout for each i-th post to show,
         *               cout << "[" << i << "]\t";
                        post[i]->show();
         * and then return true */
        return true;
    }

    bool deletePost(int index) {
        /* if the user is not logged in, or
         * the requested index is invalid,
         * do not delete.
         * return false.
         */

        /* free the memory of the post */

        doCompaction(index);

        return true;
    }

    void clearPosts() {
        /*
         * Free memory of all the posts and update the counter
         */
    }
};

class Cistagram {
    User* allUsers[LIMIT];
    int userCount = 0;

public:
    int getUserCount() {
        return userCount;
    }

    User* findUserByName(string username) {
        /* return the user which matches the name
         * return nullptr if nothing matches.
         * Do appropriate cout to convey the message
         */
    }

    User* registerUser(string username) {

        /*
         * If number of users exceeds the limit or if the username is already taken,
         * cannot do registration.
         * Return nullptr and do appropriate cout to convey this message.
         */

        /*Create a user by allocating dynamic memory,
         * assign username.
         */

        cout << "Registered successfully. You can now log in.\n";

        /* return the proper object and delete the dummy return statement below*/
        return nullptr;
    }

    User* loginUser(string username) {
        /*
         * find the user from the user array using appropriate method.
         * Do not naively iterate to find.
         */

        /*
         * make the user login by calling the appropriate user method.
         */

        /* return the proper object and delete the dummy return statement below*/
        return nullptr;
    }

    void runLoggedInMenu(User* currentUser) {
        /*
         * return if input is nullptr
         */

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

                /* Create a photo object by dynamically allocating memory,
                 * and create a post using it.
                 * If post could not be created,
                 * free the photo memory.
                 * Do proper cout to convey what happened, post created or not.
                 */

            } else if (choice_enum == USER_CHOICE::SCROLL_DOWN) {
                cout << "Your posts:\n";
                /*
                 * call proper method on proper object to display the posts.
                 * Code should not be more than one line here.
                 */

            } else if (choice_enum == USER_CHOICE::DELETE_POST) {
                int index;

                /* Show all the posts first, so that the user can decide which one to delete*/

                    cout << "Enter the index of the post to delete: ";
                    cin >> index;

                /* delete if the index is valid,
                 * do proper cout to convey the message
                 */

            } else if (choice_enum == USER_CHOICE::LOGOUT) {
                /* make the user logout */
                cout << "Logged out.\n";

            } else {
                cout << "Invalid choice.\n";
                cin.ignore();
            }
        }
    }

    void clearUsers() {
        /*
         * free all user memory and update the count
         */
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

    app.clearUsers();

    cout << "Goodbye!\n";

    return 0;
}
