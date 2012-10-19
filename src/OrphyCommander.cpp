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

#include "OrphyCommander.h"
#include "config.h"

#include <QIODevice>

#include <QDebug>



OrphyCommander::OrphyCommander(QObject* parent): QThread(parent)
{
    _state=TESTING_ORPHY;
    _connected=false;
    _serial = new QextSerialPort(QextSerialPort::Polling);
   //  _serial = new QextSerialPort;
    _serial->setBaudRate(BAUD9600);
    _serial->setParity(PAR_NONE);
    _serial->setStopBits(STOP_1);
    _serial->setRts(false);
    _serial->setTimeout(1);
}

OrphyCommander::~OrphyCommander()
{

}

void OrphyCommander::Write( const char * data)
{
#ifdef DEBUG_MODE
    std::cout << data << std::endl;
#else
    _serial->write(data);
#endif
}


void OrphyCommander::Write( const QByteArray & byteArray , bool showValue)
{
#ifdef DEBUG_MODE
    if(showValue) {
        for(int i=0; i<byteArray.size(); i++)
            std::cout << int((unsigned char)(byteArray.at(i)))  << std::endl;
    } else {
        for(int i=0; i<byteArray.size(); i++)
            std::cout << byteArray.at(i)  << std::endl;
    }
#else
    _serial->write(byteArray);
#endif
}

void OrphyCommander::Start()
{
    _state=TESTING_ORPHY;
    _serial->open(QIODevice::ReadWrite);
    start();
}
void OrphyCommander::SetPort(const QString& port)
{
    _port=port;
    _mutex.lock();
    _serial->close();
    _serial->setPortName(port);
    _serial->open(QIODevice::ReadWrite);
    _mutex.unlock();
}

void OrphyCommander::run()
{


    while(_state!=STOP) {
        if(_state == TESTING_ORPHY) {
#ifndef DEBUG_MODE
            _mutex.lock();


            Write("WVERSION\r");

            _connected=false;
            while(_serial->bytesAvailable()>0) {qDebug() << "tryc";
                QByteArray results = _serial->readAll();
                QString resultsStr(results);
                if(resultsStr.contains("ORPHY GTS2")) {
                    _connected=true;
                    _versionInfo = resultsStr;
                }
            }

            if(_connected)
                emit connectionStateChanged(true);
            else {
                emit connectionStateChanged(false);
            }

            _mutex.unlock();
#endif

            //Attente de 2s en vérifiant l'état du thread
            for(int i=0; i<4; i++) {
                msleep(500);
                if(_state==STOP)
                    break;
            }


        }
    }
}
const QString& OrphyCommander::GetVersionInfo() const
{
    return _versionInfo;
}

bool OrphyCommander::IsConnected() const
{
    return _connected;
}

void OrphyCommander::SendSignal(const std::vector< char >& points, int t)
{
#ifndef DEBUG_MODE
    if(_connected)
#endif
    {
        if(points.size() <= ORPHY_MAX_POINTS && points.size() >= ORPHY_MIN_POINTS && t>= ORPHY_MIN_PERIOD*1000 && t<= ORPHY_MAX_PERIOD*1000) {
            _state=SENDING;
            _mutex.lock();
            Write("WRESET\r");
            sleep(1);
            Write("WTELE ");
            Write(QString::number(points.size()).toAscii(), false);
            Write("\r");


            std::vector< char >::const_iterator it = points.begin();
            while(it != points.end()) {
                QByteArray array;

                array.append(*it);

                Write(array);
                it++;
            }
            Write("WSYNTH 0 ");
            Write(QString::number(t).toAscii(), false);
            Write("\r");

            _mutex.unlock();

            _state=TESTING_ORPHY;
        }
    }
}


OrphyCommander* OrphyCommander::Get()
{
    static OrphyCommander com;
    return &com;
}

void OrphyCommander::Stop()
{
    _connected=false;
    _state=STOP;

}
