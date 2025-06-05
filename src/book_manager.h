#ifndef BOOK_MANAGER_H
#define BOOK_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_BOOK_NAME_LENGTH 64
#define MAX_BOOKS 20

typedef struct {
    char name[MAX_BOOK_NAME_LENGTH];
    uint16_t totalPages;
    uint16_t pagesRead;
} Book;

// Initialize book manager and load books from JSON
bool book_manager_init(void);

// Create default JSON file if it doesn't exist
bool book_manager_create_default_json(void);

// Build roller options string from loaded books
void book_manager_build_roller_options(void);

// Get number of books loaded
uint8_t book_manager_get_count(void);

// Get book by index
const Book* book_manager_get_book(uint8_t index);

// Get book by name
const Book* book_manager_get_book_by_name(const char* name);

// Update pages read for a book
bool book_manager_update_pages_read(const char* bookName, uint16_t pagesRead);

// Save books back to JSON file
bool book_manager_save_books(void);

// Get all book names as a string for roller (format: "Book1\nBook2\nBook3")
const char* book_manager_get_roller_options(void);

#endif // BOOK_MANAGER_H