#include "msgbox.h"
#include <QApplication>

msgBox::msgBox(QWidget *parent)
    : QMessageBox(parent)
{
    setObjectName("msgBox");
    QFont f = qApp->font();
    f.setBold(true);
    setFont(f);
    setFixedSize(400,200);
    //应用样式
    QString style("QDialog {"
                "  border: 1px solid rgb(111, 156, 207);"
                 " background: rgb(0,215,225); }"
         " QDialog QPushButton {"
            "  border-radius: 15px;"
           "   border: none;"
          "padding: 5px;"
          "    color: rgb(245,245,220);"
        "      background: rgb(0,215,225);}"
          "QDialog QPushButton:hover {"
         "     background: rgb(0,180,195);}"

       "   QDialog QPushButton:pressed {"
         "     background: rgb(0,197,205);}"
         "QDialog QLabel {"
          "    color: rgb(245,245,220);}"
          );

    setStyleSheet(style);

    addButton(QMessageBox::Ok);

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);

}

msgBox::~msgBox()
{

}
