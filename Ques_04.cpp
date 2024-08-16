#include <bits/stdc++.h>
using namespace std;

// Base Account Class
class Account {
public:
    Account(int number, double balance) : accountNumber(number), balance(balance) {}

    virtual ~Account() = default;

    int getAccountNumber() const { return accountNumber; }
    double getBalance() const { return balance; }

    virtual void deposit(double amount) {
        if (amount <= 0) {
            throw invalid_argument("Deposit amount must be positive.");
        }
        balance += amount;
    }

    virtual void withdraw(double amount) {
        if (amount <= 0) {
            throw invalid_argument("Withdrawal amount must be positive.");
        }
        if (amount > balance) {
            throw runtime_error("Insufficient funds.");
        }
        balance -= amount;
    }

    virtual string getAccountType() const = 0;

    virtual void display() const {
        cout << "Account Number: " << accountNumber
             << ", Type: " << getAccountType()
             << ", Balance: $" << balance << endl;
    }

protected:
    int accountNumber;
    double balance;
};

// Savings Account Class
class SavingsAccount : public Account {
public:
    SavingsAccount(int number, double balance) : Account(number, balance) {}

    string getAccountType() const override { return "Savings"; }
};

// Current Account Class
class CurrentAccount : public Account {
public:
    CurrentAccount(int number, double balance) : Account(number, balance) {}

    string getAccountType() const override { return "Current"; }
};

// Transaction Class
class Transaction {
public:
    Transaction(int fromAccount, int toAccount, double amount, const string& type)
        : fromAccountNumber(fromAccount), toAccountNumber(toAccount), amount(amount), type(type) {
        time(&timestamp);
    }

    string toString() const {
        char timeStr[20];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localtime(&timestamp));
        ostringstream oss;
        oss << "Transaction: " << type
            << " from Account " << fromAccountNumber
            << " to Account " << toAccountNumber
            << " Amount: $" << amount
            << " Date: " << timeStr;
        return oss.str();
    }

private:
    int fromAccountNumber;
    int toAccountNumber;
    double amount;
    string type;
    time_t timestamp;
};

// Bank Class
class Bank {
public:
    ~Bank() {
        for (auto account : accounts) {
            delete account;
        }
    }

    void addAccount(Account* account) {
        accounts.push_back(account);
    }

    Account* findAccount(int accountNumber) const {
        for (auto account : accounts) {
            if (account->getAccountNumber() == accountNumber) {
                return account;
            }
        }
        return nullptr;
    }

    void deposit(int accountNumber, double amount) {
        Account* account = findAccount(accountNumber);
        if (!account) {
            throw runtime_error("Account not found.");
        }
        account->deposit(amount);
        transactions.push_back(Transaction(accountNumber, -1, amount, "Deposit"));
    }

    void withdraw(int accountNumber, double amount) {
        Account* account = findAccount(accountNumber);
        if (!account) {
            throw runtime_error("Account not found.");
        }
        account->withdraw(amount);
        transactions.push_back(Transaction(accountNumber, -1, amount, "Withdrawal"));
    }

    void transfer(int fromAccountNumber, int toAccountNumber, double amount) {
        Account* fromAccount = findAccount(fromAccountNumber);
        Account* toAccount = findAccount(toAccountNumber);
        if (!fromAccount || !toAccount) {
            throw runtime_error("One or both accounts not found.");
        }
        fromAccount->withdraw(amount);
        toAccount->deposit(amount);
        transactions.push_back(Transaction(fromAccountNumber, toAccountNumber, amount, "Transfer"));
    }

    void displayAccounts() const {
        for (const auto& account : accounts) {
            account->display();
        }
    }

    void displayTransactions() const {
        for (const auto& transaction : transactions) {
            cout << transaction.toString() << endl;
        }
    }

private:
    vector<Account*> accounts;
    vector<Transaction> transactions;
};

// Main Function
int main() {
    Bank bank;

    while (true) {
        cout << "\nBanking System\n";
        cout << "1. Create Account\n";
        cout << "2. View Accounts\n";
        cout << "3. Deposit\n";
        cout << "4. Withdraw\n";
        cout << "5. Transfer\n";
        cout << "6. View Transactions\n";
        cout << "7. Exit\n";
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            int accountNumber;
            double initialBalance;
            char accountType;
            cout << "Enter account number: ";
            cin >> accountNumber;
            cout << "Enter initial balance: ";
            cin >> initialBalance;
            cout << "Enter account type (S for Savings, C for Current): ";
            cin >> accountType;

            if (accountType == 'S') {
                bank.addAccount(new SavingsAccount(accountNumber, initialBalance));
            } else if (accountType == 'C') {
                bank.addAccount(new CurrentAccount(accountNumber, initialBalance));
            } else {
                cout << "Invalid account type." << endl;
            }
        } else if (choice == 2) {
            bank.displayAccounts();
        } else if (choice == 3) {
            int accountNumber;
            double amount;
            cout << "Enter account number: ";
            cin >> accountNumber;
            cout << "Enter amount: ";
            cin >> amount;
            try {
                bank.deposit(accountNumber, amount);
                cout << "Deposited $" << amount << " to account " << accountNumber << endl;
            } catch (const exception& e) {
                cout << "Error: " << e.what() << endl;
            }
        } else if (choice == 4) {
            int accountNumber;
            double amount;
            cout << "Enter account number: ";
            cin >> accountNumber;
            cout << "Enter amount: ";
            cin >> amount;
            try {
                bank.withdraw(accountNumber, amount);
                cout << "Withdrew $" << amount << " from account " << accountNumber << endl;
            } catch (const exception& e) {
                cout << "Error: " << e.what() << endl;
            }
        } else if (choice == 5) {
            int fromAccount, toAccount;
            double amount;
            cout << "Enter from account number: ";
            cin >> fromAccount;
            cout << "Enter to account number: ";
            cin >> toAccount;
            cout << "Enter amount: ";
            cin >> amount;
            try {
                bank.transfer(fromAccount, toAccount, amount);
                cout << "Transferred $" << amount << " from account " << fromAccount << " to account " << toAccount << endl;
            } catch (const exception& e) {
                cout << "Error: " << e.what() << endl;
            }
        } else if (choice == 6) {
            bank.displayTransactions();
        } else if (choice == 7) {
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
