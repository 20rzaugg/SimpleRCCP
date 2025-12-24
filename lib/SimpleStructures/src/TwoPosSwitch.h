#ifndef TWOPOSSWITCH_H
#define TWOPOSSWITCH_H

#include <Arduino.h>
#include "SwitchCommon.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class TwoPosSwitch
/// \brief Class to handle a two position switch connected to two digital input pins
class TwoPosSwitch
{
public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Constructor for TwoPosSwitch class
    /// \param pinPos1 Pin number for position 1
    /// \param pinPos2 Pin number for position 2
    /// \param configMode Configuration mode for the switch
    TwoPosSwitch(pin_size_t pinPos1, pin_size_t pinPos2, SwitchConfigMode configMode);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Reads the position of the switch
    /// \return Current position of the switch
    SwitchPosition getPosition();

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Reads the position of the switch
    /// \param[out] stateChanged Boolean flag indicating if the switch position has changed since the last read
    /// \return Current position of the switch
    SwitchPosition getPosition(bool &stateChanged);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Gets the last saved position of the switch without reading the pins
    /// \return The last saved position of the switch
    [[nodiscard]] SwitchPosition getSavedPosition() const;

protected:
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Reads the raw position of the switch without updating internal state
    /// \return The raw position of the switch
    SwitchPosition getRawPosition() const;

private:
    /// \brief Pin number for position 1
    pin_size_t m_pinPos1;
    /// \brief Pin number for position 2
    pin_size_t m_pinPos2;
    /// \brief Configuration mode for the switch
    SwitchConfigMode m_configMode;
    /// \brief Last known position of the switch
    SwitchPosition m_position = SwitchPosition::POSITION_INVALID;
};

#endif // TWOPOSSWITCH_H
