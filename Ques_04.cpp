#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <stdexcept>

using namespace std;

// Base class for Account
class Account {
public:
    Account(string number, double balance)
        : number(number), balance(balance) {}

    virtual ~Account() {}

    string getNumber() const { return number; }
    double getBalance() const { return balance; }

    virtual void display() const = 0;

    void deposit(double amount) {
        if (amount <= 0) throw invalid_argument("Deposit amount must be positive.");
        balance += amount;
    }

    void withdraw(double amount) {
        if (amount <= 0) throw invalid_argument("Withdrawal amount must be positive.");
        if (amount > balance) throw runtime_error("Insufficient funds.");
        balance -= amount;
    }

protected:
    string number;
    double balance;
};

// Derived class for Savings Account
class SavingsAccount : public Account {
public:
    SavingsAccount(string number, double balance)
        : Account(number, balance) {}

    void display() const override {
        cout << "Savings Account Number: " << number << ", Balance: $" << balance << endl;
    }
};

// Derived class for Current Account
class CurrentAccount : public Account {
public:
    CurrentAccount(string number, double balance)
        : Account(number, balance) {}

    void display() const override {
        cout << "Current Account Number: " << number << ", Balance: $" << balance << endl;
    }
};

// Class for Customer
class Customer {
public:
    Customer(string name, string id)
        : name(name), id(id) {}

    string getName() const { return name; }
    string getID() const { return id; }

    void display() const {
        cout << "Customer Name: " << name << ", ID: " << id << endl;
    }

private:
    string name;
    string id;
};

// Class for Transaction
class Transaction {
public:
    Transaction(shared_ptr<Account> account, double amount, const string& type)
        : account(account), amount(amount), type(type) {}

    void display() const {
        cout << "Transaction Type: " << type << ", Amount: $" << amount << ", Account: " << account->getNumber() << endl;
    }

private:
    shared_ptr<Account> account;
    double amount;
    string type;
};

// Class for Bank
class Bank {
public:
    void addAccount(shared_ptr<Account> account) {
        accounts.push_back(account);
    }

    void addCustomer(shared_ptr<Customer> customer) {
        customers.push_back(customer);
    }

    void deposit(string accountNumber, double amount) {
        auto accountIt = find_if(accounts.begin(), accounts.end(), [&](const shared_ptr<Account>& acc) {
            return acc->getNumber() == accountNumber;
        });

        if (accountIt == accounts.end()) {
            throw runtime_error("Account not found.");
        }

        (*accountIt)->deposit(amount);
        transactions.push_back(make_shared<Transaction>(*accountIt, amount, "Deposit"));
    }

    void withdraw(string accountNumber, double amount) {
        auto accountIt = find_if(accounts.begin(), accounts.end(), [&](const shared_ptr<Account>& acc) {
            return acc->getNumber() == accountNumber;
        });

        if (accountIt == accounts.end()) {
            throw runtime_error("Account not found.");
        }

        (*accountIt)->withdraw(amount);
        transactions.push_back(make_shared<Transaction>(*accountIt, amount, "Withdrawal"));
    }

    void transfer(string fromAccountNumber, string toAccountNumber, double amount) {
        auto fromAccountIt = find_if(accounts.begin(), accounts.end(), [&](const shared_ptr<Account>& acc) {
            return acc->getNumber() == fromAccountNumber;
        });

        auto toAccountIt = find_if(accounts.begin(), accounts.end(), [&](const shared_ptr<Account>& acc) {
            return acc->getNumber() == toAccountNumber;
        });

        if (fromAccountIt == accounts.end()) {
            throw runtime_error("Source account not found.");
        }
        if (toAccountIt == accounts.end()) {
            throw runtime_error("Destination account not found.");
        }

        (*fromAccountIt)->withdraw(amount);
        (*toAccountIt)->deposit(amount);
        transactions.push_back(make_shared<Transaction>(*fromAccountIt, amount, "Transfer Out"));
        transactions.push_back(make_shared<Transaction>(*toAccountIt, amount, "Transfer In"));
    }

    void saveToFile() const {
        ofstream file("bank_data.txt");
        if (!file.is_open()) {
            throw runtime_error("Error opening file for writing.");
        }

        // Save accounts
        file << "Accounts:\n";
        for (const auto& account : accounts) {
            file << typeid(*account).name() << "," << account->getNumber() << "," << account->getBalance() << "\n";
        }

        // Save customers
        file << "Customers:\n";
        for (const auto& customer : customers) {
            file << customer->getName() << "," << customer->getID() << "\n";
        }

        file.close();
    }

    void loadFromFile() {
        ifstream file("bank_data.txt");
        if (!file.is_open()) {
            throw runtime_error("Error opening file for reading.");
        }

        accounts.clear();
        customers.clear();

        string line;
        bool readingAccounts = true;

        while (getline(file, line)) {
            if (line == "Accounts:") {
                readingAccounts = true;
                continue;
            } else if (line == "Customers:") {
                readingAccounts = false;
                continue;
            }

            if (readingAccounts) {
                stringstream ss(line);
                string type, number;
                double balance;
                getline(ss, type, ',');
                getline(ss, number, ',');
                ss >> balance;
                
                if (type == "class SavingsAccount") {
                    accounts.push_back(make_shared<SavingsAccount>(number, balance));
                } else if (type == "class CurrentAccount") {
                    accounts.push_back(make_shared<CurrentAccount>(number, balance));
                }
            } else {
                stringstream ss(line);
                string name, id;
                getline(ss, name, ',');
                getline(ss, id);
                customers.push_back(make_shared<Customer>(name, id));
            }
        }

        file.close();
    }

private:
    vector<shared_ptr<Account>> accounts;
    vector<shared_ptr<Customer>> customers;
    vector<shared_ptr<Transaction>> transactions;
};

// Main function
int main() {
    Bank bank;

    try {
        bank.loadFromFile();

        // Add some accounts and customers for testing
        bank.addAccount(make_shared<SavingsAccount>("S001", 1000.00));
        bank.addAccount(make_shared<CurrentAccount>("C001", 2000.00));
        bank.addCustomer(make_shared<Customer>("Alice", "C001"));
        bank.addCustomer(make_shared<Customer>("Bob", "C002"));

        // Perform some operations
        bank.deposit("S001", 500.00);
        bank.withdraw("C001", 300.00);
        bank.transfer("S001", "C001", 200.00);

        // Save bank data to file
        bank.saveToFile();

    } catch (const runtime_error& e) {
        cerr << "Runtime error: " << e.what() << endl;
    } catch (const invalid_argument& e) {
        cerr << "Invalid argument: " << e.what() << endl;
    }

    return 0;
}
