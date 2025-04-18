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

#ifndef _DEOALENVIRONMENT_H_
#define _DEOALENVIRONMENT_H_

#include "deoalEnvProbeListenerSmooth.h"
#include "raytrace/deoalImpulseResponse.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decLayerMask.h>

class deoalAudioThread;
class deoalAWorld;
class deoalAMicrophone;
class deoalRayTraceHitElement;
class deoalRayTraceResult;
class deoalEnergyHistogram;
class deoalEnvProbe;
class deoalEnvironmentDebug;

class deDebugBlockInfo;
class deDebugDrawer;



/**
 * Environment tracking for speakers.
 * 
 * Keeps track of the environment a speaker is located in. Stores the effect parameters
 * required to be assigned to a speaker to give the appropriate impression.
 * 
 * Instances of this object are typically stored in active speakers and regularily
 * updated to reflect changes in the environment.
 */
class deoalEnvironment{
private:
	deoalAudioThread &pAudioThread;
	
	deoalAWorld *pWorld;
	decDVector pPosition;
	float pRange;
	float pRangeSquared;
	float pAttenuationRefDist;
	float pAttenuationRolloff;
	float pAttenuationDistanceOffset;
	decLayerMask pLayerMask;
	bool pValid;
	
	float pGainLow;
	float pGainMedium;
	float pGainHigh;
	
	float pBandPassGain;
	float pBandPassGainLF;
	float pBandPassGainHF;
	
	float pReverbGain;
	float pReverbGainLF;
	float pReverbGainHF;
	float pReverbDecayTime;
	float pReverbDecayHFRatio;
	float pReverbDecayLFRatio;
	float pReverbReflectionGain;
	float pReverbReflectionDelay;
	decVector pReverbReflectionPan;
	float pReverbLateReverbGain;
	float pReverbLateReverbDelay;
	decVector pReverbLateReverbPan;
	float pReverbEchoTime;
	
	float pEffectKeepAliveTimeout;
	
	deoalEnvProbeListenerSmooth pListenerSmooth;
	bool pResetListenerSmooth;
	
	deoalEnvProbe *pEnvProbe;
	
	float pCompareReverbGain;
	float pCompareReverbGainLF;
	float pCompareReverbGainHF;
	float pCompareReverbDecayTime;
	float pCompareReverbDecayHFRatio;
	float pCompareReverbDecayLFRatio;
	float pCompareReverbReflectionGain;
	float pCompareReverbReflectionDelay;
	decVector pCompareReverbReflectionPan;
	float pCompareReverbLateReverbGain;
	float pCompareReverbLateReverbDelay;
	decVector pCompareReverbLateReverbPan;
	float pCompareReverbEchoTime;
	
	deoalEnvironmentDebug *pDebug;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create environment tracker. */
	deoalEnvironment( deoalAudioThread &audioThread );
	
	/** Clean up environment. */
	~deoalEnvironment();
	/*@}*/
	
	
	
	/** \name Manegement */
	/*@{*/
	/** Audio thread. */
	inline deoalAudioThread &GetAudioThread() const{ return pAudioThread; }
	
	/** World or NULL. */
	inline deoalAWorld *GetWorld() const{ return pWorld; }
	
	/** Set world or NULL. */
	void SetWorld( deoalAWorld *world );
	
	/** Position of speaker. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** Set position of speaker. */
	void SetPosition( const decDVector &position );
	
	/** Range of speaker. */
	inline float GetRange() const{ return pRange; }
	
	/** Range squared of speaker. */
	inline float GetRangeSquared() const{ return pRangeSquared; }
	
	/** Set range of speaker. */
	void SetRange( float range );
	
	/** Attenuation reference distance. */
	inline float GetAttenuationRefDist() const{ return pAttenuationRefDist; }
	
	/** Attenuation rolloff. */
	inline float GetAttenuationRolloff() const{ return pAttenuationRolloff; }
	
	/** Attenuation distance offset. */
	inline float GetAttenuationDistanceOffset() const{ return pAttenuationDistanceOffset; }
	
