#include "stdafx.h"

Index::Index()
{
	//To make data invalid
	//This empty constructor is for the head node
}

Index::Index(Html* html, unsigned int firstPos)
{
	this->html = html;
	this->position.append(firstPos);
}

Index & Index::operator= (Index &other)
{
	if (this == &other)
		return *this;
	this->html = other.html;
	this->position = other.position;
	return *this;
}

Html* Index::getHtml()
{
	return this->html;
}

unsigned int Index::getFrequency()
{
	return this->position.size();
}

List<unsigned int> &Index::getPosition()
{
	return this->position;
}

