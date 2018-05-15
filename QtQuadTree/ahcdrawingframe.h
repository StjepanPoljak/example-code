#ifndef AHCDRAWINGFRAME_H
#define AHCDRAWINGFRAME_H

#include <QWidget>
#include <QRect>
#include <QMouseEvent>
#include <QPainter>
#include "quadnode.h"

class AHCDrawingFrame : public QWidget
{
    Q_OBJECT

public:

    explicit            AHCDrawingFrame     (QWidget *parent = nullptr);
    void                setup               (int maxPoints);
    void                clear               ();

private:

    void                paintEvent          (QPaintEvent *event);
    void                mouseReleaseEvent   (QMouseEvent *event);

    QuadNode            *quadTree;

signals:

public slots:

};

#endif // AHCDRAWINGFRAME_H
