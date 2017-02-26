#ifndef MDIJOB_H
#define MDIJOB_H

#include <QWidget>
#include <atpdb.h>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>

namespace Ui {
class mdiJob;
}

class mdiJob : public QWidget {
    Q_OBJECT

public:
    explicit mdiJob(atpDb *importDb, QWidget *parent = 0);
    ~mdiJob();

signals:
    void closeFrmJob();
    void openJobDetaild(QString, QString);

private slots:
    void on_buttonClose_clicked();
    void on_buttonCancel_clicked();
    void on_buttonNew_clicked();
    void on_buttonEdit_clicked();
    void on_buttonDelete_clicked();

    void updateModel();
    void diesableAll();
    void enableAll();
    void clearAllFields();
    void initButtons();

    void on_editFilter_textChanged(const QString &arg1);

    void on_tableView_clicked(const QModelIndex &index);

    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::mdiJob *ui;
    atpDb *myDb;
    QSqlTableModel *model;
    QSortFilterProxyModel *proxyModel;
    QMap<QString, QVariant> *data;
    int jobId;
};

#endif // MDIJOB_H
