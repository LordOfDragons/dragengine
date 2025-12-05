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

#ifndef _DEOALASOUNDLEVELMETERSPEAKER_H_
#define _DEOALASOUNDLEVELMETERSPEAKER_H_

#include <dragengine/deObject.h>

class deoalASoundLevelMeter;
class deoalASpeaker;
class deoalEnvProbe;
class deoalEnvProbeListener;
class deoalRayTraceHitElement;
class deoalRayTraceResult;


/**
 * \brief Sound level meter tracked speaker.
 */
class deoalASoundLevelMeterSpeaker : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoalASoundLevelMeterSpeaker> Ref;
	
	
private:
	deoalASoundLevelMeter &pSoundLevelMeter;
	deoalASpeaker *pSpeaker;
	float pVolume;
	
	deoalEnvProbe *pEnvProbe;
	deoalEnvProbeListener *pListener;
	
	float pGainLow;
	float pGainMedium;
	float pGainHigh;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create copy of sound level meter speaker. */
	deoalASoundLevelMeterSpeaker( deoalASoundLevelMeter &soundLevelMeter, deoalASpeaker *speaker );
	
protected:
	/** \brief Clean up sound level meter speaker. */
	virtual ~deoalASoundLevelMeterSpeaker();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Sound level meter. */
	inline deoalASoundLevelMeter &GetSoundLevelMeter() const{ return pSoundLevelMeter; }
	
	/** \brief Speaker. */
	inline deoalASpeaker *GetSpeaker() const{ return pSpeaker; }
	
	/** \brief Volume. */
	inline float GetVolume() const{ return pVolume; }
	
	/** \brief Set volume. */
	void SetVolume( float volume );
	
	/** \brief Env probe of \em NULL if not present. */
	inline deoalEnvProbe *GetEnvProbe() const{ return pEnvProbe; }
	
	/** \brief Drop env probe from world octree if present. */
	void EnvProbeDropOctreeNode();
	
	/**
	 * \brief Speaker position changed.
	 * \warning For use by deoalASpeaker only.
	 */
	void SpeakerPositionChanged();
	
	/**
	 * \brief Speaker layer mask changed.
	 * \warning For use by deoalASpeaker only.
	 */
	void SpeakerLayerMaskChanged();
	
	/**
	 * \brief Speaker range changed.
	 * \warning For use by deoalASpeaker only.
	 */
	void SpeakerRangeChanged();
	
	/**
	 * \brief Speaker attenuation changed.
	 * \warning For use by deoalASpeaker only.
	 */
	void SpeakerAttenuationChanged();
	
	/** \brief Update volume. */
	void Listen();
	/*@}*/
	
	
	
private:
	void pListenDirect();
	void pListenReflected();
	const deoalRayTraceHitElement *pNextHitElement( const deoalRayTraceResult &rtresult,
		int &index, bool forwardFacing ) const;
};

#endif

