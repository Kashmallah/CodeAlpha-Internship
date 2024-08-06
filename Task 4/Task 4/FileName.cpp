#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

// Transaction class
class Transaction {
public:
    string transactionID;
    string date;
    string type;
    double amount;
    string details;

    void recordTransaction(const string& id, const string& date, const string& type, double amount, const string& details) {
        transactionID = id;
        this->date = date;
        this->type = type;
        this->amount = amount;
        this->details = details;
    }

    string getTransactionDetails() const {
        stringstream ss;
        ss << "ID: " << transactionID << ", Date: " << date << ", Type: " << type << ", Amount: " << amount << ", Details: " << details;
        return ss.str();
    }
};

// Account class
class Account {
private:
    string accountNumber;
    string accountType;
    double balance;
    vector<Transaction> transactions;

public:
    Account(const string& number, const string& type, double initialBalance) : accountNumber(number), accountType(type), balance(initialBalance) {}

    void deposit(double amount) {
        balance += amount;
        Transaction t;
        t.recordTransaction("T" + to_string(rand() % 1000), "2024-08-01", "Deposit", amount, "Deposited into account " + accountNumber);
        transactions.push_back(t);
    }

    bool withdraw(double amount) {
        if (amount > balance) {
            cout << "Insufficient balance!" << endl;
            return false;
        }
        balance -= amount;
        Transaction t;
        t.recordTransaction("T" + to_string(rand() % 1000), "2024-08-01", "Withdrawal", amount, "Withdrawn from account " + accountNumber);
        transactions.push_back(t);
        return true;
    }

    bool transfer(double amount, Account& targetAccount) {
        if (withdraw(amount)) {
            targetAccount.deposit(amount);
            Transaction t;
            t.recordTransaction("T" + to_string(rand() % 1000), "2024-08-01", "Transfer", amount, "Transferred to account " + targetAccount.accountNumber);
            transactions.push_back(t);
            return true;
        }
        return false;
    }

    double viewBalance() const {
        return balance;
    }

    vector<Transaction> viewTransactions() const {
        return transactions;
    }

    string getAccountNumber() const {
        return accountNumber;
    }
};

// Customer class
class Customer {
private:
    string customerID;
    string name;
    string address;
    string phoneNumber;
    string email;
    vector<Account> accounts;

public:
    Customer(const string& id, const string& name, const string& address, const string& phone, const string& email)
        : customerID(id), name(name), address(address), phoneNumber(phone), email(email) {}

    void createAccount(const string& number, const string& type, double initialBalance) {
        accounts.emplace_back(number, type, initialBalance);
    }

    void updateDetails(const string& address, const string& phone, const string& email) {
        this->address = address;
        this->phoneNumber = phone;
        this->email = email;
    }

    void viewAccountInfo() const {
        cout << "Customer ID: " << customerID << endl;
        cout << "Name: " << name << endl;
        cout << "Address: " << address << endl;
        cout << "Phone: " << phoneNumber << endl;
        cout << "Email: " << email << endl;
        for (const auto& account : accounts) {
            cout << "Account Number: " << account.getAccountNumber() << endl;
            cout << "Balance: " << account.viewBalance() << endl;
        }
    }

    bool deleteAccount(const string& accountNumber) {
        auto it = remove_if(accounts.begin(), accounts.end(), [&accountNumber](const Account& acc) {
            return acc.getAccountNumber() == accountNumber;
            });
        if (it != accounts.end()) {
            accounts.erase(it, accounts.end());
            return true;
        }
        return false;
    }

    string getCustomerID() const {
        return customerID;
    }

    vector<Account>& getAccounts() {
        return accounts;
    }
};

// BankingService class
class BankingService {
private:
    string bankName;
    string bankAddress;
    vector<Customer> customers;

public:
    BankingService(const string& name, const string& address) : bankName(name), bankAddress(address) {}

