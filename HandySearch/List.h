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
	ListNode(const T& data);
	ListNode();
	~ListNode();
};

template<typename T>
ListNode<T>::ListNode(const T& data)
{
	this->data = data;
	this->next = nullptr;
	this->prior = nullptr;
}

template<typename T>
ListNode<T>::ListNode()
{
	this->next = nullptr;
	this->prior = nullptr;
}

template<typename T>
ListNode<T>::~ListNode()
{

}

template<typename T>
class List
{
private:
	long length;
	ListNode<T>* head;
	ListNode<T>* tail;
	bool remove(ListNode<T>* p);
public:
	List();
	~List();
	/* Get the i(th) node of the list (Starting from 0) */
	T& get(int i);
	/* Append a node to the tail */
	void append(const T data);
	/* Append a new list to the tail */
	void append(const List<T>& list);
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
	
};

template<typename T>
List<T>::List()
{
	this->head = new ListNode<T>();
	this->tail = head;
	this->length = 0;
}

template<typename T>
List<T>::~List()
{
	this->clear();
}

template<typename T>
T& List<T>::operator[](int i)
{
	return this->get(i);
}

template<typename T>
T& List<T>::get(int i) 
{
	ListNode<T>* p = this->head->next;
	for (int n = 0; n < i; n++)
	{
		if (!p)
			throw QNullPointerException();
		else
			p = p->next;
	}
	return p->data;
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
void List<T>::append(const T data)
{
	ListNode<T>* p = this->tail;
	p->next = new ListNode<T>(data);
	p->next->prior = p;
	this->tail = p->next;
	this->length++;
}

template<typename T>
void List<T>::append(const List<T>& list)
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
	if ()
	for (int n = 0; n < i; n++)
	{
		if (p)
			throw QNullPointerException();
		p = p->next;
	}	
	this->remove(p);
}

template<typename T>
bool List<T>::remove(T &value)
{
	ListNode<T>* p = this->head;
	bool hasRemove = false;
	for (int i = 0; i < this->size(); i++)
	{
		if (p)
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
		if (p)
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
		if (p)
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
	for (int n = 0; n < i; n++)
	{
		if (p)
			throw QNullPointerException("in contains(const T &value) function");
		p = p->next;
	}
	temp = p->next;
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
