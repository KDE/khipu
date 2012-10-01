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


#ifndef KEOMATH_FUNCTIONLIBRARYEDIT_H
#define KEOMATH_FUNCTIONLIBRARYEDIT_H

#include <KLineEdit>
#include <QLabel>
#include <QTreeView>
#include <analitzaplot/plottingenums.h>

class QKeyEvent;
namespace Analitza { class PlotsDictionaryModel; }

class FunctionLibraryEdit : public KLineEdit
{
    Q_OBJECT

public:
    explicit FunctionLibraryEdit(QWidget *parent = 0);
    ~FunctionLibraryEdit();

    void setFilterArguments(const QStringList args);
    void setFilterDimension(Analitza::Dimensions dim);

    void setModel(Analitza::PlotsDictionaryModel* model);

signals:
    void selectedFunction(const QString & newExp, Analitza::Dimensions dimension, const QString &name, const QStringList &bvars);

protected:
    bool eventFilter(QObject *o, QEvent *e);

private slots:
    void emitSelFunction(const QModelIndex &index);
    void setFilterText(const QString &text);

private:
    void showPopup(const QRect& rect = QRect());

private:
    Analitza::PlotsDictionaryModel *m_proxyModel;

    QTreeView *m_functionLibraryView;

    bool eatFocusOut;
    bool m_clearText;
};

#endif
