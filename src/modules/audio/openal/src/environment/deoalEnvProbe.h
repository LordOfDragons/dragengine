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

#ifndef _DEOALENVPROBE_H_
#define _DEOALENVPROBE_H_

#include "raytrace/deoalSoundRayList.h"
#include "raytrace/deoalSoundRayInteractionList.h"
#include "../audiothread/deoalATRayTracing.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/common/collection/decPointerList.h>

class deoalAMicrophone;
class deoalAudioThread;
class deoalASoundLevelMeter;
class deoalAWorld;
class deoalRTWorldBVH;
class deoalEnvProbeLevel;
class deoalEnvProbeListener;
class deoalEnvProbeListenerCached;
class deoalRayTraceConfig;
class deoalWorldOctree;


/**
 * \brief Environment probe.
 * 
 * Stores the result of an environment probe. Probes test for collision in all directions
 * around a given position in world coordinates.
 * 
 * The probe is inserted into the world octree as "Environment Probe" with an AABB around
 * probe position and reuse range. Sound sources test AABB with reuse range against the octree
 * to find probe inside resuse distance. If no such probe exists the sound source has to add a
 * new probe. If more than one probe matches the probe with the closest distance is chosen.
 * The deoalEnvProbeList takes care of all this.
 * 
 * The number of rays cast is limited to a fixed amount.
 * 
 * The length of rays cast is set to the range of the source source.
 * 
 * The AABB of the probe is the AABB around all hit points. This is used to insert the probe
 * into the world octree as "Probe Invalidation" entry. If components change they test against
 * the octree removing all overlappiong probe invalidation entries. This ensures probes are
 * recreated if a component changes. Games can optimize this by making component textures
 * not affecting sound.
 * 
 * The sound parameters of each hit element are used to calculate the environment parameters
 * used for a sound source located at this position in space.
 * 
 * Probes have a last-used value assigned. This indicates the last time the probe has been used.
 * If the number of probes gets larger probes not used since a long time can be removed to avoid
 * the number of probes to get too high. Last-used is simply a frame index. Each frame update
 * in the game world increments the counter by one. If the probe is used the current frame
 * index is assigned to the probe as last-used value.
 */
class deoalEnvProbe{
private:
	deoalAudioThread &pAudioThread;
	
	decDVector pPosition;
	float pRange;
	float pAttenuationRefDist;
	float pAttenuationRolloff;
	float pAttenuationDistanceOffset;
	decLayerMask pLayerMask;
	const deoalATRayTracing::sConfigSoundTracing *pRTConfig;
	
	decDVector pMinExtend;
	decDVector pMaxExtend;
	deoalSoundRayList pSoundRayList;
	deoalSoundRayInteractionList pSRIList;
	int pRayCount;
	float pRayOpeningAngle;
	
	float pReverberationTimeMedium;
	float pReverberationTimeLow;
	float pReverberationTimeHigh;
	float pEchoDelay;
	
	decDVector pRoomCenter;
	float pRoomVolume;
	float pRoomSurface;
	float pRoomSabineLow;
	float pRoomSabineMedium;
	float pRoomSabineHigh;
	float pMeanFreePath;
	float pSepTimeFirstLateRefl;
	float pAvgAbsorptionLow;
	float pAvgAbsorptionMedium;
	float pAvgAbsorptionHigh;
	
	decPointerList pLevels;
	
	decPointerList pListeners;
	float pListenerRange;
	unsigned short pListenerTracking;
	
	bool pEstimated;
	
	unsigned int pLastUsed;
	
	deoalWorldOctree *pOctreeNode;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create environment probe. */
	deoalEnvProbe( deoalAudioThread &audioThread );
	
	/** \brief Clean up environment probe. */
	~deoalEnvProbe();
	/*@}*/
	
	
	
	/** \name Manegement */
	/*@{*/
	/** \brief Probe position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decDVector &position );
	
	/** \brief Probe range. */
	inline float GetRange() const{ return pRange; }
	
	/** \brief Set probe range. */
	void SetRange( float range );
	
