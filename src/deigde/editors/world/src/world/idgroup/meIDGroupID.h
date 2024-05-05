/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _MEIDGROUPID_H_
#define _MEIDGROUPID_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/deObject.h>


/**
 * \brief Identifier group identifier.
 */
class meIDGroupID : public deObject{
private:
	decString pID;
	int pUsageCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create identifier group identifier with usage count 1. */
	meIDGroupID( const char *id );
	
	/** \brief Clean up identifier group identifier. */
	virtual ~meIDGroupID();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Identifier. */
	inline const decString &GetID() const{ return pID; }
	
	/** \brief Usage count. */
	inline int GetUsageCount() const{ return pUsageCount; }
	
	/** \brief Increment usage count by one. */
	void Increment();
	
	/** \brief Decrement usage count by one if larger than 0. */
	void Decrement();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Objects are equal. */
	bool operator==( const meIDGroupID &id ) const;
	/*@}*/
};

#endif
