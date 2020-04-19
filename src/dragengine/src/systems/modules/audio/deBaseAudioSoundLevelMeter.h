/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEBASEAUDIOSOUNDLEVELMETER_H_
#define _DEBASEAUDIOSOUNDLEVELMETER_H_

#include "../../../resources/sensor/deSoundLevelMeter.h"



/**
 * \brief Audio module sound level meter peer.
 */
class deBaseAudioSoundLevelMeter{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deBaseAudioSoundLevelMeter();
	
	/** \brief Clean up peer. */
	virtual ~deBaseAudioSoundLevelMeter();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Type changed. */
	virtual void TypeChanged();
	
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Orientation changed. */
	virtual void OrientationChanged();
	
	/** \brief Cone angle changed. */
	virtual void ConeAngleChanged();
	
	/** \brief Audible distance changed. */
	virtual void AudibleDistanceChanged();
	
	/** \brief Layer mask changed. */
	virtual void LayerMaskChanged();
	
	/** \brief Enabled changed. */
	virtual void EnabledChanged();
	
	/** \brief Number of audible speakers. */
	virtual int GetAudibleSpeakerCount() = 0;
	
	/** \brief Audible speaker at index. */
	virtual const deSoundLevelMeter::cAudibleSpeaker &GetAudibleSpeakerAt( int index ) = 0;
	/*@}*/
};

#endif
