#pragma once

class Index
{
private:
	List<unsigned int> position;
	Html* html;
public:
	Html* getHtml();
	unsigned int getFrequency();
	List<unsigned int> &getPosition();
	Index();
	Index(Html* html, unsigned int firstPos);  
	Index &operator= (Index &other);
};


