/*********************************************************
* Copyright (C) VERTVER, 2018. All rights reserved.
* OpenSoundRefenation - WINAPI open-source DAW
* MIT-License
**********************************************************
* Module Name: OSR entry-point
**********************************************************
* OSRMainWindow.cpp
* UI implementation
*********************************************************/
#include "OSRMainWindowr.h"
#include "ui_OSRMainWindow.h"

OSR::OSR(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OSR)
{
    ui->setupUi(this);
}

OSR::~OSR()
{
    delete ui;
}

BackEnd::BackEnd(QObject *parent) :
	QObject(parent)
{
}

QString BackEnd::userName()
{
	return m_userName;
}

void BackEnd::setUserName(const QString &userName)
{
	if (userName == m_userName) { return; }

	m_userName = userName;
	emit userNameChanged();
}
