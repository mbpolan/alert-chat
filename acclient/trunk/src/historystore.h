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
// historystore.h: definition of the HistoryStore class

#ifndef HISTORYSTORE_H
#define HISTORYSTORE_H

#include <QList>
#include <QMap>
#include <QObject>

class HistoryStore: public QObject {
    Q_OBJECT
    public:
	  typedef QMap<QString, QStringList> DataMap;

	   HistoryStore(QObject *parent=NULL);

	  // adds a line of message to a user's saved history
	  void appendTextMessage(const QString &account, const QString &username, const QString &line);

	  // returns a list of all saved user histories for all accounts
	  DataMap savedHistories();

	  // returns the chat history for a particular user
	  QString userHistory(const QString &account, const QString &username);
};

#endif
