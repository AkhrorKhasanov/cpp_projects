#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct User
{
    string username;
    string password;
    double balance;
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
    double bl;
    while (file >> un >> pw >> bl)
    {
        users.push_back({un, pw, bl});
    }
    file.close();
    return users;
}

void saveAllUsers(const vector<User> &users)
{
    ofstream file("database.txt");
    for (const auto &u : users)
    {
        file << u.username << " " << u.password << " " << u.balance << endl;
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
    file << un << " " << processPassword(pw) << " " << 0.0 << endl;
    file.close();
    cout << "You are registered!" << endl;
}

void deposit(double dp, string loggedUserIn)
{

    if (dp <= 0)
    {
        cout << "Enter valid deposit" << endl;
    }
    else
    {

        vector<User> users = loadUsers();
        for (int i = 0; i < users.size(); i++)
        {
            if (users[i].username == loggedUserIn)
            {
                users[i].balance += dp;
                break;
            }
        }
        saveAllUsers(users);
        cout << "Your deposit has been increased!" << endl;
    }
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
                    cout << "1. Deposit\n2. Withdraw\n3. Change password\n4. Delete account\n5. Logout\nChoosen: ";
                    cin >> userChoice;

                    if (userChoice == 1)
                    {
                        double dp;
                        cout << "Enter your deposit: ";
                        cin >> dp;
                        deposit(dp, loggedInUser);
                    }
                    // else if (userChoice == 2)
                    // {
                    //     int wt;
                    //     cout << "Enter your withdraw: ";
                    //     cin >> wt;
                    //     withdraw(wt);
                    // }
                    else if (userChoice == 3)
                    {
                        changePassword(loggedInUser);
                    }
                    else if (userChoice == 4)
                    {
                        if (deleteAccount(loggedInUser))
                            break;
                    }
                    // else if (userChoice ==)
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
