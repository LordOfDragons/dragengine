/* 
 * Drag[en]gine Animator Module
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

#ifndef _DEARLINK_H_
#define _DEARLINK_H_

#include <dragengine/common/curve/decCurveBezierEvaluator.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/animator/deAnimatorLink.h>

class decIntList;
class dearAnimatorInstance;
class deAnimatorLink;
class dearControllerStates;


/**
 * \brief Animator link.
 */
class dearLink{
private:
	const dearAnimatorInstance &pInstance;
	deAnimatorLink pLink;
	const decCurveBezierEvaluator pEvaluator;
	int pBoneIndex;
	const bool pWrapY;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create link. */
	dearLink( dearAnimatorInstance &instance, const deAnimatorLink &link,
		const decIntList &controllerMapping );
	
	/** \brief Clean up link. */
	~dearLink();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Animator link. */
	inline const deAnimatorLink &GetLink() const{ return pLink; }
	
	/** \brief Link has valid controller. */
	bool HasController() const;
	
	/** \brief Controller index relative to parent animator instance. */
	int GetController() const;
	
	/** \brief Link has valid bone. */
	bool HasBone() const;
	
	
	
	/** \brief Value of link. */
	float GetValue( float defaultValue ) const;
	
	/** \brief Vector of link. */
	void GetVector( decVector &vector ) const;
	
	/** \brief Quaternion of link. */
	void GetQuaternion( decQuaternion &quaternion ) const;
	/*@}*/
};

#endif
