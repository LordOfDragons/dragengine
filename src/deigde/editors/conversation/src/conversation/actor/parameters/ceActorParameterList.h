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

#ifndef _CEACTORPARAMETERLIST_H_
#define _CEACTORPARAMETERLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceActorParameter;



/**
 * \brief Actor Parameter List.
 */
class ceActorParameterList{
private:
	decObjectOrderedSet pParameters;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new list. */
	ceActorParameterList();
	/** Cleans up the list. */
	~ceActorParameterList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of parameters. */
	int GetCount() const;
	/** Retrieves the parameter at the given position. */
	ceActorParameter *GetAt( int index ) const;
	/** Retrieves the named parameter or NULL if not found. */
	ceActorParameter *GetNamed( const char *name ) const;
	/** Retrieves the index of a parameter or -1 if not found. */
	int IndexOf( ceActorParameter *parameter ) const;
	/** Retrieves the index of a named parameter or -1 if not found. */
	int IndexOfNamed( const char *name ) const;
	/** Determines if a parameter exists. */
	bool Has( ceActorParameter *parameter ) const;
	/** Determines if a named parameter exists. */
	bool HasNamed( const char *name ) const;
	/** Sets a parameter adding it if not existing. */
	void Set( const char *name, int value );
	/** Removes a parameter. */
	void Remove( ceActorParameter *parameter );
	/** Removes all parameters. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	ceActorParameterList &operator=( const ceActorParameterList &list );
	/*@}*/
};

#endif