	/** \brief Attenuation reference distance. */
	inline float GetAttenuationRefDist() const{ return pAttenuationRefDist; }
	
	/** \brief Attenuation rolloff. */
	inline float GetAttenuationRolloff() const{ return pAttenuationRolloff; }
	
	/** \brief Attenuation distance offset. */
	inline float GetAttenuationDistanceOffset() const{ return pAttenuationDistanceOffset; }
	
	/** \brief Set attenuation parameters. */
	void SetAttenuation( float refDist, float rolloff, float distanceOffset );
	
	/** \brief Layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** \brief Set layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	/** \brief Ray tracing configuration to use. */
	inline const deoalATRayTracing::sConfigSoundTracing *GetRTConfig() const{ return pRTConfig; }
	
	/** \brief Set ray tracing configuration to use. */
	void SetRTConfig( const deoalATRayTracing::sConfigSoundTracing *config );
	
	
	
	/** \brief Minimum extend of box around all hit points. */
	inline const decDVector &GetMinExtend() const{ return pMinExtend; }
	
	/** \brief Maximum extend of box around all hit points. */
	inline const decDVector &GetMaxExtend() const{ return pMaxExtend; }
	
	/** \brief Set extends. */
	void SetExtends( const decDVector &minExtend, const decDVector &maxExtend );
	
	/** \brief Traced sound ray list. */
	inline deoalSoundRayList &GetSoundRayList(){ return pSoundRayList; }
	inline const deoalSoundRayList &GetSoundRayList() const{ return pSoundRayList; }
	
	/** \brief First reflection sound ray interaction list. */
	inline deoalSoundRayInteractionList &GetSRIList(){ return pSRIList; }
	inline const deoalSoundRayInteractionList &GetSRIList() const{ return pSRIList; }
	
	/** \brief Number of rays used to calculate probe. */
	inline int GetRayCount() const{ return pRayCount; }
	
	/** \brief Set number of rays used to calculate probe. */
	void SetRayCount( int rayCount );
	
	/** \brief Ray beam opening angle used for calculating probe. */
	inline float GetRayOpeningAngle() const{ return pRayOpeningAngle; }
	
	/** \brief Set ray beam opening angle used for calculating probe. */
	void SetRayOpeningAngle( float angle );
	
	
	
	/** \brief Reverberation time in seconds for medium frequencies. */
	inline float GetReverberationTimeMedium() const{ return pReverberationTimeMedium; }
	
	/** \brief Reverberation time in seconds for low frequencies. */
	inline float GetReverberationTimeLow() const{ return pReverberationTimeLow; }
	
	/** \brief Reverberation time in seconds for high frequencies. */
	inline float GetReverberationTimeHigh() const{ return pReverberationTimeHigh; }
	
	/** \brief Set reverberation. */
	void SetReverberationTime( float timeLow, float timeMedium, float timeHigh );
	
	/** \brief Echo delay. */
	inline float GetEchoDelay() const{ return pEchoDelay; }
	
	/** \brief Set echo. */
	void SetEcho( float delay );
	
	
	
	/** \brief Room center. */
	inline const decDVector &GetRoomCenter() const{ return pRoomCenter; }
	
	/** \brief Set room center. */
	void SetRoomCenter( const decDVector &center );
	
	/** \brief Room volume. */
	inline float GetRoomVolume() const{ return pRoomVolume; }
	
	/** \brief Room surface. */
	inline float GetRoomSurface() const{ return pRoomSurface; }
	
	/** \brief Room sabine for low frequencies. */
	inline float GetRoomSabineLow() const{ return pRoomSabineLow; }
	
	/** \brief Room sabine for medium frequencies. */
	inline float GetRoomSabineMedium() const{ return pRoomSabineMedium; }
	
	/** \brief Room sabine for high frequencies. */
	inline float GetRoomSabineHigh() const{ return pRoomSabineHigh; }
	
	/** \brief Set room indirect sound parameters. */
	void SetRoom( float volume, float surface, float sabineLow, float sabineMedium, float sabineHigh );
	
