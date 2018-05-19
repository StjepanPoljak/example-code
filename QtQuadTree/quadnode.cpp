#include "quadnode.h"

QuadNode::QuadNode (QRect rect, int maxPoints) :

    bounds          (rect),
    topLeft         (nullptr),
    bottomLeft      (nullptr),
    bottomRight     (nullptr),
    topRight        (nullptr),
    points          (QVector <QPoint> (0)),
    maxPoints       (maxPoints)
{

}

bool QuadNode::hasAllChildren ()
{
    return (topLeft && bottomLeft && bottomRight && topRight);
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

void QuadNode::addPointWithCondition (QPoint point, bool (*fptr)(QuadNode *))
{
    if (!bounds.contains (point)) { return; }

    if (fptr (this))
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

void QuadNode::addPoint (QPoint point)
{
    this->addPointWithCondition (point, [](QuadNode *node) {

        return (node->points.size () < node->maxPoints && !(node->hasAllChildren ()));
    });
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
