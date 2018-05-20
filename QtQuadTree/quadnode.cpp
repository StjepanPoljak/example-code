#include "quadnode.h"

QuadNode::QuadNode (QRect rect, int maxPoints) :

    bounds          (rect),
    maxPoints       (maxPoints),
    points          (QVector <QPoint> (0)),
    topLeft         (nullptr),
    bottomLeft      (nullptr),
    bottomRight     (nullptr),
    topRight        (nullptr)
{

}

bool QuadNode::hasAllChildren ()
{
    return topLeft && bottomLeft && bottomRight && topRight;
}

void QuadNode::createChildrenIfNeeded ()
{
    if (hasAllChildren ()) { return; }

    float newWidth    = bounds.width  () / 2;
    float newHeight   = bounds.height () / 2;

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

void QuadNode::addPoint (QPoint point, int maxPoints)
{
    struct AddWithMaxPoints
    {
        int maxPoints;

        AddWithMaxPoints (int max) : maxPoints (max) { };

        bool operator () (QuadNode *curr)
        {
            return curr->points.size () < this->maxPoints;
        }
    };

    this->addPointWithPredicate (point, AddWithMaxPoints (maxPoints));
}

void QuadNode::deleteChildNodes ()
{
    if (!hasAllChildren ()) return;

    delete topLeft;
    delete bottomLeft;
    delete bottomRight;
    delete topRight;
}

QuadNode::~QuadNode ()
{
    deleteChildNodes ();
}
