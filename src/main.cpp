/*************************************************************************************
 *  Copyright (C) 2010-2012 by Percy Camilo T. Aucahuasi <percy.camilo.ta@gmail.com> *
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


#include "mainwindow.h"

#include <KDE/KApplication>
#include <KDE/KAboutData>
#include <KDE/KCmdLineArgs>
#include <KDE/KLocale>
#include <KDE/KIcon>
#include <QStyleFactory>
#include <QDebug>
#include <QSplashScreen>
#include <QTimer>


#include <KStandardDirs>

static const char description[] = I18N_NOOP("Graficador de Curvas y Superficies Matemáticas");

static const char version[] = "RC 13";

int main(int argc, char **argv)
{
    KAboutData about("khipu", 0, ki18n(I18N_NOOP("Khipu")), version, ki18n(description),
                     KAboutData::License_GPL, ki18n("(C) 2010, Percy Camilo Triveño Aucahuasi"));

    about.addAuthor(ki18n("Percy Camilo Triveño Aucahuasi"), ki18n("Jefe de Proyecto y Desarrollador Principal"), "percy.camilo.ta@gmail.com");

    about.addCredit(ki18n("Manuel Álvarez Blanco"), ki18n("Asesor de Tesis - Guía y Supervisión durante la concepción del Proyecto, Apoyo Bibliográfico, Soporte en Matemática Numérica y Algoritmos (2009)"), "");
    about.addCredit(ki18n("José Ignacio Cuevas Gonzáles"), ki18n("Asesor de Tesis - Supervisión, Guía del Producto, Promoción del Proyecto dentro del Área de Ciencias y Cliente Principal (2009-2010)"), "jose.cuevas@upc.edu.pe");
    about.addCredit(ki18n("Eduardo Fernandini Capurro"), ki18n("Asesor de Tesis - Supervisión, Apoyo Bibliográfico, Guia del Producto y Cliente Principal (2009-2010)"), "eduardo.fernandini@upc.edu.pe");
    about.addCredit(ki18n("Jaime Urbina Pereyra"), ki18n("Asesor de Tesis - Supervisor y Mentor Principal del Proyecto (2009-2010)"), "pcsijurb@upc.edu.pe");

    about.addCredit(ki18n("Aleix Pol Gonzalez"), ki18n("Autor de KAlgera y el parser Analitza, ambos vitales para GPLACS"));

    about.addCredit(ki18n("José Fernando Ramos Ramirez"), ki18n("Construcción de la Base de Datos de Curvas Famosas y Desarrollo del Instalador"), "ferramos1990@gmail.com");
    about.addCredit(ki18n("Susan Pamela Rios Sarmiento"), ki18n("Construcción de la Base de Datos de Curvas Famosas y Traducción"), "susanriossarmiento@gmail.com");

    about.addCredit(ki18n("Bertha Nérida Triveño Aucahuasi"), ki18n("Diseño del Ícono de GPLACS"));
    about.addCredit(ki18n("Edgar Velasquez"), ki18n("Mejoras en GPLACS 2D"));
    about.addCredit(ki18n("Elizabeth Portilla Flores"), ki18n("Mejoras en GPLACS 3D"));
    about.addCredit(ki18n("Paul Murat Landauro Minaya"), ki18n("Mejoras en GPLACS 3D"));

    KCmdLineArgs::init(argc, argv, &about);

    KCmdLineOptions options;

    KCmdLineArgs::addCmdLineOptions(options);

    KApplication app;
    QCoreApplication::addLibraryPath("../lib/kde4/plugins/");
    QCoreApplication::addLibraryPath("./lib/kde4/plugins/");



    KGlobal::locale()->setLanguage("es", app.sessionConfig());

    QApplication::setStyle(QStyleFactory::create("Oxygen"));

    QApplication::setWindowIcon(KIcon("khipu"));










    GPLACS::MainWindow *mainWindow = new GPLACS::MainWindow;









    if (app.isSessionRestored())
    {
        RESTORE(GPLACS::MainWindow)
    }
    else
    {

        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
        if (args->count() == 0)
            mainWindow->show();
        else
        {
            int i = 0;
            for (; i < args->count(); i++)
            {
                mainWindow->show();
            }
        }
        args->clear();
    }



    return app.exec();
}
