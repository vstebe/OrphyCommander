/*
Copyright (C) 2012 St�b� Vincent

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

#include "MainWindow.h"
#include "GraphData.h"
#include "Graph.h"
#include "OrphyCommander.h"
#include "config.h"
#include <qextserialenumerator.h>
#include <QMessageBox>
#include <QtScript>
#include <QScriptEngine>
MainWindow::MainWindow() : QMainWindow()
{
    setupUi(this);

    connect(rectSigButton, SIGNAL(clicked()), this, SLOT(setRectSig()));
    connect(triSigButton, SIGNAL(clicked()), this, SLOT(setTriSig()));
    connect(sinSigButton, SIGNAL(clicked()), this, SLOT(setSinSig()));
    connect(generateButton, SIGNAL(clicked()), this, SLOT(generate()));
    connect(previewCanCheckBox, SIGNAL(clicked(bool)), this, SLOT(updateCurvesVisibility()));
    connect(perfectSigCheckBox, SIGNAL(clicked(bool)), this, SLOT(updateCurvesVisibility()));
    connect(sendToOrphyButton, SIGNAL(clicked()), this, SLOT(sendToOrphy()));
    connect(changePortButton, SIGNAL(clicked()), this, SLOT(changePort()));
    connect(freqSpin, SIGNAL(valueChanged(double)), this, SLOT(onFrequencyChanged(double)));
    connect(orphyFrequency, SIGNAL(valueChanged(int)), this, SLOT(onOrphyFrequencyChanged(int)));
    connect(orphyPoints, SIGNAL(valueChanged(int)), this, SLOT(onOrphyPointsChanged(int)));
    
    connect(orphyFrequency, SIGNAL(focusOut()), this, SLOT(updateOrphyFrequency()));

    connect(OrphyCommander::Get(), SIGNAL(connectionStateChanged(bool)), this, SLOT(changeConnectionState(bool)));



    connectionOkWidget->setVisible(false);

    sinParams->setVisible(false);
    triParams->setVisible(false);


    plot = new Graph(this);
    plotContainer->addWidget(plot);
    _curve = new QwtPlotCurve;
    _curve->attach(plot);
    _curve->setRenderHint( QwtPlotItem::RenderAntialiased );

    _canCurve = new QwtPlotCurve;
    _canCurve->attach(plot);
    _canCurve->setRenderHint( QwtPlotItem::RenderAntialiased );
    _canCurve->setPen(QPen(Qt::red));

    plot->setAxisTitle(QwtPlot::xBottom,QString("t (s)"));
    plot->setAxisTitle(QwtPlot::yLeft,QString("U (V)"));

    _data = new GraphData;
    _data->setPerfectMode(true);

    _canData = new GraphData;
    _canData->setPerfectMode(false);

    _custMathWin = new CustMathFuncWindow(this);
    custMathFuncWindowContainer->addWidget(_custMathWin);

    connect(_custMathWin, SIGNAL(frequencyChanged(double)), this, SLOT(onFrequencyChanged(double)));

    connect(actualiserButton, SIGNAL(clicked()), this, SLOT(actualiserPorts()));

    _dontEmitSignal=false;
    
    orphyFrequency->setMinimum(ORPHY_MIN_FREQUENCY+1); //+1 sinon tronqué - pas bon
    orphyPoints->setMinimum(ORPHY_MIN_POINTS+1);
    
    actualiserPorts();

#ifdef DEBUG_MODE
    sendToOrphyButton->setEnabled(true);
#endif
}

MainWindow::~MainWindow()
{
    delete _curve;
    delete plot;
    delete _custMathWin;
}

void MainWindow::actualiserPorts()
{
    QList<QextPortInfo> portsInfo = QextSerialEnumerator::getPorts();
    portsComboBox->clear();
    for(QList<QextPortInfo>::const_iterator port = portsInfo.begin(); port != portsInfo.end(); port++) {
        portsComboBox->addItem(port->portName);
    }
}

void MainWindow::setRectSig()
{
    triSigButton->setChecked(false);
    sinSigButton->setChecked(false);
    rectParams->setVisible(true);
    sinParams->setVisible(false);
    triParams->setVisible(false);
}
void MainWindow::setSinSig()
{
    triSigButton->setChecked(false);
    rectSigButton->setChecked(false);
    rectParams->setVisible(false);
    sinParams->setVisible(true);
    triParams->setVisible(false);
}
void MainWindow::setTriSig()
{
    sinSigButton->setChecked(false);
    rectSigButton->setChecked(false);
    rectParams->setVisible(false);
    sinParams->setVisible(false);
    triParams->setVisible(true);
}

void MainWindow::updateCurvesVisibility()
{
    _curve->setVisible(perfectSigCheckBox->isChecked());
    _canCurve->setVisible(previewCanCheckBox->isChecked());
    plot->replot();
}

void MainWindow::changePort()
{
    if(portsComboBox->currentIndex()!=-1 && QextSerialEnumerator::getPorts().count()>portsComboBox->currentIndex())
        OrphyCommander::Get()->SetPort(QextSerialEnumerator::getPorts()[portsComboBox->currentIndex()].portName);
    else
        actualiserPorts();
}

double MainWindow::GetFrequency() const
{
    if(tabWidget->indexOf(tabWidget->currentWidget())==0) {
        return freqSpin->value();
    } else {
        return _custMathWin->GetFunction().GetFrequency();
    }
}

void MainWindow::generate()
{
    if(tabWidget->indexOf(tabWidget->currentWidget())==0) {
        if(rectSigButton->isChecked()) {
            Params params;
            params.frequency = freqSpin->value();
            params.highVoltage = highVoltageSpin->value();
            params.lowVoltage = lowVoltageSpin->value();
            params.rcy = rcySpin->value();

            if(params.highVoltage <= params.lowVoltage) {
                QMessageBox::critical(this, "Erreur", "La tension �  l'état haut ne peut pas être inférieure ou égale �  la tension �  l'état bas.");
            } else {
                _data->setParams(params);
                _data->setType(GraphData::RECT);
                _canData->setParams(params);
                _canData->setType(GraphData::RECT);
                _canCurve->setData(_canData);
                _curve->setData(_data);
            }

        } else if(triSigButton->isChecked()) {
            Params params;
            params.frequency = freqSpin->value();
            params.highVoltage = highVoltageTriSpin->value();
            params.lowVoltage = lowVoltageTriSpin->value();

            if(params.highVoltage <= params.lowVoltage) {
                QMessageBox::critical(this, "Erreur", "La tension maximale ne peut pas être inférieure ou égale �  la tension minimale.");
            } else {
                _data->setParams(params);
                _data->setType(GraphData::TRI);
                _canData->setParams(params);
                _canData->setType(GraphData::TRI);
                _canCurve->setData(_canData);
                _curve->setData(_data);
            }
        } else if(sinSigButton->isChecked()) {
            Params params;
            params.frequency = freqSpin->value();
            params.amplitude = amplitudeSpin->value();
            params.decalage = decalageSpin->value();
            _data->setParams(params);
            _data->setType(GraphData::SIN);
            _canData->setParams(params);
            _canData->setType(GraphData::SIN);
            _canCurve->setData(_canData);
            _curve->setData(_data);
        }
        plot->setAxisScale(QwtPlot::xBottom, 0, 3/freqSpin->value());
    } else if(tabWidget->indexOf(tabWidget->currentWidget())==1) {
        if(QScriptEngine::checkSyntax (simpleFunctionEdit->text() ).state() != QScriptSyntaxCheckResult::Valid) {
            QMessageBox::warning(this,"Erreur de syntaxe","Veuillez vérifier la syntaxe de l'expression mathématique.");
        } else {
            _data->setType(GraphData::SIMPLE_FUNC);
            _data->setSimpleFunction(simpleFunctionEdit->text());
            _canData->setSimpleFunction(simpleFunctionEdit->text());
            _canData->setType(GraphData::SIMPLE_FUNC);
            _canCurve->setData(_canData);
            _curve->setData(_data);
        }
    } else {
        _data->setType(GraphData::PERIODIC_FUNC);
        PeriodicFunction func = _custMathWin->GetFunction();
        _data->setPeriodicFunction(func);
        _canData->setPeriodicFunction(func);
        _canData->setType(GraphData::PERIODIC_FUNC);
        _canCurve->setData(_canData);
        _curve->setData(_data);
        if(func.GetFrequency()!=0)
            plot->setAxisScale(QwtPlot::xBottom, 0, 3/(func.GetFrequency()));
    }

    plot->replot();
}

void MainWindow::changeConnectionState(bool connec)
{
    if(connec) {
        connectionOkWidget->setVisible(true);
        connectionNonOkWidget->setVisible(false);
        infoOrphyLabel->setText(OrphyCommander::Get()->GetVersionInfo());
        sendToOrphyButton->setEnabled(true);
    } else {
        connectionNonOkWidget->setVisible(true);
        connectionOkWidget->setVisible(false);
#ifndef DEBUG_MODE
        sendToOrphyButton->setEnabled(false);
#endif
    }
}

void MainWindow::sendToOrphy()
{
    generate();

    if(tabWidget->indexOf(tabWidget->currentWidget())==1)
        QMessageBox::critical(this, QString::fromUtf8("Non géré"), QString::fromUtf8("Cette fonctionnalité n'est pas encore disponible."));
    else
        _canData->sendToOrphy();
}

void MainWindow::onFrequencyChanged(double freq)
{
    if(!_dontEmitSignal) {
        _dontEmitSignal=true;
        double orphyFreq = freq*orphyPoints->value();

        if(orphyFreq >= ORPHY_MAX_FREQUENCY) {
            orphyPoints->setValue(ORPHY_MAX_FREQUENCY/freq);
            orphyFrequency->setValue(freq*orphyPoints->value());

        } else if(orphyFreq <= ORPHY_MIN_FREQUENCY) {
            orphyPoints->setValue(ORPHY_MIN_FREQUENCY/freq);
            orphyFrequency->setValue(freq*orphyPoints->value());

        } else {
            orphyFrequency->setValue(orphyFreq);
        }
        GraphData::SetOrphyFrequency(orphyFrequency->value());
	
        orphyPoints->setMaximum(ORPHY_MAX_FREQUENCY/freq);
        orphyPoints->setMinimum(ORPHY_MIN_FREQUENCY/freq);
	if(orphyPoints->maximum()>ORPHY_MAX_POINTS)
	  orphyPoints->setMaximum(ORPHY_MAX_POINTS);
	
	orphyFrequency->setMaximum(freq*orphyPoints->maximum());
	
	
        _dontEmitSignal=false;
    }
}
void MainWindow::onOrphyFrequencyChanged(int freq)
{
    if(!_dontEmitSignal) {
        _dontEmitSignal=true;
        GraphData::SetOrphyFrequency(freq);
        orphyPoints->setValue(double(freq)/GetFrequency());

        _dontEmitSignal=false;
    }
}
void MainWindow::onOrphyPointsChanged(int points)
{   if(!_dontEmitSignal) {
        _dontEmitSignal=true;
        
        orphyFrequency->setValue(GetFrequency()*points);
	GraphData::SetOrphyFrequency(orphyFrequency->value());
        _dontEmitSignal=false;
    }
}

void MainWindow::updateOrphyFrequency()
{
    orphyFrequency->setValue(GetFrequency()*orphyPoints->value());
    GraphData::SetOrphyFrequency(orphyFrequency->value());
}
void MainWindow::closeEvent(QCloseEvent* event)
{
    OrphyCommander::Get()->Stop();
    if(OrphyCommander::Get()->wait(2500))
        QWidget::closeEvent(event);
}
