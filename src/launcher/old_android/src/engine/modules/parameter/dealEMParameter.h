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

#ifndef _DEALEMPARAMETER_H_
#define _DEALEMPARAMETER_H_

#include "../../../common/string/decString.h"
#include "../../../deObject.h"



/**
 * @brief Game Engine Module Parameter.
 */
class dealEMParameter : public deObject{
private:
	int pIndex;
	decString pName;
	decString pDescription;
	decString pValue;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<dealEMParameter> Ref;


	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new engine module parameter. */
	dealEMParameter();
	/** Cleans up the engine module parameter. */
	virtual ~dealEMParameter();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the parameter index. */
	inline int GetIndex() const{ return pIndex; }
	/** Sets the parameter index. */
	void SetIndex( int index );
	/** Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	/** Sets the name. */
	void SetName( const char *name );
	/** Retrieves the description. */
	inline const decString &GetDescription () const{ return pDescription; }
	/** Sets the description. */
	void SetDescription( const char *description );
	/** Retrieves the value. */
	inline const decString &GetValue() const{ return pValue; }
	/** Sets the value. */
	void SetValue( const char *value );
	/*@}*/
};

#endif
