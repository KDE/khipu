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

#include "document.h"
#include "spacesmodel.h"
#include <analitzaplot/plotsmodel.h>
#include <analitzaplot/planecurve.h>
#include <analitza/variables.h>
#include <analitza/expression.h>

#include <QDomImplementation>
#include <QBuffer>
#include <qitemselectionmodel.h>
#include <kicon.h>


Document::Document(QObject* parent)
: QObject(parent), m_currentSpace(-1)
{
    m_spacesModel = new SpacesModel(this);

//     m_spacesModel->addSpace(2, "adasd", "333", KIcon("kde").pixmap(QSize(256,256)));
//     m_spacesModel->addSpace(3, "123 2 234 424 23424 adaadad ad a ada adad sadassd", "333", KIcon("list-add").pixmap(QSize(256,256)));
//     m_spacesModel->addSpace(2, "adasd", "333", KIcon("roll").pixmap(QSize(256,256)));
//     m_spacesModel->addSpace(3, "adasd", "333", KIcon("list-remove").pixmap(QSize(256,256)));
//     m_spacesModel->addSpace(3, "adasd", "333", KIcon("oxygen").pixmap(QSize(256,256)));
//     m_spacesModel->addSpace(3, "adasd", "333", KIcon("okular").pixmap(QSize(256,256)));
//     m_spacesModel->addSpace(2, "adasd", "333", KIcon("dolphin").pixmap(QSize(256,256)));

    m_variables = new Analitza::Variables;
    
    m_plotsModel = new PlotsModel(this, m_variables);
    
    
    connect(m_plotsModel, SIGNAL(rowsInserted(QModelIndex,int,int)), SLOT(mapPlot(QModelIndex,int,int)));
    connect(m_plotsModel, SIGNAL(rowsRemoved(QModelIndex,int,int)), SLOT(unmapPlot(QModelIndex,int,int)));

//     connect(m_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
//         this, SLOT(updateFuncs(QModelIndex,QModelIndex)));
//     connect(m_model, SIGNAL(rowsInserted(QModelIndex,int,int)),
//         this, SLOT(addFuncs(QModelIndex,int,int)));
//     connect(m_model, SIGNAL(rowsRemoved(QModelIndex,int,int)),
//         this, SLOT(removeFuncs(QModelIndex,int,int)));
        
//     //test code
//     PlotsModel *model = m_plotsModel;
// 
//     model->addPlaneCurve(Analitza::Expression("x->x*x"), "para", Qt::cyan);
//     model->addPlaneCurve(Analitza::Expression("q->q+2"), "polar simple", Qt::green);
//     model->addPlaneCurve(Analitza::Expression("t->vector{t*t+1, t+2}"), "vec", Qt::yellow);
//     model->addPlaneCurve(Analitza::Expression("5*(x**2+y**2)**3=15*(x*y*72)**2"), "impl", Qt::red);
//     model->addPlaneCurve(Analitza::Expression("x->2+x*x"), "otra simple", Qt::blue);
//     model->addPlaneCurve(Analitza::Expression("(x**2+y**2)**3=4*(x**2)*(y**2)"), "otra simple", Qt::lightGray);
//     model->addPlaneCurve(Analitza::Expression("(y-x**2)**2=x*y**3"), "otra simple", Qt::lightGray);
//     model->addPlaneCurve(Analitza::Expression("sin(x)*sin(y)=1/2"), "otra simple", Qt::yellow);    
//     model->addPlaneCurve(Analitza::Expression("x->x*x+2"), "asdads", Qt::yellow);
    

}

Document::~Document()
{
    delete m_variables;
}

