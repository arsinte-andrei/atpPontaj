#include "mdiworker.h"
#include "ui_mdiworker.h"
#include <QMessageBox>
#include <QDebug>
#include <QMessageBox>

mdiWorker::mdiWorker(atpDb *tataDb, QWidget *parent) : QWidget(parent), ui(new Ui::mdiWorker) {
    ui->setupUi(this);

    myDb = tataDb;
    model = new QSqlTableModel(this, myDb->mainDatabase());
    proxyModel = new QSortFilterProxyModel(this);
    data = new QMap<QString, QVariant>;
    data->clear();

    setModel();
    on_buttonCancel_clicked();
}

mdiWorker::~mdiWorker() {
    delete ui;
}

void mdiWorker::on_buttonClose_clicked() {
    emit closeFrmWorker();
}

void mdiWorker::on_buttonCancel_clicked() {
    updateModel();
    diesableAll();
    initButtons();
    clearAllFields();
}

void mdiWorker::on_buttonNew_clicked() {
    QString buttonName;
    buttonName = ui->buttonNew->text();
    if ( buttonName == "New"){
        ui->buttonNew->setText("Save");
        ui->buttonDelete->setEnabled(false);
        ui->buttonEdit->setEnabled(false);
        enableAll();
        clearAllFields();
    } else {
        if(allCompleated()){
            insertNewRecord();
            on_buttonCancel_clicked();
        } else {
            QMessageBox msgBox;
            msgBox.setText(errorReturned);
            msgBox.setInformativeText("Do you want to continue?");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::No);
            int ret = msgBox.exec();
            if(ret == QMessageBox::Yes){
                insertNewRecord();
                on_buttonCancel_clicked();
            }
        }
    }
}

void mdiWorker::on_buttonEdit_clicked() {
    QString buttonName;
    buttonName = ui->buttonEdit->text();
    if ( buttonName == "Edit"){
        ui->buttonEdit->setText("Save");
        ui->buttonDelete->setEnabled(false);
        ui->buttonNew->setEnabled(false);
        enableAll();
    } else {
        if(allCompleated()){
            updateOldRecord();
            on_buttonCancel_clicked();
        } else {
            QMessageBox msgBox;
            msgBox.setText(errorReturned);
            msgBox.setInformativeText("Do you want to continue?");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::No);
            int ret = msgBox.exec();
            if(ret == QMessageBox::Yes){
                updateOldRecord();
                on_buttonCancel_clicked();
            }
        }
    }

}

void mdiWorker::on_buttonDelete_clicked() {
    QMessageBox msgBox;
    msgBox.setText("Delete a worker.");
    msgBox.setInformativeText("Are you shure you whant to delete this worker records? Proceding will distroy all job/worker data!");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();
    switch (ret) {
        case QMessageBox::No:
            diesableAll();
            break;
        case QMessageBox::Yes:{
            QString whereTo = QString("u_id = %1").arg(userId);
            myDb->atpDelete("mainConnection", "tbl_users", whereTo);
            on_buttonCancel_clicked(); }
            break;
        default:
            break;
    }
}

void mdiWorker::on_editSearch_textChanged(const QString &arg1) {
    int columnToSort = proxyModel->sortColumn();
    if (columnToSort < 1) {
        columnToSort = 1;
    }
    proxyModel->setFilterKeyColumn(columnToSort);
    proxyModel->setFilterFixedString(arg1);
}

void mdiWorker::on_tableView_clicked(const QModelIndex &index) {
    int rand = index.row();

    QString u_id            = ui->tableView->model()->data(ui->tableView->model()->index(rand,0)).toString();
    userId = u_id.toInt();

    if (userId > 0) {
        QString u_cardid        = ui->tableView->model()->data(ui->tableView->model()->index(rand,1)).toString();
        QString u_name          = ui->tableView->model()->data(ui->tableView->model()->index(rand,2)).toString();
        QString u_secondname    = ui->tableView->model()->data(ui->tableView->model()->index(rand,3)).toString();
        QString u_dob           = ui->tableView->model()->data(ui->tableView->model()->index(rand,4)).toString();
        QString u_address       = ui->tableView->model()->data(ui->tableView->model()->index(rand,5)).toString();
        QString u_tel           = ui->tableView->model()->data(ui->tableView->model()->index(rand,6)).toString();
        QString u_nino          = ui->tableView->model()->data(ui->tableView->model()->index(rand,7)).toString();
        QString u_utr           = ui->tableView->model()->data(ui->tableView->model()->index(rand,8)).toString();
        QString u_bankshortcode = ui->tableView->model()->data(ui->tableView->model()->index(rand,9)).toString();
        QString u_bankacount    = ui->tableView->model()->data(ui->tableView->model()->index(rand,10)).toString();
        QString u_maxhdaily     = ui->tableView->model()->data(ui->tableView->model()->index(rand,11)).toString();
        QString u_subcontractor = ui->tableView->model()->data(ui->tableView->model()->index(rand,12)).toString();

        ui->editFirstName->setText(u_name);
        ui->editSecondName->setText(u_secondname);
        ui->editNino->setText(u_nino);
        ui->editUtr->setText(u_utr);
        ui->editCardId->setText(u_cardid);
        ui->editAddress->setText(u_address);
        ui->editTel->setText(u_tel);
        ui->editBankShortcode->setText(u_bankshortcode);
        ui->editBankAccount->setText(u_bankacount);
        ui->editHday->setValue(u_maxhdaily.toDouble());

        QDate dateOfBirth;
        ui->editDob->setDate(dateOfBirth.fromString(u_dob, "yyyy-MM-dd"));

        if (u_subcontractor == "YES"){
            ui->editSubcontractor->setChecked(true);
        } else {
            ui->editSubcontractor->setChecked(false);
        }

        getTheWages(u_id);

    //TODO the calculations
    //    ui->editJobSpendings->setText(job_startdate);

        ui->buttonEdit->setEnabled(true);
        ui->buttonDelete->setEnabled(true);
    } else {
        ui->buttonEdit->setEnabled(false);
        ui->buttonDelete->setEnabled(false);
    }
}

