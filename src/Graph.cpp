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

#include "Graph.h"
#include <qapplication.h>
#include <qlayout.h>
#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_marker.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_legend.h>
#include <qwt/qwt_series_data.h>
#include <qwt/qwt_plot_canvas.h>
#include <qwt/qwt_plot_panner.h>
#include <qwt/qwt_plot_magnifier.h>
#include <qwt/qwt_text.h>
#include <qwt/qwt_math.h>
Graph::Graph(QWidget* parent): QwtPlot(parent )
{
        // panning with the left mouse button
    ( void ) new QwtPlotPanner( canvas() );

    // zoom in/out with the wheel
    QwtPlotMagnifier * magn = new QwtPlotMagnifier( canvas());
    magn->setAxisEnabled(QwtPlot::yLeft, false);
    
    setAxisScale(QwtPlot::yLeft,-5.5,5.5, 1.0);
}