void Document::load(const KUrl& fileUrl)
{
//    QString filter = QString("*.gplacs|") + i18n("All Supported Files") +
//                      QString("\n*.gplacs|") + i18n("GPLACS standard file format") + QString(" (*.gplacs)");
// 
//     KUrl paletteUrl = KFileDialog::getOpenUrl(KUrl(), filter);
// 
// 
// 
//     QString paletteFileName;
// 
//     if (!paletteUrl.isEmpty())
//     {
//         if (KIO::NetAccess::download(paletteUrl, paletteFileName, this))
//         {
//             if (m_gplacsWidget->load(paletteFileName))
//             {
//                 updateTittleWhenOpenSaveDoc();
// 
//                 m_gplacsWidget->setModified(false);
//                 m_gplacsWidget->setSaved(true);
//             }
// 
// 
// 
//             KIO::NetAccess::removeTempFile(paletteFileName);
//         }
//         else
//             KMessageBox::error(this, KIO::NetAccess::lastErrorString());
//     }

///



//TODO no se a definido el formato
/*
    m_dashboardWidget->setCurrentIndex(1);
    m_dashboardWidget->setCurrentIndex(2);
    m_dashboardWidget->setCurrentIndex(0);



    QFile device(file);


    if (!device.exists())
    {

        m_lastErrorString = i18n("El archivo no existe.");

        return false;
    }

    if (!device.open(QFile::ReadOnly | QFile::Text))
    {

        m_lastErrorString = i18n("No se pudo abrir el archivo.");

        return false;
    }

    m_spacesModel->clear();
    //TODO
//     m_functionsModel->clear();
    

    QString errorStr;
    int errorLine;
    int errorColumn;

    QDomDocument domDocument;

    if (!domDocument.setContent(&device, true, &errorStr, &errorLine, &errorColumn))
    {

        m_lastErrorString  = i18n("El documento no es válido.");
        return false;
    }

    QDomElement root = domDocument.documentElement();

    QDomElement spacesElement = root.firstChildElement();

    QDomElement spaceElement = spacesElement.firstChildElement();

    while (!spaceElement.isNull())
    {
        QDomNodeList spaceDataElements = spaceElement.childNodes();

         Space space(spaceDataElements.at(2).toElement().text().toInt());
        space.setName(spaceDataElements.at(0).toElement().text());
        space.setDescription(spaceDataElements.at(1).toElement().text());
        space.setThumbnail(utf8ToPixmap(spaceDataElements.at(3).toElement().text().toUtf8()));
        space.setDate(spaceDataElements.at(4).toElement().text());
        space.setId(QUuid(spaceDataElements.at(5).toElement().text()));

        m_spacesModel->addSpace(space);

        spaceElement = spaceElement.nextSiblingElement();
    }

    QDomElement functionsElement = spacesElement.nextSiblingElement();

    QDomElement functionElement = functionsElement.firstChildElement();

    while (!functionElement.isNull())
    {
        QDomNodeList functionDataElements = functionElement.childNodes();

         RealInterval::List domain;

        for (int i = 0; i < functionDataElements.at(4).toElement().childNodes().size(); i +=1)
        {
            QDomElement argumentElement = functionDataElements.at(4).toElement().childNodes().at(i).toElement();
            QDomNodeList argumentDataElement = argumentElement.childNodes();

            






            domain <<  RealInterval(argumentDataElement.at(1).toElement().text().toDouble(), 
                                            argumentDataElement.at(2).toElement().text().toDouble()); 
        }


         Function f(Analitza::Expression(functionDataElements.at(2).toElement().text(), false), 
                            functionDataElements.at(3).toElement().text().toInt(), 
                            domain,
                            m_dashboardWidget->space2D->variables(), 
                            functionDataElements.at(1).toElement().text(), 
                            QColor(functionDataElements.at(5).toElement().text())); 

        f.setSpaceId(QUuid(functionDataElements.at(0).toElement().text()));
        f.setDrawingType(( Function::DrawingType)(functionDataElements.at(10).toElement().text().toInt()));

        f.setShown(true);

        if (functionDataElements.at(6).toElement().text() == QString("false"))
            f.setShown(false);

        m_functionsModel->addFunction(f);








        functionElement = functionElement.nextSiblingElement();
    }

    m_file = file;*/

//     return true;
}

