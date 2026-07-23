#include "components/Missile.hpp"

#include <algorithm>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Missile::Missile(int x_cord, int y_cord, int spd, int x_dest, int y_dest, std::string target, std::string identity) : x(x_cord), y(y_cord), speed(spd), x_destination(x_dest), y_destination(y_dest), x_original(x_cord), y_original(y_cord), blownUp(false), target_id(target), id(identity) {}

int Missile::getX() {return x;}

int Missile::getY() {return y;}

int Missile::getSpeed() {return speed;}

int Missile::getXDestination() {return x_destination;}

int Missile::getYDestination() {return y_destination;}

std::string Missile::getTargetID() {return target_id;}

bool Missile::isBlownUp() {return blownUp;}

void Missile::advance() {

    if (blownUp)
        return;

    if (x == x_destination && y == y_destination) {
        blownUp = true;
        return;
    }

    // find the direction to go to
    if (x_destination - x_original > 0) {
        // x must increase
        x = std::min(x + speed, x_destination);
    } else {
        // x must decrease
        x = std::max(x - speed, x_destination);
    }

    if (y_destination - y_original > 0) {
        // increase
        y = std::min(y + speed, y_destination);
    } else {
        // decrease
        y = std::max(y - speed, y_destination);
    }
}

void to_json(json& j, const Missile& missile) {
    j = json{
        {"id", missile.id},

        {"position", {
            {"x", missile.x},
            {"y", missile.y}
        }},

        {"originalPosition", {
            {"x", missile.x_original},
            {"y", missile.y_original}
        }},

        {"destination", {
            {"x", missile.x_destination},
            {"y", missile.y_destination}
        }},

        {"speed", missile.speed},
        {"target_id", missile.target_id},
        {"blownUp", missile.blownUp}
    };
}
