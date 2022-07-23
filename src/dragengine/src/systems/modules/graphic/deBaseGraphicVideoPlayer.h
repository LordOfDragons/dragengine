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

#ifndef _DEBASEGRAPHICVIDEOPLAYER_H_
#define _DEBASEGRAPHICVIDEOPLAYER_H_

#include "../../../dragengine_export.h"


/**
 * \brief Graphic Module Video Player Peer.
 */
class DE_DLL_EXPORT deBaseGraphicVideoPlayer{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new peer. */
	deBaseGraphicVideoPlayer();
	
	/** \brief Clean up peer. */
	virtual ~deBaseGraphicVideoPlayer();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Sound source changed. */
	virtual void SourceChanged();
	
	/** \brief Looping changed. */
	virtual void LoopingChanged();
	
	/** \brief Play range changed. */
	virtual void PlayRangeChanged();
	
	/** \brief Play speed changed. */
	virtual void PlaySpeedChanged();
	
	/**
	 * \brief Play position changed.
	 * \param[in] seeking Changed due to seeking or by deVideoPlayer::Update().
	 */
	virtual void PlayPositionChanged( bool seeking );
	
	/** \brief Play state changed. */
	virtual void PlayStateChanged();
	/*@}*/
};

#endif
