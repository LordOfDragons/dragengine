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
class deModelVertexPositionSet;
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
	
	deModelVertexPositionSet **pVertexPositionSets;
	int pVertexPositionSetCount;
	int pVertexPositionSetSize;
	
	deBaseGraphicModel *pPeerGraphic;
	deBasePhysicsModel *pPeerPhysics;
	deBaseAudioModel *pPeerAudio;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new model object with the given resource manager and filename. */
	deModel(deModelManager *manager, deVirtualFileSystem *vfs, const char *filename,
		TIME_SYSTEM modificationTime);
	
protected:
	/**
	 * \brief Clean up model.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~deModel() override;
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
	deModelBone *GetBoneAt(int index) const;
	
	/** \brief Index of the bone with the given name or -1 if not found. */
	int IndexOfBoneNamed(const char *name) const;
	
	/** \brief Determiens if a bone with the given name exists. */
	bool HasBoneNamed(const char *name) const;
	
	/** \brief Adds a new bone with the given name. */
	void AddBone(deModelBone *bone);
	/*@}*/
	
	
	
	/** \name Texture Management */
	/*@{*/
	/** \brief Count of textures. */
	inline int GetTextureCount() const{ return pTextureCount; }
	
	/** \brief Texture at the given index. */
	deModelTexture *GetTextureAt(int index) const;
	
	/** \brief Index of the texture with the given name or -1 if not found. */
	int IndexOfTextureNamed(const char *name) const;
	
	/** \brief Determiens if a texture with the given name exists. */
	bool HasTextureNamed(const char *name) const;
	
	/** \brief Adds a new texture with the given name and size. */
	void AddTexture(deModelTexture *texture);
	/*@}*/
	
	
	
	/** \name LOD Management */
	/*@{*/
	/** \brief Count of LOD meshes. */
	inline int GetLODCount() const{ return pLODCount; }
	
	/** \brief LOD mesh at the given index. */
	deModelLOD *GetLODAt(int index) const;
	
	/** \brief Adds a new LOD mesh. */
	void AddLOD(deModelLOD *lod);
	/*@}*/
	
	
	
	/** \name Vertex position set management */
	/*@{*/
	/** \brief Count of vertex position sets. */
	inline int GetVertexPositionSetCount() const{ return pVertexPositionSetCount; }
	
	/** \brief Vertex position set at index. */
	deModelVertexPositionSet *GetVertexPositionSetAt(int index) const;
	
	/** \brief Index of named vertex position set with name or -1 if absent. */
	int IndexOfVertexPositionSetNamed(const char *name) const;
	
	/** \brief Named vertex position set is present. */
	bool HasVertexPositionSetNamed(const char *name) const;
	
	/** \brief Add vertex position set. */
	void AddVertexPositionSet(deModelVertexPositionSet *set);
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic system peer. */
	inline deBaseGraphicModel *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphic system peer. */
	void SetPeerGraphic(deBaseGraphicModel *peer);
	
	/** \brief Physics system peer. */
	inline deBaseAudioModel *GetPeerAudio() const{ return pPeerAudio; }
	
	/** \brief Set physics system peer. */
	void SetPeerAudio (deBaseAudioModel *peer);
	
	/** \brief Physics system peer. */
	inline deBasePhysicsModel *GetPeerPhysics() const{ return pPeerPhysics; }
	
	/** \brief Set physics system peer. */
	void SetPeerPhysics(deBasePhysicsModel *peer);
	/*@}*/
	
	
	
private:
	void pCalcBoneMatrices();
};

#endif
