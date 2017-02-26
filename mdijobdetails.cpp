#include "mdijobdetails.h"
#include "ui_mdijobdetails.h"
#include <QDebug>
#include <QMessageBox>

mdiJobDetails::mdiJobDetails(atpDb *importDb, QString companyId, QWidget *parent) : QWidget(parent), ui(new Ui::mdiJobDetails) {
    ui->setupUi(this);
    jobId = companyId;
    whereFilter = QString("jd_job_id = '%1'").arg(jobId);
    myDb = importDb;
    model = new QSqlTableModel(this, myDb->mainDatabase());
    proxyModel = new QSortFilterProxyModel(this);
    diesableAll();
    initData();
    updateModel();
}

mdiJobDetails::~mdiJobDetails() {
    delete ui;
}

void mdiJobDetails::diesableAll() {
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

void mdiJobDetails::initData() {
    // get details from tbl_jobs
    checkMap = new QMap<QString, QVariant>;
    checkMap->clear();
    checkMap->insert(":job_id", jobId);
    QSqlQuery checkQuerry = myDb->atpSelect("mainConnection", "SELECT * FROM tbl_jobs WHERE job_id = :job_id ;", checkMap);
    if(checkQuerry.last()){
        //get details amount from tbl_attendance
        checkMap->clear();
        checkMap->insert(":att_job_id", jobId);
        QSqlQuery checkQuerry1 = myDb->atpSelect("mainConnection", "SELECT SUM(att_amount) AS totalamount FROM tbl_attendance WHERE att_job_id = :att_job_id ", checkMap);
        if(checkQuerry1.last()){
            ui->editJobSpendings->setValue(checkQuerry1.value("totalamount").toDouble());
        }

        ui->editReference->setText(checkQuerry.value("job_ref").toString());
        ui->editOwner->setText(checkQuerry.value("job_owner").toString());
        ui->editSiteManager->setText(checkQuerry.value("job_sitemanager").toString());
        ui->editPostcode->setText(checkQuerry.value("job_postcode").toString());
        ui->editAddress->setText(checkQuerry.value("job_address").toString());
        ui->editJobValue->setValue(checkQuerry.value("job_value").toDouble());
        ui->editJobStart->setDate(QDate::fromString(checkQuerry.value("job_startdate").toString(),"dd/MM/yyyy"));
        ui->editJobFinishBy->setDate(QDate::fromString(checkQuerry.value("job_enddate").toString(),"dd/MM/yyyy"));
        ui->editJobFinished->setDate(QDate::fromString(checkQuerry.value("job_finished").toString(),"dd/MM/yyyy"));
        if (checkQuerry.value("job_closed").toString() == "NO"){
            ui->editJobClosed->setChecked(false);
        } else {
            ui->editJobClosed->setChecked(true);
        }
    }
}

void mdiJobDetails::updateModel() {

    model->setTable("view_jobdetails");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setHeaderData(2, Qt::Horizontal,"Name");
    model->setHeaderData(3, Qt::Horizontal,"Amount");

    model->setFilter(whereFilter);

    proxyModel->setSourceModel(model);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    ui->tableView->setModel(proxyModel);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setColumnHidden(0, true);
     ui->tableView->setColumnHidden(1, true);

    ui->tableView->setColumnWidth(2, 250);
    ui->tableView->setColumnWidth(3, 100);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setAlternatingRowColors(true);

    if (model->select()){
        qDebug() <<"merge";
    }
}

void mdiJobDetails::on_buttonClose_clicked() {
    emit closeFrmJobDetails();
}

void mdiJobDetails::on_editFilterBy_textChanged(const QString &arg1) {
    int columnToSort = proxyModel->sortColumn();
    if (columnToSort < 1) {
        columnToSort = 1;
    }
    proxyModel->setFilterKeyColumn(columnToSort);
    proxyModel->setFilterFixedString(arg1);
}

void mdiJobDetails::on_tableView_doubleClicked(const QModelIndex &index) {
    int rand = index.row();
    QString vJdJobId  = ui->tableView->model()->data(ui->tableView->model()->index(rand,0)).toString();
    QString vJdUserId = ui->tableView->model()->data(ui->tableView->model()->index(rand,1)).toString();
    QString vJdUserName = ui->tableView->model()->data(ui->tableView->model()->index(rand,2)).toString();
    if (jobId == vJdJobId){
        emit openJobUserDetails(vJdJobId, vJdUserId, vJdUserName);
    }
}
