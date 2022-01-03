/* 
 * Drag[en]gine Animator Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEARRULEMIRROR_H_
#define _DEARRULEMIRROR_H_

#include "dearRule.h"

#include <dragengine/resources/animator/rule/deAnimatorRuleMirror.h>



/**
 * \brief Mirror rule.
 */
class dearRuleMirror : public dearRule{
private:
	struct sBonePair{
		int first;
		int second;
	};
	
	const deAnimatorRuleMirror &pMirror;
	
	int pMirrorBone;
	decMatrix pMirrorMatrix;
	sBonePair *pBonePairs;
	int pBonePairCount;
	
	const deAnimatorRuleMirror::eMirrorAxis pMirrorAxis;
	const bool pEnablePosition;
	const bool pEnableOrientation;
	const bool pEnableSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create rule. */
	dearRuleMirror( dearAnimatorInstance &instance, int firstLink, const deAnimatorRuleMirror &rule );
	
	/** \brief Clean up rule. */
	virtual ~dearRuleMirror();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Apply to animator. */
	virtual void Apply( dearBoneStateList &stalist );
	
	/** \brief Rule changed. */
	virtual void RuleChanged();
	/*@}*/
	
	
	
private:
	void pUpdateBones();
};

#endif
