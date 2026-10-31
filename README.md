# Library Management System Using C

**Student:** Deepanshu Kumar  
**UID:** 26BCA10246  
**Course:** Bachelor of Computer Applications (BCA), First Year  
**University:** Chandigarh University  
**Submitted to:** Khushi Agarwal Ma'am  
**Academic Session:** 2026–2027

## Project description
A menu-driven, beginner-friendly C program for maintaining a small library's book records. It supports adding, displaying, searching, issuing, returning, and deleting books. Records are stored in a binary data file so they remain available after the program closes.

## Features
- Add a book with a unique positive ID, title, author, and copy count
- Display all stored books
- Search by book ID
- Issue and return copies while checking availability
- Delete a record only when all copies have been returned
- Save changes to `books.dat`
- Basic validation for menu choices and numeric input

## Technologies used
- C programming language
- Standard C library: `stdio.h`, `stdlib.h`, `string.h`, `ctype.h`
- Structures, arrays, pointers, functions, loops, `switch`, and file handling
- GCC compiler; Visual Studio Code can be used as the editor

## Compile and run

### GCC (Linux / macOS / MinGW on Windows)
```bash
gcc library.c -o library
./library
```
On Windows Command Prompt:
```bat
gcc library.c -o library.exe
library.exe
```

### Visual Studio Code
1. Install a C/C++ extension and a GCC compiler (for example, MinGW-w64 on Windows).
2. Open this folder in VS Code.
3. Open Terminal → New Terminal.
4. Run `gcc library.c -o library.exe` (Windows) or `gcc library.c -o library` (Linux/macOS).
5. Run the generated executable from the terminal.

## Data file
The program creates `books.dat` in its current working directory. Keep that file if you want to keep the records. It is a binary file and is not intended to be edited manually. Back it up before moving the project to another computer.

## Sample output
```text
Library Management System - BCA First Year
Student: Deepanshu Kumar | UID: 26BCA10246

========== LIBRARY MANAGEMENT SYSTEM ==========
1. Add New Book
2. Display All Books
3. Search Book by ID
4. Issue Book
5. Return Book
6. Delete Book Record
7. Exit
===============================================
Enter your choice (1-7): 1
Enter book ID: 101
Enter book title: The C Programming Language
Enter author name: Kernighan and Ritchie
Enter total copies: 3
Book added and saved successfully.
```

## Limitations
This is a learning project. It does not track individual borrowers, issue dates, due dates, fines, login accounts, or multiple simultaneous users. The binary file format is intended for the same compiled program and structure definition.

## Repository contents
- `library.c` — source code
- `README.md` — project overview and instructions
- `Library_Management_System_Report.docx` — editable report
