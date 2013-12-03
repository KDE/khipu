/*************************************************************************************
 *  Copyright (C) 2012-2013 by Percy Camilo T. Aucahuasi <percy.camilo.ta@gmail.com> *
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

#include "spacesdelegate.h"

#include "analitzaplot/plotsdictionarymodel.h"

#include <QPainter>
#include <QPaintEvent>
#include <QToolButton>
#include <QHBoxLayout>
#include <QListView>
#include <QDebug>
#include <KApplication>
#include <KLocale>

//local includes
#include "spacesmodel.h"
#include "datastore.h"
#include "spaceplotsproxymodel.h"
#include "spaceitem.h"

LineEdit::LineEdit(QWidget* parent): KLineEdit(parent)
{
    connect(this, SIGNAL(editingFinished()), SLOT(procsSditingFinished()));
}

void LineEdit::procsSditingFinished()
{
    emit editingFinished(text());
}

SpacesDelegate::SpacesDelegate(SpacesView *itemView, QObject *parent)
    : QStyledItemDelegate(parent)
    , m_itemView(itemView)
    , m_isEditing(false)
    , m_operationBar(0)
    , m_titleEditor(0)
{
    Q_ASSERT(m_itemView->viewMode() == QListView::IconMode);

    itemView->viewport()->installEventFilter(this);

    setupOperationBar();

    connect(this, SIGNAL(showSpace(QModelIndex)), itemView, SIGNAL(doubleClicked(QModelIndex)));

    connect(itemView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(setCurrentSpace(QModelIndex)));
}

SpacesDelegate::~SpacesDelegate()
{
}

void SpacesDelegate::setupOperationBar()
{
    //BEGIN btns

    m_itemView->setEditTriggers(QListView::EditKeyPressed);

    m_operationBar = new QWidget(m_itemView->viewport());
    //     m_operationBar->setFocusPolicy(Qt::NoFocus);

    QToolButton *m_removeButton = new QToolButton(m_operationBar);
    m_removeButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    m_removeButton->setAutoRepeat(false);
    m_removeButton->setToolTip(i18n("Remove"));
    m_removeButton->setIcon(KIcon("list-remove"));
    connect(m_removeButton, SIGNAL(pressed()), SLOT(removeCurrentSpace()));

    QToolButton *m_editButton = new QToolButton(m_operationBar);
    m_editButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    m_editButton->setAutoRepeat(false);
    m_editButton->setToolTip(i18n("Rename"));
    m_editButton->setIcon(KIcon("document-edit"));
    connect(m_editButton, SIGNAL(pressed()), SLOT(editCurrentSpace()));

    QToolButton *m_showButton = new QToolButton(m_operationBar);
    m_showButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    m_showButton->setAutoRepeat(false);
    m_showButton->setToolTip(i18n("Export as dictionary"));
    m_showButton->setIcon(KIcon("document-export"));
    connect(m_showButton, SIGNAL(pressed()), SLOT(exportSpace()));

    QHBoxLayout* layout = new QHBoxLayout(m_operationBar);

    layout->setSpacing(1);
    layout->addWidget(m_removeButton);
    layout->addWidget(m_editButton);
    layout->addWidget(m_showButton);

    m_operationBar->adjustSize();
    m_operationBar->hide();
    //END btns

    m_titleEditor = new LineEdit(m_itemView->viewport());
    m_titleEditor->setClearButtonShown(true);
    m_titleEditor->hide();

    connect(m_titleEditor, SIGNAL(editingFinished(QString)), SLOT(finishEditingTitle(QString)));
}

void SpacesDelegate::exportSpace()
{
    emit saveDictionary(m_itemView->currentIndex());
}

QWidget* SpacesDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem&, const QModelIndex& index) const
{
    m_currentEditingIndex = index;
    m_isEditing = true;
    LineEdit *editor = new LineEdit(parent);

    QRect rect = m_itemView->visualRect(m_itemView->currentIndex());

    editor->resize(rect.width(), m_titleEditor->height());
    editor->move(rect.left()/*+(PreviewWidth-m_operationBar->width())/2*/,rect.top() + rect.height() - m_operationBar->height() - .8*m_titleEditor->height());
    editor->selectAll();
    editor->setFocus();
    editor->setClearButtonShown(true);

    return editor;
}

void SpacesDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    if (!index.isValid())
        return ;

    static_cast<LineEdit*>(editor)->setText(m_itemView->currentIndex().data().toString());
}

void SpacesDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    if (!index.isValid())
        return ;

    m_currentEditingIndex = QModelIndex();
    m_isEditing = false;

    static_cast<SpacesFilterProxyModel*>(model)->sourceModel()->setData(index, static_cast<LineEdit*>(editor)->text());
}

QSize SpacesDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QSize size;

    size.setWidth(PreviewWidth+2*ItemMargin);
    size.setHeight(PreviewHeight+4*option.fontMetrics.height()+2*ItemMargin); // up to 6 lines of text, and two margins
    return size;
}

void SpacesDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid() || !m_itemView->model()) return QStyledItemDelegate::paint(painter, option, index);

    painter->save();

    if (option.state & QStyle::State_Selected)
    {
        //focus
        QStyle *style = QApplication::style();
        style->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, 0);
        //endfocus

        painter->setPen(QPen(option.palette.highlightedText().color()));
    } 
    else 
        painter->setPen(QPen(option.palette.text().color()));

    painter->drawPixmap(option.rect.topLeft()+QPoint(ItemMargin,ItemMargin), index.data(Qt::DecorationRole).value<QPixmap>());

    QString text = painter->fontMetrics().elidedText(index.data().toString(), Qt::ElideRight, option.rect.width(), Qt::TextSingleLine);

    QFont p = painter->font();
    p.setBold(true);
    painter->setFont(p);
    painter->drawText(option.rect.topLeft()+QPoint((option.rect.width() - painter->fontMetrics().width(text))/2,
                      PreviewHeight + 3.8*ItemMargin + FrameThickness*2), text);

    painter->restore();
}

bool SpacesDelegate::eventFilter(QObject *watched, QEvent *event)
{
    if (m_itemView->model())
    {
        if (event->type() == QEvent::MouseMove)
        {

            QMouseEvent *e = static_cast<QMouseEvent*>(event);

            m_currentCurPos = e->pos();
            QModelIndex index = m_itemView->indexAt(e->pos());

            if (!index.isValid() && !m_isEditing)
            {
                if (m_operationBar && m_titleEditor && !m_itemView->currentIndex().isValid()
                        && m_itemView->currentIndex() != m_currentEditingIndex )
                {
                    m_operationBar->hide();
                    m_titleEditor->hide();
                }

//                 m_itemView->setCurrentIndex(QModelIndex());
            } 
            else 
                if (index.isValid())
                {
                    //NOTE es importante que el viewport tenga el foco para que funcione correctamte la seleccion de items con el movimieto del mouse
                    if (m_currentEditingIndex != index && !m_isEditing)
                        m_itemView->viewport()->setFocus();

                    setCurrentSpace(index);
                    
                }

            return true;
        }
        else if (event->type() == QEvent::MouseButtonPress)
        {

            QMouseEvent *e = static_cast<QMouseEvent*>(event);

            QModelIndex index = m_itemView->indexAt(e->pos());

            if (!index.isValid())
            {
                m_itemView->setCurrentIndex(QModelIndex());

                m_isEditing = false;

                if (m_operationBar && m_titleEditor)
                {
                    m_operationBar->hide();
                    m_titleEditor->hide();
                    m_titleEditor->clear();
                }
            }
            else
            {
                invalidClick(index);
            }

            return true;
        }
        else if (event->type() == QEvent::MouseButtonDblClick)
        {
            QMouseEvent *e = static_cast<QMouseEvent*>(event);

            QModelIndex index = m_itemView->indexAt(e->pos());

            if (index.isValid())
                emit showSpace(index);
        }
    }

    return QStyledItemDelegate::eventFilter(watched, event);
}

