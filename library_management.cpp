
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Book {
    std::string title;
    std::string author;
    int id;
    bool isBorrowed;
};

struct Borrower {
    std::string name;
    int bookId; // -1 if no book is borrowed
};

void addBook() {
    std::ofstream bookFile("books.txt", std::ios::app);
    Book book;
    std::cout << "Enter book title: ";
    std::cin.ignore();
    std::getline(std::cin, book.title);
    std::cout << "Enter book author: ";
    std::getline(std::cin, book.author);
    std::cout << "Enter book ID: ";
    std::cin >> book.id;
    book.isBorrowed = false;
    bookFile << book.id << "," << book.title << "," << book.author << "," << book.isBorrowed << "\n";
    bookFile.close();
    std::cout << "Book added successfully!\n";
}

void addBorrower() {
    std::ofstream borrowerFile("borrowers.txt", std::ios::app);
    Borrower borrower;
    std::cout << "Enter borrower name: ";
    std::cin.ignore();
    std::getline(std::cin, borrower.name);
    borrower.bookId = -1; // Initially, no book is borrowed
    borrowerFile << borrower.name << "," << borrower.bookId << "\n";
    borrowerFile.close();
    std::cout << "Borrower added successfully!\n";
}

void borrowBook() {
    int bookId;
    std::string borrowerName;
    std::cout << "Enter borrower name: ";
    std::cin.ignore();
    std::getline(std::cin, borrowerName);
    std::cout << "Enter book ID to borrow: ";
    std::cin >> bookId;

    std::ifstream bookFile("books.txt");
    std::ofstream tempFile("temp_books.txt");
    std::string line;
    bool bookFound = false, bookBorrowed = false;

    while (std::getline(bookFile, line)) {
        std::istringstream iss(line);
        int id;
        std::string title, author;
        bool isBorrowed;
        char comma;
        iss >> id >> comma;
        std::getline(iss, title, ',');
        std::getline(iss, author, ',');
        iss >> isBorrowed;
        
        if (id == bookId) {
            bookFound = true;
            if (isBorrowed) {
                bookBorrowed = true;
                tempFile << line << "\n";
            } else {
                isBorrowed = true;
                tempFile << id << "," << title << "," << author << "," << isBorrowed << "\n";
            }
        } else {
            tempFile << line << "\n";
        }
    }
    bookFile.close();
    tempFile.close();

    if (bookFound && !bookBorrowed) {
        std::remove("books.txt");
        std::rename("temp_books.txt", "books.txt");
        
        std::ifstream borrowerFile("borrowers.txt");
        std::ofstream tempBorrowerFile("temp_borrowers.txt");
        while (std::getline(borrowerFile, line)) {
            std::istringstream iss(line);
            std::string name;
            int borrowedBookId;
            std::getline(iss, name, ',');
            iss >> borrowedBookId;
            
            if (name == borrowerName) {
                tempBorrowerFile << name << "," << bookId << "\n";
            } else {
                tempBorrowerFile << line << "\n";
            }
        }
        borrowerFile.close();
        tempBorrowerFile.close();
        std::remove("borrowers.txt");
        std::rename("temp_borrowers.txt", "borrowers.txt");

        std::cout << "Book borrowed successfully!\n";
    } else if (bookFound && bookBorrowed) {
        std::cout << "Book is already borrowed!\n";
    } else {
        std::cout << "Book not found!\n";
    }
}

void displayBooks() {
    std::ifstream bookFile("books.txt");
    std::string line;
    std::cout << "Books List:\n";
    while (std::getline(bookFile, line)) {
        std::cout << line << "\n";
    }
    bookFile.close();
}

int main() {
    int choice;
    do {
        std::cout << "\n--- Library Management System ---\n";
        std::cout << "1. Add Book\n";
        std::cout << "2. Add Borrower\n";
        std::cout << "3. Borrow Book\n";
        std::cout << "4. Display Books\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                addBook();
                break;
            case 2:
                addBorrower();
                break;
            case 3:
                borrowBook();
                break;
            case 4:
                displayBooks();
                break;
            case 5:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice!\n";
        }
    } while (choice != 5);

    return 0;
}
