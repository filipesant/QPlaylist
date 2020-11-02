/********************************************************************************
** Form generated from reading UI file 'dialogcreateplaylist.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGCREATEPLAYLIST_H
#define UI_DIALOGCREATEPLAYLIST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_DialogCreatePlayList
{
public:
    QGridLayout *gridLayout;
    QGridLayout *gridLayout_2;
    QPushButton *pushButtonCreate;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonCancel;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *lineEdit;
    QLabel *label;

    void setupUi(QDialog *DialogCreatePlayList)
    {
        if (DialogCreatePlayList->objectName().isEmpty())
            DialogCreatePlayList->setObjectName(QString::fromUtf8("DialogCreatePlayList"));
        DialogCreatePlayList->resize(493, 127);
        DialogCreatePlayList->setMinimumSize(QSize(0, 0));
        DialogCreatePlayList->setMaximumSize(QSize(16777215, 16777215));
        gridLayout = new QGridLayout(DialogCreatePlayList);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(9, 9, 9, 9);
        pushButtonCreate = new QPushButton(DialogCreatePlayList);
        pushButtonCreate->setObjectName(QString::fromUtf8("pushButtonCreate"));
        pushButtonCreate->setMinimumSize(QSize(0, 35));
        pushButtonCreate->setMaximumSize(QSize(16777215, 35));

        gridLayout_2->addWidget(pushButtonCreate, 2, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 2, 0, 1, 1);

        pushButtonCancel = new QPushButton(DialogCreatePlayList);
        pushButtonCancel->setObjectName(QString::fromUtf8("pushButtonCancel"));
        pushButtonCancel->setMinimumSize(QSize(0, 35));
        pushButtonCancel->setMaximumSize(QSize(16777215, 35));

        gridLayout_2->addWidget(pushButtonCancel, 2, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 2, 3, 1, 1);

        lineEdit = new QLineEdit(DialogCreatePlayList);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setMinimumSize(QSize(0, 40));

        gridLayout_2->addWidget(lineEdit, 1, 0, 1, 4);

        label = new QLabel(DialogCreatePlayList);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        gridLayout_2->addWidget(label, 0, 0, 1, 4);


        gridLayout->addLayout(gridLayout_2, 2, 0, 1, 2);


        retranslateUi(DialogCreatePlayList);

        QMetaObject::connectSlotsByName(DialogCreatePlayList);
    } // setupUi

    void retranslateUi(QDialog *DialogCreatePlayList)
    {
        DialogCreatePlayList->setWindowTitle(QApplication::translate("DialogCreatePlayList", "Create Playlist", nullptr));
        pushButtonCreate->setText(QApplication::translate("DialogCreatePlayList", "Create", nullptr));
        pushButtonCancel->setText(QApplication::translate("DialogCreatePlayList", "Cancel", nullptr));
        label->setText(QApplication::translate("DialogCreatePlayList", "PlaylistName", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogCreatePlayList: public Ui_DialogCreatePlayList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGCREATEPLAYLIST_H
