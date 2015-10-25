#pragma once

/**
 * Class:	QNullPointerException
 *
 * Brief:	An exception thrown when encountered null pointer.
 *
 * Date:	Oct. 2015
 */
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


/**
 * Class:	QOutOfBoundaryException
 *
 * Brief:	An exception thrown when visit is out of boudary.
 *
 * Date:	Oct. 2015
 */
class QOutOfBoundaryException : private QException
{
private:
	QString errorMsg;
public:
	QOutOfBoundaryException()
	{
		this->errorMsg = "QOutOfBoundaryException";
	}

	QOutOfBoundaryException(const QString &errorMsg)
	{
		this->errorMsg = errorMsg;
	}

	const QString getErrorMsg()
	{
		return errorMsg;
	}
};