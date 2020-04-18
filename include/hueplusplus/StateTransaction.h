/**
    \file StateTransaction.h
    Copyright Notice\n
    Copyright (C) 2020  Jan Rogall		- developer\n
    Copyright (C) 2020  Moritz Wirger	- developer\n

    This file is part of hueplusplus.

    hueplusplus is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    hueplusplus is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with hueplusplus.  If not, see <http://www.gnu.org/licenses/>.
**/

#ifndef INCLUDE_HUEPLUSPLUS_STATE_TRANSACTION_H
#define INCLUDE_HUEPLUSPLUS_STATE_TRANSACTION_H

#include <string>

#include "HueCommandAPI.h"

#include "json/json.hpp"

namespace hueplusplus
{
//! \brief Transaction class which can be used for either light or group state.
//!
//! This is intended to be used in-line, all calls are chained until a \ref commit() call.
//! \code
//! light.transaction().setOn(true).setBrightness(29).setColorHue(3000).setColorSaturation(128).commit();
//! \endcode
class StateTransaction
{
public:
    //! \brief Creates a StateTransaction to a group or light state
    //! \param commands HueCommandAPI for making requests
    //! \param path Path to which the final PUT request is made (without username)
    //! \param currentState JSON object with the current state to check whether changes are needed.
    //! Pass an empty object to always include all requests (for groups, because individual lights might be different).
    StateTransaction(const HueCommandAPI& commands, const std::string& path, const nlohmann::json& currentState);

    //! \brief Deleted copy constructor, do not store StateTransaction in a variable.
    StateTransaction(const StateTransaction&) = delete;
    StateTransaction(StateTransaction&&) = default;

    //! \brief Commit transaction and make request.
    //! \returns true on success or when no change was requested.
    //! \note After changing the state of a HueLight or Group, 
    //! refresh() must be called if the updated values are needed immediately.
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contains no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    bool commit() &&;

    //! \brief Turn light on or off.
    //! \param on true for on, false for off
    //! \returns This transaction for chaining calls
    StateTransaction&& setOn(bool on) &&;
    //! \brief Set light brightness.
    //! \param brightness Brightness from 0 = off to 254 = fully lit.
    //! \returns This transaction for chaining calls
    //! \note If this transaction is for a light, the light needs to have brightness control.
    //! \note Brightness 0 will also turn off the light if nothing else is specified,
    //! any other value will also turn on the light.
    StateTransaction&& setBrightness(uint8_t brightness) &&;
    //! \brief Set light hue.
    //! \param hue Color hue from 0 to 65535
    //! \returns This transaction for chaining calls
    //! \note If this transaction is for a light, the light needs to have rgb color control.
    //! \note Will also turn on the light if nothing else is specified
    StateTransaction&& setColorHue(uint16_t hue) &&;
    //! \brief Set light saturation.
    //! \param saturation Color saturation from 0 to 254
    //! \returns This transaction for chaining calls
    //! \note If this transaction is for a light, the light needs to have rgb color control.
    //! \note Will also turn on the light if nothing else is specified
    StateTransaction&& setColorSaturation(uint8_t saturation) &&;
    //! \brief Set light color in xy space.
    //! \param x x coordinate in CIE color space
    //! \param y y coordinate in CIE color space
    //! \returns This transaction for chaining calls
    //! \note If this transaction is for a light, the light needs to have rgb color control.
    //! \note Will also turn on the light if nothing else is specified
    StateTransaction&& setColorXY(float x, float y) &&;
    //! \brief Set light color temperature.
    //! \param mired Color temperature in mired from 153 to 500
    //! \returns This transaction for chaining calls
    //! \note If this transaction is for a light, the light needs to have color temperature control.
    //! \note Will also turn on the light if nothing else is specified
    StateTransaction&& setColorTemperature(unsigned int mired) &&;
    //! \brief Enables or disables color loop.
    //! \param on true to enable, false to disable color loop.
    //! \returns This transaction for chaining calls
    //! \note If this transaction is for a light, the light needs to have rgb color control.
    //! \note Will also turn on the light if nothing else is specified
    StateTransaction&& setColorLoop(bool on) &&;
    //! \brief Increment/Decrement brightness.
    //! \param increment Brightness change from -254 to 254.
    //! \returns This transaction for chaining calls
    //! \note If this transaction is for a light, the light needs to have brightness control.
    StateTransaction&& incrementBrightness(int increment) &&;
    //! \brief Increment/Decrement saturaction.
    //! \param increment Saturation change from -254 to 254.
    //! \returns This transaction for chaining calls
    //! \note If this transaction is for a light, the light needs to have rgb color control.
    StateTransaction&& incrementSaturation(int increment) &&;
    //! \brief Increment/Decrement hue.
    //! \param increment Hue change from -65535 to 65535.
    //! \returns This transaction for chaining calls
    //! \note If this transaction is for a light, the light needs to have rgb color control.
    StateTransaction&& incrementHue(int increment) &&;
    //! \brief Increment/Decrement color temperature.
    //! \param increment Color temperature change in mired from -65535 to 65535.
    //! \returns This transaction for chaining calls
    //! \note If this transaction is for a light, the light needs to have color temperature control.
    StateTransaction&& incrementColorTemperature(int increment) &&;
    //! \brief Increment/Decrement color xy.
    //! \param xInc x color coordinate change from -0.5 to 0.5.
    //! \param yInc y color coordinate change from -0.5 to 0.5.
    //! \returns This transaction for chaining calls
    //! \note If this transaction is for a light, the light needs to have rgb color control.
    StateTransaction&& incrementColorXY(float xInc, float yInc) &&;
    //! \brief Set transition time for the request.
    //! \param transition Transition time in 100ms, default for any request is 400ms.
    //! \returns This transaction for chaining calls
    //! \note The transition only applies to the current request. 
    //! A request without any changes only containing a transition is pointless and is not sent.
    StateTransaction&& setTransition(uint16_t transition) &&;
    //! \brief Trigger an alert.
    //!
    //! The light performs one breathe cycle.
    //! \returns This transaction for chaining calls
    StateTransaction&& alert() &&;
    //! \brief Trigger a long alert (15s).
    //! \returns This transaction for chaining calls
    StateTransaction&& longAlert() &&;
    //! \brief Stop an ongoing long alert.
    //! \returns This transaction for chaining calls
    StateTransaction&& stopAlert() &&;

protected:
    const HueCommandAPI& commands;
    std::string path;
    nlohmann::json state;
    nlohmann::json request;
};

} // namespace hueplusplus

#endif