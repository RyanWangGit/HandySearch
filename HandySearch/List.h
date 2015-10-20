#pragma once
#include "Exceptions.h"

//#define DEBUG
template<typename T>
class List;

template<typename T>
class ListNode
{
	T data;
	ListNode<T>* next;
	ListNode<T>* prior;
public:
	friend class List<T>;
	ListNode()
	{
		this->next = nullptr;
		this->prior = nullptr;
	}
	ListNode(T& data)
	{
		this->data = data;
		this->next = nullptr;
		this->prior = nullptr;
	}
};


template<typename T>
class List
{
private:
	long length;
	ListNode<T>* head;
	ListNode<T>* tail;
	/* Store the last queried pointer to reduce search time when iterating */
	ListNode<T>* last;
	int lastIndex;
	bool remove(ListNode<T>* p);
	ListNode<T>* List<T>::getNode(int i);
	bool List<T>::removeNode(ListNode<T>* node);
public:
	List();
	~List();
	/* Get the i(th) node of the list (Starting from 0) */
	T& get(int i);
	/* Append a node to the tail */
	List<T>& append(T& data);
	/* Append a new list to the tail */
	List<T>& append(List<T>& list);
	/* Clear the list */
	bool clear();
	/* Returns whether this list is empty or not */
	bool isEmpty();
	/* Remove node in the list */
	bool remove(int i);
	/* Return the index of the value */
	int indexOf(T& value);
	/* Replace i(th) node with the given value */
	bool replace(int i, T& value);
	/* Returns the size of the list */
	long size();
	/* Returns whether the list contains the value */
	bool contains(const T& value);
	/* Insert the value after the i(th) node */
	bool insertAfter(int i, T& value);
	/* Override operator[] to get i(th) node's data */
	T& operator[](int i);
	/* Override operator= to make assign objects */
	List<T>& operator=(List<T>& other);
	
};

template<typename T>
List<T>::List()
{
	this->head = new ListNode<T>();
	this->tail = head;
	this->length = 0;
	this->last = nullptr;
	this->lastIndex = -MAXSHORT;
}

template<typename T>
List<T>::~List()
{
	this->clear();
	delete this->head;
}

template<typename T>
T& List<T>::operator[](int i)
{
	return this->get(i);
}

template<typename T>
List<T>& List<T>::operator=(List<T>& other)
{
	if (this == &other)
		return *this;
	this->clear();
	this->append(other);
	return *this;
}

template<typename T>
ListNode<T>* List<T>::getNode(int i)
{
	//Return the head node if index is -1
	if (i == -1)
	{
		this->last = nullptr;
		this->lastIndex = -MAXSHORT;
		return this->head;
	}
	if (i >= this->size())
		throw new QOutOfBoundaryException();

	//Use of last pointer to optimize when iterating
	//by finding the shortest path to the index queried

	//The distance between target and last queried pointer
	int distances[3] = { i, abs(this->lastIndex - i), this->size() - 1 - i };
	int minDistance = MAXINT;	
	int minIndex;	
	for (int j = 0; j < 3; j++)
	{
		if (minDistance > distances[j])
		{
			minDistance = distances[j];
			minIndex = j;
		}
	}

	//Amount of steps from source node to target node
	int steps = 0;
	//Index of source pointer
	int sourceIndex = 0;
	ListNode<T>* sourcePtr = nullptr;
	switch (minIndex)
	{
	//From head node
	case 0:
		sourceIndex = 0;
		sourcePtr = this->head->next;
		steps = i;
		break;
	//From last-queried node
	case 1:
		sourceIndex = this->lastIndex;
		sourcePtr = this->last;
		steps = abs(this->lastIndex - i);
		break;
	//From tail node
	case 2:
		sourceIndex = this->size();
		sourcePtr = this->tail;
		steps = this->size() - 1 - i;
		break;
	}

	//Walk from source to destination node and return
	for (int j = 0; j <= steps; j++)
	{
		if (sourcePtr == nullptr)
			throw QNullPointerException();
		else if (j == steps)
		{
			this->last = sourcePtr;
			this->lastIndex = i;
			return sourcePtr;
		}
		else
		{
			if (i >= sourceIndex)
				sourcePtr = sourcePtr->next;
			else
				sourcePtr = sourcePtr->prior;
		}
			
	}
}

template<typename T>
T& List<T>::get(int i) 
{
	return this->getNode(i)->data;
}

template<typename T>
bool List<T>::clear()
{
	ListNode<T>* temp = nullptr;
	while (this->tail != this->head)
	{
		temp = this->tail;
		this->tail = this->tail->prior;
		delete temp;
	}
	this->last = nullptr;
	this->lastIndex = -MAXSHORT;

	return true;
}

template<typename T>
List<T>& List<T>::append(T& data)
{
	ListNode<T>* p = this->tail;
	p->next = new ListNode<T>(data);
	
	p->next->prior = p;
	this->tail = p->next;
	this->length++;

	return *this;
}

template<typename T>
List<T>&  List<T>::append(List<T>& list)
{
	for (int i = 0; i < list.size(); i++)
		this->append(list.get(i));

	return *this;
}

template<typename T>
bool List<T>::isEmpty()
{
	return length == 0 ? true : false;
}

template<typename T>
bool List<T>::removeNode(ListNode<T> *p)
{
	if (p == nullptr)
		throw QNullPointerException();

	if (p == this->last)
	{
		if (p->prior == this->head)
		{
			this->last = nullptr;
			this->lastIndex = -MAXSHORT;
		}
		else
		{
			this->last = p->prior;
			this->lastIndex--;
		}
	}

	p->prior->next = p->next;
	if (p->next == nullptr)
		this->tail = p->prior;
	else
		p->next->prior = p->prior;
		
	delete p;
	this->length--;

	return true;
}

template<typename T>
bool List<T>::remove(int i)
{
	return this->removeNode(this->getNode(i));
}


template<typename T>
int List<T>::indexOf(T &value)
{
	ListNode<T>* p = this->head->next;
	int i;
	for (i = 0; i < this->size(); i++)
	{
		if (p == nullptr)
			throw QNullPointerException("in indexOf(T &value) function");

		if (p->data == value)
			break;
		p = p->next;
	}
	return i;
}

template<typename T>
bool List<T>::contains(const T &value)
{ 
	ListNode<T> *p = nullptr;
	for (int i = 0; i < this->size(); i++)
	{
		p = this->getNode(i);
		
		if (p->data == value)
			return true;
	}
	return false;
}

template<typename T>
bool List<T>::insertAfter(int i, T& value)
{
	ListNode<T> *p = nullptr;
	ListNode<T> *temp = nullptr;

	p = this->getNode(i);

	temp = p->next;
	p->next = new ListNode<T>(value); 
	p->next->prior = p;
	p->next->next = temp;
	if (temp != nullptr)
		temp->prior = p->next;

	this->length++;

	return true;
}

template<typename T>
bool List<T>::replace(int i, T& value)
{
	this->remove(i);
	this->insertAfter(i - 1, value);
	return true;
}

template<typename T>
long List<T>::size()
{
	return this->length;
}
