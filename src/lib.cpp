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
#include <cpr/cpr.h>
#include <exception>
#include <nlohmann/json.hpp>
#include <ostream>
#include <splatdotink.hpp>
#include <stdexcept>
#include <string>
#ifdef SDI_DEBUG_BUILD
#include <sample_data.h>
#endif
#define SDI_DATA_URL "https://splatoon2.ink/data/"

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

    Schedules Schedules::fetch() {
#ifndef SDI_DEBUG_BUILD
        cpr::Response r(cpr::Get(cpr::Url { SDI_DATA_URL "schedules.json" },
            cpr::Header { "User-Agent", "splatdotink-lib/" PACKAGE_VERSION }));

        if (r.status_code != 200)
            throw std::runtime_error(r.error.message);

        return Schedules(r.text);
#else

        std::string text(reinterpret_cast<char*>(test_sample_data_json), test_sample_data_json_len);
        return Schedules(text);
#endif
    }

    Schedules::Schedules(std::string text)
        : m_turf()
        , m_ranked()
        , m_league() {

        json j(json::parse(text));

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

    namespace salmonrun {

        Rotation::Rotation(std::chrono::system_clock::time_point m_start,
            std::chrono::system_clock::time_point m_end,
            std::optional<std::string> m_stage,
            std::optional<std::vector<Weapon>> weapons)
            : m_start_time(m_start)
            , m_end_time(m_end)
            , m_stage(m_stage)
            , m_weapons(weapons) { }

        std::vector<Rotation> fetch() {

#ifdef SDI_DEBUG_BUILD
#include <salmon_sample.h>
            std::string text(reinterpret_cast<char*>(test_salmon_sample_json), test_salmon_sample_json_len);
#else
            cpr::Response r(cpr::Get(cpr::Url { SDI_DATA_URL "coop-schedules.json" }),
                cpr::Header { "User-Agent", "splatdotink-lib/" PACKAGE_VERSION });
            if (r.status_code != 200)
                throw std::runtime_error(r.error.message);

            std::string text(r.text);
#endif

            json j(json::parse(text));

            auto schedules(j["schedules"]);
            auto details(j["details"]);

            std::vector<Rotation> ret;

            for (int i = 0; i < 5; i++) {
                std::chrono::system_clock::time_point start{std::chrono::seconds(schedules[i]["start_time"])};
                std::chrono::system_clock::time_point end{std::chrono::seconds(schedules[i]["end_time"])};

                std::optional<std::string> stage(std::nullopt);
                std::optional<std::vector<Rotation::Weapon>> weapons(std::nullopt);

                if (i < 2) {
                    stage = details[i]["stage"]["name"];
                    weapons = {};
                    for (auto& weapon: details[i]["weapons"]) {
                        weapons.value().push_back({ weapon["id"], weapon["weapon"]["name"] });
                    }
                }

                ret.push_back(Rotation(start, end, stage, weapons));
            }

            return ret;
        }

    }; // namespace salmonrun

} // namespace splatdotink
