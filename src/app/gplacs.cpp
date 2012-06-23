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


#include "gplacs.h"


#include <KDE/KApplication>


#include <QtGui/QDropEvent>
#include <QtGui/QPainter>
#include <QtGui/QPrinter>
#include <QtGui/QDockWidget>
#include <QtGui/QLayout>
#include <QtGui/QLabel>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsProxyWidget>
#include <QLineEdit>
#include <QDebug>
#include <KDE/KLocale>
#include <KDE/KLocalizedString>
#include <KDE/KStandardDirs>
#include <KDE/KAction>
#include <KDE/KActionCollection>
#include <KDE/KStandardAction>
#include <KDE/KStatusBar>
#include <KDE/KConfigDialog>
#include <KDE/KPageWidget>
#include <KDE/KPageModel>
#include <KDE/KPushButton>
#include <KDE/KFileDialog>
#include <KDE/KMessageBox>
#include <KIO/NetAccess>
#include <KDE/KMessageBox>
#include <KDE/KRecentFilesAction>
#include <KDE/KConfigDialog>
#include <KDE/KStandardDirs>
#include <KToolBar>
#include "keomath/function/functionsmodel.h"
#include "keomath/space/spacesmodel.h"
#include "widgets/gplacswidget.h"
#include "keomath/widgets/button.h"
#include <KAboutApplicationDialog>
#include <KMenuBar>
#include "widgets/dashboard.h"

//BEGIN fun

#include "analitza/variables.h"
#include "analitzagui/expressionedit.h"
#include "analitzaplot/function.h"
#include "analitzaplot/functionsmodel.h"
//END? fun :p


namespace GPLACS
{

///
class FunctionGraphEditor : public QWidget
{
public:
    //setmodel instead ... si quiero construir uno de estos sin modelo esta bien 
    //la clase padre no necesariamente puede teerner el modelo en el ctor y este le puede
    //pasar despues
    FunctionGraphEditor(FunctionGraphModel *model, QWidget *parent = 0);
    
public slots:
    void setup(const QModelIndex &index);
    void submit();

private:
    FunctionGraphModel *m_model;
    
    //TODO mas adelante + alla de funciones poner esto widget en un editor superior en la jerarquia
    class NameWidget;
    class ExpressionWidget;
    class FunctionExpressionWidget;
    class ArgumentsWidget;
    class CoordinateSystemWidget;
    class ColorWidget;
    class DrawingPrecisionWidget;
    
    NameWidget *m_nameWidget;
    FunctionExpressionWidget *m_functionExpressionWidget;
//     ArgumentsWidget *m_argumentsWidget;
//     CoordinateSystemWidget *m_;
//     ColorWidget *m_;
//     DrawingPrecisionWidget *m_;
};

class FunctionGraphEditor::NameWidget : public QWidget
{
public:
    NameWidget(QWidget *parent)
    :QWidget(parent)
    {
        m_name = new QLineEdit(this);
        
        QHBoxLayout *layout = new QHBoxLayout(this);
        layout->addWidget(new QLabel(i18n("Name:"), this));
        layout->addWidget(m_name);
    }
    
    QString name() const { return m_name->text(); }
    void setName(const QString &name) { m_name->setText(name); }

private:
    QLineEdit *m_name;
};

class FunctionGraphEditor::ExpressionWidget : public QWidget
{
public:
    ExpressionWidget(QWidget *parent)
    : QWidget(parent)
    {
        m_expressionEdit = new ExpressionEdit(this);
        m_label = new QLabel(this);
        
        QHBoxLayout *layout = new QHBoxLayout(this);
        layout->addWidget(m_label);
        layout->addWidget(m_expressionEdit);
    }

    void setLabel(const QString &label) { m_label->setText(label); }
    
