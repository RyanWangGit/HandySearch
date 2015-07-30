#pragma once

class Index
{
private:
	unsigned int htmlID;
	unsigned int frequency;
	List<unsigned int> position;
public:
	unsigned int getHtmlID();
	unsigned int getFrequency();
	List<unsigned int> &getPosition();
	Index();
	Index(unsigned int htmlID,unsigned int frequency,unsigned int firstPos);  
	Index &operator= (Index &other);
};


