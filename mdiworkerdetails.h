#ifndef MDIWORKERDETAILS_H
#define MDIWORKERDETAILS_H

#include <QWidget>

namespace Ui {
class mdiWorkerDetails;
}

class mdiWorkerDetails : public QWidget
{
    Q_OBJECT

public:
    explicit mdiWorkerDetails(QWidget *parent = 0);
    ~mdiWorkerDetails();

private:
    Ui::mdiWorkerDetails *ui;
};

#endif // MDIWORKERDETAILS_H
