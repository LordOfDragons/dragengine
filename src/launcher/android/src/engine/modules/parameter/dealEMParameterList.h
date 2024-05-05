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

#ifndef _DEALEMPARAMETERLIST_H_
#define _DEALEMPARAMETERLIST_H_

#include "../../../common/collection/decObjectList.h"

class dealEMParameter;



/**
 * @brief Engine Parameter Parameter List.
 */
class dealEMParameterList{
private:
	decObjectList pParameters;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new parameter list. */
	dealEMParameterList();
	/** Cleans up the parameter list. */
	~dealEMParameterList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of parameters. */
	int GetParameterCount() const;
	/** Retrieves the parameter at the given position. */
	dealEMParameter *GetParameterAt( int index ) const;
	/** Retrieves the parameter with the given name or NULL if not found. */
	dealEMParameter *GetParameterNamed( const char *name ) const;
	/** Determines if a parameter exists. */
	bool HasParameter( dealEMParameter *parameter ) const;
	/** Determines if a parameter with the given name exists. */
	bool HasParameterNamed( const char *name ) const;
	/** Retrieves the index of a parameter or -1 if not found. */
	int IndexOfParameter( dealEMParameter *parameter ) const;
	/** Retrieves the index of a parameter with the given name or -1 if not found. */
	int IndexOfParameterNamed( const char *name ) const;
	/** Adds a parameter. */
	void AddParameter( dealEMParameter *parameter );
	/** Removes a parameter. */
	void RemoveParameter( dealEMParameter *parameter );
	/** Removes all parameters. */
	void RemoveAllParameters();
	/*@}*/
};

#endif // _DEGLGAME_H_
