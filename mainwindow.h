#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <atpdb.h>
#include <atpini.h>
#include <dialogoptions.h>
#include <dialogimportattandance.h>

#include <mdijob.h>
#include <mdijobdetails.h>
#include <mdijobdetaileddetails.h>
#include <mdiworker.h>
#include <mdiattendance.h>
#include <mdipayments.h>
#include <mdisubcontracts.h>
#include <mdiuserrates.h>
#include <mdiusernotes.h>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    atpDb *myDb;
    atpIni *myIni;

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void on_actionOptions_triggered();
    void on_actionE_xit_triggered();
    void newMainDatabaseConnection();

    void on_action_Import_triggered();
    void on_actionJobs_triggered();
    void on_actionWorker_triggered();
    void on_actionAttendance_triggered();
    void on_actionPayments_triggered();
    void on_actionSubcontracts_triggered();
    void on_actionWorker_rates_triggered();
    void on_actionWorker_notes_triggered();

    void openJobDetails(QString jobDetailsId, QString jobDetailsName);
    void openJobDetailedDetails(QString job_Id, QString user_Id, QString user_name);

private:
    Ui::MainWindow *ui;

    dialogOptions *frmDialogOptions;
    dialogImportAttandance *frmDialogAttendance;

    mdiJob *frmMdiJob;
    mdiJobDetails *frmJobDetails;
    mdiJobDetailedDetails *frmJobDetailedDetails;
    mdiWorker *frmMdiWorker;
    mdiAttendance *frmAttendance;
    mdiPayments *frmPayments;
    mdiSubcontracts *frmSubcontracts;
    mdiUserRates *frmUserRates;
    mdiUserNotes *frmUserNotes;

    void initMainWindow();

};

#endif // MAINWINDOW_H
