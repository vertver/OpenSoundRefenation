/********************************************************************************
** Form generated from reading UI file 'defaulscreen.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEFAULSCREEN_H
#define UI_DEFAULSCREEN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DefaulScreen
{
public:
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QOpenGLWidget *openGLWidget;
    QProgressBar *progressBar;
    QProgressBar *progressBar_2;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label;

    void setupUi(QWidget *DefaulScreen)
    {
        if (DefaulScreen->objectName().isEmpty())
            DefaulScreen->setObjectName(QStringLiteral("DefaulScreen"));
        DefaulScreen->resize(665, 464);
        horizontalLayoutWidget = new QWidget(DefaulScreen);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(0, 0, 661, 391));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
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

        verticalLayoutWidget = new QWidget(DefaulScreen);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(0, 390, 56, 53));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
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


        retranslateUi(DefaulScreen);

        QMetaObject::connectSlotsByName(DefaulScreen);
    } // setupUi

    void retranslateUi(QWidget *DefaulScreen)
    {
        DefaulScreen->setWindowTitle(QApplication::translate("DefaulScreen", "Form", nullptr));
        label_2->setText(QApplication::translate("DefaulScreen", "00:00:00", nullptr));
        label_3->setText(QApplication::translate("DefaulScreen", "44100 16 2", nullptr));
        label->setText(QApplication::translate("DefaulScreen", "FLAC", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DefaulScreen: public Ui_DefaulScreen {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEFAULSCREEN_H
