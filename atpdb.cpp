#include "atpdb.h"

#include <QtSql>
#include <QFile>
#include <QMessageBox>
#include <QApplication>

atpDb::atpDb(QObject *parent) : QObject(parent){
    openStatus = false;
//reading ini file
    atpIniSettings = new atpIni;
    dbPath = atpIniSettings->get_dbPath();
    dbType = atpIniSettings->get_dbType();
}

bool atpDb::isDriverAvailable(){
//is driver available
    if(!QSqlDatabase::isDriverAvailable(dbType)){
        QMessageBox::critical(0, qApp->tr("Driver is not available"),
        qApp->tr("Unable to establish a database connection.\n"
        "This app needs SQLite support. Please read "
        "the Qt SQL driver documentation for information how "
        "to build it.\n\n"
        "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }
    return true;
}

bool atpDb::setupNewConnection(QString connName){
    dbPath = atpIniSettings->get_dbPath();
//check for the file exist
    QFile db_file(dbPath);
    if (!db_file.exists()) {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
        qApp->tr("Database file not found. Create or import one!"), QMessageBox::Ok);
        emit dbRefresh(false);
        return false;
    }
    mydb = QSqlDatabase::addDatabase(dbType, connName);
    mydb.setDatabaseName(dbPath);
    if (!mydb.open()){
        qCritical() << "couldn't connect to database Error[" << mydb.lastError().text() << "]"  << dbPath;
        emit dbRefresh(false);
        return false;
    } else {
        qDebug() << "succsessfully connected to database " << dbPath;
        openStatus = true;
        emit dbRefresh(true);
        return true;
    }
}

bool atpDb::isOpen(){
    return openStatus;
}

void atpDb::setDataBasePath(QString newPath){
    dbPath = newPath;
}

QSqlQuery atpDb::atpSelect(QString connName, QString sqlQuerry, QMap<QString, QVariant> *myArray ) {
    QSqlQuery *myQuerry;
    if(connName == "importConnection"){
        myQuerry = new QSqlQuery(sqlQuerry, myImportDb);
    } else {
        myQuerry = new QSqlQuery(sqlQuerry, mydb);
    }

    myQuerry->prepare(sqlQuerry);
    if (myArray && !myArray->isEmpty()){
        QMap<QString, QVariant>::const_iterator i;
        for (i = myArray->constBegin(); i != myArray->constEnd(); ++i) {
            myQuerry->bindValue(i.key(), i.value());
        }
    }
    myQuerry->exec();
    return *myQuerry;
}

QSqlQuery atpDb::atpInsert(QString connName, QString tableName, QMap<QString, QVariant> *data ) {
    QSqlQuery *myQuerry;
    if(connName == "importConnection"){
        myQuerry = new QSqlQuery(myImportDb);
    } else {
        myQuerry = new QSqlQuery(mydb);
    }

    QString fieldNames, fieldValues, temp;
    QMap<QString, QVariant>::const_iterator i1;
    for (i1 = data->constBegin(); i1 != data->constEnd(); ++i1) {
        temp = i1.key();
        fieldValues += temp + ", ";
        fieldNames += temp.remove(0,1) + ", ";
    }
    fieldNames.remove(fieldNames.lastIndexOf(","),1);
    fieldValues.remove(fieldValues.lastIndexOf(","),1);

    myQuerry->prepare("INSERT INTO " + tableName + " ( " + fieldNames + ") VALUES ( " + fieldValues + ")");
    QMap<QString, QVariant>::const_iterator i;
    for (i = data->constBegin(); i != data->constEnd(); ++i) {
        myQuerry->bindValue(i.key(), i.value());
    }
    myQuerry->exec();

    qDebug() << myQuerry->lastError().text();

    return *myQuerry;
}

QSqlQuery atpDb::atpUpdate(QString connName, QString tableName, QMap<QString, QVariant> *data, QString whereTo) {
    QSqlQuery *myQuerry;
    if(connName == "importConnection"){
        myQuerry = new QSqlQuery(myImportDb);
    } else {
        myQuerry = new QSqlQuery(mydb);
    }

    QString fieldDetails, fieldNames, fieldValues, temp;
    QMap<QString, QVariant>::const_iterator i1;
    for (i1 = data->constBegin(); i1 != data->constEnd(); ++i1) {
        temp = i1.key();
        fieldValues = temp;
        fieldNames = temp.remove(0,1);
        fieldDetails += fieldNames + " = " + fieldValues + ", ";
    }
    fieldDetails.remove(fieldDetails.lastIndexOf(","),1);
    myQuerry->prepare("UPDATE " + tableName + " SET " + fieldDetails + " WHERE " + whereTo);
    QMap<QString, QVariant>::const_iterator i;
    for (i = data->constBegin(); i != data->constEnd(); ++i) {
        myQuerry->bindValue(i.key(), i.value());
    }
    myQuerry->exec();
    return *myQuerry;
}

QSqlQuery atpDb::atpDelete(QString connName, QString tableName, QString whereTo){
    QSqlQuery *myQuerry;
    if(connName == "importConnection"){
        myQuerry = new QSqlQuery(myImportDb);
    } else {
        myQuerry = new QSqlQuery(mydb);
    }

    myQuerry->prepare("DELETE FROM " + tableName + " WHERE " + whereTo);
    myQuerry->exec();
    return *myQuerry;
}

QSqlDatabase atpDb::mainDatabase() {
    return mydb;
}

QSqlDatabase atpDb::importDatabase() {
    return myImportDb;
}

bool atpDb::setupNewImportConnection(QString dbImportPath, QString connName){

    //check for the file exist
    QFile db_file(dbImportPath);
    if (!db_file.exists()) {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
        qApp->tr("Database file not found!"), QMessageBox::Ok);
        return false;
    }
    myImportDb = QSqlDatabase::addDatabase(dbType, connName);
    myImportDb.setDatabaseName(dbImportPath);
    if (!myImportDb.open()){
        qCritical() << "couldn't connect to database Error[" << mydb.lastError().text() << "]"  << dbImportPath;
        return false;
    } else {
        qDebug() << "succsessfully connected to database " << dbImportPath;
        return true;
    }
}

bool atpDb::atp_create_new_atpdb(QString dbPath) {
    atpIni *atpIniSettings = new atpIni;
    QString dbType;
    dbType = atpIniSettings->get_dbType();
    QSqlDatabase mydb;
    mydb = QSqlDatabase::addDatabase(dbType);
    mydb.setDatabaseName(dbPath);
    mydb.open();
    QVector<QString> tables(0);

    // TABLE tbl_attendance
    tables.append("CREATE TABLE 'tbl_attendance' ( `att_id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, `att_pi_id` INTEGER, `att_job_id` INTEGER, `att_u_id` INTEGER, `att_timein` TEXT, `att_timeout` TEXT, `att_dateworked` TEXT, `att_oredisplay` TEXT, `att_amount` REAL, `att_orecalcul` TEXT, `att_un_id` INTEGER, `att_payed` TEXT )");
    /*  att_id
        att_pi_id
        att_job_id
        att_u_id
        att_timein
        att_timeout
        att_dateworked
        att_oredisplay
        att_orecalcul
        att_amount
        att_un_id
        att_payed */

    // TABLE tbl_jobs
    tables.append("CREATE TABLE 'tbl_jobs' ( `job_id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, `job_ref` TEXT, `job_owner` TEXT, `job_address` TEXT, `job_postcode` TEXT, `job_sitemanager` TEXT, `job_value` TEXT, `job_startdate` TEXT, `job_enddate` TEXT, `job_finished` TEXT, `job_closed` TEXT )");
    /*  job_id
        job_ref
        job_owner
        job_address
        job_postcode
        job_sitemanager
        job_value
        job_startdate
        job_enddate
        job_finished
        job_closed */

    // TABLE tbl_payments
    tables.append("CREATE TABLE 'tbl_payments' ( `pay_id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, `pay_u_id` INTEGER, `pay_date` TEXT, `pay_amount` REAL, `pay_un_id` INTEGER )");
    /*  pay_id
        pay_u_id
        pay_date
        pay_amount
        pay_un_id */

    // TABLE tbl_subcontract
    tables.append("CREATE TABLE 'tbl_subcontract' ( `sub_id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, `sub_u_id` INTEGER, `sub_j_id` INTEGER, `sub_date` TEXT, `sub_item` TEXT, `sub_quantity` REAL, `sub_um` TEXT, `sub_total` REAL, `sub_paied` TEXT, `sub_un_id` INTEGER )");
    /*  sub_id
        sub_u_id
        sub_j_id
        sub_date
        sub_item
        sub_quantity
        sub_um
        sub_total
        sub_paied
        sub_un_id*/

    // TABLE tbl_userrates
    tables.append("CREATE TABLE 'tbl_userrate' ( `ur_id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, `ur_u_id` INTEGER, `ur_amount` REAL, `ur_dateeffect` TEXT )");
    /*  ur_id
        ur_u_id
        ur_amount
        ur_dateeffect */

    // TABLE tbl_usernotes
    tables.append("CREATE TABLE 'tbl_usernotes' ( `un_id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, `un_u_id` INTEGER, `un_note` TEXT )");
    /*  un_id
        un_u_id
        un_note */

    // TABLE tbl_users
    tables.append("CREATE TABLE 'tbl_users' ( `u_id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, `u_cardid` INTEGER, `u_name` TEXT, `u_secondname` TEXT, `u_dob` TEXT, `u_address` TEXT, `u_nino` TEXT, `u_utr` TEXT, `u_bankshortcode` TEXT, `u_bankacount` TEXT, `u_maxhdaily` INTEGER, `u_subcontractor` TEXT )");
    /*  u_id
        u_cardid
        u_name
        u_secondname
        u_dob
        u_address
        u_nino
        u_utr
        u_bankshortcode
        u_bankacount
        u_maxhdaily
        u_subcontractor */

    QSqlQuery myquerry;
    for (int i = 0; i < tables.size(); ++i) {
        myquerry.exec(tables[i]);
    }
    createViews();
    return true;
}

void atpDb::createViews(){

    QVector<QString> viewTable(0);
    // VIEW view_jobdetails
    viewTable.append("CREATE VIEW IF NOT EXISTS 'view_jobdetails' AS SELECT att_job_id AS jd_job_id, ( u_name || ' ' || u_secondname) AS jd_numeleintreg, SUM(att_amount) AS jd_amount FROM tbl_attendance INNER JOIN tbl_users ON tbl_users.u_id = tbl_attendance.att_u_id GROUP BY tbl_attendance.att_u_id, tbl_attendance.att_job_id ");
    /* jd_job_id
       jd_u_id
       jd_numeleintreg
       jd_amount */

    QSqlQuery myquerry;
    for (int i = 0; i < viewTable.size(); ++i) {
        myquerry.exec(viewTable[i]);
    }
}
