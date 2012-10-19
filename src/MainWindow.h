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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "qwt/qwt_plot_curve.h"
#include "ui_MainWindow.h"
#include "GraphData.h"
#include "CustMathFuncWindow.h"


class MainWindow : public QMainWindow, private Ui::MainWindow
{
Q_OBJECT
public:
    MainWindow();
    virtual ~MainWindow();
    
public slots:
    void setRectSig();
    void setTriSig();
    void setSinSig();
    void generate();
    void updateCurvesVisibility();
    void changeConnectionState(bool connec);
    void sendToOrphy();
    void changePort();
    void actualiserPorts();
    
    void onFrequencyChanged(double freq);
    void onOrphyPointsChanged(int points);
    void onOrphyFrequencyChanged(int freq);

    double GetFrequency() const;
    
    void updateOrphyFrequency();
    
protected:
    void closeEvent ( QCloseEvent * event );
    
private:
    QwtPlotCurve * _curve;
    QwtPlotCurve * _canCurve;
    QwtPlot * plot;
    GraphData * _data;
    GraphData * _canData;
    CustMathFuncWindow * _custMathWin;
    
    bool _dontEmitSignal;
};

#endif // MAINWINDOW_H
