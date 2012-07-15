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


#include "dashboard.h"
#include <QDomImplementation>

#include <QtGui/QLayout>
#include <QtGui/QStackedWidget>
#include <QtGui/QLabel>
#include <QtGui/QRadioButton>
#include <QDebug>
#include <KDE/KPushButton>
#include <KDE/KLineEdit>
#include <KDE/KComboBox>
#include <KFileDialog>
#include "functionsmodel.h"
#include "space.h"
#include "viewer2d.h"
#include "viewer3d.h"
#include "spacesmodel.h"
#include "analitza/variables.h"
#include "ui_dashboardwidget.h"
#include <QDomDocument>
#include <analitza/analyzer.h>
#include <analitzaplot/plotsview2d.h>
#include <QTextCodec>
#include <QBuffer>
namespace GPLACS
{



class DashboardWidget : public QStackedWidget, public Ui::DashboardWidget
{
public:
    DashboardWidget(QWidget *parent = 0)
        : QStackedWidget(parent)
    {
        setupUi(this);

        

        openFile->setIcon(KIcon("document-open"));
        saveFile->setIcon(KIcon("document-save"));


        addSpace2D->setIcon(KIcon("add-space2d"));
        addSpace3D->setIcon(KIcon("add-space3d"));
        backFromSpace2D->setIcon(KIcon("go-previous"));
        backFromSpace3D->setIcon(KIcon("go-previous"));

        filterAllSpaces->setIcon(KIcon("format-justify-fill"));

        filter2DSpaces->setIcon(KIcon("office-chart-line"));
        filter3DSpaces->setIcon(KIcon("office-chart-ring"));

        filterAllFunctions->setIcon(KIcon("format-justify-fill"));
        filter2DFunctions->setIcon(KIcon("office-chart-line"));
        filter3DFunctions->setIcon(KIcon("office-chart-ring"));

        showCoordSysSettings2D->setIcon(KIcon("configure"));
        showCoordSysSettings3D->setIcon(KIcon("configure"));

        showFunctionEditor2D->setIcon(KIcon("address-book-new"));
        showFunctionEditor3D->setIcon(KIcon("address-book-new"));



        showSpace2DInfo->setIcon(KIcon("document-edit"));
        showSpace3DInfo->setIcon(KIcon("document-edit"));

        saveSpace2DImage->setIcon(KIcon("view-preview"));
        saveSpace3DImage->setIcon(KIcon("view-preview"));

        copySpace2DImage->setIcon(KIcon("application-x-mswrite"));
        copySpace3DImage->setIcon(KIcon("application-x-mswrite"));

        showAboutAppDialog->setIcon(KIcon("help-about"));



        filterTextFunctions->setClearButtonShown(true);
        filterTextSpaces->setClearButtonShown(true);
    }
};






Dashboard::Dashboard(Keomath::FunctionsModel *functionsModel, Keomath::SpacesModel *spacesModel, QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent, f)
    , m_functionsModel(functionsModel)
    , m_spacesModel(spacesModel)
    , m_saved(false)
    , m_modified(false)
{
    setupWidget();

    m_spacesProxyModel = new Keomath::SpacesFilterProxyModel(this);
    m_spacesProxyModel->setSourceModel(m_spacesModel);
    m_spacesProxyModel->setCategorizedModel(true);











    m_functionsProxyModel = new Keomath::FunctionsFilterProxyModel(this);
    m_functionsProxyModel->setSourceModel(m_functionsModel);







    m_dashboardWidget->spaces->setModel(m_spacesProxyModel);
    m_dashboardWidget->functions->setModel(m_functionsProxyModel);

    m_dashboardWidget->functions->setIsMainFunctionsView(true);


    

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(m_dashboardWidget);




    m_dashboardWidget->spaces->setMainFunctionsModel(m_functionsModel);


    






    Analitza::Variables *vars = new Analitza::Variables;
    m_dashboardWidget->space2D->setVariables(vars);
    m_dashboardWidget->space3D->setVariables(vars);


    connect(m_dashboardWidget->openFile, SIGNAL(clicked()), SIGNAL(openRequest()));


    connect(m_dashboardWidget->saveFile, SIGNAL(clicked()), SIGNAL(saveRequest()));

}

Dashboard::~Dashboard()
{
}

void Dashboard::updateGPLACSDocument()
{
    m_modified = true;

    emit modified();
}


QByteArray Dashboard::pixmapToUtf8(const QPixmap &thumbnail) const
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



bool Dashboard::save(const QString &m_fileName)
{



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


    return true;

}

QPixmap Dashboard::utf8ToPixmap(const QString &pixdata) const
{



    




    QByteArray readdata = qUncompress(QByteArray::fromHex(pixdata.toUtf8()));

    QBuffer rbuff(&readdata);
    rbuff.open(QIODevice::ReadOnly);



    

    QImage rimg;
    rimg.loadFromData(rbuff.data(), "PNG");


    


    return QPixmap::fromImage(rimg);
}


bool Dashboard::load(const QString &file)
{

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

        Keomath::Space space(spaceDataElements.at(2).toElement().text().toInt());
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

        Keomath::RealInterval::List domain;

        for (int i = 0; i < functionDataElements.at(4).toElement().childNodes().size(); i +=1)
        {
            QDomElement argumentElement = functionDataElements.at(4).toElement().childNodes().at(i).toElement();
            QDomNodeList argumentDataElement = argumentElement.childNodes();

            






            domain << Keomath::RealInterval(argumentDataElement.at(1).toElement().text().toDouble(), 
                                            argumentDataElement.at(2).toElement().text().toDouble()); 
        }


        Keomath::Function f(Analitza::Expression(functionDataElements.at(2).toElement().text(), false), 
                            functionDataElements.at(3).toElement().text().toInt(), 
                            domain,
                            m_dashboardWidget->space2D->variables(), 
                            functionDataElements.at(1).toElement().text(), 
                            QColor(functionDataElements.at(5).toElement().text())); 

        f.setSpaceId(QUuid(functionDataElements.at(0).toElement().text()));
        f.setDrawingType((Keomath::Function::DrawingType)(functionDataElements.at(10).toElement().text().toInt()));

        f.setShown(true);

        if (functionDataElements.at(6).toElement().text() == QString("false"))
            f.setShown(false);

        m_functionsModel->addFunction(f);








        functionElement = functionElement.nextSiblingElement();
    }

