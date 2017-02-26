#include "mdiuserrates.h"
#include "ui_mdiuserrates.h"

mdiUserRates::mdiUserRates(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mdiUserRates)
{
    ui->setupUi(this);
}

mdiUserRates::~mdiUserRates()
{
    delete ui;
}
