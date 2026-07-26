#pragma once

#include <nlohmann/json.hpp>

class DetectedTarget {

    public:
        DetectedTarget(int x, int y, int speed);
        int getX();
        int getY();
        int getSpeed();

        friend void to_json(nlohmann::json& j, const DetectedTarget& target);

    private:
        int x;
        int y;
        int speed;
};