    m_file = file;*/

    return true;
}

void Dashboard::showDashboard()
{
    
    if (m_dashboardWidget->currentIndex() == 2)
    {


        m_dashboardWidget->space3D->saveSpace();
    }
    else if (m_dashboardWidget->currentIndex() == 1) 
        m_dashboardWidget->space2D->saveSpace();



    m_dashboardWidget->setCurrentIndex(0);
}

void Dashboard::setupWidget()
{
    m_dashboardWidget = new DashboardWidget(this);

    m_proxyViewer2D = new Keomath::FunctionsFilterProxyModel(this);
    m_proxyViewer3D = new Keomath::FunctionsFilterProxyModel(this);

    m_proxyViewer2D->setSourceModel(m_functionsModel);
    m_proxyViewer3D->setSourceModel(m_functionsModel);

    m_dashboardWidget->space2D->setFunctionsModel(m_proxyViewer2D);
    m_dashboardWidget->space2D->setSpacesModel(m_spacesModel);

    m_dashboardWidget->space3D->setFunctionsModel(m_proxyViewer3D);
    m_dashboardWidget->space3D->setSpacesModel(m_spacesModel);


    connect(m_dashboardWidget->addSpace2D, SIGNAL(clicked()), SLOT(addSpace2D()));
    connect(m_dashboardWidget->addSpace3D, SIGNAL(clicked()), SLOT(addSpace3D()));
    connect(m_dashboardWidget->backFromSpace2D, SIGNAL(clicked()), SLOT(showDashboard()));
    connect(m_dashboardWidget->backFromSpace3D, SIGNAL(clicked()), SLOT(showDashboard()));

    






    connect(m_dashboardWidget->showFunctionEditor2D, SIGNAL(clicked()),
            m_dashboardWidget->space2D, SLOT(toggleShownFunctionEditor()));

    connect(m_dashboardWidget->showFunctionEditor3D, SIGNAL(clicked()),
            m_dashboardWidget->space3D, SLOT(toggleShownFunctionEditor()));

    connect(m_dashboardWidget->showCoordSysSettings2D, SIGNAL(clicked()),
            m_dashboardWidget->space2D, SLOT(toggleShownCoordSysSettings()));

    connect(m_dashboardWidget->showCoordSysSettings3D, SIGNAL(clicked()),
            m_dashboardWidget->space3D, SLOT(toggleShownCoordSysSettings()));







    connect(m_dashboardWidget->showSpace2DInfo, SIGNAL(clicked()),
            m_dashboardWidget->space2D, SLOT(toggleShownSpaceInfo()));

    connect(m_dashboardWidget->showSpace3DInfo, SIGNAL(clicked()),
            m_dashboardWidget->space3D, SLOT(toggleShownSpaceInfo()));













    connect(m_dashboardWidget->filterDimensionsSpaces, SIGNAL(clicked(int)), SLOT(setFilterDimension(int)));
    connect(m_dashboardWidget->filterDimensionsFunctions, SIGNAL(clicked(int)), SLOT(setFilterDimension(int)));

    connect(m_dashboardWidget->filterTextSpaces, SIGNAL(textChanged(QString)), SLOT(setFilterText(QString)));
    connect(m_dashboardWidget->filterTextFunctions, SIGNAL(textChanged(QString)), SLOT(setFilterText(QString)));




    connect(m_dashboardWidget->spaces, SIGNAL(spaceShown(Keomath::Space)), SLOT(showSpace(Keomath::Space)));


    connect(m_dashboardWidget->functions, SIGNAL(functionOnSpaceShown(QUuid)), SLOT(showFunctionOnSpace(QUuid)));






    connect(m_dashboardWidget->saveSpace2DImage, SIGNAL(clicked()), SLOT(saveSpace2DImage()));
    connect(m_dashboardWidget->saveSpace3DImage, SIGNAL(clicked()), SLOT(saveSpace3DImage()));

    connect(m_dashboardWidget->copySpace2DImage, SIGNAL(clicked()), SLOT(copySpace2DImage()));
    connect(m_dashboardWidget->copySpace3DImage, SIGNAL(clicked()), SLOT(copySpace3DImage()));









    connect(m_dashboardWidget->showAboutAppDialog, SIGNAL(clicked()), SIGNAL(dashemitShowAppInfo()));










    m_dashboardWidget->viewMode->setCurrentIndex(1);
    m_dashboardWidget->viewMode->setCurrentIndex(0);



}


