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


#include "functionlibraryedit.h"
#include <QDesktopWidget>
#include <QMenu>
#include <QLabel>
#include <QKeyEvent>
#include <QTreeView>
#include <QHeaderView>
#include <QApplication>
#include <QTimer>
#include <QDebug>
#include <KLocale>
#include <QScrollBar>
#include "functionlibrarymodel.h"
#include <analitzaplot/plotsdictionarymodel.h>
#include <QKeyEvent>


FunctionLibraryEdit::FunctionLibraryEdit(QWidget *parent)
    : KLineEdit(parent)
{
    setClearButtonShown(true);


    m_functionLibraryView = new QTreeView(this);
    m_functionLibraryView->setWindowFlags(Qt::Popup);
    m_functionLibraryView->setFocusPolicy(Qt::NoFocus);
    m_functionLibraryView->setFocusProxy(this);
    m_functionLibraryView->installEventFilter(this);
//     m_functionLibraryView->setModel(m_proxyModel);
    m_functionLibraryView->setRootIsDecorated(true);
    m_functionLibraryView->setItemsExpandable(true);
    m_functionLibraryView->setAllColumnsShowFocus(true);
//     m_functionLibraryView->header()->hide();
    m_functionLibraryView->resizeColumnToContents(1);
    m_functionLibraryView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_functionLibraryView->setMinimumWidth(350);


    connect(this, SIGNAL(textEdited(QString)), SLOT(setFilterText(QString)));

    connect(m_functionLibraryView, SIGNAL(clicked(QModelIndex)), SLOT(emitSelFunction(QModelIndex)));

}

FunctionLibraryEdit::~FunctionLibraryEdit()
{
}

void FunctionLibraryEdit::setFilterArguments(const QStringList args)
{
//     m_proxyModel->setFilterArguments(args);
}

void FunctionLibraryEdit::setFilterDimension(Analitza::Dimensions dim)
{
    if (!m_proxyModel) return ;
    //TODO newdic
//     m_proxyModel->setFilterSpaceDimension(dim);
}

void FunctionLibraryEdit::setModel(Analitza::PlotsDictionaryModel* model)
{
    m_proxyModel = model;
    m_functionLibraryView->setModel(m_proxyModel);
//     m_functionLibraryView->setColumnWidth(0,0);
//     m_functionLibraryView->showColumn(0);
//     m_functionLibraryView->hideColumn(1);
}

void FunctionLibraryEdit::emitSelFunction(const QModelIndex &index)
{

    QString indexName = m_proxyModel->index(index.row(), 0).data().toString();
    QString indexLambda = m_proxyModel->index(index.row(), 1).data().toString();
    Analitza::Dimensions indexDimension = Analitza::Dimension(m_proxyModel->index(index.row(), 2).data().toInt());
    QStringList indexArguments = m_proxyModel->index(index.row(), 3).data().toStringList();

    QString finalLambda;

    if (indexArguments.size() == 2)
        finalLambda = "(" + indexArguments.join(",") + ")" + "->" + indexLambda;
    else if (indexArguments.size() == 1)
        finalLambda = indexArguments.at(0) + "->" + indexLambda;

    setText(indexName);

//     qDebug() << "q index prev al quick ? " << index.row() << indexName << finalLambda << indexDimension << indexArguments;

    m_functionLibraryView->hide();

    emit selectedFunction(finalLambda, indexDimension,
                          indexName, indexArguments);

}

void FunctionLibraryEdit::setFilterText(const QString &text)
{
    //TODO new dict infr
//     m_proxyModel->setFilterRegExp(QRegExp(text, Qt::CaseInsensitive, QRegExp::RegExp));


    if (m_proxyModel->rowCount() > 0)
        showPopup();
    else
        m_functionLibraryView->hide();

    setFocus();
}