void Document::save()
{
//     if (!m_gplacsWidget->fileName().isEmpty())
//     {
//         if (m_gplacsWidget->save(m_gplacsWidget->fileName()))
//         {
//             updateTittleWhenOpenSaveDoc();
//         }
// 
//     }
//     else
//     {
//         QString filter = QString("*.gplacs|") + i18n("All Supported Files") +
//                          QString("\n*.gplacs|") + i18n("GPLACS standard file format") + QString(" (*.gplacs)");
// 
//         QString paletteFileName = KFileDialog::getSaveFileName(KUrl(QDir::homePath()), filter);
// 
//         if (KIO::NetAccess::exists(KUrl(paletteFileName), KIO::NetAccess::DestinationSide, widget()))
//             if (KMessageBox::warningContinueCancel(widget(), i18n("A file named \"%1\" already exists. Are you sure you want to overwrite it?", paletteFileName), QString(), KGuiItem(i18n("Overwrite"))) != KMessageBox::Continue)
//                 return;
// 
// 
//         if (m_gplacsWidget->save(paletteFileName))
//         {
//             updateTittleWhenOpenSaveDoc();
// 
//         }
// 
//     }


///




//TODO no se ha definido el formato aun


/*
    QFile device(m_fileName);

    if (!device.open(QFile::WriteOnly | QFile::Text))
        return false; 



    QDomDocument domDocument("gplacs");;

    QDomElement gplacsElement = domDocument.createElement("gplacs");

    QString text;

    
    QDomElement spacesElement = domDocument.createElement("spaces");


    for (int i = 0; i < m_spacesModel->rowCount(); i+=1)
    {
        QDomElement spaceElement = domDocument.createElement("space");

        QDomElement nameElement = domDocument.createElement("name");
        text = m_spacesModel->spaceFromIndex(i).name();
        QDomText textNameElement = domDocument.createTextNode(text);
        nameElement.appendChild(textNameElement);

        QDomElement descriptionElement = domDocument.createElement("description");
        text = m_spacesModel->spaceFromIndex(i).description();
        QDomText textDescriptionElement = domDocument.createTextNode(text);
        descriptionElement.appendChild(textDescriptionElement);

        QDomElement dimensionElement = domDocument.createElement("dimension");
        text = QString::number(m_spacesModel->spaceFromIndex(i).dimension());
        QDomText textDimensionElement = domDocument.createTextNode(text);
        dimensionElement.appendChild(textDimensionElement);

        QDomElement thumbnailElement = domDocument.createElement("thumbnail");
        text =  pixmapToUtf8(m_spacesModel->spaceFromIndex(i).thumbnail());
        QDomText textThumbnailElement = domDocument.createTextNode(text);
        thumbnailElement.appendChild(textThumbnailElement);

        QDomElement dateElement = domDocument.createElement("created");
        text = m_spacesModel->spaceFromIndex(i).dateTime().toString();
        QDomText textDateElement = domDocument.createTextNode(text);
        dateElement.appendChild(textDateElement);

        QDomElement idElement = domDocument.createElement("id");
        text = m_spacesModel->spaceFromIndex(i).id();
        QDomText textidElement = domDocument.createTextNode(text);
        idElement.appendChild(textidElement);

        spaceElement.appendChild(nameElement);
        spaceElement.appendChild(descriptionElement);
        spaceElement.appendChild(dimensionElement);
        spaceElement.appendChild(thumbnailElement);
        spaceElement.appendChild(dateElement);
        spaceElement.appendChild(idElement);

        spacesElement.appendChild(spaceElement);
    }


    



    QDomElement functionsElement = domDocument.createElement("functions");


    for (int i = 0; i < m_functionsModel->rowCount(); i+=1)
    {
        QDomElement functionElement = domDocument.createElement("function");

        QDomElement spaceIdElement = domDocument.createElement("spaceId");
        text = m_functionsModel->editFunction(i)->spaceId().toString();
        QDomText textspaceidElement = domDocument.createTextNode(text);
        spaceIdElement.appendChild(textspaceidElement);


        QDomElement nameElement = domDocument.createElement("name");
        text = m_functionsModel->editFunction(i)->name();
        QDomText textNameElement = domDocument.createTextNode(text);
        nameElement.appendChild(textNameElement);

        QDomElement lambdaElement = domDocument.createElement("lambda");
        text = m_functionsModel->editFunction(i)->lambda().toString();
        QDomText textLambdaElement = domDocument.createTextNode(text);
        lambdaElement.appendChild(textLambdaElement);

        QDomElement dimensionElement = domDocument.createElement("dimension");
        text = QString::number(m_functionsModel->editFunction(i)->dimension());
        QDomText textDimensionElement = domDocument.createTextNode(text);
        dimensionElement.appendChild(textDimensionElement);

        QDomElement argumentsElement = domDocument.createElement("arguments");


        for (int j = 0; j < m_functionsModel->editFunction(i)->lambda().bvarList().size(); j+=1)
        {
            QDomElement argumentElement = domDocument.createElement("argument");

            QDomElement argumentNameElement = domDocument.createElement("argumentName");
            text = m_functionsModel->editFunction(i)->lambda().bvarList().at(j);
            QDomText textArgumentNameElement = domDocument.createTextNode(text);
            argumentNameElement.appendChild(textArgumentNameElement);

            QDomElement lowerElement = domDocument.createElement("lower");
            text = QString::number(m_functionsModel->editFunction(i)->domain().at(j).lower());
            QDomText textLowerElement = domDocument.createTextNode(text);
            lowerElement.appendChild(textLowerElement);

            QDomElement upperElement = domDocument.createElement("upper");
            text = QString::number(m_functionsModel->editFunction(i)->domain().at(j).upper());
            QDomText textUpperElement = domDocument.createTextNode(text);
            upperElement.appendChild(textUpperElement);

            argumentElement.appendChild(argumentNameElement);
            argumentElement.appendChild(lowerElement);
            argumentElement.appendChild(upperElement);

            argumentsElement.appendChild(argumentElement);
        }

        QDomElement colorElement = domDocument.createElement("color");
        text = m_functionsModel->editFunction(i)->color().name();
        QDomText textColorElement = domDocument.createTextNode(text);
        colorElement.appendChild(textColorElement);

        QDomElement visibleElement = domDocument.createElement("visible");
        text = m_functionsModel->editFunction(i)->isShown()? "true" : "false";
        QDomText textVisibleElement = domDocument.createTextNode(text);
        visibleElement.appendChild(textVisibleElement);

        QDomElement resolutionElement = domDocument.createElement("resolution");
        text = QString::number(m_functionsModel->editFunction(i)->resolution());
        QDomText textResolutionElement = domDocument.createTextNode(text);
        resolutionElement.appendChild(textResolutionElement);

        QDomElement axeTypeWidthElement = domDocument.createElement("axeType");
        text = QString::number((int)(m_functionsModel->editFunction(i)->axeType()));
        QDomText textaxeTypeElement = domDocument.createTextNode(text);
        axeTypeWidthElement.appendChild(textaxeTypeElement);

        QDomElement lineWidthElement = domDocument.createElement("lineWidth");
        text = QString::number(m_functionsModel->editFunction(i)->lineWidth());
        QDomText textlineWidthElement = domDocument.createTextNode(text);
        lineWidthElement.appendChild(textlineWidthElement);

        QDomElement drawingTypeElement = domDocument.createElement("drawingType");
        text = QString::number((int)(m_functionsModel->editFunction(i)->drawingType()));
        QDomText textdrawingTypeElement = domDocument.createTextNode(text);
        drawingTypeElement.appendChild(textdrawingTypeElement);


        QDomElement dateElement = domDocument.createElement("created");
        text = m_functionsModel->editFunction(i)->dateTime().toString();
        QDomText textDateElement = domDocument.createTextNode(text);
        dateElement.appendChild(textDateElement);


        functionElement.appendChild(spaceIdElement);
        functionElement.appendChild(nameElement);
        functionElement.appendChild(lambdaElement);
        functionElement.appendChild(dimensionElement);
        functionElement.appendChild(argumentsElement);
        functionElement.appendChild(colorElement);
        functionElement.appendChild(visibleElement);
        functionElement.appendChild(resolutionElement);
        functionElement.appendChild(axeTypeWidthElement);
        functionElement.appendChild(lineWidthElement);
        functionElement.appendChild(drawingTypeElement);
        functionElement.appendChild(dateElement);

        functionsElement.appendChild(functionElement);
    }



    gplacsElement.appendChild(spacesElement);
    gplacsElement.appendChild(functionsElement);



    domDocument.appendChild(gplacsElement);

    QTextStream out(&device);


    QTextCodec *codec = QTextCodec::codecForName("UTF-8");

    out.setCodec(codec);

    domDocument.save(out, 4, QDomNode::EncodingFromTextStream);


    m_modified = false;
    m_saved = true;
    m_file = m_fileName;
*/
}

