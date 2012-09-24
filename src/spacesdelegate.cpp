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

#include "spaceinformation.h"
#include "spaceoptions.h"
#include "dictionariesmodel.h"
#include "datastore.h"
#include "spaceplotsproxymodel.h"
#include "analitza/variables.h"
#include <analitzaplot/plotsmodel.h>
#include <analitzaplot/plotsdictionarymodel.h>
#include <dictionaryitem.h>
#include <QDebug>
#include <QApplication>
#include <KRichTextWidget>

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
#include <QTextEdit>


#include <kpushbutton.h>
#include <klineedit.h>
#include <kicon.h>
#include <kwidgetitemdelegate.h>
#include <KLocalizedString>
#include <ksqueezedtextlabel.h>

// #include "ui_spaceeditor.h"
/*
SpaceEditor::SpaceEditor(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f)
{
    m_widget = new Ui::SpaceEditorWidget;
    m_widget->setupUi(this);
}

SpaceEditor::~SpaceEditor()
{
    delete m_widget;
}


void SpaceEditor::setSpace(SpaceItem* space)
{
    m_space = space;
    
    m_widget->title->setText(space->title());
    m_widget->description->setText(space->description());    
    m_widget->thumbnail->setPixmap(space->thumbnail());
    m_widget->info->setText("Hola");
    m_widget->timestamp->setText(space->timestamp().toString("%A %l:%M %p %B %Y"));
}*/

///

LineEdit::LineEdit(QWidget* parent): KLineEdit(parent)
{

    connect(this, SIGNAL(editingFinished()), SLOT(procsSditingFinished()));
}

void LineEdit::procsSditingFinished()
{
    emit editingFinished(text());
}


///

SpacesDelegate::SpacesDelegate(QListView *itemView, QObject *parent)
    : QStyledItemDelegate(parent), m_itemView(itemView)
, m_isEditing(false)
, m_iconMode(true) // el default en el listvieew es listmode no el iconmode
, m_operationBar(0)
, m_titleEditor(0)
{
    setIconMode(true);

//     if (itemView->viewMode() == QListView::IconMode)
//         setIconMode(true);
    
    itemView->setMouseTracking(true);
    itemView->setSelectionMode(QListView::SingleSelection);
    itemView->setSelectionBehavior(QAbstractItemView::SelectItems);
    itemView->viewport()->installEventFilter(this);
    

    //no funciona ... haciend un methodo publcio
//     connect(itemView->model(), SIGNAL(layoutAboutToBeChanged()), SLOT(filterEvent()));
//     connect(itemView->model(), SIGNAL(layoutChanged()), SLOT(filterEvent()));

    
    connect(this, SIGNAL(showSpace(QModelIndex)), itemView, SIGNAL(doubleClicked(QModelIndex)));

    connect(itemView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), 
            SLOT(setCurrentSpace(QModelIndex, QModelIndex)));
        
//         connect(this, SIGNAL(activateSpace(QModelIndex)), itemView, SLOT(setCurrentIndex(QModelIndex)));

//     connect(itemView, SIGNAL(entered(QModelIndex)), SLOT(setCurrentSpace(QModelIndex)));
//     connect(itemView, SIGNAL(clicked(QModelIndex)), SLOT(invalidClick(QModelIndex)));
    
    
}

SpacesDelegate::~SpacesDelegate()
{
    if (m_iconMode)
    {
        //NOTE esto falta en kdelibs
//         m_operationBar->deleteLater();
//         m_titleEditor->deleteLater();
    }
}

