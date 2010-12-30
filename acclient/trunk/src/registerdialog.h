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
// registerdialog.h: definition of the RegisterDialog class

#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>

#include "networkmanager.h"

namespace Ui {
    class RegisterDialog;
}

class RegisterDialog: public QDialog {
    Q_OBJECT

    public:
	  explicit RegisterDialog(const QString &host, int port, QWidget *parent=NULL);

    public slots:
	  void onNetError(int code);
	  void onNetConnected();
	  void onNetRegistrationResults(int code);

    protected:
	  virtual void accept();

    private:
	  QString m_Host;
	  int m_Port;

	  Ui::RegisterDialog *ui;

	  NetworkManager *m_Network;
};

#endif
