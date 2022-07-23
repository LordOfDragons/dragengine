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

#ifndef _DEBASEGRAPHICLIGHT_H_
#define _DEBASEGRAPHICLIGHT_H_

#include "../../../dragengine_export.h"


/**
 * \brief Graphic module light peer.
 */
class DE_DLL_EXPORT deBaseGraphicLight{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deBaseGraphicLight();
	
	/** \brief Clean up peer. */
	virtual ~deBaseGraphicLight();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Light type changed. */
	virtual void TypeChanged();
	
	/** \brief Geomtry parameter changed. */
	virtual void GeometryParameterChanged();
	
	/** \brief Attenuation changed. */
	virtual void AttenuationChanged();
	
	/**
	 * \brief Light parameter changed.
	 * 
	 * - Color
	 * - Intensity
	 * - Ambient Ratio
	 * - Spot Smoothness
	 * - Spot Exponent
	 */
	virtual void LightParameterChanged();
	
	/** \brief Shadow parameter changed. */
	virtual void ShadowParameterChanged();
	
	/** \brief Light shape changed. */
	virtual void ShapeChanged();
	
	/** \brief Layer mask changed. */
	virtual void LayerMaskChanged();
	
	/** \brief Shadow layer mask changed. */
	virtual void ShadowLayerMaskChanged();
	
	/** \brief Shadow ignore components changed. */
	virtual void ShadowIgnoreComponentsChanged();
	
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Orientation changed. */
	virtual void OrientationChanged();
	
	/** \brief Skin, canvas view or dynamic skin changed. */
	virtual void SourceChanged();
	
	/** \brief Transform changed. */
	virtual void TransformChanged();
	
	/** \brief Activated changed. */
	virtual void ActivatedChanged();
	
	/** \brief Hint changed. */
	virtual void HintChanged();
	/*@}*/
};

#endif
