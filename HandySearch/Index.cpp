#include "stdafx.h"

Index::Index()
{
	//To make data invalid
	//This empty constructor is for the head node
	this->htmlID = -1;
	this->frequency = -1;
}

Index::Index(unsigned int htmlID, unsigned int frequency, unsigned int firstPos)
{
	this->htmlID = htmlID;
	this->frequency = frequency;
	this->position.append(firstPos);
}

Index & Index::operator= (Index &other)
{
	if (this == &other)
		return *this;
	this->frequency = other.frequency;
	this->htmlID = other.htmlID;
	this->position = other.position;
	return *this;
}

unsigned int  Index::getHtmlID()
{
	return this->htmlID;
}

unsigned int Index::getFrequency()
{
	return this->frequency;
}

List<unsigned int> &Index::getPosition()
{
	return this->position;
}

