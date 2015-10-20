#pragma once
#include "Html.h"

class Index
{
private:
	List<unsigned int> position;
	Html* html;
	//The weight member is for sorting  when displaying the results
	//NOTE:
	//Everytime new search begins the weight becomes meaningless
	//and will be restored when sorting.
	int weight;
public:
	int& getRefWeight();
	Html* getHtml();
	unsigned int getFrequency();
	List<unsigned int> &getPosition();
	Index();
	Index(Html* html, unsigned int firstPos);  
	Index &operator= (Index &other);
};


