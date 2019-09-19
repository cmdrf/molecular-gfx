/*	RTree.h
	Copyright 2012-2014 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef RTREE_H
#define RTREE_H

#include <molecular/util/AxisAlignedBox.h>
#include "NonCopyable.h"
#include <vector>
#include <algorithm>

namespace molecular
{
namespace util
{

class RTreeBase
{
protected:
	static const float kReinsertP;
};

/// R* tree
/** @bug Has some memory leaks.
	@deprecated Will be replaced by simpler data structures. */
template<class DataType>
class RTree : public RTreeBase, NonCopyable
{
public:
	class Visitor;

private:
	class Node;
	class Item : NonCopyable
	{
		friend class RTree;
	public:
		virtual ~Item() {}

	private:
		Item(const AxisAlignedBox& bounds, Node* parent = nullptr) : mBounds(bounds), mParent(parent) {}
		
		virtual bool CheckIntegrity() = 0;
		virtual void Print(int level) = 0;

		virtual void Visit(Visitor& visitor) = 0;

		AxisAlignedBox mBounds;
		Node* mParent;
	};

public:
	class Leaf : public Item
	{
		friend class RTree;
	public:
		DataType& GetData() {return mData;}
		const DataType& GetData() const {return mData;}
		bool CheckIntegrity() override;
		void Print(int level) override;

	private:
		Leaf(DataType data, const AxisAlignedBox& bounds) : Item(bounds), mData(data) {}

		virtual void Visit(Visitor& visitor) override {visitor.Visit(mData);}

		DataType mData;
	};

	/// Visitor base class
	/** Derive this to use queries. Implement either Visit(Leaf*) or
		Visit(DataType&), not both. */
	class Visitor
	{
	public:
		virtual ~Visitor() {}
		virtual void Visit(Leaf* leaf) {Visit(leaf->GetData());}
		virtual void Visit(DataType& data) {}
	};

	/// Output of an acceptor functor for Query()
	enum AcceptorOutput
	{
		kDescend,
		kAccept, ///< Accept all children and sub-children without further inspection
		kDiscard
	};

	RTree();
	~RTree();

	/// Insert leaf
	/** @returns Pointer to a Leaf which can later be passed to Remove() or Update(). */
	Leaf* Insert(DataType data, const AxisAlignedBox& bounds);

	/// Remove leaf
	/** @param leaf Pointer to the Leaf returned by Insert(). The pointer is
			invalid after this operation. */
	void Remove(Leaf* leaf);

	/// Update bounds of a leaf
	void Update(Leaf* leaf, const AxisAlignedBox& newBounds);

	/// Query by bounds
	void Query(const AxisAlignedBox& bounds, Visitor& visitor);

	/// Query by arbitrary shape
	/** See BoxAcceptor for an example on an acceptor. */
	template<class Acceptor>
	void Query(Acceptor& acceptor, Visitor& visitor);

	bool CheckIntegrity() {return !mRoot || mRoot->CheckIntegrity();}
	void Print();

	/// Basic acceptor for R-tree queries
	/** Uses a box shape. */
	class BoxAcceptor
	{
	public:
		BoxAcceptor(const AxisAlignedBox& box) : mBox(box) {}
		AcceptorOutput operator() (const AxisAlignedBox& box)
		{
			if(mBox.Encloses(box))
				return kAccept;
			else if(mBox.Overlaps(box))
				return kDescend;
			else
				return kDiscard;
		}

	private:
		AxisAlignedBox mBox;
	};

private:
	static const unsigned int kMinChildren = 4;
	static const unsigned int kMaxChildren = 8;

	class Node : public Item
	{
	public:
		Node(const AxisAlignedBox& bounds, Node* parent) : Item(bounds, parent){}
		~Node()
		{
			for(Item* child: mChildren)
				delete child;
		}

		bool CheckIntegrity() override;
		void Print(int level) override;

		void Visit(Visitor& visitor) override
		{
			for(Item* child: mChildren)
				child->Visit(visitor);
		}

