#include "mdiattendance.h"
#include "ui_mdiattendance.h"

mdiAttendance::mdiAttendance(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mdiAttendance)
{
    ui->setupUi(this);
}

mdiAttendance::~mdiAttendance()
{
    delete ui;
}
