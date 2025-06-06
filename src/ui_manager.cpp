#include "ui_manager.h"
#include "book_manager.h"
#include <Arduino.h>
#include <ui.h>
#include <lvgl.h>

// Static variable to store selected book name
static char selectedbookname[64] = "No Book Selected";

// External UI objects from ui.h
extern lv_obj_t * ui_StartScreen;

extern lv_obj_t * ui_SelectBookScreen;
extern lv_obj_t * ui_BookNames;

extern lv_obj_t * ui_SelectStartScreen;
extern lv_obj_t * ui_StartingPageCount;
extern lv_obj_t * ui_SBSelectStart;

extern lv_obj_t * ui_TimerScreen;
extern lv_obj_t * ui_Time;
extern lv_obj_t * ui_SBTimer;

extern lv_obj_t * ui_RecordingScreen;
extern lv_obj_t * ui_TimeRecording;
extern lv_obj_t * ui_SBRecording;

extern lv_obj_t * ui_PausedScreen;
extern lv_obj_t * ui_Resume;
extern lv_obj_t * ui_End;
extern lv_obj_t * ui_TimePaused;
extern lv_obj_t * ui_SBPaused;

extern lv_obj_t * ui_SelectEndScreen;
extern lv_obj_t * ui_TotalPage;
extern lv_obj_t * ui_EndingPage;
extern lv_obj_t * ui_SBEnd;

extern lv_obj_t * ui_SuccessScreen;
extern lv_obj_t * ui_SBSuccess;

const char* ui_get_selected_book_name(void)
{
    return selectedbookname;
}

void ui_update_selected_book_name(void)
{
    if (ui_BookNames != NULL) {
        // Get the selected option index from the roller
        uint16_t selected_index = lv_roller_get_selected(ui_BookNames);
        
        // Get the text of the selected option
        char temp_buffer[64];
        lv_roller_get_selected_str(ui_BookNames, temp_buffer, sizeof(temp_buffer));
        
        // Update the selectedbookname variable
        strncpy(selectedbookname, temp_buffer, sizeof(selectedbookname) - 1);
        selectedbookname[sizeof(selectedbookname) - 1] = '\0'; // Ensure null termination
        
        Serial.print("Selected book updated to: ");
        Serial.println(selectedbookname);
    }
}

void ui_sync_selected_book_name(void)
{
    // Update all SB (Selected Book) UI objects with the current book name
    if (ui_SBSelectStart != NULL) {
        lv_label_set_text(ui_SBSelectStart, selectedbookname);
    }
    
    if (ui_SBTimer != NULL) {
        lv_label_set_text(ui_SBTimer, selectedbookname);
    }
    
    if (ui_SBRecording != NULL) {
        lv_label_set_text(ui_SBRecording, selectedbookname);
    }
    
    if (ui_SBPaused != NULL) {
        lv_label_set_text(ui_SBPaused, selectedbookname);
    }
    
    if (ui_SBEnd != NULL) {
        lv_label_set_text(ui_SBEnd, selectedbookname);
    }
    
    if (ui_SBSuccess != NULL) {
        lv_label_set_text(ui_SBSuccess, selectedbookname);
    }
    
    Serial.print("Synced book name across all SB objects: ");
    Serial.println(selectedbookname);
}

bool ui_init_book_roller(void)
{
    if (ui_BookNames == NULL) {
        Serial.println("ui_BookNames is NULL, cannot initialize roller");
        return false;
    }
    
    // Get roller options from book manager
    const char* options = book_manager_get_roller_options();
    if (options == NULL || strlen(options) == 0) {
        Serial.println("No book options available");
        return false;
    }
    
    // Set the roller options
    lv_roller_set_options(ui_BookNames, options, LV_ROLLER_MODE_NORMAL);
    
    // Set default selection to first book
    lv_roller_set_selected(ui_BookNames, 0, LV_ANIM_OFF);
    
    Serial.print("Initialized book roller with ");
    Serial.print(book_manager_get_count());
    Serial.println(" books");
    
    return true;
}

bool ui_init_starting_page_roller(void)
{
    if (ui_StartingPageCount == NULL) {
        Serial.println("ui_StartingPageCount is NULL, cannot initialize roller");
        return false;
    }

    const Book* book = book_manager_get_book_by_name(selectedbookname);
    if (book == nullptr) {
        Serial.println("Selected book not found for starting page roller");
        return false;
    }

    static char page_options[2048];
    page_options[0] = '\0';

    for (uint16_t i = 0; i <= book->totalPages; ++i) {
        char buf[8];
        sprintf(buf, "%u", i);
        strcat(page_options, buf);
        if (i < book->totalPages) {
            strcat(page_options, "\n");
        }
    }

    lv_roller_set_options(ui_StartingPageCount, page_options, LV_ROLLER_MODE_NORMAL);

    uint16_t default_page = book->pagesRead;
    if (default_page > book->totalPages) {
        default_page = 0;
    }
    lv_roller_set_selected(ui_StartingPageCount, default_page, LV_ANIM_OFF);

    Serial.print("Initialized start page roller for ");
    Serial.print(selectedbookname);
    Serial.print(" with total pages: ");
    Serial.println(book->totalPages);

    return true;
}

void ui_manager_init(void)
{
    // Initialize UI from LVGL helper
    ui_init();
    
    // Initialize book manager and load books from JSON
    if (!book_manager_init()) {
        Serial.println("Failed to initialize book manager");
        return;
    }
    
    // Initialize the book roller with loaded book data
    if (!ui_init_book_roller()) {
        Serial.println("Failed to initialize book roller");
        return;
    }
    
    Serial.println("UI Manager initialized successfully");
}

void ui_handle_button_press(void)
{
    lv_obj_t* current_screen = lv_scr_act();
    
    // Handle screen transitions based on current screen
    if (current_screen == ui_StartScreen) {
        lv_disp_load_scr(ui_SelectBookScreen);
    }
    else if (current_screen == ui_SelectBookScreen) {
        // Update selected book name before transitioning
        ui_update_selected_book_name();
        ui_sync_selected_book_name();
        ui_init_starting_page_roller();
        lv_disp_load_scr(ui_SelectStartScreen);
    }
    else if (current_screen == ui_SelectStartScreen) {
        lv_disp_load_scr(ui_TimerScreen);
    }
    else if (current_screen == ui_TimerScreen) {
        lv_disp_load_scr(ui_RecordingScreen);
    }
    else if (current_screen == ui_RecordingScreen) {
        lv_disp_load_scr(ui_PausedScreen);
    }
    else if (current_screen == ui_PausedScreen) {
        lv_disp_load_scr(ui_SelectEndScreen);
    }
    else if (current_screen == ui_SelectEndScreen) {
        lv_disp_load_scr(ui_SuccessScreen);
    }
    else if (current_screen == ui_SuccessScreen) {
        lv_disp_load_scr(ui_StartScreen);
    }
}