    Analitza::Expression expression() const { return m_expressionEdit->expression(); }
    void setExpression(const Analitza::Expression &expression) { m_expressionEdit->setExpression(expression); }

private:
    QLabel *m_label;
    ExpressionEdit *m_expressionEdit;
};

class FunctionGraphEditor::FunctionExpressionWidget : public QWidget
{
public:
    FunctionExpressionWidget(QWidget *parent)
    : QWidget(parent)
    {
        QVBoxLayout *layout = new QVBoxLayout(this);

        //hide/show this widgets segun convenga
        for (int i = 1; i<= 3; i++)
        {
            ExpressionWidget *expwidget = new ExpressionWidget(this);
            m_expressionWidgets.append(expwidget);
            layout->addWidget(expwidget);
        }
    }
    
    void setExpressions(const QList<Analitza::Expression> &expressions) 
    {
        Q_ASSERT(expressions.size()>1); // no hay dim 0 para el conjunto de llegada
        
        switch (expressions.size()) //Rn->Rm or Rn->R"expressions.size()"
        {
            case 1:
            {
                setVisibilityForExpresions(2, false);
                
                m_expressionWidgets[0]->setLabel("("+expressions[0].bvarList().join(",")+")"); //TODO
                m_expressionWidgets[0]->setExpression(expressions[0]);

                break;
            }
            
//             case 2:
//             {
//                 setVisibilityForExpresions(2, false);
//                 
//                 m_expressionWidgets[0]->setLabel(expressions[0].bvarList()); //TODO
//                 m_expressionWidgets[0]->setExpression(expressions[0]);
// 
//                 break;
//             }
        }
    }
    
    void reset() { setVisibilityForExpresions(1, true); }

private:
    //TODO 3 que sea private static const de esta clase ... para que se diga su significado explicitamente
    void setVisibilityForExpresions(int n, bool visible) //set the Visibility since the n (n>=1 and n <= 3) expression
    {
        for (int i = n; i <= 3; ++i)
            m_expressionWidgets[i-1]->setVisible(visible);
    }

    QList<ExpressionWidget*> m_expressionWidgets;
};


FunctionGraphEditor::FunctionGraphEditor(FunctionGraphModel* model, QWidget* parent)
: QWidget(parent), m_model(model)
, m_nameWidget(new NameWidget(this))
, m_functionExpressionWidget(new FunctionExpressionWidget(this))
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_nameWidget);
    layout->addWidget(m_functionExpressionWidget);
    layout->addStretch();
}

void FunctionGraphEditor::setup(const QModelIndex& index)
{
    m_nameWidget->setName(m_model->data(index, FunctionGraphModel::NameRole).toString());

}

void FunctionGraphEditor::submit()
{
//model->setdata(...)
}

///

MainWindow::MainWindow()
    : KXmlGuiWindow()
{



    m_functionsModel = new Keomath::FunctionsModel(this);
    m_spacesModel = new Keomath::SpacesModel(this);

    m_gplacsWidget = new Dashboard(m_functionsModel, m_spacesModel, this);

    
    statusBar()->hide();
    setupActions();
    setupGUI();

    
    
    //BEGIN WIDGET TESTS
    
    Analitza::Variables *vars = new Analitza::Variables;
        
    PlaneCurveModel *model = new PlaneCurveModel(vars, this);
    
    model->addItem(Analitza::Expression("x->x*x"), "Hola", Qt::green);

    FunctionGraphEditor *feditor = new FunctionGraphEditor(model, this);
    feditor->setup(model->index(0));
    
    QTabWidget *w = new QTabWidget(this);
    w->addTab(m_gplacsWidget, "Main");
    w->addTab(feditor, "new func editor");
    w->setCurrentIndex(1);
    setCentralWidget(w);
    
    //END WIDGET TESTS


    updateTittleWhenOpenSaveDoc();


    connect(m_gplacsWidget, SIGNAL( modified() ), SLOT( updateTittleWhenChangeDocState() ));

    connect(m_gplacsWidget, SIGNAL(dashemitShowAppInfo()), SLOT(showAboutAppDialog()));

    menuBar()->hide();






}

MainWindow::~MainWindow()
{
}

void MainWindow::setupActions()
{





    KStandardAction::quit   (this, SLOT( close()      ), actionCollection());

    
    



    
    
    

    

    connect(m_gplacsWidget, SIGNAL(saveRequest()), SLOT(saveFile()));
    connect(m_gplacsWidget, SIGNAL(openRequest()), SLOT(openFile()));

}