void mdiWorker::setModel() {
    model->setTable("tbl_users");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setHeaderData(0, Qt::Horizontal,"Id");
    model->setHeaderData(1, Qt::Horizontal,"Card ID");
    model->setHeaderData(2, Qt::Horizontal,"First Name");
    model->setHeaderData(3, Qt::Horizontal,"Second Name");
    model->setHeaderData(4, Qt::Horizontal,"DOB");
    model->setHeaderData(5, Qt::Horizontal,"Address");
    model->setHeaderData(6, Qt::Horizontal,"NINO");
    model->setHeaderData(7, Qt::Horizontal,"UTR");
    model->setHeaderData(8, Qt::Horizontal,"Bank account");
    model->setHeaderData(9, Qt::Horizontal,"Bank shortcode");
    model->setHeaderData(10, Qt::Horizontal,"Max h/day");
    model->setHeaderData(11, Qt::Horizontal,"Subcontractor");

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
    ui->tableView->setColumnWidth(11, 75);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setAlternatingRowColors(true);
}

void mdiWorker::updateModel(){

    if (model->select()){
        qDebug() <<"merge";
    }
}

void mdiWorker::diesableAll(){
    ui->editFirstName->setEnabled(false);
    ui->editSecondName->setEnabled(false);
    ui->editAddress->setEnabled(false);
    ui->editTel->setEnabled(false);
    ui->editNino->setEnabled(false);
    ui->editUtr->setEnabled(false);
    ui->editBankShortcode->setEnabled(false);
    ui->editBankAccount->setEnabled(false);
    ui->editDob->setEnabled(false);
    ui->editCardId->setEnabled(false);
    ui->editHday->setEnabled(false);
    ui->editSubcontractor->setEnabled(false);
    ui->tableView->setEnabled(true);
    ui->editWage->setEnabled(false);
    ui->editFromDate->setEnabled(false);
    ui->editSearch->setEnabled(true);
}

void mdiWorker::enableAll(){
    ui->editFirstName->setEnabled(true);
    ui->editSecondName->setEnabled(true);
    ui->editAddress->setEnabled(true);
    ui->editTel->setEnabled(true);
    ui->editNino->setEnabled(true);
    ui->editUtr->setEnabled(true);
    ui->editBankShortcode->setEnabled(true);
    ui->editBankAccount->setEnabled(true);
    ui->editDob->setEnabled(true);
    ui->editCardId->setEnabled(true);
    ui->editHday->setEnabled(true);
    ui->editSubcontractor->setEnabled(true);
    ui->tableView->setEnabled(false);
    ui->editWage->setEnabled(true);
    ui->editFromDate->setEnabled(true);
    ui->editSearch->setEnabled(false);
}

void mdiWorker::clearAllFields(){
    ui->editFirstName->clear();
    ui->editSecondName->clear();
    ui->editAddress->clear();
    ui->editTel->clear();
    ui->editNino->clear();
    ui->editUtr->clear();
    ui->editBankShortcode->clear();
    ui->editBankAccount->clear();
    ui->editDob->setDate(QDate::fromString("04/11/1984", "dd/MM/yyyy"));
    ui->editCardId->clear();
    ui->editHday->clear();
    ui->editSubcontractor->setChecked(false);
    ui->editWage->clear();
    ui->editFromDate->setDate(QDate::currentDate());
    userId = 0;
    wageChanged = false;
}

void mdiWorker::initButtons(){
    ui->buttonEdit->setEnabled(false);
    ui->buttonDelete->setEnabled(false);
    ui->buttonNew->setEnabled(true);

    ui->buttonNew->setText("New");
    ui->buttonEdit->setText("Edit");
}

