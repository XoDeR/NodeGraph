#include "NodeGraph.h"

void main()
{
	using namespace Bt;

	NodeId root;
	root.i = 1;
	NodeId child1;
	root.i = 2;

	NodeGraph graph;
	graph.createGraphNode(root);
}