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

#ifndef _DEBASEGRAPHICSMOKEEMITTER_H_
#define _DEBASEGRAPHICSMOKEEMITTER_H_

#include "../../../dragengine_export.h"


class deSmokeEmitter;


/**
 * \brief Graphic Module Smoke Emitter Peer
 *
 * Peer for the smoke emitter resource used by graphic modules.
 */
class DE_DLL_EXPORT deBaseGraphicSmokeEmitter{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new peer. */
	deBaseGraphicSmokeEmitter();
	
	/** \brief Clean up peer. */
	virtual ~deBaseGraphicSmokeEmitter();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Volume position changed. */
	virtual void VolumePositionChanged();
	
	/** \brief Skin changed. */
	virtual void SkinChanged();
	
	/** \brief Points changed. */
	virtual void PointsChanged();
	/*@}*/
};

#endif
