/********************************************************************************
** Form generated from reading UI file 'propertieswindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROPERTIESWINDOW_H
#define UI_PROPERTIESWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_PropertiesWindow
{
public:
    QHBoxLayout *horizontalLayout;
    QSplitter *splitter;
    QTableView *tableView;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;

    void setupUi(QDialog *PropertiesWindow)
    {
        if (PropertiesWindow->objectName().isEmpty())
            PropertiesWindow->setObjectName(QStringLiteral("PropertiesWindow"));
        PropertiesWindow->setWindowModality(Qt::ApplicationModal);
        PropertiesWindow->resize(240, 310);
        PropertiesWindow->setAutoFillBackground(false);
        PropertiesWindow->setSizeGripEnabled(false);
        PropertiesWindow->setModal(true);
        horizontalLayout = new QHBoxLayout(PropertiesWindow);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        splitter = new QSplitter(PropertiesWindow);
        splitter->setObjectName(QStringLiteral("splitter"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy);
        splitter->setOrientation(Qt::Vertical);
        splitter->setOpaqueResize(false);
        splitter->setChildrenCollapsible(false);
        tableView = new QTableView(splitter);
        tableView->setObjectName(QStringLiteral("tableView"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(2);
        sizePolicy1.setHeightForWidth(tableView->sizePolicy().hasHeightForWidth());
        tableView->setSizePolicy(sizePolicy1);
        tableView->setMinimumSize(QSize(0, 200));
        QFont font;
        font.setPointSize(7);
        tableView->setFont(font);
        tableView->setAutoFillBackground(false);
        tableView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        tableView->setAutoScroll(true);
        tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableView->setProperty("showDropIndicator", QVariant(false));
        tableView->setDragDropOverwriteMode(false);
        tableView->setAlternatingRowColors(true);
        tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView->setTextElideMode(Qt::ElideRight);
        tableView->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
        tableView->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
        tableView->setShowGrid(true);
        tableView->setWordWrap(true);
        tableView->setCornerButtonEnabled(false);
        splitter->addWidget(tableView);
        tableView->horizontalHeader()->setHighlightSections(false);
        tableView->horizontalHeader()->setStretchLastSection(true);
        tableView->verticalHeader()->setVisible(false);
        tableView->verticalHeader()->setDefaultSectionSize(14);
        tableView->verticalHeader()->setHighlightSections(false);
        groupBox = new QGroupBox(splitter);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(1);
        sizePolicy2.setVerticalStretch(1);
        sizePolicy2.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy2);
        groupBox->setMinimumSize(QSize(0, 75));
        groupBox->setFont(font);
        groupBox->setAutoFillBackground(false);
        groupBox->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        groupBox->setFlat(false);
        groupBox->setCheckable(false);
        horizontalLayout_2 = new QHBoxLayout(groupBox);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(1);
        sizePolicy3.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy3);
        label->setMinimumSize(QSize(0, 0));
        label->setFont(font);
        label->setTextFormat(Qt::PlainText);
        label->setScaledContents(true);
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        label->setWordWrap(true);
        label->setMargin(1);
        label->setTextInteractionFlags(Qt::NoTextInteraction);

        horizontalLayout_2->addWidget(label);

        splitter->addWidget(groupBox);

        horizontalLayout->addWidget(splitter);


        retranslateUi(PropertiesWindow);

        QMetaObject::connectSlotsByName(PropertiesWindow);
    } // setupUi

    void retranslateUi(QDialog *PropertiesWindow)
    {
        PropertiesWindow->setWindowTitle(QApplication::translate("PropertiesWindow", "Properties", 0));
        groupBox->setTitle(QApplication::translate("PropertiesWindow", "Description", 0));
        label->setText(QApplication::translate("PropertiesWindow", "Information on the selected object and its value.", 0));
    } // retranslateUi

};

namespace Ui {
    class PropertiesWindow: public Ui_PropertiesWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROPERTIESWINDOW_H
