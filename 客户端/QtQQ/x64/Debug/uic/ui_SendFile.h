/********************************************************************************
** Form generated from reading UI file 'SendFile.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SENDFILE_H
#define UI_SENDFILE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SendFile
{
public:
    QWidget *titleWidget;
    QWidget *bodyWiget;
    QPushButton *openBtn;
    QPushButton *sendBtn;
    QLineEdit *lineEdit;

    void setupUi(QWidget *SendFile)
    {
        if (SendFile->objectName().isEmpty())
            SendFile->setObjectName(QStringLiteral("SendFile"));
        SendFile->resize(519, 169);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SendFile->sizePolicy().hasHeightForWidth());
        SendFile->setSizePolicy(sizePolicy);
        SendFile->setMinimumSize(QSize(519, 169));
        SendFile->setMaximumSize(QSize(519, 619));
        titleWidget = new QWidget(SendFile);
        titleWidget->setObjectName(QStringLiteral("titleWidget"));
        titleWidget->setGeometry(QRect(0, 0, 519, 50));
        titleWidget->setProperty("titleskin", QVariant(true));
        bodyWiget = new QWidget(SendFile);
        bodyWiget->setObjectName(QStringLiteral("bodyWiget"));
        bodyWiget->setGeometry(QRect(0, 50, 519, 119));
        bodyWiget->setProperty("bottomskin", QVariant(true));
        openBtn = new QPushButton(bodyWiget);
        openBtn->setObjectName(QStringLiteral("openBtn"));
        openBtn->setGeometry(QRect(0, 30, 93, 28));
        sendBtn = new QPushButton(bodyWiget);
        sendBtn->setObjectName(QStringLiteral("sendBtn"));
        sendBtn->setGeometry(QRect(420, 70, 93, 28));
        lineEdit = new QLineEdit(bodyWiget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(110, 30, 301, 28));

        retranslateUi(SendFile);

        QMetaObject::connectSlotsByName(SendFile);
    } // setupUi

    void retranslateUi(QWidget *SendFile)
    {
        SendFile->setWindowTitle(QApplication::translate("SendFile", "SendFile", Q_NULLPTR));
        openBtn->setText(QApplication::translate("SendFile", "\346\211\223\345\274\200...", Q_NULLPTR));
        sendBtn->setText(QApplication::translate("SendFile", "\345\217\221\351\200\201", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SendFile: public Ui_SendFile {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SENDFILE_H
