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
    void                addPointWithCondition   (QPoint point, bool (*fptr)(QuadNode *));

};

#endif // QUADNODE_H