void SpacesDelegate::setIconMode(bool im)
{
//     if (im == m_iconMode) return ;

    //TODO
    m_iconMode = im;
    
    //BEGIN btns
    
    if (m_iconMode)
    {
        itemView()->setEditTriggers(QListView::EditKeyPressed);

        if (!m_operationBar && ! m_titleEditor)
        {
            m_operationBar = new QWidget(itemView()->viewport());
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
//             connect(m_showButton, SIGNAL(pressed()), SLOT(showCurrentSpace()));

            QHBoxLayout* layout = new QHBoxLayout(m_operationBar);

            layout->setSpacing(1);
            layout->addWidget(m_removeButton);
            layout->addWidget(m_editButton);
            layout->addWidget(m_showButton);

            m_operationBar->adjustSize();
            m_operationBar->hide();
            //END btns
            
            m_titleEditor = new LineEdit(itemView()->viewport());
            m_titleEditor->setClearButtonShown(true);
            m_titleEditor->hide();
            
            connect(m_titleEditor, SIGNAL(editingFinished(QString)), SLOT(finishEditingTitle(QString)));
        }
    }
    else //list mode
    {
        itemView()->setEditTriggers(QAbstractItemView::CurrentChanged);
    }

    dummyUpdate(); 
}

QWidget* SpacesDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
//     if (!m_iconMode && index.isValid())
//     {
//         m_currentEditingIndex = index;
//         m_isEditing = true;
//         
//         return new SpaceEditor(parent);
//     }
    
///

    if (m_iconMode)
    {
        m_currentEditingIndex = index;
        m_isEditing = true;
        LineEdit *editor = new LineEdit(parent);
        
        QRect rect = itemView()->visualRect(itemView()->currentIndex());

        editor->resize(rect.width(), m_titleEditor->height());
        editor->move(rect.left()/*+(PreviewWidth-m_operationBar->width())/2*/,rect.top() + rect.height() - m_operationBar->height() - .8*m_titleEditor->height());
        editor->selectAll();
        editor->setFocus();
        editor->setClearButtonShown(true);
        
        return editor;
    }

    return new QWidget(parent);
}

void SpacesDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (!index.isValid()) return ;

//     m_currentEditingIndex = index;
//     m_isEditing = true;
//     
//     editor->setGeometry(option.rect);
//     editor->hide();
}

void SpacesDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    if (!index.isValid()) return ;

//     m_currentEditingIndex = index;
//     m_isEditing = true;
// 
//     SpaceEditor *spaceEditor = static_cast<SpaceEditor*>(editor);
//     
//     spaceEditor->setSpace(static_cast<SpacesModel*>(itemView()->model())->item(index.row()));
    static_cast<LineEdit*>(editor)->setText(itemView()->currentIndex().data().toString());
}

void SpacesDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    if (!index.isValid()) return ;    

        m_currentEditingIndex = QModelIndex();
        m_isEditing = false;

    static_cast<SpacesFilterProxyModel*>(model)->sourceModel()->setData(index, static_cast<LineEdit*>(editor)->text());
    
//     
//     m_currentEditingIndex = QModelIndex();
//     m_isEditing = false;
}


QSize SpacesDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
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
    if (!index.isValid() || !itemView()->model()) return QStyledItemDelegate::paint(painter, option, index);

    if (m_iconMode)
    {


        painter->save();

        if (option.state & QStyle::State_Selected) 
        {
            //focus
            QStyle *style = QApplication::style();
            style->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, 0);
            //endfocus
            
            painter->setPen(QPen(option.palette.highlightedText().color()));
        } else {
            painter->setPen(QPen(option.palette.text().color()));
        }

        painter->drawPixmap(option.rect.topLeft()+QPoint(ItemMargin,ItemMargin), index.data(Qt::DecorationRole).value<QPixmap>());

        QString text = painter->fontMetrics().elidedText(index.data().toString(), Qt::ElideRight, option.rect.width(), Qt::TextSingleLine);

        QFont p = painter->font();
        p.setBold(true);
        painter->setFont(p);
        painter->drawText(option.rect.topLeft()+QPoint((option.rect.width() - painter->fontMetrics().width(text))/2, 
            PreviewHeight + 3.8*ItemMargin + FrameThickness*2), text);

        painter->restore();
    }
    else    // listmode
    {
        int margin = option.fontMetrics.height() / 2;

        QStyle *style = QApplication::style();
        style->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, 0);

        if (!m_isEditing || m_currentEditingIndex != index)
        {
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
        else
        {
            
//             itemView()->ac
        }
    }
    
}