	/** Set attenuation parameters. */
	void SetAttenuation( float refDist, float rolloff, float distanceOffset );
	
	/** Layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** Set layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	/** Environment data are valid. */
	inline bool GetValid() const{ return pValid; }
	
	/** Prepare quick dispose. */
	void PrepareQuickDispose();
	
	
	
	/** Low frequency gain. */
	inline float GetGainLow() const{ return pGainLow; }
	
	/** Medium frequency gain. */
	inline float GetGainMedium() const{ return pGainMedium; }
	
	/** High frequency gain. */
	inline float GetGainHigh() const{ return pGainHigh; }
	
	
	
	/** Band pass gain value. */
	inline float GetBandPassGain() const{ return pBandPassGain; }
	
	/** Band pass low frequency gain value. */
	inline float GetBandPassGainLF() const{ return pBandPassGainLF; }
	
	/** Band pass high frequency gain value. */
	inline float GetBandPassGainHF() const{ return pBandPassGainHF; }
	
	
	
	/** Reverb effect gain value. */
	inline float GetReverbGain() const{ return pReverbGain; }
	
	/** Reverb effect low frequency gain value. */
	inline float GetReverbGainLF() const{ return pReverbGainLF; }
	
	/** Reverb effect high frequency gain value. */
	inline float GetReverbGainHF() const{ return pReverbGainHF; }
	
	/** Reverb effect decay time value. */
	inline float GetReverbDecayTime() const{ return pReverbDecayTime; }
	
	/** Reverb effect decay lf ratio value. */
	inline float GetReverbDecayLFRatio() const{ return pReverbDecayLFRatio; }
	
	/** Reverb effect decay hf ratio value. */
	inline float GetReverbDecayHFRatio() const{ return pReverbDecayHFRatio; }
	
	/** Reverb effect reflection gain value. */
	inline float GetReverbReflectionGain() const{ return pReverbReflectionGain; }
	
	/** Reverb effect reflection delay value. */
	inline float GetReverbReflectionDelay() const{ return pReverbReflectionDelay; }
	
	/** Reverb effect reflection pan direction. */
	inline const decVector &GetReverbReflectionPan() const{ return pReverbReflectionPan; }
	
	/** Reverb effect late reverb gain value. */
	inline float GetReverbLateReverbGain() const{ return pReverbLateReverbGain; }
	
	/** Reverb effect late reverb delay value. */
	inline float GetReverbLateReverbDelay() const{ return pReverbLateReverbDelay; }
	
	/** Reverb effect late reverb pan direction. */
	inline const decVector &GetReverbLateReverbPan() const{ return pReverbLateReverbPan; }
	
	/** Reverbe effect Echo time. */
	inline float GetReverbEchoTime() const{ return pReverbEchoTime; }
	
	
	
	/** Effect keep-alive timeout. */
	inline float GetEffectKeepAliveTimeout() const{ return pEffectKeepAliveTimeout; }
	
	
	
	/** Distance between this environment and another one. */
	float Distance( const deoalEnvironment &env, bool withPan ) const;
	
	
	
	/** Update environment. */
	void Update();
	
	
	
	/** Debug print. */
	void DebugPrint();
	
	/**
	 * Update debug information.
	 * \warning Called during synchronization time from main thread.
	 */
	void DebugUpdateInfo( deDebugBlockInfo &debugInfo );
	void DebugUpdateDirect( deDebugDrawer &debugDrawer, const deoalAMicrophone &microphone );
	void DebugSoundRays( deDebugDrawer &debugDrawer );
	/*@}*/
	
	
	
private:
	void pDirectPath( const deoalAMicrophone &microphone, const decDVector &micPos );
	void pEnvReflection( deoalAMicrophone &microphone, const decDVector &micPos,
		const decQuaternion &micOrient );
	void pSetSilent();
	void pCalcEffectParameters();
	void pCalcEffectKeepAliveTimeout();
	void pCalcCompareParameters();
	decVector pCalcComparePan( const decVector &pan ) const;
};

#endif
