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

#ifndef _DEBASEGRAPHICCAMERA_H_
#define _DEBASEGRAPHICCAMERA_H_

class deEffect;


/**
 * \brief Graphic Module Camera Peer.
 *
 */
class deBaseGraphicCamera{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new peer. */
	deBaseGraphicCamera();
	
	/** \brief Clean up peer. */
	virtual ~deBaseGraphicCamera();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Orientation changed. */
	virtual void OrientationChanged();
	
	/** \brief Camera parameter changed. */
	virtual void ParameterChanged();
	
	/** \brief Adaption parameter changed. */
	virtual void AdaptionChanged();
	
	/** \brief Layer mask changed. */
	virtual void LayerMaskChanged();
	
	/** \brief Effect has been added. */
	virtual void EffectAdded( int index, deEffect *effect );
	
	/** \brief Effect has been removed. */
	virtual void EffectRemoved( int index, deEffect *effect );
	
	/** \brief All effects have been removed. */
	virtual void AllEffectsRemoved();
	/*@}*/
};

#endif
