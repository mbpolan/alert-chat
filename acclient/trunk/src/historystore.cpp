/***************************************************************************
 *   Copyright (C) 2010 by Michal Polan                                    *
 *   kanadakid@gmail.com                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
// historystore.cpp: implementation of the HistoryStore class

#include <QDesktopServices>
#include <QDir>
#include <QFile>

#include "historystore.h"

HistoryStore::HistoryStore(QObject *parent): QObject(parent) {
    // create our storage directory if needed
    QDir dir(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation));
    if (!dir.exists("Alert Chat Histories"))
	  dir.mkdir("Alert Chat Histories");
}

// adds a line of message to a user's saved history
void HistoryStore::appendTextMessage(const QString &username, const QString &line) {
    // open the target file, based on lowercase username
    QString path=QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    path+="/Alert Chat Histories/";
    path+=username.toLower();
    path+=".txt";

    QFile file(path);
    if (file.open(QIODevice::Append)) {
	  file.write(QString(line+"\n").toStdString().c_str());
	  file.close();
    }
}

QList<QString> HistoryStore::savedHistories() {
    // saved histories are stored as text files in the user's document store location,
    // under the Alert Chat Histories directory
    QDir dir(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation));
    dir.cd("Alert Chat Histories");

    // pull all file names, ending with .txt
    QStringList filters;
    filters << "*.txt";
    return dir.entryList(filters);
}

QString HistoryStore::userHistory(const QString &username) {
    // open the target file, based on lowercase username
    QString path=QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    path+="/";
    path+=username.toLower();
    path+=".txt";

    QString contents="";
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
	  contents=file.readAll();
	  file.close();
    }

    return contents;
}
