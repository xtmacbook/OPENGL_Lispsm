//Copyright and Disclaimer:
//This code is copyright Vienna University of Technology, 2004.

#ifndef OcclusionH
#define OcclusionH

#include <Engine/EngineBase/KDTree.h>
#include <Engine/EngineBase/Geometry.h>
#include "FrameState.h"

struct Occlusion {
protected:
	//contains the additional data needed for visibility
	struct TreeNodeData {
		bool visible;
		TreeNodeData(): visible(true) { }
	};

	//a KDTree with Geometry objects in the leafes and a TreeNodeData struct at every node
	typedef KDTreeNode<Geometry,TreeNodeData> KDNode;

	//the KD-Tree data structure
	KDNode *kdTree;

	class TreeTraversal : public KDNode::Traversal {
	protected:
		FrameState& fs;
		virtual void traverseChildren(KDNode::Between&) const;
		virtual void onNodeBetween(KDNode::Between&) const;

	public:
		TreeTraversal(FrameState& vfs): fs(vfs) { }
	};

	//KDTree traversal used for View Frustum Culling
	class ViewFrustumCullingTraversal : public TreeTraversal {
	protected:
		virtual void onNodeBetween(KDNode::Between&) const;
		virtual void onLeafNode(KDNode::Leaf&) const;
	public:
		ViewFrustumCullingTraversal(FrameState& vfs): TreeTraversal(vfs) { }
	};

public:
	Occlusion(const std::list<SmpGeometry>&);
	~Occlusion();

	void checkVisibility(FrameState&);
	void renderVisibleTree(FrameState&, bool withState);
	void renderWithCheck(FrameState&, bool withState);

	const KDNode& getKDTree() const { return *kdTree; }

};


#endif