QList<QWidget*> SpacesDelegate::createItemWidgets() const
{
    if (!itemView()->model()) return QList<QWidget*>();

    if (m_iconMode)
    {
        KSqueezedTextLabel *title = new KSqueezedTextLabel();
        title->setSelection(0,0);
        title->setTextElideMode(Qt::ElideRight);
        title->setWordWrap(true);
        title->setAlignment(Qt::AlignCenter);
        title->setTextInteractionFlags(Qt::TextSelectableByMouse);
                
        return QList<QWidget*>() << title;
    }

    // else listmode

    KSqueezedTextLabel *info = new KSqueezedTextLabel();
    info->setSelection(0,0);
    info->setTextElideMode(Qt::ElideRight);
    info->setWordWrap(true);
    info->setAlignment(Qt::AlignJustify);
    info->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);

    QToolButton *remove = new QToolButton();
    QToolButton *edit = new QToolButton();
    QToolButton *show = new QToolButton();

    show->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    edit->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    remove->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    connect(remove, SIGNAL(pressed()), SLOT(removeCurrentSpace()));
    connect(edit, SIGNAL(pressed()), SLOT(editCurrentSpace()));

    
    //editors
    LineEdit *titleEditor = new LineEdit();
    titleEditor->setClearButtonShown(true);

    connect(titleEditor, SIGNAL(editingFinished(QString)), SLOT(finishEditingTitle(QString)));

    KRichTextEdit *descriptionEditor = new KRichTextEdit();
    
    return QList<QWidget*>() << info << titleEditor << descriptionEditor << show << edit << remove;
}

