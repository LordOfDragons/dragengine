/* 
 * Drag[en]gine IGDE World Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

// include only once
#ifndef _MELSPFCACHETYPELIST_H_
#define _MELSPFCACHETYPELIST_H_

// includes

// predefinitions



/**
 * @brief Load/Save Prop Field Type List
 *
 * Helper class for the prop field cache load/save to store a list of types.
 */
class meLSPFCacheTypeList{
public:
	struct sType{
		int vlayer;
		int variation;
	};
	
private:
	sType *pTypes;
	int pTypeCount;
	int pTypeSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a type list. */
	meLSPFCacheTypeList();
	/** Cleans up the type list. */
	~meLSPFCacheTypeList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of types. */
	inline int GetTypeCount() const{ return pTypeCount; }
	/** Retrieves the type at the given index. */
	sType &GetTypeAt( int index ) const;
	/** Retrieves the index of the type with the given vlayer and variation or -1 if not found. */
	int IndexOfTypeWith( int vlayer, int variation ) const;
	/** Adds the type if not existing already. */
	void AddType( int vlayer, int variation );
	/** Removes all types. */
	void RemoveAllTypes();
	/*@}*/
};

// end of include only once
#endif