		bool mOnlyLeafChildren;
		std::vector<Item*> mChildren;
	};

	/// Comparator for CS2 (ChooseSubtree)
	class CompareNeededEnlargement : public std::binary_function<Item*, Item*, bool>
	{
	public:
		CompareNeededEnlargement(const AxisAlignedBox& box) : mBox(box) {}
		bool operator()(Item* item1, Item* item2);
	private:
		const AxisAlignedBox& mBox;
	};

	class CompareEdge : public std::binary_function<Item*, Item*, bool>
	{
	public:
		CompareEdge(int axis, bool minEdge) : mAxis(axis), mMinEdge(minEdge) {assert(axis >= 0 && axis < 3);}
		bool operator()(Item* item1, Item* item2);
	private:
		int mAxis, mMinEdge;
	};

	class CompareCenterDistance : public std::binary_function<Item*, Item*, bool>
	{
	public:
		CompareCenterDistance(const Vector3& ref) : mRef(ref) {}
		bool operator()(Item* item1, Item* item2);
	private:
		Vector3 mRef;
	};

	void Insert(Leaf* leaf, bool first = true);
	void Remove(Leaf* leaf, bool deleteLeaf);

	/// Internal recursive query method
	template<class Acceptor>
	void Query(Acceptor& acceptor, Visitor& visitor, Item* node);

	Node* ChooseSubtree(const AxisAlignedBox& box);

	/** @returns Newly created Node if a split was performed, NULL otherwise. */
	Node* OverflowTreatment(Node* level, bool firstInvocation);

	/** @returns Newly created Node if a split was performed, NULL otherwise. */
	Node* Split(Node* node);

	void Reinsert(Node* node);
	void RemoveLeaves(Node* parent, std::vector<Item*>& removedLeaves);

	void ChooseSplitAxisAndIndex(Node* node, int& chosenAxis, int& chosenIndex, int& chosenEdge);

	Node* mRoot;
};

template<class DataType>
bool RTree<DataType>::Leaf::CheckIntegrity()
{
	return this->mParent && static_cast<Node*>(this->mParent)->mOnlyLeafChildren;
}

template<class DataType>
void RTree<DataType>::Leaf::Print(int level)
{
	for(int i = 0; i < level; ++i)
		std::cout << "  ";
	std::cout << "Leaf " << " " << this->mBounds.GetMin() << " " << this->mBounds.GetMax() << std::endl;
}


template<class DataType>
RTree<DataType>::RTree()
{
	mRoot = nullptr;
}

template<class DataType>
RTree<DataType>::~RTree()
{
	delete mRoot;
}

template<class DataType>
typename RTree<DataType>::Leaf* RTree<DataType>::Insert(DataType data, const AxisAlignedBox& bounds)
{
	Leaf* leaf = new Leaf(data, bounds);
	if(mRoot)
		Insert(leaf, mRoot);
	else
	{
		mRoot = new Node(bounds, nullptr);
		mRoot->mOnlyLeafChildren = true;
		mRoot->mChildren.push_back(leaf);
		leaf->mParent = mRoot;
	}
	return leaf;
}

template<class DataType>
void RTree<DataType>::Remove(Leaf* item)
{
	Remove(item, true);
}

template<class DataType>
void RTree<DataType>::Update(Leaf* item, const AxisAlignedBox& newBounds)
{
	assert(item && item->mParent);
	if(item->mParent->mBounds.Encloses(newBounds))
		item->mBounds = newBounds;
	else
	{
		Remove(item, false);
		item->mBounds = newBounds;
		Insert(item);
	}
}

template<class DataType>
void RTree<DataType>::Query(const AxisAlignedBox& bounds, Visitor& visitor)
{
	BoxAcceptor acceptor(bounds);
	Query(acceptor, visitor);
}


template<class DataType>
template<class Acceptor>
void RTree<DataType>::Query(Acceptor& acceptor, Visitor& visitor)
{
	if(mRoot)
		Query(acceptor, visitor, mRoot);
}