void SpacesDelegate::updateItemWidgets(const QList<QWidget*> widgets, const QStyleOptionViewItem &option, const QPersistentModelIndex &index) const
{
    if (!index.isValid() || !itemView()->model() || widgets.isEmpty()) return;

    if (m_iconMode)
    {
        KSqueezedTextLabel  * title = qobject_cast<KSqueezedTextLabel *>(widgets[0]);

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
                title->move((PreviewWidth-title->width())/2, PreviewHeight + ItemMargin + FrameThickness*2);
                title->show();
            }
        }
    }
    else //listmode
    {
        QSize m_buttonSize;

        // setup the install button
    //         int margin = option.fontMetrics.height() / 2;
        int margin = 0.8*option.fontMetrics.height();

        int right = option.rect.width();

        QToolButton * showButton = qobject_cast<QToolButton*>(widgets.at(3));
        if (showButton != 0)
        {
            showButton->setText(i18n("Show"));
            showButton->setIcon(KIcon("kig"));
        }

        QToolButton* editButton = qobject_cast<QToolButton*>(widgets.at(4));
        if (editButton) {
            editButton->setText(i18n("Edit"));
            editButton->setIcon(KIcon("document-edit"));
        }

        if (showButton && editButton) 
        {
            if (m_buttonSize.width() < showButton->sizeHint().width()) 
            {
//                 const_cast<QSize&>(m_buttonSize) = QSize(
                m_buttonSize = QSize(qMax(option.fontMetrics.height() * 7,
                                                            qMax(showButton->sizeHint().width(), editButton->sizeHint().width())),
                                                    showButton->sizeHint().height());
            }
            
            showButton->resize(m_buttonSize);
            showButton->move(right - showButton->width() - margin, option.rect.height()/2 - showButton->height()*1.5);
            editButton->resize(m_buttonSize);
            editButton->move(right - showButton->width() - margin, option.rect.height()/2 - showButton->height()/2);
        }

        QToolButton * removeButton = qobject_cast<QToolButton*>(widgets.at(5));
        if (removeButton)
        {
            removeButton->setText(i18n("Remove"));
            removeButton->setIcon(KIcon("list-remove"));
            removeButton->move(right - showButton->width() - margin, option.rect.height()/2 + showButton->height()/2);
            removeButton->resize(m_buttonSize);
        }

        if (m_isEditing && m_currentEditingIndex == index)
        {
            KSqueezedTextLabel * infoLabel = qobject_cast<KSqueezedTextLabel*>(widgets.at(0));
            if (infoLabel) infoLabel->hide();
            
            LineEdit *titleEditor = qobject_cast<LineEdit*>(widgets.at(1));
            KRichTextEdit *descriptionEditor = qobject_cast<KRichTextEdit*>(widgets.at(2));

            if (titleEditor && descriptionEditor) 
            {
                //el ancho de 1 PreviewWidth  (por eso el 2*PreviewWidth) sera para informacion que no se podra editar fechas caitdad de plot ets
                titleEditor->move(2*PreviewWidth + margin, margin*.5);
                titleEditor->resize(QSize(option.rect.width() - 2*PreviewWidth - (margin * 2) - m_buttonSize.width(), titleEditor->height()));
                titleEditor->setText(index.data().toString());
                titleEditor->show();

                descriptionEditor->move(2*PreviewWidth + margin, margin*.5 + titleEditor->height());
                descriptionEditor->resize(QSize(option.rect.width() - 2*PreviewWidth - (margin * 2) - m_buttonSize.width(), 
                                                option.rect.size().height() - titleEditor->height() - 2*margin));
                descriptionEditor->setHtml(static_cast<DictionariesModel*>(itemView()->model())->space(index.row())->description());
                descriptionEditor->show();
            }
        }
        else
        {
            LineEdit *titleEditor = qobject_cast<LineEdit*>(widgets.at(1));
            if (titleEditor) titleEditor->hide();

            KRichTextEdit *descriptionEditor = qobject_cast<KRichTextEdit*>(widgets.at(2));
            if (descriptionEditor) descriptionEditor->hide();

            KSqueezedTextLabel * infoLabel = qobject_cast<KSqueezedTextLabel*>(widgets.at(0));
            if (infoLabel) 
            {
                QString text = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
                    "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">p, li { white-space: pre-wrap; margin:0 0 0 0;}\n"
                    "</style></head><body><p><b>";

                    text += index.data().toString();

                text += "</b></p>\n";

                
                infoLabel->setWordWrap(true);
                infoLabel->move(PreviewWidth + margin * 3, margin*.5);
                infoLabel->resize(QSize(option.rect.width() - PreviewWidth - (margin * 6) - m_buttonSize.width(), option.fontMetrics.height() * 7));
                infoLabel->setText(text);
                infoLabel->show();
            }    
        }
    }
}

