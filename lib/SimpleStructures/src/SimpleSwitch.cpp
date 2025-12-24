#include "SimpleSwitch.h"

SimpleSwitch::SimpleSwitch(const pin_size_t pin, const SwitchConfigMode configMode)
    : m_pin(pin), m_configMode(configMode), m_state(SwitchPosition::POSITION_INVALID)
{
    if (m_configMode == SwitchConfigMode::ACTIVE_LOW)
    {
        pinMode(m_pin, INPUT_PULLUP);
    }
    else
    {
        pinMode(m_pin, INPUT);
    }
}

SwitchPosition SimpleSwitch::getRawState() const
{
    PinStatus pinState = digitalRead(m_pin);

    if (m_configMode == SwitchConfigMode::ACTIVE_HIGH)
    {
        return (pinState == HIGH) ? SwitchPosition::POSITION_1 : SwitchPosition::POSITION_2;
    }
    else
    {
        return (pinState == LOW) ? SwitchPosition::POSITION_1 : SwitchPosition::POSITION_2;
    }
}

SwitchPosition SimpleSwitch::getState()
{
    m_state = getRawState();
    return m_state;
}

SwitchPosition SimpleSwitch::getState(bool &stateChanged)
{
    SwitchPosition newState = getRawState();
    stateChanged = stateChanged || (newState != m_state);
    m_state = newState;
    return m_state;
}

SwitchPosition SimpleSwitch::getSavedState() const
{
    return m_state;
}
