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
// preferencesdialog.cpp: implementation of the PreferencesDialog class

#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"

PreferencesDialog::PreferencesDialog(ConfigLoader *config, QWidget *parent) :
	  QDialog(parent), ui(new Ui::PreferencesDialog) {
    ui->setupUi(this);

    // synchronize the widgets with the data
    ui->server->setText(config->valueForKey("server"));
    ui->port->setValue(config->valueForKey("port").toInt());

    ui->saveChatHistory->setChecked(config->valueForKey("saveChatHistory").toInt());
    ui->promptUnknown->setChecked(config->valueForKey("promptUnknown").toInt());

    ui->enableSounds->setChecked(config->valueForKey("enableSounds").toInt());
}

PreferencesDialog::~PreferencesDialog() {
    delete ui;
}

void PreferencesDialog::synchronize(ConfigLoader *config) {
    config->clear();

    config->setValueForKey("server", ui->server->text());
    config->setValueForKey("port", QString("%1").arg(ui->port->value()));
    config->setValueForKey("saveChatHistory", QString("%1").arg(ui->saveChatHistory->isChecked()));
    config->setValueForKey("promptUnknown", QString("%1").arg(ui->promptUnknown->isChecked()));
    config->setValueForKey("enableSounds", QString("%1").arg(ui->enableSounds->isChecked()));
}
