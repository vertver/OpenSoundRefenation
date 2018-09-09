/********************************************************************************
** Form generated from reading UI file 'OSRMainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OSRMAINWINDOW_H
#define UI_OSRMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OSR
{
public:
    QAction *actionCreate;
    QWidget *centralWidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QWidget *tab_2;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuEffects;
    QMenu *menuView;
    QStatusBar *statusBar;
    QToolBar *mainToolBar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *OSR)
    {
        if (OSR->objectName().isEmpty())
            OSR->setObjectName(QStringLiteral("OSR"));
        OSR->resize(1000, 500);
        QPalette palette;
        QBrush brush(QColor(208, 208, 208, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(30, 30, 30, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush1);
        QBrush brush2(QColor(20, 20, 20, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush2);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette.setBrush(QPalette::Active, QPalette::Window, brush2);
        QBrush brush3(QColor(75, 91, 105, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush3);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush2);
        OSR->setPalette(palette);
        actionCreate = new QAction(OSR);
        actionCreate->setObjectName(QStringLiteral("actionCreate"));
        centralWidget = new QWidget(OSR);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(0, 0, 481, 321));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        tabWidget = new QTabWidget(verticalLayoutWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        QPalette palette1;
        QBrush brush4(QColor(27, 27, 27, 102));
        brush4.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipBase, brush4);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush4);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush2);
        tabWidget->setPalette(palette1);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        QPalette palette2;
        QBrush brush5(QColor(214, 214, 214, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush5);
        palette2.setBrush(QPalette::Active, QPalette::Midlight, brush2);
        palette2.setBrush(QPalette::Active, QPalette::Mid, brush2);
        QBrush brush6(QColor(190, 190, 190, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::HighlightedText, brush6);
        QBrush brush7(QColor(20, 20, 20, 102));
        brush7.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush5);
        palette2.setBrush(QPalette::Inactive, QPalette::Midlight, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Mid, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::HighlightedText, brush6);
        palette2.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Midlight, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Mid, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::HighlightedText, brush6);
        palette2.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        tab->setPalette(palette2);
        label = new QLabel(tab);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(9, 9, 176, 42));
        QFont font;
        font.setFamily(QStringLiteral("Proxima Nova"));
        font.setPointSize(26);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label_2 = new QLabel(tab);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(9, 57, 103, 16));
        QFont font1;
        font1.setFamily(QStringLiteral("Proxima Nova"));
        font1.setPointSize(10);
        label_2->setFont(font1);
        label_2->setStyleSheet(QLatin1String("font: 10pt \"Proxima Nova\";\n"
"color: rgb(9, 105, 231);"));
        label_3 = new QLabel(tab);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(9, 80, 110, 16));
        label_3->setFont(font1);
        label_3->setStyleSheet(QLatin1String("font: 10pt \"Proxima Nova\";\n"
"color: rgb(9, 105, 231);"));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::Midlight, brush2);
        palette3.setBrush(QPalette::Active, QPalette::Mid, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::Midlight, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::Mid, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::Midlight, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::Mid, brush2);
        tab_2->setPalette(palette3);
        tabWidget->addTab(tab_2, QString());

        verticalLayout->addWidget(tabWidget);

        OSR->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(OSR);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1000, 20));
        menuBar->setMaximumSize(QSize(16777215, 16777213));
        QPalette palette4;
        QBrush brush8(QColor(247, 247, 247, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette4.setBrush(QPalette::Active, QPalette::WindowText, brush8);
        palette4.setBrush(QPalette::Active, QPalette::Text, brush8);
        palette4.setBrush(QPalette::Active, QPalette::AlternateBase, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::WindowText, brush8);
        palette4.setBrush(QPalette::Inactive, QPalette::Text, brush8);
        palette4.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush2);
        QBrush brush9(QColor(164, 166, 168, 96));
        brush9.setStyle(Qt::SolidPattern);
        palette4.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette4.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette4.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush2);
        menuBar->setPalette(palette4);
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        menuEffects = new QMenu(menuBar);
        menuEffects->setObjectName(QStringLiteral("menuEffects"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QStringLiteral("menuView"));
        OSR->setMenuBar(menuBar);
        statusBar = new QStatusBar(OSR);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        OSR->setStatusBar(statusBar);
        mainToolBar = new QToolBar(OSR);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QPalette palette5;
        palette5.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette5.setBrush(QPalette::Active, QPalette::Light, brush1);
        palette5.setBrush(QPalette::Active, QPalette::Dark, brush2);
        palette5.setBrush(QPalette::Active, QPalette::Text, brush);
        palette5.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette5.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette5.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::Light, brush1);
        palette5.setBrush(QPalette::Inactive, QPalette::Dark, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::Light, brush1);
        palette5.setBrush(QPalette::Disabled, QPalette::Dark, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        mainToolBar->setPalette(palette5);
        mainToolBar->setMovable(true);
        OSR->addToolBar(Qt::TopToolBarArea, mainToolBar);
        toolBar = new QToolBar(OSR);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        OSR->addToolBar(Qt::TopToolBarArea, toolBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuEffects->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuFile->addAction(actionCreate);
        mainToolBar->addSeparator();

        retranslateUi(OSR);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(OSR);
    } // setupUi

    void retranslateUi(QMainWindow *OSR)
    {
        OSR->setWindowTitle(QApplication::translate("OSR", "OpenSoundRefenition", nullptr));
        actionCreate->setText(QApplication::translate("OSR", "Create...", nullptr));
#ifndef QT_NO_SHORTCUT
        actionCreate->setShortcut(QApplication::translate("OSR", "Ctrl+N", nullptr));
#endif // QT_NO_SHORTCUT
        label->setText(QApplication::translate("OSR", "Main menu", nullptr));
        label_2->setText(QApplication::translate("OSR", "Load latest project", nullptr));
        label_3->setText(QApplication::translate("OSR", "Record new sample", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("OSR", "Tab 1", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("OSR", "Tab 2", nullptr));
        menuFile->setTitle(QApplication::translate("OSR", "File", nullptr));
        menuEdit->setTitle(QApplication::translate("OSR", "Edit", nullptr));
        menuEffects->setTitle(QApplication::translate("OSR", "Effects", nullptr));
        menuView->setTitle(QApplication::translate("OSR", "View", nullptr));
        toolBar->setWindowTitle(QApplication::translate("OSR", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class OSR: public Ui_OSR {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OSRMAINWINDOW_H
