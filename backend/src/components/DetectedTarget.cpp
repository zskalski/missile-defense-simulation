#include "components/DetectedTarget.hpp"

#include <nlohmann/json.hpp>

DetectedTarget::DetectedTarget(int x_location, int y_location, int spd) : x(x_location), y(y_location), speed(spd) {}

int DetectedTarget::getX() {return x;}
int DetectedTarget::getY() {return y;}
int DetectedTarget::getSpeed() {return speed;}

void to_json(nlohmann::json& j, const DetectedTarget& target) {
    j = nlohmann::json{
        {"position", {
            {"x", target.x},
            {"y", target.y}
        }},
        {"speed", target.speed}
    };
}
