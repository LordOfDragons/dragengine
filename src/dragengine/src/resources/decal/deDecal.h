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

#ifndef _DEDECAL_H_
#define _DEDECAL_H_

#include "../deResource.h"
#include "../skin/deSkinReference.h"
#include "../skin/dynamic/deDynamicSkinReference.h"
#include "../../common/math/decMath.h"

class deDecalManager;
class deComponent;
class deHeightTerrainSector;
class deDecalBoneState;
class deBaseGraphicDecal;
class deBasePhysicsDecal;
class deBaseAudioDecal;


/**
 * \brief Decal Resource.
 * 
 * Decals are rectangular textures splatted on a curved surface like a component. Decals have
 * a slight offset from the hosting surface hence they cover the surface below. The ordering
 * of the decal determines which decal covers others. A decal is defined by projection
 * parameters. The actual projection is carried out by the Graphic Module. A decal is first
 * located in space using a position and orientation. The size determines the width and height
 * of the decal on the projection plane previously defined. The texture coordinates defined
 * the texture coordinates of the corners of the decal starting in the upper left corner going
 * around the decal in clockwise order. The decal is then projected along the view direction
 * on the geometry it is attached to.
 */
class deDecal : public deResource{
private:
	decVector pPosition;
	decQuaternion pOrientation;
	decVector pSize;
	decTexMatrix2 pTransform;
	
	deSkinReference pSkin;
	int pTexture;
	deDynamicSkinReference pDynamicSkin;
	
	bool pVisible;
	
	deDecalBoneState *pBoneStates;
	int pBoneStateCount;
	
	deComponent *pParentComponent;
	deDecal *pLLComponentPrev;
	deDecal *pLLComponentNext;
	
	deHeightTerrainSector *pParentHeightTerrainSector;
	deDecal *pLLHeightTerrainSectorPrev;
	deDecal *pLLHeightTerrainSectorNext;
	
	deBaseGraphicDecal *pPeerGraphic;
	deBasePhysicsDecal *pPeerPhysics;
	deBaseAudioDecal *pPeerAudio;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create decal. */
	deDecal( deDecalManager *manager );
	
protected:
	/**
	 * \brief Clean up decal.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deDecal();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decVector &position );
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation( const decQuaternion &orientation );
	
	/** \brief Size. */
	inline const decVector &GetSize() const{ return pSize; }
	
	/** \brief Set size. */
	void SetSize( const decVector &size );
	
	/** \brief Texture coordinate transformation matrix. */
	inline const decTexMatrix2 &GetTransform() const{ return pTransform; }
	
	/** \brief Set texture coordinate transformation matrix. */
	void SetTransform( const decTexMatrix2 &matrix );
	
	/** \brief Skin. */
	inline deSkin *GetSkin() const{ return pSkin; }
	
	/** \brief Set skin. */
	void SetSkin( deSkin *skin );
	
	/** \brief Texture number in the skin. */
	inline int GetTexture() const{ return pTexture; }
	
	/** \brief Set texture number in the skin. */
	void SetTexture( int texture );
	
	/** \brief Dynamic skin or NULL if not used. */
	inline deDynamicSkin *GetDynamicSkin() const{ return pDynamicSkin; }
	
	/** \brief Set dynamic skin or NULL if not used. */
	void SetDynamicSkin( deDynamicSkin *dynamicSkin );
	
	/** \brief Decal is visible. */
	inline bool GetVisible() const{ return pVisible; }
	
	/** \brief Set if decal is visible. */
	void SetVisible( bool visible );
	/*@}*/
	
	
	
	/** \name Bone States */
	/*@{*/
	/** \brief Number of bone states. */
	inline int GetBoneStateCount() const{ return pBoneStateCount; }
	
	/** \brief Set number of bone states. */
	void SetBoneStateCount( int count );
	
	/** \brief Bone state at the given index. */
	deDecalBoneState &GetBoneStateAt( int index );
	
	/** \brief Copy current bone state from the provided component. */
	void SnapshotBoneStatesFrom( deComponent *component );
	/*@}*/
	
	
	
	/** \name Component linked list */
	/*@{*/
	/** \brief Parent component or NULL. */
	inline deComponent *GetParentComponent() const{ return pParentComponent; }
	
	/** \brief Set parent component or NULL. */
	void SetParentComponent( deComponent *component );
	
	/** \brief Previous decal in component linked list. */
	inline deDecal *GetLLComponentPrev() const{ return pLLComponentPrev; }
	
	/**
	 * \brief Set next decal in the component linked list.
	 * \warning For use by deComponent only.
	 */
	void SetLLComponentPrev( deDecal *decal );
	
	/** \brief Next decal in the component linked list. */
	inline deDecal *GetLLComponentNext() const{ return pLLComponentNext; }
	
	/**
	 * \brief Set next decal in the component linked list.
	 * \warning For use by deComponent only.
	 */
	void SetLLComponentNext( deDecal *decal );
	/*@}*/
	
	
	
	/** \name Height terrain sector linked list */
	/*@{*/
	/** \brief Parent height terrain sector or NULL. */
	inline deHeightTerrainSector *GetParentHeightTerrainSector() const{ return pParentHeightTerrainSector; }
	
	/** \brief Set parent height terrain sector or NULL. */
	void SetParentHeightTerrainSector( deHeightTerrainSector *sector );
	
	/** \brief Previous decal in height terrain sector linked list. */
	inline deDecal *GetLLHeightTerrainSectorPrev() const{ return pLLHeightTerrainSectorPrev; }
	
	/**
	 * \brief Set next decal in the height terrain sector linked list.
	 * \warning For use by deHeightTerrainSector only.
	 */
	void SetLLHeightTerrainSectorPrev( deDecal *decal );
	
	/** \brief Next decal in the height terrain sector linked list. */
	inline deDecal *GetLLHeightTerrainSectorNext() const{ return pLLHeightTerrainSectorNext; }
	
	/**
	 * \brief Set next decal in the height terrain sector linked list.
	 * \warning For use by deHeightTerrainSector only.
	 */
	void SetLLHeightTerrainSectorNext( deDecal *decal );
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic system peer. */
	inline deBaseGraphicDecal *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphic system peer. */
	void SetPeerGraphic( deBaseGraphicDecal *peer );
	
	/** \brief Physics system peer. */
	inline deBasePhysicsDecal *GetPeerPhysics() const{ return pPeerPhysics; }
	
	/** \brief Set physics system peer. */
	void SetPeerPhysics( deBasePhysicsDecal *peer );
	
	/** \brief Audio system peer. */
	inline deBaseAudioDecal *GetPeerAudio() const{ return pPeerAudio; }
	
	/** \brief Set audio system peer. */
	void SetPeerAudio( deBaseAudioDecal *peer );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pRestoreBoneStates();
};

#endif
