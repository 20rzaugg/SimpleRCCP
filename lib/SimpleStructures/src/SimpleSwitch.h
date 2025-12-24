#ifndef SIMPLESWITCH_H
#define SIMPLESWITCH_H

#include <Arduino.h>
#include "SwitchCommon.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class SimpleSwitch
/// \brief Class to handle a single switch connected to one digital input pin
class SimpleSwitch
{
public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Constructor for SimpleSwitch class
    /// \param pin Pin number for the switch
    /// \param configMode Configuration mode for the switch
    SimpleSwitch(pin_size_t pin, SwitchConfigMode configMode);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Reads the state of the switch
    /// \return Current state of the switch
    SwitchPosition getState();

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Reads the state of the switch
    /// \param[out] stateChanged Boolean flag indicating if the switch state has changed since the last read
    /// \return Current state of the switch
    SwitchPosition getState(bool &stateChanged);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Gets the last saved state of the switch without reading the pin
    /// \return The last saved state of the switch
    [[nodiscard]] SwitchPosition getSavedState() const;

protected:
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Reads the raw state of the switch without updating internal state
    /// \return The raw state of the switch
    SwitchPosition getRawState() const;

private:
    /// \brief Pin number for the switch
    pin_size_t m_pin;
    /// \brief Configuration mode for the switch
    SwitchConfigMode m_configMode;
    /// \brief Last known state of the switch
    SwitchPosition m_state = SwitchPosition::POSITION_INVALID;
};

#endif // SIMPLESWITCH_H
