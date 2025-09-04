#include <iostream>
#include <SQLiteCpp/SQLiteCpp.h>

int main() {
    // A try-catch block is essential for database operations
    try {
        // Open a database file named "UserConnect.db" in the same folder as the executable.
        // It will be created automatically if it doesn't exist.
        SQLite::Database db("UserConnect.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

        std::cout << "Database file created/opened successfully." << std::endl;

        // Create the Users table
        std::cout << "Creating Users table..." << std::endl;
        db.exec("CREATE TABLE IF NOT EXISTS Users (id INTEGER PRIMARY KEY, name TEXT)");
        std::cout << "Table 'Users' created successfully." << std::endl;

    } catch (const std::exception& e) {
        // If anything goes wrong, this will print the error
        std::cerr << "SQLite exception: " << e.what() << std::endl;
        return 1; // Return a non-zero value to indicate failure
    }

    return 0;
}