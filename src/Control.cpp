#include "Gladiator.hpp"
#include <algorithm>

float kw = 1.2;
float kv = 1.f;
float wlimit = 3.f;
float vlimit = 0.6;
float erreurPos = 0.07;

double reductionAngle(double x)
{
    x = fmod(x + PI, 2 * PI);
    if (x < 0)
        x += 2 * PI;
    return x - PI;
}
void go_to(Position cons, Position pos)
{
    double consvl, consvr;
    double dx = cons.x - pos.x;
    double dy = cons.y - pos.y;
    double d = sqrt(dx * dx + dy * dy);

    if (d > erreurPos)
    {
        double rho = atan2(dy, dx);
        double alpha = reductionAngle(rho - pos.a);

        bool reverse = (abs(alpha) > PI / 2);
        if (reverse) {
            alpha = reductionAngle(alpha + PI);
            d = -d;
        }

        double consw = kw * alpha;
        double consv = kv * d * cos(alpha);

        consw = std::clamp(consw, -wlimit, wlimit);
        consv = std::clamp(consv, -vlimit, vlimit);

        double robotRadius = gladiator->robot->getRobotRadius();
        consvl = consv - robotRadius * consw;
        consvr = consv + robotRadius * consw;
    }
    else
    {
        consvr = 0;
        consvl = 0;
    }

    gladiator->control->setWheelSpeed(WheelAxis::RIGHT, consvr, false);
    gladiator->control->setWheelSpeed(WheelAxis::LEFT, consvl, false);
}
