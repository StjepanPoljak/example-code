
template <class Lambda>
void QuadNode::addPointWithPredicate (QPoint point, Lambda predicate)
{
    if (!bounds.contains (point)) { return; }

    if (predicate (this))
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

template <class Lambda>
void QuadNode::traverseWithAction (Lambda fptr)
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
