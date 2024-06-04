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
