#include "registryviewer.h"

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

RegistryDialog::RegistryDialog(QWidget* parent) : QDialog(parent), ui(new Ui::RegistryDialog)
{
    ui->setupUi(this);
    //regfile = NULL;
    //regerr = NULL;
    /*
    connect(ui->cancelbutton, SIGNAL(clicked()), this, SLOT(HideClicked()));
    connect(ui->carvebutton, SIGNAL(clicked()), this, SLOT(Assign()));
    connect(ui->titlelineedit, SIGNAL(editingFinished()), this, SLOT(UpdateAssign()));
    ui->carvebutton->setEnabled(false);
    UpdateList();
    */
}

RegistryDialog::~RegistryDialog()
{
    //this->close();
    delete ui;
}

void RegistryDialog::HideClicked()
{
    this->close();
}

void RegistryDialog::Assign()
{
    //qDebug() << ui->tagcombobox->currentText();
    //qDebug() << ui->titlelineedit->text();
    //emit TagCarved(ui->titlelineedit->text(), ui->tagcombobox->currentText());
    this->close();
}

void RegistryDialog::UpdateAssign()
{
    /*
    if(ui->titlelineedit->text().isEmpty())
        ui->carvebutton->setEnabled(false);
    else
        ui->carvebutton->setEnabled(true);
    */
}

void RegistryDialog::UpdateList()
{
    /*
    taglist.clear();
    bookmarkfile.setFileName(wombatvariable.tmpmntpath + "bookmarks");
    bookmarkfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(bookmarkfile.isOpen())
        taglist = QString(bookmarkfile.readLine()).split(",", Qt::SkipEmptyParts);
    bookmarkfile.close();
    for(int i=0; i < taglist.count(); i++)
    {
        ui->tagcombobox->addItem(taglist.at(i));
    }
    */
}

void RegistryDialog::closeEvent(QCloseEvent* e)
{
    //libregf_file_close(regfile, &regerr);
    //libregf_file_free(&regfile, &regerr);
    //libregf_error_free(&regerr);
    e->accept();
}

void RegistryDialog::LoadRegistryFile(QString regid)
{
    qDebug() << "regid:" << regid;
    libregf_file_t* regfile = NULL;
    libregf_error_t* regerr = NULL;
    libregf_file_initialize(&regfile, &regerr);
    QString regfilestr = wombatvariable.tmpfilepath + regid + "-fhex";
    libregf_file_open(regfile, regfilestr.toStdString().c_str(), LIBREGF_OPEN_READ, &regerr);
    libregf_file_close(regfile, &regerr);
    libregf_file_free(&regfile, &regerr);
    libregf_error_free(&regerr);
    this->show();
}