/*check if all data has been completed if any mistake spoted return false*/
bool mdiWorker::allCompleated() {
    bool myRet = true;
    errorReturned.clear();

    if(ui->editAddress->document()->isEmpty()) {
        myRet = false;
        errorReturned.append("No address! ");
    }
    if(ui->editBankAccount->text().isEmpty()) {
        myRet = false;
        errorReturned.append("No bank account! ");
    }
    if(ui->editBankShortcode->text().isEmpty()) {
        myRet = false;
        errorReturned.append("No branch short code! ");
    }
    if(ui->editCardId->text().isEmpty()) {
        myRet = false;
        errorReturned.append("No card ID! ");
    }
    if(ui->editDob->text().isEmpty()) {
        myRet = false;
        errorReturned.append("No date of birth! ");
    }
    if(ui->editFirstName->text().isEmpty()) {
        myRet = false;
        errorReturned.append("No first name! ");
    }
    if(ui->editFromDate->text().isEmpty()) {
        myRet = false;
        errorReturned.append("No date for the wage to start from! ");
    }
    if(ui->editHday->text().isEmpty()) {
        myRet = false;
        errorReturned.append("No max time @day! ");
    }
    if(ui->editNino->text().isEmpty()) {
        myRet = false;
        errorReturned.append("No NINO! ");
    }
    if(ui->editSecondName->text().isEmpty()) {
        myRet = false;
        errorReturned.append("No family name! ");
    }
    if(ui->editTel->text().isEmpty()) {
        myRet = false;
        errorReturned.append("No telephone! ");
    }
    if(ui->editUtr->text().isEmpty()) {
        myRet = false;
        errorReturned.append("No UTR! ");
    }
    if(ui->editWage->text().isEmpty()) {
        myRet = false;
        errorReturned.append("No wage seted! ");
    }

    return myRet;
}

void mdiWorker::insertNewRecord() {
    /*Insert new record in tbl_users*/
    data->clear();
    data->insert(":u_cardid", ui->editCardId->text());
    data->insert(":u_name", ui->editFirstName->text());
    data->insert(":u_secondname", ui->editSecondName->text());
    data->insert(":u_dob", ui->editDob->date().toString("yyyy-MM-dd"));
    data->insert(":u_address", ui->editAddress->document()->toPlainText());
    data->insert(":u_tel", ui->editTel->text());
    data->insert(":u_nino", ui->editNino->text());
    data->insert(":u_utr", ui->editUtr->text());
    data->insert(":u_bankshortcode", ui->editBankShortcode->text());
    data->insert(":u_bankacount", ui->editBankAccount->text());
    data->insert(":u_maxhdaily", ui->editHday->value());
    if(ui->editSubcontractor->isChecked()){
        data->insert(":u_subcontractor", "YES");
    } else {
        data->insert(":u_subcontractor", "NO");
    }
    myDb->atpInsert("mainConnection", "tbl_users", data);

    /*Search tbl_users to see last inserted ID*/
    QSqlQuery myQ = myDb->atpSelect("mainConnection", "SELECT u_id FROM tbl_users WHERE u_name = '" + ui->editFirstName->text() + "' AND u_secondname = '" + ui->editSecondName->text() + "' AND u_dob = '" + ui->editDob->text() +"'");
    myQ.first();

    /*Insert new record in tbl_userrate based on last interted ID*/
    data->clear();
    data->insert("ur_u_id", myQ.value("u_id").toString());
    data->insert("ur_amount", ui->editWage->value());
    data->insert("ur_dateefect", ui->editFromDate->date().toString("yyyy-MM-dd"));
    myDb->atpInsert("mainConnection", "tbl_userrate", data);
}

void mdiWorker::updateOldRecord() {
    data->clear();
    data->insert(":u_id", userId);
    data->insert(":u_cardid", ui->editCardId->text());
    data->insert(":u_name", ui->editFirstName->text());
    data->insert(":u_secondname", ui->editSecondName->text());
    data->insert(":u_dob", ui->editDob->text());
    data->insert(":u_address", ui->editAddress->document()->toPlainText());
    data->insert(":u_tel", ui->editTel->text());
    data->insert(":u_nino", ui->editNino->text());
    data->insert(":u_utr", ui->editUtr->text());
    data->insert(":u_bankshortcode", ui->editBankShortcode->text());
    data->insert(":u_bankacount", ui->editBankAccount->text());
    data->insert(":u_maxhdaily", ui->editHday->value());
    if(ui->editSubcontractor->isChecked()){
        data->insert(":u_subcontractor", "YES");
    } else {
        data->insert(":u_subcontractor", "NO");
    }

    myDb->atpUpdate("mainConnection", "tbl_users", data, "u_id = :u_id");

    if (wageChanged) {
        data->clear();
        data->insert("ur_u_id", userId);
        data->insert("ur_amount", ui->editWage->value());
        data->insert("ur_dateefect", ui->editFromDate->date());
        myDb->atpInsert("mainConnection", "tbl_userrate", data);
    }
}

void mdiWorker::getTheWages(QString personId) {
    QSqlQuery myQ = myDb->atpSelect("mainConnection", "SELECT * FROM tbl_userrate WHERE ur_u_id = '" + personId + "' ORDER BY date(ur_dateeffect) DESC LIMIT 1");
    myQ.first();
    ui->editWage->setValue(myQ.value("ur_amount").toDouble());
    ui->editFromDate->setDate(myQ.value("ur_dateeffect").toDate());
}

void mdiWorker::on_editWage_valueChanged(double arg1) {
    Q_UNUSED(arg1);
    wageChanged = true;
}
