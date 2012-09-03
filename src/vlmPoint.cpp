#include "vlmPoint.h"
#include "Util.h"
#include "Orthodromie.h"

#include "Polar.h"
#include <QLineF>

vlmPoint::vlmPoint(double lon,double lat)
{
    this->lon=lon;
    this->lat=lat;
    this->origin=NULL;
    this->isStart=false;
    this->startCap=-1;
    this->isDead=false;
    this->eta=0;
    this->distIso=-1;
    this->capOrigin=0;
    this->needRoute=false;
    this->isBroken=false;
    this->debugInt=0;
    this->internal_1=0;
    this->internal_2=0;
    this->capVmg=0;
    this->notSimplificable=false;
    this->maxDistIso=10e5;
    this->isPOI=false;
    this->xP1=10e5;
    this->yP1=10e5;
    this->xM1=10e5;
    this->yM1=10e5;
    this->speed=10e5;
    this->timeStamp=0;
    this->distArrival=0;
    current_speed=-1;
    current_angle=0;
    this->debug=0;
    this->foundByNewtonRaphson=false;
}
