/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLRMODEL_H_
#define _DEOGLRMODEL_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/math/decMath.h>

class deoglImposterBillboard;
class deoglModelLOD;
class deoglRenderThread;
class deoglSharedSPBListUBO;

class deModel;



/**
 * \brief OpenGL Model Peer.
 */
class deoglRModel : public deObject{
public:
	struct sExtends{
		decVector minimum;
		decVector maximum;
	};
	
	
	
private:
	deoglRenderThread &pRenderThread;
	decString pFilename;
	
	sExtends pExtends;
	sExtends pWeightlessExtends;
	bool pHasWeightlessExtends;
	sExtends *pBoneExtends;
	int pBoneCount;
	
	decStringList pBoneNames;
	decStringList pTextureNames;
	
	deoglModelLOD **pLODs;
	int pLODCount;
	
	bool pDoubleSided;
	bool pIsCached;
	
	deoglImposterBillboard *pImposterBillboard;
	
	deoglSharedSPBListUBO *pSharedSPBListUBO;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render model. */
	deoglRModel( deoglRenderThread &renderThread, const deModel &model );
	
	/** \brief Clean up render model. */
	virtual ~deoglRModel();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** \brief Filename. */
	inline const decString &GetFilename() const{ return pFilename; }
	
	/** \brief Extends. */
	inline const sExtends &GetExtends() const{ return pExtends; }
	
	/** \brief Weightless extends. */
	inline const sExtends &GetWeightlessExtends() const{ return pWeightlessExtends; }
	
	/** \brief Has weightless extends. */
	inline bool GetHasWeightlessExtends() const{ return pHasWeightlessExtends; }
	
	/** \brief Bone extends. */
	inline const sExtends *GetBoneExtends() const{ return pBoneExtends; }
	
	/** \brief Number of bones. */
	inline int GetBoneCount() const{ return pBoneCount; }
	
	/** \brief List of bone names. */
	inline const decStringList &GetBoneNames() const{ return pBoneNames; }
	
	/** \brief List of texture names. */
	inline const decStringList &GetTextureNames() const{ return pTextureNames; }
	
	
	
	/** \brief Number of lods. */
	inline int GetLODCount() const{ return pLODCount; }
	
	/** \brief Lod at index. */
	deoglModelLOD &GetLODAt( int index ) const;
	
	/** \brief Model has double sided textures. */
	inline bool GetDoubleSided() const{ return pDoubleSided; }
	
	/** \brief Prepare imposter billboard. */
	void PrepareImposterBillboard();
	
	/** \brief Imposter billboard or \em NULL if not existing. */
	inline deoglImposterBillboard *GetImposterBillboard() const{ return pImposterBillboard; }
	
	/** \brief Shared shader parameter block list using UBO. */
	deoglSharedSPBListUBO &GetSharedSPBListUBO();
	
	/** \brief Prepare octrees if not existing. */
	void PrepareOctrees();
	
	/** \brief Print debug informations. */
	void PrintDebugInfo();
	
	/** \brief Debug VCOptimize. */
	void DebugVCOptimize();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pInitTextureNames( const deModel &engModel );
	void pInitBoneNames( const deModel &engModel );
	void pInitLODs( const deModel &engModel );
	void pInitExtends( const deModel &engModel );
	
	void pLoadCached( int lodCount, int boneCount );
	void pSaveCached();
	
	void pCreateImposterBillboard();
};

#endif
