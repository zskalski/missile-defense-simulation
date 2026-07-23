#include "components/DetectedTarget.hpp"

DetectedTarget::DetectedTarget(int x_location, int y_location, int spd) : x(x_location), y(y_location), speed(spd) {}

int DetectedTarget::getX() {return x;}
int DetectedTarget::getY() {return y;}
int DetectedTarget::getSpeed() {return speed;}