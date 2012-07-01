/*************************************************************************************
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


#include "button.h"

#include <QtGui/QStyleOptionGraphicsItem>
#include <QtGui/QPainter>

#include <QtGui/QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <QMenu>
#include <QEvent>

namespace Keomath
{

Button::Button(QWidget *parent)
    : QToolButton(parent)
{
    setMouseTracking(true);
    setIconSize(QSize(48, 48));
    setMinimumSize(48, 48);
    setMaximumSize(48, 48);
    setCursor(Qt::PointingHandCursor);
}

void Button::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing |QPainter::SmoothPixmapTransform);

    bool down = isDown();

    QRect r = QRect(QPoint(0,0), iconSize());

    QLinearGradient grad(r.topLeft(), r.bottomRight());

    grad.setColorAt(down ? 1 : 0, m_onHover ? Qt::white : Qt::lightGray);
    grad.setColorAt(down ? 0 : 1, Qt::darkGray);

    painter.setPen(Qt::NoPen);
    painter.setBrush(grad);
    painter.drawEllipse(r);

    grad.setColorAt(down ? 1 : 0, Qt::darkGray);
    grad.setColorAt(down ? 0 : 1, Qt::lightGray);

    painter.setBrush(grad);

    if (down)
        painter.translate(2, 2);

    painter.drawEllipse(r.adjusted(5, 5, -5, -5));
    painter.drawPixmap(0, 0, icon().pixmap(iconSize()));
    painter.drawText(r, Qt::AlignCenter, text());
}

void Button::enterEvent (QEvent *event)
{
    emit mouseHovered();

    m_onHover = true;
    update();
    QAbstractButton::enterEvent(event);
}

void Button::leaveEvent (QEvent *event)
{
    emit mouseLeft();

    m_onHover = false;
    update();
    QAbstractButton::leaveEvent(event);
}

} 