bool SpacesDelegate::eventFilter(QObject *watched, QEvent *event)
{
    if (itemView()->model())
    {
        if (event->type() == QEvent::MouseMove)
        {
            
            QMouseEvent *e = static_cast<QMouseEvent*>(event);
        
            m_currentCurPos = e->pos();
            QModelIndex index = itemView()->indexAt(e->pos());

            if (!index.isValid() && !m_isEditing)
            {
                
                if (m_iconMode && m_operationBar && m_titleEditor && !itemView()->currentIndex().isValid()
                    && itemView()->currentIndex() != m_currentEditingIndex )
                {
                    m_operationBar->hide();
                    m_titleEditor->hide();
                }
                
//                 itemView()->setCurrentIndex(QModelIndex());
            }else
                if (index.isValid())
                {
                    //NOTE es importante que el viewport tenga el foco para que funcione correctamte la seleccion de items con el movimieto del mouse 
                    if (m_currentEditingIndex != index && !m_isEditing)
                        itemView()->viewport()->setFocus();

                        

                        setCurrentSpace(index);
                        
//                     if (!m_iconMode)
//                     {
//                         itemView()->setCurrentIndex(index);
//                         //BUG qt seleciona un area vacia no se porque
//                         itemView()->clearSelection();
//                         itemView()->update(index);
//                         itemView()->viewport()->update();
//                         itemView()->update();
//                     }
                }
        
            return true;
        }
        else 
            if (event->type() == QEvent::MouseButtonPress)
            {
                
                QMouseEvent *e = static_cast<QMouseEvent*>(event);
                        
                QModelIndex index = itemView()->indexAt(e->pos());
                
                if (!index.isValid())
                {
                    itemView()->setCurrentIndex(QModelIndex());
                    
                    m_isEditing = false;
                    
                    if (m_iconMode && m_operationBar && m_titleEditor)
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

                //forzamos a que se ejecute el updateItemWidgets del delegate
                dummyUpdate();
                
                return true;
            }
            else
                if (event->type() == QEvent::MouseButtonDblClick)
                {
                    QMouseEvent *e = static_cast<QMouseEvent*>(event);
                            
                    QModelIndex index = itemView()->indexAt(e->pos());

                    if (index.isValid())
                        emit showSpace(index);
                }
                else
                    if (event->type() == QEvent::KeyPress)
                    {
                        invalidClick(itemView()->currentIndex());
                        
                        QKeyEvent *e = static_cast<QKeyEvent*>(event);
                        
                        if (e->key() == Qt::Key_Delete)
                        {
                            removeCurrentSpace();
                        }
                         
//                         m_operationBar->hide();
//                         m_titleEditor->hide();
//                         m_titleEditor->clear();
//                         
//                         setCurrentSpace(itemView()->currentIndex());
                  }
    }

   return QStyledItemDelegate::eventFilter(watched, event);
}

void SpacesDelegate::dummyUpdate()
{
    //TODO GSOC remove next iter
    //ya no es necesario pues ya no se usa kwidgetitemdelegate
//     if (!itemView()->model()) return ;
//         
//     //TODO
//     for (int i = 0; i < itemView()->model()->rowCount(); ++i)
//         itemView()->model()->setData(static_cast<SpacesModel*>(itemView()->model())->index(i), static_cast<SpacesModel*>(itemView()->model())->index(i).data());
}

void SpacesDelegate::setCurrentSpace(const QModelIndex& index, const QModelIndex& oldcurent)
{
    if (m_isEditing) return;
    
    itemView()->setCurrentIndex(index);
    
    if (m_iconMode && m_operationBar && m_titleEditor)
    {
        QRect rect = itemView()->visualRect(index);
        
        m_operationBar->move(rect.left()+(PreviewWidth-m_operationBar->width())/2,rect.top() + rect.height() - m_operationBar->height());
        m_operationBar->show();
    }
    else
    {
//         emit activateSpace(index);
        
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
        
        filterEvent();
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
    
    emit showSpace(itemView()->currentIndex());
}

void SpacesDelegate::finishEditingTitle(const QString &newtitle )
{
    if (!itemView()->currentIndex().isValid()) return;

    m_isEditing = false;

    if (m_iconMode)
    {
        itemView()->model()->setData(itemView()->currentIndex(), m_titleEditor->text());
        
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
//         static_cast<SpacesModel*>(itemView()->model())->item(itemView()->currentIndex().row())->set;
        itemView()->model()->setData(itemView()->currentIndex(), newtitle);
    }
}

void SpacesDelegate::invalidClick(const QModelIndex& index)
{
        if (index != m_currentEditingIndex)
        {
            m_isEditing = false;
            if (m_iconMode && m_operationBar && m_titleEditor)
            {
                m_titleEditor->hide();
                m_titleEditor->clear();
                
            }
            else // listmode
            {
                
            }
            
            dummyUpdate();
        
            setCurrentSpace(index);
        }
}

//NOTE ejecutar este metodo cuando se a cambiado de filtro en el proxy ... es decir ejecutarlo desde afuera de esta clase
void SpacesDelegate::filterEvent()
{
    m_isEditing = false;
    m_operationBar->hide();
    m_titleEditor->clear();
    m_titleEditor->hide();
    
    itemView()->clearSelection();
    itemView()->setCurrentIndex(QModelIndex());
    setCurrentSpace(QModelIndex());
    invalidClick(QModelIndex());
}
