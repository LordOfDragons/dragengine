/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEDECALLIST_H_
#define _DEDECALLIST_H_

#include "../../deObject.h"

// predefintions
class deDecal;


/**
 * \brief Collision Decal List.
 
 * Stores a list of decals. This class is used to collect decals from collision
 * test into a list for later processing.
 */
class DE_DLL_EXPORT deDecalList : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deDecalList> Ref;
	
	
	
private:
	deDecal **pDecals;
	int pDecalCount;
	int pDecalSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new decal list object. */
	deDecalList();
	
	/** \brief Clean up decal list object. */
	virtual ~deDecalList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of decals. */
	inline int GetDecalCount() const{ return pDecalCount; }
	
	/** \brief Decal at the given index. */
	deDecal *GetDecalAt( int index ) const;
	
	/** \brief Determines if a decal exists. */
	bool HasDecal( deDecal *decal ) const;
	
	/** \brief Index of the decal or -1 if not found. */
	int IndexOfDecal( deDecal *decal ) const;
	
	/** \brief Adds a decal. */
	void AddDecal( deDecal *decal );
	
	/** \brief Removes all decals. */
	void RemoveAllDecals();
	/*@}*/
};

#endif
