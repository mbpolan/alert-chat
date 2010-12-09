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
// networkmanager.h: definition of the NetworkManager class

#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QList>
#include <QObject>
#include <QTcpSocket>

#include "packet.h"

class NetworkManager : public QObject {
    Q_OBJECT
    public:
		explicit NetworkManager(QObject *parent=NULL);
		~NetworkManager();

		void connect(const QString &host, int port);
		void performLogin(const QString &username, const QString &password);
		void disconnect();
		
		void terminate();
	
    signals:
		void authenticate();
		void connected();
		void disconnected();
		void message(QString, bool);
		void updateFriendList(QList<QString>);
		void updateUserStatus(QString, int);
	
	private slots:
		void onConnected();
		void onDisconnected();
		void onSocketError(QAbstractSocket::SocketError);
		void onSocketState(QAbstractSocket::SocketState);
		void onDataReady();

    private:
		void handlePacket(Packet &p);
		void serverSentFriendList(Packet &p);
		void serverSentUserStatusUpdate(Packet &p);
		
		QTcpSocket *m_Socket;
};

#endif // NETWORKMANAGER_H