void Dashboard::saveSpace2DImage()
{
    QString path = KFileDialog::getSaveFileName(KUrl(), i18n("*.png|PNG Image File\n*.svg|SVG File"), this);
    if(!path.isEmpty())
        m_dashboardWidget->space2D->toImage(path);
}

void Dashboard::saveSpace3DImage()
{
    QString path = KFileDialog::getSaveFileName(KUrl(), i18n("*.png|PNG Image File\n*.jpg|JPG Image File"), this);
    if(!path.isEmpty())
        m_dashboardWidget->space3D->toImage(path);
}

void Dashboard::copySpace2DImage()
{
    m_dashboardWidget->space2D->copyImageToClipboard();

}

void Dashboard::copySpace3DImage()
{
    m_dashboardWidget->space3D->copyImageToClipboard();
}

void Dashboard::setFilterText(const QString &text)
{
    switch (m_dashboardWidget->viewMode->currentIndex())
    {
    case 0: 
        m_spacesProxyModel->setFilterRegExp(QRegExp(m_dashboardWidget->filterTextSpaces->text(),
                                            Qt::CaseInsensitive, QRegExp::RegExp));
        break;

    case 1: 
        m_functionsProxyModel->setFilterRegExp(QRegExp(m_dashboardWidget->filterTextFunctions->text(),
                                               Qt::CaseInsensitive, QRegExp::RegExp));
        break;
    }
}

void Dashboard::setFilterDimension(int radioButton)
{
    switch (m_dashboardWidget->viewMode->currentIndex())
    {
    case 0: 
    {
        switch (radioButton)
        {
        case 0:
            m_spacesProxyModel->setFilterDimension(-1);
            break;
        case 1:
            m_spacesProxyModel->setFilterDimension(2);
            break;
        case 2:
            m_spacesProxyModel->setFilterDimension(3);
            break;
        }
    }
    break;

    case 1: 
    {
        switch (radioButton)
        {
        case 0:
            m_functionsProxyModel->setFilterDimension(-1);
            break;
        case 1:
            m_functionsProxyModel->setFilterDimension(2);
            break;
        case 2:
            m_functionsProxyModel->setFilterDimension(3);
            break;
        }
    }
    break;
    }
}

void Dashboard::showFunctionOnSpace(const QUuid &spaceId)
{

    Keomath::Space space = m_spacesModel->spaceFromId(spaceId);

    if (space.dimension() == 2)
    {
        m_dashboardWidget->setCurrentIndex(1);
        m_dashboardWidget->space2D->setSpace(space);

        m_proxyViewer2D->setFilterSpaceId(space.id());
    }
    else if (space.dimension() == 3)
    {
        m_dashboardWidget->setCurrentIndex(2);

        m_dashboardWidget->space3D->setSpace(space);

        m_proxyViewer3D->setFilterSpaceId(space.id());
    }

}

void Dashboard::showSpace(const Keomath::Space &space)
{
    if (space.dimension() == 2)
    {
        m_dashboardWidget->setCurrentIndex(1);
        m_dashboardWidget->space2D->setSpace(space);

        m_proxyViewer2D->setFilterSpaceId(space.id());

        
        
        
        m_dashboardWidget->space2D->view()->update();
    }
    else if (space.dimension() == 3)
    {
        m_dashboardWidget->setCurrentIndex(2);

        m_dashboardWidget->space3D->setSpace(space);

        m_proxyViewer3D->setFilterSpaceId(space.id());
    }
}

void Dashboard::addSpace2D()
{
    Keomath::Space space(2);


    m_dashboardWidget->setCurrentIndex(1);
    m_spacesModel->addSpace(space);

    m_dashboardWidget->space2D->setSpace(space);
    m_proxyViewer2D->setFilterSpaceId(space.id());

    updateGPLACSDocument();
}

void Dashboard::addSpace3D()
{
    Keomath::Space space(3);

    m_dashboardWidget->setCurrentIndex(2);
    m_spacesModel->addSpace(space);

    m_dashboardWidget->space3D->setSpace(space);

    m_proxyViewer3D->setFilterSpaceId(space.id());





    updateGPLACSDocument();

    

}

















































































} 
