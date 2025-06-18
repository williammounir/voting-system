# 🗳️ Voting System in C++

## 📌 Project Overview
This is a console-based Voting System written in C++ that allows two main user roles: **Administrator** and **Elector (Voter)**. Each role has specific functionalities and access levels, making the system suitable for managing and conducting small to medium-scale digital elections.

## 👥 User Roles

### 👨‍💼 Administrator
Admins can:
- Log in or sign up.
- Edit their personal information.
- Create new elections.
- Edit election descriptions.
- Delete nominees from elections.
- End elections and finalize results.
- Switch to elector view.

### 🧑‍💻 Elector (Voter)
Electors can:
- Log in or sign up.
- Edit their personal information.
- View available elections and cast votes.
- View results of past elections.
- Switch to admin view (if they have credentials).

## 🧠 Key Features
- Persistent storage using file I/O for admins, elections, and electors.
- Separation of functionality using enums and clean switch-case structures.
- Ability to manage elections dynamically (add/edit/end).
- Vote tracking and result viewing for electors.
- Admin authorization per election (admins can only manage elections they created).
- Simple navigation menus and input validation.

## 🔧 Technologies Used
- **Language:** C++
- **Concepts:** File handling, structs, enums, control structures, functions, vector manipulation
- **IDE:** Code::Blocks, Visual Studio, or any C++ compiler

## 📂 Project Structure
```bash
VotingSystem/
├── main.cpp                # Main program with menus and core logic
├── elections.txt           # Election data (ID, name, nominees, votes, etc.)
├── admins.txt              # Admin accounts and credentials
├── electors.txt            # Elector accounts and credentials
└── README.md               # Project description and usage instructions
```

## 🧪 How to Run
1. Open the project in your favorite C++ IDE.
2. Compile the `main.cpp` file.
3. Run the executable from the console.
4. Choose your role (Admin or Elector) and follow the menu prompts.

## ⚠️ Notes
- Input handling uses both `cin` and `getline()` — be careful with input buffers.
- Elections are tied to the admins who created them — only they can edit or end them.
- Elections cannot be voted in once ended.

## 📈 Future Improvements
- Password encryption
- GUI (Graphical User Interface)
- Date/time support for elections
- Voting history logs
- Search and filter for elections and nominees

---

## 📬 Contact
Created by **William Mounir**  
For any questions or suggestions, feel free to reach out!
