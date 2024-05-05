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

#ifndef _DEALGPMPARAMETERLIST_H_
#define _DEALGPMPARAMETERLIST_H_

#include "../../common/collection/decObjectList.h"

class dealGPMParameter;



/**
 * \brief Game Profile Module Parameter List.
 */
class dealGPMParameterList{
private:
	decObjectList pParameters;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game profile parameter list. */
	dealGPMParameterList();
	
	/** \brief Clean up game profile parameter list. */
	~dealGPMParameterList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of parameters. */
	int GetParameterCount() const;
	
	/** \brief Parameter at index. */
	dealGPMParameter *GetParameterAt( int index ) const;
	
	/** \brief Parameter with name or \em NULL if not found. */
	dealGPMParameter *GetParameterNamed( const char *name ) const;
	
	/** \brief Parameter exists. */
	bool HasParameter( dealGPMParameter *parameter ) const;
	
	/** \brief Parameter with name exists. */
	bool HasParameterNamed( const char *name ) const;
	
	/** \brief Index of a parameter or -1 if not found. */
	int IndexOfParameter( dealGPMParameter *parameter ) const;
	
	/** \brief Index of parameter with name or -1 if not found. */
	int IndexOfParameterNamed( const char *name ) const;
	
	/** \brief Add parameter. */
	void AddParameter( dealGPMParameter *parameter );
	
	/** \brief Remove parameter. */
	void RemoveParameter( dealGPMParameter *parameter );
	
	/** \brief Remove all parameters. */
	void RemoveAllParameters();
	/*@}*/
};

#endif
