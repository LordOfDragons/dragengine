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

#ifndef _AERULE_H_
#define _AERULE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>

#include "../controller/aeControllerTarget.h"
#include "../link/aeLink.h"

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
	typedef decTObjectOrderedSet<aeRule> List;
	
	
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
	
	aeControllerTarget::Ref pTargetBlendFactor;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create a new animator rule. */
	aeRule(deAnimatorRuleVisitorIdentify::eRuleTypes type);
	/** Create a copy of an animator rule. */
	aeRule(const aeRule &copy);
	/** Clean up the animator rule. */
protected:
	virtual ~aeRule();
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieve the parent animator. */
	aeAnimator *GetAnimator() const;
	/** Set the parent animator. */
	void SetAnimator(aeAnimator *animator);
	
	/** Retrieve the engine animator rule or nullptr. */
	inline deAnimatorRule *GetEngineRule() const{ return pEngRule; }
	/** Set the engine animator rule or nullptr. */
	void SetEngineRule(deAnimatorRule *rule);
	/** Create an engine animator rule. */
	virtual deAnimatorRule::Ref CreateEngineRule() = 0;
	/** Init the given engine rule with the default rule properties. */
	void InitEngineRule(deAnimatorRule *engRule) const;
	
	/** Retrieve the rule type. */
	inline deAnimatorRuleVisitorIdentify::eRuleTypes GetType() const{ return pType; }
	
	/** Retrieve the parent group or nullptr if there is none. */
	inline aeRuleGroup *GetParentGroup() const{ return pParentGroup; }
	/** Set the parent group or nullptr if there is none. */
	void SetParentGroup(aeRuleGroup *group);
	
	/** Retrieve the name. */
	inline const decString &GetName() const{ return pName; }
	/** Set the name. */
	void SetName(const char *filename);
	
	/** Determine if the rule is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	/** Set if the rule is enabled. */
	void SetEnabled(bool enabled);
	inline deAnimatorRule::eBlendModes GetBlendMode() const{ return pBlendMode; }
	/** Set the blend mode. */
	void SetBlendMode(deAnimatorRule::eBlendModes mode);
	/** Retrieve the blend factor. */
	inline float GetBlendFactor() const{ return pBlendFactor; }
	/** Set the source blend factor. */
	void SetBlendFactor(float factor);
	
	/** Invert blend factor. */
	inline bool GetInvertBlendFactor() const{ return pInvertBlendFactor; }
	
	/** Set invert blend factor. */
	void SetInvertBlendFactor(bool invert);
	
	/** Update Component and Animation. */
	virtual void UpdateCompAnim();
	/** Update targets. */
	virtual void UpdateTargets();
	/** Retrieve the number of targets using a given link. */
	virtual int CountLinkUsage(aeLink *link) const;
	/** Remove a link from all targets using it. */
	virtual void RemoveLinkFromTargets(aeLink *link);
	/** Remove all links from all targets. */
	virtual void RemoveLinksFromAllTargets();
	
	/** Retrieve the source factor target. */
	inline const aeControllerTarget::Ref &GetTargetBlendFactor() const{ return pTargetBlendFactor; }
	
	/** List all links of all rule targets. */
	virtual void ListLinks(aeLink::List& list);
	
	/** Notify the engine that the rule changed. */
	void NotifyRuleChanged();
	
	/** Create a copy of this rule. */
	virtual aeRule::Ref CreateCopy() const = 0;
	
	/** Parent animator changed. */
	virtual void OnParentAnimatorChanged();
	/*@}*/
	
	
	
	/** \name Bone Management */
	/*@{*/
	/** Retrieve the list of bones. */
	inline const decStringSet &GetListBones() const{ return pListBones; }
	
	/** Set list of bones. */
	void SetListBones(const decStringSet &bones);
	
	/** Add a bone. */
	void AddBone(const char *bone);
	
	/** Remove the given bone. */
	void RemoveBone(const char *bone);
	
	/** Remove all bones. */
	void RemoveAllBones();
	/*@}*/
	
	
	
	/** \name Vertex position set management */
	/*@{*/
	/** List of vertex position sets. */
	inline const decStringSet &GetListVertexPositionSets() const{ return pListVertexPositionSets; }
	
	/** Set list of vertex position sets. */
	void SetListVertexPositionSets(const decStringSet &sets);
	
	/** Add a vertex position set. */
	void AddVertexPositionSet(const char *vertexPositionSet);
	
	/** Remove vertex position set. */
	void RemoveVertexPositionSet(const char *vertexPositionSet);
	
	/** Remove all vertex position sets. */
	void RemoveAllVertexPositionSets();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Copy another animator rule to this animator rule. */
	aeRule &operator=(const aeRule &copy);
	/*@}*/
	
	/** \name Helper */
	/*@{*/
	/** Create a new rule from a rule type. */
	static aeRule::Ref CreateRuleFromType(deAnimatorRuleVisitorIdentify::eRuleTypes type);
	/*@}*/
};

#endif
