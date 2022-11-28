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

#ifndef _DEBASEAUDIOWORLD_H_
#define _DEBASEAUDIOWORLD_H_

#include "../../../dragengine_export.h"

class deSpeaker;
class deMicrophone;
class deComponent;
class deSoundLevelMeter;



/**
 * \brief Audio module world peer.
 */
class DE_DLL_EXPORT deBaseAudioWorld{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deBaseAudioWorld();
	
	/** \brief Clean up peer. */
	virtual ~deBaseAudioWorld();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Update world. */
	virtual void Update( float elapsed );
	
	/** \brief Size changed. */
	virtual void SizeChanged();
	
	/**
	 * \brief Audio changed.
	 * \version 1.16
	 */
	virtual void AudioChanged();
	
	
	
	/** \brief Speaker has been added. */
	virtual void SpeakerAdded( deSpeaker *speaker );
	
	/** \brief Speaker has been removed. */
	virtual void SpeakerRemoved( deSpeaker *speaker );
	
	/** \brief All speakers have been removed. */
	virtual void AllSpeakersRemoved();
	
	
	
	/** \brief Microphone has been added. */
	virtual void MicrophoneAdded( deMicrophone *microphone );
	
	/** \brief Microphone has been removed. */
	virtual void MicrophoneRemoved( deMicrophone *microphone );
	
	/** \brief All microphones have been removed. */
	virtual void AllMicrophonesRemoved();
	
	
	
	/** \brief Component has been added. */
	virtual void ComponentAdded( deComponent *component );
	
	/** \brief Component has been removed. */
	virtual void ComponentRemoved( deComponent *component );
	
	/** \brief All components have been removed. */
	virtual void AllComponentsRemoved();
	
	
	
	/** \brief Sound level meter has been added. */
	virtual void SoundLevelMeterAdded( deSoundLevelMeter *soundLevelMeter );
	
	/** \brief Sound level meter has been removed. */
	virtual void SoundLevelMeterRemoved( deSoundLevelMeter *soundLevelMeter );
	
	/** \brief All sound level meters have been removed. */
	virtual void AllSoundLevelMetersRemoved();
	/*@}*/
};

#endif
