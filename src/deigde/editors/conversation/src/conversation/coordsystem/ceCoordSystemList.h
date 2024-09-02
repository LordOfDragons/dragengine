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

#ifndef _CECOORDSYSTEMLIST_H_
#define _CECOORDSYSTEMLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceCoordSystem;



/**
 * \brief Conversation coordinate system list.
 */
class ceCoordSystemList{
private:
	decObjectOrderedSet pCoordSystems;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new coordinate system list. */
	ceCoordSystemList();
	/** \brief Cleans up the coordinate system list. */
	~ceCoordSystemList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the number of coordinate systems. */
	int GetCount() const;
	/** \brief Retrieves the coordinate system at the given position. */
	ceCoordSystem *GetAt( int index ) const;
	/** \brief Retrieves the coordinate system with the given id or NULL if not found. */
	ceCoordSystem *GetWithID( const char *id ) const;
	/** \brief Retrieves the coordinate system with the given alias id or NULL if not found. */
	ceCoordSystem *GetWithAliasID( const char *id ) const;
	/** \brief Retrieves the coordinate system with the given id or alias id or NULL if not found. */
	ceCoordSystem *GetWithIDOrAliasID( const char *id ) const;
	/** \brief Retrieves the index of the given coordinate system or -1 if not found. */
	int IndexOf( ceCoordSystem *coordSystem ) const;
	/** \brief Retrieves the index of the coordinate system with the given id or -1 if not found. */
	int IndexWithID( const char *id ) const;
	/** \brief Retrieves the index of the coordinate system with the given alias id or -1 if not found. */
	int IndexWithAliasID( const char *id ) const;
	/** \brief Retrieves the index of the coordinate system with the given id or alias id or -1 if not found. */
	int IndexWithIDOrAliasID( const char *id ) const;
	/** \brief Determines if a coordinate system exists. */
	bool Has( ceCoordSystem *coordSystem ) const;
	/** \brief Determines if a coordinate system with the given id exists. */
	bool HasWithID( const char *id ) const;
	/** \brief Determines if a coordinate system with the given alias id exists. */
	bool HasWithAliasID( const char *id ) const;
	/** \brief Determines if a coordinate system with the given id or alias id exists. */
	bool HasWithIDOrAliasID( const char *id ) const;
	/** \brief Adds a coordinate system. */
	void Add( ceCoordSystem *coordSystem );
	/** \brief Inserts a new coordinate system. */
	void InsertAt( ceCoordSystem *coordSystem, int index );
	/** \brief Moves a coordinate system to a new position. */
	void MoveTo( ceCoordSystem *coordSystem, int index );
	/** \brief Removes a coordinate system. */
	void Remove( ceCoordSystem *coordSystem );
	/** \brief Removes all coordinate systems. */
	void RemoveAll();
	
	/** \brief Sets the list from another list. */
	ceCoordSystemList &operator=( const ceCoordSystemList &list );
	/*@}*/
};

#endif
