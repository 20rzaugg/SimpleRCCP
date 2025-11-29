#include "TwoPosSwitch.h"

TwoPosSwitch::TwoPosSwitch(const pin_size_t pinPos1, const pin_size_t pinPos2)
    : m_pinPos1(pinPos1), m_pinPos2(pinPos2), m_position(SwitchPosition::POSITION_INVALID)
{
    pinMode(m_pinPos1, INPUT);
    pinMode(m_pinPos2, INPUT);
}

SwitchPosition TwoPosSwitch::getRawPosition() const
{
    PinStatus pos1State = digitalRead(m_pinPos1);
    PinStatus pos2State = digitalRead(m_pinPos2);

    SwitchPosition newPosition;

    if (pos1State == HIGH && pos2State == LOW)
    {
        // Switch is in position 1
        newPosition = SwitchPosition::POSITION_1;
    }
    else if (pos1State == LOW && pos2State == HIGH)
    {
        // Switch is in position 2
        newPosition = SwitchPosition::POSITION_2;
    }
    else
    {
        // Switch is in an invalid position
        newPosition = SwitchPosition::POSITION_INVALID;
    }

    return newPosition;
}

SwitchPosition TwoPosSwitch::getPosition()
{
    m_position = getRawPosition();
    return m_position;
}

SwitchPosition TwoPosSwitch::getPosition(bool &stateChanged)
{
    SwitchPosition newPosition = getRawPosition();
    stateChanged = stateChanged || (newPosition != m_position);
    m_position = newPosition;
    return m_position;
}

SwitchPosition TwoPosSwitch::getSavedPosition() const
{
    return m_position;
}
