#include <Arduino.h>
#include <string>
#include "PinDef.h"
#include "LEDButton.h"
#include "TwoPosSwitch.h"

LEDButton dispatchButton1(DISPATCH_BTN_1, DISPATCH_LED, ACTIVE_HIGH);
LEDButton dispatchButton2(DISPATCH_BTN_2, DISPATCH_LED, ACTIVE_HIGH);
LEDButton eStopButton(ESTOP_BTN, ESTOP_LED, ACTIVE_HIGH);
TwoPosSwitch modeSwitch(SWITCH_POS_1, SWITCH_POS_2);

const uint32_t LOOP_PERIOD_US = 10000;
const uint32_t BLINK_PERIOD_US = 500000;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Syncs the loop timing to maintain a consistent loop period
/// \param[inout] previousTimeUs The previous loop time in microseconds, updated to the current loop time after syncing
/// \return void
void syncTiming(uint32_t &previousTimeUs)
{
    uint32_t currentTimeUs = micros();

    if ((currentTimeUs - previousTimeUs) < LOOP_PERIOD_US)
    {
        uint32_t remaining = LOOP_PERIOD_US - (currentTimeUs - previousTimeUs);
        delayMicroseconds(remaining);
    }

    previousTimeUs += LOOP_PERIOD_US;
}

void updateBlinkCounter(uint32_t &blinkCounterUs, PinStatus &globalBlinkState)
{
    blinkCounterUs += LOOP_PERIOD_US;

    if (blinkCounterUs >= BLINK_PERIOD_US)
    {
        // Toggle global blink state and reset the counter
        toggleStatus(globalBlinkState);
        blinkCounterUs = blinkCounterUs - BLINK_PERIOD_US;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Updates the LCD display with current parameters
/// \return void
void updateLCD()
{
    // Placeholder for LCD update logic
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Runs at the start of the program to initialize components
/// \return void
void setup()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Main loop function that runs repeatedly
/// \return void
void loop()
{
    static uint32_t lastLoopTimeUs = micros();
    static uint32_t blinkCounterUs = 0;
    static PinStatus globalBlinkState = LOW;
    bool lcdParamChanged = false;



    // If an LCD parameter has changed, update the display
    if (lcdParamChanged)
    {
      updateLCD();
    }

    // Sync loop timing and update blink counter
    syncTiming(lastLoopTimeUs);
    updateBlinkCounter(blinkCounterUs, globalBlinkState);
}
