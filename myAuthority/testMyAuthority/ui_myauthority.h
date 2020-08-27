/********************************************************************************
** Form generated from reading UI file 'myauthority.ui'
**
** Created by: Qt User Interface Compiler version 5.9.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYAUTHORITY_H
#define UI_MYAUTHORITY_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MyAuthority
{
public:
    QGridLayout *gridLayout_2;
    QSpacerItem *verticalSpacer;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *MyAuthority)
    {
        if (MyAuthority->objectName().isEmpty())
            MyAuthority->setObjectName(QStringLiteral("MyAuthority"));
        MyAuthority->resize(277, 186);
        gridLayout_2 = new QGridLayout(MyAuthority);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 1, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));

        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 1, 1, 1);


        retranslateUi(MyAuthority);

        QMetaObject::connectSlotsByName(MyAuthority);
    } // setupUi

    void retranslateUi(QWidget *MyAuthority)
    {
        MyAuthority->setWindowTitle(QApplication::translate("MyAuthority", "Form", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MyAuthority: public Ui_MyAuthority {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYAUTHORITY_H
