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

#ifndef _DEHEIGHTTERRAINSECTOR_H_
#define _DEHEIGHTTERRAINSECTOR_H_

#include "../../../common/collection/decPointerOrderedSet.h"
#include "../../../common/math/decMath.h"
#include "../../../resources/image/deImage.h"
#include <dragengine/resources/decal/deDecal.h>

class deHeightTerrain;
class deHeightTerrainNavSpace;
class deHeightTerrainTexture;
class deDecalList;


/**
 * \brief Height Terrain Sector.
 *
 * Defines a sector of a height terrain. The height of grid points in each sector
 * are defined using a height image and an offset and scaling. In addition a prop
 * field can be maintained. As height image only 1-component images can be used.
 * Any bit depth is possible allowing for fine grained resolution if required.
 * The optional scaling and offset parameter alter the height retrieved from the
 * height map before being applied to grid points. Individual grid faces can be
 * hidden. If the height image is missing all grid points take on the height of
 * the offset parameter.
 */
class DE_DLL_EXPORT deHeightTerrainSector{
private:
	decPoint pSector;
	deHeightTerrain *pParentHeightTerrain;
	int pIndex;
	
	deImage::Ref pHeightImage;
	
	unsigned char *pVisibleFaces;
	int pVFByteCount;
	
	decPointerOrderedSet pTextures;
	decPointerOrderedSet pNavSpaces;
	
	deDecal *pDecalRoot;
	deDecal::Ref pDecalTail;
	int pDecalCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sector. */
	deHeightTerrainSector(const decPoint &sector);
	
	/** \brief Clean up sector. */
	~deHeightTerrainSector();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Sector coordinates. */
	inline const decPoint &GetSector() const{ return pSector; }
	
	/** \brief Parent height terrain. */
	inline deHeightTerrain *GetParentHeightTerrain() const{ return pParentHeightTerrain; }
	
	/** \brief Set parent height terrain. */
	void SetParentHeightTerrain(deHeightTerrain *heightTerrain);
	
	/** \brief Sector index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** \brief Set sector index. */
	void SetIndex(int index);
	
	
	
	/** \brief Height image or NULL if not set. */
	inline deImage *GetHeightImage() const{ return pHeightImage; }
	
	/** \brief Set height image or NULL if not set. */
	void SetHeightImage(deImage *heightImage);
	
	/**
	 * \brief Calculate height extends.
	 * 
	 * The extends contain the base height and scaling has been taken into account.
	 */
	void CalculateHeightExtends(float &minHeight, float &maxHeight);
	/*@}*/
	
	
	
	/** \name Face Visibility */
	/*@{*/
	/** \brief Face at coordinate is visible. */
	bool GetFaceVisibleAt(int x, int y) const;
	
	/** \brief Set if face at coordinate is visible. */
	void SetFaceVisibleAt(int x, int y, bool visible);
	
	/** \brief Set visibility of all faces. */
	void SetAllFacesVisible(bool visible);
	/*@}*/
	
	
	
	/** \name Textures */
	/*@{*/
	/** \brief Number of textures. */
	int GetTextureCount() const;
	
	/** \brief Texture at index. */
	deHeightTerrainTexture *GetTextureAt(int index) const;
	
	/** \brief Index of texture or -1 if absent. */
	int IndexOfTexture(deHeightTerrainTexture *texture) const;
	
	/** \brief Texture is present. */
	bool HasTexture(deHeightTerrainTexture *texture) const;
	
	/** \brief Add texture. */
	void AddTexture(deHeightTerrainTexture *texture);
	
	/** \brief Remove texture. */
	void RemoveTexture(deHeightTerrainTexture *texture);
	
	/** \brief Remove all textures. */
	void RemoveAllTextures();
	/*@}*/
	
	
	
	/** \name Decals Management */
	/*@{*/
	/** \brief Number of decals. */
	inline int GetDecalCount() const{ return pDecalCount; }
	
	/** \brief Root decal or NULL if there are none. */
	inline deDecal *GetRootDecal() const{ return pDecalRoot; }
	
	/**
	 * \brief Add decal.
	 * \throws deeInvalidParam \em decal is NULL.
	 * \throws deeInvalidParam \em decal has a parent world.
	 */
	void AddDecal(deDecal *decal);
	
	/**
	 * \brief Remove decal.
	 * \throws deeInvalidParam \em decal is NULL.
	 * \throws deeInvalidParam Parent world of \em decal is not this world.
	 */
	void RemoveDecal(deDecal *decal);
	
	/** \brief Remove all decals. */
	void RemoveAllDecals();
	/*@}*/
	
	
	
	/** \name Navigation spaces */
	/*@{*/
	/** \brief Number of navigation spaces. */
	int GetNavSpaceCount() const;
	
	/** \brief Navigation space at index. */
	deHeightTerrainNavSpace *GetNavSpaceAt(int index) const;
	
	/** \brief Index of navigation space or -1 if absent. */
	int IndexOfNavSpace(deHeightTerrainNavSpace *navspace) const;
	
	/** \brief Navigation space is present. */
	bool HasNavSpace(deHeightTerrainNavSpace *navspace) const;
	
	/** \brief Add navigation space. */
	void AddNavSpace(deHeightTerrainNavSpace *navspace);
	
	/** \brief Remove navigation space. */
	void RemoveNavSpace(deHeightTerrainNavSpace *navspace);
	
	/** \brief Remove all navigation spaces. */
	void RemoveAllNavSpaces();
	
	/** \brief Notify peers navigation space layer changed. */
	void NotifyNavSpaceLayerChanged(int navspace);
	
	/** \brief Notify peers navigation space type changed. */
	void NotifyNavSpaceTypeChanged(int navspace);
	
	/** \brief Notify peers navigation space snapping changed. */
	void NotifyNavSpaceSnappingChanged(int navspace);
	
	/** \brief Notify peers navigation space layout changed. */
	void NotifyNavSpaceLayoutChanged(int navspace);
	/*@}*/
	
	
	
private:
	void pCreateVisibleFaces();
};

#endif
