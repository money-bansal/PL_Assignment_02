#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <ctime>
#include <sstream>
#include <algorithm>

using namespace std;

// Helper function to get the current date as a string
string getCurrentDate() {
    time_t now = time(0);
    tm *ltm = localtime(&now);

    stringstream ss;
    ss << (1900 + ltm->tm_year) << '-'
       << (1 + ltm->tm_mon) << '-'
       << ltm->tm_mday;

    return ss.str();
}

// Convert a date string to a tm struct
tm stringToTm(const string& date) {
    tm t = {};
    sscanf(date.c_str(), "%d-%d-%d", &t.tm_year, &t.tm_mon, &t.tm_mday);
    t.tm_year -= 1900;
    t.tm_mon -= 1;
    return t;
}

// Calculate the number of days between two tm structs
int calculateDaysDifference(const tm& start, const tm& end) {
    time_t start_time = mktime(const_cast<tm*>(&start));
    time_t end_time = mktime(const_cast<tm*>(&end));
    double diff = difftime(end_time, start_time);
    return static_cast<int>(diff / (60 * 60 * 24));
}

// Exception classes
class BookNotFoundException : public runtime_error {
public:
    BookNotFoundException(const string& msg) : runtime_error(msg) {}
};

class MemberNotFoundException : public runtime_error {
public:
    MemberNotFoundException(const string& msg) : runtime_error(msg) {}
};

class BookAlreadyIssuedException : public runtime_error {
public:
    BookAlreadyIssuedException(const string& msg) : runtime_error(msg) {}
};

class BookNotIssuedException : public runtime_error {
public:
    BookNotIssuedException(const string& msg) : runtime_error(msg) {}
};

// Class for Book
class Book {
public:
    Book(int id, const string& title, const string& author) 
        : id(id), title(title), author(author), issued(false) {}

    int getId() const { return id; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    bool isIssued() const { return issued; }
    string getIssueDate() const { return issueDate; }

    void issue(const string& date) {
        issued = true;
        issueDate = date;
    }

    void returnBook() {
        issued = false;
        issueDate.clear();
    }

private:
    int id;
    string title;
    string author;
    bool issued;
    string issueDate; // Stores the date when the book was issued
};

// Class for Member
class Member {
public:
    Member(int id, const string& name) : id(id), name(name) {}

    int getId() const { return id; }
    string getName() const { return name; }

private:
    int id;
    string name;
};

// Class for Library
class Library {
public:
    void addBook(const Book& book) {
        books.push_back(book);
    }

    void addMember(const Member& member) {
        members.push_back(member);
    }

    Book& findBook(int id) {
        for (auto& book : books) {
            if (book.getId() == id) {
                return book;
            }
        }
        throw BookNotFoundException("Book not found");
    }

    const Book& findBook(int id) const {
        for (const auto& book : books) {
            if (book.getId() == id) {
                return book;
            }
        }
        throw BookNotFoundException("Book not found");
    }

    Member& findMember(int id) {
        for (auto& member : members) {
            if (member.getId() == id) {
                return member;
            }
        }
        throw MemberNotFoundException("Member not found");
    }

    const Member& findMember(int id) const {
        for (const auto& member : members) {
            if (member.getId() == id) {
                return member;
            }
        }
        throw MemberNotFoundException("Member not found");
    }

    void issueBook(int bookId, int memberId) {
        Book& book = findBook(bookId);
        Member& member = findMember(memberId);

        if (book.isIssued()) {
            throw BookAlreadyIssuedException("Book is already issued");
        }

        string today = getCurrentDate();
        book.issue(today);
        loans.push_back(make_pair(bookId, memberId));
        cout << "Book issued successfully.\n";
    }

    void returnBook(int bookId, int memberId) {
        Book& book = findBook(bookId);
        auto it = find_if(loans.begin(), loans.end(), [bookId, memberId](const pair<int, int>& loan) {
            return loan.first == bookId && loan.second == memberId;
        });

        if (it == loans.end()) {
            throw BookNotIssuedException("Book was not issued to this member");
        }

        book.returnBook();
        loans.erase(it);
        cout << "Book returned successfully.\n";
    }

