//Copyright and Disclaimer:
//This code is copyright Vienna University of Technology, 2005.
#include "Occlusion.h"

Occlusion::Occlusion(const std::list<SmpGeometry>& data) {
	kdTree = createKDTree<Geometry,TreeNodeData>(data);
}

Occlusion::~Occlusion() {
	delete kdTree;
}

void Occlusion::TreeTraversal::onNodeBetween(KDNode::Between& n) const {
	if(n.nodeData.visible) {
		traverseChildren(n);
	}
}

void Occlusion::TreeTraversal::traverseChildren(KDNode::Between& n) const {
	typedef Math::Vector3f V3;
	const float distSmall = n.getChildSmall().getBoundingBox().squaredDistance(fs.getPosF());
	const float distBig = n.getChildBig().getBoundingBox().squaredDistance(fs.getPosF());
	//traverse nearer child first
	if(distSmall < distBig) {
		n.getChildSmallPtr()->traversal(*this);
		n.getChildBigPtr()->traversal(*this);
	}
	else {
		n.getChildBigPtr()->traversal(*this);
		n.getChildSmallPtr()->traversal(*this);
	}
}


void Occlusion::ViewFrustumCullingTraversal::onNodeBetween(KDNode::Between& n) const {
	Frustum::ActivePlanes a(fs.frustum.getActivePlanes());
	n.nodeData.visible = !fs.frustum.cull(n.getBoundingBox());
	TreeTraversal::onNodeBetween(n);
	fs.frustum.getActivePlanes() = a;
}

void Occlusion::ViewFrustumCullingTraversal::onLeafNode(KDNode::Leaf& n) const {
	Geometry& g = *n.data;
	n.nodeData.visible = fs.frustum.visible(g.getBoundingBox());
}

void Occlusion::checkVisibility(FrameState& fs) {
	kdTree->traversal(ViewFrustumCullingTraversal(fs));
}

void Occlusion::renderVisibleTree(FrameState& fs, bool withState) {
	class RenderVisibleTreeGeometry : public TreeTraversal {
	protected:
		virtual void onLeafNode(KDNode::Leaf& n) const {
			if(n.nodeData.visible) {
				n.data->drawGeometry(fs);
			}
		}

	public:
		RenderVisibleTreeGeometry(FrameState& vfs): TreeTraversal(vfs) { }
	};

	class RenderVisibleTree : public TreeTraversal {
	protected:
		virtual void onLeafNode(KDNode::Leaf& n) const {
			if(n.nodeData.visible) {
				n.data->draw(fs);
			}
		}

	public:
		RenderVisibleTree(FrameState& vfs): TreeTraversal(vfs) { }
	};

	if(withState) {
		kdTree->traversal(RenderVisibleTree(fs));
	}
	else {
		kdTree->traversal(RenderVisibleTreeGeometry(fs));
	}
}


void Occlusion::renderWithCheck(FrameState& fs, bool withState) {
	class VFCTraversalGeometry : public ViewFrustumCullingTraversal {
	protected:
		virtual void onLeafNode(KDNode::Leaf& n) const {
			ViewFrustumCullingTraversal::onLeafNode(n);
			if(n.nodeData.visible) {
				n.data->drawGeometry(fs);
			}
		}
	public:
		VFCTraversalGeometry(FrameState& vfs): ViewFrustumCullingTraversal(vfs) { }
	};

	class VFCTraversalRender : public ViewFrustumCullingTraversal {
	protected:
		virtual void onLeafNode(KDNode::Leaf& n) const {
			ViewFrustumCullingTraversal::onLeafNode(n);
			if(n.nodeData.visible) {
				n.data->draw(fs);
			}
		}
	public:
		VFCTraversalRender(FrameState& vfs): ViewFrustumCullingTraversal(vfs) { }
	};


	if(withState) {
		kdTree->traversal(VFCTraversalRender(fs));
	}
	else {
		kdTree->traversal(VFCTraversalGeometry(fs));
	}
}

