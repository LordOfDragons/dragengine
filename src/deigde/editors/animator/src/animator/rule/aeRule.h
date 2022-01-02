/* 
 * Drag[en]gine IGDE Animator Editor
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

#ifndef _AERULE_H_
#define _AERULE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>

#include "../controller/aeControllerTarget.h"

class aeLinkList;
class aeAnimator;
class aeRuleGroup;
class deAnimatorRule;
class deAnimatorRuleVisitor;



/**
 * \brief Animator rule.
 */
class aeRule : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<aeRule> Ref;
	
	
	
private:
	aeAnimator *pAnimator;
	aeRuleGroup *pParentGroup;
	deAnimatorRule *pEngRule;
	
	decString pName;
	deAnimatorRuleVisitorIdentify::eRuleTypes pType;
	
	decStringSet pListBones;
	
	deAnimatorRule::eBlendModes pBlendMode;
	float pBlendFactor;
	bool pEnabled;
	
	aeControllerTarget pTargetBlendFactor;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new animator rule. */
	aeRule( deAnimatorRuleVisitorIdentify::eRuleTypes type );
	/** \brief Create a copy of an animator rule. */
	aeRule( const aeRule &copy );
	/** \brief Clean up the animator rule. */
	virtual ~aeRule();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieve the parent animator. */
	aeAnimator *GetAnimator() const;
	/** \brief Set the parent animator. */
	void SetAnimator( aeAnimator *animator );
	
	/** \brief Retrieve the engine animator rule or NULL. */
	inline deAnimatorRule *GetEngineRule() const{ return pEngRule; }
	/** \brief Set the engine animator rule or NULL. */
	void SetEngineRule( deAnimatorRule *rule );
	/** \brief Create an engine animator rule. */
	virtual deAnimatorRule *CreateEngineRule() = 0;
	/** \brief Init the given engine rule with the default rule properties. */
	void InitEngineRule( deAnimatorRule *engRule ) const;
	
	/** \brief Retrieve the rule type. */
	inline deAnimatorRuleVisitorIdentify::eRuleTypes GetType() const{ return pType; }
	
	/** \brief Retrieve the parent group or NULL if there is none. */
	inline aeRuleGroup *GetParentGroup() const{ return pParentGroup; }
	/** \brief Set the parent group or NULL if there is none. */
	void SetParentGroup( aeRuleGroup *group );
	
	/** \brief Retrieve the name. */
	inline const decString &GetName() const{ return pName; }
	/** \brief Set the name. */
	void SetName( const char *filename );
	
	/** \brief Determine if the rule is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	/** \brief Set if the rule is enabled. */
	void SetEnabled( bool enabled );
	inline deAnimatorRule::eBlendModes GetBlendMode() const{ return pBlendMode; }
	/** \brief \brief Set the blend mode. */
	void SetBlendMode( deAnimatorRule::eBlendModes mode );
	/** \brief \brief Retrieve the blend factor. */
	inline float GetBlendFactor() const{ return pBlendFactor; }
	/** \brief \brief Set the source blend factor. */
	void SetBlendFactor( float factor );
	
	/** \brief Update Component and Animation. */
	virtual void UpdateCompAnim();
	/** \brief Update targets. */
	virtual void UpdateTargets();
	/** \brief Retrieve the number of targets using a given link. */
	virtual int CountLinkUsage( aeLink *link ) const;
	/** \brief Remove a link from all targets using it. */
	virtual void RemoveLinkFromTargets( aeLink *link );
	/** \brief Remove all links from all targets. */
	virtual void RemoveLinksFromAllTargets();
	
	/** \brief Retrieve the source factor target. */
	inline aeControllerTarget &GetTargetBlendFactor(){ return pTargetBlendFactor; }
	inline const aeControllerTarget &GetTargetBlendFactor() const{ return pTargetBlendFactor; }
	
	/** \brief List all links of all rule targets. */
	virtual void ListLinks( aeLinkList& list );
	
	/** \brief Notify the engine that the rule changed. */
	void NotifyRuleChanged();
	
	/** \brief Create a copy of this rule. */
	virtual aeRule *CreateCopy() const = 0;
	
	/** \brief Parent animator changed. */
	virtual void OnParentAnimatorChanged();
	/*@}*/
	
	/** \name Bone Management */
	/*@{*/
	/** \brief Retrieve the list of bones. */
	inline const decStringSet &GetListBones() const{ return pListBones; }
	
	/** \brief Set list of bones. */
	void SetListBones( const decStringSet &bones );
	
	/** \brief Add a bone. */
	void AddBone( const char *bone );
	/** \brief Remove the given bone. */
	void RemoveBone( const char *bone );
	/** \brief Remove all bones. */
	void RemoveAllBones();
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy another animator rule to this animator rule. */
	virtual aeRule &operator=( const aeRule &copy );
	/*@}*/
	
	/** \name Helper */
	/*@{*/
	/** \brief Create a new rule from a rule type. */
	static aeRule *CreateRuleFromType( deAnimatorRuleVisitorIdentify::eRuleTypes type );
	/*@}*/
};

#endif
