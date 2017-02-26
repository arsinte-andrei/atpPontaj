#include "mdiworkerdetails.h"
#include "ui_mdiworkerdetails.h"

mdiWorkerDetails::mdiWorkerDetails(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mdiWorkerDetails)
{
    ui->setupUi(this);
}

mdiWorkerDetails::~mdiWorkerDetails()
{
    delete ui;
}
