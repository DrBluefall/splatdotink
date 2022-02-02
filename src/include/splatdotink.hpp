#ifndef SPLATDOTINK_H_
#define SPLATDOTINK_H_

#include <chrono>
#include <cstdint>
#include <nlohmann/json.hpp>
#include <ostream>
#include <string>

namespace splatdotink {

    class Gamemode {

      public:
        enum Value : std::uint8_t {
            TurfWar,
            SplatZones,
            Rainmaker,
            TowerControl,
            ClamBlitz,
        };

        Gamemode() = default;
        Gamemode(const std::string& str);
        constexpr Gamemode(Value a)
            : val(a) { }

        operator Value() const { return val; }
        explicit operator bool() = delete;
        friend std::ostream& operator<<(std::ostream& os, const splatdotink::Gamemode& gm);

      private:
        Value val;
    };

    class Stage {
      private:
        std::uint8_t m_id;
        std::string m_name;

      public:
        std::uint8_t id() const { return m_id; }
        std::string name() const { return m_name; }
        Stage(const nlohmann::json&);
    };

    class Rotation {
      private:
        std::uint64_t m_id;
        std::chrono::system_clock::time_point m_start_time;
        std::chrono::system_clock::time_point m_end_time;
        Stage m_stage_a;
        Stage m_stage_b;
        Gamemode m_mode;

      public:
        std::uint8_t id() const;
        std::chrono::system_clock::time_point start_time() const;
        std::chrono::system_clock::time_point end_time() const;
        Stage stage_a() const;
        Stage stage_b() const;
        Gamemode mode() const;
        Rotation(const nlohmann::json&);
    };

    class Schedules {
      private:
        std::vector<Rotation> m_turf;
        std::vector<Rotation> m_ranked;
        std::vector<Rotation> m_league;
        Schedules(std::vector<Rotation> t, std::vector<Rotation> r, std::vector<Rotation> l);

      public:
        std::vector<Rotation> turf() const;
        std::vector<Rotation> ranked() const;
        std::vector<Rotation> league() const;
        Schedules(std::string);
        static Schedules fetch();
    };
}

#endif // SPLATDOTINK_H_
