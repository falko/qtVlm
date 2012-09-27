/**********************************************************************
qtVlm: Virtual Loup de mer GUI
Copyright (C) 2008 - Christophe Thomas aka Oxygen77

http://qtvlm.sf.net

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

***********************************************************************/

#ifndef VLMLINE_H
#define VLMLINE_H

#include <QGraphicsScene>
#include <QGraphicsWidget>
#include <QPainter>
#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>

#include "class_list.h"
#include "Projection.h"
#include "vlmPoint.h"
#include "GshhsReader.h"


#define VLMLINE_LINE_MODE   0
#define VLMLINE_POINT_MODE  1
#define VLMLINE_GATE_MODE   2

class vlmLine : public QGraphicsWidget
{ Q_OBJECT
    public:
        vlmLine(Projection * proj, QGraphicsScene * myScene,double z_level);

        void addPoint(double lat,double lon);
        void addVlmPoint(vlmPoint point);
        void removeVlmPoint(int index);
        void deleteAll(void);
        void setLinePen ( const QPen & pen ) {linePen = pen;lineWidth=linePen.widthF();update(); }
        QPen getLinePen (){return this->linePen;}
        void setPoly(QList<vlmPoint> & points);

        void setLineMode();
        void setPointMode(QColor pt_color);
        void setGateMode(QString desc);
        void setIceGate(int i){iceGate=i;}
        int  isIceGate(){return iceGate;}
        void setTip(QString tip);
        void setNbVacPerHour(int nbVacPerHour) {this->nbVacPerHour=nbVacPerHour;}
        void setPorteOnePoint(void){this->onePoint=true;}
        void setHidden(bool hidden) {this->hidden=hidden;update();}
        bool getHidden(void){return this->hidden;}
        QList<vlmPoint> * getPoints(){return & this->line;}
        void setSolid(bool solid){this->solid=solid;}

        int count(void) { return line.count(); }
        void setPointDead(int n){this->line[n].isDead=true;}
        void setPointStartCap(int n,double c){this->line[n].startCap=c;}
        void setPointWind(int n, double twd, double tws){this->line[n].wind_angle=twd;this->line[n].wind_speed=tws;}
        void setPointCurrent(int n, double cd, double cs){this->line[n].current_angle=cd;this->line[n].current_speed=cs;}
        void setPointDistIso(int n, double d){this->line[n].distIso=d;}
        void setPointCapVmg(int n, double d){this->line[n].capVmg=d;}
        void setPointcapOrigin(int n,double d){this->line[n].capOrigin=d;}
        void setPointCoordProj(int n,double x,double y){this->line[n].lonProj=x;this->line[n].latProj=y;}
        void setNotSimplificable(int n){this->line[n].notSimplificable=true;}
        void setLastPointIsPoi(){this->line[line.count()-1].isPOI=true;}
        const vlmPoint * getOrigin(int n) {return this->line.at(n).origin;}
        const vlmPoint * getPoint(int n) {return & line.at(n);}
        void setInterpolated(double lon,double lat){this->interpolatedLon=lon;this->interpolatedLat=lat;update();}
        void setHasInterpolated(bool b){this->hasInterpolated=b;update();}
        vlmPoint * getLastPoint() {return & line.last();}
        void setRoundedEnd(bool b){this->roundedEnd=b;}
        void setCoastDetection(bool b){this->coastDetection=b;}
        bool getCoastDetected(){return this->coastDetected;}
        void setMap(GshhsReader * map){this->map=map;}
        ~vlmLine();

    protected:
        void paint(QPainter * pnt, const QStyleOptionGraphicsItem * , QWidget * );
        QRectF boundingRect() const;
        QPainterPath shape() const;
        void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
        void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
//        void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
//        void  mousePressEvent(QGraphicsSceneMouseEvent * e);
//        void  mouseReleaseEvent(QGraphicsSceneMouseEvent * e);
    public slots:
        void slot_showMe(void);
        void slot_shLab(bool state){this->labelHidden=state;update();}
        void slot_startReplay(bool b){this->replayMode=b;slot_showMe();}
        void slot_replay(int i);
    signals:
        void hovered();
        void unHovered();
    private:
        QList<vlmPoint> line;
        QPen linePen;
        QColor pt_color;
        QList<QPolygon*> polyList;
        bool onePoint;
        int mode;
        int iceGate;
        void calculatePoly(void);
        int nbVacPerHour;
        Projection * proj;
        QGraphicsScene * myScene;
        QString desc;
        QRectF boundingR;
        bool hidden;
        bool labelHidden;
        bool solid;
        double interpolatedLon;
        double interpolatedLat;
        bool hasInterpolated;
        QRectF r;
        bool replayMode;
        int replayStep;
        bool roundedEnd;
        bool coastDetection;
        bool coastDetected;
        QList<bool>collision;
        GshhsReader *map;
        double lineWidth;
        QPainterPath myPath;
        double myZvalue;
};


#endif // VLMLINE_H
