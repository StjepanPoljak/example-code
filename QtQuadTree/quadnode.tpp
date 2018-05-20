
template <class Lambda>
void QuadNode::addPointWithPredicate (QPoint point, Lambda predicate)
{
    QVector <QuadNode *> stack = { this };

    do
    {
        QuadNode *curr = stack.last ();

        stack.remove (stack.size () - 1);

        if (!curr->bounds.contains (point)) continue;

        if (predicate (curr) && !curr->hasAllChildren ())
        {
            curr->points.append (point);
            return;
        }

        curr->createChildrenIfNeeded ();

        for (QPoint prevPoint: curr->points)
        {
            if (curr->topLeft       ->bounds.contains   (prevPoint))
                curr->topLeft       ->points.append     (prevPoint);
            if (curr->bottomLeft    ->bounds.contains   (prevPoint))
                curr->bottomLeft    ->points.append     (prevPoint);
            if (curr->bottomRight   ->bounds.contains   (prevPoint))
                curr->bottomRight   ->points.append     (prevPoint);
            if (curr->topRight      ->bounds.contains   (prevPoint))
                curr->topRight      ->points.append     (prevPoint);
        }

        curr->points.clear  ();
        stack.clear         ();
        stack.append        (curr->topLeft);
        stack.append        (curr->bottomLeft);
        stack.append        (curr->bottomRight);
        stack.append        (curr->topRight);

    } while (!stack.isEmpty ());
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
