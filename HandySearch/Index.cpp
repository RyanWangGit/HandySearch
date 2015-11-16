/*************************************
 * Copyright(C),2015-2016,Ryan Wang 
 * 
 * File:	Index.cpp
 *
 * Version: V1.0
 * 
 * Brief:	This is the implementations of Index class,
 * it is an entry of the hashmap,which provides wrapper
 * of html pointer and list of positions of word appearance.
 *
 * Author:	Ryan
 
 * Date:	Oct. 2015
*************************************/
#include "stdafx.h"
#include "List.h"
#include "Html.h"
#include "Index.h"


/*--------------------------
* Index::Index
* 	To make data invalid,this empty constructor is for the head node.
----------------------------*/
Index::Index()
{
	this->html = nullptr;
}


/*--------------------------
* Index::Index
* 	To put html pointer and the first position it appeared in the html
* into index,this is the common-used constructor.
* Parameter:
* 	Html * html - Html object pointer.
* 	unsigned int firstPos - First position the word appeared in the html.
----------------------------*/
Index::Index(Html* html, unsigned int firstPos)
{
	this->html = html;
	this->position.append(firstPos);
}


/*--------------------------
* Index::operator=
* 	Override operator method to directly assign value.
* Returns:	Index & - The index itself.
* Parameter:
* 	Index & other - The other index object.
----------------------------*/
Index & Index::operator= (const Index &other)
{
	if (this == &other)
		return *this;
	this->html = other.html;
	this->position = other.position;
	return *this;
}


/*--------------------------
* Index::getHtml
* 	Returns the html object pointer the index posseses.
* Returns:	Html* - The html pointer.
----------------------------*/
Html* Index::getHtml() const
{
	return html;
}


/*--------------------------
* Index::getFrequency
* 	Returns the frequency the word appears in the html.
* Returns:	unsigned int - The frequency.
----------------------------*/
unsigned int Index::getFrequency() const
{
	return position.size();
}


/*--------------------------
* Index::getPosition
* 	Returns a list of positions the word appears in the html.
* Returns:	List<unsigned int> & - List of positions.
----------------------------*/
List<unsigned int> &Index::getPosition()
{
	return position;
}

