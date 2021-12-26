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

#ifndef _DEMODEL_H_
#define _DEMODEL_H_

#include "../deFileResource.h"
#include "../../common/math/decMath.h"
#include "../../common/string/decStringList.h"

class deModelBone;
class deModelTexture;
class deModelLOD;
class deModelManager;
class deBaseGraphicModel;
class deBasePhysicsModel;
class deBaseAudioModel;

class deModelWeight;


/**
 * \brief Model Resource.
 * 
 * Triangle mesh representing visual geometry of a Component in a scene. Contains various
 * information to support texturing, animation, level-of-detail and physics.
 */
class DE_DLL_EXPORT deModel : public deFileResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deModel> Ref;
	
	
	
private:
	deModelBone **pBones;
	int pBoneCount;
	int pBoneSize;
	
	deModelTexture **pTextures;
	int pTextureCount;
	int pTextureSize;
	
	deModelLOD **pLODs;
	int pLODCount;
	int pLODSize;
	
	decStringList pTextureCoordinatesSetList;
	
	deBaseGraphicModel *pPeerGraphic;
	deBasePhysicsModel *pPeerPhysics;
	deBaseAudioModel *pPeerAudio;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new model object with the given resource manager and filename. */
	deModel( deModelManager *manager, deVirtualFileSystem *vfs, const char *filename,
		TIME_SYSTEM modificationTime );
	
protected:
	/**
	 * \brief Clean up model.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deModel();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief List of texture coordinates sets names. */
	inline decStringList &GetTextureCoordinatesSetList(){ return pTextureCoordinatesSetList; }
	inline const decStringList &GetTextureCoordinatesSetList() const{ return pTextureCoordinatesSetList; }
	/*@}*/
	
	
	
	/** \name Loading */
	/*@{*/
	/** \brief Verifies that the data in the model are valid. */
	bool Verify();
	
	/** \brief Prepare model for later use. */
	void Prepare();
	/*@}*/
	
	
	
	/** \name Bone Management */
	/*@{*/
	/** \brief Count of bones. */
	inline int GetBoneCount() const{ return pBoneCount; }
	
	/** \brief Bone at the given index. */
	deModelBone *GetBoneAt( int index ) const;
	
	/** \brief Index of the bone with the given name or -1 if not found. */
	int IndexOfBoneNamed( const char *name ) const;
	
	/** \brief Determiens if a bone with the given name exists. */
	bool HasBoneNamed( const char *name ) const;
	
	/** \brief Adds a new bone with the given name. */
	void AddBone( deModelBone *bone );
	/*@}*/
	
	
	
	/** \name Texture Management */
	/*@{*/
	/** \brief Count of textures. */
	inline int GetTextureCount() const{ return pTextureCount; }
	
	/** \brief Texture at the given index. */
	deModelTexture *GetTextureAt( int index ) const;
	
	/** \brief Index of the texture with the given name or -1 if not found. */
	int IndexOfTextureNamed( const char *name ) const;
	
	/** \brief Determiens if a texture with the given name exists. */
	bool HasTextureNamed( const char *name ) const;
	
	/** \brief Adds a new texture with the given name and size. */
	void AddTexture( deModelTexture *texture );
	/*@}*/
	
	
	
	/** \name LOD Management */
	/*@{*/
	/** \brief Count of LOD meshes. */
	inline int GetLODCount() const{ return pLODCount; }
	
	/** \brief LOD mesh at the given index. */
	deModelLOD *GetLODAt( int index ) const;
	
	/** \brief Adds a new LOD mesh. */
	void AddLOD( deModelLOD *lod );
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic system peer. */
	inline deBaseGraphicModel *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphic system peer. */
	void SetPeerGraphic( deBaseGraphicModel *peer );
	
	/** \brief Physics system peer. */
	inline deBaseAudioModel *GetPeerAudio() const{ return pPeerAudio; }
	
	/** \brief Set physics system peer. */
	void SetPeerAudio ( deBaseAudioModel *peer );
	
	/** \brief Physics system peer. */
	inline deBasePhysicsModel *GetPeerPhysics() const{ return pPeerPhysics; }
	
	/** \brief Set physics system peer. */
	void SetPeerPhysics( deBasePhysicsModel *peer );
	/*@}*/
	
	
	
private:
	void pCalcBoneMatrices();
};

#endif
