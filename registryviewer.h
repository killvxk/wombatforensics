#ifndef REGISTRYVIEWER_H
#define REGISTRYVIEWER_H

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "wombatfunctions.h"
#include "ui_registryviewer.h"
#include "libregf.h"

namespace Ui
{
    class RegistryDialog;
}

class RegistryDialog : public QDialog
{
    Q_OBJECT

public:
    RegistryDialog(QWidget* parent = 0);
    ~RegistryDialog();
    void LoadRegistryFile(QString regid);

private slots:
    void HideClicked();
    void Assign();
    void UpdateList();
    void UpdateAssign();
    void KeySelected(void);
    void ValueSelected(void);
signals:
    //void TagCarved(QString ctitle, QString ctag);
protected:
    void closeEvent(QCloseEvent* event);

private:
    Ui::RegistryDialog* ui;
    void PopulateChildKeys(libregf_key_t* curkey, QTreeWidgetItem* curitem, libregf_error_t* curerr);
    QString regfilepath;
    //QFile bookmarkfile;
    //QStringList taglist;
};

Q_DECLARE_METATYPE(RegistryDialog*);

#endif // REGISTRYVIEWER_H