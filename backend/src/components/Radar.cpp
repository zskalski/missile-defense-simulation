#include <vector>
#include <cmath>
#include <string>
#include "components/Radar.hpp"
#include "components/Missile.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Radar::Radar(int x_cord, int y_cord, int rnge, std::string identity) : x(x_cord), y(y_cord), range(rnge), id(identity) {}

std::vector<DetectedTarget> Radar::scan(std::vector<Missile> missiles) {
    std::vector<DetectedTarget> targets;
    for(int i = 0; i < missiles.size(); i++) {
        if (isInRange(missiles[i])) {
            targets.push_back(DetectedTarget((missiles[i]).getX(), (missiles[i]).getY(), (missiles[i]).getSpeed()));
        }
    }
    return targets;
}

void Radar::setRange(int rnge) {
    range = rnge;
}

bool Radar::isInRange(Missile m) {
    int maxVisibleDistance = range * 5 + 60;
    int distanceFromRadar = std::hypot(m.getX() - x, m.getY() - y);
    return maxVisibleDistance > distanceFromRadar;
}

void to_json(nlohmann::json& j, const Radar& radar) {
    j = nlohmann::json{
        {"id", radar.id},
        {"position", {
            {"x", radar.x},
            {"y", radar.y}
        }},
        {"range", radar.range}
    };
}