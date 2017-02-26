#include "mdisubcontracts.h"
#include "ui_mdisubcontracts.h"

mdiSubcontracts::mdiSubcontracts(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mdiSubcontracts)
{
    ui->setupUi(this);
}

mdiSubcontracts::~mdiSubcontracts()
{
    delete ui;
}
