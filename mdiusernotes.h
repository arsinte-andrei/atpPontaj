#ifndef MDIUSERNOTES_H
#define MDIUSERNOTES_H

#include <QWidget>

namespace Ui {
class mdiUserNotes;
}

class mdiUserNotes : public QWidget
{
    Q_OBJECT

public:
    explicit mdiUserNotes(QWidget *parent = 0);
    ~mdiUserNotes();

private:
    Ui::mdiUserNotes *ui;
};

#endif // MDIUSERNOTES_H
