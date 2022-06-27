#pragma once
template <class objectType>

class myContainer
{
public:

	int Size;
	objectType* Content;
	
	myContainer() : Size(0), Content(nullptr)
	{}

	myContainer(int size) : Size(size), Content(new objectType[size])
	{}

	myContainer(const myContainer& container) : Size(container.Size), Content(new objectType[container.Size])
	{
		for (int i = 0; i < Size; i++)
		{
			Content[i] = container.Content[i];
		}
	}

	myContainer& operator=(const myContainer& container)
	{
		Size = container.Size;
		Content = new objectType[Size];

		for (int i = 0; i < Size; i++)
		{
			Content[i] = container.Content[i];
		}

		return *this;
	}

	~myContainer()
	{
		delete[] Content;
	}

	void push(objectType content)
	{
		const int newSize = Size + 1;
		objectType* tempArray = new objectType[newSize];
		for (int i = 0; i < Size; i++)
		{
			tempArray[i] = Content[i];
		}

		tempArray[Size] = content;
		
		delete[] Content;
		
		Content = tempArray;
		Size += 1;
	}

	int getSize() const
	{
		return Size;
	}

	int Last() const
	{
		return Size - 1;
	}

	objectType& operator[](int index)
	{
		return Content[index];
	}	
};