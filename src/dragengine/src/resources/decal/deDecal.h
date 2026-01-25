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

#ifndef _DEDECAL_H_
#define _DEDECAL_H_

#include "../deResource.h"
#include "../skin/deSkin.h"
#include "../skin/dynamic/deDynamicSkin.h"
#include "../../common/math/decMath.h"
#include "../../common/collection/decTOrderedSet.h"
#include "../../common/collection/decTLinkedList.h"
#include "../../common/collection/decTList.h"

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
class DE_DLL_EXPORT deDecal : public deResource{
public:
	/** \brief Type holding strong reference. */
	using Ref = deTObjectReference<deDecal>;
	
	/** \brief List typedef. */
	using List = decTObjectOrderedSet<deDecal>;
	
	
private:
	decVector pPosition;
	decQuaternion pOrientation;
	decVector pSize;
	decTexMatrix2 pTransform;
	
	deSkin::Ref pSkin;
	int pTexture;
	deDynamicSkin::Ref pDynamicSkin;
	
	bool pVisible;
	
	decTList<deDecalBoneState> pBoneStates;
	
	deComponent *pParentComponent;
	decTObjectLinkedList<deDecal>::Element pLLComponent;
	
	deHeightTerrainSector *pParentHeightTerrainSector;
	decTObjectLinkedList<deDecal>::Element pLLHeightTerrainSector;
	
	deBaseGraphicDecal *pPeerGraphic;
	deBasePhysicsDecal *pPeerPhysics;
	deBaseAudioDecal *pPeerAudio;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create decal. */
	deDecal(deDecalManager *manager);
	
protected:
	/**
	 * \brief Clean up decal.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~deDecal() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition(const decVector &position);
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation(const decQuaternion &orientation);
	
	/** \brief Size. */
	inline const decVector &GetSize() const{ return pSize; }
	
	/** \brief Set size. */
	void SetSize(const decVector &size);
	
	/** \brief Texture coordinate transformation matrix. */
	inline const decTexMatrix2 &GetTransform() const{ return pTransform; }
	
	/** \brief Set texture coordinate transformation matrix. */
	void SetTransform(const decTexMatrix2 &matrix);
	
	/** \brief Skin. */
	inline const deSkin::Ref &GetSkin() const{ return pSkin; }
	
	/** \brief Set skin. */
	void SetSkin(deSkin *skin);
	
	/** \brief Texture number in the skin. */
	inline int GetTexture() const{ return pTexture; }
	
	/** \brief Set texture number in the skin. */
	void SetTexture(int texture);
	
	/** \brief Dynamic skin or NULL if not used. */
	inline const deDynamicSkin::Ref &GetDynamicSkin() const{ return pDynamicSkin; }
	
	/** \brief Set dynamic skin or NULL if not used. */
	void SetDynamicSkin(deDynamicSkin *dynamicSkin);
	
	/** \brief Decal is visible. */
	inline bool GetVisible() const{ return pVisible; }
	
	/** \brief Set if decal is visible. */
	void SetVisible(bool visible);
	/*@}*/
	
	
	
	/** \name Bone States */
	/*@{*/
	/** \brief Bone states. */
	inline const decTList<deDecalBoneState> &GetBoneStates() const{ return pBoneStates; }
	
	/** \brief Number of bone states. */
	inline int GetBoneStateCount() const{ return pBoneStates.GetCount(); }
	
	/** \brief Set number of bone states. */
	void SetBoneStateCount(int count);
	
	/** \brief Bone state at the given index. */
	deDecalBoneState &GetBoneStateAt(int index);
	
	/** \brief Copy current bone state from the provided component. */
	void SnapshotBoneStatesFrom(deComponent *component);
	/*@}*/
	
	
	
	/** \name Component linked list */
	/*@{*/
	/** \brief Parent component or NULL. */
	inline deComponent *GetParentComponent() const{ return pParentComponent; }
	
	/** \brief Set parent component or NULL. */
	void SetParentComponent(deComponent *component);
	
	/** \brief Component linked list. */
	inline decTObjectLinkedList<deDecal>::Element &GetLLComponent(){ return pLLComponent; }
	inline const decTObjectLinkedList<deDecal>::Element &GetLLComponent() const{ return pLLComponent; }
	/*@}*/
	
	
	
	/** \name Height terrain sector linked list */
	/*@{*/
	/** \brief Parent height terrain sector or NULL. */
	inline deHeightTerrainSector *GetParentHeightTerrainSector() const{ return pParentHeightTerrainSector; }
	
	/** \brief Set parent height terrain sector or NULL. */
	void SetParentHeightTerrainSector(deHeightTerrainSector *sector);
	
	/** \brief Height terrain sector linked list. */
	inline decTObjectLinkedList<deDecal>::Element &GetLLHeightTerrainSector(){ return pLLHeightTerrainSector; }
	inline const decTObjectLinkedList<deDecal>::Element &GetLLHeightTerrainSector() const{ return pLLHeightTerrainSector; }
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic system peer. */
	inline deBaseGraphicDecal *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphic system peer. */
	void SetPeerGraphic(deBaseGraphicDecal *peer);
	
	/** \brief Physics system peer. */
	inline deBasePhysicsDecal *GetPeerPhysics() const{ return pPeerPhysics; }
	
	/** \brief Set physics system peer. */
	void SetPeerPhysics(deBasePhysicsDecal *peer);
	
	/** \brief Audio system peer. */
	inline deBaseAudioDecal *GetPeerAudio() const{ return pPeerAudio; }
	
	/** \brief Set audio system peer. */
	void SetPeerAudio(deBaseAudioDecal *peer);
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pRestoreBoneStates();
};

#endif
