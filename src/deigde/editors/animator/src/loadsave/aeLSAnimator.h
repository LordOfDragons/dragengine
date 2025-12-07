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

#ifndef _AELSANIMATOR_H_
#define _AELSANIMATOR_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

#include <deigde/utils/igdeBaseXML.h>

class aeRuleGroup;
class aeRuleLimit;
class aeAnimator;
class aeController;
class aeLink;
class aeRule;
class aeRuleAnimation;
class aeRuleAnimationDifference;
class aeRuleAnimationSelect;
class aeRuleBoneTransformator;
class aeRuleStateManipulator;
class aeRuleStateSnapshot;
class aeRuleInverseKinematic;
class aeRuleForeignState;
class aeRuleSubAnimator;
class aeRuleTrackTo;
class aeRuleMirror;
class aeControllerTarget;
class aeLoadSaveSystem;

class aeController;
class decXmlWriter;
class decXmlElementTag;
class decBaseFileReader;
class decBaseFileWriter;
class deAnimatorLink;



/**
 * Loads and saves actor animators in the Drag[en]gine Actor Animator XML format.
 */
class aeLSAnimator : public igdeBaseXML{
private:
	aeLoadSaveSystem *pLSSys;
	
	decString pName;
	decString pPattern;
	
public:
	// constructor, destructor
	aeLSAnimator(aeLoadSaveSystem *lssys);
	~aeLSAnimator();
	
	// management
	inline const decString &GetName() const{ return pName; }
	void SetName(const char *name);
	inline const decString &GetPattern() const{ return pPattern; }
	void SetPattern(const char *pattern);
	
	// loading and saving
	void LoadAnimator(aeAnimator *animator, decBaseFileReader *file);
	void SaveAnimator(aeAnimator *animator, decBaseFileWriter *file);
	
private:
	void pSaveDisplay(decXmlWriter &writer, const aeAnimator &animator);
	void pSaveLocomotion(decXmlWriter &writer, const aeAnimator &animator);
	void pSaveController(decXmlWriter &writer, const aeController &controller);
	void pSaveLink(decXmlWriter &writer, const aeAnimator &animator, const aeLink &link);
	void pSaveRuleCommon(decXmlWriter &writer, const aeAnimator &animator, const aeRule &rule);
	void pSaveControllerTarget(decXmlWriter &writer, const aeAnimator &animator, const aeControllerTarget &target, const char *name);
	void pSaveRule(decXmlWriter &writer, const aeAnimator &animator, const aeRule &rule);
	void pSaveRuleAnimation(decXmlWriter &writer, const aeAnimator &animator, const aeRuleAnimation &rule);
	void pSaveRuleAnimationDifference(decXmlWriter &writer, const aeAnimator &animator, const aeRuleAnimationDifference &rule);
	void pSaveRuleAnimationSelect(decXmlWriter &writer, const aeAnimator &animator, const aeRuleAnimationSelect &rule);
	void pSaveRuleBoneTransformator(decXmlWriter &writer, const aeAnimator &animator, const aeRuleBoneTransformator &rule);
	void pSaveRuleStateManipulator(decXmlWriter &writer, const aeAnimator &animator, const aeRuleStateManipulator &rule);
	void pSaveRuleStateSnapshot(decXmlWriter &writer, const aeAnimator &animator, const aeRuleStateSnapshot &rule);
	void pSaveRuleInverseKinematic(decXmlWriter &writer, const aeAnimator &animator, const aeRuleInverseKinematic &rule);
	void pSaveRuleForeignState(decXmlWriter &writer, const aeAnimator &animator, const aeRuleForeignState &rule);
	void pSaveRuleGroup(decXmlWriter &writer, const aeAnimator &animator, const aeRuleGroup &rule);
	void pSaveRuleSubAnimator(decXmlWriter &writer, const aeAnimator &animator, const aeRuleSubAnimator &rule);
	void pSaveRuleTrackTo(decXmlWriter &writer, const aeAnimator &animator, const aeRuleTrackTo &rule);
	void pSaveRuleLimit(decXmlWriter &writer, const aeAnimator &animator, const aeRuleLimit &rule);
	void pSaveRuleMirror(decXmlWriter &writer, const aeAnimator &animator, const aeRuleMirror &rule);
	
	bool pHasAttribute(decXmlElementTag *tag, const char *name) const;
	const char *pGetAttributeString(decXmlElementTag *tag, const char *name) const;
	int pGetAttributeInt(decXmlElementTag *tag, const char *name) const;
	float pGetAttributeFloat(decXmlElementTag *tag, const char *name) const;
	
	void pLoadAnimator(decXmlElementTag *root, aeAnimator &animator);
	void pLoadDisplay(decXmlElementTag *root, aeAnimator &animator);
	void pLoadLocomotion(decXmlElementTag *root, aeAnimator &animator);
	void pLoadLocomotionLeg(decXmlElementTag *root, aeAnimator &animator, int leg);
	void pLoadController(decXmlElementTag *root, aeAnimator &animator);
	void pLoadControllerLimit(decXmlElementTag *root, aeController &controller);
	void pLoadLink(decXmlElementTag *root, aeAnimator &animator);
	aeRule *pLoadRule(decXmlElementTag *root, aeAnimator &animator);
	aeRule *pLoadRuleAnimation(decXmlElementTag *root, aeAnimator &animator);
	aeRule *pLoadRuleAnimationDifference(decXmlElementTag *root, aeAnimator &animator);
	aeRule *pLoadRuleAnimationSelect(decXmlElementTag *root, aeAnimator &animator);
	aeRule *pLoadRuleLimit(decXmlElementTag *root, aeAnimator &animator);
	aeRule *pLoadRuleStateManipulator(decXmlElementTag *root, aeAnimator &animator);
	aeRule *pLoadRuleStateSnapshot(decXmlElementTag *root, aeAnimator &animator);
	aeRule *pLoadRuleInverseKinematic(decXmlElementTag *root, aeAnimator &animator);
	aeRule *pLoadRuleForeignState(decXmlElementTag *root, aeAnimator &animator);
	aeRule *pLoadRuleGroup(decXmlElementTag *root, aeAnimator &animator);
	aeRule *pLoadRuleSubAnimator(decXmlElementTag *root, aeAnimator &animator);
	aeRule *pLoadRuleTrackTo(decXmlElementTag *root, aeAnimator &animator);
	aeRule *pLoadRuleBoneTransformator(decXmlElementTag *root, aeAnimator &animator);
	aeRule *pLoadRuleMirror(decXmlElementTag *root, aeAnimator &animator);
	bool pLoadRuleCommon(decXmlElementTag *root, aeAnimator &animator, aeRule &rule);
	void pLoadControllerTarget(decXmlElementTag *root, aeAnimator &animator, aeControllerTarget &target);
	void pLoadVector(decXmlElementTag *root, decVector &vector);
	
	void pLoadSubAnimators(aeAnimator &animator);
	void pLoadSubAnimators(aeAnimator &animator, const aeRuleGroup &group);
};

// end of include only once
#endif
