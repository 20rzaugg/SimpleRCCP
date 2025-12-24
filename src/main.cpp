#include <Arduino.h>
#include <LCDi2c.h>
#include <string>
#include <Wire.h>

#include "PinDef.h"
#include "LEDButton.h"
#include "TwoPosSwitch.h"
#include "SimpleSwitch.h"
#include "USBHID.h"

SimpleSwitch keySwitch(KEY_SWITCH, SwitchConfigMode::ACTIVE_LOW);
LEDButton dispatchButton1(DISPATCH_BTN1, DISPATCH_LED1, ButtonConfigMode::ACTIVE_LOW);
LEDButton dispatchButton2(DISPATCH_BTN2, DISPATCH_LED2, ButtonConfigMode::ACTIVE_LOW);
LEDButton eStopButton(ESTOP_BTN, ESTOP_LED, ButtonConfigMode::ACTIVE_LOW); // This might actually be active high, no idea yet
LEDButton resetButton(RESET_BTN, RESET_LED, ButtonConfigMode::ACTIVE_LOW);
LEDButton testButton(TEST_BTN, TEST_LED, ButtonConfigMode::ACTIVE_LOW);
TwoPosSwitch modeSwitch(MODE_SW_AUTO, MODE_SW_MANUAL, SwitchConfigMode::ACTIVE_LOW);
TwoPosSwitch floorSwitch(FLOOR_SW_OPEN, FLOOR_SW_CLOSE, SwitchConfigMode::ACTIVE_LOW);
TwoPosSwitch seatsSwitch(SEATS_SW_OPEN, SEATS_SW_CLOSE, SwitchConfigMode::ACTIVE_LOW);
TwoPosSwitch restraintsSwitch(RESTRAINTS_SW_OPEN, RESTRAINTS_SW_CLOSE, SwitchConfigMode::ACTIVE_LOW);
TwoPosSwitch gatesSwitch(GATES_SW_OPEN, GATES_SW_CLOSE, SwitchConfigMode::ACTIVE_LOW);
LCDi2c lcd(0x27, Wire1);

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
    lcd.cls();
    lcd.locate(1, 1);
    lcd.print("Hello World!");
    // Placeholder for LCD update logic
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Runs at the start of the program to initialize components
/// \return void
void setup()
{
    lcd.begin(4, 20);
    pinMode(LCD_BACKLIGHT, OUTPUT);
    pinMode(POWER_LED, OUTPUT);
    pinMode(CONNECTED_LED, OUTPUT);
    pinMode(ENGAGED_LED, OUTPUT);
    pinMode(READY_LED, OUTPUT);
    pinMode(TRAIN_STATION_LED, OUTPUT);
    pinMode(FLOOR_LED, OUTPUT);
    pinMode(SEATS_LED, OUTPUT);
    pinMode(RESTRAINTS_LED, OUTPUT);
    pinMode(GATES_LED, OUTPUT);
    pinMode(FAULT_LED, OUTPUT);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Main loop function that runs repeatedly
/// \return void
void loop()
{
    static uint32_t lastLoopTimeUs = micros();
    static uint32_t blinkCounterUs = 0;
    static PinStatus globalBlinkState = LOW;
    static PinStatus estopLed = LOW;
    static PinStatus dispatchLed1 = LOW;
    static PinStatus dispatchLed2 = LOW;
    static PinStatus lcdBacklight = LOW;
    static PinStatus powerLed = LOW;
    static PinStatus connectedLed = LOW;
    static PinStatus engagedLed = LOW;
    static PinStatus resetLed = LOW;
    static PinStatus readyLed = LOW;
    static PinStatus trainStationLed = LOW;
    static PinStatus floorLed = LOW;
    static PinStatus seatsLed = LOW;
    static PinStatus restraintsLed = LOW;
    static PinStatus gatesLed = LOW;
    static PinStatus faultLed = LOW;
    static PinStatus testLed = LOW;

    bool lcdParamChanged = false;

    if (keySwitch.getState() == SwitchPosition::POSITION_2)
    {
        estopLed = LOW;
        dispatchLed1 = LOW;
        dispatchLed2 = LOW;
        lcdBacklight = LOW;
        powerLed = LOW;
        connectedLed = LOW;
        engagedLed = LOW;
        resetLed = LOW;
        readyLed = LOW;
        trainStationLed = LOW;
        floorLed = LOW;
        seatsLed = LOW;
        restraintsLed = LOW;
        gatesLed = LOW;
        faultLed = LOW;
        testLed = LOW;
    }
    else
    {
        estopLed = globalBlinkState;
        dispatchLed1 = globalBlinkState;
        dispatchLed2 = globalBlinkState;
        lcdBacklight = HIGH;
        powerLed = globalBlinkState;
        connectedLed = globalBlinkState;
        engagedLed = globalBlinkState;
        resetLed = globalBlinkState;
        readyLed = globalBlinkState;
        trainStationLed = globalBlinkState;
        floorLed = globalBlinkState;
        seatsLed = globalBlinkState;
        restraintsLed = globalBlinkState;
        gatesLed = globalBlinkState;
        faultLed = globalBlinkState;
        testLed = globalBlinkState;
    }

    eStopButton.setLED(estopLed);
    dispatchButton1.setLED(dispatchLed1);
    dispatchButton2.setLED(dispatchLed2);
    resetButton.setLED(resetLed);
    testButton.setLED(testLed);
    digitalWrite(LCD_BACKLIGHT, lcdBacklight);
    digitalWrite(POWER_LED, powerLed);
    digitalWrite(CONNECTED_LED, connectedLed);
    digitalWrite(ENGAGED_LED, engagedLed);
    digitalWrite(READY_LED, readyLed);
    digitalWrite(TRAIN_STATION_LED, trainStationLed);
    digitalWrite(FLOOR_LED, floorLed);
    digitalWrite(SEATS_LED, seatsLed);
    digitalWrite(RESTRAINTS_LED, restraintsLed);
    digitalWrite(GATES_LED, gatesLed);
    digitalWrite(FAULT_LED, faultLed);

    // If an LCD parameter has changed, update the display
    if (lcdParamChanged)
    {
      updateLCD();
    }

    // Sync loop timing and update blink counter
    syncTiming(lastLoopTimeUs);
    updateBlinkCounter(blinkCounterUs, globalBlinkState);
}