bool FunctionLibraryEdit::eventFilter(QObject *o, QEvent *e)
{
    if (eatFocusOut && o == this && e->type() == QEvent::FocusOut)
    {
        if (m_functionLibraryView && m_functionLibraryView->isVisible())
            return true;
    }

    if (o != m_functionLibraryView)
        return QObject::eventFilter(o, e);

    switch (e->type())
    {
    case QEvent::KeyPress:
    {
        QKeyEvent *ke = static_cast<QKeyEvent *>(e);

        QModelIndex curIndex = m_functionLibraryView->currentIndex();
        QModelIndexList selList = m_functionLibraryView->selectionModel()->selectedIndexes();

        const int key = ke->key();
     
        switch (key)
        {
        case Qt::Key_End:
        case Qt::Key_Home:
            if (ke->modifiers() & Qt::ControlModifier)
                return false;
            break;

        case Qt::Key_Up:
            if (!curIndex.isValid())
            {
                int rowCount = m_proxyModel->rowCount();
                QModelIndex lastIndex = m_proxyModel->index(rowCount - 1, 0);
                m_functionLibraryView->setCurrentIndex(lastIndex);
                return true;
            }
            else if (curIndex.row() == 0)
            {
                
                
                return true;
            }
            return false;

        case Qt::Key_Down:
            if (!curIndex.isValid())
            {
                QModelIndex firstIndex = m_proxyModel->index(0, 0);
                m_functionLibraryView->setCurrentIndex(firstIndex);
                return true;
            }
            else if (curIndex.row() == m_proxyModel->rowCount() - 1)
            {
                
                
                return true;
            }
            return false;

        case Qt::Key_PageUp:
        case Qt::Key_PageDown:
            return false;
        }

        eatFocusOut = false;
        (static_cast<QObject *>(this))->event(ke);
        eatFocusOut = true;
        if (e->isAccepted() || !m_functionLibraryView->isVisible())
        {
            
            if (!hasFocus())
                m_functionLibraryView->hide();
            if (e->isAccepted())
                return true;
        }

        switch (key)
        {
        case Qt::Key_Return:
        case Qt::Key_Enter:
        case Qt::Key_Tab:

            if (m_functionLibraryView->currentIndex().isValid())
                emitSelFunction(m_functionLibraryView->currentIndex());

            break;

        case Qt::Key_F4:
            if (ke->modifiers() & Qt::AltModifier)
                m_functionLibraryView->hide();
            break;

        case Qt::Key_Backtab:
        case Qt::Key_Escape:
            m_functionLibraryView->hide();
            break;

        default:
            break;
        }

        return true;
    }

    case QEvent::KeyRelease:
    {
        QKeyEvent *ke = static_cast<QKeyEvent *>(e);
        eatFocusOut = false;
        static_cast<QObject *>(this)->event(ke);
        eatFocusOut = true;

        if (text().isEmpty())
            m_functionLibraryView->hide();

        return true;
    }

    case QEvent::MouseButtonPress:
    {
        if (!m_functionLibraryView->underMouse())
        {
            m_functionLibraryView->hide();
            return true;
        }
    }

    case QEvent::InputMethod:
    case QEvent::ShortcutOverride:
        QApplication::sendEvent(this, e);
        break;

    default:
        return false;
    }
    return false;
}


void FunctionLibraryEdit::showPopup(const QRect& rect)
{
    m_functionLibraryView->resizeColumnToContents(1);
    m_functionLibraryView->header()->setResizeMode(0, QHeaderView::ResizeToContents);
    m_functionLibraryView->header()->setResizeMode(1, QHeaderView::ResizeToContents);
    m_functionLibraryView->header()->setResizeMode(2, QHeaderView::ResizeToContents);

//     m_functionLibraryView->showColumn(0);
//     m_functionLibraryView->hideColumn(1);
    
    const QRect screen = QApplication::desktop()->availableGeometry(this);
    Qt::LayoutDirection dir = this->layoutDirection();
    QPoint pos;
    int rh, w;
    int h = (m_functionLibraryView->sizeHintForRow(0) * qMin(20, m_functionLibraryView->model()->rowCount()) + 3) + 3;
    QScrollBar *hsb = m_functionLibraryView->horizontalScrollBar();
    if (hsb && hsb->isVisible())
        h += m_functionLibraryView->horizontalScrollBar()->sizeHint().height();

    if (rect.isValid())
    {
        rh = rect.height();
        w = rect.width();
        pos = this->mapToGlobal(dir == Qt::RightToLeft ? rect.bottomRight() : rect.bottomLeft());
    }
    else
    {
        rh = this->height();
        pos = this->mapToGlobal(QPoint(0, this->height() - 2));
        w = this->width();
    }

    if (w > screen.width())
        w = screen.width();
    if ((pos.x() + w) > (screen.x() + screen.width()))
        pos.setX(screen.x() + screen.width() - w);
    if (pos.x() < screen.x())
        pos.setX(screen.x());

    int top = pos.y() - rh - screen.top() + 2;
    int bottom = screen.bottom() - pos.y();
    h = qMax(h, m_functionLibraryView->minimumHeight());
    if (h > bottom)
    {
        h = qMin(qMax(top, bottom), h);

        if (top > bottom)
            pos.setY(pos.y() - h - rh + 2);
    }

    m_functionLibraryView->setGeometry(pos.x(), pos.y(), w, h);

    if (!m_functionLibraryView->isVisible())
        m_functionLibraryView->show();
}
