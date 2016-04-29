# C-plus-plus-intrusive-container-templates
Abstract node links, no use of new/delete (only AVL tree available currently).

Also look at boost::instrusive, which is STL-compatible.  Links are in the
Boost approach are unabstracted pointers.  There is no function to build
a balanced binary tree from a sequence, but you could port mine into the
Boost lib pretty easily.

Implementations of hash tables and doubly-linked lists using this approach exist,
but they are Nokia proprietary closed source.  I will write freeware versions
(same license) as time permits.  Or, nag Nokia to open source the existing
implementations.
