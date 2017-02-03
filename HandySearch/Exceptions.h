/*****************************************
* DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER.
*
* Copyright 2015 Ryan Wang, wangyuxin@hust.edu.cn
*
* This file is part of HandySearch.
* HandySearch is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* HandySearch is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with HandySearch.  If not, see <http://www.gnu.org/licenses/>.
*
* - Author: Ryan Wang
* - Contact: wangyuxin@hust.edu.cn
* - License: GNU Lesser General Public License (LGPL)
* - Blog and source code availability: http://ryanwanggit.github.io/HandySearch/
*****************************************/
#pragma once
#include <QException>
/**
 * Class:    QNullPointerException
 *
 * Brief:    An exception thrown when encountered null pointer.
 *
 * Date:    Oct. 2015
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

    const QString &getErrorMsg()
    {
        return errorMsg;
    } 
};


/**
 * Class:    QOutOfBoundaryException
 *
 * Brief:    An exception thrown when visit is out of boudary.
 *
 * Date:    Oct. 2015
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

    const QString &getErrorMsg()
    {
        return errorMsg;
    }
};
