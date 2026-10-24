// This Lab is essentially just about making methods for classes and initializing structs

/**************************************  NO EDIT ZONE *******************************/

#include <iostream>
#include <string>

using namespace std;

#define LIMIT 10
#define GRID_SIZE 5
#define MAX_FOLLOWERS 10
#define MAX_FOLLOWING LIMIT
#define MAX_FEEDS 100

enum class CISTA_CHOICE {
    LOGIN = 1,
    REGISTER,
    EXIT
};

enum class USER_CHOICE {
    CREATE_POST = 1,
    SCROLL_DOWN,
    DELETE_POST,
    FOLLOW_USER,
    VIEW_FEED,
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
    // Constructor
    Photo() = default;

    // Copy Constructor
    Photo(const Photo& other)
    {
        seed = other.seed;
        for (int i = 0; i < GRID_SIZE; i++)
        {
            for (int j = 0; j < GRID_SIZE; j++)
            {
                content[i][j] = other.content[i][j];
            }
        }
        timestamp = other.timestamp;
        shot_at = other.shot_at;
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

    User* followers[MAX_FOLLOWERS];
    User* following[MAX_FOLLOWING];
    int followerCount = 0;
    int followingCount = 0;
    
    Post* feeds[MAX_FEEDS];
    int feedCount = 0;

    void freePost(Post* post) {
        delete post->photo;
    }

    void doCompaction(int index) {
        for (int i = index; i < postsDone-1; i++) posts[i] = posts[i+1];
    }

    
    
    public:    
    void freePostAt(int index)
    {
        Post* post = posts[index];
        for (int i = 0; i < followerCount; i++) followers[i]->removeFromFeed(post);
        freePost(post);
        delete posts[index];
        doCompaction(index);
        postsDone--;
    }

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

        for (int i = 0; i < followerCount; i++)  followers[i]->addToFeed(post);

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

        freePostAt(index);
        return true;
    }
    
    bool addFollower(User* user)
    {
        if (user == nullptr || user == this || followerCount >= MAX_FOLLOWERS) return false;
        for (int i = 0; i < followerCount; i++)
            if (followers[i] == user) return false;

        followers[followerCount] = user;
        followerCount++;
        return true;
    }

    void removeFollower(User* user)
    {
        int index = -1;
        for (int i = 0; i < followerCount; i++)
        {
            if (followers[i] == user)
            {
                index = i;
                break;
            }
        }
        if (index == -1) return;

        for (int i = index; i < followerCount-1; i++) followers[i] = followers[i+1];
        followerCount--;
    }

    bool addFollowing(User* user)
    {
        if (user == nullptr || user == this || followingCount >= MAX_FOLLOWING) return false;
        for (int i = 0; i < followingCount; i++)
            if (following[i] == user) return false;

        following[followingCount] = user;
        followingCount++;
        return true;
    }

    void removeFollowing(User* user)
    {
        int index = -1;
        for (int i = 0; i < followingCount; i++)
        {
            if (following[i] == user)
            {
                index = i;
                break;
            }
        }
        if (index == -1) return;

        for (int i = index; i < followingCount-1; i++) following[i] = following[i+1];
        followingCount--;   
    }

    void showFollowers() const 
    {
        for (int i = 0; i < followerCount; i++)
        {
            cout << "[" << i << "]\t" << endl;
            cout << "  " << followers[i]->getUsername() << endl;
        }
    }

    bool follow(User* user)
    {
        if (user == nullptr) return false;

        if (!user->addFollower(this)) return false;
        if (!this->addFollowing(user)) 
        {
            user->removeFollower(this);   
            return false;
        }
        return true;
    }

    bool addToFeed(Post* post)
    {
        if (post == nullptr || feedCount >= MAX_FEEDS) return false;
        for (int i = 0; i < feedCount; i++)
            if (feeds[i] == post) return false;

        feeds[feedCount] = post;
        feedCount++;
        return true;
    }

    void removeFromFeed(Post* post)
    {
        int index = -1;
        for (int i = 0; i < feedCount; i++)
        {
            if (feeds[i] == post)
            {
                index = i;
                break;
            }
        }
        if (index == -1) return;

        for (int i = index; i < feedCount-1; i++) feeds[i] = feeds[i+1];
        feedCount--; 
    }

    // resubmission check (it is a const)
    bool showFeed() const
    {
        if (feedCount == 0) return false;

        for (int i = 0; i < feedCount; i++)
        {
            cout << "[" << i << "]\t" << endl;
            feeds[i]->show();
        }
        return true;
    }

    int getFeedCount() const
    {
        return feedCount;
    }
    
    int getFollowerCount() const
    {
        return followerCount;
    }

    int getFollowingCount() const
    {
        return followingCount;
    }

    /*
    void clearPosts() {
        while (postsDone > 0) deletePost(0); // I delete post[0] because we shift everything leftwards
    }
*/

    ~User()
    {
        while (postsDone > 0) freePostAt(0);
        for (int i = 0; i < followingCount; i++) following[i]->removeFollower(this);
        for (int i = 0; i < followerCount;  i++) followers[i]->removeFollowing(this);
        followingCount = 0;
        followerCount  = 0;
        // Feed is automatically cleared if following are cleared
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


    bool followUser(User* me, const string& name) const
    {       
        if (me == nullptr) return false;
        User* followedUser = findUserByName(name);
        return me->follow(followedUser);
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
            cout << "4. Follow User\n";
            cout << "5. View Feed\n";
            cout << "6. Logout\n";
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

            } else if (choice_enum == USER_CHOICE::FOLLOW_USER) {
                string name;
                cout << "Enter the name of the user you want to follow," << endl;
                cin >> name;

                if (!followUser(currentUser, name)) cout << "Can't follow user" << endl;

            } else if (choice_enum == USER_CHOICE::VIEW_FEED) {
                cout << "Your Feed:\n";
                currentUser->showFeed();

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
