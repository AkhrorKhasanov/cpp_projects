#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct User
{
    string username;
    string password;
};

string processPassword(string data)
{
    char key = 'K';
    for (int i = 0; i < data.length(); i++)
    {
        data[i] ^= key;
    }
    return data;
}

vector<User> loadUsers()
{
    vector<User> users;
    ifstream file("database.txt");
    string un, pw;
    while (file >> un >> pw)
    {
        users.push_back({un, pw});
    }
    file.close();
    return users;
}

void saveAllUsers(const vector<User> &users)
{
    ofstream file("database.txt");
    for (const auto &u : users)
    {
        file << u.username << " " << u.password << endl;
    }
    file.close();
}

bool deleteAccount(string currentLoggedUser)
{
    char confirm;
    cout << "\nWarning! Do you really want to delete your account? (y/n): ";
    cin >> confirm;

    if (confirm == 'y' || confirm == 'Y')
    {
        vector<User> users = loadUsers();
        vector<User> updateUsers;

        for (const auto &u : users)
        {
            if (u.username != currentLoggedUser)
            {
                updateUsers.push_back(u);
            }
        }
        saveAllUsers(updateUsers);
        cout << "Account has been deleted successfully. Bye!\n";
        return true;
    }
    return false;
}

void changePassword(string currentLoggedUser)
{
    string newPass;
    cout << "\nEnter a new password: ";
    cin >> newPass;

    vector<User> users = loadUsers();
    for (int i = 0; i < users.size(); i++)
    {
        if (users[i].username == currentLoggedUser)
        {
            users[i].password = processPassword(newPass);
            break;
        }
    }
    saveAllUsers(users);
    cout << "Password has been changed!" << endl;
}

bool loginUser(string &currentLoggedUser)
{
    string inputUser, inputPass;
    cout << "\n--- Enter ---" << endl;
    cout << "Login: ";
    cin >> inputUser;
    cout << "Password: ";
    cin >> inputPass;

    ifstream file("database.txt");
    string fUser, fPass;
    while (file >> fUser >> fPass)
    {
        if (fUser == inputUser && processPassword(fPass) == inputPass)
        {
            currentLoggedUser = fUser;
            return true;
        }
    }
    return false;
}

void registerUser()
{
    string un, pw;
    cout << "\nNew login: ";
    cin >> un;

    vector<User> users = loadUsers();
    for (auto const &u : users)
    {
        if (u.username == un)
        {
            cout << "Login taken!" << endl;
            return;
        }
    }

    cout << "New password: ";
    cin >> pw;
    ofstream file("database.txt", ios::app);
    file << un << " " << processPassword(pw) << endl;
    file.close();
    cout << "You are registered!" << endl;
}

int main()
{
    int choice;
    while (true)
    {
        cout << "\n1. Registration\n2. Login\n3. Logout\nChoosen: ";
        cin >> choice;
        if (choice == 1)
        {
            registerUser();
        }
        else if (choice == 2)
        {
            string loggedInUser = "";
            if (loginUser(loggedInUser))
            {
                int userChoice;
                while (true)
                {
                    cout << "\n--- PERSONAL CABINET (" << loggedInUser << ") ---" << endl;
                    cout << "1. Change password\n2. Delete account\n3. Logout\nChoosen: ";
                    cin >> userChoice;
                    
                    if (userChoice == 1)
                    {
                        changePassword(loggedInUser);
                    }
                    else if (userChoice == 2)
                    {
                        if (deleteAccount(loggedInUser))
                            break;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else
            {
                cout << "Error!" << endl;
            }
        }
        else if (choice == 3)
            break;
    }
    return 0;
}
