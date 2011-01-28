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
// listdialog.cpp: implementations of ListDialog class

#include <QListWidget>

#include "listdialog.h"
#include "ui_listdialog.h"

ListDialog::ListDialog(const HistoryStore::DataMap &data, QWidget *parent):
	  QDialog(parent), ui(new Ui::ListDialog) {
    ui->setupUi(this);
    m_ChatData=data;

    // fill the account combo box
    QStringList accounts;
    for (HistoryStore::DataMap::Iterator it=m_ChatData.begin(); it!=m_ChatData.end(); ++it)
	  accounts.push_back(it.key());

    ui->accountBox->addItems(accounts);

    if (!accounts.empty()) {
        ui->accountBox->setCurrentIndex(0);
        onAccountChanged(accounts[0]);
    }

    // connect signals
    connect(ui->accountBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(onAccountChanged(QString)));
}

void ListDialog::onAccountChanged(const QString &account) {
    QStringList usernames=m_ChatData[account];
    ui->userList->clear();

    // append all usernames into the list
    for (int i=0; i<usernames.size(); i++) {
	  QString entry=usernames.at(i);
	  QListWidgetItem *item=new QListWidgetItem(entry, ui->userList);
    }
}

QString ListDialog::selectedAccount() const {
    return ui->accountBox->currentText();
}

QString ListDialog::selectedUsername() const {
    QListWidgetItem *item=ui->userList->currentItem();
    if (item)
	  return item->text();

    return "";
}
