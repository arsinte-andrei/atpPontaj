#include "dialogoptions.h"
#include "ui_dialogoptions.h"

#include <QDebug>
#include <QFileDialog>
#include <QDateTime>
#include <atpdb.h>

dialogOptions::dialogOptions(QWidget *parent) : QDialog(parent), ui(new Ui::dialogOptions) {
    ui->setupUi(this);

   prepareAllTabs();

    myIniSettings = new atpIni(this);

    removeAllTabs();
}

dialogOptions::~dialogOptions() {
    delete ui;
}

/************************************************
 ******************TABS Menu*********************
 ************************************************/

void dialogOptions::on_listMenu_currentRowChanged(int currentRow) {
    qDebug() << currentRow;
    if (currentRow == 0){
        databaseTabSelected();
    } else if(currentRow == 1){
        autosaveTabSelected();
    } else {
        removeAllTabs();
        ui->tabPanel->setVisible(false);
        ui->lblMenuName->setText("Select menu from left");
    }
}

void dialogOptions::removeAllTabs() {
    ui->tabPanel->setVisible(false);
    ui->tabPanel->removeTab(0);
    ui->tabPanel->removeTab(1);
}

void dialogOptions::prepareAllTabs() {
    myOwnTabs = new QMap<int, QWidget *>();
    myOwnTabs->clear();
    myOwnTabs->insert(1,ui->tabDatabase);
    myOwnTabs->insert(2, ui->tabAutosave);
}

/************************************************
 ******************Buttons***********************
 ************************************************/

void dialogOptions::on_buttonOk_clicked() {
    this->accept();
}

void dialogOptions::on_buttonApply_clicked() {
    ui->listMenu->setCurrentRow(0);
}

void dialogOptions::on_buttonCancel_clicked() {
    this->reject();
}


/************************************************
 ******************DATABASE**********************
 ************************************************/

void dialogOptions::databaseTabSelected() {
    removeAllTabs();
    ui->tabPanel->addTab(myOwnTabs->value(1), "Database");
    ui->tabPanel->setVisible(true);
    ui->lblMenuName->setText("Database");
    ui->editDbPath->setText(myIniSettings->get_dbPath());
}

void dialogOptions::on_buttonDbOpen_clicked() {
    QString oldPath = ui->editDbPath->text();
    QString dbFileName = QFileDialog::getOpenFileName(this,
                               tr("Find dataBase file"),
                                QDir::currentPath(),
                                tr("atp dataBase(*.atpDb)"));

    if (!dbFileName.isEmpty() && oldPath != dbFileName) {
        ui->editDbPath->setText(dbFileName);
        myIniSettings->set_dbPath(dbFileName);
        emit newDatabase();
        qDebug() << "nu e gol si e diferit";
    }
}

void dialogOptions::on_buttonDbCreate_clicked(){
    ui->editDbPath->clear();
    QFileDialog *mySaveFileDialog = new QFileDialog(this,
                                                    tr("Open or creaye new atp dataBase file"),
                                                     QDir::currentPath(),
                                                     tr("atp dataBase(*.atpDb)"));
    mySaveFileDialog->setFileMode(QFileDialog::AnyFile);
    mySaveFileDialog->setViewMode(QFileDialog::Detail);
    mySaveFileDialog->setNameFilter(tr("atp dataBase(*.atpDb)"));
    mySaveFileDialog->setDefaultSuffix("atpDb");
    mySaveFileDialog->setAcceptMode(QFileDialog::AcceptSave);
    mySaveFileDialog->exec();
    QStringList dbFileName = mySaveFileDialog->selectedFiles();

    qDebug() << "lista save open: " << dbFileName;

   if(mySaveFileDialog->Accepted && !dbFileName.isEmpty()){
        ui->editDbPath->setText(dbFileName[0]);
        if(atpDb::atp_create_new_atpdb(dbFileName[0])){
            myIniSettings->set_dbPath(dbFileName[0]);
            QString mesaj = "Database created: ";
            mesaj.append(dbFileName[0]);
            ui->lblDbStatus->setText(mesaj);
            emit newDatabase();
        } else {
            ui->lblDbStatus->setText("Database create error - old db will be used!");
            ui->editDbPath->setText(myIniSettings->get_dbPath());
        }
    } else {
       ui->lblDbStatus->setText("Database create error - old db will be used!");
       ui->editDbPath->setText(myIniSettings->get_dbPath());
   }

}

void dialogOptions::on_buttonDbBackup_clicked(){
    QString currentDbPath = myIniSettings->get_dbPath();
    if (currentDbPath.endsWith(".atpDb")){
        QString backupDbPath = currentDbPath;
        backupDbPath.remove(".atpDb");
        backupDbPath.append(QDateTime::currentDateTime().toString("dd-MM-yyyy hh-mm-ss"));
        backupDbPath.append(".atpDb");
        qDebug() << currentDbPath << " *** " << backupDbPath;
        if(QFile::copy(currentDbPath, backupDbPath)){
            QString mesaj = "Backup finished: ";
            mesaj.append(backupDbPath);
            qDebug() <<"Finished Backup";
            ui->lblDbStatus->setText(mesaj);
        }
    }
}


/************************************************
 ******************AUTOSAVE**********************
 ************************************************/

void dialogOptions::autosaveTabSelected() {
    removeAllTabs();
    ui->tabPanel->addTab(myOwnTabs->value(2), "Autosave");
    ui->tabPanel->setVisible(true);
    ui->lblMenuName->setText("Autosave");
}

