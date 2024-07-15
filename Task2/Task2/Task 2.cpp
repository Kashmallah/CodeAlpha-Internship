#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <conio.h> // For getch()

using namespace std;

// Function to take password input as hidden
string takePasswordInput() {
    string password;
    char ch;
    cout << "Enter a password: ";
    ch = _getch();
    while (ch != 13) { // Character 13 is Enter
        if (ch != 8) { // Character 8 is Backspace
            password.push_back(ch);
            cout << '*';
        }
        else if (!password.empty()) {
            password.pop_back();
            cout << "\b \b"; // Handle backspace
        }
        ch = _getch();
    }
    cout << endl;
    return password;
}

// Function to register a new user
void registerUser() {
    string username, password;
    cout << "Enter a username: ";
    cin >> username;
    password = takePasswordInput();

    ofstream outfile("database.txt", ios::app); // Append mode
    if (outfile.is_open()) {
        outfile << username << " " << password << endl;
        cout << "Registration successful!" << endl;
        outfile.close();
    }
    else {
        cout << "Error opening file for writing." << endl;
    }
}

// Function to load the credentials from the file into a map
unordered_map<string, string> loadCredentials() {
    unordered_map<string, string> credentials;
    string username, password;
    ifstream infile("database.txt");

    if (infile.is_open()) {
        while (infile >> username >> password) {
            credentials[username] = password;
        }
        infile.close();
    }
    else {
        cout << "Error opening file for reading." << endl;
    }

    return credentials;
}

// Function to log in a user
void loginUser() {
    string username, password;
    cout << "Enter your username: ";
    cin >> username;
    password = takePasswordInput();

    unordered_map<string, string> credentials = loadCredentials();

    if (credentials.find(username) != credentials.end() && credentials[username] == password) {
        cout << "Login successful!" << endl;
    }
    else {
        cout << "Invalid username or password." << endl;
    }
}

int main() {
    int choice;

    while (true) {
        cout << "1. Register\n2. Login\n3. Exit\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            registerUser();
            break;
        case 2:
            loginUser();
            break;
        case 3:
            cout << "Exiting..." << endl;
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
