#include "Node.h"


Node::~Node()
{
    delete firstChild;
    delete nextSibling;
}
