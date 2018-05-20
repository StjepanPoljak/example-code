#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include "ahcdrawingframe.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:

    explicit            MainWindow  (QWidget *parent = nullptr);
    void                setup       ();

private:

    AHCDrawingFrame 	*ahcFrame;
    QPushButton         *button;

signals:

public slots:

    void 		buttonWasClicked    ();
};

#endif // MAINWINDOW_H
