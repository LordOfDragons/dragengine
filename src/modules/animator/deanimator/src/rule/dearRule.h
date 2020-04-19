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

#ifndef _DEARRULE_H_
#define _DEARRULE_H_

#include "../dearControllerTarget.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>

class dearAnimatorInstance;
class dearBoneStateList;
class deDEAnimator;



/**
 * \brief Base animator rule class.
 */
class dearRule{
private:
	dearAnimatorInstance &pInstance;
	const deAnimatorRule &pRule;
	int *pBoneMappings;
	int pBoneMappingCount;
	bool pUseAllBones;
	
	dearControllerTarget pTargetBlendFactor;
	
	const deAnimatorRule::eBlendModes pBlendMode;
	const float pBlendFactor;
	const bool pEnabled;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create rule. */
	dearRule( dearAnimatorInstance &instance, int firstLink, const deAnimatorRule &rule );
	
	/** \brief Clean up animator. */
	virtual ~dearRule();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Animator module. */
	deDEAnimator &GetModule() const;
	
	/** \brief Animator instance. */
	inline dearAnimatorInstance &GetInstance(){ return pInstance; } 
	inline const dearAnimatorInstance &GetInstance() const{ return pInstance; } 
	
	/** \brief Animator rule. */
	inline const deAnimatorRule &GetRule() const{ return pRule; }
	
	/** \brief All bones are used or a list of bones. */
	inline bool GetUseAllBones() const{ return pUseAllBones; }
	
	/** \brief Bone mapping count. */
	int GetBoneMappingCount() const;
	
	/** \brief Bone mapping for bone index. */
	int GetBoneMappingFor( int boneIndex ) const;
	
	
	
	/** \brief Blend factor after applying a controller to it if one is set. */
	float GetBlendFactor() const;
	
	
	
	/** \brief Thread-safe rule blend mode. */
	inline deAnimatorRule::eBlendModes GetBlendMode() const{ return pBlendMode; }
	
	/** \brief Thread-safe rule enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	
	
	/**
	 * \brief Capture animator state.
	 * \details The default implementation throws an exception.
	 */
	virtual void CaptureStateInto( int identifier );
	
	/**
	 * \brief Store animation frame.
	 * \details The default implementation throws an exception.
	 */
	virtual void StoreFrameInto( int identifier, const char *moveName, float moveTime );
	
	/**
	 * \brief Check if a full rebuild of the animator instance is required.
	 */
	virtual bool RebuildInstance();
	
	/** \brief Apply to animator. */
	virtual void Apply( dearBoneStateList &stalist ) = 0;
	
	/** \brief Controller changed. */
	virtual void ControllerChanged( int controller );
	
	/** \brief Rule changed. */
	virtual void RuleChanged();
	/*@}*/
	
private:
	void pUpdateBoneMappings();
};

#endif
