//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2005.
#ifndef KDTreeH
#define KDTreeH
#include <list>
#include <algorithm>
#include <Base/System.h>
#include <Base/Persistent.h>
#include <Base/SmartPointer.h>
#include <Mathematic/Geometry/GeometryIO.h> //debug

template<class LEAFDATA, class NODEDATA> struct KDTreeLeafNode;
template<class LEAFDATA, class NODEDATA> class KDTreeNodeBetween;

//LEAFDATA: is some user class that is only present in the leaf nodes
//the tree stores a smartpointer to the real data, so he doesn't ownes it
//this class needs a getBoundingBox function
//NODEDATA: is some userdata that is stored at every node
//the tree ownes this data.
template<class LEAFDATA, class NODEDATA>
struct KDTreeNode : public Persistent {
	typedef KDTreeNode<LEAFDATA,NODEDATA> Node;
protected:
	Node *parent;

public:
	typedef KDTreeLeafNode<LEAFDATA,NODEDATA> Leaf;
	typedef KDTreeNodeBetween<LEAFDATA,NODEDATA> Between;
	typedef SmartPointer<LEAFDATA> SmpLeafData;

	NODEDATA nodeData;

	KDTreeNode(Node* vParent): parent(vParent) { }
	virtual ~KDTreeNode() { }
	typedef typename LEAFDATA::AABox AABox;
	
	struct const_Traversal {
		virtual void traverseChildren(const Between& n) const {
			n.getChildSmall().traversal(*this);
			n.getChildBig().traversal(*this);
		}

		virtual void onNodeBetween(const Between&) const = 0;
		virtual void onLeafNode(const Leaf&) const = 0;
	};

	struct Traversal {
		virtual void traverseChildren(Between& n) const {
			n.getChildSmallPtr()->traversal(*this);
			n.getChildBigPtr()->traversal(*this);
		}

		virtual void onNodeBetween(Between&) const = 0;
		virtual void onLeafNode(Leaf&) const = 0;
	};

	virtual void traversal(const const_Traversal&) const = 0;
	virtual void traversal(const Traversal&) = 0;
	virtual bool isLeaf() const = 0;
	virtual const AABox& getBoundingBox() const = 0;
	virtual const unsigned depthMax() const = 0;
	virtual std::ostream& put(std::ostream& s) const { 
		return s << getBoundingBox(); 
//		const AABox::V3 min = getBoundingBox().getMin();
//		const AABox::V3 max = getBoundingBox().getMax();
//		return s << '(' << min[0] << ',' << min[2] << ';' << max[0] << ',' << max[2] << ')'; 
	}

	Node* getParent() const { return parent; }

	unsigned getDepth() const { 
		Node *p = getParent();
		unsigned i = 0;
		while(0 != p) {
			i++;
			p = p->getParent();
		}
		return i;
	}
};


template<class LEAFDATA, class NODEDATA>
struct KDTreeLeafNode : public KDTreeNode<LEAFDATA,NODEDATA> {
	const SmpLeafData data;
	KDTreeLeafNode(Node* vParent, const SmpLeafData& vData): 
		Node(vParent), data(vData) { }

	virtual void traversal(const const_Traversal& tr) const {
		tr.onLeafNode(*this);
	}

	virtual void traversal(const Traversal& tr) {
		tr.onLeafNode(*this);
	}

	virtual bool isLeaf() const { return true; }
	virtual const AABox& getBoundingBox() const { return data->getBoundingBox(); }
	virtual const unsigned depthMax() const { return 0; }
	virtual std::ostream& put(std::ostream& s) const { s << "L:"; return Node::put(s); } 

	template<class NODEDATA2> 
	KDTreeLeafNode<LEAFDATA,NODEDATA2>* copyStructure(KDTreeNode<LEAFDATA,NODEDATA2>* parent) {
		return new KDTreeLeafNode<LEAFDATA,NODEDATA2>(parent,data);
	}
};


template<class LEAFDATA, class NODEDATA>
class KDTreeNodeBetween : public KDTreeNode<LEAFDATA,NODEDATA> {
protected:
	typedef std::list<SmpLeafData> LstData;
	typedef typename AABox::V3::ElementType R;
	
	friend const_Traversal;

	class Compare {
		const unsigned id;
	public:
		Compare(const unsigned vID): id(vID) { }
		bool operator()(const SmpLeafData& a, const SmpLeafData& b) const {
			return a->getBoundingBox().getCenter()[id] < b->getBoundingBox().getCenter()[id];
		}
	};

	class SubNodeData {
		AABox* bb[2];
		bool smallerIncreaseWithA(const unsigned i, const AABox& a, const AABox& b, 
								  const AABox& input) {
			AABox::V3::ElementType diffA = Math::abs(input.getMax()[i]-a.getMax()[i]);
			AABox::V3::ElementType diffB = Math::abs(b.getMin()[i]-input.getMin()[i]);
			return diffA < diffB;
			//volume based:
			//return (a+input).volume() < (b+input).volume();
		}

		void moveToEnd(AABox& bb, LstData& output, LstData& data, typename LstData::iterator i) {
			LEAFDATA& g = **i;
			bb += g.getBoundingBox();
			output.splice(output.end(),data,i);
		}

		void moveToBegin(AABox& bb, LstData& output, LstData& data, typename LstData::iterator i) {
			LEAFDATA& g = **i;
			bb += g.getBoundingBox();
			output.splice(output.begin(),data,i);
		}

