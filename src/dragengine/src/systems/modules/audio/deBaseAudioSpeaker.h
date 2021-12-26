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

#ifndef _DEBASEAUDIOSPEAKER_H_
#define _DEBASEAUDIOSPEAKER_H_

#include "../../../dragengine_export.h"



/**
 * \brief Audio Module Speaker Peer.
 */
class DE_DLL_EXPORT deBaseAudioSpeaker{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deBaseAudioSpeaker();
	
	/** \brief Clean up peer. */
	virtual ~deBaseAudioSpeaker();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Type changed. */
	virtual void TypeChanged();
	
	/** \brief Sound source changed. */
	virtual void SourceChanged();
	
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Orientation changed. */
	virtual void OrientationChanged();
	
	/** \brief Velocity changed. */
	virtual void VelocityChanged();
	
	/** \brief Muted changed. */
	virtual void MutedChanged();
	
	/** \brief Looping changed. */
	virtual void LoopingChanged();
	
	/** \brief Play position changed. */
	virtual void PlayPositionChanged();
	
	/** \brief Play speed changed. */
	virtual void PlaySpeedChanged();
	
	/** \brief Volume changed. */
	virtual void VolumeChanged();
	
	/** \brief Range changed. */
	virtual void RangeChanged();
	
	/** \brief Roll off factor changed. */
	virtual void RollOffChanged();
	
	/** \brief Shape changed. */
	virtual void ShapeChanged();
	
	/** \brief Layer mask changed. */
	virtual void LayerMaskChanged();
	
	/** \brief Play state changed. */
	virtual void PlayStateChanged();
	/*@}*/
};

#endif
