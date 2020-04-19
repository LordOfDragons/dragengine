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

#ifndef _DEOGLRPARTICLEEMITTERINSTANCE_H_
#define _DEOGLRPARTICLEEMITTERINSTANCE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decLayerMask.h>

#include "../deoglBasics.h"

class deoglRenderThread;
class deoglEnvironmentMap;
class deoglRParticleEmitter;
class deoglRParticleEmitterInstanceType;
class deoglVAO;
class deoglRWorld;
class deoglWorldOctree;

class deParticleEmitterInstance;


/**
 * \brief Particle Emitter Instance Peer.
 */
class deoglRParticleEmitterInstance : public deObject{
public:
	/** \brief Particle data. */
	struct sParticle{
		deoglRParticleEmitterInstance *emitterInstance;
		int type;
		int particle;
		signed char ribbonLine[ 3 ];
		unsigned char renderType;
	};
	
	/** \brief Local vbo data. */
	struct sLocalVBOData{
		/** \brief Beam location from 0 to 1. */
		float beamLocation;
	};
	
private:
	deoglRenderThread &pRenderThread;
	
	deoglRParticleEmitter *pEmitter;
	
	deoglRWorld *pParentWorld;
	deoglWorldOctree *pOctreeNode;
	
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
	
	deoglEnvironmentMap *pRenderEnvMap;
	bool pDirtyRenderEnvMap;
	
	GLuint pVBOShared;
	GLuint pVBOLocal;
	GLuint pIBO;
	deoglVAO *pVAO;
	
	bool pDirtyParticles;
	
	bool pWorldMarkedRemove;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render emitter instance. */
	deoglRParticleEmitterInstance( deoglRenderThread &renderThread );
	
	/** \brief Clean up render emitter instance. */
	virtual ~deoglRParticleEmitterInstance();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	
	
	/** \brief Emitter or \em NULL if not set. */
	inline deoglRParticleEmitter *GetEmitter() const{ return pEmitter; }
	
	/** \brief Set emitter or \em NULL if not set. */
	void SetEmitter( deoglRParticleEmitter *emitter );
	
	
	
	/** \brief Parent world. */
	inline deoglRWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world. */
	void SetParentWorld( deoglRWorld *world );
	
	
	
	/** \brief Octree node or \em NULL if there is none. */
	inline deoglWorldOctree *GetOctreeNode() const{ return pOctreeNode; }
	
	/** \brief Set octree node or \em NULL if there is none. */
	void SetOctreeNode( deoglWorldOctree *node );
	
	/** \brief Update octree node. */
	void UpdateOctreeNode();
	
	
	
	/** \brief Burst time. */
	inline float GetBurstTime() const{ return pBurstTime; }
	
	/** \brief Set burst time. */
	void SetBurstTime( float burstTime );
	
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decDVector &position );
	
	/** \brief Reference position. */
	inline const decDVector &GetReferencePosition() const{ return pReferencePosition; }
	
	/** \brief Set reference position. */
	void SetReferencePosition( const decDVector &position );
	
	/** \brief Layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** \brief Set layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	
	
	/** \brief Prepare for rendering. */
	void PrepareForRender();
	
	
	
	/** \brief VAO. */
	inline deoglVAO *GetVAO() const{ return pVAO; }
	
	
	
	/** \brief Minimum extend. */
	inline const decDVector &GetMinExtend() const{ return pMinExtend; }
	
	/** \brief Maximum extend. */
	inline const decDVector &GetMaxExtend() const{ return pMaxExtend; }
	
	/** \brief Update extends. */
	void UpdateExtends( const deParticleEmitterInstance &instance );
	
	
	
	/** \brief Render environment map or \em NULL if not used. */
	inline deoglEnvironmentMap *GetRenderEnvMap() const{ return pRenderEnvMap; }
	
	/** \brief Set render environment map or \em NULL if not assigned yet. */
	void SetRenderEnvMap( deoglEnvironmentMap *envmap );
	
	
	
	/** \brief The world environment map layout changed. */
	void WorldEnvMapLayoutChanged();
	
	/** \brief Update the render environment map. */
	void UpdateRenderEnvMap();
	
	/** \brief Invalidates the render environment map. */
	void InvalidateRenderEnvMap();
	
	/** \brief Invalidates the render environment map. */
	void InvalidateRenderEnvMapIf( deoglEnvironmentMap *envmap );
	
	/** \brief World reference point changed. */
	void WorldReferencePointChanged();
	
	
	
	/** \brief Prepare for quick disposal of particle emitter instance. */
	void PrepareQuickDispose();
	/*@}*/
	
	
	
	/** \name Particles. */
	/*@{*/
	/** \brief Particles. */
	inline const sParticle *GetParticles() const{ return pParticles; }
	
	/** \brief Number of particles. */
	inline int GetParticleCount() const{ return pParticleCount; }
	
	/** \brief Update particles. */
	void UpdateParticles( const deParticleEmitterInstance &instance );
	
	/** \brief Update particles. */
	void UpdateParticlesVBO();
	
	/** \brief Release particles from memory. */
	void ReleaseParticles();
	
	/**
	 * \brief Get particle position.
	 * \warning Temporary HACK!
	 */
	decVector GetParticlePositionAt( int index ) const;
	/*@}*/
	
	
	
	/** \name Types. */
	/*@{*/
	/** \brief Number of types. */
	int GetTypeCount() const;
	
	/** \brief Type by index. */
	deoglRParticleEmitterInstanceType &GetTypeAt( int index ) const;
	
	/** \brief Remove all types. */
	void RemoveAllTypes();
	
	/** \brief Add type. */
	void AddType( deoglRParticleEmitterInstanceType *type );
	
	/** \brief Invalidate parameter blocks of all types. */
	void InvalidateAllTypesParamBlocks();
	
	/** \brief Mark parameter blocks of all types dirty. */
	void MarkAllTypesParamBlocksDirty();
	
	/** \brief Mark texture units configurations of all types dirty. */
	void MarkAllTypesTUCsDirty();
	/*@}*/
	
	
	
	/** \name Index Buffer */
	/*@{*/
	/** \brief Number of indices used in the index buffer. */
	inline int GetIBOUsedIndexCount() const{ return pIndexUsedCount; }
	
	/** \brief Remove all entries from the index buffer. */
	void ClearIBO();
	
	/** \brief Add index buffer entry. */
	void AddIBOEntry( int index );
	
	/** \brief Add four index buffer entries at the same time. */
	void AddIBOEntries( int index1, int index2, int index3, int index4 );
	
	/** \brief Update index buffer if required. */
	void UpdateIBO();
	/*@}*/
	
	
	
	/** \name Render world usage */
	/*@{*/
	/**
	 * \brief Marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	inline bool GetWorldMarkedRemove() const{ return pWorldMarkedRemove; }
	
	/**
	 * \brief Set marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	void SetWorldMarkedRemove( bool marked );
	/*@}*/
};

#endif
