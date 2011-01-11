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

#include <QCloseEvent>
#include <QDateTime>
#include <QDir>
#include <QDesktopServices>
#include <QIcon>
#include <QInputDialog>
#include <QMessageBox>

#include "historyviewer.h"
#include "listdialog.h"
#include "logindialog.h"
#include "mainwindow.h"
#include "preferencesdialog.h"
#include "registerdialog.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // load the configuration file
    m_Config=new ConfigLoader();
    m_Config->loadConfiguration(QDir::currentPath()+"/client.conf");

    // prepare the friend list with default items
    resetTreeView();

     // connect actions
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(onAbout()));
    connect(ui->actionNew_Account, SIGNAL(triggered()), this, SLOT(onNewAccount()));
    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(onConnect()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(onDisconnect()));
    connect(ui->actionPreferences, SIGNAL(triggered()), this, SLOT(onPreferences()));
    connect(ui->actionAdd_By_Name, SIGNAL(triggered()), this, SLOT(onAddFriend()));
    connect(ui->actionRemove, SIGNAL(triggered()), this, SLOT(onRemoveFriend()));
    connect(ui->actionBlock_Unblock, SIGNAL(triggered()), this, SLOT(onBlockUser()));
    connect(ui->actionView_History, SIGNAL(triggered()), this, SLOT(onViewHistory()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(onQuit()));
		
    // allocate the network manager
    m_Network=new NetworkManager(NetworkManager::User, this);
    connect(m_Network, SIGNAL(authenticate()), this, SLOT(onNetAuth()));
    connect(m_Network, SIGNAL(connected()), this, SLOT(onNetConnected()));
    connect(m_Network, SIGNAL(disconnected()), this, SLOT(onNetDisconnected()));
    connect(m_Network, SIGNAL(message(QString,bool)), this, SLOT(onNetMessage(QString,bool)));
    connect(m_Network, SIGNAL(updateFriendList(QList<QString>)), this, SLOT(onNetUpdateFriendList(QList<QString>)));
    connect(m_Network, SIGNAL(updateUserStatus(QString,int)), this, SLOT(onNetUpdateUserStatus(QString,int)));
    connect(m_Network, SIGNAL(textMessage(QString,QString)), this, SLOT(onNetTextMessage(QString,QString)));

    // connect treeview actions
    connect(ui->friendView, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
		this, SLOT(onFriendNameClicked(QTreeWidgetItem*,int)));

    // create the tray icon
    m_Icon=new QSystemTrayIcon(QIcon(":/icons/icon16.png"), this);
    m_Icon->show();

    QMenu *trayMenu=new QMenu;
    QAction *quit=new QAction(tr("Quit"), trayMenu);

    trayMenu->addAction(quit);
    m_Icon->setContextMenu(trayMenu);

    // connect tray icon signals
    connect(m_Icon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		this, SLOT(onTrayIconActivated(QSystemTrayIcon::ActivationReason)));
    connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()));

    // allocate history storage
    m_HistStore=new HistoryStore(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason) {
    if (reason==QSystemTrayIcon::Trigger)
	  show();
}

void MainWindow::onNewAccount() {
    RegisterDialog rd(m_Config->valueForKey("server"),
			    m_Config->valueForKey("port").toInt(), this);
    rd.exec();
}

void MainWindow::onConnect() {
	ui->actionConnect->setEnabled(false);
	ui->actionDisconnect->setEnabled(true);

	m_Network->connect(m_Config->valueForKey("server"),
				 m_Config->valueForKey("port").toInt());
}

void MainWindow::onDisconnect() {
	m_Network->disconnect();
}

void MainWindow::onPreferences() {
    PreferencesDialog pd(m_Config, this);
    if (pd.exec()==QDialog::Accepted) {
	  // synchornize new preferences to internal configuration and save it
	  pd.synchronize(m_Config);
	  m_Config->saveConfiguration(QDir::currentPath()+"/client.conf");
    }
}

void MainWindow::onAddFriend() {
    bool ok;
    QString username=QInputDialog::getText(this, tr("Add Friend"),
							 tr("Username"), QLineEdit::Normal, QString(), &ok);

    if (ok && !username.isEmpty())
	  m_Network->sendAddFriend(username);
}

