#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <lvgl.h>
#include <LV_Helper.h>
#include <LilyGo_AMOLED.h>
#include <ui.h>

// Include our modular components
#include "ui_manager.h"
#include "rotary_encoder.h"
#include "book_manager.h"

// https://www.youtube.com/nishad2m8
// https://buymeacoffee.com/nishad2m8

// Define display hardware
LilyGo_Class amoled;

void setup(void)
{
    Serial.begin(115200);

    // Initialize AMOLED Display
    if (!amoled.begin())
    {
        Serial.println("AMOLED init failed");
        while (1)
            delay(1000);
    }

    // Set the orientation of the AMOLED display
    amoled.setRotation(0); // Portrait mode. USB port on the right .

    // Set brightness adjustable range 0 ~ 255
    amoled.setBrightness(153);

    // Initialize LVGL
    beginLvglHelper(amoled);

    // Initialize UI Manager
    ui_manager_init();
    
    // Initialize I2C for seesaw
    Wire.begin(3, 2);
    while (!Serial) delay(10);

    // Initialize rotary encoder
    if (!rotary_encoder_init()) {
        Serial.println("Failed to initialize rotary encoder");
        while(1) delay(10);
    }
}

void loop()
{
    // Handle LVGL tasks
    lv_task_handler();
    
    // Check if button is pressed
    if (rotary_encoder_button_pressed()) {
        Serial.println("Button pressed!");
        
        // Handle UI navigation
        ui_handle_button_press();
        
        // Add a delay to prevent multiple rapid button presses
        delay(300);
    }

    // Handle encoder rotation
    rotary_encoder_handle_rotation();
    
    delay(5);
}