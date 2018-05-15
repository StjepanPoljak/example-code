#include "mainwindow.h"

MainWindow::MainWindow (QWidget *parent) : QWidget(parent)
{
    setFixedSize (640, 480);

    ahcFrame    =   new AHCDrawingFrame (this);
    ahcFrame    ->  setGeometry         (140, 48, 360, 360);
    ahcFrame    ->  setup               (1);

    button      =   new QPushButton     (this);
    button      ->  setText             ("Clear");
    button      ->  adjustSize          ();

    int bottom  =   ahcFrame->height    ()
                  + ahcFrame->y         ();

    button      ->  setGeometry         (ahcFrame->x () + (ahcFrame->width  ()
                                                         - button->width    ()) / 2,
                                         bottom + (this->height             ()
                                                 - bottom - button->height  ()) / 2,
                                         button->width                      (),
                                         button->height                     ());
}

void MainWindow::buttonWasClicked ()
{
    ahcFrame->clear ();
}

void MainWindow::setup ()
{
    button->connect (button, SIGNAL (clicked (bool)), this, SLOT (buttonWasClicked ()));
}
