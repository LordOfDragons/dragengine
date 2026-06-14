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

#ifndef _AEMCPCONTROLLER_H_
#define _AEMCPCONTROLLER_H_

#include "aeTMCPAnimator.h"
#include "../../animator/aeAnimator.h"
#include "../../animator/controller/aeController.h"

#include <deigde/meta/igdeMetaContextItemInfo.h>
#include <deigde/meta/property/igdeMetaPropertyList.h>
#include <deigde/meta/property/undo/igdeMetaPropertyListUndo.h>


/** Controllers. */
class aeMCPControllers : public aeTMCPAnimator<igdeMetaPropertyListTypeStorage<aeController, aeController::List>>{
public:
	aeMCPControllers() : aeTMCPAnimator("animator.controllers",
	"@Animator.WPController.Controllers", "@Animator.WPController.Controllers.ToolTip"){
		SetHideLabel(true);
	}
	
	~aeMCPControllers() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).controllers;
	}
	
	void GetObjectItemInfoType(const ContextRef&, const ObjectTypeRef &controller, igdeMetaContextItemInfo &info) const override{
		info.SetAll(decString::Formatted("{0}: {1}", controller->GetIndex(), controller->GetName()));
	}
	
	igdeMetaPropertyListUndo::Ref ChangePropertyValue(const ContextRef &context, const List &newValue,
		const char *undoInfo = nullptr, const char *undoInfoLong = nullptr) override;
	
	igdeAction::Ref CreateButtonAction(TargetButton target, const ContextRef &context, igdeWidget &owner) override;
	void AddContextMenuEntries(igdeMenuCascade &contextMenu, const ContextRef &context, igdeWidget &owner) override;
	
};

#endif
