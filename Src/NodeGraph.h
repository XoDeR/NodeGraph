#pragma once

#include "BtCommon.h"

namespace Bt
{
	struct NodeId
	{
		uint32_t i;
	};

	struct GraphId
	{
		GraphId()
			: i(UINT32_MAX)
		{}
		GraphId(uint32_t id)
			: i(id)
		{}
		uint32_t i;
	};

	struct GraphData
	{
		uint32_t size = 0;
		uint32_t capacity = 0;

		vector<NodeId> nodeList;
		vector<GraphId> parentList;
		vector<GraphId> firstChildList;
		vector<GraphId> nextSiblingList;
		vector<GraphId> prevSiblingList;
	};

	struct NodeGraph
	{
	public:
		void createGraphNode(NodeId nodeId);
		void destroyGraphNode(NodeId nodeId);
		GraphId getGraphIdByNodeId(NodeId nodeId);
		uint32_t getGraphNodeCount() const;
		
		void setParent(NodeId child, NodeId parent);
		NodeId getRoot(); // root is assumed to be the first node added (with graph id == 0)
		vector<NodeId> getChildren(NodeId parent);
	private:
		bool isValid(GraphId i);
		void grow();

		void setParent(GraphId child, GraphId parent);
		void clearParent(GraphId child);
		void traverseAllChildrenRecursively(GraphId i);

		GraphData graphData;
		unordered_map<NodeId, GraphId> nodeToGraphMap;
	};

} // namespace Bt
