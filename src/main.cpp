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
#include <QApplication>
#include <KAboutData>
#include <KLocalizedString>
#include <QCommandLineParser>

//local includes
#include "mainwindow.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    KLocalizedString::setApplicationDomain("khipu");
    KAboutData about("khipu", i18n("Khipu"), "1.0", i18n("Advanced Mathematical Function Plotter"),
                     KAboutLicense::GPL, i18n("(C) 2010-2012, Percy Camilo Triveño Aucahuasi"));

    about.addAuthor(i18n("Percy Camilo Triveño Aucahuasi"), i18n("Main developer"), "percy.camilo.ta@gmail.com");

    about.addCredit(i18n("Punit Mehta"), i18n("GSoC-2013 student - Persistance file support. Plot-dictionary support. Worked for application actions, command-line improvements and space filtering. Several bug fixings"), "punit9462@gmail.com");
    about.addCredit(i18n("Manuel Álvarez Blanco"), i18n("Thesis mentor - Guide and supervision during project conception. Bibliographical support. Numeric Mathematics and Algorithms support"), "");
    about.addCredit(i18n("José Ignacio Cuevas Gonzáles"), i18n("Thesis mentor - Supervision, Product Guide, Product promotion and former Client"), "jose.cuevas@upc.edu.pe");
    about.addCredit(i18n("Eduardo Fernandini Capurro"), i18n("Thesis mentor - Supervision, Bibliographical Support, Product Guide and former Client"), "eduardo.fernandini@upc.edu.pe");
    about.addCredit(i18n("Jaime Urbina Pereyra"), i18n("Thesis mentor - Supervision and former Main Project Mentor"), "pcsijurb@upc.edu.pe");

    about.addCredit(i18n("Aleix Pol Gonzalez"), i18n("KAlgebra and Analitza parser author, both vitals for the project"));

    about.addCredit(i18n("José Fernando Ramos Ramirez"), i18n("First version of Famous Curves Database. Build former windows installer"), "ferramos1990@gmail.com");
    about.addCredit(i18n("Susan Pamela Rios Sarmiento"), i18n("First version of Famous Curves Database"), "susanriossarmiento@gmail.com");

    about.addCredit(i18n("Edgar Velasquez"), i18n("2D Improvements"));
    about.addCredit(i18n("Jose Torres Cardenas"), i18n("3D Improvements"));
    about.addCredit(i18n("Elizabeth Portilla Flores"), i18n("3D Improvements"));
    about.addCredit(i18n("Paul Murat Landauro Minaya"), i18n("3D Improvements"));

    KAboutData::setApplicationData(about);

    QCommandLineParser parser;
    about.setupCommandLine(&parser);
    parser.addPositionalArgument("urls", i18n("Khipu files to open"));
    parser.addOption(QCommandLineOption("ignoreautosavedfile", i18n("Always start with a new file, ignoring any autosaved file")));
    parser.process(app);
    about.processCommandLine(&parser);

    MainWindow *mainWindow = new MainWindow;

    if (app.isSessionRestored()) {
        RESTORE(MainWindow)
    } else {
        if (parser.positionalArguments().isEmpty()) {
            if (parser.isSet("ignoreautosavedfile")) {
                mainWindow->checkforAutoSavedFile();
            }
            mainWindow->show();
        } else {
            bool exit = false;
            for (const auto &urlString: parser.positionalArguments()) {
                if (!mainWindow->openFile(QUrl(urlString))) {
                    exit = true;
                    break;
                }
            }
            if (exit)
                mainWindow->deleteLater(); // can't open a khipu file, so just exit !
            else
                mainWindow->show();
        }
        parser.clearPositionalArguments();
    }
    return app.exec();
}
