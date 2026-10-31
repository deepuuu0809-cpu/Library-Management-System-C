/*
 * Library Management System
 * BCA First Year - C Programming
 * Student: Deepanshu Kumar | UID: 26BCA10246
 *
 * Records are stored in books.dat in the same folder as the executable.
 * This is a small educational project using structures and binary file handling.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DATA_FILE "books.dat"
#define TEMP_FILE "books.tmp"
#define MAX_TITLE 100
#define MAX_AUTHOR 80
#define MAX_BOOKS 1000

typedef struct {
    int id;
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
    int totalCopies;
    int availableCopies;
} Book;

static void clearInput(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) { }
}

static int readInt(const char *prompt, int *value) {
    char line[100], extra;
    printf("%s", prompt);
    if (!fgets(line, sizeof(line), stdin)) return 0;
    if (sscanf(line, " %d %c", value, &extra) != 1) {
        printf("Invalid input. Please enter a whole number.\n");
        return 0;
    }
    return 1;
}

static void readText(const char *prompt, char *out, size_t size) {
    printf("%s", prompt);
    if (fgets(out, (int)size, stdin)) {
        size_t n = strlen(out);
        if (n > 0 && out[n - 1] == '\n') out[n - 1] = '\0';
        else clearInput();
    } else {
        out[0] = '\0';
    }
    /* Avoid blank titles/authors. */
    while (out[0] == '\0') {
        printf("This field cannot be empty. Try again: ");
        if (!fgets(out, (int)size, stdin)) { out[0] = '\0'; return; }
        size_t n = strlen(out);
        if (n > 0 && out[n - 1] == '\n') out[n - 1] = '\0';
        else clearInput();
    }
}

static int loadBooks(Book books[], int capacity) {
    FILE *fp = fopen(DATA_FILE, "rb");
    int count = 0;
    if (!fp) return 0; /* First run: file does not exist yet. */
    while (count < capacity && fread(&books[count], sizeof(Book), 1, fp) == 1)
        count++;
    fclose(fp);
    return count;
}

static int saveBooks(const Book books[], int count) {
    FILE *fp = fopen(TEMP_FILE, "wb");
    if (!fp) {
        printf("Error: Could not open temporary data file for saving.\n");
        return 0;
    }
    if (count > 0 && fwrite(books, sizeof(Book), (size_t)count, fp) != (size_t)count) {
        printf("Error: Could not write all records.\n");
        fclose(fp);
        remove(TEMP_FILE);
        return 0;
    }
    if (fclose(fp) != 0) {
        printf("Error: Could not close data file properly.\n");
        remove(TEMP_FILE);
        return 0;
    }
    if (remove(DATA_FILE) != 0) {
        /* It is normal for DATA_FILE not to exist on the first save. */
    }
    if (rename(TEMP_FILE, DATA_FILE) != 0) {
        printf("Error: Could not replace the data file.\n");
        return 0;
    }
    return 1;
}

static int findBookIndex(const Book books[], int count, int id) {
    int i;
    for (i = 0; i < count; i++)
        if (books[i].id == id) return i;
    return -1;
}

static void addBook(Book books[], int *count) {
    Book b;
    if (*count >= MAX_BOOKS) {
        printf("Storage limit reached.\n");
        return;
    }
    if (!readInt("Enter book ID: ", &b.id)) return;
    if (b.id <= 0) { printf("Book ID must be positive.\n"); return; }
    if (findBookIndex(books, *count, b.id) != -1) {
        printf("A book with this ID already exists.\n");
        return;
    }
    readText("Enter book title: ", b.title, sizeof(b.title));
    readText("Enter author name: ", b.author, sizeof(b.author));
    if (!readInt("Enter total copies: ", &b.totalCopies)) return;
    if (b.totalCopies <= 0) { printf("Copies must be at least 1.\n"); return; }
    b.availableCopies = b.totalCopies;
    books[*count] = b; /* Array stores each structure record. */
    (*count)++;
    if (saveBooks(books, *count))
        printf("Book added and saved successfully.\n");
}

