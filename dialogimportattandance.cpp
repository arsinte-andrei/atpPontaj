#include "dialogimportattandance.h"
#include "ui_dialogimportattandance.h"

#include <QFileDialog>
#include <QDebug>
#include <QDir>
#include <QMessageBox>


dialogImportAttandance::dialogImportAttandance(atpDb *paramDB, QWidget *parent) : QDialog(parent), ui(new Ui::dialogImportAttandance) {
    ui->setupUi(this);
    myAtpDb = paramDB;
}

dialogImportAttandance::~dialogImportAttandance() {
    delete ui;
}

void dialogImportAttandance::on_buttonClose_clicked(){
    this->close();
}

void dialogImportAttandance::on_buttonImportDb_clicked(){
    QString dbFileName = QFileDialog::getOpenFileName(this,
                               tr("Find dataBase file"),
                                QDir::currentPath(),
                                tr("atp dataBase(*.atpDb)"));

    if (!dbFileName.isEmpty()) {
        importDatabase(dbFileName);
    }
}

void dialogImportAttandance::importDatabase(QString dbPath){

    if(myAtpDb->setupNewImportConnection(dbPath, "importConnection")){
        QMap<QString, QVariant> *checkQMap = new QMap<QString, QVariant>;
        QMap<QString, QVariant> *insertQMap = new QMap<QString, QVariant>;
        QMap<QString, QString> timpulMuncit;
        QString amountTobePayed;

        QSqlQuery importQuerry = myAtpDb->atpSelect("importConnection", "SELECT * FROM tbl_attendance WHERE time_worked <> '' ");
        importQuerry.last();
        int rowNo = importQuerry.at() +1;
        if(rowNo > 0){
            ui->buttonClose->setEnabled(false);
            ui->buttonImportDb->setEnabled(false);
            ui->progressBar->setMaximum(rowNo);
            ui->progressBar->setValue(0);
            for (int i = 0; i < rowNo; i++) {
                importQuerry.seek(i);
                QString pi_id = importQuerry.value("pi_id").toString();
                QString job_id = importQuerry.value("job_id").toString();
                QString user_id = importQuerry.value("user_id").toString();
                QString time_int_in = importQuerry.value("time_int_in").toString();
                QString time_int_out = importQuerry.value("time_int_out").toString();

                checkQMap->clear();
                checkQMap->insert(":att_pi_id", pi_id);
                checkQMap->insert(":att_job_id", job_id);
                QSqlQuery checkQuerry = myAtpDb->atpSelect("mainConnection", "SELECT * FROM tbl_attendance WHERE att_pi_id = :att_pi_id AND att_job_id = :att_job_id ", checkQMap);
                checkQuerry.last();
                int rowsNo = checkQuerry.at() +1;
                //TODO check for user in user db - if user exists and is the same name
                if(rowsNo > 0){
                    qDebug() << " Deja exista inregistrarea - nimic de facut";
                    //TODO se vor specifica recordurile care sunt gasite duble
                } else {
                    timpulMuncit.clear();
                    timpulMuncit = timeWorked(time_int_in, time_int_out);
                    amountTobePayed = amountToPay(timpulMuncit["dateWorked"], timpulMuncit["oreCalcul"], user_id);

                    if (amountTobePayed != "ERROR"){
                        insertQMap->clear();
                        insertQMap->insert(":att_pi_id", pi_id);
                        insertQMap->insert(":att_job_id", job_id);
                        insertQMap->insert(":att_u_id", user_id);
                        insertQMap->insert(":att_timein", timpulMuncit["oraIn"]);
                        insertQMap->insert(":att_timeout", timpulMuncit["oraOut"]);
                        insertQMap->insert(":att_dateworked", timpulMuncit["dateWorked"]);
                        insertQMap->insert(":att_oredisplay", timpulMuncit["oreDisplay"]);
                        insertQMap->insert(":att_orecalcul", timpulMuncit["oreCalcul"]);
                        insertQMap->insert(":att_amount", amountTobePayed);
                        insertQMap->insert(":att_payed", "NO");

                        QSqlQuery q = myAtpDb->atpInsert("mainConnection", "tbl_attendance", insertQMap);
                        int idToUpdate = q.lastInsertId().toInt();
                        qDebug () <<"last in: " << idToUpdate;
                    } else {
                        //TODO report amountToBePayed = ERROR
                        qDebug() << "amountToBePayed = ERROR - " <<pi_id << " - " <<job_id << " - " <<user_id;
                    }
                }
                ui->progressBar->setValue(i+1);
            }
        } else {
            QMessageBox::critical(0, qApp->tr("Database Opened"),
            qApp->tr("Database file found & opened, but no record found!"), QMessageBox::Ok);
        }
        ui->buttonClose->setEnabled(true);
        ui->buttonImportDb->setEnabled(true);
    } else {
        QMessageBox::critical(0, qApp->tr("No connection possible"),
        qApp->tr("Database file corrupted!"), QMessageBox::Ok);
    }
}

