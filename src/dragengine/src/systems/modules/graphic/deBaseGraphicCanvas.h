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

#ifndef _DEBASEGRAPHICCANVAS_H_
#define _DEBASEGRAPHICCANVAS_H_

/**
 * \brief Graphic module canvas peer.
 */
class deBaseGraphicCanvas{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new peer. */
	deBaseGraphicCanvas();
	
	/** \brief Cleans up peer. */
	virtual ~deBaseGraphicCanvas();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Size changed. */
	virtual void SizeChanged();
	
	/** \brief Transform changed. */
	virtual void TransformChanged();
	
	/** \brief Color transform changed. */
	virtual void ColorTransformChanged();
	
	/** \brief Visible changed. */
	virtual void VisibleChanged();
	
	/** \brief Render order changed. */
	virtual void OrderChanged();
	
	/** \brief Transparency changed. */
	virtual void TransparencyChanged();
	
	/** \brief Blend mode changed. */
	virtual void BlendModeChanged();
	
	/** \brief Content changed. */
	virtual void ContentChanged();
	/*@}*/
};

#endif
