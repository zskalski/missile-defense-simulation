#include "components/Missile.hpp"

#include <algorithm>
#include <cmath>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Missile::Missile(int x_cord, int y_cord, int spd, int x_dest, int y_dest, std::string target, std::string identity)
    : x(x_cord),
      y(y_cord),
      preciseX(x_cord),
      preciseY(y_cord),
      speed(spd),
      x_destination(x_dest),
      y_destination(y_dest),
      x_original(x_cord),
      y_original(y_cord),
      blownUp(false),
      target_id(target),
      id(identity) {}

int Missile::getX() {return x;}

int Missile::getY() {return y;}

int Missile::getSpeed() {return speed;}

int Missile::getXDestination() {return x_destination;}

int Missile::getYDestination() {return y_destination;}

double Missile::getDirectionDegrees() const {
    int xDirection = x_destination - x;
    int yDirection = y_destination - y;

    if (xDirection == 0 && yDirection == 0) {
        xDirection = x_destination - x_original;
        yDirection = y_destination - y_original;
    }

    double radiansToDegrees = 180.0 / 3.14159265358979323846;
    double degrees = std::atan2(yDirection, xDirection) * radiansToDegrees;

    if (degrees < 0) {
        degrees += 360.0;
    }

    return degrees;
}

std::string Missile::getTargetID() {return target_id;}

bool Missile::isBlownUp() {return blownUp;}

void Missile::advance() {

    if (blownUp)
        return;

    if (x == x_destination && y == y_destination) {
        blownUp = true;
        return;
    }

    const double xDistance = x_destination - preciseX;
    const double yDistance = y_destination - preciseY;
    const double distanceRemaining = std::hypot(xDistance, yDistance);

    if (distanceRemaining <= speed) {
        preciseX = x_destination;
        preciseY = y_destination;
        x = x_destination;
        y = y_destination;
        blownUp = true;
        return;
    }

    const double xStep = (xDistance / distanceRemaining) * speed;
    const double yStep = (yDistance / distanceRemaining) * speed;

    preciseX += xStep;
    preciseY += yStep;
    x = static_cast<int>(std::round(preciseX));
    y = static_cast<int>(std::round(preciseY));
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
        {"direction", {
            {"degrees", missile.getDirectionDegrees()}
        }},
        {"target_id", missile.target_id},
        {"blownUp", missile.blownUp}
    };
}
