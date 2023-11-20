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

#ifndef _MEHEIGHTTERRAINNAVSPACETYPE_H_
#define _MEHEIGHTTERRAINNAVSPACETYPE_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/deObject.h>

class decPointerList;
class meHeightTerrainNavSpaceFace;
class meHeightTerrainNavSpace;

class deDebugDrawerShape;



/**
 * \brief Height terrain navigation space type.
 */
class meHeightTerrainNavSpaceType : public deObject{
public:
	typedef deTObjectReference<meHeightTerrainNavSpaceType> Ref;
	
	
	
private:
	meHeightTerrainNavSpace *pNavSpace;
	
	decString pName;
	decColor pColor;
	int pType;
	
	bool pActive;
	
	decObjectOrderedSet pFaces;
	deDebugDrawerShape *pDDShape;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates height terrain navigation space type. */
	meHeightTerrainNavSpaceType( const char *name = "" );
	
	/** \brief Clean up height terrain navigation space type. */
	virtual ~meHeightTerrainNavSpaceType();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent height terrain navigation space. */
	meHeightTerrainNavSpace *GetNavSpace() const{ return pNavSpace; }
	
	/** \brief Set parent height terrain navigation space. */
	void SetNavSpace( meHeightTerrainNavSpace *navspace );
	
	
	
	/** \brief Display name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set display name. */
	void SetName( const char *name );
	
	/** \brief Display color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** \brief Set display color. */
	void SetColor( const decColor &color );
	
	/** \brief Navigation space type. */
	inline int GetType() const{ return pType; }
	
	/** \brief Set navigation type. */
	void SetType( int type );
	
	
	
	/** \brief Notify listeners navigation space changed. */
	void NotifyChanged();
	
	/** \brief Notify listeners navigation space type changed. */
	void NotifyTypeChanged();
	
	
	
	/** \brief Debug drawer shape or \em NULL. */
	inline deDebugDrawerShape *GetDDShape() const{ return pDDShape; }
	
	/** \brief Set debug drawer shape or \em NULL. */
	void SetDDShape( deDebugDrawerShape *shape );
	
	/** \brief Update heights. */
	void UpdateHeights();
	/*@}*/
	
	
	
	/** \name Faces */
	/*@{*/
	/** \brief Number of faces. */
	int GetFaceCount() const;
	
	/** \brief Face at index. */
	meHeightTerrainNavSpaceFace *GetFaceAt( int index ) const;
	
	/** \brief Face is present. */
	bool HasFace( meHeightTerrainNavSpaceFace *face ) const;
	
	/** \brief Index of the face or -1 if absent. */
	int IndexOfFace( meHeightTerrainNavSpaceFace *face ) const;
	
	/** \brief Add face. */
	void AddFace( meHeightTerrainNavSpaceFace *face );
	
	/** \brief Remove face. */
	void RemoveFace( meHeightTerrainNavSpaceFace *face );
	
	/** \brief Remove all faces. */
	void RemoveAllFaces();
	
	/** \brief Notify listeners faces changed. */
	void NotifyFacesChanged();
	
	/** \brief Update debug drawer faces (for internal use only). */
	void UpdateDDFaces();
	/*@}*/
	
	
	
private:
	/** \brief Update debug drawer faces. */
	void pUpdateDDFaces();
};

#endif