void Document::saveAs(const KUrl& fileUrl)
{
///emit
}

void Document::setCurrentSpace(int spaceidx)
{
//TODO aser  limites
    m_currentSpace = spaceidx;
}

void Document::setCurrentSpace(const QModelIndex & current, const QModelIndex & previous)
{
    if (current.isValid()) // assert
        m_currentSpace = current.row();
}

void Document::setCurrentSpace(const QItemSelection & selected, const QItemSelection & deselected)
{
    if (selected.indexes().first().isValid()) // assert
        m_currentSpace = selected.indexes().first().row();
}

void Document::mapPlot(const QModelIndex & parent, int start, int end)
{
    m_maps[m_currentSpace] = start;
}
//asrtos para verificar que no existan un plot asociado a mas de un space
void Document::unmapPlot(const QModelIndex& parent, int start, int end)
{
    for (int i = 0; i < m_maps.values().size(); ++i)
        if (m_maps.values().at(i) == start)
        {
            m_maps.remove(m_maps.key(start));
            
            break;
        }
}

QByteArray Document::pixmapToUtf8(const QPixmap &thumbnail) const
{
    QImage image = thumbnail.toImage();
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG"); 

    QByteArray cdata = qCompress(buffer.data(), 9).toHex();

    QString str(cdata);

    return str.toUtf8();
}

QPixmap Document::utf8ToPixmap(const QString &pixdata) const
{
    QByteArray readdata = qUncompress(QByteArray::fromHex(pixdata.toUtf8()));

    QBuffer rbuff(&readdata);
    rbuff.open(QIODevice::ReadOnly);

    QImage rimg;
    rimg.loadFromData(rbuff.data(), "PNG");

    return QPixmap::fromImage(rimg);
}
