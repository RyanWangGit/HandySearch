#pragma once

class QNullPointerException : private QException
{
private :
	QString errorMsg;
public:
	QNullPointerException()
	{
		this->errorMsg = "QNullPointerException";
	}

	QNullPointerException(const QString &errorMsg)
	{
		this->errorMsg = errorMsg;
	}

	const QString getErrorMsg()
	{
		return errorMsg;
	} 
};