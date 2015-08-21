#pragma once
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
public:
	List();
	~List();
	/* Get the i(th) node of the list (Starting from 0) */
	T& get(int i);
	/* Append a node to the tail */
	void append(T& data);
	/* Append a new list to the tail */
	void append(List<T>& list);
	/* Clear the list */
	bool clear();
	/* Returns whether this list is empty or not */
	bool isEmpty();
	/* Remove node in the list */
	bool remove(int i);
	bool remove(T& value);
	/* Return the index of the value */
	int indexOf(T& value);
	/* Replace i(th) node with the given value */
	bool replace(int i, T& value);
	/* Returns the size of the list */
	long size();
	/* Returns whether the list contains the value */
	bool contains(const T& value);
	/* Insert the value after the i(th) node */
	bool insertAfter(int i, const T& value);
	/* Operator[] to get i(th) node's data */
	T& operator[](int i);
	List<T>& operator=(List<T>& other);
	
};

template<typename T>
List<T>::List()
{
	this->head = new ListNode<T>();
	this->tail = head;
	this->length = 0;
	this->last = head;
	this->lastIndex = 0;
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
T& List<T>::get(int i) 
{
	//TODO: Use of last pointer to optimize when iterating
	//by finding the shortest path to the index queried
	ListNode<T>* p = this->head->next;
	//if (i > (i - this->lastIndex) ? ((i - this->lastIndex) > ))
	for (int n = 0; n <= i; n++)
	{
		if (!p)
			throw QNullPointerException();
		else if (n == i)
		{
			this->last = p;
			this->lastIndex = n;
			return p->data;
		}
			
		else
			p = p->next;
	}
	
}

template<typename T>
bool List<T>::clear()
{
	ListNode<T>* temp = nullptr;
	try
	{
		while (this->tail != this->head)
		{
			temp = this->tail;
			this->tail = this->tail->prior;
			delete temp;
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}

template<typename T>
void List<T>::append(T& data)
{
	ListNode<T>* p = this->tail;
	p->next = new ListNode<T>(data);
	
	p->next->prior = p;
	this->tail = p->next;
	this->length++;
}

template<typename T>
void List<T>::append(List<T>& list)
{
	for (int i = 0; i < list.size(); i++)
		this->append(list.get(i));
}

template<typename T>
bool List<T>::isEmpty()
{
	return length == 0 ? true : false;
}

template<typename T>
bool List<T>::remove(ListNode<T> *p)
{
	p->prior->next = p->next;
	p->next->prior = p->prior;
	delete p;
	this->length--;
	return true;
}

template<typename T>
bool List<T>::remove(int i)
{
	ListNode<T>* p = this->head->next;
	for (int n = 0; n <= i; n++)
	{
		if (!p)
			throw QNullPointerException("In remove method");
		else if (n == i)
			return this->remove(p);
		else
			p = p->next;
	}	
}

template<typename T>
bool List<T>::remove(T &value)
{
	ListNode<T>* p = this->head;
	bool hasRemove = false;
	for (int i = 0; i < this->size(); i++)
	{
		if (!p)
			throw QNullPointerException("in remove(T &value) function.");
		if (p->data == value)
		{
			this->remove(p);
			hasRemove = true;
		}
		p = p->next;
	}
	return hasRemove;
}

template<typename T>
int List<T>::indexOf(T &value)
{
	ListNode<T>* p = this->head->next;
	int i;
	for (i = 0; i < this->size(); i++)
	{
		if (!p)
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
	ListNode<T> *p = this->head->next;
	for (int i = 0; i < this->size(); i++)
	{
		if (!p)
			throw QNullPointerException("in contains(const T &value) function");

		if (this->get(i) == value)
			return true;

		p = p->next;
	}
	return false;
}

template<typename T>
bool List<T>::insertAfter(int i, const T& value)
{
	ListNode<T> *p = this->head->next;
	ListNode<T> *temp = nullptr;
	for (int n = 0; n <= i; n++)
	{
		if (!p)
			throw QNullPointerException("in contains(const T &value) function");
		if (n == i)
			temp = p->next;
		else
			p = p->next;
	}
	p->next = new ListNode<T>(value); 
	p->next->prior = p;
	p->next->next = temp;
	temp->prior = p->next;
	this->length++;
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
