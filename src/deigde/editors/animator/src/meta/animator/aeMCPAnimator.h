/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _AEMCPANIMATOR_H_
#define _AEMCPANIMATOR_H_

#include "aeTMCPAnimator.h"
#include "../../animator/aeAnimator.h"

#include <deigde/meta/igdeMetaContextItemInfo.h>
#include <deigde/meta/property/igdeMetaPropertyPath.h>
#include <deigde/meta/property/igdeMetaPropertyStringSet.h>


/** Rig path. */
class aeMCPAnimatorRig : public aeTMCPAnimator<igdeMetaPropertyPath>{
public:
	aeMCPAnimatorRig() : aeTMCPAnimator("animator.rig",
		"@Animator.WPAnimator.Rig", "@Animator.WPAnimator.Rig.ToolTip",
		igdeEnvironment::efpltRig){}
	
	virtual ~aeMCPAnimatorRig() override{}
	
	const decString &GetPropertyValue(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetRigPath();
	}
	
	void SetPropertyValue(const igdeMetaContext::Ref &context, const decString &value) override{
		Animator(context).SetRigPath(value);
	}
	
	const decString &GetPropertyBasePath(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetDirectoryPath();
	}
};


/** Animation path. */
class aeMCPAnimatorAnimation : public aeTMCPAnimator<igdeMetaPropertyPath>{
public:
	aeMCPAnimatorAnimation() : aeTMCPAnimator("animator.animation",
		"@Animator.WPAnimator.Animation", "@Animator.WPAnimator.Animation.ToolTip",
		igdeEnvironment::efpltAnimation){}
	
	virtual ~aeMCPAnimatorAnimation() override{}
	
	const decString &GetPropertyValue(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetAnimationPath();
	}
	
	void SetPropertyValue(const igdeMetaContext::Ref &context, const decString &value) override{
		Animator(context).SetAnimationPath(value);
	}
	
	const decString &GetPropertyBasePath(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetDirectoryPath();
	}
};


/** Affected bones. */
class aeMCPAnimatorAffectedBones : public aeTMCPAnimator<igdeMetaPropertyStringSet>{
public:
	aeMCPAnimatorAffectedBones() : aeTMCPAnimator("animator.affectedBones",
		"@Animator.WPAnimator.AffectedBones", "@Animator.WPAnimator.AffectedBones.ToolTip"){
			SetMultiSelection(true);
		}
	
	~aeMCPAnimatorAffectedBones() override{}
	
	decStringSet GetPropertyValue(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetListBones();
	}
	
	void SetPropertyValue(const igdeMetaContext::Ref &context, const decStringSet &value) override{
		Animator(context).SetListBones(value);
	}
	
	decString GetActiveString(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetActiveBone();
	}
	
	void SetActiveString(const igdeMetaContext::Ref &context, const decString &activeString) override{
		Animator(context).SetActiveBone(activeString);
	}
	
	decStringSet GetSelection(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetSelectedBones();
	}
	
	void SetSelection(const igdeMetaContext::Ref &context, const decStringSet &selection) override{
		Animator(context).SetSelectedBones(selection);
	}
	
	void GetStringItemInfo(const decString &string, igdeMetaContextItemInfo &info) const override{
		info.SetAll(string);
	}
	
	decStringSet GetValidStrings(const igdeMetaContext::Ref &context) const override;
	
	igdeAction::Ref CreateButtonAction(TargetButton target, const igdeMetaContext::Ref &context, igdeWidget &owner) override;
	void AddContextMenuEntries(igdeMenuCascade &contextMenu, const igdeMetaContext::Ref &context, igdeWidget &owner) override;
};


/** Affected vertex position sets. */
class aeMCPAnimatorAffectedVertexPositionSets : public aeTMCPAnimator<igdeMetaPropertyStringSet>{
public:
	aeMCPAnimatorAffectedVertexPositionSets() : aeTMCPAnimator("animator.affectedVertexPositionSets",
		"@Animator.WPAnimator.AffectedVertexPositionSets", "@Animator.WPAnimator.AffectedVertexPositionSets.ToolTip"){
			SetMultiSelection(true);
		}
	
	~aeMCPAnimatorAffectedVertexPositionSets() override{}
	
	decStringSet GetPropertyValue(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetListVertexPositionSets();
	}
	
	void SetPropertyValue(const igdeMetaContext::Ref &context, const decStringSet &value) override{
		Animator(context).SetListVertexPositionSets(value);
	}
	
	decString GetActiveString(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetActiveVertexPositionSet();
	}
	
	void SetActiveString(const igdeMetaContext::Ref &context, const decString &activeString) override{
		Animator(context).SetActiveVertexPositionSet(activeString);
	}
	
	void GetStringItemInfo(const decString &string, igdeMetaContextItemInfo &info) const override{
		info.SetAll(string);
	}
	
	igdeAction::Ref CreateButtonAction(TargetButton target, const igdeMetaContext::Ref &context, igdeWidget &owner) override;
	void AddContextMenuEntries(igdeMenuCascade &contextMenu, const igdeMetaContext::Ref &context, igdeWidget &owner) override;
};

#endif
