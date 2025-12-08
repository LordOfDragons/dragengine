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
	meWorld::Ref pWorld;
	meHeightTerrainTexture::Ref pHTTexture;
	meHeightTerrainNavSpace::Ref pHTNavSpace;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create entry. */
	meWCEntry(meWindowChangelog &windowChangelog, eElementTypes type);
	
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
	void SetSector(const decPoint3 &sector);
	
	/** \brief World or \em NULL. */
	inline const meWorld::Ref &GetWorld() const{ return pWorld; }
	
	/** \brief Set world or \em NULL. */
	void SetWorld(meWorld *world);
	
	/** \brief Height terrain texture or \em NULL. */
	inline const meHeightTerrainTexture::Ref &GetHTTexture() const{ return pHTTexture; }
	
	/** \brief Set height terrain texture or \em NULL. */
	void SetHTTexture(meHeightTerrainTexture *texture);
	
	/** \brief Height terrain navigation space or \em NULL. */
	inline const meHeightTerrainNavSpace::Ref &GetHTNavSpace() const{ return pHTNavSpace; }
	
	/** \brief Set height terrain navigation space or \em NULL. */
	void SetHTNavSpace(meHeightTerrainNavSpace *navspace);
	
	/** \brief Update item text. */
	void UpdateText();
	/*@}*/
};

#endif
