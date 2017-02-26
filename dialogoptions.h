#ifndef DIALOGOPTIONS_H
#define DIALOGOPTIONS_H

#include <QDialog>
#include <QListWidgetItem>
#include <atpini.h>

namespace Ui {
class dialogOptions;
}

class dialogOptions : public QDialog {
    Q_OBJECT

public:
    explicit dialogOptions(QWidget *parent = 0);
    ~dialogOptions();

signals:
    void newDatabase();

private slots:
    void on_listMenu_currentRowChanged(int currentRow);
    void on_buttonOk_clicked();
    void on_buttonApply_clicked();
    void on_buttonCancel_clicked();
    void on_buttonDbOpen_clicked();
    void on_buttonDbCreate_clicked();
    void on_buttonDbBackup_clicked();

    void databaseTabSelected();
    void autosaveTabSelected();

private:
    Ui::dialogOptions *ui;
    atpIni *myIniSettings;
    QMap<int, QWidget *> *myOwnTabs;

    void removeAllTabs();
    void prepareAllTabs();

};

#endif // DIALOGOPTIONS_H
