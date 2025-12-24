#include "LEDButton.h"

LEDButton::LEDButton(const pin_size_t btnPin, const pin_size_t ledPin, const ButtonConfigMode configMode)
    : m_btnPin(btnPin), m_ledPin(ledPin), m_configMode(configMode)
{
    pinMode(m_btnPin, INPUT);
    pinMode(m_ledPin, OUTPUT);
}

void LEDButton::setLED(const PinStatus state)
{
    digitalWrite(m_ledPin, state);
}

ButtonState LEDButton::getButtonState()
{
    PinStatus buttonState = digitalRead(m_btnPin);
    ButtonState newState;

    if (m_configMode == ButtonConfigMode::ACTIVE_HIGH)
    {
        newState = (buttonState == HIGH) ? ButtonState::BUTTON_PRESSED : ButtonState::BUTTON_RELEASED;
    }
    else // ACTIVE_LOW
    {
        newState = (buttonState == LOW) ? ButtonState::BUTTON_PRESSED : ButtonState::BUTTON_RELEASED;
    }

    m_buttonState = newState;
    return m_buttonState;
}

ButtonState LEDButton::getButtonState(bool &stateChanged)
{
    PinStatus buttonState = digitalRead(m_btnPin);
    ButtonState newState;

    if (m_configMode == ButtonConfigMode::ACTIVE_HIGH)
    {
        newState = (buttonState == HIGH) ? ButtonState::BUTTON_PRESSED : ButtonState::BUTTON_RELEASED;
    }
    else // ACTIVE_LOW
    {
        newState = (buttonState == LOW) ? ButtonState::BUTTON_PRESSED : ButtonState::BUTTON_RELEASED;
    }

    stateChanged = stateChanged || (newState != m_buttonState);
    m_buttonState = newState;
    return m_buttonState;
}
