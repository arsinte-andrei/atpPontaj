#include "mdijob.h"
#include "ui_mdijob.h"
#include <QDebug>
#include <QMessageBox>
#include <QDate>

mdiJob::mdiJob(atpDb *importDb, QWidget *parent) : QWidget(parent), ui(new Ui::mdiJob) {
    ui->setupUi(this);

    myDb = importDb;

    model = new QSqlTableModel(this, myDb->mainDatabase());
    proxyModel = new QSortFilterProxyModel(this);
    data = new QMap<QString, QVariant>;
    data->clear();

    on_buttonCancel_clicked();
}

mdiJob::~mdiJob() {
    delete ui;
}

/*Update the model*/

void mdiJob::updateModel() {
    model->setTable("tbl_jobs");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setHeaderData(0, Qt::Horizontal,"Id");
    model->setHeaderData(1, Qt::Horizontal,"Reference");
    model->setHeaderData(2, Qt::Horizontal,"Owner");
    model->setHeaderData(3, Qt::Horizontal,"Address");
    model->setHeaderData(4, Qt::Horizontal,"Postcode");
    model->setHeaderData(5, Qt::Horizontal,"Sitemanager");
    model->setHeaderData(6, Qt::Horizontal,"Value");
    model->setHeaderData(7, Qt::Horizontal,"Start date");
    model->setHeaderData(8, Qt::Horizontal,"Estimated finish");
    model->setHeaderData(9, Qt::Horizontal,"Job finised");
    model->setHeaderData(10, Qt::Horizontal,"Job closed");

    proxyModel->setSourceModel(model);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    ui->tableView->setModel(proxyModel);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setColumnHidden(0, true);

    ui->tableView->setColumnWidth(1, 100);
    ui->tableView->setColumnWidth(2, 200);
    ui->tableView->setColumnWidth(3, 150);
    ui->tableView->setColumnWidth(4, 75);
    ui->tableView->setColumnWidth(5, 200);
    ui->tableView->setColumnWidth(6, 100);
    ui->tableView->setColumnWidth(7, 100);
    ui->tableView->setColumnWidth(8, 100);
    ui->tableView->setColumnWidth(9, 100);
    ui->tableView->setColumnWidth(10, 75);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setAlternatingRowColors(true);
    if (model->select()){
        qDebug() <<"merge";
    }
}

void mdiJob::diesableAll() {
    ui->editReference->setEnabled(false);
    ui->editOwner->setEnabled(false);
    ui->editSiteManager->setEnabled(false);
    ui->editPostcode->setEnabled(false);
    ui->editAddress->setEnabled(false);
    ui->editJobValue->setEnabled(false);
    ui->editJobSpendings->setEnabled(false);
    ui->editJobClosed->setEnabled(false);
    ui->editJobStart->setEnabled(false);
    ui->editJobFinishBy->setEnabled(false);
    ui->editJobFinished->setEnabled(false);
    ui->tableView->setEnabled(true);
}

void mdiJob::enableAll() {
    ui->editReference->setEnabled(true);
    ui->editOwner->setEnabled(true);
    ui->editSiteManager->setEnabled(true);
    ui->editPostcode->setEnabled(true);
    ui->editAddress->setEnabled(true);
    ui->editJobValue->setEnabled(true);
    ui->editJobSpendings->setEnabled(false);
    ui->editJobClosed->setEnabled(true);
    ui->editJobStart->setEnabled(true);
    ui->editJobFinishBy->setEnabled(true);
    ui->editJobFinished->setEnabled(true);
    ui->tableView->setEnabled(false);
}

void mdiJob::clearAllFields(){
    ui->editReference->clear();
    ui->editOwner->clear();
    ui->editSiteManager->clear();
    ui->editPostcode->clear();
    ui->editAddress->clear();
    ui->editJobValue->clear();
    ui->editJobSpendings->clear();
    ui->editJobClosed->setChecked(false);
    ui->editJobStart->clear();
    ui->editJobFinishBy->clear();
    ui->editJobFinished->clear();
    jobId = 0;
}

void mdiJob::initButtons(){
    ui->buttonEdit->setEnabled(false);
    ui->buttonDelete->setEnabled(false);
    ui->buttonNew->setEnabled(true);

    ui->buttonNew->setText("New");
    ui->buttonEdit->setText("Edit");
}

void mdiJob::on_buttonClose_clicked() {
    emit closeFrmJob();
}

void mdiJob::on_buttonCancel_clicked() {
    updateModel();
    diesableAll();
    initButtons();
    clearAllFields();
}

void mdiJob::on_buttonNew_clicked() {
    QString buttonName;
    buttonName = ui->buttonNew->text();
    if ( buttonName == "New"){
        ui->buttonNew->setText("Save");
        ui->buttonDelete->setEnabled(false);
        ui->buttonEdit->setEnabled(false);
        enableAll();
        clearAllFields();
    } else {
        data->clear();
        data->insert(":job_ref", ui->editReference->text());
        data->insert(":job_owner", ui->editOwner->text());
        data->insert(":job_sitemanager", ui->editSiteManager->text());
        data->insert(":job_postcode", ui->editPostcode->text());
        data->insert(":job_address", ui->editAddress->document()->toPlainText());
        data->insert(":job_value", ui->editJobValue->value());
        data->insert(":job_startdate", ui->editJobStart->text());
        data->insert(":job_enddate", ui->editJobFinishBy->text());
        data->insert(":job_finished", ui->editJobFinished->text());
        if(ui->editJobClosed->isChecked()){
            data->insert(":job_closed", "YES");
        } else {
            data->insert(":job_closed", "NO");
        }

        myDb->atpInsert("mainConnection", "tbl_jobs", data);

        on_buttonCancel_clicked();
    }
}

