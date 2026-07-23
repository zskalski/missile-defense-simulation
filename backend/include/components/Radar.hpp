#pragma once

#include "components/DetectedTarget.hpp"
#include "components/Missile.hpp"

#include <vector>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Radar {
    public:
        Radar(int x, int y, int range, std::string id);
        std::vector<DetectedTarget> scan(std::vector<Missile> missiles);
        void setRange(int range);
        friend void to_json(nlohmann::json& j, const Radar& radar);
    private:
        int x;
        int y;
        int range;
        std::string id;

        bool isInRange(Missile m);
};