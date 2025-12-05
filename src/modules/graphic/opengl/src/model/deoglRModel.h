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
 * OpenGL Model Peer.
 */
class deoglRModel : public deObject{
public:
	/** Type holding strong reference. */
	typedef deTObjectReference<deoglRModel> Ref;
	
	
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
	decStringList pVPSNames;
	
	deoglModelLOD **pLODs;
	int pLODCount;
	
	bool pDoubleSided;
	bool pIsCached;
	
	deoglImposterBillboard *pImposterBillboard;
	
	deoglSharedSPBListUBO *pSharedSPBListUBO;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render model. */
	deoglRModel( deoglRenderThread &renderThread, const deModel &model );
	
	/** Clean up render model. */
	virtual ~deoglRModel();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Filename. */
	inline const decString &GetFilename() const{ return pFilename; }
	
	/** Extends. */
	inline const sExtends &GetExtends() const{ return pExtends; }
	
	/** Weightless extends. */
	inline const sExtends &GetWeightlessExtends() const{ return pWeightlessExtends; }
	
	/** Has weightless extends. */
	inline bool GetHasWeightlessExtends() const{ return pHasWeightlessExtends; }
	
	/** Bone extends. */
	inline const sExtends *GetBoneExtends() const{ return pBoneExtends; }
	
	/** Number of bones. */
	inline int GetBoneCount() const{ return pBoneCount; }
	
	/** List of bone names. */
	inline const decStringList &GetBoneNames() const{ return pBoneNames; }
	
	/** List of texture names. */
	inline const decStringList &GetTextureNames() const{ return pTextureNames; }
	
	/** List of vertex position set names. */
	inline const decStringList &GetVPSNames() const{ return pVPSNames; }
	
	
	
	/** Number of lods. */
	inline int GetLODCount() const{ return pLODCount; }
	
	/** Lod at index. */
	deoglModelLOD &GetLODAt( int index ) const;
	
	/** Model has double sided textures. */
	inline bool GetDoubleSided() const{ return pDoubleSided; }
	
	/** Prepare imposter billboard. */
	void PrepareImposterBillboard();
	
	/** Imposter billboard or \em NULL if not existing. */
	inline deoglImposterBillboard *GetImposterBillboard() const{ return pImposterBillboard; }
	
	/** Shared shader parameter block list using UBO. */
	deoglSharedSPBListUBO &GetSharedSPBListUBO();
	
	/** Prepare octrees if not existing. */
	void PrepareOctrees();
	
	/** Print debug information. */
	void PrintDebugInfo();
	
	/** Debug VCOptimize. */
	void DebugVCOptimize();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pInitTextureNames( const deModel &engModel );
	void pInitBoneNames( const deModel &engModel );
	void pInitVPSNames( const deModel &engModel );
	void pInitLODs( const deModel &engModel );
	void pInitExtends( const deModel &engModel, const deoglModelLOD &baseLod );
	
	void pLoadCached( int lodCount, int boneCount );
	void pSaveCached();
	
	void pCreateImposterBillboard();
};

#endif
