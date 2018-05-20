#ifndef QUADNODE_H
#define QUADNODE_H

#include <QRect>
#include <QVector>

class QuadNode
{

public:

    QuadNode                                    (QRect rect, int maxPoints);
    ~QuadNode                                   ();

    QRect               bounds;
    int                 maxPoints;
    QVector <QPoint>    points;

    QuadNode            *topLeft;
    QuadNode            *bottomLeft;
    QuadNode            *bottomRight;
    QuadNode            *topRight;

    void                addPoint                (QPoint point, int maxPoints);
    void                addPoint                (QPoint point, float maxDistance);
    bool                hasAllChildren          ();
    void                createChildrenIfNeeded  ();
    void                deleteChildNodes        ();

    template            <class Lambda>
    void                addPointWithPredicate   (QPoint point, Lambda predicate);

    template            <class Lambda>
    void                traverseWithAction      (Lambda fptr);

    template            <class Lambda>
    void                addPoint                (QPoint point, float maxDistance, Lambda distance);

};

#include "quadnode.tpp"

#endif // QUADNODE_H
