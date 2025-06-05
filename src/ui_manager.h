#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <lvgl.h>

// UI screen navigation function
void ui_handle_button_press(void);

// UI initialization (loads books and initializes roller)
void ui_manager_init(void);

// Function to sync selected book name across all SB UI objects
void ui_sync_selected_book_name(void);

// Function to get current selected book name from roller
void ui_update_selected_book_name(void);

// Function to get the selected book name
const char* ui_get_selected_book_name(void);

// Function to initialize the book roller with data from JSON
bool ui_init_book_roller(void);

#endif // UI_MANAGER_H