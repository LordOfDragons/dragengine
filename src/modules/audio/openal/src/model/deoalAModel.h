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

#ifndef _DEOALAMODEL_H_
#define _DEOALAMODEL_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decStringList.h>

class deoalModelFace;
class deoalAudioThread;
class deoalRayCache;

class deoalModelOctree;
class deoalModelRTOctree;
class deoalModelRTBVH;
class deModel;
class deModelLOD;



/**
 * \brief Audio model.
 */
class deoalAModel : public deObject{
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoalAModel> Ref;


public:
	/** \brief Weight. */
	struct sWeight{
		int bone;
		float weight;
	};
	
	/** \brief Weight set. */
	struct sWeightSet{
		int firstWeight;
		int weightCount;
	};
	
	
	
private:
	deoalAudioThread &pAudioThread;
	decString pFilename;
	
	decStringList pBoneNames;
	decStringList pTextureNames;
	
	deoalModelFace *pFaces;
	int pFaceCount;
	
	sWeight *pWeights;
	int pWeightCount;
	sWeightSet *pWeightSets;
	int pWeightSetCount;
	
	decVector pMinExtend;
	decVector pMaxExtend;
	decVector pSize;
// 	decVector pRTSphereCenter;
// 	float pRTSphereRadiusSquared;
	
	deoalModelOctree *pOctree;
	deoalModelOctree *pOctreeOverlap;
// 	deoalModelRTOctree *pRTOctree;
	deoalModelRTBVH *pRTBVH;
	
	deoalRayCache *pRayCache;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create audio model. */
	deoalAModel(deoalAudioThread &audioThread, const deModel &model);
	
protected:
	/** \brief Clean up model peer. */
	virtual ~deoalAModel();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deoalAudioThread &GetAudioThread() const{ return pAudioThread; }
	
	/** \brief Filename of resource. */
	inline const decString &GetFilename() const{ return pFilename; }
	
	
	
	/** \brief List of bone names. */
	inline const decStringList &GetBoneNames() const{ return pBoneNames; }
	
	/** \brief List of texture names. */
	inline const decStringList &GetTextureNames() const{ return pTextureNames; }
	
	
	
	/** \brief Face count. */
	inline int GetFaceCount() const{ return pFaceCount; }
	
	/** \brief Face at index. */
	const deoalModelFace &GetFaceAt(int index) const;
	
	
	
	/** \brief Weights. */
	inline const sWeight *GetWeights() const{ return pWeights; }
	
	/** \brief Number of weights. */
	inline int GetWeightCount() const{ return pWeightCount; }
	
	/** \brief Weight sets. */
	inline sWeightSet *GetWeightSets() const{ return pWeightSets; }
	
	/** \brief Weight set count. */
	inline int GetWeightSetCount() const{ return pWeightSetCount; }
	
	
	
	/** \brief Minimum extend. */
	inline const decVector &GetMinExtend() const{ return pMinExtend; }
	
	/** \brief Maximum extend. */
	inline const decVector &GetMaxExtend() const{ return pMaxExtend; }
	
	/** \brief Size. */
	inline const decVector &GetSize() const{ return pSize; }
	
	/** \brief Ray tracing sphere center. */
// 	inline const decVector &GetRTSphereCenter() const{ return pRTSphereCenter; }
	
	/** \brief Ray tracing sphere radius squared. */
// 	inline float GetRTSphereRadiusSquared() const{ return pRTSphereRadiusSquared; }
	
	
	
	/** \brief Octree or NULL if not present. */
	inline deoalModelOctree *GetOctree() const{ return pOctree; }
	
	/** \brief Overlap octree or NULL if not present. */
	inline deoalModelOctree *GetOctreeOverlap() const{ return pOctreeOverlap; }
	
	/** \brief Ray-tracing octree or NULL if not present. */
// 	inline deoalModelRTOctree *GetRTOctree() const{ return pRTOctree; }
	
	/** \brief Ray-tracing BVH or NULL if not present. */
	inline deoalModelRTBVH *GetRTBVH() const{ return pRTBVH; }
	
	/** \brief Prepare octree if not present. */
	void PrepareOctree();
	
	
	
	/** \brief Ray cache. */
	inline deoalRayCache &GetRayCache(){ return *pRayCache; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pInitBoneNames(const deModel &model);
	void pInitTextureNames(const deModel &model);
	void pBuildWeights(const deModelLOD &lod);
	void pBuildFaces(const deModelLOD &lod);
	void pBuildOctree();
// 	void pInitRTSphere( const deModelLOD &lod );
	
	void pDebugLogOctreePerfMetrics(const deoalModelOctree &octree);
	void pDebugLogOctreePerfMetrics(const deoalModelOctree &node, int level, int &count);
};

#endif
