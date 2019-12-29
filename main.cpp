#include "mrcclient.h"

#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MRCClient w;
    w.show();

    return a.exec();
}
