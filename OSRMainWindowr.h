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

#define QML_MAIN_UI L"QMainUIPreview.qml"

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

private:
    Ui::OSR *ui;
};

class BackEnd : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)

public:
	explicit BackEnd(QObject* pObjectParent = nullptr);

	QString userName();
	void setUserName(const QString &userName);

signals:
	void userNameChanged();

private:
	QString m_userName;
};
