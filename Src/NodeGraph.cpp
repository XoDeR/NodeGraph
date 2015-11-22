#include "NodeGraph.h"

namespace Bt
{
	void NodeGraph::createGraphNode(NodeId nodeId)
	{
		if (graphData.capacity == graphData.size)
		{
			grow();
		}

		const uint32_t last = graphData.size;

		graphData.nodeList[last] = nodeId;
		graphData.parentList[last].i = UINT32_MAX;
		graphData.firstChildList[last].i = UINT32_MAX;
		graphData.nextSiblingList[last].i = UINT32_MAX;
		graphData.prevSiblingList[last].i = UINT32_MAX;

		nodeToGraphMap[nodeId] = GraphId(last);

		++graphData.size;
	}

	void NodeGraph::destroyGraphNode(NodeId nodeId)
	{
		GraphId graphId = getGraphIdByNodeId(nodeId);

		// swap data with the last node

		const uint32_t last = graphData.size - 1;
		const NodeId nodeToRemove = graphData.nodeList[graphId.i];
		const NodeId lastNode = graphData.nodeList[last];

		graphData.nodeList[graphId.i] = graphData.nodeList[last];
		graphData.parentList[graphId.i] = graphData.parentList[last];
		graphData.firstChildList[graphId.i] = graphData.firstChildList[last];
		graphData.nextSiblingList[graphId.i] = graphData.nextSiblingList[last];
		graphData.prevSiblingList[graphId.i] = graphData.prevSiblingList[last];

		nodeToGraphMap[lastNode] = graphId;

		nodeToGraphMap.erase(nodeToRemove);

		--graphData.size;
	}

	GraphId NodeGraph::getGraphIdByNodeId(NodeId nodeId)
	{
		GraphId result;
		auto it = nodeToGraphMap.find(nodeId);
		if (it != nodeToGraphMap.end())
		{
			result = it->second;
		}

		return result;
	}

	uint32_t NodeGraph::getGraphNodeCount() const
	{
		return graphData.size;
	}

	bool NodeGraph::isValid(GraphId i)
	{
		return i.i != UINT32_MAX;
	}

	void NodeGraph::grow()
	{
		graphData.capacity = graphData.capacity * 2 + 1;
		graphData.nodeList.resize(graphData.capacity);
		graphData.parentList.resize(graphData.capacity);
		graphData.firstChildList.resize(graphData.capacity);
		graphData.nextSiblingList.resize(graphData.capacity);
		graphData.prevSiblingList.resize(graphData.capacity);
	}

	void NodeGraph::setParent(GraphId child, GraphId parent)
	{
		clearParent(child);

		if (!isValid(graphData.firstChildList[parent.i]))
		{
			graphData.firstChildList[parent.i] = child;
			graphData.parentList[child.i] = parent;
		}
		else
		{
			GraphId prev;
			GraphId node = graphData.firstChildList[parent.i];
			while (isValid(node))
			{
				prev = node;
				node = graphData.nextSiblingList[node.i];
			}

			graphData.nextSiblingList[prev.i] = child;

			graphData.firstChildList[child.i].i = UINT32_MAX;
			graphData.nextSiblingList[child.i].i = UINT32_MAX;
			graphData.prevSiblingList[child.i] = prev;
		}
	}

	void NodeGraph::clearParent(GraphId child)
	{
		if (!isValid(graphData.parentList[child.i]))
		{
			return;
		}

		if (!isValid(graphData.prevSiblingList[child.i]))
		{
			graphData.firstChildList[graphData.parentList[child.i].i] = graphData.nextSiblingList[child.i];
		}
		else
		{
			graphData.nextSiblingList[graphData.prevSiblingList[child.i].i] = graphData.nextSiblingList[child.i];
		}

		if (isValid(graphData.nextSiblingList[child.i]))
		{
			graphData.prevSiblingList[graphData.nextSiblingList[child.i].i] = graphData.prevSiblingList[child.i];
		}

		graphData.parentList[child.i].i = UINT32_MAX;
		graphData.nextSiblingList[child.i].i = UINT32_MAX;
		graphData.prevSiblingList[child.i].i = UINT32_MAX;
	}

	void NodeGraph::traverseAllChildrenRecursively(GraphId i)
	{
		GraphId child = graphData.firstChildList[i.i];
		while (isValid(child))
		{
			traverseAllChildrenRecursively(child);
			child = graphData.nextSiblingList[child.i];
		}
	}

	void NodeGraph::setParent(NodeId child, NodeId parent)
	{
		GraphId graphChild = getGraphIdByNodeId(child);
		GraphId graphParent = getGraphIdByNodeId(parent);
		// Assert(isValid(graphChild) && isValid(graphParent));
		setParent(graphChild, graphParent);
	}

	NodeId NodeGraph::getRoot()
	{
		// Assert(graphData.size > 0);
		NodeId result;
		result = graphData.nodeList[0];
		return result;
	}

	vector<NodeId> NodeGraph::getChildren(NodeId parent)
	{
		vector<NodeId> result;
		GraphId graphParent = getGraphIdByNodeId(parent);
		// Assert(isValid(graphParent));
		GraphId child = graphData.firstChildList[graphParent.i];
		while (isValid(child))
		{
			result.push_back(graphData.nodeList[child.i]);
			child = graphData.nextSiblingList[child.i];
		}
		return result;
	}

} // namespace Bt
