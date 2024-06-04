/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
class DE_DLL_EXPORT igdePropertyStringEncodeShapeList : public decShapeVisitor{
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
