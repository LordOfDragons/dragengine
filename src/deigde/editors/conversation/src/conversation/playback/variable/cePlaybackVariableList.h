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

#ifndef _CEPLAYBACKVARIABLELIST_H_
#define _CEPLAYBACKVARIABLELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class cePlaybackVariable;



/**
 * \brief Playback Variable List.
 */
class cePlaybackVariableList{
private:
	decObjectOrderedSet pVariables;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new list. */
	cePlaybackVariableList();
	/** Cleans up the list. */
	~cePlaybackVariableList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of variables. */
	int GetCount() const;
	/** Retrieves the variable at the given position. */
	cePlaybackVariable *GetAt( int index ) const;
	/** Retrieves the named variable or NULL if not found. */
	cePlaybackVariable *GetNamed( const char *name ) const;
	/** Retrieves the index of a variable or -1 if not found. */
	int IndexOf( cePlaybackVariable *variable ) const;
	/** Retrieves the index of a named variable or -1 if not found. */
	int IndexOfNamed( const char *name ) const;
	/** Determines if a variable exists. */
	bool Has( cePlaybackVariable *variable ) const;
	/** Determines if a named variable exists. */
	bool HasNamed( const char *name ) const;
	/** Sets a variable adding it if not existing. */
	void Set( const char *name, int value );
	/** Removes a variable. */
	void Remove( cePlaybackVariable *variable );
	/** Removes all variables. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	cePlaybackVariableList &operator=( const cePlaybackVariableList &list );
	/*@}*/
};

#endif
