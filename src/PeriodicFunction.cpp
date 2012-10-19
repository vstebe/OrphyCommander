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

#include "PeriodicFunction.h"

PeriodicFunction::PeriodicFunction()
{

}

int PeriodicFunction::GetDuration(int i) const
{
    return _parts[i%_parts.size()].first;
}
double PeriodicFunction::GetFrequency() const
{
    return double(1000)/GetPeriodMs();
}
const std::string& PeriodicFunction::GetFunction(int i) const
{
    return _parts[i%_parts.size()].second;
}
int PeriodicFunction::GetPeriodMs() const
{
    int period = 0;
    std::vector<std::pair<int,std::string> >::const_iterator it = _parts.begin();
    while(it != _parts.end()) {
      period += it->first;
      it++;
    }
    return period;
}

void PeriodicFunction::AddPart(int duration, const std::string& function)
{
    _parts.push_back(std::pair<int,std::string>(duration, function));
}

int PeriodicFunction::NbParts() const
{
    return _parts.size();
}



PeriodicFunction::~PeriodicFunction()
{

}

