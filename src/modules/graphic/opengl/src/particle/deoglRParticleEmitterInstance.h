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

#ifndef _DEOGLRPARTICLEEMITTERINSTANCE_H_
#define _DEOGLRPARTICLEEMITTERINSTANCE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decLayerMask.h>

#include "../deoglBasics.h"
#include "../world/deoglWorldComputeElement.h"
#include "deoglRParticleEmitter.h"
#include "deoglEnvironmentMap.h"

class deoglRenderThread;
class deoglRParticleEmitterInstanceType;
class deoglVAO;
class deoglRWorld;
class deoglWorldOctree;

class deParticleEmitterInstance;


/**
 * Particle Emitter Instance Peer.
 */
class deoglRParticleEmitterInstance : public deObject{
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoglRParticleEmitterInstance> Ref;


public:
	/** Particle data. */
	struct sParticle{
		deoglRParticleEmitterInstance *emitterInstance;
		int type;
		int particle;
		signed char ribbonLine[3];
		unsigned char renderType;
	};
	
	/** Local vbo data. */
	struct sLocalVBOData{
		/** Beam location from 0 to 1. */
		float beamLocation;
	};
	
private:
	/** World compute element. */
	class WorldComputeElement: public deoglWorldComputeElement{
		deoglRParticleEmitterInstance &pEmitter;
	public:
		WorldComputeElement(deoglRParticleEmitterInstance &emitter);
		void UpdateData(sDataElement &data) const override;
		void UpdateDataGeometries(sDataElementGeometry *data) const override;
	};
	
	
	
	deoglRenderThread &pRenderThread;
	
	deoglRParticleEmitter::Ref pEmitter;
	
	deoglRWorld *pParentWorld;
	deoglWorldOctree *pOctreeNode;
	deoglWorldComputeElement::Ref pWorldComputeElement;
	
	float pBurstTime;
	decDVector pPosition;
	decDVector pReferencePosition;
	decLayerMask pLayerMask;
	
	decObjectList pTypes;
	
	sParticle *pParticles;
	int pParticleCount;
	int pParticleSize;
	
	sLocalVBOData *pLocalVBOData;
	char *pSharedVBOData;
	
	GLushort *pIndices;
	int pIndexCount;
	int pIndexSize;
	int pIndexUsedCount;
	bool pDirtyIBO;
	
	decDVector pMinExtend;
	decDVector pMaxExtend;
	
	deoglEnvironmentMap::Ref pRenderEnvMap;
	bool pDirtyRenderEnvMap;
	
	GLuint pVBOShared;
	GLuint pVBOLocal;
	GLuint pIBO;
	deoglVAO *pVAO;
	
	bool pDirtyParticles;
	
	uint32_t pCSOctreeIndex;
	
	bool pWorldMarkedRemove;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render emitter instance. */
	deoglRParticleEmitterInstance(deoglRenderThread &renderThread);
	
	/** Clean up render emitter instance. */
	virtual ~deoglRParticleEmitterInstance();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	
	
	/** Emitter or \em NULL if not set. */
	inline const deoglRParticleEmitter::Ref &GetEmitter() const{ return pEmitter; }
	
	/** Set emitter or \em NULL if not set. */
	void SetEmitter(deoglRParticleEmitter *emitter);
	
	
	
	/** Parent world. */
	inline deoglRWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** Set parent world. */
	void SetParentWorld(deoglRWorld *world);
	
	
	
	/** Octree node or \em NULL if there is none. */
	inline deoglWorldOctree *GetOctreeNode() const{ return pOctreeNode; }
	
	/** Set octree node or \em NULL if there is none. */
	void SetOctreeNode(deoglWorldOctree *node);
	
	/** Update octree node. */
	void UpdateOctreeNode();
	
	
	
	/** Burst time. */
	inline float GetBurstTime() const{ return pBurstTime; }
	
	/** Set burst time. */
	void SetBurstTime(float burstTime);
	
	/** Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** Set position. */
	void SetPosition(const decDVector &position);
	
	/** Reference position. */
	inline const decDVector &GetReferencePosition() const{ return pReferencePosition; }
	