template<class DataType>
void RTree<DataType>::Print()
{
	if(mRoot)
		mRoot->Print(0);
}

template<class DataType>
bool RTree<DataType>::Node::CheckIntegrity()
{
	if(this->mParent && mChildren.size() < kMinChildren)
		return false;
	for(Item* child: mChildren)
	{
		if(child->mParent != this)
			return false;
		if(!this->mBounds.Encloses(child->mBounds))
				return false;
		if(!child->CheckIntegrity())
			return false;
	}
	return true;
}

template<class DataType>
void RTree<DataType>::Node::Print(int level)
{
	for(int i = 0; i < level; ++i)
		std::cout << "  ";
	std::cout << "Node " << this->mBounds.GetMin() << " " << this->mBounds.GetMax() << std::endl;
	for(Item* child: mChildren)
	{
		child->Print(level + 1);
	}
}


template<class DataType>
bool RTree<DataType>::CompareNeededEnlargement::operator()(Item* item1, Item* item2)
{
	assert(item1 && item2);
	AxisAlignedBox stretchedBox1 = item1->mBounds;
	AxisAlignedBox stretchedBox2 = item2->mBounds;
	stretchedBox1.Stretch(mBox);
	stretchedBox2.Stretch(mBox);
	// Compare difference between original volume and volume after stretch:
	return (stretchedBox1.Volume() - item1->mBounds.Volume() < stretchedBox2.Volume() - item2->mBounds.Volume());
}

template<class DataType>
bool RTree<DataType>::CompareEdge::operator()(Item* item1, Item* item2)
{
	assert(item1 && item2);
	if(mMinEdge)
		return (item1->mBounds.GetMin(mAxis) < item2->mBounds.GetMin(mAxis));
	else
		return (item1->mBounds.GetMax(mAxis) < item2->mBounds.GetMax(mAxis));
}

template<class DataType>
bool RTree<DataType>::CompareCenterDistance::operator()(Item* item1, Item* item2)
{
	double dist1 = mRef.DistanceSquared(item1->mBounds.Center());
	double dist2 = mRef.DistanceSquared(item2->mBounds.Center());
	return (dist1 < dist2);
}

template<class DataType>
void RTree<DataType>::Insert(Leaf* leaf, bool first)
{
	/* I1: Invoke ChooseSubtree, with the level as a parameter, to find an
	 * appropriate node N, in which to place the new entry E: */
	Node* level = ChooseSubtree(leaf->mBounds);
	level->mChildren.push_back(leaf);
	leaf->mParent = level;

	/* I4: Adjust all covering rectangles in the insertion path such that they
	 *	are minimum bounding boxes enclosing their children rectangles: */
	Node* pathNode = level;
	do
	{
		pathNode->mBounds.Stretch(leaf->mBounds);
		pathNode = pathNode->mParent;
	}while(pathNode);

	/* I2: If N has less than M entries, accommodate E in N. If N has M
	 * entries, invoke OverflowTreatment with the level of N as a parameter
	 * [for reinsertion or split]. */
	if(level->mChildren.size() >= kMaxChildren)
	{
		/* I3: If OverflowTreatment was called and a split was performed, propagate
		 * OverflowTreatment upwards if necessary. */
		Node* toSplit = level;
		while(Node* newNode = OverflowTreatment(toSplit, first))
		{
			/* If OverflowTreatment caused a split of the root, create a new root. */
			if(toSplit == mRoot)
			{
				Node* newRoot = new Node(AxisAlignedBox(), nullptr);
				newRoot->mOnlyLeafChildren = false;
				mRoot->mParent = newRoot;
				newNode->mParent = newRoot;
				newRoot->mChildren.push_back(mRoot);
				newRoot->mChildren.push_back(newNode);
				newRoot->mBounds.Stretch(mRoot->mBounds);
				newRoot->mBounds.Stretch(newNode->mBounds);
				mRoot = newRoot;
				break;
			}

			toSplit->mParent->mChildren.push_back(newNode);
			newNode->mParent = toSplit->mParent;
			if(toSplit->mParent->mChildren.size() < kMaxChildren)
				break;

			toSplit = toSplit->mParent;
		}
	}
}

