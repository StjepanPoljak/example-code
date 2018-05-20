#ifndef QUADNODE_H
#define QUADNODE_H

#include <QRect>
#include <QVector>

class QuadNode
{

public:

    QuadNode                                    (QRect rect, int maxPoints);
    void                addPoint                (QPoint point);
    ~QuadNode                                   ();

    QuadNode            *topLeft;
    QuadNode            *bottomLeft;
    QuadNode            *bottomRight;
    QuadNode            *topRight;

    QRect               bounds;
    int                 maxPoints;
    QVector <QPoint>    points;

    bool                hasAllChildren          ();
    void                createChildrenIfNeeded  ();
    void                deleteChildNodes        ();

    template            <class Lambda>
    void                addPointWithPredicate   (QPoint point, Lambda predicate);

    template            <class Lambda>
    void                traverseWithAction      (Lambda fptr);

};

#include "quadnode.tpp"

#endif // QUADNODE_H
