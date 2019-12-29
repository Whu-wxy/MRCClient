#ifndef MSGBOX_H
#define MSGBOX_H

#include <QWidget>
#include <QMessageBox>

class msgBox : public QMessageBox
{
public:
    msgBox(QWidget *parent = 0);
    ~msgBox();
};

#endif // MSGBOX_H
