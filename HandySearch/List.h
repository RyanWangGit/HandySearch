#pragma once
#include "Exceptions.h"
//#define DEBUG

//Declaration of List class
template<typename T>
class List;

/**
 * Class:	ListNode
 *
 * Brief:	A node of linked list.
 *
 * Date:	Oct. 2015
 */
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


/**
 * Class:	List
 *
 * Brief:	Fundamental data structure linked list,which is optimized
 * for visiting and iterating.
 *
 * Date:	Oct. 2015
 */
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

	ListNode<T>* List<T>::getNode(int i);
	bool List<T>::removeNode(ListNode<T>* node);
public:
	List();
	~List();
	T& operator[](int i);
	List<T>& operator=(List<T>& other);
	T& get(int i);
	List<T>& append(T& data);
	List<T>& append(List<T>& list);
	bool clear();
	bool isEmpty();
	bool remove(int i);
	int indexOf(T& value);
	bool replace(int i, T& value);
	long size();
	bool contains(const T& value);
	bool insertAfter(int i, T& value);
};


/*--------------------------
* List<T>::List
* 	Default list constructor,create default head node and initiate varibles.
----------------------------*/
template<typename T>
List<T>::List()
{
	this->head = new ListNode<T>();
	this->tail = head;
	this->length = 0;
	this->last = nullptr;
	this->lastIndex = -MAXSHORT;
}


/*--------------------------
* List<T>::~List
* 	Destructor,clear all nodes and delete head node.
----------------------------*/
template<typename T>
List<T>::~List()
{
	this->clear();
	delete this->head;
}

/*--------------------------
* List<T>::operator[]
* 	Override operator[] to get i(th) node's data.
* Returns:	T& - i(th) node's data.
* Parameter:
* 	int i - Requested index.
----------------------------*/
template<typename T>
T& List<T>::operator[](int i)
{
	return this->get(i);
}


/*--------------------------
* List<T>::getNode
* 	Returns the i(th) node pointer.
* Returns:	ListNode<T>* - The node.
* Parameter:
* 	int i - Requested index.
----------------------------*/
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


/*--------------------------
* List<T>::removeNode
* 	Remove the node which the pointer in the parameter points to.
* Returns:	bool - The result.
* Parameter:
* 	ListNode<T> * node - The node to remove from the list.
----------------------------*/
template<typename T>
bool List<T>::removeNode(ListNode<T> *node)
{
	if (node == nullptr)
		throw QNullPointerException();

	if (node == this->last)
	{
		if (node->prior == this->head)
		{
			this->last = nullptr;
			this->lastIndex = -MAXSHORT;
		}
		else
		{
			this->last = node->prior;
			this->lastIndex--;
		}
	}

	node->prior->next = node->next;
	if (node->next == nullptr)
		this->tail = node->prior;
	else
		node->next->prior = node->prior;

	delete node;
	this->length--;

	return true;
}


/*--------------------------
* List<T>::operator=
* 	Override operator= to assign value to objects directly.
* Returns:	List<T>& - The List itself.
* Parameter:
* 	List<T> & other - The other list.
----------------------------*/
template<typename T>
List<T>& List<T>::operator=(List<T>& other)
{
	if (this == &other)
		return *this;
	this->clear();
	this->append(other);
	return *this;
}


/*--------------------------
* List<T>::get
* 	Get the i(th) node of the list (Starting from 0).
* Returns:	T& - The data of the i(th) node.
* Parameter:
* 	int i - Requested index.
----------------------------*/
template<typename T>
T& List<T>::get(int i)
{
	return this->getNode(i)->data;
}


/*--------------------------
* List<T>::append
* 	Append a node to the tail.
* Returns:	List<T>& - The list itself.
* Parameter:
* 	T & data - The data to append.
----------------------------*/
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


/*--------------------------
* List<T>::append
* 	Append a new list to the tail.
* Returns:	List<T>& - The list itself.
* Parameter:
* 	List<T> & list - The list to append.
----------------------------*/
template<typename T>
List<T>&  List<T>::append(List<T>& list)
{
	for (int i = 0; i < list.size(); i++)
		this->append(list.get(i));

	return *this;
}


/*--------------------------
* List<T>::clear
* 	Clear the list.
* Returns:	bool - The result.
----------------------------*/
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


/*--------------------------
* List<T>::isEmpty
* 	Returns whether this list is empty or not.
* Returns:	bool - The result.
----------------------------*/
template<typename T>
bool List<T>::isEmpty()
{
	return length == 0 ? true : false;
}


/*--------------------------
* List<T>::remove
* 	Remove node in the list.
* Returns:	bool - The result.
* Parameter:
* 	int i - Requested index.
----------------------------*/
template<typename T>
bool List<T>::remove(int i)
{
	return this->removeNode(this->getNode(i));
}


/*--------------------------
* List<T>::indexOf
* 	Return the index of the value.
* Returns:	int - The index.
* Parameter:
* 	T & value - The value to search for.
----------------------------*/
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


/*--------------------------
* List<T>::replace
* 	Replace i(th) node with the given value.
* Returns:	bool - The result.
* Parameter:
* 	int i - Requested index.
* 	T & value - Data to replace.
----------------------------*/
template<typename T>
bool List<T>::replace(int i, T& value)
{
	this->remove(i);
	this->insertAfter(i - 1, value);
	return true;
}


/*--------------------------
* List<T>::size
* 	Returns the size of the list.
* Returns:	long - The size of the list.
----------------------------*/
template<typename T>
long List<T>::size()
{
	return this->length;
}

/*--------------------------
* List<T>::contains
* 	Returns whether the list contains the value.
* Returns:	bool - The result.
* Parameter:
* 	const T & value - The value to search for.
----------------------------*/
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

/*--------------------------
* List<T>::insertAfter
* 	Insert the value after the i(th) node.
* Returns:	bool - The result.
* Parameter:
* 	int i - Requested index.
* 	T & value - Data to insert after i(t) node.
----------------------------*/
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
