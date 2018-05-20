#include "ahcdrawingframe.h"
#include <cmath>

AHCDrawingFrame::AHCDrawingFrame (QWidget *parent) :

    QWidget     (parent),
    quadTree    (nullptr)
{

}

void AHCDrawingFrame::setup (int maxPoints)
{
    if (quadTree == nullptr)

        quadTree = new QuadNode (QRect (0, 0,
                                        this -> width  (),
                                        this -> height ()),
                                 maxPoints);
}

void AHCDrawingFrame::mouseReleaseEvent (QMouseEvent *event)
{
    QWidget::mouseReleaseEvent (event);

    quadTree->addPoint (QPoint (event->x (), event->y ()), 1);
//    quadTree->addPoint(QPoint (event->x (),
//                               event->y ()),
//                       100, [](QPoint p1, QPoint p2) -> float
//    {
//        float diffX = p1.x () - p2.x ();
//        float diffY = p1.y () - p2.y ();

//        return sqrt(diffX * diffX + diffY * diffY);
//    });

    repaint ();
}

void AHCDrawingFrame::paintEvent (QPaintEvent *event)
{   
    QWidget::paintEvent (event);

    class TraverseWithPaint
    {

    public:

        TraverseWithPaint (QPainter *painter) : paint (painter) { }
        void operator () (QuadNode *curr)
        {
            if (curr->hasAllChildren ())
            {
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

            for (int i = 0; i < curr->points.size (); i++)
            {
                paint-> setBrush    (QBrush (Qt::black));
                paint-> drawEllipse (QPoint (curr->points[i].x  (),
                                             curr->points[i].y  ()),
                                     2, 2);
            }
        }

    private:

        QPainter *paint;
    };

    if (quadTree == nullptr) return;

    QPainter *paint = new QPainter (this);

    paint-> setBrush    (QBrush (Qt::black));

    QVector <QPoint> corners = {quadTree->bounds.topLeft        (),
                                quadTree->bounds.bottomLeft     (),
                                quadTree->bounds.bottomRight    (),
                                quadTree->bounds.topRight       (),
                                quadTree->bounds.topLeft        ()};

    QPolygon            polygon                 (corners);
    paint               ->drawPolyline          (polygon);
    TraverseWithPaint   ioTraverse              (paint);
    quadTree            ->traverseWithAction    (ioTraverse);
    paint               ->end                   ();
}

void AHCDrawingFrame::clear ()
{
    int maxPoints   = quadTree->maxPoints;
    quadTree        ->deleteChildNodes ();
    quadTree        = nullptr;
    setup           (maxPoints);
    repaint         ();
}

