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

#ifndef _DEBASEGRAPHICDEBUGDRAWER_H_
#define _DEBASEGRAPHICDEBUGDRAWER_H_

class deDebugDrawerShape;


/**
 * \brief Graphic module debug drawer peer.
 */
class deBaseGraphicDebugDrawer{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new peer. */
	deBaseGraphicDebugDrawer();
	
	/** \brief Clean up peer. */
	virtual ~deBaseGraphicDebugDrawer();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Orientation changed. */
	virtual void OrientationChanged();
	
	/** \brief Scaling changed. */
	virtual void ScalingChanged();
	
	/** \brief Visibile changed. */
	virtual void VisibleChanged();
	
	/** \brief X-Ray changed. */
	virtual void XRayChanged();
	
	/** \brief Shape visible changed. */
	virtual void ShapeVisibleChanged();
	
	/** \brief Shape edge or fill colors changed. */
	virtual void ShapeColorChanged();
	
	/** \brief Shape position, orientation or scaling changed. */
	virtual void ShapeGeometryChanged();
	
	/** \brief Shape shapes or faces changed. */
	virtual void ShapeContentChanged();
	
	/** \brief Shapes have been added or removed. */
	virtual void ShapeLayoutChanged();
	/*@}*/
};
#endif
