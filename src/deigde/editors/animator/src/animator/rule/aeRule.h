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

#include "../controller/aeControllerTarget.h"
#include "../link/aeLink.h"
#include "../../meta/animator/aeMCRule.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertySelection.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>
#include <deigde/meta/property/igdeMetaPropertyObjectSet.h>
#include <deigde/meta/property/igdeMetaPropertyStringSet.h>

class aeWindowMain;
class aeAnimator;
class aeRuleGroup;
class deAnimatorRule;
class deAnimatorRuleVisitor;
class igdeTranslationManager;

/**
 * Animator rule.
 */
class aeRule : public deObject{
public:
	/** Type holding strong reference. */
	using Ref = deTObjectReference<aeRule>;
	using List = decTObjectOrderedSet<aeRule>;
	
	
private:
	aeAnimator *pAnimator;
	aeRuleGroup *pParentGroup;
	deAnimatorRule *pEngRule;
	int pIndex;
	
	aeMCRule::Ref pMetaContext;
	deAnimatorRuleVisitorIdentify::eRuleTypes pType;
	
	aeControllerTarget::Ref pTargetBlendFactor;
	
public:
	igdeMetaPropertyStringStorage::Storage name;
	igdeMetaPropertySelectionEnumStorage<deAnimatorRule::eBlendModes>::Storage blendMode;
	igdeMetaPropertyFloatStorage::Storage blendFactor;
	igdeMetaPropertyBooleanStorage::Storage invertBlendFactor;
	igdeMetaPropertyBooleanStorage::Storage enabled;
	igdeMetaPropertyStringSetStorage::Storage affectedBones;
	igdeMetaPropertyStringSetStorage::Storage affectedVertexPositionSets;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage targetBlendFactor;
	
protected:
	/** \name Constructors and Destructors */
	/*@{*/
	aeRule() = delete;
	aeRule(const aeRule&) = delete;
	
	/** Create a new animator rule. */
	aeRule(aeWindowMain &windowMain, aeMCRule::Ref &&metaContext,
		deAnimatorRuleVisitorIdentify::eRuleTypes type, const char *name);
	
	/** Create a copy of an animator rule. */
	aeRule(aeWindowMain &windowMain, aeMCRule::Ref &&metaContext, const aeRule &copy);
	
	/** Clean up the animator rule. */
	~aeRule() override;
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** Retrieve the parent animator. */
	aeAnimator *GetAnimator() const;
	/** Set the parent animator. */
	void SetAnimator(aeAnimator *animator);
	
	/** Parent animator throwing exception if nullptr. */
	aeAnimator &GetAnimatorRef() const;
	
	/** Meta context. */
	inline const aeMCRule::Ref &GetMetaContext() const{ return pMetaContext; }
	
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
	
	/** Index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** Sets the index. */
	void SetIndex(int index);
	
	/** Retrieve the parent group or nullptr if there is none. */
	inline aeRuleGroup *GetParentGroup() const{ return pParentGroup; }
	/** Set the parent group or nullptr if there is none. */
	void SetParentGroup(aeRuleGroup *group);
	
	/** Retrieve the name. */
	inline const decString &GetName() const{ return name; }
	/** Set the name. */
	void SetName(const char *filename);
	
	/** Determine if the rule is enabled. */
	inline bool GetEnabled() const{ return enabled; }
	/** Set if the rule is enabled. */
	void SetEnabled(bool enabled);
	inline deAnimatorRule::eBlendModes GetBlendMode() const{ return blendMode; }
	/** Set the blend mode. */
	void SetBlendMode(deAnimatorRule::eBlendModes mode);
	/** Retrieve the blend factor. */
	inline float GetBlendFactor() const{ return blendFactor; }
	/** Set the source blend factor. */
	void SetBlendFactor(float factor);
	
	/** Invert blend factor. */
	inline bool GetInvertBlendFactor() const{ return invertBlendFactor; }
	
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
	virtual aeRule::Ref CreateCopy(aeWindowMain &windowMain) const = 0;
	
	/** Parent animator changed. */
	virtual void OnParentAnimatorChanged();
	/*@}*/
	
	
	
	/** \name Bone Management */
	/*@{*/
	/** Retrieve the list of bones. */
	inline const decStringSet &GetListBones() const{ return affectedBones; }
	
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
	inline const decStringSet &GetListVertexPositionSets() const{ return affectedVertexPositionSets; }
	
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
	aeRule &operator=(const aeRule &copy) = delete;
	/*@}*/
	
	/** \name Helper */
	/*@{*/
	/** Create a new rule from a rule type. */
	static aeRule::Ref CreateRuleFromType(aeWindowMain &windowMain,
		deAnimatorRuleVisitorIdentify::eRuleTypes type,
		const igdeTranslationManager &tm);
	/*@}*/
	
	
protected:
	void pInitCopy(const aeRule &copy);
	
	void pUpdateEngineTarget(deAnimatorControllerTarget &target,
		const igdeMetaPropertyObjectSetStorage<aeLink>::Storage &storage) const;
};

#endif
