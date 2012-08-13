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

#include "spacesdelegate.h"

#include "spaceitem.h"
#include "spaceinformation.h"
#include "spaceoptions.h"
#include "spacesmodel.h"
#include "datastore.h"
#include "spaceplotsproxymodel.h"
#include "analitza/variables.h"
#include <analitzaplot/plotsmodel.h>
#include <analitzaplot/plotsdictionarymodel.h>
#include <QDebug>
#include <QApplication>
///
#include <QtGui/QAbstractItemView>
#include <QtGui/QListView>
#include <QtGui/QStringListModel>
#include <QtGui/QMainWindow>
#include <QtGui/QPainter>
#include <QtGui/QRadialGradient>
#include <QtGui/QPaintEvent>
#include <QtCore/QDebug>
#include <QtGui/QToolButton>
#include <QtGui/QMenu>
#include <QLabel>
#include <QHBoxLayout>


#include <kpushbutton.h>
#include <klineedit.h>
#include <kicon.h>
#include <kwidgetitemdelegate.h>
#include <KLocalizedString>
#include <ksqueezedtextlabel.h>

SpacesDelegate::SpacesDelegate(QListView *itemView, QObject *parent)
    : KWidgetItemDelegate(itemView, parent)
, m_isEditing(false)
, m_iconMode(false) // el default en el listvieew es listmode no el iconmode
{
    if (itemView->viewMode() == QListView::IconMode)
        m_iconMode = true;
    
    itemView->setMouseTracking(true);
    itemView->setSelectionMode(QListView::NoSelection);
    
    itemView->viewport()->installEventFilter(this);

//     connect(itemView, SIGNAL(entered(QModelIndex)), SLOT(setCurrentSpace(QModelIndex)));
//     connect(itemView, SIGNAL(clicked(QModelIndex)), SLOT(invalidClick(QModelIndex)));
    
    //BEGIN btns
    
    if (m_iconMode)
    {
        m_operationBar = new QWidget(itemView->viewport());
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
        m_editButton->setToolTip(i18n("Edit"));
        m_editButton->setIcon(KIcon("document-edit"));
        connect(m_editButton, SIGNAL(pressed()), SLOT(editCurrentSpace()));

        QToolButton *m_showButton = new QToolButton(m_operationBar);
        m_showButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
        m_showButton->setAutoRepeat(false);
        m_showButton->setToolTip(i18n("Show"));
        m_showButton->setIcon(KIcon("kig"));
    //         connect(m_detailsButton, SIGNAL(clicked()), this, SLOT(slotDetailsClicked()));

        QHBoxLayout* layout = new QHBoxLayout(m_operationBar);

        layout->setSpacing(1);
        layout->addWidget(m_removeButton);
        layout->addWidget(m_editButton);
        layout->addWidget(m_showButton);

        m_operationBar->adjustSize();
        m_operationBar->hide();
        //END btns
        
        m_titleEditor = new KLineEdit(itemView->viewport());
        m_titleEditor->setClearButtonShown(true);
        m_titleEditor->hide();
        
        connect(m_titleEditor, SIGNAL(editingFinished()), SLOT(finishEditing()));
    }
}

SpacesDelegate::~SpacesDelegate()
{
    if (m_iconMode)
    {
        //NOTE esto falta en kdelibs
        m_operationBar->deleteLater();
        m_titleEditor->deleteLater();
    }
}

QSize SpacesDelegate::sizeHint(const QStyleOptionViewItem &option,
                                       const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QSize size;

    if (m_iconMode)
    {
        size.setWidth(PreviewWidth+2*ItemMargin);
        size.setHeight(PreviewHeight+4*option.fontMetrics.height()+2*ItemMargin); // up to 6 lines of text, and two margins
        return size;
    }

    // else listmode
    
    size.setWidth(option.fontMetrics.height() * 4);
//         size.setHeight(qMax(option.fontMetrics.height() * 7, PreviewHeight)); // up to 6 lines of text, and two margins
    size.setHeight(PreviewHeight+0.8*option.fontMetrics.height()); // up to PreviewHeight and two margins
    return size;
}

void SpacesDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (m_iconMode)
    {
        QStyle *style = QApplication::style();
        style->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, 0);

        painter->save();

        if (option.state & QStyle::State_Selected) {
            painter->setPen(QPen(option.palette.highlightedText().color()));
        } else {
            painter->setPen(QPen(option.palette.text().color()));
        }

        painter->drawPixmap(option.rect.topLeft()+QPoint(ItemMargin,ItemMargin), index.data(Qt::DecorationRole).value<QPixmap>());

        painter->restore();
    }
    else    // listmode
    {
        int margin = option.fontMetrics.height() / 2;

        QStyle *style = QApplication::style();
        style->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, 0);

        painter->save();

        if (option.state & QStyle::State_Selected) {
            painter->setPen(QPen(option.palette.highlightedText().color()));
        } else {
            painter->setPen(QPen(option.palette.text().color()));
        }

        int height = option.rect.height();
        QPoint point(option.rect.left() + margin, option.rect.top() + ((height - PreviewHeight) / 2));

        QPixmap image = index.data(Qt::DecorationRole).value<QPixmap>();
    //         point.setX((PreviewWidth - image.width())/2 + 5);
    //         point.setY(option.rect.top() + ((height - image.height()) / 2));
        painter->drawPixmap(point, image);

        painter->restore();
    }
}

QList<QWidget*> SpacesDelegate::createItemWidgets() const
{
    if (m_iconMode)
    {
        KSqueezedTextLabel  *title = new KSqueezedTextLabel ();
        title->setSelection(0,0);
        title->setTextElideMode(Qt::ElideRight);
        title->setWordWrap(true);
        title->setAlignment(Qt::AlignHCenter);
        title->setTextInteractionFlags(Qt::TextSelectableByMouse);
                
        return QList<QWidget*>() << title;
    }
    
    // else listmode

    QLabel *info = new QLabel();

    QToolButton *show = new QToolButton();
    QToolButton *edit = new QToolButton();
    QToolButton *remove = new QToolButton();

    show->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    edit->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    remove->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    return QList<QWidget*>() << info << show << edit << remove;
    
}

void SpacesDelegate::updateItemWidgets(const QList<QWidget*> widgets, const QStyleOptionViewItem &option, const QPersistentModelIndex &index) const
{
    if (m_iconMode)
    {
        int elementYPos = PreviewHeight + ItemMargin + FrameThickness*2;

        KSqueezedTextLabel  * title = qobject_cast<KSqueezedTextLabel *>(widgets.first());

        if (title) 
        {
            if (m_isEditing && m_currentEditingIndex == index)
            {
                title->hide();
            }
            else
            {
                title->setTextInteractionFlags(Qt::TextSelectableByMouse /*| Qt::TextSelectableByKeyboard*/);
                title->setText(index.data().toString());
                title->resize(QSize(option.rect.width() - (ItemMargin * 2), option.fontMetrics.height()));
                title->move((PreviewWidth-title->width())/2, elementYPos);
                title->show();
            }
        }
    }
    else //listmode
    {
        // setup the install button
    //         int margin = option.fontMetrics.height() / 2;
        int margin = 0.8*option.fontMetrics.height();

        int right = option.rect.width();

        QToolButton * installButton = qobject_cast<QToolButton*>(widgets.at(1));
        if (installButton != 0)
        {
            installButton->setText(i18n("Show"));
            installButton->setIcon(KIcon("kig"));
        }

        QToolButton* detailsButton = qobject_cast<QToolButton*>(widgets.at(2));
        if (detailsButton) {
            detailsButton->setText(i18n("Edit"));
            detailsButton->setIcon(KIcon("document-edit"));
        }

        if (installButton && detailsButton) {
            if (m_buttonSize.width() < installButton->sizeHint().width()) {
                const_cast<QSize&>(m_buttonSize) = QSize(
                                                    qMax(option.fontMetrics.height() * 7,
                                                            qMax(installButton->sizeHint().width(), detailsButton->sizeHint().width())),
                                                    installButton->sizeHint().height());
            }
            installButton->resize(m_buttonSize);
            installButton->move(right - installButton->width() - margin, option.rect.height()/2 - installButton->height()*1.5);
            detailsButton->resize(m_buttonSize);
            detailsButton->move(right - installButton->width() - margin, option.rect.height()/2 - installButton->height()/2);
        }

        QToolButton * rating = qobject_cast<QToolButton*>(widgets.at(3));
        if (rating)
        {
            rating->setText(i18n("Remove"));
            rating->setIcon(KIcon("list-remove"));

            // put the rating label below the install button
            rating->move(right - installButton->width() - margin, option.rect.height()/2 + installButton->height()/2);
            rating->resize(m_buttonSize);
        }

        QLabel * infoLabel = qobject_cast<QLabel*>(widgets.at(0));
        infoLabel->setWordWrap(true);
        if (infoLabel != NULL) {
            infoLabel->move(PreviewWidth + margin * 3, 0);
            infoLabel->resize(QSize(option.rect.width() - PreviewWidth - (margin * 6) - m_buttonSize.width(), option.fontMetrics.height() * 7));
            infoLabel->setText(index.data().toString());
        }    
    }
}

