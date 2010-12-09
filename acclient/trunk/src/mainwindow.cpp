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
// mainwindow.cpp: implementation of MainWindow class

#include <QMessageBox>

#include "logindialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // prepare the friend list with default items
    resetTreeView();

     // connect actions
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(onAbout()));
    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(onConnect()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(onDisconnect()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(onQuit()));
		
	// allocate the network manager
	m_Network=new NetworkManager(this);
	connect(m_Network, SIGNAL(authenticate()), this, SLOT(onNetAuth()));
	connect(m_Network, SIGNAL(connected()), this, SLOT(onNetConnected()));
	connect(m_Network, SIGNAL(disconnected()), this, SLOT(onNetDisconnected()));
	connect(m_Network, SIGNAL(message(QString,bool)), this, SLOT(onNetMessage(QString,bool)));
	connect(m_Network, SIGNAL(updateFriendList(QList<QString>)), this, SLOT(onNetUpdateFriendList(QList<QString>)));
	connect(m_Network, SIGNAL(updateUserStatus(QString,int)), this, SLOT(onNetUpdateUserStatus(QString,int)));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onConnect() {
	ui->actionConnect->setEnabled(false);
	ui->actionDisconnect->setEnabled(true);

	m_Network->connect("172.30.64.204", 9090);
}

void MainWindow::onDisconnect() {
	m_Network->disconnect();
}

void MainWindow::onNetAuth() {
	LoginDialog ld(this);
	if (ld.exec()==QDialog::Accepted) {
		m_User=ld.username();
		m_Network->performLogin(m_User, ld.password());
	}
	else
		m_Network->terminate();
}

void MainWindow::onNetConnected() {
    statusBar()->showMessage(tr("Connected to server"));

    setWindowTitle("Alert-Chat: "+m_User);
}

void MainWindow::onNetDisconnected() {
	statusBar()->showMessage(tr("Disconnected from server"));
	ui->actionConnect->setEnabled(true);
	ui->actionDisconnect->setEnabled(false);

	resetTreeView();

}

void MainWindow::onNetMessage(QString msg, bool passive) {
	if (passive)
		statusBar()->showMessage(msg);
	else {
		QMessageBox::information(this, tr("Message"), msg);

		// TODO: have the networkmanager flag a critical state
		ui->actionConnect->setEnabled(true);
		ui->actionDisconnect->setEnabled(false);
	}
}

void MainWindow::onNetUpdateFriendList(QList<QString> lst) {
    for (QList<QString>::iterator it=lst.begin(); it!=lst.end(); ++it) {
	  QStringList text;
	  text << (*it);

	  QTreeWidgetItem *item=new QTreeWidgetItem(ui->friendView->topLevelItem(1), text);
    }
}

void MainWindow::onNetUpdateUserStatus(QString username, int status) {
    // online or offline
    if (status==0 || status==1) {
	  QList<QTreeWidgetItem*> items=ui->friendView->findItems(username, Qt::MatchExactly | Qt::MatchRecursive);
	  if (items.empty())
		qDebug() << "User " << username << " not found!";

	  QTreeWidgetItem *item=items.first();
	  item->parent()->removeChild(item);

	  // repare this item based on the user's status
	  if (status==1)
		ui->friendView->topLevelItem(0)->addChild(item);
	  else
		ui->friendView->topLevelItem(1)->addChild(item);
    }

    // future: other types of user status (idle, away, etc)
}

void MainWindow::onQuit() {
    qApp->quit();
}

void MainWindow::onAbout() {

}

void MainWindow::resetTreeView() {
    ui->friendView->clear();

    // prepare the tree widget with predefined groups
    QStringList lst;
    lst << "Online";
    ui->friendView->addTopLevelItem(new QTreeWidgetItem(ui->friendView, lst));

    lst.clear();
    lst << "Offline";
    ui->friendView->addTopLevelItem(new QTreeWidgetItem(ui->friendView, lst));
}
