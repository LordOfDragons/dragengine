/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEWCONNECTION_H_
#define _IGDEWCONNECTION_H_

#include "igdeWDebugDrawerShape.h"



/**
 * \brief Connection Wrapper.
 * 
 * Draws a connection between two points using a debug drawer shapes. The connection
 * is represented as a line with optional arrow heads on both ends. The arrows and the
 * line are each a separate shape. This way the geometry of the connection line can be
 * altered without changing the shape faces just by changing position, orientation and
 * scaling of the individual shapes.
 */
class DE_DLL_EXPORT igdeWConnection{
private:
	igdeWDebugDrawerShape pDDSConnection;
	igdeWDebugDrawerShape pDDSArrowAnchor;
	igdeWDebugDrawerShape pDDSArrowTarget;
	
	decVector pAnchorPosition;
	decVector pTargetPosition;
	decColor pColor;
	
	bool pArrowOnAnchor;
	bool pArrowOnTarget;
	float pArrowSize;
	float pArrowLength;
	float pArrowRadius;
	
	bool pVisible;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create wrapper. */
	igdeWConnection();
	
	/** \brief Clean up wrapper. */
	~igdeWConnection();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Set parent debug drawer or NULL. */
	void SetParentDebugDrawer( deDebugDrawer *debugDrawer );
	
	/** \brief Anchor position. */
	inline const decVector &GetAnchorPosition() const{ return pAnchorPosition; }
	
	/** \brief Set anchor position. */
	void SetAnchorPosition( const decVector &position );
	
	/** \brief Target position. */
	inline const decVector &GetTargetPosition() const{ return pTargetPosition; }
	
	/** \brief Set target position. */
	void SetTargetPosition( const decVector &position );
	
	/** \brief Set anchor and target position at the same time. */
	void SetPositions( const decVector &anchorPosition, const decVector &targetPosition );
	
	/** \brief Connection color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** \brief Set connection color. */
	void SetColor( const decColor &color );
	
	/** \brief Determines if an arrow head is drawn on the anchor end. */
	inline bool GetArrowOnAnchor() const{ return pArrowOnAnchor; }
	
	/** \brief Sets if an arrow head is drawn on the anchor end. */
	void SetArrowOnAnchor( bool arrowOnAnchor );
	
	/** \brief Determines if an arrow head is drawn on the target end. */
	inline bool GetArrowOnTarget() const{ return pArrowOnTarget; }
	
	/** \brief Sets if an arrow head is drawn on the target end. */
	void SetArrowOnTarget( bool arrowOnTarget );
	
	/** \brief Size of the arrow heads. */
	inline float GetArrowSize() const{ return pArrowSize; }
	
	/** \brief Set size of the arrow heads. */
	void SetArrowSize( float size );
	
	/** \brief Determines if the debug drawer shape is visible. */
	inline bool GetVisible() const{ return pVisible; }
	
	/** \brief Sets if the debug drawer shape is visible. */
	void SetVisible( bool visible );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pRebuildArrow();
	void pUpdateGeometries();
};

#endif
