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

    template <class Lambda>
    void traverseWithAction (Lambda fptr)
    {
        QVector <QuadNode *> stack = { this };

        do
        {
            QuadNode *curr = stack.last ();

            fptr (curr);

            stack.remove (stack.size () - 1);

            if (curr->hasAllChildren ())
            {
                stack.append (curr->topLeft);
                stack.append (curr->bottomLeft);
                stack.append (curr->bottomRight);
                stack.append (curr->topRight);
            }
        } while (!stack.isEmpty ());
    }
};

#endif // QUADNODE_H
