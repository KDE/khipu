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

//KDE includes
#include <KDE/KApplication>
#include <KDE/KAboutData>
#include <KDE/KCmdLineArgs>
#include <KDE/KLocale>

//local includes
#include "mainwindow.h"

static const char description[] = I18N_NOOP("Advanced Mathematical Function Plotter");

static const char version[] = "1.0";

int main(int argc, char **argv)
{
    KAboutData about("khipu", "gplacs", ki18n(I18N_NOOP("Khipu")), version, ki18n(description),
                     KAboutData::License_GPL, ki18n("(C) 2010-2012, Percy Camilo Triveño Aucahuasi"));

    about.addAuthor(ki18n("Percy Camilo Triveño Aucahuasi"), ki18n("Main developer"), "percy.camilo.ta@gmail.com");

    about.addCredit(ki18n("Punit Mehta"), ki18n("GSoC-2013 student - Persistance file support. Plot-dictionary support. Worked for application actions, command-line improvements and space filtering. Several bug fixings"), "punit9462@gmail.com");
    about.addCredit(ki18n("Manuel Álvarez Blanco"), ki18n("Thesis mentor - Guide and supervision during project conception. Bibliographical support. Numeric Mathematics and Algorithms support"), "");
    about.addCredit(ki18n("José Ignacio Cuevas Gonzáles"), ki18n("Thesis mentor - Supervision, Product Guide, Product promotion and former Client"), "jose.cuevas@upc.edu.pe");
    about.addCredit(ki18n("Eduardo Fernandini Capurro"), ki18n("Thesis mentor - Supervision, Bibliographical Support, Product Guide and former Client"), "eduardo.fernandini@upc.edu.pe");
    about.addCredit(ki18n("Jaime Urbina Pereyra"), ki18n("Thesis mentor - Supervision and former Main Project Mentor"), "pcsijurb@upc.edu.pe");

    about.addCredit(ki18n("Aleix Pol Gonzalez"), ki18n("KAlgebra and Analitza parser author, both vitals for the project"));

    about.addCredit(ki18n("José Fernando Ramos Ramirez"), ki18n("First version of Famous Curves Database. Build former windows installer"), "ferramos1990@gmail.com");
    about.addCredit(ki18n("Susan Pamela Rios Sarmiento"), ki18n("First version of Famous Curves Database"), "susanriossarmiento@gmail.com");

    about.addCredit(ki18n("Edgar Velasquez"), ki18n("2D Improvements"));
    about.addCredit(ki18n("Jose Torres Cardenas"), ki18n("3D Improvements"));
    about.addCredit(ki18n("Elizabeth Portilla Flores"), ki18n("3D Improvements"));
    about.addCredit(ki18n("Paul Murat Landauro Minaya"), ki18n("3D Improvements"));

    KCmdLineArgs::init(argc, argv, &about);

    KCmdLineOptions options;
    options.add("+[URL]", ki18n( "A Khipu-file to open" ));
    KCmdLineArgs::addCmdLineOptions(options);

    KApplication app;

    MainWindow *mainWindow = new MainWindow;

    if (app.isSessionRestored()) {
        RESTORE(MainWindow)
    } else {
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
        if (args->count() == 0) {
            mainWindow->checkforAutoSavedFile();
            mainWindow->show();
        } else {
            int i = 0;
            bool exit = false;
            for (; i < args->count(); i++) {
                if (i==0) {
                    if(args->arg(0)!="ignoreautosavedfile"){
                        if (!(mainWindow->openFile(args->url(0).path())))
                            exit = true;
                    }
                }
                mainWindow->show();
            }
            if (exit)
                mainWindow->deleteLater(); // can't open a khipu file, so just exit !
        }
        args->clear();
    }
    return app.exec();
}
