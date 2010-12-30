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
// registerdialog.cpp: implementation of the RegisterDialog class

#include <QMessageBox>

#include "registerdialog.h"
#include "ui_registerdialog.h"

RegisterDialog::RegisterDialog(const QString &host, int port, QWidget *parent) :
	  QDialog(parent), ui(new Ui::RegisterDialog) {
    m_Host=host;
    m_Port=port;

    ui->setupUi(this);

    m_Network=new NetworkManager(NetworkManager::Register, this);

    connect(m_Network, SIGNAL(connected()), this, SLOT(onNetConnected()));
    connect(m_Network, SIGNAL(networkError(int)), this, SLOT(onNetError(int)));
    connect(m_Network, SIGNAL(registrationResult(int)), this, SLOT(onNetRegistrationResults(int)));
}

void RegisterDialog::onNetError(int code) {
    if (code!=NetworkManager::UnknownError)
	  QMessageBox::critical(this, tr("Network Error"), tr("Unable to connect to the server."));

    setEnabled(true);
}

void RegisterDialog::onNetConnected() {
    // send account details
    m_Network->createAccount(ui->firstNameEdit->text(),
				     ui->lastNameEdit->text(),
				     ui->locationEdit->text(),
				     ui->usernameEdit->text(),
				     ui->passwordEdit->text());
}

void RegisterDialog::onNetRegistrationResults(int code) {
    if (code!=NetworkManager::NoError) {
	  QMessageBox::critical(this, tr("Error"), tr("Unable to create your new account: the username is already taken."));
	  setEnabled(true);

	  m_Network->disconnect();
    }

    else {
	  QMessageBox::information(this, tr("Success!"), tr("Your new account has been created."));
	  QDialog::accept();
    }
}

void RegisterDialog::accept() {
    /** FIXME: this stuff should really be handled on the server level **/
    // usernames must be at least 3 characters long
    if (ui->usernameEdit->text().length()<3) {
	  QMessageBox::critical(this, tr("Error"), tr("Usernames cannot be less than 3 characters."));
	  return;
    }

    // passwords cannot be empty
    else if (ui->passwordEdit->text().isEmpty()) {
	  QMessageBox::critical(this, tr("Error"), tr("Your password cannot be empty."));
	  return;
    }

    // make sure passwords match
    else if (ui->passwordEdit->text()!=ui->confirmEdit->text()) {
	  QMessageBox::critical(this, tr("Error"), tr("The given passwords do not match."));
	  return;
    }
    /** end FIXME section **/

    // disable dialog buttons
    setEnabled(false);

    // connect to server and attempt to register the account
    m_Network->connect(m_Host, m_Port);
}
