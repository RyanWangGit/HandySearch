#pragma once

class QNullPointerException : private QException
{
public:
	const QString what()
	{
		return "Null Pointer was called.";
	} 
};