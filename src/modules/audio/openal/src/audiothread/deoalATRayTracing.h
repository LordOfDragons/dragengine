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

#ifndef _DEOALATRAYTRACING_H_
#define _DEOALATRAYTRACING_H_

#include <dragengine/common/collection/decPointerList.h>

class deoalAudioThread;
class deoalRayTraceConfig;



/**
 * \brief Audio thread ray tracing.
 */
class deoalATRayTracing{
public:
	/** \brief Sound tracing configuration. */
	struct sConfigSoundTracing{
		float addRayMinLength;
		int maxBounceCount;
		int maxTransmitCount;
		float thresholdReflect;
		float thresholdTransmit;
		const deoalRayTraceConfig *rtConfig;
		bool inverseRayTracing;
	};
	
	
	
private:
	deoalAudioThread &pAudioThread;
	
	deoalRayTraceConfig *pConfigTraceSoundRays;
	deoalRayTraceConfig *pConfigRoomEstimate;
	deoalRayTraceConfig *pConfigTraceSoundRaysSensor;
// 	decPointerList pProbeConfigs;
	
	sConfigSoundTracing pConfigSoundTracingMicrophone;
	sConfigSoundTracing pConfigSoundTracingMetering;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render thread ray tracing. */
	deoalATRayTracing( deoalAudioThread &audioThread );
	
	/** \brief Clean up render thread renderers. */
	~deoalATRayTracing();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Configuration for tracing sound rays. */
	inline deoalRayTraceConfig *GetConfigTraceSoundRays() const{ return pConfigTraceSoundRays; }
	
	/** \brief Configuration for estimating rooms. */
	inline deoalRayTraceConfig *GetConfigEstimateRoom() const{ return pConfigRoomEstimate; }
	
	/** \brief Configuration for tracing sound rays. */
	inline deoalRayTraceConfig *GetConfigTraceSoundRaysSensor() const{ return pConfigTraceSoundRaysSensor; }
	
	/** \brief Sound tracing configuration for microphone usage. */
	inline const sConfigSoundTracing &GetConfigSoundTracingMicrophone() const{ return pConfigSoundTracingMicrophone; }
	
	/** \brief Sound tracing configuration for metering usage. */
	inline const sConfigSoundTracing &GetConfigSoundTracingMetering() const{ return pConfigSoundTracingMetering; }
	
	
	
	/** \brief Model configuration changed. */
	void ConfigurationChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pCreateRayTraceConfigs();
};

#endif