		void addToNearer(LstData& data, typename LstData::iterator i) {
			LEAFDATA& g = **i;
			if(smallerIncreaseWithA(axe,*bb[0],*bb[1],g.getBoundingBox())) {
				moveToEnd(*bb[0],small,data,i);
			}
			else {
				moveToBegin(*bb[1],big,data,i);
			}
		}

	public:
		const unsigned axe;
		LstData small;
		LstData big;

		SubNodeData(const unsigned vAxe, const LstData& vData): axe(vAxe) { 
			bb[0] = (bb[1] = 0);
			LstData data(vData);
			data.sort(Compare(axe));

			small.splice(small.end(),data,data.begin());
			bb[0] = new AABox(small.back()->getBoundingBox());

			LstData::iterator it = data.end();
			it--;
			big.splice(big.end(),data,it);
			bb[1] = new AABox(big.back()->getBoundingBox());

			//this creates a balanced (countwise) tree
			const unsigned count = data.size()/2;
			for(unsigned i = 0; i < count; i++) {
				moveToEnd(*bb[0],small,data,data.begin());
				LstData::iterator it = data.end();
				it--;
				moveToBegin(*bb[1],big,data,it);
			}
			if(!data.empty()) {
				addToNearer(data,data.begin());
			}
/*			while(!data.empty()) {
				addToNearer(data,data.begin());
				if(!data.empty()) {
					LstData::iterator it = data.end();
					it--;
					addToNearer(data,it);
				}
			}*/
		}

		~SubNodeData() {
			DMS::saveDelete(bb[0]);
			DMS::saveDelete(bb[1]);
		}

		R volume() const { return bb[0]->volume()+bb[1]->volume(); }

		const AABox& getSmallBox() const { return *bb[0]; }
		const AABox& getBigBox() const { return *bb[1]; }
	};

	Node *small, *big;
	unsigned axe;
	const AABox boundingBox;
public:
    
	KDTreeNodeBetween(Node* vParent, const AABox& bb, unsigned vAxe, Node* sml, Node* bg):
		Node(vParent), boundingBox(bb), axe(vAxe), small(sml), big(bg) { }

	KDTreeNodeBetween(Node* vParent, const AABox& bb, const std::list<SmpLeafData>& data): 
			Node(vParent), boundingBox(bb), axe(0), small(0), big(0) {
		if(data.size() > 1) {
			//create potential sub nodes along the 3 axes
			SubNodeData n[3] = { SubNodeData(0,data), SubNodeData(1,data), SubNodeData(2,data) };
			//decide which one is best
			R min = n[0].volume();
			for(unsigned i = 1; i < 3; i++) {
				R volume(n[i].volume());
				if(volume < min) {
					min = volume;
					axe = i;
				}
			}
			const SubNodeData& sub = n[axe];
			if(sub.small.size() == 1) {
				small = new Leaf(this,sub.small.front());
			}
			else {
				small = new Between(this,sub.getSmallBox(),sub.small);
			}
			if(sub.big.size() == 1) {
				big = new Leaf(this,sub.big.front());
			}
			else {
				big = new Between(this,sub.getBigBox(),sub.big);
			}
		}
	}
    
	virtual ~KDTreeNodeBetween() {
		DMS::saveDelete(small);
		DMS::saveDelete(big);
	}

	virtual const AABox& getBoundingBox() const { return boundingBox; }
	const unsigned getAxe() const { return axe; }
	virtual const unsigned depthMax() const { 
		const unsigned depth1 = getChildSmall().depthMax();
		const unsigned depth2 = getChildBig().depthMax();
		return 1+Math::maximum(depth1,depth2);
	}

	virtual void traversal(const const_Traversal& tr) const {
		tr.onNodeBetween(*this);
	}

	virtual void traversal(const Traversal& tr) {
		tr.onNodeBetween(*this);
	}

	virtual bool isLeaf() const { return false; }

	const Node& getChildSmall() const { return *small; }
	const Node& getChildBig() const { return *big; }
	Node* getChildSmallPtr() const { return small; }
	Node* getChildBigPtr() const { return big; }

	virtual std::ostream& put(std::ostream& s) const { 
		s << "\nB{";
		Node::put(s);
		return s << getChildSmall() << getChildBig() << '}'; 
	}

	//template<class NODEDATA2> 
	//KDTreeNodeBetween<LEAFDATA,NODEDATA2>* copyStructure(KDTreeNodeBetween<LEAFDATA,NODEDATA2>* parent) {
	//	return new KDTreeNodeBetween<LEAFDATA,NODEDATA2>(parent,boundingBox,axe,);
	//}
};

template<class LEAFDATA, class NODEDATA> 
KDTreeNode<LEAFDATA,NODEDATA>* createKDTree(const std::list< SmartPointer<LEAFDATA> >& data) {
	if(data.size() == 0) {
		return 0;
	}
	typedef KDTreeNode<LEAFDATA,NODEDATA> KDTree;
	if(data.size() == 1) {
		return new KDTree::Leaf(0,data.front());
	}

	typedef std::list< SmartPointer<LEAFDATA> > LstData;
	LEAFDATA::AABox box(data.front()->getBoundingBox());
	for(LstData::const_iterator i = data.begin(); i != data.end(); i++) {
		box += (*i)->getBoundingBox();
	}
	return new KDTree::Between(0,box,data);
}

#endif
