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

#ifndef _DEDEBUGDRAWERSHAPE_H_
#define _DEDEBUGDRAWERSHAPE_H_

#include "../../common/math/decMath.h"
#include "../../common/shape/decShapeList.h"
#include "../../common/collection/decPointerList.h"

class deDebugDrawerShapeFace;


/**
 * \brief Debug drawer shape.
 * 
 * Describes the a shape to be drawn. There are a couple of parameters
 * that can be set to define the appearance. The edge and fill color flag
 * determine the color and transparency of the edges and the faces of
 * the shape. By default the edge is set to white fully opaque and the
 * fill is set to white fully transparent. Both shapes and faces can be
 * used at the same time. If none of them are present nothing is drawn.
 */
class deDebugDrawerShape{
private:
	decVector pPosition;
	decQuaternion pOrientation;
	decVector pScale;
	
	decShapeList pShapeList;
	
	decPointerList pFaces;
	
	decColor pEdgeColor;
	decColor pFillColor;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create debug drawer shape. */
	deDebugDrawerShape();
	
	/** \brief Clean up debug drawer shape. */
	~deDebugDrawerShape();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decVector &position );
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation( const decQuaternion &orientation );
	
	/** \brief Scale. */
	inline const decVector &GetScale() const{ return pScale; }
	
	/** \brief Set scale. */
	void SetScale( const decVector &scale );
	
	
	
	/** \brief Edge color. */
	inline const decColor &GetEdgeColor() const{ return pEdgeColor; }
	
	/** \brief Set edge color. */
	void SetEdgeColor( const decColor &color );
	
	/** \brief Fill color. */
	inline const decColor &GetFillColor() const{ return pFillColor; }
	
	/** \brief Set fill color. */
	void SetFillColor( const decColor &color );
	
	/** \brief Shape list. */
	inline decShapeList &GetShapeList(){ return pShapeList; }
	inline const decShapeList &GetShapeList() const{ return pShapeList; }
	/*@}*/
	
	
	
	/** \name Volume Faces */
	/*@{*/
	/** \brief Number of volume faces. */
	int GetFaceCount() const;
	
	/** \brief Face at index. */
	deDebugDrawerShapeFace *GetFaceAt( int index ) const;
	
	/** \brief Add face. */
	void AddFace( deDebugDrawerShapeFace *face );
	
	/** \brief Remove all faces. */
	void RemoveAllFaces();
	/*@}*/
};

#endif
