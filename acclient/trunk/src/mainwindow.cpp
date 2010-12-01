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
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onConnect() {
	ui->actionConnect->setEnabled(false);
	ui->actionDisconnect->setEnabled(true);
	m_Network->connect("127.0.0.1", 9090);
}

void MainWindow::onDisconnect() {
	m_Network->disconnect();
}

void MainWindow::onNetAuth() {
	LoginDialog ld(this);
	if (ld.exec()==QDialog::Accepted)
		m_Network->performLogin(ld.username(), ld.password());
	else
		m_Network->terminate();
}

void MainWindow::onNetConnected() {
    statusBar()->showMessage(tr("Connected to server"));
}

void MainWindow::onNetDisconnected() {
	statusBar()->showMessage(tr("Disconnected from server"));
	ui->actionConnect->setEnabled(true);
	ui->actionDisconnect->setEnabled(false);

}

void MainWindow::onNetMessage(QString msg, bool passive) {
	if (passive)
		statusBar()->showMessage(msg);
	else
		QMessageBox::information(this, tr("Message"), msg);
}

void MainWindow::onQuit() {
    qApp->quit();
}

void MainWindow::onAbout() {

}
