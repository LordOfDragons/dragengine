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

#ifndef _DESKINPROPERTYNODESHAPE_H_
#define _DESKINPROPERTYNODESHAPE_H_

#include "deSkinPropertyNode.h"


/**
 * \brief Skin property shape node for constructed texture property.
 */
class deSkinPropertyNodeShape : public deSkinPropertyNode{
public:
	/** \brief Shape types. */
	enum eShapeTypes{
		/** \brief Rectangle/box. */
		estRectangle,
		
		/** \brief Ellipse/Ellipsoid. */
		estEllipse
	};
	
	
	
private:
	eShapeTypes pShapeType;
	decColor pFillColor;
	decColor pLineColor;
	float pThickness;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create constructed property node. */
	deSkinPropertyNodeShape();
	
	/** \brief Clean up constructed property node. */
	virtual ~deSkinPropertyNodeShape();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Shape type. */
	inline eShapeTypes GetShapeType() const{ return pShapeType; }
	
	/** \brief Set shape type. */
	void SetShapeType( eShapeTypes type );
	
	/** \brief Fill color. */
	inline const decColor &GetFillColor() const{ return pFillColor; }
	
	/** \brief Set fill color. */
	void SetFillColor( const decColor &color );
	
	/** \brief Line color. */
	inline const decColor &GetLineColor() const{ return pLineColor; }
	
	/** \brief Set line color. */
	void SetLineColor( const decColor &color );
	
	/** \brief Line thickness in pixels. */
	inline float GetThickness() const{ return pThickness; }
	
	/** \brief Set thicknss in pixels. */
	void SetThickness( float thickness );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits node. */
	virtual void Visit( deSkinPropertyNodeVisitor &visitor );
	/*@}*/
};

#endif