template<class DataType>
void RTree<DataType>::Remove(Leaf* leaf, bool deleteLeaf)
{
	assert(leaf && leaf->mParent);
	std::vector<Item*> itemsToReinsert;
	Item* cur = leaf;
	typename std::vector<Item*>::iterator it;
	while(cur->mParent)
	{
		std::vector<Item*>& siblings = cur->mParent->mChildren;
		for(it = siblings.begin(); it != siblings.end(); ++it)
		{
			if(*it == cur)
			{
				siblings.erase(it);
				break;
			}
		}

		if(siblings.size() >= kMinChildren || cur->mParent == mRoot)
			break;
		RemoveLeaves(cur->mParent, itemsToReinsert);
		// Fehler?
		Item* toDelete = cur;
		cur = cur->mParent; // Parent is empty now, so delete it too
		if(toDelete != leaf)
			delete toDelete;
	}
	if(deleteLeaf)
		delete leaf;

	for(it = itemsToReinsert.begin(); it != itemsToReinsert.end(); ++it)
		Insert(static_cast<Leaf*>(*it));
}

template<class DataType>
template<class Acceptor>
void RTree<DataType>::Query(Acceptor& acceptor, Visitor& visitor, Item* item)
{
	assert(item);
	AcceptorOutput output = acceptor(item->mBounds);
	if(output == kAccept || (output == kDescend && item->mParent && item->mParent->mOnlyLeafChildren))
		item->Visit(visitor);
	else if(output == kDescend)
	{
		Node* node = static_cast<Node*>(item);
		for(Item* child: node->mChildren)
			Query(acceptor, visitor, child);
	}
}

template<class DataType>
typename RTree<DataType>::Node* RTree<DataType>::ChooseSubtree(const AxisAlignedBox& box)
{
	/* CS1: Set N to be the root. */
	Node* node = mRoot;
	while(!node->mOnlyLeafChildren)
	{
		/* CS2: choose the leaf in N whose rectangle needs least overlap enlargement
		 * to include the new data rectangle. Resolve ties by choosing the leaf
		 * whose rectangle needs least area enlargement, then the leaf with the
		 * rectangle of smallest area. */
		node = static_cast<Node*>(*std::min_element(node->mChildren.begin(), node->mChildren.end(), CompareNeededEnlargement(box)));
		/* CS3: Set N to be the childnode pointed to by the childpointer of the
		 * chosen entry and repeat from CS2 */
	}
	return node;
}

template<class DataType>
typename RTree<DataType>::Node* RTree<DataType>::OverflowTreatment(Node* level, bool firstInvocation)
{
	assert(level);
	if(level != mRoot && firstInvocation)
	{
		Reinsert(level);
		return nullptr;
	}
	else
	{
		return Split(level);
	}
}

template<class DataType>
typename RTree<DataType>::Node* RTree<DataType>::Split(Node* node)
{
	assert(node);
	int axis, index, edge;
	ChooseSplitAxisAndIndex(node, axis, index, edge); // S1 + S2
	std::partial_sort(node->mChildren.begin(), node->mChildren.begin() + index, node->mChildren.end(), CompareEdge(axis, edge == 0));

	Node* newNode = new Node(AxisAlignedBox(), node->mParent);
	newNode->mChildren.assign(node->mChildren.begin() + index, node->mChildren.end());
	node->mChildren.erase(node->mChildren.begin() + index, node->mChildren.end());
	newNode->mOnlyLeafChildren = node->mOnlyLeafChildren;

	node->mBounds = AxisAlignedBox();
	for(Item* child: node->mChildren)
		node->mBounds.Stretch(child->mBounds);

	for(Item* child: newNode->mChildren)
	{
		newNode->mBounds.Stretch(child->mBounds);
		child->mParent = newNode;
	}
	return newNode;
}

