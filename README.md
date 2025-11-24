# UserConnectPro

**A C++ Academic Social Platform**

UserConnectPro is a console-based application designed to bridge the gap between Students and Teachers. It allows users to share share their views with tweets, and have functionalities like enrolling in projects, courses, and interact via a global feed. Built completely from scratch using **Modern C++** and **SQLite**.

## 🚀 Key Features

* **Role-Based System:** Distinct dashboards and permissions for **Students** and **Teachers**.
* **Secure Authentication:** Custom password hashing and salting (no plain text passwords stored).
* **Content Management:** Teachers can **Tweet**, create and manage **Courses** and **Projects**.
* **Application Workflow:**
    * Students and Teachers can view/create **Tweets**
    * Students can apply and enroll to **Courses** and **Projects**.
    * Teachers can view a live list of applicants and **Approve** or **Reject** them via an interactive menu.
* **Global Tweet Feed:** A real-time community feed where all users can post their thoughts through Tweet system.

## 🛠️ Tech Stack

* **Language:** C++ (C++17 Standard)
* **Database:** SQLite (integrated via SQLiteCpp)
* **Build System:** CMake
* **Version Control:** Git

## 🏗️ Architecture

I designed this project using the **Model-View-Controller (MVC)** pattern to ensure clean code and separation of work:

* **Model (`DatabaseManager`):** Handles all SQL queries, data logic, and security (hashing).
* **View/Controller (`ViewController`):** Manages the UI, user input, and application flow.
* **Data Objects:** Clean Classes (`User`, `Post`, `Tweets`, `Enrollments`) to pass data between layers.

![MVC Diagram](https://upload.wikimedia.org/wikipedia/commons/thumb/a/a0/MVC-Process.svg/1200px-MVC-Process.svg.png)
*(Standard MVC flow implemented in this project)*

## 💻 How to Build and Run

You need **CMake** and a C++ compiler (GCC or Clang) installed.

1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/kaushikdevani/UserConnectPro.git](https://github.com/kaushikdevani/UserConnectPro.git)
    cd UserConnectPro
    ```

2.  **Build the project:**
    ```bash
    mkdir build
    cd build
    cmake ..
    cmake --build .
    ```

3.  **Run the application:**
    ```bash
    ./UserConnectPro
    ```

## 🗄️ Database Schema

The application automatically generates `UserConnect.db` with the following relationships:
* **Users:** Stores login info and roles.
* **Posts:** Linked to Users (Owners).
* **Tweets:** Linked to Users with timestamps.
* **Enrollments:** A join-table linking Users to Posts with status tracking (`Applied`, `Enrolled`).

---
*Created by Kaushik Devani*