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


#include "spacesview.h"



#include <KDebug>
#include <KLocale>
#include <KIcon>
#include <QMouseEvent>
#include <QMenu>
#include <KCategoryDrawer>
#include <QHeaderView>
#include "functiondelegate.h"
#include <KDE/KCursor>
#include "spacesmodel.h"
#include "space.h"

#include "functionsmodel.h"
namespace Keomath
{


SpacesView::SpacesView(QWidget * parent)
    : KCategorizedView(parent)
    , m_mainFunctionsModel(0)
{


    setSelectionBehavior(QAbstractItemView::SelectItems);
    setSelectionMode(QAbstractItemView::SingleSelection);




    setMouseTracking(true);

    
    int marg = 10;

    setGridSizeOwn(QSize(256 + marg,256 + marg));

    

    setCategoryDrawer(new KCategoryDrawerV2());
    setViewMode(QListView::IconMode);



}

void SpacesView::selectionChanged(const QItemSelection & selected, const QItemSelection &)
{
    if(!selected.indexes().isEmpty())
    {
        QModelIndex idx=selected.indexes().first();
        model()->setData(idx, QVariant(), SpacesModel::Selection);
    }
}

void SpacesView::mouseMoveEvent ( QMouseEvent * e )
{
    QModelIndex clickIdx(indexAt(e->pos()));

    if(clickIdx.isValid())
    {
        setCursor(Qt::PointingHandCursor);
    }
    else
    {
        clearSelection();
        setCursor(Qt::ArrowCursor);
    }










    KCategorizedView::mousePressEvent(e);



}
void SpacesView::setMainFunctionsModel(FunctionsModel *fm)
{
    m_mainFunctionsModel = fm;
}

void SpacesView::mousePressEvent(QMouseEvent * e)
{
    QModelIndex clickIdx(indexAt(e->pos()));

    if(clickIdx.isValid())
    {
        if (e->button()==Qt::RightButton)
        {
            QModelIndex nameIdx(clickIdx.sibling(clickIdx.row(), 0));
            
            QString actuallyShown;
            
            
            
            
            
            
            
            

            QMenu menu(this);
            
            QAction* actionShown = menu.addAction(KIcon("user-online"), actuallyShown);
            actionShown->setText(i18n("Mostrar Espacio"));
            QAction* actionRemove=menu.addAction(KIcon("list-remove"), i18n("Eliminar Espacio '%1'", model()->data(nameIdx).toString()));
            QAction* result=menu.exec(e->globalPos());
            if(result==actionShown)
            {
                
                

                SpacesFilterProxyModel *sp = qobject_cast<Keomath::SpacesFilterProxyModel*>(model());
                Keomath::SpacesModel *sm = qobject_cast<Keomath::SpacesModel*>(sp->sourceModel());



                emit spaceShown(sm->m_spaceList.at(sp->mapToSource(clickIdx).row()));

            }
            else if(result == actionRemove)
            {


                
                

                if (m_mainFunctionsModel)
                {

                    SpacesFilterProxyModel *sp = qobject_cast<Keomath::SpacesFilterProxyModel*>(model());
                    Keomath::SpacesModel *sm = qobject_cast<Keomath::SpacesModel*>(sp->sourceModel());


                    m_mainFunctionsModel->removeItemsBySpace(sm->m_spaceList.at(sp->mapToSource(clickIdx).row()).id());
                }

                
                model()->removeRow(clickIdx.row());
            }
        }
        else if (e->button()==Qt::LeftButton)
        {

            SpacesFilterProxyModel *sp = qobject_cast<Keomath::SpacesFilterProxyModel*>(model());
            Keomath::SpacesModel *sm = qobject_cast<Keomath::SpacesModel*>(sp->sourceModel());



            emit spaceShown(sm->m_spaceList.at(sp->mapToSource(clickIdx).row()));



        }


    }
    else
    {
        KCategorizedView::mousePressEvent(e);
    }


}

} 
