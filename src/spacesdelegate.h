/*************************************************************************************
 *  Copyright (C) 2012 by Percy Camilo T. Aucahuasi <percy.camilo.ta@gmail.com>      *
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

#ifndef GPLACS_DASHBOARD_H_DEL
#define GPLACS_DASHBOARD_H_DEL

#include <QtGui/QWidget>
#include <QStackedWidget>

#include <QModelIndex>
#include "plotseditor.h"
#include "document.h"

class QLabel;
class PlotsView3D;
class PlotsView2D;
class QTreeView;
class Document;

class QSortFilterProxyModel;
class QItemSelection;
class DashboardWidget;
class SpacesView;

namespace Ui
{
class DashboardWidget;
}

#include <QtGui/QListView>
#include <KWidgetItemDelegate>
#include <KLineEdit>

class QFocusEvent;
class QToolButton;

class LineEdit : public KLineEdit
{
    Q_OBJECT
    
public :
    LineEdit(QWidget* parent = 0);
    
signals:
    void editingFinished(const QString &newtext);
    
private slots:
    void procsSditingFinished();
    
};

class SpacesDelegate : public KWidgetItemDelegate
{
    Q_OBJECT

public:
    static const int FrameThickness = 5;
    static const int ItemMargin = 4;

    SpacesDelegate(QListView *itemView, QObject *parent = 0);
    ~SpacesDelegate();

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    QList<QWidget*> createItemWidgets() const;
    void updateItemWidgets(const QList<QWidget*> widgets, const QStyleOptionViewItem &option, const QPersistentModelIndex &index) const;
    
    bool eventFilter(QObject *watched, QEvent *event);
    void dummyUpdate(); // force rowsAboutToBeRemoved ... //TODO here is ovio

private slots:
    void setCurrentSpace(const QModelIndex &index);
    void removeCurrentSpace();
    void editCurrentSpace();
    void showCurrentSpace();
    void finishEditingTitle(const QString &newtitle = QString()); // save current index data
    void invalidClick(const QModelIndex &index);

private:
    
    QWidget *m_operationBar;
    LineEdit *m_titleEditor;
    
    QModelIndex m_currentEditingIndex; // current editing index
    bool m_isEditing;
    QPoint m_currentCurPos;

    ///
    bool m_iconMode;
};

#endif
