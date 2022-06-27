#pragma once
#include "Node.h"

template <class objectType>
class BVH
{
public:

	BVH();
	BVH(Node<objectType>* root);
	BVH(myContainer<objectType*> objects);
	~BVH();

	objectType* closest() const;
	bool checkHit(const Ray& ray);

	Node<objectType>* Root;
	objectType* surfacePP;
};

template <class objectType>
BVH<objectType>::BVH() : Root(nullptr), surfacePP(nullptr)
{
}

template <class objectType>
BVH<objectType>::BVH(Node<objectType>* root) : Root(root), surfacePP(nullptr)
{
}

template <class objectType>
BVH<objectType>::BVH(myContainer<objectType*> objects) : surfacePP(nullptr)
{
	Root = new Node<objectType>(objects, 0);
}

template <class objectType>
BVH<objectType>::~BVH()
{
	delete Root;
}

template <class objectType>
objectType* BVH<objectType>::closest() const
{
	return surfacePP;
}

template <class objectType>
bool BVH<objectType>::checkHit(const Ray& ray)
{
	surfacePP = nullptr;

	if (Root->BB->Hit(ray))
	{
		Root->checkHit(ray, surfacePP);
	}

	if (surfacePP == nullptr)
	{
		return false;
	}

	return true;
}
