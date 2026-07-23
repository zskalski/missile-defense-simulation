
#pragma once

class SimulationOptions {
    public:
        SimulationOptions(bool autoM, int radar, int sim);

        bool getAuto();
        int getRadarVis();
        int getSimSpeed();

        void printOptions();        // for debug

        // returns false for error
        bool setAuto(bool mode);
        bool setRadarVis(unsigned short vis);
        bool setSimSpeed(unsigned short speed);

    private:
        bool autoMode;
        int radarVis;
        int simSpeed;
};
