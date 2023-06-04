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

class dearAnimator;
class dearAnimatorInstance;
class dearAnimation;
class dearBoneStateList;
class dearVPSStateList;
class deDEAnimator;



/**
 * Base animator rule class.
 */
class dearRule{
private:
	dearAnimatorInstance &pInstance;
	const dearAnimator &pAnimator;
	const deAnimatorRule &pRule;
	
	int *pBoneMappings;
	int pBoneMappingCount;
	bool pUseAllBones;
	
	int *pVPSMappings;
	int pVPSMappingCount;
	bool pUseAllVPS;
	
	dearControllerTarget pTargetBlendFactor;
	
	const deAnimatorRule::eBlendModes pBlendMode;
	const float pBlendFactor;
	const bool pInvertBlendFactor;
	const bool pEnabled;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create rule. */
	dearRule( dearAnimatorInstance &instance, const dearAnimator &animator,
		int firstLink, const deAnimatorRule &rule );
	
	/** Clean up animator. */
	virtual ~dearRule();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Animator module. */
	deDEAnimator &GetModule() const;
	
	/** Animator. */
	inline const dearAnimator &GetAnimator() const{ return pAnimator; }
	
	/** Animator instance. */
	inline dearAnimatorInstance &GetInstance(){ return pInstance; } 
	inline const dearAnimatorInstance &GetInstance() const{ return pInstance; } 
	
	/** Animator rule. */
	inline const deAnimatorRule &GetRule() const{ return pRule; }
	
	
	
	/** All bones are used or a list of bones. */
	inline bool GetUseAllBones() const{ return pUseAllBones; }
	
	/** Bone mapping count. */
	int GetBoneMappingCount() const;
	
	/** Bone mapping for bone index. */
	int GetBoneMappingFor( int boneIndex ) const;
	
	
	
	/** All vertex position sets are used or a list of vertex position sets. */
	inline bool GetUseAllVPS() const{ return pUseAllVPS; }
	
	/** Vertex position set mapping count. */
	int GetVPSMappingCount() const;
	
	/** Vertex position set mapping for vertex position set index. */
	int GetVPSMappingFor( int vpsIndex ) const;
	
	
	
	/** Blend factor after applying a controller to it if one is set. */
	float GetBlendFactor() const;
	
	/** Animation to use. */
	dearAnimation *GetUseAnimation() const;
	
	
	
	/** Thread-safe rule blend mode. */
	inline deAnimatorRule::eBlendModes GetBlendMode() const{ return pBlendMode; }
	
	/** Thread-safe rule enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	
	
	/**
	 * Capture animator state.
	 * \details The default implementation throws an exception.
	 */
	virtual void CaptureStateInto( int identifier );
	
	/**
	 * Store animation frame.
	 * \details The default implementation throws an exception.
	 */
	virtual void StoreFrameInto( int identifier, const char *moveName, float moveTime );
	
	/**
	 * Check if a full rebuild of the animator instance is required.
	 */
	virtual bool RebuildInstance() const;
	
	/** Apply to animator. */
	virtual void Apply( dearBoneStateList &stalist, dearVPSStateList &vpsstalist ) = 0;
	
	/** Controller changed. */
	virtual void ControllerChanged( int controller );
	
	/** Rule changed. */
	virtual void RuleChanged();
	/*@}*/
	
private:
	void pUpdateBoneMappings();
	void pUpdateVPSMappings();
};

#endif