bool SpacesDelegate::eventFilter(QObject *watched, QEvent *event)
{
    if (m_iconMode)
    {
        switch (event->type())
        {
            case QEvent::MouseMove:
            {
                QMouseEvent *e = static_cast<QMouseEvent*>(event);
            
                m_currentCurPos = e->pos();
                QModelIndex index = itemView()->indexAt(e->pos());

                if (!index.isValid() && !m_isEditing)
                {
                    itemView()->setCurrentIndex(QModelIndex());
                    
                    m_operationBar->hide();
                    m_titleEditor->hide();
                }
                else 
                    if (index.isValid())
                    {
                        setCurrentSpace(index);
                    }
            
                return true;
            }
            break;
            
            case QEvent::MouseButtonPress:
            {
                
                QMouseEvent *e = static_cast<QMouseEvent*>(event);
                        
                QModelIndex index = itemView()->indexAt(e->pos());
                
                if (!index.isValid())
                {
                    m_operationBar->hide();
                    
                    m_isEditing = false;
                    m_titleEditor->hide();
                    m_titleEditor->clear();
                }
                else
                {
                    invalidClick(index);
                }

                //forzamos a que se ejecute el updateItemWidgets del delegate
                dummyUpdate();
                
                return true;
            }
            break;
        }
    }

   return KWidgetItemDelegate::eventFilter(watched, event);
}

void SpacesDelegate::dummyUpdate()
{
    //TODO
    for (int i = 0; i < itemView()->model()->rowCount(); ++i)
        itemView()->model()->setData(static_cast<SpacesModel*>(itemView()->model())->index(i), static_cast<SpacesModel*>(itemView()->model())->index(i).data());
}

void SpacesDelegate::setCurrentSpace(const QModelIndex& index)
{
    if (m_isEditing) return;
    
    itemView()->setCurrentIndex(index);
    
    if (m_iconMode)
    {
        QRect rect = itemView()->visualRect(index);
        
        m_operationBar->move(rect.left()+(PreviewWidth-m_operationBar->width())/2,rect.top() + rect.height() - m_operationBar->height());
        m_operationBar->show();
    }
    else
    {
        
    }
}

void SpacesDelegate::removeCurrentSpace()
{
    if (!itemView()->currentIndex().isValid()) return;

    itemView()->model()->removeRow(itemView()->currentIndex().row());

    m_isEditing = false;
    
    if (m_iconMode)
    {
        m_operationBar->hide();
        m_titleEditor->hide();
    }
    else
    {
        
    }
}

void SpacesDelegate::editCurrentSpace()
{
    if (!itemView()->currentIndex().isValid()) return;
    
    m_currentEditingIndex = itemView()->currentIndex();

    m_isEditing = true;
    
    dummyUpdate();
    
    if (m_iconMode)
    {
        QRect rect = itemView()->visualRect(itemView()->currentIndex());

        m_titleEditor->setText(itemView()->currentIndex().data().toString());
        m_titleEditor->resize(rect.width(), m_titleEditor->height());
        m_titleEditor->move(rect.left()/*+(PreviewWidth-m_operationBar->width())/2*/,rect.top() + rect.height() - m_operationBar->height() - .8*m_titleEditor->height());
        m_titleEditor->selectAll();
        m_titleEditor->setFocus();
        m_titleEditor->show();
    }
    else
    {
        
    }
}

void SpacesDelegate::showCurrentSpace()
{
    if (!itemView()->currentIndex().isValid()) return;
    
}

void SpacesDelegate::finishEditing(const QString &newtitle )
{
    if (!itemView()->currentIndex().isValid()) return;

    itemView()->model()->setData(itemView()->currentIndex(), m_titleEditor->text());
    
    m_isEditing = false;
    
    if (m_iconMode)
    {
        m_titleEditor->hide();
        
        //NOTE como se asume que se esta editando un solo item y que el resto esta ok despues de un dummyupdate
        //entonces aqui no es necesario llamar a dummyupdate pues el setdata de arriva actualiza el estado de este 
        //intem en el delegate
        
        //verifico la posision del cursor
        
        QModelIndex posi = itemView()->indexAt(m_currentCurPos);
        
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
    else
    {
        
    }
}

void SpacesDelegate::invalidClick(const QModelIndex& index)
{
    if (m_iconMode)
    {

        if (index != m_currentEditingIndex)
        {
            m_isEditing = false;
            m_titleEditor->hide();
            m_titleEditor->clear();
            
            dummyUpdate();
        
            setCurrentSpace(index);
        }
    }
    else
    {
        
    }
}
