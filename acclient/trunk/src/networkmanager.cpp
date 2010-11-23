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

NetworkManager::NetworkManager(QObject *parent) : QObject(parent) {
    m_Socket=NULL;
}

NetworkManager::~NetworkManager() {
    if (m_Socket)
	delete m_Socket;
}

void NetworkManager::connect(const QString &host, int port) {
    // create the socket
    m_Socket=new QTcpSocket(this);

    // connect socket signals
    QObject::connect(m_Socket, SIGNAL(connected()), this, SIGNAL(connected()));
    QObject::connect(m_Socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
}

void NetworkManager::performLogin(const QString &username, const QString &password) {

}

void NetworkManager::disconnect() {
    m_Socket->close();
}
