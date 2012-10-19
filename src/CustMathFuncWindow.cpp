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

#include "CustMathFuncWindow.h"
#include <QMessageBox>
#include <QDebug>
CustMathFuncWindow::CustMathFuncWindow(QWidget* parent): QWidget(parent)
{
    setupUi(this);

    connect(addButton, SIGNAL(clicked()), this, SLOT(addPart()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deletePart()));
    connect(partList, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(changePart(QListWidgetItem*,QListWidgetItem*)));
    connect(durationSpin, SIGNAL(valueChanged(int)), this, SLOT(changeItemText(int)));

}

double CustMathFuncWindow::GetFrequency() const
{
    double res=0;
    for(int i=0; i<partList->count(); i++)
    {
        res+=partList->item(i)->text().remove(" ms").toDouble();
    }
    return 1000/res;
}

void CustMathFuncWindow::addPart()
{
    QListWidgetItem * newItem = new QListWidgetItem("10 ms",partList);

    paramsWidget->setEnabled(true);
    _items[newItem].first = 10;
    _items[newItem].second = "x";

    partList->setCurrentItem(newItem);

    emit frequencyChanged(GetFrequency());
}
void CustMathFuncWindow::deletePart()
{
    if(partList->currentItem()==NULL) {
        QMessageBox::critical(this, "Erreur", "Aucune partie du motif n'est sélectionnée.");
    } else if(QMessageBox::question(this, "Confirmation", "Voulez-vous vraiment supprimer cette partie du motif ?", QMessageBox::Yes | QMessageBox::No)==QMessageBox::Yes) {
        QListWidgetItem * item = partList->takeItem(partList->currentIndex().row());
        delete item;
        _items.erase(item);

        //On vérifie si qqchose est encore sélectionné
        if(partList->currentItem() == NULL)
            paramsWidget->setEnabled(false);
    }
}

void CustMathFuncWindow::changePart(QListWidgetItem* current,QListWidgetItem* previous)
{
    if(current != previous) {
	//Enregistrement
        _items[previous].first=durationSpin->value();
        _items[previous].second=functionEdit->text().toStdString();
	//Chargement
        durationSpin->setValue(_items[current].first);
        functionEdit->setText(QString::fromStdString(_items[current].second));
    }
}

void CustMathFuncWindow::changeItemText(int duration)
{
    if(partList->currentItem()) {
        partList->currentItem()->setText(QString::number(duration) + " ms");

        // _items[partList->currentItem()].first=durationSpin->value();

        emit frequencyChanged(GetFrequency());
    }
}

PeriodicFunction CustMathFuncWindow::GetFunction()
{   if(partList->currentItem()) {
        _items[partList->currentItem()].first=durationSpin->value();
        _items[partList->currentItem()].second=functionEdit->text().toStdString();
    }
    PeriodicFunction func;
    std::map<QListWidgetItem*, std::pair<int,std::string> >::const_iterator it = _items.begin();
    while(it != _items.end()) {
      func.AddPart(it->second.first, it->second.second);
      it++;
    }
    return func;
}



CustMathFuncWindow::~CustMathFuncWindow()
{

}

