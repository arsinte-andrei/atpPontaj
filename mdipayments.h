#ifndef MDIPAYMENTS_H
#define MDIPAYMENTS_H

#include <QWidget>

namespace Ui {
class mdiPayments;
}

class mdiPayments : public QWidget
{
    Q_OBJECT

public:
    explicit mdiPayments(QWidget *parent = 0);
    ~mdiPayments();

private:
    Ui::mdiPayments *ui;
};

#endif // MDIPAYMENTS_H
