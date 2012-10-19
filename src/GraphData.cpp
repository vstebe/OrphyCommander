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

#include "GraphData.h"
#include "OrphyCommander.h"
#include <math.h>
#include <QScriptEngine>

int GraphData::_orphyFrequency = 2000;
int GraphData::_orphyPoints = 2000;



double GraphData::y(double x) const
{
    if(_perfectMode)
        return yPerfect(x);
    else {
        double y = yPerfect(x);
        if(y>5)
            return 5;
        if(y<0)
            return 0;



        double floorx = floor(x*_orphyFrequency)/_orphyFrequency;


        if(x==floorx)
            return 0.019*round(y/0.019);
        else {
            double y = yPerfect(floorx);
            if(y>5)
                return 5;
            if(y<0)
                return 0;
            return y;
        }


    }
}

double GraphData::yPerfect(double x) const
{
    if(x<0)
        return 0;
    if(_type==RECT) {
        if(x<_params.rcy/_params.frequency)
            return _params.highVoltage;

        if(x<double(1)/_params.frequency)
            return _params.lowVoltage;

        int b = double(x)*_params.frequency;
        return yPerfect(x-double(b)/_params.frequency);


    } else if(_type == TRI) {
        if(x<1/(2*_params.frequency))
            return 2*(_params.highVoltage-_params.lowVoltage)*_params.frequency*x+_params.lowVoltage;
        if(x<1/_params.frequency)
            return -2*(_params.highVoltage-_params.lowVoltage)*_params.frequency*x+2*(_params.highVoltage+_params.lowVoltage);

        int b = x*_params.frequency;
        return yPerfect(x-b/_params.frequency);


    } else if(_type==SIN) {
        return _params.amplitude*sin(x*2*3.14*_params.frequency)+_params.decalage;

    } else if(_type==SIMPLE_FUNC) {
        QScriptEngine engine;
        QScriptValue func = engine.evaluate(QString("(function(x) { return ")+_simpleFunction+";})");
        QScriptValueList args;
        args << x;
        return func.call(QScriptValue(), args).toNumber();

    } else if(_type==PERIODIC_FUNC) {
        if(x>double(_periodicFunctionPeriod)/1000)
            return yPerfect(x-int(1000*x/_periodicFunctionPeriod)*(double(_periodicFunctionPeriod)/1000));

        double t=0;
        for(int i=0; i<_periodicFunction.NbParts(); i++) {
            if(x<double(_periodicFunction.GetDuration(i))/1000+t) {
                QScriptEngine engine;
                QScriptValue func = engine.evaluate(QString("(function(x) { return ")+QString::fromStdString(_periodicFunction.GetFunction(i))+";})");
                QScriptValueList args;
                args << x;
                return func.call(QScriptValue(), args).toNumber();
            }
            t+=double(_periodicFunction.GetDuration(i))/1000;
        }

        return 0;

    } else {
        return 0;
    }
}

void GraphData::SetOrphyFrequency(int freq)
{
    _orphyFrequency = freq;
}
void GraphData::SetOrphyPoints(int points)
{
    _orphyPoints=points;
}


GraphData::GraphData() : QwtSyntheticPointData(300)
{
//       _data=data;
}

void GraphData::setParams(const Params& params)
{
    _params=params;
}
void GraphData::setType(GraphData::Type type)
{
    _type=type;
}
void GraphData::setPerfectMode(bool mode)
{
    _perfectMode=mode;
}
void GraphData::setSimpleFunction(const QString& function)
{
    _simpleFunction=function;
}

void GraphData::setPeriodicFunction(const PeriodicFunction& func)
{
    _periodicFunction=func;
    _periodicFunctionPeriod = func.GetPeriodMs();
}

void GraphData::sendToOrphy()
{
    double frequency;
    if(_type==RECT || _type==SIN || _type==TRI)
        frequency=_params.frequency;
    else
        frequency=1000/_periodicFunctionPeriod;

    int periodEch = 1000000/_orphyFrequency;
    std::vector<char> points;
    for(double x =0; (x<double(1)/frequency && points.size()<2000); x+=double(1)/_orphyFrequency) {
        points.push_back(char((y(x)*255)/5));
    }

    OrphyCommander::Get()->SendSignal(points, periodEch);
}
