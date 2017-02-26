#include "mdipayments.h"
#include "ui_mdipayments.h"

mdiPayments::mdiPayments(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mdiPayments)
{
    ui->setupUi(this);
}

mdiPayments::~mdiPayments()
{
    delete ui;
}
