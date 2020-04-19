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

#ifndef _MEWCENTRY_H_
#define _MEWCENTRY_H_

#include <deigde/gui/model/igdeListItem.h>

#include <dragengine/common/math/decMath.h>

class meHeightTerrainNavSpace;
class meHeightTerrainTexture;
class meWindowChangelog;
class meWorld;



/**
 * \brief Changelog entry.
 */
class meWCEntry : public igdeListItem{
public:
	/** \brief Element type. */
	enum eElementTypes{
		/** \brief World. */
		eetWorld,
		
		/** \brief Height Terrain Sector. */
		      eetHeightTerrain,
		
		/** \brief Height Terrain Height Image. */
		eetHTHeight,
		
		/** \brief Height Terrain Visibility Image. */
		eetHTVisibility,
		
		/** \brief Height Terrain Texture Mask. */
		eetHTTextureMask,
		
		/** \brief Height terrain navigation space. */
		eetHTNavSpace,
		
		/** \brief Height Terrain Prop Field Cache. */
		eetHTPFCache
	};
	
	
	
private:
	meWindowChangelog &pWindowChangelog;
	
	eElementTypes pType;
	decPoint3 pSector;
	meWorld *pWorld;
	meHeightTerrainTexture *pHTTexture;
	meHeightTerrainNavSpace *pHTNavSpace;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create entry. */
	meWCEntry( meWindowChangelog &windowChangelog, eElementTypes type );
	
protected:
	/** \brief Clean up entry. */
	virtual ~meWCEntry();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Changelog window. */
	inline meWindowChangelog &GetWindowChangelog() const{ return pWindowChangelog; }
	
	/** \brief Type. */
	inline eElementTypes GetType() const{ return pType; }
	
	/** \brief Ssector. */
	inline const decPoint3 &GetSector() const{ return pSector; }
	
	/** \brief Set sector. */
	void SetSector( const decPoint3 &sector );
	
	/** \brief World or \em NULL. */
	inline meWorld *GetWorld() const{ return pWorld; }
	
	/** \brief Set world or \em NULL. */
	void SetWorld( meWorld *world );
	
	/** \brief Height terrain texture or \em NULL. */
	inline meHeightTerrainTexture *GetHTTexture() const{ return pHTTexture; }
	
	/** \brief Set height terrain texture or \em NULL. */
	void SetHTTexture( meHeightTerrainTexture *texture );
	
	/** \brief Height terrain navigation space or \em NULL. */
	inline meHeightTerrainNavSpace *GetHTNavSpace() const{ return pHTNavSpace; }
	
	/** \brief Set height terrain navigation space or \em NULL. */
	void SetHTNavSpace( meHeightTerrainNavSpace *navspace );
	
	/** \brief Update item text. */
	void UpdateText();
	/*@}*/
};

#endif
