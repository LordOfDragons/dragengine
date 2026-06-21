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
#include <deigde/meta/property/igdeMetaPropertyGroup.h>


/** Hidden. */
class aeMCPHiddenBoneNames : public aeTMCPAnimator<igdeMetaPropertyStringSetStorage>{
public:
	aeMCPHiddenBoneNames() : aeTMCPAnimator("animator.hiddenBoneNames", "", ""){}
	~aeMCPHiddenBoneNames() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).hiddenBoneNames;
	}
};

class aeMCPHiddenVPSNames : public aeTMCPAnimator<igdeMetaPropertyStringSetStorage>{
public:
	aeMCPHiddenVPSNames() : aeTMCPAnimator("animator.hiddenVPSNames", "", ""){}
	~aeMCPHiddenVPSNames() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).hiddenVPSNames;
	}
};

class aeMCPHiddenMoveNames : public aeTMCPAnimator<igdeMetaPropertyStringSetStorage>{
public:
	aeMCPHiddenMoveNames() : aeTMCPAnimator("animator.hiddenMoveNames", "", ""){}
	~aeMCPHiddenMoveNames() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).hiddenMoveNames;
	}
};


/** Rig path. */
class aeMCPAnimatorRig : public aeTMCPAnimator<igdeMetaPropertyPathStorage>{
public:
	aeMCPAnimatorRig() : aeTMCPAnimator("animator.rig", "Animator.WPAnimator.Rig", igdeEnvironment::efpltRig){}
	~aeMCPAnimatorRig() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).rigPath;
	}
	
	decString GetPropertyBasePath(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetDirectoryPath();
	}
};


/** Animation path. */
class aeMCPAnimatorAnimation : public aeTMCPAnimator<igdeMetaPropertyPathStorage>{
public:
	aeMCPAnimatorAnimation() : aeTMCPAnimator("animator.animation", "Animator.WPAnimator.Animation", igdeEnvironment::efpltAnimation){}
	~aeMCPAnimatorAnimation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).animationPath;
	}
	
	decString GetPropertyBasePath(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetDirectoryPath();
	}
};


/** Affected bones. */
class aeMCPAnimatorAffectedBones : public aeTMCPAnimator<igdeMetaPropertyStringSetStorage>{
public:
	aeMCPAnimatorAffectedBones() : aeTMCPAnimator("animator.affectedBones", "Animator.WPAnimator.AffectedBones"){
		SetMultiSelection(true);
		SetRows(5);
	}
	
	~aeMCPAnimatorAffectedBones() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).affectedBones;
	}
	
	decStringSet GetAllowedStrings(const igdeMetaContext::Ref &context) const override;
	void AddContextMenuEntries(igdeMenuCascade &contextMenu, const igdeMetaContext::Ref &context, igdeWidget &owner) override;
};


/** Affected vertex position sets. */
class aeMCPAnimatorAffectedVertexPositionSets : public aeTMCPAnimator<igdeMetaPropertyStringSetStorage>{
public:
	aeMCPAnimatorAffectedVertexPositionSets() : aeTMCPAnimator("animator.affectedVertexPositionSets", "Animator.WPAnimator.AffectedVertexPositionSets"){
		SetMultiSelection(true);
		SetRows(5);
	}
	
	~aeMCPAnimatorAffectedVertexPositionSets() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).affectedVertexPositionSets;
	}
	
	decStringSet GetAllowedStrings(const igdeMetaContext::Ref &context) const override;
	void AddContextMenuEntries(igdeMenuCascade &contextMenu, const igdeMetaContext::Ref &context, igdeWidget &owner) override;
};

#endif
