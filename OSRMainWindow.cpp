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

VOID
OSR::on_actionOpen_triggered()
{
#ifdef WIN32
	WSTRING_PATH szPath = { NULL };
	OpenFileDialog(&szPath);
#else
	STRING_PATH szPath = { NULL };
	OpenFileDialog(&szPath);
#endif
}