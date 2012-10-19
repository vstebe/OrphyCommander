/*
Copyright (C) 2012 Stébé Vincent

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.*/

#ifndef GRAPHDATA_H
#define GRAPHDATA_H

#include <qwt/qwt_series_data.h>
#include "PeriodicFunction.h"


struct Params {
    double rcy;
    double highVoltage;
    double lowVoltage;
    double frequency;
    double amplitude;
    double decalage;
};


class GraphData : public QwtSyntheticPointData
{

public:
    enum Type {RECT, TRI, SIN, SIMPLE_FUNC, PERIODIC_FUNC, OTHER};


    virtual double y(double x) const;
    double yPerfect(double x) const;

    void setParams(const Params& params);
    void setType(Type type);
    void setPerfectMode(bool mode);
    void setSimpleFunction(const QString& function);
    void setPeriodicFunction(const PeriodicFunction& func);
    void sendToOrphy();
    
    static void SetOrphyFrequency(int freq);
    static void SetOrphyPoints(int points);

    GraphData();
    
private:
    std::vector< double > _data;
    Type _type;
    Params _params;
    bool _perfectMode;
    QString _simpleFunction;
    PeriodicFunction _periodicFunction;
    int _periodicFunctionPeriod;
    
    static int _orphyFrequency;
    static int _orphyPoints;
    
};

#endif // GRAPHDATA_H
