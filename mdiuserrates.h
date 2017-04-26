#ifndef MDIUSERRATES_H
#define MDIUSERRATES_H

#include <QWidget>

namespace Ui {
class mdiUserRates;
}

class mdiUserRates : public QWidget {
    Q_OBJECT

public:
    explicit mdiUserRates(QWidget *parent = 0);
    ~mdiUserRates();

private:
    Ui::mdiUserRates *ui;
};

#endif // MDIUSERRATES_H