bool MainWindow::queryClose()
{
    if (m_gplacsWidget->isModified())
    {
        QString paletteFileName = m_gplacsWidget->fileName();

        if (paletteFileName.isEmpty())
            paletteFileName = i18n("Untitled");

        switch (KMessageBox::warningYesNoCancel(this,
                                                i18n( "The document \"%1\" has been modified.\n"
                                                        "Do you want to save your changes or discard them?", paletteFileName),
                                                i18n( "Close Document" ), KStandardGuiItem::save(), KStandardGuiItem::discard()))
        {
        case KMessageBox::Yes:
        {
            
            
            
            
            
            m_gplacsWidget->showDashboard();

            saveFile();

            return m_gplacsWidget->isSaved();
        }
        case KMessageBox::No :
            return true;
            
        default :
            return false;
        }
    }

    return true;
}

void MainWindow::openFile()
{
    QString filter = QString("*.gplacs|") + i18n("All Supported Files") +
                     QString("\n*.gplacs|") + i18n("GPLACS standard file format") + QString(" (*.gplacs)");

    KUrl paletteUrl = KFileDialog::getOpenUrl(KUrl(), filter);



    QString paletteFileName;

    if (!paletteUrl.isEmpty())
    {
        if (KIO::NetAccess::download(paletteUrl, paletteFileName, this))
        {
            if (m_gplacsWidget->load(paletteFileName))
            {
                updateTittleWhenOpenSaveDoc();

                m_gplacsWidget->setModified(false);
                m_gplacsWidget->setSaved(true);
            }



            KIO::NetAccess::removeTempFile(paletteFileName);
        }
        else
            KMessageBox::error(this, KIO::NetAccess::lastErrorString());
    }
}

void MainWindow::showAboutAppDialog()
{
    const KAboutData *aboutData = KGlobal::mainComponent().aboutData();
    KAboutApplicationDialog a(aboutData, KAboutApplicationDialog::HideTranslators, this);
    a.exec();
}

void MainWindow::newFile()
{
    MainWindow * newWnd = new MainWindow();
    newWnd->show();
}

void MainWindow::saveFile()
{
    

    if (!m_gplacsWidget->fileName().isEmpty())
    {
        if (m_gplacsWidget->save(m_gplacsWidget->fileName()))
        {
            updateTittleWhenOpenSaveDoc();
        }

    }
    else
    {
        QString filter = QString("*.gplacs|") + i18n("All Supported Files") +
                         QString("\n*.gplacs|") + i18n("GPLACS standard file format") + QString(" (*.gplacs)");

        QString paletteFileName = KFileDialog::getSaveFileName(KUrl(QDir::homePath()), filter);

        if (KIO::NetAccess::exists(KUrl(paletteFileName), KIO::NetAccess::DestinationSide, widget()))
            if (KMessageBox::warningContinueCancel(widget(), i18n("A file named \"%1\" already exists. Are you sure you want to overwrite it?", paletteFileName), QString(), KGuiItem(i18n("Overwrite"))) != KMessageBox::Continue)
                return;


        if (m_gplacsWidget->save(paletteFileName))
        {
            updateTittleWhenOpenSaveDoc();

        }

    }
}

void MainWindow::saveFileAs(const QString &filename)
{






    
}


void MainWindow::updateInputTypePreviewImage(bool text_input)
{






}

void MainWindow::updateTittleWhenChangeDocState()
{
    QString paletteFileName = m_gplacsWidget->fileName();

    if (paletteFileName.isEmpty())
        paletteFileName = i18n("Untitled");

    setWindowTitle(QString("%1 - GPLACS " + i18n("[modificado]")).arg(paletteFileName));
}

void MainWindow::updateTittleWhenOpenSaveDoc()
{
    QString paletteFileName = m_gplacsWidget->fileName();

    if (paletteFileName.isEmpty())
        paletteFileName = i18n("Untitled");

    setWindowTitle(QString("%1 - GPLACS").arg(paletteFileName));
}

void MainWindow::optionsPreferences()
{
    
    
    
    
    

















}

} 
