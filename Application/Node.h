#pragma once
#include "BoundingBox.h"
#include "myContainer.h"

template <class objectType>
class Node
{
public:

	Node();
	Node(objectType* content1);
	Node(objectType* content1, objectType* content2);
	Node(myContainer<objectType*> objects, int axis);
	~Node();

	void checkHit(const Ray& ray, objectType*& Object);
	void sort(myContainer<objectType*>& container, int axis);

	bool IsLeaf;
	
	objectType* Content1;
	objectType* Content2;

	Node* LeftNode;
	Node* RightNode;

	BoundingBox<objectType>* BB;
};

template <class objectType>
Node<objectType>::Node() : IsLeaf(true), Content1(nullptr), Content2(nullptr), LeftNode(nullptr), RightNode(nullptr), BB(new BoundingBox<objectType>(Vector3(-1, 0, 1), Vector3(1, 1, -1)))
{
}

template <class objectType>
Node<objectType>::Node(objectType* content1) : IsLeaf(true), Content1(content1), Content2(nullptr), LeftNode(nullptr), RightNode(nullptr), BB(new BoundingBox<objectType>(Vector3(-1, 0, 1), Vector3(1, 1, -1)))
{
}

template <class objectType>
Node<objectType>::Node(objectType* content1, objectType* content2) : IsLeaf(true), Content1(content1), Content2(content2), LeftNode(nullptr), RightNode(nullptr), BB(new BoundingBox<objectType>(Vector3(-1, 0, 1), Vector3(1, 1, -1)))
{
}

template <class objectType>
Node<objectType>::Node(myContainer<objectType*> objects, int axis) : IsLeaf(true), Content1(nullptr), Content2(nullptr), LeftNode(nullptr), RightNode(nullptr), BB(new BoundingBox<objectType>(objects))
{
	switch (objects.getSize())
	{
	case 1:

		Content1 = objects[0];
		break;

	case 2:

		Content1 = objects[0];
		Content2 = objects[1];

		break;

	default:

		IsLeaf = false;

		sort(objects, axis);

		const int N = objects.getSize();

		myContainer<objectType*> leftArray(N / 2);
		for (int i = 0; i < N / 2; i++)
		{
			leftArray[i] = objects[i];
		}

		myContainer<objectType*> rightArray(N - N / 2);
		int j = 0;
		for (int i = N / 2 ; i < N; i++)
		{
			rightArray[j] = objects[i];
			j++;
		}

		LeftNode = new Node(leftArray, (axis + 1) % 3);
		RightNode = new Node(rightArray, (axis + 1) % 3);

		break;
	}
}

template <class objectType>
Node<objectType>::~Node()
{
	delete Content1;
	delete Content2;

	delete LeftNode;
	delete RightNode;

	delete BB;
}

template <class objectType>
void Node<objectType>::checkHit(const Ray& ray, objectType*& Object)
{
	if (!IsLeaf)
	{
		if (LeftNode->BB->Hit(ray))
		{
			LeftNode->checkHit(ray, Object);
		}

		if (RightNode->BB->Hit(ray))
		{
			RightNode->checkHit(ray, Object);
		}
	}

	else
	{
		bool hitLeft = false;
		if (Content1 != nullptr)
		{
			hitLeft = Content1->Hit(ray);
		}

		bool hitRight = false;
		if (Content2 != nullptr)
		{
			hitRight = Content2->Hit(ray);
		}

		if (hitLeft && !hitRight)
		{
			if (Object == nullptr || Content1->t < Object->t)
			{
				Object = Content1;
			}
		}

		else
			if (!hitLeft && hitRight)
			{
				if (Object == nullptr || Content2->t < Object->t)
				{
					Object = Content2;
				}
			}

			else
				if (hitLeft && hitRight)
				{
					if (Content1->t < Content2->t)
					{
						if (Object == nullptr || Content1->t < Object->t)
						{
							Object = Content1;
						}
					}
					else
					{
						if (Object == nullptr || Content2->t < Object->t)
						{
							Object = Content2;
						}
					}
				}
	}
}

template <class objectType>
void Node<objectType>::sort(myContainer<objectType*>& container, int axis)
{
	const int size = container.getSize();

	for (int i = 0; i < size - 1; i++)
	{
		for (int j = size - 1; i < j; --j)
		{
			switch (axis)
			{
			case 0:

				if (container[j]->minPointBB.x < container[j - 1]->minPointBB.x)
				{
					objectType* temp = container[j];
					container[j] = container[j - 1];
					container[j - 1] = temp;
				}

				break;

			case 1:

				if (container[j]->minPointBB.y < container[j - 1]->minPointBB.y)
				{
					objectType* temp = container[j];
					container[j] = container[j - 1];
					container[j - 1] = temp;
				}

				break;

			case 2:

				if (container[j]->minPointBB.z < container[j - 1]->minPointBB.z)
				{
					objectType* temp = container[j];
					container[j] = container[j - 1];
					container[j - 1] = temp;
				}

				break;

			default:

				break;
			}
		}
	}
}
