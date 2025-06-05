#include "rotary_encoder.h"
#include <Arduino.h>
#include "Adafruit_seesaw.h"
#include <seesaw_neopixel.h>
#include <Wire.h>
#include <lvgl.h>
#include <ui.h>

#define SS_SWITCH        24
#define SS_NEOPIX        6
#define SEESAW_ADDR      0x36

// Static variables for internal state
static Adafruit_seesaw ss;
static seesaw_NeoPixel sspixel = seesaw_NeoPixel(1, SS_NEOPIX, NEO_GRB + NEO_KHZ800);
static int32_t encoder_position;

// External UI objects
extern lv_obj_t * ui_SelectBookScreen;
extern lv_obj_t * ui_BookNames;

// Color wheel function for NeoPixel
static uint32_t Wheel(uint8_t WheelPos) {
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85) {
        return sspixel.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if (WheelPos < 170) {
        WheelPos -= 85;
        return sspixel.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return sspixel.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

bool rotary_encoder_init(void)
{
    Serial.println("Looking for seesaw!");
    
    if (! ss.begin(SEESAW_ADDR) || ! sspixel.begin(SEESAW_ADDR)) {
        Serial.println("Couldn't find seesaw on default address");
        return false;
    }
    Serial.println("seesaw started");

    uint32_t version = ((ss.getVersion() >> 16) & 0xFFFF);
    if (version != 4991) {
        Serial.print("Wrong firmware loaded? ");
        Serial.println(version);
        return false;
    }
    Serial.println("Found Product 4991");

    // Set not so bright!
    sspixel.setBrightness(20);
    sspixel.show();
    
    // Use a pin for the built in encoder switch
    ss.pinMode(SS_SWITCH, INPUT_PULLUP);

    // Get starting position
    encoder_position = ss.getEncoderPosition();

    Serial.println("Turning on interrupts");
    delay(10);
    ss.setGPIOInterrupts((uint32_t)1 << SS_SWITCH, 1);
    ss.enableEncoderInterrupt();
    
    return true;
}

bool rotary_encoder_button_pressed(void)
{
    return !ss.digitalRead(SS_SWITCH);
}

void rotary_encoder_handle_rotation(void)
{
    int32_t new_position = ss.getEncoderPosition();
    
    // Check if encoder moved
    if (encoder_position != new_position) {
        lv_obj_t* current_screen = lv_scr_act();
        
        if (current_screen == ui_SelectBookScreen) {
            Serial.print("Encoder position: ");
            Serial.println(new_position);
            
            // Calculate which roller option to select based on encoder position
            // Use absolute value and modulo to map encoder position to 0, 1, or 2
            int rollerOption = abs(new_position) % 3;
            
            Serial.print("Setting roller to option: ");
            Serial.println(rollerOption);
            
            lv_roller_set_selected(ui_BookNames, rollerOption, LV_ANIM_ON);
            
            Serial.print("Selected option: ");
            Serial.println(lv_roller_get_selected(ui_BookNames));
        } else {
            // For other screens, just print position
            Serial.println(new_position);
        }
        
        // Change the neopixel color
        sspixel.setPixelColor(0, Wheel(new_position & 0xFF));
        sspixel.show();
        
        encoder_position = new_position;
    }
}

int32_t rotary_encoder_get_position(void)
{
    return encoder_position;
}