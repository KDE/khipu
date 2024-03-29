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

//Qt includes
#include <QWidget>
#include <QStackedWidget>
#include <QStyledItemDelegate>
#include <QListView>
#include <QLineEdit>

//KDE includes
#include <KWidgetItemDelegate>

class QSortFilterProxyModel;
class QItemSelection;
class QResizeEvent;
class QFocusEvent;
class QToolButton;

class SpaceItem;
class DataStore;
class SpacesView;

class LineEdit : public QLineEdit
{
    Q_OBJECT
    
public :
    explicit LineEdit(QWidget* parent = nullptr);
    
signals:
    void editingFinished(const QString &newtext);
    
private slots:
    void procsSditingFinished();
    
};

/// This delegate only works if the view is a SpacesView (wich is a listview) and the viewmode is IconMode
class SpacesDelegate : public QStyledItemDelegate
{
    Q_OBJECT

friend class SpacesView;

public:
    static const int FrameThickness = 5;
    static const int ItemMargin = 4;

    explicit SpacesDelegate(SpacesView *itemView, QObject *parent = nullptr);
    ~SpacesDelegate() override;

public slots:
    //NOTE ejecutar este metodo cuando se a cambiado de filtro en el proxy ... es decir ejecutarlo desde afuera de esta clase
    void filterEvent(); // se supone que el proxy emite esta signal layoutchanged y este slots debe ocultar los botnes y editores
    void setCurrentSpace(const QModelIndex& index);

public:
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setDocument(DataStore *doc);
    
    /// call this when you want to update the delegate state, by rebind a new model
    void hideOperatorBar();
    
signals:
    void showSpace(const QModelIndex &index);
    void saveDictionary(const QModelIndex &index);
    
private:
    QListView *m_itemView;
    bool eventFilter(QObject *watched, QEvent *event) override;
    void setupOperationBar();
    void updateOperationBarPos(const QModelIndex &idx);

private slots:
    void editCurrentSpace();
    void removeCurrentSpace();
    void finishEditingTitle(const QString &newtitle = QString()); // save current index data
    void invalidClick(const QModelIndex &index);
    void exportSpace();
    
private:
    DataStore *m_document;
    mutable bool m_isEditing;
    QWidget *m_operationBar;
    LineEdit *m_titleEditor;

    mutable QModelIndex m_currentEditingIndex; // current editing index
    QPoint m_currentCurPos;
};

class SpacesView : public QListView
{
Q_OBJECT

public:
    explicit SpacesView(QWidget* parent = nullptr);
    
protected:
    void resizeEvent(QResizeEvent* e) override;
};



#endif
