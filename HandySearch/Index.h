#pragma once
#include "Html.h"

/**
 * Class:	Index
 *
 * Brief:	An index item of requested word,include the html pointer
 * and list of positions.
 *
 * Date:	Oct. 2015
 */
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


