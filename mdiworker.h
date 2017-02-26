#ifndef MDIWORKER_H
#define MDIWORKER_H

#include <QWidget>
#include <atpdb.h>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>

namespace Ui {
class mdiWorker;
}

class mdiWorker : public QWidget {
    Q_OBJECT

public:
    explicit mdiWorker(atpDb *tataDb, QWidget *parent = 0);
    ~mdiWorker();

signals:
    void closeFrmWorker();

private slots:
    void on_buttonClose_clicked();
    void on_buttonCancel_clicked();
    void on_buttonNew_clicked();
    void on_buttonEdit_clicked();
    void on_buttonDelete_clicked();
    void on_editSearch_textChanged(const QString &arg1);
    void on_tableView_clicked(const QModelIndex &index);

    void updateModel();
    void diesableAll();
    void enableAll();
    void clearAllFields();
    void initButtons();

private:
    Ui::mdiWorker *ui;
    atpDb *myDb;
    QSqlTableModel *model;
    QSortFilterProxyModel *proxyModel;
    QMap<QString, QVariant> *data;
    int userId;
};

#endif // MDIWORKER_H
