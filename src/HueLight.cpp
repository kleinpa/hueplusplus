/**
        \file HueLight.cpp
        Copyright Notice\n
        Copyright (C) 2017  Jan Rogall		- developer\n
        Copyright (C) 2017  Moritz Wirger	- developer\n

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

#include "hueplusplus/HueLight.h"

#include <cmath>
#include <iostream>
#include <thread>

#include "hueplusplus/HueExceptionMacro.h"
#include "hueplusplus/Utils.h"
#include "json/json.hpp"

namespace hueplusplus
{
bool HueLight::On(uint8_t transition)
{
    return transaction().setOn(true).setTransition(transition).commit();
}

bool HueLight::Off(uint8_t transition)
{
    return transaction().setOn(false).setTransition(transition).commit();
}

bool HueLight::isOn()
{
    return state.GetValue().at("state").at("on").get<bool>();
}

bool HueLight::isOn() const
{
    return state.GetValue().at("state").at("on").get<bool>();
}

int HueLight::getId() const
{
    return id;
}

std::string HueLight::getType() const
{
    return state.GetValue()["type"].get<std::string>();
}

std::string HueLight::getName()
{
    return state.GetValue()["name"].get<std::string>();
}

std::string HueLight::getName() const
{
    return state.GetValue()["name"].get<std::string>();
}

std::string HueLight::getModelId() const
{
    return state.GetValue()["modelid"].get<std::string>();
}

std::string HueLight::getUId() const
{
    return state.GetValue().value("uniqueid", std::string());
}

std::string HueLight::getManufacturername() const
{
    return state.GetValue().value("manufacturername", std::string());
}

std::string HueLight::getProductname() const
{
    return state.GetValue().value("productname", std::string());
}

std::string HueLight::getLuminaireUId() const
{
    return state.GetValue().value("luminaireuniqueid", std::string());
}

std::string HueLight::getSwVersion()
{
    return state.GetValue()["swversion"].get<std::string>();
}

std::string HueLight::getSwVersion() const
{
    return state.GetValue()["swversion"].get<std::string>();
}

bool HueLight::setName(const std::string& name)
{
    nlohmann::json request = nlohmann::json::object();
    request["name"] = name;
    nlohmann::json reply = SendPutRequest(request, "/name", CURRENT_FILE_INFO);
    state.Refresh();

    // Check whether request was successful (returned name is not necessarily the actually set name)
    // If it already exists, a number is added, if it is too long to be returned, "Updated" is returned
    return utils::safeGetMember(reply, 0, "success", "/lights/" + std::to_string(id) + "/name").is_string();
}

ColorType HueLight::getColorType() const
{
    return colorType;
}

unsigned int HueLight::KelvinToMired(unsigned int kelvin) const
{
    return int(0.5f + (1000000 / kelvin));
}

unsigned int HueLight::MiredToKelvin(unsigned int mired) const
{
    return int(0.5f + (1000000 / mired));
}

bool HueLight::alert()
{
    return transaction().alert().commit();
}

StateTransaction HueLight::transaction()
{
    return StateTransaction(commands, "/lights/" + std::to_string(id) + "/state", state.GetValue().at("state"));
}

HueLight::HueLight(int id, const HueCommandAPI& commands) : HueLight(id, commands, nullptr, nullptr, nullptr) {}

HueLight::HueLight(int id, const HueCommandAPI& commands, std::shared_ptr<const BrightnessStrategy> brightnessStrategy,
    std::shared_ptr<const ColorTemperatureStrategy> colorTempStrategy,
    std::shared_ptr<const ColorHueStrategy> colorHueStrategy, std::chrono::steady_clock::duration refreshDuration)
    : id(id),
      state("/lights/" + std::to_string(id), commands, refreshDuration),
      colorType(ColorType::NONE),
      brightnessStrategy(std::move(brightnessStrategy)),
      colorTemperatureStrategy(std::move(colorTempStrategy)),
      colorHueStrategy(std::move(colorHueStrategy)),
      commands(commands)
{
    state.Refresh();
}

nlohmann::json HueLight::SendPutRequest(const nlohmann::json& request, const std::string& subPath, FileInfo fileInfo)
{
    return commands.PUTRequest("/lights/" + std::to_string(id) + subPath, request, std::move(fileInfo));
}
} // namespace hueplusplus
