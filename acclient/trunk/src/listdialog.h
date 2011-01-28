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
// listdialog.h: implementations of the ListDialog class

#ifndef LISTDIALOG_H
#define LISTDIALOG_H

#include <QDialog>
#include <QMap>
#include <QStringList>

#include "historystore.h"

namespace Ui {
    class ListDialog;
}

class ListDialog: public QDialog {
    Q_OBJECT
    public:
	  explicit ListDialog(const HistoryStore::DataMap &data, QWidget *parent=NULL);

	  QString selectedAccount() const;
	  QString selectedUsername() const;

    private slots:
	  void onAccountChanged(const QString &account);

    private:
	  HistoryStore::DataMap m_ChatData;

	  Ui::ListDialog *ui;
};

#endif
