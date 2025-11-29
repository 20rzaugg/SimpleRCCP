#ifndef PINDEF_H
#define PINDEF_H

#include <Arduino.h>

const pin_size_t DISPATCH_BTN_1 = 3;
const pin_size_t DISPATCH_BTN_2 = 4;
const pin_size_t DISPATCH_LED = 5;
const pin_size_t SWITCH_POS_1 = 6;
const pin_size_t SWITCH_POS_2 = 7;
const pin_size_t LCD_SDA = 8;
const pin_size_t LCD_SCL = 9;
const pin_size_t ESTOP_LED = 10;
const pin_size_t ESTOP_BTN = 11;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Helper function to toggle a PinStatus value between HIGH and LOW
/// \param[inout] status Reference to the PinStatus variable to toggle
void toggleStatus(PinStatus &status)
{
    status = (status == HIGH) ? LOW : HIGH;
}

#endif // PINDEF_H
