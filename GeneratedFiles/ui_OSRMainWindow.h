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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OSR
{
public:
    QAction *actionOpen;
    QAction *actionOpen_files;
    QAction *actionOpen_CD_Audio;
    QAction *actionClose_file;
    QAction *actionProject;
    QAction *actionFile;
    QAction *actionDecoder_parameters;
    QAction *actionClose;
    QAction *actionExit;
    QAction *actionCut;
    QAction *actionRedo;
    QAction *actionLow_pass_filter;
    QAction *actionHigh_pass_filter;
    QAction *actionCompare_audios;
    QAction *actionCopy_to_buffer;
    QAction *actionCopy_to_next_project;
    QAction *actionPaste_from_buffer;
    QAction *actionPaste_to_this_project;
    QAction *actionAll_files;
    QAction *actionOnly_this_file;
    QAction *actionPreferences;
    QWidget *centralWidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QOpenGLWidget *openGLWidget;
    QProgressBar *progressBar;
    QProgressBar *progressBar_2;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuNew;
    QMenu *menuEdit;
    QMenu *menuAudio_tools;
    QMenu *menuSelect;
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
        actionOpen = new QAction(OSR);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionOpen_files = new QAction(OSR);
        actionOpen_files->setObjectName(QStringLiteral("actionOpen_files"));
        actionOpen_CD_Audio = new QAction(OSR);
        actionOpen_CD_Audio->setObjectName(QStringLiteral("actionOpen_CD_Audio"));
        actionClose_file = new QAction(OSR);
        actionClose_file->setObjectName(QStringLiteral("actionClose_file"));
        actionProject = new QAction(OSR);
        actionProject->setObjectName(QStringLiteral("actionProject"));
        actionFile = new QAction(OSR);
        actionFile->setObjectName(QStringLiteral("actionFile"));
        actionDecoder_parameters = new QAction(OSR);
        actionDecoder_parameters->setObjectName(QStringLiteral("actionDecoder_parameters"));
        actionClose = new QAction(OSR);
        actionClose->setObjectName(QStringLiteral("actionClose"));
        actionExit = new QAction(OSR);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionCut = new QAction(OSR);
        actionCut->setObjectName(QStringLiteral("actionCut"));
        actionRedo = new QAction(OSR);
        actionRedo->setObjectName(QStringLiteral("actionRedo"));
        actionLow_pass_filter = new QAction(OSR);
        actionLow_pass_filter->setObjectName(QStringLiteral("actionLow_pass_filter"));
        actionHigh_pass_filter = new QAction(OSR);
        actionHigh_pass_filter->setObjectName(QStringLiteral("actionHigh_pass_filter"));
        actionCompare_audios = new QAction(OSR);
        actionCompare_audios->setObjectName(QStringLiteral("actionCompare_audios"));
        actionCopy_to_buffer = new QAction(OSR);
        actionCopy_to_buffer->setObjectName(QStringLiteral("actionCopy_to_buffer"));
        actionCopy_to_next_project = new QAction(OSR);
        actionCopy_to_next_project->setObjectName(QStringLiteral("actionCopy_to_next_project"));
        actionPaste_from_buffer = new QAction(OSR);
        actionPaste_from_buffer->setObjectName(QStringLiteral("actionPaste_from_buffer"));
        actionPaste_to_this_project = new QAction(OSR);
        actionPaste_to_this_project->setObjectName(QStringLiteral("actionPaste_to_this_project"));
        actionAll_files = new QAction(OSR);
        actionAll_files->setObjectName(QStringLiteral("actionAll_files"));
        actionOnly_this_file = new QAction(OSR);
        actionOnly_this_file->setObjectName(QStringLiteral("actionOnly_this_file"));
        actionPreferences = new QAction(OSR);
        actionPreferences->setObjectName(QStringLiteral("actionPreferences"));
        centralWidget = new QWidget(OSR);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(0, 390, 56, 53));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(verticalLayoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout->addWidget(label_2);

        label_3 = new QLabel(verticalLayoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout->addWidget(label_3);

        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout->addWidget(label);

        horizontalLayoutWidget = new QWidget(centralWidget);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(0, 0, 661, 391));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        openGLWidget = new QOpenGLWidget(horizontalLayoutWidget);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));

        horizontalLayout->addWidget(openGLWidget);

        progressBar = new QProgressBar(horizontalLayoutWidget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setValue(24);
        progressBar->setOrientation(Qt::Vertical);

        horizontalLayout->addWidget(progressBar);

        progressBar_2 = new QProgressBar(horizontalLayoutWidget);
        progressBar_2->setObjectName(QStringLiteral("progressBar_2"));
        progressBar_2->setValue(24);
        progressBar_2->setOrientation(Qt::Vertical);

        horizontalLayout->addWidget(progressBar_2);

        OSR->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(OSR);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1000, 21));
        menuBar->setMaximumSize(QSize(16777215, 16777213));
        QPalette palette1;
        QBrush brush4(QColor(247, 247, 247, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush4);
        palette1.setBrush(QPalette::Active, QPalette::AlternateBase, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush4);
        palette1.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush2);
        QBrush brush5(QColor(164, 166, 168, 96));
        brush5.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush5);
        palette1.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush2);
        menuBar->setPalette(palette1);
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuNew = new QMenu(menuFile);
        menuNew->setObjectName(QStringLiteral("menuNew"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        menuAudio_tools = new QMenu(menuEdit);
        menuAudio_tools->setObjectName(QStringLiteral("menuAudio_tools"));
        menuSelect = new QMenu(menuEdit);
        menuSelect->setObjectName(QStringLiteral("menuSelect"));
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
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Active, QPalette::Light, brush1);
        palette2.setBrush(QPalette::Active, QPalette::Dark, brush2);
        palette2.setBrush(QPalette::Active, QPalette::Text, brush);
        palette2.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette2.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette2.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Light, brush1);
        palette2.setBrush(QPalette::Inactive, QPalette::Dark, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Light, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::Dark, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        mainToolBar->setPalette(palette2);
        mainToolBar->setMovable(true);
        OSR->addToolBar(Qt::TopToolBarArea, mainToolBar);
        toolBar = new QToolBar(OSR);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        OSR->addToolBar(Qt::TopToolBarArea, toolBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuEffects->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuFile->addAction(menuNew->menuAction());
        menuFile->addSeparator();
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionOpen_files);
        menuFile->addAction(actionOpen_CD_Audio);
        menuFile->addSeparator();
        menuFile->addAction(actionClose_file);
        menuFile->addAction(actionClose);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuNew->addAction(actionProject);
        menuNew->addAction(actionFile);
        menuNew->addSeparator();
        menuNew->addAction(actionDecoder_parameters);
        menuEdit->addAction(actionCut);
        menuEdit->addAction(actionRedo);
        menuEdit->addSeparator();
        menuEdit->addAction(menuAudio_tools->menuAction());
        menuEdit->addSeparator();
        menuEdit->addAction(actionCopy_to_buffer);
        menuEdit->addAction(actionCopy_to_next_project);
        menuEdit->addAction(actionPaste_from_buffer);
        menuEdit->addAction(actionPaste_to_this_project);
        menuEdit->addSeparator();
        menuEdit->addAction(menuSelect->menuAction());
        menuEdit->addSeparator();
        menuEdit->addAction(actionPreferences);
        menuAudio_tools->addAction(actionLow_pass_filter);
        menuAudio_tools->addAction(actionHigh_pass_filter);
        menuAudio_tools->addSeparator();
        menuAudio_tools->addAction(actionCompare_audios);
        menuSelect->addAction(actionAll_files);
        menuSelect->addAction(actionOnly_this_file);
        mainToolBar->addSeparator();

        retranslateUi(OSR);

        QMetaObject::connectSlotsByName(OSR);
    } // setupUi

    void retranslateUi(QMainWindow *OSR)
    {
        OSR->setWindowTitle(QApplication::translate("OSR", "OpenSoundRefenition", nullptr));
        actionOpen->setText(QApplication::translate("OSR", "Open...", nullptr));
#ifndef QT_NO_SHORTCUT
        actionOpen->setShortcut(QApplication::translate("OSR", "Ctrl+O", nullptr));
#endif // QT_NO_SHORTCUT
        actionOpen_files->setText(QApplication::translate("OSR", "Open files...", nullptr));
#ifndef QT_NO_SHORTCUT
        actionOpen_files->setShortcut(QApplication::translate("OSR", "Ctrl+Shift+O", nullptr));
#endif // QT_NO_SHORTCUT
        actionOpen_CD_Audio->setText(QApplication::translate("OSR", "Open CD Audio...", nullptr));
#ifndef QT_NO_SHORTCUT
        actionOpen_CD_Audio->setShortcut(QApplication::translate("OSR", "Ctrl+Alt+O", nullptr));
#endif // QT_NO_SHORTCUT
        actionClose_file->setText(QApplication::translate("OSR", "Close file", nullptr));
#ifndef QT_NO_SHORTCUT
        actionClose_file->setShortcut(QApplication::translate("OSR", "Ctrl+W", nullptr));
#endif // QT_NO_SHORTCUT
        actionProject->setText(QApplication::translate("OSR", "Project...", nullptr));
#ifndef QT_NO_SHORTCUT
        actionProject->setShortcut(QApplication::translate("OSR", "Ctrl+Shift+N", nullptr));
#endif // QT_NO_SHORTCUT
        actionFile->setText(QApplication::translate("OSR", "File...", nullptr));
#ifndef QT_NO_SHORTCUT
        actionFile->setShortcut(QApplication::translate("OSR", "Ctrl+N", nullptr));
#endif // QT_NO_SHORTCUT
        actionDecoder_parameters->setText(QApplication::translate("OSR", "Decoder parameters...", nullptr));
#ifndef QT_NO_SHORTCUT
        actionDecoder_parameters->setShortcut(QApplication::translate("OSR", "Ctrl+Alt+K", nullptr));
#endif // QT_NO_SHORTCUT
        actionClose->setText(QApplication::translate("OSR", "Close all files", nullptr));
#ifndef QT_NO_SHORTCUT
        actionClose->setShortcut(QApplication::translate("OSR", "Ctrl+Alt+W", nullptr));
#endif // QT_NO_SHORTCUT
        actionExit->setText(QApplication::translate("OSR", "Exit", nullptr));
#ifndef QT_NO_SHORTCUT
        actionExit->setShortcut(QApplication::translate("OSR", "Ctrl+Shift+Q", nullptr));
#endif // QT_NO_SHORTCUT
        actionCut->setText(QApplication::translate("OSR", "Undo", nullptr));
#ifndef QT_NO_SHORTCUT
        actionCut->setShortcut(QApplication::translate("OSR", "Ctrl+Z", nullptr));
#endif // QT_NO_SHORTCUT
        actionRedo->setText(QApplication::translate("OSR", "Redo", nullptr));
#ifndef QT_NO_SHORTCUT
        actionRedo->setShortcut(QApplication::translate("OSR", "Ctrl+Shift+Z", nullptr));
#endif // QT_NO_SHORTCUT
        actionLow_pass_filter->setText(QApplication::translate("OSR", "Low-pass filter", nullptr));
        actionHigh_pass_filter->setText(QApplication::translate("OSR", "High-pass filter", nullptr));
        actionCompare_audios->setText(QApplication::translate("OSR", "Compare files...", nullptr));
        actionCopy_to_buffer->setText(QApplication::translate("OSR", "Copy to buffer", nullptr));
#ifndef QT_NO_SHORTCUT
        actionCopy_to_buffer->setShortcut(QApplication::translate("OSR", "Ctrl+C", nullptr));
#endif // QT_NO_SHORTCUT
        actionCopy_to_next_project->setText(QApplication::translate("OSR", "Copy to next project", nullptr));
#ifndef QT_NO_SHORTCUT
        actionCopy_to_next_project->setShortcut(QApplication::translate("OSR", "Ctrl+Shift+C", nullptr));
#endif // QT_NO_SHORTCUT
        actionPaste_from_buffer->setText(QApplication::translate("OSR", "Paste from buffer", nullptr));
#ifndef QT_NO_SHORTCUT
        actionPaste_from_buffer->setShortcut(QApplication::translate("OSR", "Ctrl+V", nullptr));
#endif // QT_NO_SHORTCUT
        actionPaste_to_this_project->setText(QApplication::translate("OSR", "Paste to this project", nullptr));
#ifndef QT_NO_SHORTCUT
        actionPaste_to_this_project->setShortcut(QApplication::translate("OSR", "Ctrl+Shift+V", nullptr));
#endif // QT_NO_SHORTCUT
        actionAll_files->setText(QApplication::translate("OSR", "All files...", nullptr));
        actionOnly_this_file->setText(QApplication::translate("OSR", "Only this file", nullptr));
        actionPreferences->setText(QApplication::translate("OSR", "Preferences...", nullptr));
#ifndef QT_NO_SHORTCUT
        actionPreferences->setShortcut(QApplication::translate("OSR", "Ctrl+K", nullptr));
#endif // QT_NO_SHORTCUT
        label_2->setText(QApplication::translate("OSR", "00:00:00", nullptr));
        label_3->setText(QApplication::translate("OSR", "44100 16 2", nullptr));
        label->setText(QApplication::translate("OSR", "FLAC", nullptr));
        menuFile->setTitle(QApplication::translate("OSR", "File", nullptr));
        menuNew->setTitle(QApplication::translate("OSR", "New", nullptr));
        menuEdit->setTitle(QApplication::translate("OSR", "Edit", nullptr));
        menuAudio_tools->setTitle(QApplication::translate("OSR", "Audio tools", nullptr));
        menuSelect->setTitle(QApplication::translate("OSR", "Select", nullptr));
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
