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
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onConnect() {

}

void MainWindow::onDisconnect() {

}

void MainWindow::onNetConnect() {
    statusBar()->showMessage(tr("Connected to server"));
}

void MainWindow::onNetDisconnected() {
    statusBar()->showMessage(tr("Disconnected from server"));
}

void MainWindow::onQuit() {
    qApp->quit();
}

void MainWindow::onAbout() {

}
