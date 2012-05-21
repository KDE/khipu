/*************************************************************************************
 *  Copyright (C) 2007-2009 by Aleix Pol <aleixpol@kde.org>                          *
 *  Copyright (C) 2010 by Percy Camilo T. Aucahuasi <percy.camilo.ta@gmail.com>      *
 *                                                                                   *
 *  This program is free software; you can redistribute it and/or                    *
 *  modify it under the terms of the GNU General Public License                      *
 *  as published by the Free Software Foundation; either version 2                   *
 *  of the License, or (at your option) any later version.                           *
 *                                                                                   *
 *  This program is distributed in the hope that it will be useful,                  *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of                   *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                    *
 *  GNU General Public License for more details.                                     *
 *                                                                                   *
 *  You should have received a copy of the GNU General Public License                *
 *  along with this program; if not, write to the Free Software                      *
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA   *
 *************************************************************************************/


#include "solver.h"
#include <QRectF>
namespace Keomath
{



Solver::Solver(const Analitza::Expression &expression, Analitza::Variables *variables)
    : m_evaluator(variables)
    , m_resolution(50)
{
    m_evaluator.setExpression(expression);
    m_evaluator.simplify(); 
    m_evaluator.flushErrors();
}

Solver::Solver(const Solver &solver)
    : m_evaluator(solver.m_evaluator.variables())
    , m_domain(solver.m_domain)
    , m_resolution(solver.m_resolution)
{
    m_evaluator.setExpression(solver.lambda());
}

Solver::~Solver()
{
}





Solver2D::Solver2D(const Analitza::Expression &expression, Analitza::Variables *variables)
    : Solver(expression, variables)
{
}

Solver2D::Solver2D(const Solver2D &solver2d)
    : Solver(solver2d)
{
}

Solver2D::~Solver2D()
{
}

void Solver2D::setResolution(int resolution)
{
    





    if (resolution != m_resolution)
    {
        
        
        m_paths.clear();
    }

    m_resolution = resolution;


    

}

void Solver2D::buildPaths(const QRectF &viewport, const QList<QPointF> &points)
{
    
    m_paths.clear();

    








    
    
    
    


    if (points.size() <= 2)
    {
        for (int i = 1; i < points.size(); i++)
        {
            if (!std::isnan(points.at(i).x()) && !std::isnan(points.at(i).y()) &&
                    !std::isnan(points.at(i-1).x()) && !std::isnan(points.at(i-1).y()))
                m_paths.append(QLineF(points.at(i-1), points.at(i)));

        }
    }
    else
    {
        for (int i = 1; i < points.size(); i++)
        {
            
            if (!std::isnan(points.at(i).x()) && !std::isnan(points.at(i).y()) &&
                    !std::isnan(points.at(i-1).x()) && !std::isnan(points.at(i-1).y()))
            {


                
                

                
                

                m_paths.append(QLineF(points.at(i-1), points.at(i)));


            }
        }
    }

    if (m_paths.empty())
    {
        m_errors << "No se puede dibujar esta curva";

        
        return ;
    }
}






























Solver3D::Solver3D(const Analitza::Expression &expression, Analitza::Variables *variables)
    : Solver(expression, variables)
{
}

Solver3D::Solver3D(const Solver3D &solver3d)
    : Solver(solver3d)
{
}

Solver3D::~Solver3D()
{
}


} 
