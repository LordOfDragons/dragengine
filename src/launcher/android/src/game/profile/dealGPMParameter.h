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

#ifndef _DEALGPMPARAMETER_H_
#define _DEALGPMPARAMETER_H_

#include "../../deObject.h"
#include "../../common/string/decString.h"

class dealIEngineInstance;
class dealLauncher;



/**
 * \brief Game Profile Module Parameter.
 */
class dealGPMParameter : public deObject{
private:
	decString pName;
	decString pValue;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game profile module parameter. */
	dealGPMParameter();
	
	/** \brief Create game profile module parameter. */
	dealGPMParameter( const dealGPMParameter &parameter );
	
	/** \brief Clean up game profile. */
	virtual ~dealGPMParameter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parameter name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set parameter name. */
	void SetName( const char *name );
	
	/** \breif Parameter value. */
	inline const decString &GetValue() const{ return pValue; }
	
	/** \brief Set parameter value. */
	void SetValue( const char *value );
	
	/** \brief Copy from another parameter. */
	void CopyFrom( const dealGPMParameter &parameter );
	/*@}*/
};

#endif
