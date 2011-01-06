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
// mainwindow.h: definition of the MainWindow class

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QTreeWidgetItem>

#include "configloader.h"
#include "networkmanager.h"

namespace Ui {
    class MainWindow;
}

class MainWindow: public QMainWindow {
    Q_OBJECT

    public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

    private slots:
		void onTrayIconActivated(QSystemTrayIcon::ActivationReason);

		void onNewAccount();
		void onConnect();
		void onDisconnect();

		void onPreferences();

		void onAddFriend();
		void onRemoveFriend();

		void onFriendNameClicked(QTreeWidgetItem*, int);

		void onNetAuth();
		void onNetConnected();
		void onNetDisconnected();
		void onNetMessage(QString, bool);
		void onNetUpdateFriendList(QList<QString>);
		void onNetUpdateUserStatus(QString, int);
		void onNetTextMessage(QString, QString);

		void onQuit();
		void onAbout();

    private:
		void closeEvent(QCloseEvent *);

		void resetTreeView();

		ConfigLoader *m_Config;

		NetworkManager *m_Network;
		QString m_User;

		QSystemTrayIcon *m_Icon;
		
		Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
