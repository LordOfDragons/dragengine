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

#ifndef _DEBASEAUDIOMICROPHONE_H_
#define _DEBASEAUDIOMICROPHONE_H_

#include "../../../dragengine_export.h"

class deSpeaker;



/**
 * \brief Audio Module Microphone Peer.
 */
class DE_DLL_EXPORT deBaseAudioMicrophone{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deBaseAudioMicrophone();
	
	/** \brief Clean up peer. */
	virtual ~deBaseAudioMicrophone();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Type changed. */
	virtual void TypeChanged();
	
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Orientation changed. */
	virtual void OrientationChanged();
	
	/** \brief Velocity changed. */
	virtual void VelocityChanged();
	
	/** \brief Muted changed. */
	virtual void MutedChanged();
	
	/** \brief Volume changed. */
	virtual void VolumeChanged();
	
	/** \brief Layer mask changed. */
	virtual void LayerMaskChanged();
	
	/** \brief Speaker has been added. */
	virtual void SpeakerAdded( deSpeaker *speaker );
	
	/** \brief Speaker has been removed. */
	virtual void SpeakerRemoved( deSpeaker *speaker );
	
	/** \brief All speakers have been removed. */
	virtual void AllSpeakersRemoved();
	/*@}*/
};

#endif