	/** \brief Average absorption for low frequencies. */
	inline float GetAvgAbsorptionLow() const{ return pAvgAbsorptionLow; }
	
	/** \brief Average absorption for medium frequencies. */
	inline float GetAvgAbsorptionMedium() const{ return pAvgAbsorptionMedium; }
	
	/** \brief Average absorption for high frequencies. */
	inline float GetAvgAbsorptionHigh() const{ return pAvgAbsorptionHigh; }
	
	/** \brief Set average absorption. */
	void SetAvgAbsorption( float low, float medium, float high );
	
	/** \brief Mean free path. */
	inline float GetMeanFreePath() const{ return pMeanFreePath; }
	
	/** \brief Set mean free path. */
	void SetMeanFreePath( float meanFreePath );
	
	/** \brief Time in seconds separating first from late reflections. */
	inline float GetSepTimeFirstLateRefl() const{ return pSepTimeFirstLateRefl; }
	
	/** \brief Set time in seconds separating first from late reflections. */
	void SetSepTimeFirstLateRefl( float time );
	
	
	
	/** \brief Calculate gain for distance using attenuation. */
	float AttenuatedGain( float distance ) const;
	
	
	
	/**
	 * \brief Probe data is based on estimation only.
	 * 
	 * Probe data has been calculate using EstimateRoomParameters(). No sound ray list is
	 * present. To get a full tracing calling TraceSoundRays() is required.
	 */
	inline bool GetEstimated() const{ return pEstimated; }
	
	/** \brief Invalidate. */
	void Invalidate();
	
	
	
	/**
	 * \brief Calculate probe data using room parameter estimation.
	 * 
	 * Uses the set position to do ray-tracing in the world. After the calculation is
	 * finished the probe boundaries and room parameters have been updated. No sound
	 * ray list has been build. For this a call to TraceSoundRays() is required.
	 * The probe needs to be re-inserted into the world octree to work properly.
	 * The caller is responsible to do this.
	 * 
	 * \warning This method is for use by deoalEnvProbeList only.
	 */
	void EstimateRoomParameters( deoalAWorld &world, const deoalRayTraceConfig &probeConfig );
	
	/**
	 * \brief Calculate probe data using tracing sound rays.
	 * 
	 * Uses the set position to do ray-tracing in the world. After the calculation is
	 * finished the probe boundaries, room parameters and sound ray list have been updated.
	 * The probe needs to be re-inserted into the world octree to work properly.
	 * The caller is responsible to do this.
	 * 
	 * \warning This method is for use by deoalEnvProbeList only.
	 */
	void TraceSoundRays( deoalAWorld &world, deoalRTWorldBVH *rtWorldBVH,
		const deoalRayTraceConfig &probeConfig );
	
	/**
	 * \brief Calculate listener parameters.
	 * 
	 * Uses cached previous calculated listeners if present to update the listener
	 * object with the appropriate data.
	 * 
	 * \throws deInvalidParam Probe has been only estimated. No sound ray list present.
	 */
	void CalcListener( deoalEnvProbeListener &listener, deoalAWorld &world,
		const decDVector &position, deoalAMicrophone *microphone );
	
	void CalcListener( deoalEnvProbeListener &listener, deoalAWorld &world,
		const decDVector &position, deoalASoundLevelMeter *soundLevelMeter );
	
	
	
	/** \brief Last used. */
	inline unsigned int GetLastUsed() const{ return pLastUsed; }
	
	/** \brief Set last used. */
	void SetLastUsed( unsigned int lastUsed );
	
	
	
	/** \brief World octree node or NULL. */
	inline deoalWorldOctree *GetOctreeNode() const{ return pOctreeNode; }
	
	/** \brief Set world octree node or NULL. */
	void SetOctreeNode( deoalWorldOctree *node );
	/*@}*/
	
	
	
	
private:
	void pRemoveAllLevels();
	void pRemoveAllListeners();
	
	void pCalcListener( deoalEnvProbeListener &listener, deoalAWorld &world,
		const decDVector &position, deoalAMicrophone *microphone,
		deoalASoundLevelMeter *soundLevelMeter );
};

#endif

