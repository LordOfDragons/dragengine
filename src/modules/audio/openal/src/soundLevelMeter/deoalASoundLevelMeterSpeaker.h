/* 
 * Drag[en]gine OpenAL Audio Module
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