    void calculateOverdueFees() const {
        string today = getCurrentDate();
        tm currentDate = stringToTm(today);
        bool flag=false;
        int days;
        cout<<"Enter the issued days time\n";
        cin>>days;
        for (const auto& loan : loans) {
            const Book& book = findBook(loan.first);
            if (book.isIssued()) {
                tm issueDate = stringToTm(book.getIssueDate());
                int overdueDays = calculateDaysDifference(issueDate, currentDate) - days; // Assuming a 2-week loan period
                if (overdueDays > 0) {
                    flag=true;
                    cout << "Book ID " << book.getId() << " is overdue by " << overdueDays << " days.\n";
                    cout<<"Charges applied for Rs.10/day for MemberId "<< loan.second<<" are: "<<overdueDays*10;
                }
            }
        }
        
        if(!flag)
         cout<<"No book is issued for more than "<<days<<" days, no charges applicable on any member\n";
    }

    void listBooks() const {
        cout << "Books in the library:\n";
        for (const auto& book : books) {
            cout << "ID: " << book.getId() << ", Title: " << book.getTitle() 
                 << ", Author: " << book.getAuthor() 
                 << ", Issued: " << (book.isIssued() ? "Yes" : "No") 
                 << (book.isIssued() ? ", Issue Date: " + book.getIssueDate() : "") 
                 << endl;
        }
    }

    void listMembers() const {
        cout << "Members in the library:\n";
        for (const auto& member : members) {
            cout << "ID: " << member.getId() << ", Name: " << member.getName() << endl;
        }
    }

private:
    vector<Book> books;
    vector<Member> members;
    vector<pair<int, int>> loans; // (bookId, memberId) pairs

    int calculateDaysDifference(const tm& start, const tm& end) const {
        time_t start_time = mktime(const_cast<tm*>(&start));
        time_t end_time = mktime(const_cast<tm*>(&end));
        double diff = difftime(end_time, start_time);
        return static_cast<int>(diff / (60 * 60 * 24));
    }
};

// Main function
int main() {
    Library library;
    int choice;

    do {
        cout << "\nLibrary Management System\n";
        cout << "1. Add Book\n";
        cout << "2. Add Member\n";
        cout << "3. Issue Book\n";
        cout << "4. Return Book\n";
        cout << "5. Calculate Overdue Fees\n";
        cout << "6. List Books\n";
        cout << "7. List Members\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        try {
            switch (choice) {
                case 1: {
                    int id;
                    string title, author;
                    cout << "Enter book ID: ";
                    cin >> id;
                    cin.ignore(); // ignore newline character
                    cout << "Enter book title: ";
                    getline(cin, title);
                    cout << "Enter book author: ";
                    getline(cin, author);
                    library.addBook(Book(id, title, author));
                    cout << "Book added successfully.\n";
                    break;
                }
                case 2: {
                    int id;
                    string name;
                    cout << "Enter member ID: ";
                    cin >> id;
                    cin.ignore(); // ignore newline character
                    cout << "Enter member name: ";
                    getline(cin, name);
                    library.addMember(Member(id, name));
                    cout << "Member added successfully.\n";
                    break;
                }
                case 3: {
                    int bookId, memberId;
                    cout << "Enter book ID to issue: ";
                    cin >> bookId;
                    cout << "Enter member ID to issue to: ";
                    cin >> memberId;
                    library.issueBook(bookId, memberId);
                    break;
                }
                case 4: {
                    int bookId, memberId;
                    cout << "Enter book ID to return: ";
                    cin >> bookId;
                    cout << "Enter member ID who is returning: ";
                    cin >> memberId;
                    library.returnBook(bookId, memberId);
                    break;
                }
                case 5: {
                    library.calculateOverdueFees();
                    break;
                }
                case 6: {
                    library.listBooks();
                    break;
                }
                case 7: {
                    library.listMembers();
                    break;
                }
                case 8:
                    cout << "Exiting...\n";
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    } while (choice != 8);

    return 0;
}
