/* 
 * FBX Modules
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

#ifndef _FBXANIMATIONCURVE_H_
#define _FBXANIMATIONCURVE_H_


#include <stdint.h>

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>


class fbxNode;
class fbxProperty;


/**
 * \brief FBX managed animation curve.
 */
class fbxAnimationCurve : public deObject{
private:
	fbxNode &pNodeCurve;
	fbxProperty *pPropertyTime;
	fbxProperty *pPropertyValue;
	int pKeyCount;
	decVector pDefaultValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create move. */
	fbxAnimationCurve( fbxNode &nodeCurve );
	
protected:
	/** \brief Clean up move. */
	virtual ~fbxAnimationCurve();
	/*@}*/
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Curve node. */
	inline fbxNode &GetNodeCurve() const{ return pNodeCurve; }
	
	/** \brief Time property. */
	inline fbxProperty &GetPropertyTime() const{ return *pPropertyTime; }
	
	/** \brief Value property. */
	inline fbxProperty &GetPropertyValue() const{ return *pPropertyValue; }
	
	/** \brief Default value. */
	inline const decVector &GetDefaultValue() const{ return pDefaultValue; }
	/*@}*/
};

#endif
