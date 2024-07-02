#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int id;
    char title[100];
    char author[100];
    int year;
} Book;

// Function Prototype
void addBook();
void viewBooks();
void searchBook();
void updateBook();
void updateBook();
void deleteBook();
int getNextId();

int main()
{
    int choice;

    while (1)
    {
        printf("\nLibrary Management System\n");
        printf("1. Add Book\n");
        printf("2. View Books\n");
        printf("3. Search Book\n");
        printf("4. Delete Book\n");
        printf("5. Update Book\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addBook();
            break;
        case 2:
            viewBooks();
            break;
        case 3:
            searchBook();
            break;
        case 4:
            deleteBook();
            break;
        case 5:
            updateBook();
            break;
        case 6:
            exit(0);
        default:
            printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}

void addBook()
{
    FILE *file = fopen("text.txt", "a");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    Book book;
    book.id = getNextId();

    printf("Enter book title: ");
    getchar(); // Clear newline character from buffer
    fgets(book.title, 100, stdin);
    // Remove newline character from book title
    book.title[strcspn(book.title, "\n")] = '\0';

    printf("Enter author name: ");
    fgets(book.author, 100, stdin);
    book.author[strcspn(book.author, "\n")] = '\0';

    printf("Enter publication year: ");
    scanf("%d", &book.year);

    fprintf(file, "%d,%s,%s,%d\n", book.id, book.title, book.author, book.year);
    fclose(file);

    printf("Book added successfully!\n");
}

int getNextId()
{
    FILE *file = fopen("text.txt", "r");
    if (file == NULL)
    {
        return 1; // Starting ID
    }

    int id = 0;
    Book book;
    while (fscanf(file, "%d,%[^,],%[^,],%d\n", &book.id, book.title, book.author, &book.year) != EOF)
    {
        if (book.id > id)
        {
            id = book.id;
        }
    }
    fclose(file);
    return id + 1;
}

void viewBooks()
{
    FILE *file = fopen("text.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    printf("\nBook List:\n");
    printf("ID\tTitle\t\tAuthor\t\tYear\n");
    printf("-----------------------------------------------------------------\n");

    Book book;
    while (fscanf(file, "%d,%[^,],%[^,],%d\n", &book.id, book.title, book.author, &book.year) != EOF)
    {
        printf("%d\t%s\t\t%s\t%d\n", book.id, book.title, book.author, book.year);
    }
    fclose(file);
}

void searchBook()
{
    char title[100];
    printf("Enter the book title to search: ");
    getchar(); // Clear newline character from buffer
    fgets(title, 100, stdin);
    title[strcspn(title, "\n")] = '\0';

    FILE *file = fopen("text.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    Book book;
    int found = 0;
    while (fscanf(file, "%d,%[^,],%[^,],%d\n", &book.id, book.title, book.author, &book.year) != EOF)
    {
        if (strcasecmp(book.title, title) == 0)
        {
            printf("Book found:\n");
            printf("ID: %d\nTitle: %s\nAuthor: %s\nYear: %d\n", book.id, book.title, book.author, book.year);
            found = 1;
            break;
        }
    }
    if (!found)
    {
        printf("Book not found!\n");
    }
    fclose(file);
}

void updateBook()
{
    int id;
    printf("Enter the book ID to update: ");
    scanf("%d", &id);

    FILE *file = fopen("text.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL)
    {
        printf("Error opening temporary file!\n");
        fclose(file);
        return;
    }

    Book book;
    int found = 0;
    while (fscanf(file, "%d,%[^,],%[^,],%d\n", &book.id, book.title, book.author, &book.year) != EOF)
    {
        if (book.id == id)
        {
            found = 1;
            printf("Enter new title: ");
            getchar(); // Clear newline character from buffer
            fgets(book.title, 100, stdin);
            book.title[strcspn(book.title, "\n")] = '\0';

            printf("Enter new author: ");
            fgets(book.author, 100, stdin);
            book.author[strcspn(book.author, "\n")] = '\0';

            printf("Enter new publication year: ");
            scanf("%d", &book.year);
        }
        fprintf(tempFile, "%d,%s,%s,%d\n", book.id, book.title, book.author, book.year);
    }
    fclose(file);
    fclose(tempFile);

    if (found)
    {
        remove("text.txt");
        rename("temp.txt", "text.txt");
        printf("Book updated successfully!\n");
    }
    else
    {
        remove("temp.txt");
        printf("Book not found!\n");
    }
}

void deleteBook()
{
    int id;
    printf("Enter the book ID to delete: ");
    scanf("%d", &id);

    FILE *file = fopen("text.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL)
    {
        printf("Error opening temporary file!\n");
        fclose(file);
        return;
    }

    Book book;
    int found = 0;
    while (fscanf(file, "%d,%[^,],%[^,],%d\n", &book.id, book.title, book.author, &book.year) != EOF)
    {
        if (book.id == id)
        {
            found = 1;
        }
        else
        {
            fprintf(tempFile, "%d,%s,%s,%d\n", book.id, book.title, book.author, book.year);
        }
    }
    fclose(file);
    fclose(tempFile);

    if (found)
    {
        remove("text.txt");
        rename("temp.txt", "text.txt");
        printf("Book deleted successfully!\n");
    }
    else
    {
        remove("temp.txt");
        printf("Book not found!\n");
    }
}
