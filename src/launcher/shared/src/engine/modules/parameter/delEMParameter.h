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

#ifndef _DELEMPARAMETER_H_
#define _DELEMPARAMETER_H_

#include <dragengine/common/string/decString.h>

#include <dragengine/deObject.h>
#include <dragengine/systems/modules/deModuleParameter.h>



/**
 * \brief Game Engine Module Parameter.
 */
class DE_DLL_EXPORT delEMParameter : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<delEMParameter> Ref;
	
	
	
private:
	int pIndex;
	deModuleParameter pInfo;
	decString pValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create engine module parameter. */
	delEMParameter( int index, const deModuleParameter &info );
	
	/** \brief Create engine module parameter. */
	delEMParameter( int index, const deModuleParameter &info, const char *value );
	
	/** \brief Clean up engine module parameter. */
	virtual ~delEMParameter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parameter index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** \brief Parameter info. */
	inline const deModuleParameter &GetInfo() const{ return pInfo; }
	
	/** \brief Value. */
	inline const decString &GetValue() const{ return pValue; }
	
	/** \brief Set value. */
	void SetValue( const char *value );
	/*@}*/
};

#endif
