#ifndef SWITCH_COMMON_H
#define SWITCH_COMMON_H

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \enum SwitchConfigMode
/// \brief Enumeration for switch configuration modes-
enum class SwitchConfigMode
{
    ACTIVE_HIGH = 0, ///< Switch is configured to be active high
    ACTIVE_LOW       ///< Switch is configured to be active low
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \enum SwitchPosition
/// \brief Enumeration for the positions of the two position switch
enum class SwitchPosition
{
    POSITION_1 = 0,   ///< Switch is in position 1
    POSITION_2,       ///< Switch is in position 2
    POSITION_INVALID  ///< Switch is in an invalid position (neither position 1 nor position 2, or both simultaneously)
};

#endif //SWITCH_COMMON_H
