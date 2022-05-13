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

#ifndef _DEBASEGRAPHICENVMAPPROBE_H_
#define _DEBASEGRAPHICENVMAPPROBE_H_

#include "../../../common/math/decMath.h"


/**
 * \brief Graphic Module Environment Map Probe Peer.
 */
class DE_DLL_EXPORT deBaseGraphicEnvMapProbe{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new peer. */
	deBaseGraphicEnvMapProbe();
	
	/** \brief Clean up peer. */
	virtual ~deBaseGraphicEnvMapProbe();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Orientation changed. */
	virtual void OrientationChanged();
	
	/** \brief Scaling changed. */
	virtual void ScalingChanged();
	
	/** \brief Shape list influence changed. */
	virtual void ShapeListInfluenceChanged();
	
	/** \brief Shape reflection changed. */
	virtual void ShapeReflectionChanged();
	
	/** \brief Influence border size changed. */
	virtual void InfluenceBorderSizeChanged();
	
	/** \brief Influence priority changed. */
	virtual void InfluencePriorityChanged();
	
	/** \brief Image changed. */
	virtual void ImageChanged();
	
	/** \brief Layer mask changed. */
	virtual void LayerMaskChanged();
	/*@}*/
};

#endif
