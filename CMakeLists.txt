# Minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Name of My project
project(UserConnect)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tell CMake where to look for our header files (.h)
# This includes our own 'include' dir and the library dirs
include_directories(
    include
    lib/sqlite
    lib/sqlitecpp/include
)

# Create our executable named "UserConnect"
# NOTE: We now also give it the sqlite3.c file to compile!
add_executable(UserConnect
    src/main.cpp
    src/DatabaseManager.cpp
    lib/sqlite/sqlite3.c
    lib/sqlitecpp/src/Backup.cpp
    lib/sqlitecpp/src/Column.cpp
    lib/sqlitecpp/src/Database.cpp
    lib/sqlitecpp/src/Exception.cpp
    lib/sqlitecpp/src/Savepoint.cpp
    lib/sqlitecpp/src/Statement.cpp
    lib/sqlitecpp/src/Transaction.cpp
)

# Link our program with necessary system libraries that SQLite needs.
# On macOS and Linux, these are typically pthread and dl.
# But this linking is not needed for Win32 generally
if(APPLE OR UNIX)
    target_link_libraries(UserConnect PRIVATE pthread dl)
endif()
# Fun fact APPLE OR is redundant in the above code coz APPLE is certified UNIX it seems, intresting..