#include "book_manager.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>

// Static variables to store books data
static Book books[MAX_BOOKS];
static uint8_t bookCount = 0;
static char rollerOptions[MAX_BOOKS * MAX_BOOK_NAME_LENGTH] = "";

bool book_manager_init(void)
{
    Serial.println("Initializing Book Manager...");
    
    // Initialize SPIFFS
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS initialization failed!");
        return false;
    }
    
    // Check if books.json exists
    if (!SPIFFS.exists("/books.json")) {
        Serial.println("books.json not found, creating default file...");
        // Create default books.json if it doesn't exist
        if (!book_manager_create_default_json()) {
            return false;
        }
    }
    
    // Read and parse JSON file
    File file = SPIFFS.open("/books.json", "r");
    if (!file) {
        Serial.println("Failed to open books.json for reading");
        return false;
    }
    
    // Parse JSON
    DynamicJsonDocument doc(2048);
    DeserializationError error = deserializeJson(doc, file);
    file.close();
    
    if (error) {
        Serial.print("JSON parsing failed: ");
        Serial.println(error.c_str());
        return false;
    }
    
    // Load books from JSON
    JsonArray booksArray = doc["books"];
    bookCount = 0;
    
    for (JsonObject book : booksArray) {
        if (bookCount >= MAX_BOOKS) {
            Serial.println("Maximum number of books reached");
            break;
        }
        
        const char* name = book["name"];
        uint16_t totalPages = book["totalPages"];
        uint16_t pagesRead = book["pagesRead"] | 0; // Default to 0 if not present
        
        // Copy book data
        strncpy(books[bookCount].name, name, MAX_BOOK_NAME_LENGTH - 1);
        books[bookCount].name[MAX_BOOK_NAME_LENGTH - 1] = '\0';
        books[bookCount].totalPages = totalPages;
        books[bookCount].pagesRead = pagesRead;
        
        Serial.print("Loaded book: ");
        Serial.print(books[bookCount].name);
        Serial.print(" (");
        Serial.print(books[bookCount].totalPages);
        Serial.println(" pages)");
        
        bookCount++;
    }
    
    // Build roller options string
    book_manager_build_roller_options();
    
    Serial.print("Successfully loaded ");
    Serial.print(bookCount);
    Serial.println(" books");
    
    return true;
}

bool book_manager_create_default_json(void)
{
    // Create a default JSON structure
    DynamicJsonDocument doc(2048);
    JsonArray booksArray = doc.createNestedArray("books");
    
    // Add some default books
    JsonObject book1 = booksArray.createNestedObject();
    book1["name"] = "The Great Gatsby";
    book1["totalPages"] = 180;
    book1["pagesRead"] = 0;
    
    JsonObject book2 = booksArray.createNestedObject();
    book2["name"] = "To Kill a Mockingbird";
    book2["totalPages"] = 376;
    book2["pagesRead"] = 0;
    
    JsonObject book3 = booksArray.createNestedObject();
    book3["name"] = "1984";
    book3["totalPages"] = 328;
    book3["pagesRead"] = 0;
    
    // Write to file
    File file = SPIFFS.open("/books.json", "w");
    if (!file) {
        Serial.println("Failed to create books.json");
        return false;
    }
    
    serializeJson(doc, file);
    file.close();
    
    Serial.println("Created default books.json file");
    return true;
}

void book_manager_build_roller_options(void)
{
    rollerOptions[0] = '\0'; // Clear the string
    
    for (uint8_t i = 0; i < bookCount; i++) {
        if (i > 0) {
            strcat(rollerOptions, "\n");
        }
        strcat(rollerOptions, books[i].name);
    }
}

uint8_t book_manager_get_count(void)
{
    return bookCount;
}

const Book* book_manager_get_book(uint8_t index)
{
    if (index >= bookCount) {
        return nullptr;
    }
    return &books[index];
}

const Book* book_manager_get_book_by_name(const char* name)
{
    for (uint8_t i = 0; i < bookCount; i++) {
        if (strcmp(books[i].name, name) == 0) {
            return &books[i];
        }
    }
    return nullptr;
}

bool book_manager_update_pages_read(const char* bookName, uint16_t pagesRead)
{
    for (uint8_t i = 0; i < bookCount; i++) {
        if (strcmp(books[i].name, bookName) == 0) {
            books[i].pagesRead = pagesRead;
            Serial.print("Updated ");
            Serial.print(bookName);
            Serial.print(" pages read to: ");
            Serial.println(pagesRead);
            return true;
        }
    }
    return false;
}

bool book_manager_save_books(void)
{
    DynamicJsonDocument doc(2048);
    JsonArray booksArray = doc.createNestedArray("books");
    
    // Add all books to JSON
    for (uint8_t i = 0; i < bookCount; i++) {
        JsonObject book = booksArray.createNestedObject();
        book["name"] = books[i].name;
        book["totalPages"] = books[i].totalPages;
        book["pagesRead"] = books[i].pagesRead;
    }
    
    // Write to file
    File file = SPIFFS.open("/books.json", "w");
    if (!file) {
        Serial.println("Failed to open books.json for writing");
        return false;
    }
    
    serializeJson(doc, file);
    file.close();
    
    Serial.println("Books saved to JSON file");
    return true;
}

const char* book_manager_get_roller_options(void)
{
    return rollerOptions;
}