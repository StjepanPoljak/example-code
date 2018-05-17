#include "quadnode.h"

QuadNode::QuadNode (QRect rect, int maxPoints) :

    bounds          (rect),
    topLeft         (NULL),
    bottomLeft      (NULL),
    bottomRight     (NULL),
    topRight        (NULL),
    points          (QVector <QPoint> (0)),
    maxPoints       (maxPoints)
{

}

bool QuadNode::hasAllChildren ()
{

    if (topLeft     == NULL
    || bottomLeft   == NULL
    || bottomRight  == NULL
    || topRight     == NULL)
    {
        return false;
    }

    return true;
}

void QuadNode::createChildrenIfNeeded ()
{
    if (hasAllChildren ()) { return; }

    int newWidth    = bounds.width  () / 2;
    int newHeight   = bounds.height () / 2;

    topLeft     = new QuadNode (QRect (bounds.left (), bounds.top (),
                                       newWidth, newHeight),
                                maxPoints);

    bottomLeft  = new QuadNode (QRect (bounds.left (), bounds.top () + newHeight,
                                       newWidth, newHeight),
                                maxPoints);

    bottomRight = new QuadNode (QRect (bounds.left () + newWidth, bounds.top () + newHeight,
                                       newWidth, newHeight),
                                maxPoints);

    topRight    = new QuadNode (QRect (bounds.left () + newWidth, bounds.top (),
                                       newWidth, newHeight),
                                maxPoints);
}

void QuadNode::addPoint (QPoint point)
{
    if (!bounds.contains (point)) { return; }

    if (points.size () < maxPoints && !hasAllChildren ())
    {
        points.append (point);
        return;
    }

    createChildrenIfNeeded ();

    for (QPoint prevPoint: points)
    {
        topLeft     -> addPoint (prevPoint);
        bottomLeft  -> addPoint (prevPoint);
        bottomRight -> addPoint (prevPoint);
        topRight    -> addPoint (prevPoint);
    }

    points.clear ();

    topLeft     -> addPoint (point);
    bottomLeft  -> addPoint (point);
    bottomRight -> addPoint (point);
    topRight    -> addPoint (point);
}

void QuadNode::deleteChildNodes ()
{
    if (!hasAllChildren ()) { return; }

    delete topLeft;
    delete bottomLeft;
    delete bottomRight;
    delete topRight;
}

QuadNode::~QuadNode ()
{
    deleteChildNodes ();
}
