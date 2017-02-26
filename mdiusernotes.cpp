#include "mdiusernotes.h"
#include "ui_mdiusernotes.h"

mdiUserNotes::mdiUserNotes(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mdiUserNotes)
{
    ui->setupUi(this);
}

mdiUserNotes::~mdiUserNotes()
{
    delete ui;
}