	/** Set reference position. */
	void SetReferencePosition(const decDVector &position);
	
	/** Layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** Set layer mask. */
	void SetLayerMask(const decLayerMask &layerMask);
	
	
	
	/** Prepare for rendering. */
	void PrepareForRender();
	
	
	
	/** VAO. */
	inline deoglVAO *GetVAO() const{ return pVAO; }
	
	
	
	/** Minimum extend. */
	inline const decDVector &GetMinExtend() const{ return pMinExtend; }
	
	/** Maximum extend. */
	inline const decDVector &GetMaxExtend() const{ return pMaxExtend; }
	
	/** Update extends. */
	void UpdateExtends(const deParticleEmitterInstance &instance);
	
	
	
	/** Render environment map or \em NULL if not used. */
	inline const deoglEnvironmentMap::Ref &GetRenderEnvMap() const{ return pRenderEnvMap; }
	
	/** Set render environment map or \em NULL if not assigned yet. */
	void SetRenderEnvMap(deoglEnvironmentMap *envmap);
	
	
	
	/** The world environment map layout changed. */
	void WorldEnvMapLayoutChanged();
	
	/** Update the render environment map. */
	void UpdateRenderEnvMap();
	
	/** Invalidates the render environment map. */
	void InvalidateRenderEnvMap();
	
	/** Invalidates the render environment map. */
	void InvalidateRenderEnvMapIf(deoglEnvironmentMap *envmap);
	
	/** World reference point changed. */
	void WorldReferencePointChanged();
	
	
	
	/** Prepare for quick disposal of particle emitter instance. */
	void PrepareQuickDispose();
	
	
	
	/** Compute shader octree index. */
	inline uint32_t GetCSOctreeIndex() const{ return pCSOctreeIndex; }
	void SetCSOctreeIndex(uint32_t index){pCSOctreeIndex = index;}
	/*@}*/
	
	
	
	/** \name Particles. */
	/*@{*/
	/** Particles. */
	inline const sParticle *GetParticles() const{ return pParticles; }
	
	/** Number of particles. */
	inline int GetParticleCount() const{ return pParticleCount; }
	
	/** Update particles. */
	void UpdateParticles(const deParticleEmitterInstance &instance);
	
	/** Update particles. */
	void UpdateParticlesVBO();
	
	/** Release particles from memory. */
	void ReleaseParticles();
	
	/**
	 * Get particle position.
	 * \warning Temporary HACK!
	 */
	decVector GetParticlePositionAt(int index) const;
	/*@}*/
	
	
	
	/** \name Types. */
	/*@{*/
	/** Number of types. */
	int GetTypeCount() const;
	
	/** Type by index. */
	deoglRParticleEmitterInstanceType &GetTypeAt(int index) const;
	
	/** Remove all types. */
	void RemoveAllTypes();
	
	/** Add type. */
	void AddType(deoglRParticleEmitterInstanceType *type);
	
	/** Invalidate parameter blocks of all types. */
	void InvalidateAllTypesParamBlocks();
	
	/** Mark parameter blocks of all types dirty. */
	void MarkAllTypesParamBlocksDirty();
	
	/** Mark texture units configurations of all types dirty. */
	void MarkAllTypesTUCsDirty();
	/*@}*/
	
	
	
	/** \name Index Buffer */
	/*@{*/
	/** Number of indices used in the index buffer. */
	inline int GetIBOUsedIndexCount() const{ return pIndexUsedCount; }
	
	/** Remove all entries from the index buffer. */
	void ClearIBO();
	
	/** Add index buffer entry. */
	void AddIBOEntry(int index);
	
	/** Add four index buffer entries at the same time. */
	void AddIBOEntries(int index1, int index2, int index3, int index4);
	
	/** Update index buffer if required. */
	void UpdateIBO();
	/*@}*/
	
	
	
	/** \name Render world usage */
	/*@{*/
	/**
	 * Marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	inline bool GetWorldMarkedRemove() const{ return pWorldMarkedRemove; }
	
	/**
	 * Set marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	void SetWorldMarkedRemove(bool marked);
	/*@}*/
};

#endif
