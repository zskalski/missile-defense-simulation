#pragma once

class DetectedTarget {

    public:
        DetectedTarget(int x, int y, int speed);
        int getX();
        int getY();
        int getSpeed();
    private:
        int x;
        int y;
        int speed;
};