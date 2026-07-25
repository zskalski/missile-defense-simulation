
#pragma once

#include <mutex>

class SimulationOptions {
    public:
        SimulationOptions(bool autoM, int radar, int sim);

        bool getAuto() const;
        int getRadarVis() const;
        int getSimSpeed() const;

        void printOptions() const;        // for debug

        // returns false for error
        bool setAuto(bool mode);
        bool setRadarVis(unsigned short vis);
        bool setSimSpeed(unsigned short speed);

    private:
        mutable std::mutex optionsMutex;

        bool autoMode;
        int radarVis;
        int simSpeed;
};
