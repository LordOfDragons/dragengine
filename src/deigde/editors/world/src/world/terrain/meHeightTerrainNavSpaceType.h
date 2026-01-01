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

#include "meHeightTerrainNavSpaceFace.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/deObject.h>

class meHeightTerrainNavSpace;

class deDebugDrawerShape;



/**
 * Height terrain navigation space type.
 */
class meHeightTerrainNavSpaceType : public deObject{
public:
	typedef deTObjectReference<meHeightTerrainNavSpaceType> Ref;
	typedef decTObjectOrderedSet<meHeightTerrainNavSpaceType> List;
	
	
private:
	meHeightTerrainNavSpace *pNavSpace;
	
	decString pName;
	decColor pColor;
	int pType;
	
	bool pActive;
	
	meHeightTerrainNavSpaceFace::List pFaces;
	deDebugDrawerShape *pDDShape;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates height terrain navigation space type. */
	explicit meHeightTerrainNavSpaceType(const char *name = "");
	
protected:
	/** Clean up height terrain navigation space type. */
	~meHeightTerrainNavSpaceType() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Parent height terrain navigation space. */
	meHeightTerrainNavSpace *GetNavSpace() const{ return pNavSpace; }
	
	/** Set parent height terrain navigation space. */
	void SetNavSpace(meHeightTerrainNavSpace *navspace);
	
	
	
	/** Display name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Set display name. */
	void SetName(const char *name);
	
	/** Display color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** Set display color. */
	void SetColor(const decColor &color);
	
	/** Navigation space type. */
	inline int GetType() const{ return pType; }
	
	/** Set navigation type. */
	void SetType(int type);
	
	
	
	/** Notify listeners navigation space changed. */
	void NotifyChanged();
	
	/** Notify listeners navigation space type changed. */
	void NotifyTypeChanged();
	
	
	
	/** Debug drawer shape or \em nullptr. */
	inline deDebugDrawerShape *GetDDShape() const{ return pDDShape; }
	
	/** Set debug drawer shape or \em nullptr. */
	void SetDDShape(deDebugDrawerShape *shape);
	
	/** Update heights. */
	void UpdateHeights();
	/*@}*/
	
	
	
	/** \name Faces */
	/*@{*/
	/** Faces. */
	inline const meHeightTerrainNavSpaceFace::List &GetFaces() const{ return pFaces; }
	
	/** Add face. */
	void AddFace(meHeightTerrainNavSpaceFace *face);
	
	/** Remove face. */
	void RemoveFace(meHeightTerrainNavSpaceFace *face);
	
	/** Remove all faces. */
	void RemoveAllFaces();
	
	/** Notify listeners faces changed. */
	void NotifyFacesChanged();
	
	/** Update debug drawer faces (for internal use only). */
	void UpdateDDFaces();
	/*@}*/
	
	
	
private:
	/** Update debug drawer faces. */
	void pUpdateDDFaces();
};

#endif