void SpacesDelegate::setCurrentSpace(const QModelIndex& index)
{
    if (m_isEditing)
        return;

    m_itemView->setCurrentIndex(index);

    updateOperationBarPos(index);
    
    if (m_operationBar && m_titleEditor)
        m_operationBar->show();
}

void SpacesDelegate::editCurrentSpace()
{
    if (!m_itemView->currentIndex().isValid())
        return;

    m_currentEditingIndex = m_itemView->currentIndex();

    m_isEditing = true;

    QRect rect = m_itemView->visualRect(m_itemView->currentIndex());

    m_titleEditor->setText(m_itemView->currentIndex().data().toString());
    m_titleEditor->resize(rect.width(), m_titleEditor->height());
    m_titleEditor->move(rect.left()/*+(PreviewWidth-m_operationBar->width())/2*/,rect.top() + rect.height() - m_operationBar->height() - .8*m_titleEditor->height());
    m_titleEditor->selectAll();
    m_titleEditor->setFocus();
    m_titleEditor->show();
}

void SpacesDelegate::removeCurrentSpace()
{
    if (!m_itemView->currentIndex().isValid())
        return;

    m_document->removeSpace(m_itemView->currentIndex().row());

    m_isEditing = false;

    m_operationBar->hide();
    m_titleEditor->hide();

    filterEvent();
}

void SpacesDelegate::setDocument(DataStore *doc)
{
    Q_ASSERT(doc);

    m_document=doc;
}

void SpacesDelegate::hideOperatorBar()
{
    m_operationBar->hide();
}

void SpacesDelegate::finishEditingTitle(const QString &newtitle )
{
    if (!m_itemView->currentIndex().isValid())
        return;

    m_isEditing = false;

    m_itemView->model()->setData(m_itemView->currentIndex(), m_titleEditor->text());

    m_titleEditor->hide();

    QModelIndex posi = m_itemView->indexAt(m_currentCurPos);

    if (posi.isValid())
    {
        if (posi != m_currentEditingIndex)
            setCurrentSpace(posi);
    }
    else
    {
        m_operationBar->hide();
    }
}

void SpacesDelegate::invalidClick(const QModelIndex& index)
{
    if (index != m_currentEditingIndex)
    {
        m_isEditing = false;
        if (m_operationBar && m_titleEditor)
        {
            m_titleEditor->hide();
            m_titleEditor->clear();

        }

        setCurrentSpace(index);
    }
}

// this will be call always from aoutside of this class
void SpacesDelegate::filterEvent()
{
    m_isEditing = false;

    if (m_operationBar)
        m_operationBar->hide();

    if (m_titleEditor)
    {
        m_titleEditor->clear();
        m_titleEditor->hide();
    }

    m_itemView->clearSelection();
    m_itemView->setCurrentIndex(QModelIndex());

    setCurrentSpace(QModelIndex());
    invalidClick(QModelIndex());
}

void SpacesDelegate::updateOperationBarPos(const QModelIndex& index)
{
    if (m_operationBar && m_titleEditor)
    {
        QRect rect = m_itemView->visualRect(index);

        m_operationBar->move(rect.left()+(PreviewWidth-m_operationBar->width())/2,rect.top() + rect.height() - m_operationBar->height());
    }
}

///The view

SpacesView::SpacesView(QWidget* parent): QListView(parent)
{
    setViewMode(IconMode);
    setUniformItemSizes(true);
    setEditTriggers(NoEditTriggers);
    setFlow(LeftToRight);
    setWrapping(true);
    setResizeMode(Adjust);
    setSelectionRectVisible(true);
    setVerticalScrollMode(ScrollPerPixel);
    setHorizontalScrollMode(ScrollPerPixel);
    setMouseTracking(true);
    setSelectionMode(QListView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectItems);
}

void SpacesView::resizeEvent(QResizeEvent* e)
{
    static_cast<SpacesDelegate*>(itemDelegate())->updateOperationBarPos(currentIndex());
    
    QListView::resizeEvent(e);
}
