#include "mdijobdetaileddetails.h"
#include "ui_mdijobdetaileddetails.h"
#include <QDebug>

mdiJobDetailedDetails::mdiJobDetailedDetails(atpDb *importDb, QString companyId, QString userrId, QWidget *parent) : QWidget(parent), ui(new Ui::mdiJobDetailedDetails) {
    ui->setupUi(this);

    myDb = importDb;
    model = new QSqlTableModel(this, myDb->mainDatabase());
    proxyModel = new QSortFilterProxyModel(this);
}

mdiJobDetailedDetails::~mdiJobDetailedDetails() {
    delete ui;
}

void mdiJobDetailedDetails::on_buttonClose_clicked() {
    emit closeFrmJobDetailedDetails();
}

void mdiJobDetailedDetails::updateModel() {
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