static void displayBooks(const Book books[], int count) {
    int i;
    if (count == 0) { printf("No book records found.\n"); return; }
    printf("\n%-8s %-30s %-24s %-10s %-10s\n",
           "ID", "TITLE", "AUTHOR", "TOTAL", "AVAILABLE");
    printf("--------------------------------------------------------------------------------------\n");
    for (i = 0; i < count; i++)
        printf("%-8d %-30.30s %-24.24s %-10d %-10d\n",
               books[i].id, books[i].title, books[i].author,
               books[i].totalCopies, books[i].availableCopies);
}

static void searchBook(const Book books[], int count) {
    int id, index;
    if (!readInt("Enter book ID to search: ", &id)) return;
    index = findBookIndex(books, count, id);
    if (index == -1) { printf("Book not found.\n"); return; }
    printf("\nBook found:\nID: %d\nTitle: %s\nAuthor: %s\nTotal copies: %d\nAvailable copies: %d\n",
           books[index].id, books[index].title, books[index].author,
           books[index].totalCopies, books[index].availableCopies);
}

static void issueBook(Book books[], int count) {
    int id, index;
    if (!readInt("Enter book ID to issue: ", &id)) return;
    index = findBookIndex(books, count, id);
    if (index == -1) { printf("Book not found.\n"); return; }
    if (books[index].availableCopies <= 0) {
        printf("No copies are currently available.\n");
        return;
    }
    books[index].availableCopies--;
    if (saveBooks(books, count))
        printf("Book issued successfully. Available copies: %d\n",
               books[index].availableCopies);
}

static void returnBook(Book books[], int count) {
    int id, index;
    if (!readInt("Enter book ID to return: ", &id)) return;
    index = findBookIndex(books, count, id);
    if (index == -1) { printf("Book not found.\n"); return; }
    if (books[index].availableCopies >= books[index].totalCopies) {
        printf("All copies are already in the library.\n");
        return;
    }
    books[index].availableCopies++;
    if (saveBooks(books, count))
        printf("Book returned successfully. Available copies: %d\n",
               books[index].availableCopies);
}

static void deleteBook(Book books[], int *count) {
    int id, index, i;
    if (!readInt("Enter book ID to delete: ", &id)) return;
    index = findBookIndex(books, *count, id);
    if (index == -1) { printf("Book not found.\n"); return; }
    if (books[index].availableCopies != books[index].totalCopies) {
        printf("Cannot delete: one or more copies are issued. Return them first.\n");
        return;
    }
    for (i = index; i < *count - 1; i++)
        books[i] = books[i + 1]; /* Shift later records one position left. */
    (*count)--;
    if (saveBooks(books, *count))
        printf("Book record deleted successfully.\n");
}

static void showMenu(void) {
    printf("\n========== LIBRARY MANAGEMENT SYSTEM ==========\n");
    printf("1. Add New Book\n");
    printf("2. Display All Books\n");
    printf("3. Search Book by ID\n");
    printf("4. Issue Book\n");
    printf("5. Return Book\n");
    printf("6. Delete Book Record\n");
    printf("7. Exit\n");
    printf("===============================================\n");
}

int main(void) {
    Book books[MAX_BOOKS];
    int count = loadBooks(books, MAX_BOOKS);
    int choice;
    printf("Library Management System - BCA First Year\n");
    printf("Student: Deepanshu Kumar | UID: 26BCA10246\n");
    for (;;) {
        showMenu();
        if (!readInt("Enter your choice (1-7): ", &choice)) continue;
        switch (choice) {
            case 1: addBook(books, &count); break;
            case 2: displayBooks(books, count); break;
            case 3: searchBook(books, count); break;
            case 4: issueBook(books, count); break;
            case 5: returnBook(books, count); break;
            case 6: deleteBook(books, &count); break;
            case 7: printf("Thank you for using the Library Management System.\n"); return 0;
            default: printf("Invalid choice. Please select a number from 1 to 7.\n");
        }
    }
}