    void createCustomer(const string& id, const string& name, const string& address, const string& phone, const string& email) {
        customers.emplace_back(id, name, address, phone, email);
    }

    bool deleteCustomer(const string& customerID) {
        auto it = remove_if(customers.begin(), customers.end(), [&customerID](const Customer& cust) {
            return cust.getCustomerID() == customerID;
            });
        if (it != customers.end()) {
            customers.erase(it, customers.end());
            return true;
        }
        return false;
    }

    Customer* getCustomerDetails(const string& customerID) {
        for (auto& customer : customers) {
            if (customer.getCustomerID() == customerID) {
                return &customer;
            }
        }
        return nullptr;
    }

    bool processTransaction(const string& customerID, const string& fromAccountNumber, double amount, const string& type, const string& details, const string& toAccountNumber = "") {
        Customer* customer = getCustomerDetails(customerID);
        if (!customer) {
            cout << "Customer not found!" << endl;
            return false;
        }

        auto& accounts = customer->getAccounts();
        auto fromIt = find_if(accounts.begin(), accounts.end(), [&fromAccountNumber](const Account& acc) {
            return acc.getAccountNumber() == fromAccountNumber;
            });

        if (fromIt == accounts.end()) {
            cout << "Source account not found!" << endl;
            return false;
        }

        if (type == "Deposit") {
            fromIt->deposit(amount);
        }
        else if (type == "Withdrawal") {
            return fromIt->withdraw(amount);
        }
        else if (type == "Transfer") {
            auto toIt = find_if(accounts.begin(), accounts.end(), [&toAccountNumber](const Account& acc) {
                return acc.getAccountNumber() == toAccountNumber;
                });
            if (toIt == accounts.end()) {
                cout << "Target account not found!" << endl;
                return false;
            }
            return fromIt->transfer(amount, *toIt);
        }

        Transaction t;
        t.recordTransaction("T" + to_string(rand() % 1000), "2024-08-01", type, amount, details);
        fromIt->viewTransactions().push_back(t);
        return true;
    }
};