template<class DataType>
void RTree<DataType>::Reinsert(Node* node)
{
	assert(node->mOnlyLeafChildren);
	int p = int(node->mChildren.size() * kReinsertP);
	if(p == 0)
		p = 1;
	/* RI1: For all M+1 entries of a node N, compute the distance between the
	 * centers of their rectangles and the center of the bounding rectangle of N
	 * RI2: Sort the entries in decreasing order of their distances computed in RI1 */
	std::partial_sort(node->mChildren.begin(), node->mChildren.end() - p, node->mChildren.end(), CompareCenterDistance(node->mBounds.Center()));

	/* RI3: Remove the first p entries from N and adjust the bounding rectangle of N */
	std::vector<Item*> removedNodes;
	removedNodes.assign(node->mChildren.end() - p, node->mChildren.end());
	node->mChildren.erase(node->mChildren.end() - p, node->mChildren.end());
	node->mBounds = AxisAlignedBox();
	typename std::vector<Item*>::iterator it;
	for(it = node->mChildren.begin(); it != node->mChildren.end(); ++it)
		node->mBounds.Stretch((*it)->mBounds);

	/* RI4: In the sort, defined in R12, starting with the maximum distance
		(= far reinsert) or minimum distance (= close reinsert), invoke Insert
		to reinsert the entries. */
	for(it = removedNodes.begin(); it != removedNodes.end(); ++it)
		Insert(static_cast<Leaf*>(*it), false);
}


template<class DataType>
void RTree<DataType>::RemoveLeaves(Node* parent, std::vector<Item*>& leaves)
{
	if(parent->mOnlyLeafChildren)
	{
		leaves.insert(leaves.end(), parent->mChildren.begin(), parent->mChildren.end());
		parent->mChildren.clear();
	}
	else
	{
		typename std::vector<Item*>::iterator it;
		for(it = parent->mChildren.begin(); it != parent->mChildren.end(); ++it)
		{
			RemoveLeaves(static_cast<Node*>(*it), leaves);
		}
	}
}

template<class DataType>
void RTree<DataType>::ChooseSplitAxisAndIndex(Node* node, int& chosenAxis, int& chosenIndex, int& chosenEdge)
{
	size_t numChildren = node->mChildren.size();
	double chosenMargin = 0.0;
	chosenAxis = -1;
	double chosenOverlap = 0.0;
	chosenIndex = -1;
	chosenEdge = 0;

	for(int axis = 0; axis < 3; ++axis)
	{
		for(int edge = 0; edge < 2; ++edge)
		{
			std::sort(node->mChildren.begin(), node->mChildren.end(), CompareEdge(axis, edge == 0));
			for(size_t dist = kMinChildren; dist < numChildren - kMinChildren + 1; ++dist)
			{
				AxisAlignedBox r1, r2;
				for(size_t i = 0; i < dist; ++i)
					r1.Stretch(node->mChildren[i]->mBounds);
				for(size_t i = dist+1; i < numChildren; ++i)
					r2.Stretch(node->mChildren[i]->mBounds);

				double margin = r1.GetSize(0) + r1.GetSize(1) + r1.GetSize(2)
						+ r2.GetSize(0) + r2.GetSize(1) + r2.GetSize(2);

				/* CSA2: Choose the axis with the minimum S as split axis: */
				if(chosenAxis == -1 || chosenMargin > margin)
				{
					/* CSI1: Along the chosen split axis, choose the
					 * distribution with the minimum overlap-value. */
					double overlap = r1.Overlap(r2);
					if(chosenIndex == -1 || overlap < chosenOverlap || axis != chosenAxis)
					{
						chosenOverlap = overlap;
						chosenIndex = dist;
					}

					chosenMargin = margin;
					chosenAxis = axis;
					chosenEdge = edge;
				}
			}
		}
	}

	assert(chosenAxis >= 0);
	assert(chosenIndex >= 0);
}

}
}

#endif // RTREE_H
