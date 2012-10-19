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

#ifndef ORPHYCOMMANDER_H
#define ORPHYCOMMANDER_H

#include <qthread.h>
#include <QMutex>
#include <vector>
#include <qextserialport.h>

#include "config.h"

#include <iostream>

class OrphyCommander : public QThread
{
Q_OBJECT
public:
    enum State {SENDING, TESTING_ORPHY, WAITING, STOP};
  
    static OrphyCommander * Get();
    
    void Start();
    void Stop();
    void SetPort(const QString& port);
    
    void SendSignal(const std::vector<char>& points, int t);
    
    bool IsConnected() const;
    const QString& GetVersionInfo() const;

protected:
    virtual void run();
    void Write( const char * data);
    void Write ( const QByteArray & byteArray , bool showValue=true);
signals:
    void connectionStateChanged(bool);
    
private:
    OrphyCommander(QObject* parent = 0);
  
    QString _port;
    bool _connected;
    
    QMutex _mutex;
    
    QextSerialPort * _serial;
    
    QString _versionInfo;
    
    State _state;
    
    virtual ~OrphyCommander();
};

#endif // ORPHYCOMMANDER_H
