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

#ifndef _IGDEPROPERTYSTRINGENCODESHAPELIST_H_
#define _IGDEPROPERTYSTRINGENCODESHAPELIST_H_

#include <dragengine/common/shape/decShapeVisitor.h>

class decString;



/**
 * \brief Encode Property String Shape List.
 * 
 * Encodes a shape list as a propert string. The following format is used:<br/>
 *    "shape1 shape2 ... shapeN"<br/>
 * with shapeN format:<br/>
 *    type:parameter1:parameter2:...:parameterN<br/>
 * with parameterN format:<br/>
 *    name,value1,value2,...,valueN<br/>
 * <br/>
 * this would look then for a full sphere shape for example like this:<br/>
 *    sphere:position,1.2,3,9.5:radius,2.5<br/>
 * <br/>
 * or for a full box like this:<br/>
 *    box:position,10.3,0.5,8:rotation,0,0,90:halfExtends,2,1,1.5<br/>
 * <br/>
 * this format allows a decoding like this:<br/>
 *    shapeStrings = string.split(' ')<br/>
 *    for shapeString in shapeStrings<br/>
 *       parameterStrings = shapeString.split(':')<br/>
 *       shapeType = parameterStrings[0]<br/>
 *       if shapeType == sphere<br/>
 *          for parameterString in parameterStrings[1:]<br/>
 *             valueStrings = parameterString.split(',')<br/>
 *             if valueStrings[0] == 'position'<br/>
 *                sphere.position = valueStrings[1:3]<br/>
 *             else ...<br/>
 *       else ...<br/>
 * <br/>
 * Parameters with default values are not written to not blow up the string size.
 */
class igdePropertyStringEncodeShapeList : public decShapeVisitor{
private:
	decString  *pString;
	bool pRequiresSeparator;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create analytic shape visitor. */
	igdePropertyStringEncodeShapeList( decString *string );
	
	/** \brief Clean up analytic shape visitor. */
	virtual ~igdePropertyStringEncodeShapeList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Reset visitor. */
	void Reset();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit shape. */
	virtual void VisitShape( decShape &shape );
	
	/** \brief Visit sphere shape. */
	virtual void VisitShapeSphere( decShapeSphere &sphere );
	
	/** \brief Visit box shape. */
	virtual void VisitShapeBox( decShapeBox &box );
	
	/** \brief Visit cylinder shape. */
	virtual void VisitShapeCylinder( decShapeCylinder &cylinder );
	
	/** \brief Visit capsule shape. */
	virtual void VisitShapeCapsule( decShapeCapsule &capsule );
	
	/** \brief Visit hull shape. */
	virtual void VisitShapeHull( decShapeHull &hull );
	/*@}*/
	
	
	
private:
	void pAddSeparator();
};

#endif
