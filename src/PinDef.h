#ifndef PINDEF_H
#define PINDEF_H

#include <Arduino.h>

/// \brief Arduino pin definition for the E-Stop LED
const pin_size_t ESTOP_LED = 2;
/// \brief Arduino pin definition for the E-Stop button
const pin_size_t ESTOP_BTN = 3;
/// \brief Arduino pin definition for Dispatch LED 1
const pin_size_t DISPATCH_LED1 = 4;
/// \brief Arduino pin definition for Dispatch button 1
const pin_size_t DISPATCH_BTN1 = 5;
/// \brief Arduino pin definition for Dispatch LED 2
const pin_size_t DISPATCH_LED2 = 6;
/// \brief Arduino pin definition for Dispatch button 2
const pin_size_t DISPATCH_BTN2 = 8;
/// \brief Arduino pin definition for the LCD backlight
const pin_size_t LCD_BACKLIGHT = 9;
/// \brief Arduino pin definition for the key switch
const pin_size_t KEY_SWITCH = 10;
/// \brief Arduino pin definition for the power LED
const pin_size_t POWER_LED = 11;
/// \brief Arduino pin definition for the connected LED
const pin_size_t CONNECTED_LED = 12;
/// \brief Arduino pin definition for the mode switch (Auto)
const pin_size_t MODE_SW_AUTO = 13;
/// \brief Arduino pin definition for the mode switch (Manual)
const pin_size_t MODE_SW_MANUAL = 22;
/// \brief Arduino pin definition for the engaged LED
const pin_size_t ENGAGED_LED = 23;
/// \brief Arduino pin definition for the reset button
const pin_size_t RESET_BTN = 24;
/// \brief Arduino pin definition for the reset LED
const pin_size_t RESET_LED = 25;
/// \brief Arduino pin definition for the ready LED
const pin_size_t READY_LED = 26;
/// \brief Arduino pin definition for the train station LED
const pin_size_t TRAIN_STATION_LED = 27;
/// \brief Arduino pin definition for the floor switch (Open)
const pin_size_t FLOOR_SW_OPEN = 28;
/// \brief Arduino pin definition for the floor switch (Close)
const pin_size_t FLOOR_SW_CLOSE = 29;
/// \brief Arduino pin definition for the floor LED
const pin_size_t FLOOR_LED = 30;
/// \brief Arduino pin definition for the seats switch (Open)
const pin_size_t SEATS_SW_OPEN = 31;
/// \brief Arduino pin definition for the seats switch (Close)
const pin_size_t SEATS_SW_CLOSE = 32;
/// \brief Arduino pin definition for the seats LED
const pin_size_t SEATS_LED = 33;
/// \brief Arduino pin definition for the restraints switch (Open)
const pin_size_t RESTRAINTS_SW_OPEN = 34;
/// \brief Arduino pin definition for the restraints switch (Close)
const pin_size_t RESTRAINTS_SW_CLOSE = 35;
/// \brief Arduino pin definition for the restraints LED
const pin_size_t RESTRAINTS_LED = 36;
/// \brief Arduino pin definition for the gates switch (Open)
const pin_size_t GATES_SW_OPEN = 37;
/// \brief Arduino pin definition for the gates switch (Close)
const pin_size_t GATES_SW_CLOSE = 38;
/// \brief Arduino pin definition for the gates LED
const pin_size_t GATES_LED = 40;
/// \brief Arduino pin definition for the fault LED
const pin_size_t FAULT_LED = 41;
/// \brief Arduino pin definition for the test button
const pin_size_t TEST_BTN = 42;
/// \brief Arduino pin definition for the test LED
const pin_size_t TEST_LED = 43;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Helper function to toggle a PinStatus value between HIGH and LOW
/// \param[inout] status Reference to the PinStatus variable to toggle
void toggleStatus(PinStatus &status)
{
    status = (status == HIGH) ? LOW : HIGH;
}

#endif // PINDEF_H
