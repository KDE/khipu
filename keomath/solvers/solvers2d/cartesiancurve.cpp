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


#include "../solver.h"
#include "../solverfactory.h"
#include <QRectF>
#include "analitza/value.h"
#include "analitza/variable.h"

namespace Keomath
{




class KEOMATH_EXPORT CartesianCurveY : public Solver2D
{
public:
    explicit CartesianCurveY(const Analitza::Expression &expression, Analitza::Variables *variables);
    CartesianCurveY(const CartesianCurveY &cartesianCurveY);
    virtual ~CartesianCurveY();

    static QStringList supportedBVars()
    {
        return QStringList("x");
    }
    static Analitza::ExpressionType expectedType()
    {
        return Analitza::ExpressionType(Analitza::ExpressionType::Lambda).addParameter(
                   Analitza::ExpressionType(Analitza::ExpressionType::Value)).addParameter(
                   Analitza::ExpressionType(Analitza::ExpressionType::Value));
    }
    static QStringList examples()
    {
        return QStringList("x->root(x, 2)-5");
    }

    QStringList arguments() const
    {
        return supportedBVars();
    }
    Function::Axe axeType() const
    {
        return Function::Cartesian;
    }
    void solve(const RealInterval::List &spaceBounds = RealInterval::List());
    Solver * copy()
    {
        return new CartesianCurveY(*this);
    }

    QPair<QPointF, QString> calc(const QPointF &dp);
    QLineF derivative(const QPointF &point);

    bool allDisconnected() const
    {
        return false;
    }


protected:
    Analitza::Cn* m_arg; 
    Analitza::Expression m_deriv;
};

CartesianCurveY::CartesianCurveY(const Analitza::Expression &expression, Analitza::Variables *variables)
    : Solver2D(expression, variables)
    , m_arg(new Analitza::Cn)
{
    m_runStack.append(m_arg);
    m_evaluator.setStack(m_runStack);
    m_type2D = 1;



    {


        if (m_evaluator.isCorrect())
        {

            m_deriv = m_evaluator.derivative(arguments().first());

            m_evaluator.flushErrors();

        }
    }
}

CartesianCurveY::CartesianCurveY(const CartesianCurveY &cartesianCurveY)
    : Solver2D(cartesianCurveY)
    , m_arg(new Analitza::Cn)

{
    m_runStack.append(m_arg);
    m_evaluator.setStack(m_runStack);
    m_type2D = 1;


    m_deriv = cartesianCurveY.m_deriv;
    m_evaluator.flushErrors();

}

CartesianCurveY::~CartesianCurveY()
{
    delete m_arg;
}

void CartesianCurveY::solve(const RealInterval::List &spaceBounds)
{

























    double l_lim=m_domain.at(0).lower();
    double r_lim=m_domain.at(0).upper();



    QRectF viewport;
    viewport.setLeft(spaceBounds[0].lower());
    viewport.setRight(spaceBounds[0].upper());
    viewport.setBottom(spaceBounds[1].lower());
    viewport.setTop(spaceBounds[1].upper());


    QList<QPointF> points;

    qreal image = 0.;


    for (qreal domain = l_lim; domain < r_lim; domain += 0.01)
    {

        m_arg->setValue(domain);

        image = m_evaluator.calculateLambda().toReal().value();


        points.append(QPointF(domain, image));
    }

    
    buildPaths(viewport, points);

}

QPair<QPointF, QString> CartesianCurveY::calc(const QPointF& p)
{
    QPointF dp=p;
    m_arg->setValue(dp.x());
    Analitza::Expression r=m_evaluator.calculateLambda();

    if(!r.isReal())
        m_errors += i18n("We can only draw Real results.");

    dp.setY(r.toReal().value());
    
    return QPair<QPointF, QString>(dp, QString());



}

QLineF CartesianCurveY::derivative(const QPointF& p)
{
    Analitza::Analyzer a(m_evaluator.variables());
    double ret;


    qDebug() << m_deriv.isCorrect() << m_deriv.error();
    qDebug() << m_deriv.toString();

    
    {
        QVector<Analitza::Object*> vars;
        vars.append(new Analitza::Cn(p.x()));
        a.setExpression(m_evaluator.expression());
        ret=a.derivative(vars);
        qDeleteAll(vars);
    }

    return slopeToLine(ret);

}

REGISTER_FUNCTION_2D(CartesianCurveY)






class KEOMATH_EXPORT CartesianCurveX : public CartesianCurveY
{
public:
    explicit CartesianCurveX(const Analitza::Expression &expression, Analitza::Variables *variables);
    CartesianCurveX(const CartesianCurveX &cartesianCurveX);
    virtual ~CartesianCurveX();

    static QStringList supportedBVars()
    {
        return QStringList("y");
    }
    static Analitza::ExpressionType expectedType()
    {
        return Analitza::ExpressionType(Analitza::ExpressionType::Lambda).addParameter(
                   Analitza::ExpressionType(Analitza::ExpressionType::Value)).addParameter(
                   Analitza::ExpressionType(Analitza::ExpressionType::Value));
    }
    static QStringList examples()
    {
        return QStringList("y->y*sin(y)");
    }

    QStringList arguments() const
    {
        return supportedBVars();
    }
    void solve(const RealInterval::List &spaceBounds = RealInterval::List());
    Solver * copy()
    {
        return new CartesianCurveX(*this);
    }

    QPair<QPointF, QString> calc(const QPointF &dp);
    QLineF derivative(const QPointF &point);
};

CartesianCurveX::CartesianCurveX(const Analitza::Expression &expression, Analitza::Variables *variables)
    : CartesianCurveY(expression, variables)
{
}

CartesianCurveX::CartesianCurveX(const CartesianCurveX &cartesianCurveX)
    : CartesianCurveY(cartesianCurveX)
{
}

CartesianCurveX::~CartesianCurveX()
{
}

void CartesianCurveX::solve(const RealInterval::List &spaceBounds)
{


















    double l_lim=m_domain.at(0).lower();
    double r_lim=m_domain.at(0).upper();


    QRectF viewport;
    viewport.setLeft(spaceBounds[0].lower());
    viewport.setRight(spaceBounds[0].upper());
    viewport.setBottom(spaceBounds[1].lower());
    viewport.setTop(spaceBounds[1].upper());


    QList<QPointF> points;

    qreal image = 0.;

    for (qreal domain = l_lim; domain < r_lim; domain += 0.01)
    {
        m_arg->setValue(domain);

        image = m_evaluator.calculateLambda().toReal().value();

        points.append(QPointF(image, domain));
    }

    
    buildPaths(viewport, points);


}

QPair<QPointF, QString> CartesianCurveX::calc(const QPointF& p)
{
    QPointF dp=p;
    m_arg->setValue(dp.y());
    Analitza::Expression r=m_evaluator.calculateLambda();

    if(!r.isReal())
        m_errors += i18n("We can only draw Real results.");

    dp.setX(r.toReal().value());
    
    return QPair<QPointF, QString>(dp, QString());

}

QLineF CartesianCurveX::derivative(const QPointF& p)
{
    QPointF p1(p.y(), p.x());
    QLineF ret=CartesianCurveY::derivative(p1);
    return mirrorXY(ret);


    
}

REGISTER_FUNCTION_2D(CartesianCurveX)




} 

