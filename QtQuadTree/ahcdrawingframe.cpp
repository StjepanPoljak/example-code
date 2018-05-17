#include "ahcdrawingframe.h"

AHCDrawingFrame::AHCDrawingFrame (QWidget *parent) : QWidget(parent), quadTree (nullptr)
{

}

void AHCDrawingFrame::setup (int maxPoints)
{
    if (quadTree == nullptr)
    {
        quadTree = new QuadNode (QRect (0, 0,
                                        this -> width  (),
                                        this -> height ()),
                                 maxPoints);
    }
}

void AHCDrawingFrame::mouseReleaseEvent (QMouseEvent *event)
{
    QWidget::mouseReleaseEvent (event);

    quadTree->addPoint (QPoint (event->x (), event->y ()));

    repaint ();
}

void AHCDrawingFrame::paintEvent (QPaintEvent *event)
{
    QWidget::paintEvent (event);

    if (quadTree == nullptr) { return; }

    QPainter *paint = new QPainter (this);

    QVector <QuadNode *> stack = { quadTree };

    paint-> setBrush    (QBrush (Qt::black));

    QVector <QPoint> corners = {quadTree->bounds.topLeft        (),
                                quadTree->bounds.bottomLeft     (),
                                quadTree->bounds.bottomRight    (),
                                quadTree->bounds.topRight       (),
                                quadTree->bounds.topLeft        ()};

    QPolygon polygon (corners);
    paint->drawPolyline (polygon);

    do
    {
        QuadNode *curr = stack.last ();

        stack.remove (stack.size() - 1);

        for (int i = 0; i < curr->points.size (); i++)
        {
            paint-> setBrush    (QBrush (Qt::black));
            paint-> drawEllipse (QPoint (curr->points[i].x  (),
                                         curr->points[i].y  ()),
                                 2, 2);
        }

        if (curr->hasAllChildren ())
        {
            stack.append (curr->topLeft);
            stack.append (curr->bottomLeft);
            stack.append (curr->bottomRight);
            stack.append (curr->topRight);

            paint->setBrush (QBrush (Qt::gray));
            paint->drawLine (QPoint (curr->bounds.x         (),
                                     curr->bounds.y         ()
                                   + curr->bounds.height    () / 2),
                             QPoint (curr->bounds.x         ()
                                   + curr->bounds.width     (),
                                     curr->bounds.y         ()
                                   + curr->bounds.height    () / 2));
            paint->drawLine (QPoint (curr->bounds.x         ()
                                   + curr->bounds.width     () / 2,
                                     curr->bounds.y         ()),
                             QPoint (curr->bounds.x         ()
                                   + curr->bounds.width     () / 2,
                                     curr->bounds.y         ()
                                   + curr->bounds.height    ()));
        }

    } while (!stack.isEmpty ());

    paint->end ();
}

void AHCDrawingFrame::clear ()
{
    int maxPoints = quadTree->maxPoints;
    quadTree->deleteChildNodes ();
    quadTree = nullptr;
    setup (maxPoints);
    repaint ();
}
