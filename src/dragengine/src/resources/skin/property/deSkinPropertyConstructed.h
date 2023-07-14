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

#ifndef _DESKINPROPERTYCONSTRUCTED_H_
#define _DESKINPROPERTYCONSTRUCTED_H_

#include "deSkinProperty.h"
#include "../../../common/collection/decObjectOrderedSet.h"
#include "../../../common/math/decMath.h"

class deSkinPropertyNodeGroup;
class deSkinPropertyNodeMapped;


/**
 * \brief Skin texture constructed property.
 */
class DE_DLL_EXPORT deSkinPropertyConstructed : public deSkinProperty{
private:
	deSkinPropertyNodeGroup *pContent;
	decColor pColor;
	bool pTileX;
	bool pTileY;
	int pBitCount;
	decObjectOrderedSet pMapped;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create constructed property.
	 * \param type Type of the skin property obtained from adding a property
	 * name to the texture property map object held by the engine.
	 */
	deSkinPropertyConstructed( const char *type );
	
	/** \brief Clean up constructed property. */
	virtual ~deSkinPropertyConstructed();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Content. */
	inline deSkinPropertyNodeGroup &GetContent() const{ return *pContent; }
	
	/** \brief Base color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** \brief Set base color. */
	void SetColor( const decColor &color );
	
	/** \brief Nodes are tiled along x axis. */
	inline bool GetTileX() const{ return pTileX; }
	
	/** \brief Set if nodes are tiled along x axis. */
	void SetTileX( bool tileX );
	
	/** \brief Nodes are tiled along y axis. */
	inline bool GetTileY() const{ return pTileY; }
	
	/** \brief Set if nodes are tiled along y axis. */
	void SetTileY( bool tileY );
	
	/**
	 * \brief Bit count.
	 * 
	 * Can be 8, 16 or 32.
	 */
	inline int GetBitCount() const{ return pBitCount; }
	
	/**
	 * \brief Set bit count.
	 * 
	 * Allowed are 8, 16 or 32.
	 */
	void SetBitCount( int bitCount );
	/*@}*/
	
	
	
	/** \name Mapped Values */
	/*@{*/
	/** \brief Count of mapped values. */
	int GetMappedCount() const;
	
	/** \brief Mapped value at index. */
	deSkinPropertyNodeMapped *GetMappedAt( int index ) const;
	
	/** \brief Named mapped value or nullptr. */
	deSkinPropertyNodeMapped *GetMappedNamed( const char *name ) const;
	
	/** \brief Index of mapped value. */
	int IndexOfMapped( deSkinPropertyNodeMapped *mapped ) const;
	
	/** \brief Mapped value is present. */
	bool HasMapped( deSkinPropertyNodeMapped *mapped ) const;
	
	/** \brief Add mapped value. */
	void AddMapped( deSkinPropertyNodeMapped *mapped );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit property. */
	virtual void Visit( deSkinPropertyVisitor &visitor );
	/*@}*/
};

#endif
