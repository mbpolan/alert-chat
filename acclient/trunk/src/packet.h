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
// packet.h: definition of the Packet class

#ifndef PACKET_H
#define PACKET_H

#include <QObject>
#include <QTcpSocket>
#include <stdint.h>

#define PACKET_BUFFER_MAX	1024
#define PACKET_STRING_MAX	PACKET_BUFFER_MAX-2

class Packet: public QObject {
	Q_OBJECT
	
	public:
		Packet();
		
		void clear();
		
		void addByte(uint8_t n);
		void addUint16(uint16_t n);
		void addUint32(uint32_t n);
		void addString(const QString &str);
		
		uint8_t byte();
		uint16_t uint16();
		uint32_t uint32();
		QString string();
		
		bool read(QTcpSocket *sock);
		bool write(QTcpSocket *sock);
		
	private:
		uint8_t m_Buffer[PACKET_BUFFER_MAX];
		int m_Size;
		int m_Pos;
};		

#endif
