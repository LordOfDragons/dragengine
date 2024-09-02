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

#ifndef _DENETWORKVALUESTRING_H_
#define _DENETWORKVALUESTRING_H_

#include "deNetworkValue.h"
#include "../../../common/string/decString.h"


/**
 * \brief Network Value String Class.
 *
 * Defines a string network value.
 */
class DE_DLL_EXPORT deNetworkValueString : public deNetworkValue{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deNetworkValueString> Ref;
	
	
	
private:
	decString pValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create network value object. */
	deNetworkValueString();
	
	/** \brief Create network value object. */
	deNetworkValueString( const char *value );
	
	/** \brief Clean up network value object. */
	virtual ~deNetworkValueString();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Value. */
	inline const decString &GetString() const{ return pValue; }
	
	/** \brief Set value. */
	void SetString( const char *value );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit collider. */
	virtual void Visit( deNetworkValueVisitor &visitor );
	/*@}*/
};

#endif
