#pragma once

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Missile {
    public:
        Missile(int x, int y, int speed, int x_dest, int y_dest, std::string target_id, std::string identity);
       
        int getX();
        int getY();
        int getSpeed();
        int getXDestination();
        int getYDestination();
        std::string getTargetID();
        bool isBlownUp();

        void advance();

        friend void to_json(json& j, const Missile& missile);

    private:
        int x;
        int y;
        int speed;
        int x_destination;
        int y_destination;
        int x_original;
        int y_original;
        bool blownUp;
        std::string target_id;
        std::string id;
};
