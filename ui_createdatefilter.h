/********************************************************************************
** Form generated from reading UI file 'createddatefilter.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CREATEDDATEFILTER_H
#define UI_CREATEDDATEFILTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_createdatefilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *morecheckBox;
    QCheckBox *lesscheckBox;
    QDateTimeEdit *moredateTimeEdit;
    QDateTimeEdit *lessdateTimeEdit;

    void setupUi(QWidget *createdatefilter)
    {
        if (createdatefilter->objectName().isEmpty())
            createdatefilter->setObjectName(QStringLiteral("createdatefilter"));
        createdatefilter->setWindowModality(Qt::ApplicationModal);
        createdatefilter->resize(311, 72);
        QFont font;
        font.setPointSize(8);
        createdatefilter->setFont(font);
        gridLayout = new QGridLayout(createdatefilter);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
        morecheckBox = new QCheckBox(createdatefilter);
        morecheckBox->setObjectName(QStringLiteral("morecheckBox"));

        gridLayout->addWidget(morecheckBox, 0, 0, 1, 1);

        lesscheckBox = new QCheckBox(createdatefilter);
        lesscheckBox->setObjectName(QStringLiteral("lesscheckBox"));

        gridLayout->addWidget(lesscheckBox, 2, 0, 1, 1);

        moredateTimeEdit = new QDateTimeEdit(createdatefilter);
        moredateTimeEdit->setObjectName(QStringLiteral("moredateTimeEdit"));

        gridLayout->addWidget(moredateTimeEdit, 0, 1, 1, 1);

        lessdateTimeEdit = new QDateTimeEdit(createdatefilter);
        lessdateTimeEdit->setObjectName(QStringLiteral("lessdateTimeEdit"));

        gridLayout->addWidget(lessdateTimeEdit, 2, 1, 1, 1);


        retranslateUi(createdatefilter);
        QObject::connect(lesscheckBox, SIGNAL(toggled(bool)), lessdateTimeEdit, SLOT(setEnabled(bool)));
        QObject::connect(morecheckBox, SIGNAL(toggled(bool)), moredateTimeEdit, SLOT(setEnabled(bool)));

        QMetaObject::connectSlotsByName(createdatefilter);
    } // setupUi

    void retranslateUi(QWidget *createdatefilter)
    {
        createdatefilter->setWindowTitle(QApplication::translate("createdatefilter", "Filter", 0));
        morecheckBox->setText(QApplication::translate("createdatefilter", "Show Items with date >", 0));
        lesscheckBox->setText(QApplication::translate("createdatefilter", "Show Items with date <", 0));
    } // retranslateUi

};

namespace Ui {
    class createdatefilter: public Ui_createdatefilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CREATEDDATEFILTER_H