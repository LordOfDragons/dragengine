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

#ifndef _FBXCONNECTION_H_
#define _FBXCONNECTION_H_


#include <stdint.h>

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>


class deBaseModule;


/**
 * \brief FBX connection.
 */
class fbxConnection : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<fbxConnection> Ref;
	
	
private:
	int64_t pSource;
	int64_t pTarget;
	decString pProperty;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create connection. */
	fbxConnection( int64_t source, int64_t target, const char *property = "" );
	
protected:
	/** \brief Clean up connection. */
	virtual ~fbxConnection();
	/*@}*/
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Source ID. */
	inline int64_t GetSource() const{ return pSource; }
	
	/** \brief Target ID. */
	inline int64_t GetTarget() const{ return pTarget; }
	
	/** \brief Other ID. */
	int64_t OtherID( int64_t id ) const;
	
	/** \brief Property or empty string. */
	inline const decString &GetProperty() const{ return pProperty; }
	
	/** \brief Debug print property structure. */
	void DebugPrintStructure( deBaseModule &module, const decString &prefix ) const;
	/*@}*/
};

#endif
