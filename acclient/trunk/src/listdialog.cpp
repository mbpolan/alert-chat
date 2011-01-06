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

ListDialog::ListDialog(const QStringList &usernames, QWidget *parent):
	  QDialog(parent), ui(new Ui::ListDialog) {
    ui->setupUi(this);

    // append all usernames into the list
    for (int i=0; i<usernames.size(); i++) {
	  QString entry=usernames.at(i);
	  QListWidgetItem *item=new QListWidgetItem(entry, ui->userList);
    }
}

QString ListDialog::selectedUsername() const {
    QListWidgetItem *item=ui->userList->currentItem();
    if (item)
	  return item->text();

    return "";
}
