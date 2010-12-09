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
// networkmanager.cpp: implementation of the NetworkManager class

#include "networkmanager.h"
#include "packet.h"
#include "protspec.h"

NetworkManager::NetworkManager(QObject *parent) : QObject(parent) {
    m_Socket=NULL;
}

NetworkManager::~NetworkManager() {
    if (m_Socket)
		delete m_Socket;
}

void NetworkManager::connect(const QString &host, int port) {
	if (m_Socket)
		delete m_Socket;
	
    // create the socket
    m_Socket=new QTcpSocket(this);

    // connect socket signals
    QObject::connect(m_Socket, SIGNAL(connected()), this, SLOT(onConnected()));
    QObject::connect(m_Socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
	QObject::connect(m_Socket, SIGNAL(readyRead()), this, SLOT(onDataReady()));
	QObject::connect(m_Socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
	QObject::connect(m_Socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onSocketState(QAbstractSocket::SocketState)));
	
	m_Socket->connectToHost(host, port);
}

void NetworkManager::performLogin(const QString &username, const QString &password) {
	Packet p;
	p.addByte(PROT_LOGIN);
	p.addString(username);
	p.addString(password);
	
	p.write(m_Socket);
}

void NetworkManager::disconnect() {
    m_Socket->disconnectFromHost();
}

void NetworkManager::terminate() {
	m_Socket->abort();
}

void NetworkManager::onConnected() {
	emit connected();
}

void NetworkManager::onDisconnected() {
	emit disconnected();
}

void NetworkManager::onSocketError(QAbstractSocket::SocketError error) {
	switch(error) {
		case QAbstractSocket::HostNotFoundError: {
			emit message("Error: unable to find host", false);
		} break;
		
		case QAbstractSocket::ConnectionRefusedError: {
			emit message("Error: connection refused", false);
		} break;
		
		default: {
			emit message(QString("Error code: %1").arg(error), false);
		} break;
	}
}

void NetworkManager::onSocketState(QAbstractSocket::SocketState state) {
	switch(state) {
		case QAbstractSocket::HostLookupState: {
			emit message("Looking up host...", true);
		} break;
		
		case QAbstractSocket::ConnectingState: {
			emit message("Connecting...", true);
		} break;
		
		case QAbstractSocket::ConnectedState: {
			emit message("Connected", true);
		} break;
		
		case QAbstractSocket::UnconnectedState: {
			emit message("Disconnected", true);
		} break;
		
		default: break;
	}
}

void NetworkManager::onDataReady() {
	Packet p;
	if (p.read(m_Socket))
		handlePacket(p);
	
	else {
		qDebug() << "Read corrupt data";
		disconnect();
	}
}

void NetworkManager::handlePacket(Packet &p) {
	uint8_t header=p.byte();
	switch(header) {
		// server requires authentication
		case PROT_REQAUTH: emit authenticate(); break;
		
		// text message from the server
		case PROT_CLIENTMSG: emit message(p.string(), false); break;

		// an updated friend list arrived
		case PROT_FRIENDLIST: serverSentFriendList(p); break;

		// a user status has changed
		case PROT_STATUSUPDATE: serverSentUserStatusUpdate(p); break;
		
		default: qDebug() << "Unknown header: " << header; break;
	}
}

void NetworkManager::serverSentFriendList(Packet &p) {
    // first is the length of the list
    int length=p.uint16();

    // parse the usernames
    QList<QString> friends;
    for (int i=0; i<length; i++)
	  friends.append(p.string());

    emit updateFriendList(friends);
}

void NetworkManager::serverSentUserStatusUpdate(Packet &p) {
    // username followed by status
    QString username=p.string();
    int status=p.uint16();

    emit updateUserStatus(username, status);
}