int main() {
    BankingService bank("MyBank", "123 Bank St");

    while (true) {
        cout << "\nBanking System Menu:\n";
        cout << "1. Create Customer\n";
        cout << "2. Create Account\n";
        cout << "3. Deposit\n";
        cout << "4. Withdraw\n";
        cout << "5. Transfer\n";
        cout << "6. View Account Info\n";
        cout << "7. Delete Account\n";
        cout << "8. Delete Customer\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;
        cin.ignore(); // To ignore any newline characters left in the input buffer

        if (choice == 9) break;

        switch (choice) {
        case 1: {
            string id, name, address, phone, email;
            cout << "Enter customer ID: ";
            getline(cin, id);
            cout << "Enter customer name: ";
            getline(cin, name);
            cout << "Enter customer address: ";
            getline(cin, address);
            cout << "Enter customer phone: ";
            getline(cin, phone);
            cout << "Enter customer email: ";
            getline(cin, email);
            bank.createCustomer(id, name, address, phone, email);
            cout << "Customer created successfully!" << endl;
            break;
        }

        case 2: {
            string customerID, number, type;
            double initialBalance;
            cout << "Enter customer ID: ";
            getline(cin, customerID);
            Customer* customer = bank.getCustomerDetails(customerID);
            if (customer) {
                cout << "Enter account number: ";
                getline(cin, number);
                cout << "Enter account type: ";
                getline(cin, type);
                cout << "Enter initial balance: ";
                cin >> initialBalance;
                cin.ignore(); // To ignore any newline characters left in the input buffer
                customer->createAccount(number, type, initialBalance);
                cout << "Account created successfully!" << endl;
            }
            else {
                cout << "Customer not found!" << endl;
            }
            break;
        }

        case 3: {
            string customerID, accountNumber;
            double amount;
            cout << "Enter customer ID: ";
            getline(cin, customerID);
            Customer* customer = bank.getCustomerDetails(customerID);
            if (customer) {
                cout << "Enter account number: ";
                getline(cin, accountNumber);
                cout << "Enter amount to deposit: ";
                cin >> amount;
                cin.ignore(); // To ignore any newline characters left in the input buffer
                auto& accounts = customer->getAccounts();
                auto it = find_if(accounts.begin(), accounts.end(), [&accountNumber](const Account& acc) {
                    return acc.getAccountNumber() == accountNumber;
                    });
                if (it != accounts.end()) {
                    it->deposit(amount);
                    cout << "Deposit successful!" << endl;
                }
                else {
                    cout << "Account not found!" << endl;
                }
            }
            else {
                cout << "Customer not found!" << endl;
            }
            break;
        }

        case 4: {
            string customerID, accountNumber;
            double amount;
            cout << "Enter customer ID: ";
            getline(cin, customerID);
            Customer* customer = bank.getCustomerDetails(customerID);
            if (customer) {
                cout << "Enter account number: ";
                getline(cin, accountNumber);
                cout << "Enter amount to withdraw: ";
                cin >> amount;
                cin.ignore(); // To ignore any newline characters left in the input buffer
                auto& accounts = customer->getAccounts();
                auto it = find_if(accounts.begin(), accounts.end(), [&accountNumber](const Account& acc) {
                    return acc.getAccountNumber() == accountNumber;
                    });
                if (it != accounts.end()) {
                    if (it->withdraw(amount)) {
                        cout << "Withdrawal successful!" << endl;
                    }
                }
                else {
                    cout << "Account not found!" << endl;
                }
            }
            else {
                cout << "Customer not found!" << endl;
            }
            break;
        }

        case 5: {
            string customerID, fromAccountNumber, toAccountNumber, details;
            double amount;
            cout << "Enter customer ID: ";
            getline(cin, customerID);
            Customer* customer = bank.getCustomerDetails(customerID);
            if (customer) {
                cout << "Enter source account number: ";
                getline(cin, fromAccountNumber);
                cout << "Enter target account number: ";
                getline(cin, toAccountNumber);
                cout << "Enter amount to transfer: ";
                cin >> amount;
                cin.ignore(); // To ignore any newline characters left in the input buffer
                cout << "Enter transaction details: ";
                getline(cin, details);
                if (bank.processTransaction(customerID, fromAccountNumber, amount, "Transfer", details, toAccountNumber)) {
                    cout << "Transfer successful!" << endl;
                }
            }
            else {
                cout << "Customer not found!" << endl;
            }
            break;
        }

        case 6: {
            string customerID;
            cout << "Enter customer ID: ";
            getline(cin, customerID);
            Customer* customer = bank.getCustomerDetails(customerID);
            if (customer) {
                customer->viewAccountInfo();
            }
            else {
                cout << "Customer not found!" << endl;
            }
            break;
        }

        case 7: {
            string customerID, accountNumber;
            cout << "Enter customer ID: ";
            getline(cin, customerID);
            Customer* customer = bank.getCustomerDetails(customerID);
            if (customer) {
                cout << "Enter account number to delete: ";
                getline(cin, accountNumber);
                if (customer->deleteAccount(accountNumber)) {
                    cout << "Account deleted successfully!" << endl;
                }
                else {
                    cout << "Account not found!" << endl;
                }
            }
            else {
                cout << "Customer not found!" << endl;
            }
            break;
        }

        case 8: {
            string customerID;
            cout << "Enter customer ID: ";
            getline(cin, customerID);
            if (bank.deleteCustomer(customerID)) {
                cout << "Customer deleted successfully!" << endl;
            }
            else {
                cout << "Customer not found!" << endl;
            }
            break;
        }

        default:
            cout << "Invalid choice! Please enter a valid option." << endl;
        }
    }

    cout << "Exiting Banking System. Goodbye!" << endl;
    return 0;
}
