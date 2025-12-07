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

#ifndef _DEOALENVPROBEMANAGER_H_
#define _DEOALENVPROBEMANAGER_H_

#include "../world/octree/deoalWOVInvalidateEnvProbes.h"
#include "../audiothread/deoalATRayTracing.h"

#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/utils/decLayerMask.h>

class deoalAWorld;
class deoalRTWorldBVH;
class deoalEnvProbe;



/**
 * \brief Environment probe list.
 * 
 * List of environment probes for different kinds of owners. The probes are stored in the list
 * and in the world octree for faster searching. A maximum number of probes is kept in the list
 * reusing the oldest probe if a new probe is required. Probes are searched by position using
 * a reuse distance. A new probe is added if no nearby probe can be found. Probes retrieved
 * by searching are not held by the owner.
 */
class deoalEnvProbeList{
private:
	deoalAWorld &pWorld;
	
	float pRange;
	float pAttenuationRefDist;
	float pAttenuationRolloff;
	float pAttenuationDistanceOffset;
	decLayerMask pLayerMask;
	deoalRTWorldBVH *pRTWorldBVH;
	const deoalATRayTracing::sConfigSoundTracing *pRTConfig;
	
	double pReuseDistance;
	double pReuseDistanceSquared;
	unsigned int pLastUsedCounter;
	int pMaxProbeCount;
	decPointerList pProbes;
	
	deoalWOVInvalidateEnvProbes pVisitorInvalidateEnvProbes;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create environment probe manager. */
	deoalEnvProbeList(deoalAWorld &world, double reuseDistance, int maxProbeCount);
	
	/** \brief Clean up environment probe manager. */
	~deoalEnvProbeList();
	/*@}*/
	
	
	
	/** \name Manegement */
	/*@{*/
	/** \brief Parent world. */
	inline deoalAWorld &GetWorld() const{ return pWorld; }
	
	
	
	/** \brief Probe range. */
	inline float GetRange() const{ return pRange; }
	
	/** \brief Set probe range. */
	void SetRange(float range);
	
	/** \brief Attenuation reference distance. */
	inline float GetAttenuationRefDist() const{ return pAttenuationRefDist; }
	
	/** \brief Attenuation rolloff. */
	inline float GetAttenuationRolloff() const{ return pAttenuationRolloff; }
	
	/** \brief Attenuation distance offset. */
	inline float GetAttenuationDistanceOffset() const{ return pAttenuationDistanceOffset; }
	
	/** \brief Set attenuation parameters. */
	void SetAttenuation(float refDist, float rolloff, float distanceOffset);
	
	/** \brief Layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** \brief Set layer mask. */
	void SetLayerMask(const decLayerMask &layerMask);
	
	/** \brief Ray-tracing world BVH or NULL. */
	inline deoalRTWorldBVH *GetRTWorldBVH() const{ return pRTWorldBVH; }
	
	/** \brief Set ray-tracing world bvh or NULL. */
	void SetRTWorldBVH(deoalRTWorldBVH *bvh);
	
	/** \brief Set ray tracing config to use. */
	inline const deoalATRayTracing::sConfigSoundTracing *GetRTConfig() const{ return pRTConfig; }
	
	/** \brief Set ray tracing config to use. */
	void SetRTConfig(const deoalATRayTracing::sConfigSoundTracing *config);
	
	
	
	/** \brief Probe reuse distance. */
	inline double GetReuseDistance() const{ return pReuseDistance; }
	
	/** \brief Probe reuse distance squared for faster comparison. */
	inline double GetReuseDistanceSquared() const{ return pReuseDistanceSquared; }
	
	/** \brief Maximum number of proves to keep. */
	inline int GetMaxProbeCount() const{ return pMaxProbeCount; }
	
	
	
	/**
	 * \brief Prepare processing audio.
	 * 
	 * Can remove old probes. Most of the time does nothing. Called each frame update by world.
	 */
	void PrepareProcessAudio();
	
	/**
	 * \brief Get probe for world position by tracing sound rays.
	 * 
	 * Checks first for closest existing probe inside reuse distance. If probe does not exist
	 * a new probe is created doing ray-tracing to collect the required probe data. The new
	 * probe is inserted into the world octree and returned. If a new probe is required and too
	 * many probes are already in use the probe with the oldest last-used value is recycled.
	 * 
	 * \param[in] position Position of probe. This is the sound source position.
	 * \param[in] range Range of the probe. This is the sound source range.
	 */
	deoalEnvProbe *GetProbeTraceSoundRays(const decDVector &position);
	
	/**
	 * \brief Get probe for world position by estimating room parameters only.
	 * 
	 * Checks first for closest existing probe inside reuse distance. If probe does not exist
	 * a new probe is created doing ray-tracing to collect the required probe data. The new
	 * probe is inserted into the world octree and returned. If a new probe is required and too
	 * many probes are already in use the probe with the oldest last-used value is recycled.
	 * 
	 * \param[in] position Position of probe. This is the sound source position.
	 * \param[in] range Range of the probe. This is the sound source range.
	 */
	deoalEnvProbe *GetProbeEstimateRoom(const decDVector &position);
	
	/**
	 * \brief Remove probes overlapping box in world coordinates.
	 * 
	 * Called by moving components to force probes to be recreated. Probes containing the
	 * world position are removed from the world octree.
	 */
	void InvalidateProbesInside(const decDVector &minExtend, const decDVector &maxExtend);
	
	/** \brief Remove all probes. */
	void InvalidateAllProbes();
	
	/** \brief Quick dispose. */
	void PrepareQuickDispose();
	
	
	
	/** \brief Number of valid and invalid probes.*/
	int GetProbeCount() const;
	
	/** \brief Number of valid proves. */
	int GetValidProbeCount() const;
	/*@}*/
};

#endif
