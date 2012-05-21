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


#include "functiondelegate.h"

#include <QtGui/QPainter>
#include <QtGui/QTreeView>


#include "3rdparty/QtSolutions/QtMmlWidget/QtMmlDocument"
#include "functionsmodel.h"
#include "analitza/expression.h"

namespace Keomath
{


FunctionDelegate::FunctionDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

void FunctionDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

QSize FunctionDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)

    return QSize(128, 32);
}

void FunctionDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    
    if (index.column() == 1)
    {
        QTreeView *view = qobject_cast<QTreeView*>(parent());

        FunctionsFilterProxyModel *proxy = qobject_cast<FunctionsFilterProxyModel*>(view->model());


        FunctionsModel *model = qobject_cast<FunctionsModel*>(proxy->sourceModel());

        QString mathML = model->editFunction(proxy->mapToSource(index).row())->lambda().toMathMLPresentation();

        QtMmlDocument mathMLRenderer;
        mathMLRenderer.setBaseFontPointSize(12);
        mathMLRenderer.setContent(mathML);

        QRect contentRect(view->visualRect(index).left(), view->visualRect(index).top(),
                          mathMLRenderer.size().width(), mathMLRenderer.size().height());

        if (contentRect.width() > 0)
        {
            painter->save();

            QRect cellRect = view->visualRect(index);

            quint16 margin = 4;

            contentRect.translate(cellRect.center() - contentRect.center());
            contentRect.setLeft(margin + cellRect.left());

            if (view->selectionModel()->isSelected(index))
            {
                painter->save();

                
                
                
                
                
                

                painter->setPen(option.palette.highlight().color());

                painter->setBrush(option.palette.highlight());

                QRect selRect(cellRect);
                selRect.setHeight(selRect.height() - 1); 

                painter->drawRect(selRect);
                painter->restore();

                painter->setPen(option.palette.highlightedText().color());
            }

            if (mathMLRenderer.size().width() + margin < cellRect.width())
                mathMLRenderer.paint(painter, contentRect.topLeft());
            else
            {






                QItemDelegate::paint(painter, option, index);

            }

            painter->restore();
        }
        else
            QItemDelegate::paint(painter, option, index);
    }
    else
        QItemDelegate::paint(painter, option, index);






































































}

} 
