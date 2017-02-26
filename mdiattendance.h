#ifndef MDIATTENDANCE_H
#define MDIATTENDANCE_H

#include <QWidget>

namespace Ui {
class mdiAttendance;
}

class mdiAttendance : public QWidget
{
    Q_OBJECT

public:
    explicit mdiAttendance(QWidget *parent = 0);
    ~mdiAttendance();

private:
    Ui::mdiAttendance *ui;
};

#endif // MDIATTENDANCE_H
