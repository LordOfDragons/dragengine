/* 
 * Drag[en]gine IGDE Sky Editor
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

#ifndef _SELAYERLIST_H_
#define _SELAYERLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class seLayer;



/**
 * \brief Sky layer list.
 */
class seLayerList{
private:
	decObjectOrderedSet pLayers;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create layer list. */
	seLayerList();
	
	/** \brief Clean up layer list. */
	~seLayerList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of layers. */
	int GetCount() const;
	
	/** \brief Layer at index. */
	seLayer *GetAt( int index ) const;
	
	/** \brief Index of layer or -1 if absent. */
	int IndexOf( seLayer *layer ) const;
	
	/** \brief Layer is present. */
	bool Has( seLayer *layer ) const;
	
	/** \brief Add layer. */
	void Add( seLayer *layer );
	
	/** \brief Insert layer. */
	void InsertAt( seLayer *layer, int index );
	
	/** \brief Move layer. */
	void MoveTo( seLayer *layer, int index );
	
	/** \brief Remove layer. */
	void Remove( seLayer *layer );
	
	/** \brief Remove all layers. */
	void RemoveAll();
	
	/** \brief Set list from another list. */
	seLayerList &operator=( const seLayerList &list );
	/*@}*/
};

#endif