void MainWindow::onRemoveFriend() {
    if (ui->friendView->currentItem() &&
	  ui->friendView->currentItem()->parent()) {
	  // grab the currently selected item, the username it holds, and delete it
	  QTreeWidgetItem *item=ui->friendView->currentItem();
	  QString username=item->text(0);

	  item->parent()->removeChild(item);
	  delete item;

	  // tell the server to remove this username from the client's friendlist
	  m_Network->sendRemoveFriend(username);
    }
}

void MainWindow::onBlockUser() {
    if (ui->friendView->currentItem()) {
	  QTreeWidgetItem *item=ui->friendView->currentItem();
	  QString user=item->text(0);

	  if (m_BlockedUsers.contains(user))
		m_Network->sendUnblockUser(user);
	  else
		m_Network->sendBlockUser(user);
    }
}

void MainWindow::onViewHistory() {
    // request all available history files
    ListDialog ld(m_HistStore->savedHistories(), this);
    if (ld.exec()==QDialog::Accepted) {
	  QString username=ld.selectedUsername();

	  // if the chosen username is valid, show the chat history for that user
	  if (!username.isEmpty()) {
		HistoryViewer hv(m_HistStore->userHistory(username), this);
		hv.exec();
	  }
    }
}

void MainWindow::onFriendNameClicked(QTreeWidgetItem *item, int) {
    if (item && item->text(0)!="Online" && item->text(0)!="Offline") {
	  // open a dialog window for this conversation
	  bool ok;
	  QString text=QInputDialog::getText(this, tr("Send Message"), tr("Message"), QLineEdit::Normal, QString(), &ok);

	  if (ok && !text.isEmpty()) {
		m_Network->sendTextMessage(item->text(0), text);

		// also save this in the chat history, if so requested
		if (m_Config->valueForKey("saveChatHistory").toInt()) {
		    QString line="[";
		    line+=QDateTime::currentDateTime().toString(Qt::ISODate);
		    line+="] ";
		    line+=m_User;
		    line+=": ";
		    line+=text;

		    m_HistStore->appendTextMessage(item->text(0), line);
		}
	  }
    }
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

	setWindowTitle("Alert-Chat");
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
   // try to find the user's list entry
    QList<QTreeWidgetItem*> items=ui->friendView->findItems(username, Qt::MatchExactly | Qt::MatchRecursive);

    // if this user is not on the list, make sure to create an entry for him
    QTreeWidgetItem *item;
    if (items.empty()) {
	  item=new QTreeWidgetItem;
	  item->setText(0, username);
    }

    else {
	  // remove the item from the list
	  item=items.first();
	  item->parent()->removeChild(item);
    }

    // now find the appropriate category to classify this user under
    QTreeWidgetItem *parent;
    switch(status) {
	  case NetworkManager::Offline: parent=ui->friendView->topLevelItem(1); break;
	  case NetworkManager::Online: parent=ui->friendView->topLevelItem(0); break;
	  case NetworkManager::Blocked: parent=ui->friendView->topLevelItem(2); break;
	  default: parent=NULL; break;
    }

    if (parent)
	  parent->addChild(item);
    else
	  qDebug() << "Unknown user status: " << status;
}

void MainWindow::onNetTextMessage(QString sender, QString text) {
    // see if this user is known, and if not, then see if the user wants a prompt beforehand
    QList<QTreeWidgetItem*> items=ui->friendView->findItems(sender, Qt::MatchExactly | Qt::MatchRecursive);
    if (items.empty() && m_Config->valueForKey("promptUnknown").toInt()==1) {
	  if (QMessageBox::question(this, tr("Unknown User"),
					    QString(tr("An unknown user (%1) has sent you a message. Accept?").arg(sender)),
					    QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes)
		QMessageBox::information(this, QString("Unknown user message: %1").arg(sender), text);
    }

    else
	  QMessageBox::information(this, QString("Message from: %1").arg(sender), text);

    // if chat history saving is enabled, save this message
    if (m_Config->valueForKey("saveChatHistory").toInt()) {
	  // format: [date and time] sender: message
	  QString line="[";
	  line+=QDateTime::currentDateTime().toString(Qt::ISODate);
	  line+="] ";
	  line+=sender;
	  line+=": ";
	  line+=text;

	  m_HistStore->appendTextMessage(sender, line);
    }
}

void MainWindow::onQuit() {
    qApp->quit();
}

void MainWindow::onAbout() {

}

void MainWindow::closeEvent(QCloseEvent *e) {
    hide();
    e->ignore();
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

    lst.clear();
    lst << "Blocked";
    ui->friendView->addTopLevelItem(new QTreeWidgetItem(ui->friendView, lst));
}