void mdiJob::on_buttonEdit_clicked() {
    QString buttonName;
    buttonName = ui->buttonEdit->text();
    if ( buttonName == "Edit"){
        ui->buttonEdit->setText("Save");
        ui->buttonDelete->setEnabled(false);
        ui->buttonNew->setEnabled(false);
        enableAll();
    } else {
        data->clear();
        data->insert(":job_id", jobId);
        data->insert(":job_ref", ui->editReference->text());
        data->insert(":job_owner", ui->editOwner->text());
        data->insert(":job_sitemanager", ui->editSiteManager->text());
        data->insert(":job_postcode", ui->editPostcode->text());
        data->insert(":job_address", ui->editAddress->document()->toPlainText());
        data->insert(":job_value", ui->editJobValue->value());
        data->insert(":job_startdate", ui->editJobStart->text());
        data->insert(":job_enddate", ui->editJobFinishBy->text());
        data->insert(":job_finished", ui->editJobFinished->text());
        if(ui->editJobClosed->isChecked()){
            data->insert(":job_closed", "YES");
        } else {
            data->insert(":job_closed", "NO");
        }

        myDb->atpUpdate("mainConnection", "tbl_jobs", data, "job_id = :job_id");

        on_buttonCancel_clicked();
    }
}

void mdiJob::on_buttonDelete_clicked(){
    QMessageBox msgBox;
    msgBox.setText("Delete a job.");
    msgBox.setInformativeText("Are you shure you whant to delete this job record? Proceding will distroy all job data!");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();
    switch (ret) {
        case QMessageBox::No:
            diesableAll();
            break;
        case QMessageBox::Yes:{
            QString whereTo = QString("job_id = %1").arg(jobId);
            myDb->atpDelete("mainConnection", "tbl_jobs", whereTo);
            on_buttonCancel_clicked(); }
            break;
        default:
            break;
    }
}

void mdiJob::on_editFilter_textChanged(const QString &arg1){
    int columnToSort = proxyModel->sortColumn();
    if (columnToSort < 1) {
        columnToSort = 1;
    }
    proxyModel->setFilterKeyColumn(columnToSort);
    proxyModel->setFilterFixedString(arg1);
}

void mdiJob::on_tableView_clicked(const QModelIndex &index){

    qDebug() << "Single Click";

    int rand = index.row();
    QString job_id          = ui->tableView->model()->data(ui->tableView->model()->index(rand,0)).toString();
    QString job_ref         = ui->tableView->model()->data(ui->tableView->model()->index(rand,1)).toString();
    QString job_owner       = ui->tableView->model()->data(ui->tableView->model()->index(rand,2)).toString();
    QString job_address     = ui->tableView->model()->data(ui->tableView->model()->index(rand,3)).toString();
    QString job_postcode    = ui->tableView->model()->data(ui->tableView->model()->index(rand,4)).toString();
    QString job_sitemanager = ui->tableView->model()->data(ui->tableView->model()->index(rand,5)).toString();
    QString job_value       = ui->tableView->model()->data(ui->tableView->model()->index(rand,6)).toString();
    QString job_startdate   = ui->tableView->model()->data(ui->tableView->model()->index(rand,7)).toString();
    QString job_enddate     = ui->tableView->model()->data(ui->tableView->model()->index(rand,8)).toString();
    QString job_finished    = ui->tableView->model()->data(ui->tableView->model()->index(rand,9)).toString();
    QString job_closed      = ui->tableView->model()->data(ui->tableView->model()->index(rand,10)).toString();

    ui->editReference->setText(job_ref);
    ui->editOwner->setText(job_owner);
    ui->editSiteManager->setText(job_sitemanager);
    ui->editPostcode->setText(job_postcode);
    ui->editAddress->setText(job_address);
    ui->editJobValue->setValue(job_value.toDouble());

    QDate startDate, endDate, finishedDate;
    ui->editJobStart->setDate(startDate.fromString(job_startdate, "dd/MM/yyyy"));
    ui->editJobFinishBy->setDate(endDate.fromString(job_enddate, "dd/MM/yyyy"));
    ui->editJobFinished->setDate(finishedDate.fromString(job_finished, "dd/MM/yyyy"));
    if (job_closed == "YES"){
        ui->editJobClosed->setChecked(true);
    } else {
        ui->editJobClosed->setChecked(false);
    }

    QMap<QString, QVariant> *checkQMap = new QMap<QString, QVariant>;
    checkQMap->clear();
    checkQMap->insert(":att_job_id", job_id);

    QSqlQuery checkQuerry = myDb->atpSelect("mainConnection", "SELECT SUM(att_amount) AS totalamount FROM tbl_attendance WHERE att_job_id = :att_job_id ", checkQMap);
    checkQuerry.last();
    //sum(att_amount)
    ui->editJobSpendings->setValue(checkQuerry.value("totalamount").toDouble());

    jobId = job_id.toInt();
    if (jobId > 0) {
        ui->buttonEdit->setEnabled(true);
        ui->buttonDelete->setEnabled(true);
    } else {
        ui->buttonEdit->setEnabled(false);
        ui->buttonDelete->setEnabled(false);
    }
}

void mdiJob::on_tableView_doubleClicked(const QModelIndex &index) {

    qDebug() << "Double Click";

    int rand = index.row();
    QString job_id = ui->tableView->model()->data(ui->tableView->model()->index(rand,0)).toString();
    QString job_ref = ui->tableView->model()->data(ui->tableView->model()->index(rand,1)).toString();

    emit openJobDetaild(job_id, job_ref);
}