QMap<QString, QString> dialogImportAttandance::timeWorked(QString fromS, QString toS) {
    QDateTime dtFrom, dtTo;
    int minLucrate, intreg, rest;
    double oreMuncite;

    dtFrom.setSecsSinceEpoch(fromS.toLongLong()); //= QDateTime::fromString(fromS, "dd/MM/yyyy HH:mm");
    dtTo.setSecsSinceEpoch(toS.toLongLong()); //= QDateTime::fromString(toS, "dd/MM/yyyy HH:mm");

    minLucrate = dtFrom.secsTo(dtTo);

    oreMuncite = ((double) minLucrate / 3600 );
    intreg = minLucrate / 3600;
    rest = (minLucrate % 3600) / 60;

    QString oreDisplay, oreCalcul, oraIn, oraOut, dateWorked;
    oraIn = dtFrom.toString("dd/MM/yyyy HH:mm");
    oraOut = dtTo.toString("dd/MM/yyyy HH:mm");
    dateWorked = dtFrom.toString("dd/MM/yyyy");
    oreDisplay = QString::number(intreg).append(":").append(QString::number(rest));
    oreCalcul = QString::number(oreMuncite, 'f', 2);

    QMap<QString, QString> rezultat;
    rezultat.clear();
    rezultat.insert("oraIn", oraIn);
    rezultat.insert("oraOut", oraOut);
    rezultat.insert("dateWorked", dateWorked);
    rezultat.insert("oreCalcul", oreCalcul);
    rezultat.insert("oreDisplay", oreDisplay);
    return rezultat;
}

QString dialogImportAttandance::amountToPay(QString dataLucrat, QString timpLucrat, QString idUser) {
    QMap<QString, QVariant> *checkQMap = new QMap<QString, QVariant>;
    int seekVal, dataToVerify, dataLucrata;
    seekVal = 0;
    dataToVerify = QDateTime::fromString("01/01/2000", "dd/MM/yyyy").toSecsSinceEpoch();
    dataLucrata = QDateTime::fromString(dataLucrat, "dd/MM/yyyy").toSecsSinceEpoch();
    checkQMap->clear();
    checkQMap->insert(":ur_u_id", idUser);

    QSqlQuery checkQuerry = myAtpDb->atpSelect("mainConnection", "SELECT * FROM tbl_userRate WHERE ur_u_id = :ur_u_id ", checkQMap);
    checkQuerry.last();
    int rowsNo = checkQuerry.at() +1;
    if(rowsNo <= 0){
        return "ERROR";
    } else if(rowsNo > 1){
        for (int i = 0; i < rowsNo; i++) {
            checkQuerry.seek(i);
            QString rateDate = checkQuerry.value("ur_dateeffect").toString();
            int dateFound = QDateTime::fromString(rateDate, "dd/MM/yyyy").toSecsSinceEpoch();
            if (dateFound <= dataLucrata && dateFound > dataToVerify){
                seekVal = i;
                dataToVerify = dateFound;
            }
        }
    }
    checkQuerry.seek(seekVal);
    double rateAmount = checkQuerry.value("ur_amount").toString().toDouble();
    double timeLucrat = timpLucrat.toDouble();
    double total = rateAmount * timeLucrat;
    return QString::number(total, 'f', 2);
}
