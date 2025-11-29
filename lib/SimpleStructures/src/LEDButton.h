#ifndef LEDBUTTON_H
#define LEDBUTTON_H

#include <Arduino.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \enum ButtonConfigMode
/// \brief Enumeration for button configuration modes-
enum ButtonConfigMode
{
    ACTIVE_HIGH = 0, ///< Button is configured to be active high
    ACTIVE_LOW       ///< Button is configured to be active low
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \enum ButtonState
/// \brief Enumeration for button states
enum ButtonState
{
    BUTTON_RELEASED = 0, ///< Button is released
    BUTTON_PRESSED       ///< Button is pressed
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class LEDButton
/// \brief Class to handle a button with a built-in LED
class LEDButton
{
public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Constructor for LEDButton class
    /// \param[in] btnPin Pin number for the button
    /// \param[in] ledPin Pin number for the built-in LED
    /// \param[in] configMode Configuration mode for the button (active high or active low)
    LEDButton(const pin_size_t btnPin, const pin_size_t ledPin, const ButtonConfigMode configMode);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Sets the state of the built-in LED
    /// \param[in] state Desired state of the LED (HIGH or LOW)
    /// \return void
    void setLED(const PinStatus state);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Checks if the button is currently pressed
    /// \return true if the button is pressed, false otherwise
    ButtonState getButtonState();

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Checks if the button is currently pressed
    /// \param[out] stateChanged Boolean flag indicating if the button state has changed since the last read
    /// \return true if the button is pressed, false otherwise
    ButtonState getButtonState(bool &stateChanged);

private:
    /// \brief Pin number for the button
    pin_size_t m_btnPin;
    /// \brief Pin number for the built-in LED
    pin_size_t m_ledPin;
    /// \brief Current state of the button
    ButtonState m_buttonState = BUTTON_RELEASED;
    /// \brief Configuration mode for the button
    ButtonConfigMode m_configMode;
};

#endif // LEDBUTTON_H
