/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLRCOMPONENTWCELEMENT_H_
#define _DEOGLRCOMPONENTWCELEMENT_H_

#include "../world/deoglWorldComputeElement.h"

class deoglRComponent;


/**
 * Render component world compute element.
 */
class deoglRComponentWCElement : public deoglWorldComputeElement{
private:
	deoglRComponent &pComponent;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render component world compute element. */
	deoglRComponentWCElement( deoglRComponent &component );
	
protected:
	/** Clean up render component. */
	virtual ~deoglRComponentWCElement();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Update element data. */
	virtual void UpdateData( sDataElement &data ) const;
	
	/** Update geometries data. */
	virtual void UpdateDataGeometries( sDataElementGeometry *data ) const;
};

#endif
