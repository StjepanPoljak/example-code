#include <QApplication>
#include <QPushButton>
#include <qstring.h>
#include "mainwindow.h"
#include <QPainter>

int main (int argc, char **argv)
{
    QApplication    app (argc, argv);
    MainWindow      window;

    window.setup    ();
    window.show     ();

    return app.exec ();
}
