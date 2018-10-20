/*********************************************************
* Copyright (C) VERTVER, 2018. All rights reserved.
* OpenSoundRefenation - WINAPI open-source DAW
* MIT-License
**********************************************************
* Module Name: OSR entry-point
**********************************************************
* OSRMainWindowr.h
* UI implementation header
*********************************************************/
#pragma once

#include <QMainWindow>
#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QObject>
#include <QString>
#include <QtQml>
#include "OSR.h"

namespace Ui {
class OSR;
}

class OSR : public QMainWindow
{
    Q_OBJECT

public:
	VOID InitWindow();

    explicit OSR(QWidget *parent = 0);
    ~OSR();

private slots:
    void on_actionOpen_triggered();

private:
    Ui::OSR *ui;
};
