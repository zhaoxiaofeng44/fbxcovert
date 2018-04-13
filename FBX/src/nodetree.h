#pragma once

#include <stdlib.h>


template<class T>
class NodeTree
{
public:

	NodeTree()
	{
		mRoot = NULL;
		mEnd = NULL;
		mCount = 0;
	}

	NodeTree(const NodeTree &other)
		: NodeTree()
	{
		Copy(other);
	}

	virtual ~NodeTree()
	{
		Clear();
	}

	virtual void Clear()
	{
		if (mRoot) {
			delete mRoot;
			mRoot = NULL;
		}
		mCount = 0;
	}

	virtual void Copy(const NodeTree &other)
	{
		if (other.mRoot) {
			if (mRoot) {
				mRoot->Copy(*other.mRoot);
			}
			else {
				AddChild(NULL, new T(*other.mRoot));
			}
		}
	}

	virtual T* GetRoot()
	{
		return mRoot;
	}

	virtual void AddChild(
		T *parent,
		T *item
	)
	{
		if (parent)
		{
			parent->AddChild(item);
		}
		else
		{
			if (mRoot)
			{
				mRoot->AddChild(item);
			}
			else
			{
				mRoot = item;
				mEnd = item;
			}
		}
		mCount ++;
	}

	virtual T* GetNextChildFirst(
		T *item
	)
	{
		if (item->mFirstChild)
		{
			return item->mFirstChild;
		}

		if (item->mNext)
		{
			return item->mNext;
		}

		for (T *parent = item->mParent; parent != NULL; )
		{
			if (parent->mNext)
			{
				return parent->mNext;
			}

			parent = parent->mParent;
		}

		return NULL;
	}

	int Size()
	{
		return mCount;
	}

	NodeTree& operator =(const NodeTree& other)
	{
		Copy(other);
		return *this;
	}

	T *mRoot;
	T *mEnd;
private:
	int mCount;
};

template<class T>
class NodeTreeItem
{
	friend class NodeTree<T>;
public:
	NodeTreeItem()
	{
		mNext = NULL;
		mPrevious = NULL;
		mParent = NULL;
		mFirstChild = NULL;
		mEndChild = NULL;
	}

	NodeTreeItem(const NodeTreeItem & other)
		: NodeTreeItem()
	{
		Copy(other);
	};

	virtual ~NodeTreeItem()
	{
		Clear();
	};

	virtual void Clear()
	{
		T *pItem = mFirstChild;
		while (pItem) {
			T *temp = pItem;
			pItem = pItem->mNext;
			delete temp;
		}
		mFirstChild = NULL;
		mEndChild = NULL;
	}

	virtual void Copy(const NodeTreeItem & other)
	{
		T *pItem = other.mFirstChild;
		while (pItem){
			AddChild(new T(*pItem));
			pItem = pItem->mNext;
		}
	}

	virtual void AddChild(T *item)
	{
		if (mEndChild)
		{
			mEndChild->mNext = item;
			mPrevious = mEndChild;
			mNext = NULL;
			mEndChild = mEndChild->mNext;
		}
		else
		{
			mNext = NULL;
			mPrevious = NULL;
			mFirstChild = item;
			mEndChild = mFirstChild;
		}
		item->mParent = (T*)this;
	}

public:
	T *mNext;
	T *mPrevious;
	T *mParent;
	T *mFirstChild;
	T *mEndChild;
};
