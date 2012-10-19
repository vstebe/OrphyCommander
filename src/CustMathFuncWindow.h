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

#ifndef CUSTMATHFUNCWINDOW_H
#define CUSTMATHFUNCWINDOW_H

#include "ui_CustMathFuncWindow.h"
#include "PeriodicFunction.h"


class CustMathFuncWindow : public QWidget, private Ui::CustMathFuncWindow
{
Q_OBJECT
public:
    CustMathFuncWindow(QWidget* parent = 0);
    
    PeriodicFunction GetFunction();

    double GetFrequency() const;
    
    virtual ~CustMathFuncWindow();
    
public slots:
    void addPart();
    void deletePart();
    void changePart(QListWidgetItem* current, QListWidgetItem * previous);
    void changeItemText(int duration);

signals:
    void frequencyChanged(double freq);
    
private:
    std::map<QListWidgetItem*, std::pair<int,std::string> > _items;
};

#endif // CUSTMATHFUNCWINDOW_H
