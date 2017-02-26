#ifndef MDIJOBDETAILS_H
#define MDIJOBDETAILS_H

#include <QWidget>
#include <atpdb.h>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>
#include <QMap>

namespace Ui {
class mdiJobDetails;
}

class mdiJobDetails : public QWidget {
    Q_OBJECT

public:
    explicit mdiJobDetails(atpDb *importDb, QString companyId, QWidget *parent = 0);
    ~mdiJobDetails();

signals:
    void closeFrmJobDetails();
    void openJobUserDetails(QString, QString, QString);

private slots:
    void updateModel();
    void on_buttonClose_clicked();
    void on_editFilterBy_textChanged(const QString &arg1);
    void on_tableView_doubleClicked(const QModelIndex &index);
    void diesableAll();
    void initData();

private:
    Ui::mdiJobDetails *ui;
    atpDb *myDb;
    QSqlTableModel *model;
    QSortFilterProxyModel *proxyModel;
    QString whereFilter, jobId;
    QMap<QString, QVariant> *checkMap;
};

#endif // MDIJOBDETAILS_H
