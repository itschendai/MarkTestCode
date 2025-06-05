#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

#include <stdint.h>
#include <stdbool.h>

// Rotary encoder initialization
bool rotary_encoder_init(void);

// Check if button is pressed
bool rotary_encoder_button_pressed(void);

// Handle encoder rotation and update UI/LED accordingly
void rotary_encoder_handle_rotation(void);

// Get current encoder position
int32_t rotary_encoder_get_position(void);

#endif // ROTARY_ENCODER_H