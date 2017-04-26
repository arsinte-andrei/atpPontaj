#ifndef MDIJOBDETAILEDDETAILS_H
#define MDIJOBDETAILEDDETAILS_H

#include <QWidget>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>

#include <atpdb.h>

namespace Ui {
class mdiJobDetailedDetails;
}

class mdiJobDetailedDetails : public QWidget
{
    Q_OBJECT

public:
    explicit mdiJobDetailedDetails(atpDb *importDb, QString companyId, QString userrId, QWidget *parent = 0);
    ~mdiJobDetailedDetails();

signals:
    void closeFrmJobDetailedDetails();

private slots:
    void on_buttonClose_clicked();

    void updateModel();
private:
    Ui::mdiJobDetailedDetails *ui;
    atpDb *myDb;
    QSqlTableModel *model;
    QSortFilterProxyModel *proxyModel;
};

#endif // MDIJOBDETAILEDDETAILS_H
