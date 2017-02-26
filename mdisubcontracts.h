#ifndef MDISUBCONTRACTS_H
#define MDISUBCONTRACTS_H

#include <QWidget>

namespace Ui {
class mdiSubcontracts;
}

class mdiSubcontracts : public QWidget
{
    Q_OBJECT

public:
    explicit mdiSubcontracts(QWidget *parent = 0);
    ~mdiSubcontracts();

private:
    Ui::mdiSubcontracts *ui;
};

#endif // MDISUBCONTRACTS_H
