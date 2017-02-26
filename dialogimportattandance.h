#ifndef DIALOGIMPORTATTANDANCE_H
#define DIALOGIMPORTATTANDANCE_H

#include <QDialog>
#include <atpdb.h>
#include <QString>
#include <QDateTime>

namespace Ui {
class dialogImportAttandance;
}

class dialogImportAttandance : public QDialog {
    Q_OBJECT

public:
    explicit dialogImportAttandance(atpDb *paramDB, QWidget *parent = 0);
    ~dialogImportAttandance();

private slots:
    void on_buttonClose_clicked();
    void on_buttonImportDb_clicked();

    void importDatabase(QString dbPath);
    QMap<QString, QString> timeWorked(QString fromS, QString toS);
    QString amountToPay(QString dataLucrat, QString timpLucrat, QString idUser);

private:
    Ui::dialogImportAttandance *ui;
    atpDb *myAtpDb;

};

#endif // DIALOGIMPORTATTANDANCE_H
