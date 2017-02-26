#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCloseEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    initMainWindow();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {
//    mdiArea->closeAllSubWindows();
//    if (mdiArea->currentSubWindow()) {
//        event->ignore();
//    } else {
//        writeSettings();
        event->accept();
//    }
}

void MainWindow::newMainDatabaseConnection(){
    if(myDb->isDriverAvailable()){
        if(!myDb->setupNewConnection()){
            qDebug() << "Eroare de new DB Connection";
        }
    } else {
            qDebug() << "Eroare de gasire la db";
    }
}

void MainWindow::initMainWindow(){
    myDb = new atpDb(this);
    newMainDatabaseConnection();

    myIni = new atpIni(this);
}

/************************************************
 ******************File**************************
 ************************************************/

void MainWindow::on_action_Import_triggered() {
    frmDialogAttendance = new dialogImportAttandance(myDb, this);
    frmDialogAttendance->setModal(true);
//    connect(frmDialogOptions, &dialogOptions::newDatabase, this, &MainWindow::newMainDatabaseConnection);
    frmDialogAttendance->exec();
}

void MainWindow::on_actionE_xit_triggered() {
    QApplication::quit();
}

/************************************************
 ******************Tools*************************
 ************************************************/

void MainWindow::on_actionOptions_triggered() {
    frmDialogOptions = new dialogOptions(this);
    frmDialogOptions->setModal(true);
    connect(frmDialogOptions, &dialogOptions::newDatabase, this, &MainWindow::newMainDatabaseConnection);
    frmDialogOptions->exec();
}

/************************************************
 ******************Data**************************
 ************************************************/

void MainWindow::on_actionJobs_triggered() {
    frmMdiJob = new mdiJob(myDb, this);
    ui->mdiArea->addSubWindow(frmMdiJob);
    connect(frmMdiJob, &mdiJob::closeFrmJob, ui->mdiArea, &QMdiArea::closeActiveSubWindow);
    connect(frmMdiJob, &mdiJob::openJobDetaild, this, &MainWindow::openJobDetails);
    frmMdiJob->show();
}

void MainWindow::openJobDetails(QString jobDetailsId, QString jobDetailsName) {
    frmJobDetails = new mdiJobDetails(myDb, jobDetailsId, this);
    frmJobDetails->setWindowTitle(jobDetailsName);
    ui->mdiArea->addSubWindow(frmJobDetails);
    connect(frmJobDetails, &mdiJobDetails::closeFrmJobDetails, ui->mdiArea, &QMdiArea::closeActiveSubWindow);
    connect(frmJobDetails, &mdiJobDetails::openJobUserDetails, this, &MainWindow::openJobDetailedDetails);
    frmJobDetails->show();
}

void MainWindow::openJobDetailedDetails(QString job_Id, QString user_Id, QString user_name) {
    frmJobDetailedDetails = new mdiJobDetailedDetails(myDb, job_Id, user_Id);
    frmJobDetailedDetails->setWindowTitle(user_name);
    ui->mdiArea->addSubWindow(frmJobDetailedDetails);
    connect(frmJobDetailedDetails, &mdiJobDetailedDetails::closeFrmJobDetailedDetails, ui->mdiArea, &QMdiArea::closeActiveSubWindow);
    frmJobDetailedDetails->show();
}

void MainWindow::on_actionWorker_triggered() {
    frmMdiWorker = new mdiWorker(myDb, this);
    ui->mdiArea->addSubWindow(frmMdiWorker);
    connect(frmMdiWorker, &mdiWorker::closeFrmWorker, ui->mdiArea, &QMdiArea::closeActiveSubWindow);
    frmMdiWorker->show();
}

void MainWindow::on_actionAttendance_triggered() {
    frmAttendance = new mdiAttendance(this);
    ui->mdiArea->addSubWindow(frmAttendance);
//    connect(frmAttendance, &mdiAttendance::closeFrmAttendance, ui->mdiArea, &QMdiArea::closeActiveSubWindow);
    frmAttendance->show();
}

void MainWindow::on_actionPayments_triggered() {
    frmPayments = new mdiPayments(this);
    ui->mdiArea->addSubWindow(frmPayments);
//    connect(frmPayments, &mdiAttendance::closeFrmAttendance, ui->mdiArea, &QMdiArea::closeActiveSubWindow);
    frmPayments->show();
}

void MainWindow::on_actionSubcontracts_triggered() {
    frmSubcontracts = new mdiSubcontracts(this);
    ui->mdiArea->addSubWindow(frmSubcontracts);
//    connect(frmSubcontracts, &mdiAttendance::closeFrmAttendance, ui->mdiArea, &QMdiArea::closeActiveSubWindow);
    frmSubcontracts->show();
}

void MainWindow::on_actionWorker_rates_triggered() {
    frmUserRates = new mdiUserRates(this);
    ui->mdiArea->addSubWindow(frmUserRates);
//    connect(frmUserRates, &mdiAttendance::closeFrmAttendance, ui->mdiArea, &QMdiArea::closeActiveSubWindow);
    frmUserRates->show();
}

void MainWindow::on_actionWorker_notes_triggered() {
    frmUserNotes = new mdiUserNotes(this);
    ui->mdiArea->addSubWindow(frmUserNotes);
//    connect(frmUserNotes, &mdiAttendance::closeFrmAttendance, ui->mdiArea, &QMdiArea::closeActiveSubWindow);
    frmUserNotes->show();
}
