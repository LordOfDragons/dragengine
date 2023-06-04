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
 * Animator rule.
 */
class aeRule : public deObject{
public:
	/** Type holding strong reference. */
	typedef deTObjectReference<aeRule> Ref;
	
	
	
private:
	aeAnimator *pAnimator;
	aeRuleGroup *pParentGroup;
	deAnimatorRule *pEngRule;
	
	decString pName;
	deAnimatorRuleVisitorIdentify::eRuleTypes pType;
	
	decStringSet pListBones;
	decStringSet pListVertexPositionSets;
	
	deAnimatorRule::eBlendModes pBlendMode;
	float pBlendFactor;
	bool pInvertBlendFactor;
	bool pEnabled;
	
	aeControllerTarget pTargetBlendFactor;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create a new animator rule. */
	aeRule( deAnimatorRuleVisitorIdentify::eRuleTypes type );
	/** Create a copy of an animator rule. */
	aeRule( const aeRule &copy );
	/** Clean up the animator rule. */
	virtual ~aeRule();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieve the parent animator. */
	aeAnimator *GetAnimator() const;
	/** Set the parent animator. */
	void SetAnimator( aeAnimator *animator );
	
	/** Retrieve the engine animator rule or NULL. */
	inline deAnimatorRule *GetEngineRule() const{ return pEngRule; }
	/** Set the engine animator rule or NULL. */
	void SetEngineRule( deAnimatorRule *rule );
	/** Create an engine animator rule. */
	virtual deAnimatorRule *CreateEngineRule() = 0;
	/** Init the given engine rule with the default rule properties. */
	void InitEngineRule( deAnimatorRule *engRule ) const;
	
	/** Retrieve the rule type. */
	inline deAnimatorRuleVisitorIdentify::eRuleTypes GetType() const{ return pType; }
	
	/** Retrieve the parent group or NULL if there is none. */
	inline aeRuleGroup *GetParentGroup() const{ return pParentGroup; }
	/** Set the parent group or NULL if there is none. */
	void SetParentGroup( aeRuleGroup *group );
	
	/** Retrieve the name. */
	inline const decString &GetName() const{ return pName; }
	/** Set the name. */
	void SetName( const char *filename );
	
	/** Determine if the rule is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	/** Set if the rule is enabled. */
	void SetEnabled( bool enabled );
	inline deAnimatorRule::eBlendModes GetBlendMode() const{ return pBlendMode; }
	/** Set the blend mode. */
	void SetBlendMode( deAnimatorRule::eBlendModes mode );
	/** Retrieve the blend factor. */
	inline float GetBlendFactor() const{ return pBlendFactor; }
	/** Set the source blend factor. */
	void SetBlendFactor( float factor );
	
	/** Invert blend factor. */
	inline bool GetInvertBlendFactor() const{ return pInvertBlendFactor; }
	
	/** Set invert blend factor. */
	void SetInvertBlendFactor( bool invert );
	
	/** Update Component and Animation. */
	virtual void UpdateCompAnim();
	/** Update targets. */
	virtual void UpdateTargets();
	/** Retrieve the number of targets using a given link. */
	virtual int CountLinkUsage( aeLink *link ) const;
	/** Remove a link from all targets using it. */
	virtual void RemoveLinkFromTargets( aeLink *link );
	/** Remove all links from all targets. */
	virtual void RemoveLinksFromAllTargets();
	
	/** Retrieve the source factor target. */
	inline aeControllerTarget &GetTargetBlendFactor(){ return pTargetBlendFactor; }
	inline const aeControllerTarget &GetTargetBlendFactor() const{ return pTargetBlendFactor; }
	
	/** List all links of all rule targets. */
	virtual void ListLinks( aeLinkList& list );
	
	/** Notify the engine that the rule changed. */
	void NotifyRuleChanged();
	
	/** Create a copy of this rule. */
	virtual aeRule *CreateCopy() const = 0;
	
	/** Parent animator changed. */
	virtual void OnParentAnimatorChanged();
	/*@}*/
	
	
	
	/** \name Bone Management */
	/*@{*/
	/** Retrieve the list of bones. */
	inline const decStringSet &GetListBones() const{ return pListBones; }
	
	/** Set list of bones. */
	void SetListBones( const decStringSet &bones );
	
	/** Add a bone. */
	void AddBone( const char *bone );
	
	/** Remove the given bone. */
	void RemoveBone( const char *bone );
	
	/** Remove all bones. */
	void RemoveAllBones();
	/*@}*/
	
	
	
	/** \name Vertex position set management */
	/*@{*/
	/** List of vertex position sets. */
	inline const decStringSet &GetListVertexPositionSets() const{ return pListVertexPositionSets; }
	
	/** Set list of vertex position sets. */
	void SetListVertexPositionSets( const decStringSet &sets );
	
	/** Add a vertex position set. */
	void AddVertexPositionSet( const char *vertexPositionSet );
	
	/** Remove vertex position set. */
	void RemoveVertexPositionSet( const char *vertexPositionSet );
	
	/** Remove all vertex position sets. */
	void RemoveAllVertexPositionSets();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Copy another animator rule to this animator rule. */
	virtual aeRule &operator=( const aeRule &copy );
	/*@}*/
	
	/** \name Helper */
	/*@{*/
	/** Create a new rule from a rule type. */
	static aeRule *CreateRuleFromType( deAnimatorRuleVisitorIdentify::eRuleTypes type );
	/*@}*/
};

#endif
