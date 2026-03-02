#include <Arduino.h>
#include <LCDi2c.h>
#include <string>
#include <cstdio>
#include <cstring>
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
const uint32_t TELEMETRY_TIMEOUT_MS = 1000;

struct TelemetryFrame
{
    int inPlayMode;
    int paused;
    int braking;
    int trainInStation;
    int eStop;
    int canDispatch;
    float speed;
    float gX;
    float gY;
    float gZ;
    uint32_t sequence;
};

struct SerialTelemetryState
{
    char lineBuffer[128];
    size_t lineLength;
    uint32_t lastRxMs;
    bool hasFrame;
    TelemetryFrame frame;
};

bool parseTelemetryFrame(const char *line, TelemetryFrame &outFrame)
{
    if (line == nullptr || line[0] != 'T' || line[1] != ',')
    {
        return false;
    }

    TelemetryFrame parsed = {};
    int parsedCount = sscanf(
        line,
        "T,%u,%d,%d,%d,%d,%d,%d,%f,%f,%f,%f",
        &parsed.sequence,
        &parsed.inPlayMode,
        &parsed.paused,
        &parsed.braking,
        &parsed.trainInStation,
        &parsed.eStop,
        &parsed.canDispatch,
        &parsed.speed,
        &parsed.gX,
        &parsed.gY,
        &parsed.gZ);

    if (parsedCount != 11)
    {
        return false;
    }

    outFrame = parsed;
    return true;
}

void pollTelemetrySerial(SerialTelemetryState &telemetryState)
{
    while (Serial.available() > 0)
    {
        char c = static_cast<char>(Serial.read());
        if (c == '\r')
        {
            continue;
        }

        if (c == '\n')
        {
            telemetryState.lineBuffer[telemetryState.lineLength] = '\0';
            if (parseTelemetryFrame(telemetryState.lineBuffer, telemetryState.frame))
            {
                telemetryState.hasFrame = true;
                telemetryState.lastRxMs = millis();
            }
            telemetryState.lineLength = 0;
            continue;
        }

        if (telemetryState.lineLength < (sizeof(telemetryState.lineBuffer) - 1))
        {
            telemetryState.lineBuffer[telemetryState.lineLength++] = c;
        }
        else
        {
            telemetryState.lineLength = 0;
        }
    }
}

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
void updateLCD(const SerialTelemetryState &telemetryState, bool telemetryFresh)
{
    lcd.cls();
    lcd.locate(1, 1);
    if (!telemetryFresh)
    {
        lcd.print("No telemetry");
        return;
    }

    char row1[21];
    char row2[21];
    snprintf(row1, sizeof(row1), "Spd:%6.2f  Gy:%5.2f", telemetryState.frame.speed, telemetryState.frame.gY);
    snprintf(row2, sizeof(row2), "In:%d Br:%d Seq:%lu",
             telemetryState.frame.inPlayMode,
             telemetryState.frame.braking,
             static_cast<unsigned long>(telemetryState.frame.sequence));
    lcd.print(row1);
    lcd.locate(2, 1);
    lcd.print(row2);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Runs at the start of the program to initialize components
/// \return void
void setup()
{
    Serial.begin(115200);

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
    static SerialTelemetryState telemetryState = {};

    bool keySwitchChanged = false;
    bool lcdParamChanged = false;
    static uint32_t lastLcdUpdateMs = 0;

    pollTelemetrySerial(telemetryState);
    bool telemetryFresh = telemetryState.hasFrame &&
                          ((millis() - telemetryState.lastRxMs) <= TELEMETRY_TIMEOUT_MS);

    if (keySwitch.getState(keySwitchChanged) == SwitchPosition::POSITION_2)
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
        if (keySwitchChanged)
        {
            lcdBacklight = HIGH;
            digitalWrite(LCD_BACKLIGHT, lcdBacklight);
            delay(1);
            lcd.begin(4, 20);
            lcdParamChanged = true;
        }

        if (telemetryFresh)
        {
            estopLed = telemetryState.frame.eStop ? HIGH : LOW;
            dispatchLed1 = telemetryState.frame.canDispatch ? HIGH : LOW;
            dispatchLed2 = telemetryState.frame.canDispatch ? HIGH : LOW;
            connectedLed = HIGH;
            engagedLed = telemetryState.frame.inPlayMode ? HIGH : LOW;
            readyLed = telemetryState.frame.trainInStation ? HIGH : LOW;
            trainStationLed = telemetryState.frame.trainInStation ? HIGH : LOW;
            faultLed = telemetryState.frame.paused ? HIGH : LOW;

            // Blink test LED while braking so serial updates can be visually verified.
            testLed = telemetryState.frame.braking ? globalBlinkState : LOW;
        }
        else
        {
            estopLed = globalBlinkState;
            dispatchLed1 = globalBlinkState;
            dispatchLed2 = globalBlinkState;
            connectedLed = globalBlinkState;
            engagedLed = globalBlinkState;
            readyLed = globalBlinkState;
            trainStationLed = globalBlinkState;
            faultLed = globalBlinkState;
            testLed = globalBlinkState;
        }

        lcdBacklight = HIGH;
        powerLed = globalBlinkState;
        resetLed = globalBlinkState;
        floorLed = globalBlinkState;
        seatsLed = globalBlinkState;
        restraintsLed = globalBlinkState;
        gatesLed = globalBlinkState;
    }

    if ((millis() - lastLcdUpdateMs) >= 200)
    {
        lcdParamChanged = true;
        lastLcdUpdateMs = millis();
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
      updateLCD(telemetryState, telemetryFresh);
    }

    // Sync loop timing and update blink counter
    syncTiming(lastLoopTimeUs);
    updateBlinkCounter(blinkCounterUs, globalBlinkState);
}
