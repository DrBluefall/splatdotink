// The main library file for splatdotink.

/*
This file is part of splatdotink.

splatdotink is free software: you can redistribute it and/or modify it under the
terms of the GNU Lesser General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

splatdotink is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along
with splatdotink. If not, see <https://www.gnu.org/licenses/>.
*/
#include <chrono>
#include <exception>
#include <nlohmann/json.hpp>
#include <ostream>
#include <restclient-cpp/restclient.h>
#include <splatdotink.hpp>
#include <stdexcept>
#include <string>

using json = nlohmann::json;

namespace splatdotink {

    Gamemode::Gamemode(const std::string& str)
        : val(Gamemode::TurfWar) {
        if (str == "turf_war") {
            // turf_war is the default, so we're good here.
        } else if (str == "splat_zones") {
            val = Gamemode::SplatZones;
        } else if (str == "tower_control") {
            val = Gamemode::TowerControl;
        } else if (str == "rainmaker") {
            val = Gamemode::Rainmaker;
        } else if (str == "clam_blitz") {
            val = Gamemode::ClamBlitz;
        } else {
            throw std::invalid_argument("No valid type for gamemode '" + str + "'");
        }
    }
    std::ostream& operator<<(std::ostream& os, const splatdotink::Gamemode& gm) {

        switch (gm) {
        case splatdotink::Gamemode::TurfWar:
            os << "Turf War";
            break;
        case splatdotink::Gamemode::SplatZones:
            os << "Splat Zones";
            break;
        case splatdotink::Gamemode::Rainmaker:
            os << "Rainmaker";
            break;
        case splatdotink::Gamemode::TowerControl:
            os << "Tower Control";
            break;
        case splatdotink::Gamemode::ClamBlitz:
            os << "Clam Blitz";
            break;
        }

        return os;
    }
    Gamemode Rotation::mode() const { return m_mode; };

    Rotation::Rotation(const json& input)
        : m_id(input["id"])
        , m_start_time(std::chrono::system_clock::time_point(std::chrono::seconds(input["start_time"])))
        , m_end_time(std::chrono::system_clock::time_point(std::chrono::seconds(input["end_time"])))
        , m_stage_a(Stage(input["stage_a"]))
        , m_stage_b(Stage(input["stage_b"]))
        , m_mode(Gamemode(input["rule"]["key"].get<std::string>())) { }

    std::uint8_t Rotation::id() const { return m_id; };
    std::chrono::system_clock::time_point Rotation::start_time() const { return m_start_time; };
    std::chrono::system_clock::time_point Rotation::end_time() const { return m_end_time; };
    Stage Rotation::stage_a() const { return m_stage_a; };
    Stage Rotation::stage_b() const { return m_stage_b; };

    Stage::Stage(const json& map)
        : m_id(static_cast<std::uint8_t>(std::stoi(map["id"].get<std::string>())))
        , m_name(map["name"]) { }

    std::vector<Rotation> Schedules::turf() const { return m_turf; }
    std::vector<Rotation> Schedules::ranked() const { return m_ranked; }
    std::vector<Rotation> Schedules::league() const { return m_league; }

    Schedules::Schedules()
        : m_turf(std::vector<Rotation>())
        , m_ranked(std::vector<Rotation>())
        , m_league(std::vector<Rotation>()) {
        RestClient::Response resp = RestClient::get("https://splatoon2.ink/data/schedules.json");

        json j = json::parse(resp.body);

        for (const json& obj : j["regular"]) {
            m_turf.push_back(Rotation(obj));
        }
        for (const json& obj : j["gachi"]) {
            m_ranked.push_back(Rotation(obj));
        }
        for (const json& obj : j["league"]) {
            m_league.push_back(Rotation(obj));
        }
    }
} // namespace splatdotink
