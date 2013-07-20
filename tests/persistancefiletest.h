/*************************************************************************************
 *  Copyright (C) 2013 by Punit Mehta <punit9462@gmail.com>                          *
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

#ifndef PERSISTANCEFILETEST_H
#define PERSISTANCEFILETEST_H

#include <QtCore/QObject>

/**
    @author Punit Mehta
*/

class PersistanceFileTest : public QObject
{
Q_OBJECT
    public:
        PersistanceFileTest(QObject *parent = 0);
        ~PersistanceFileTest();

   private slots:
        void initTestCase();

        void testCorrect(); // correct dictionaries
        void testCorrect_data();

        void cleanupTestCase();

private:

};

#endif
