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

#include "aeMCPShared.h"


// Class aeActionMirrorStringSet
//////////////////////////////////

aeActionMirrorStringSet::aeActionMirrorStringSet(igdeMetaPropertyStringSetStorage &property,
	igdeWidget &owner, const igdeMetaContext::Ref &context) :
igdeMetaProperty::Action(owner, context, "", nullptr, ""),
pProperty(property)
{
	pMatchNames.Add({".left", ".right", deAnimatorRuleMirror::emntLast});
	pMatchNames.Add({" left", " right", deAnimatorRuleMirror::emntLast});
	pMatchNames.Add({".L", ".R", deAnimatorRuleMirror::emntLast});
	pMatchNames.Add({" L", " R", deAnimatorRuleMirror::emntLast});
	pMatchNames.Add({".l", ".r", deAnimatorRuleMirror::emntLast});
	pMatchNames.Add({" l", " r", deAnimatorRuleMirror::emntLast});
}

void aeActionMirrorStringSet::OnAction(){
	// TODO add a dialog to allow changing the mirror parameter (or add a new menu command)
	const auto &context = GetContext();
	if(!pProperty.IsValid(context)){
		return;
	}
	
	const auto oldValue = pProperty.GetPropertyValue(context);
	decStringSet newValue, selected;
	
	if(pSelected){
		selected = pProperty.GetSelection(context);
		newValue = oldValue - selected;
		selected = Mirror(selected);
		newValue += selected;
		
	}else{
		newValue = Mirror(oldValue);
	}
	
	if(newValue == oldValue){
		return;
	}
	
	pProperty.ChangePropertyValue(context, newValue, BuildUndoInfo(pProperty));
	
	if(pSelected && selected.IsNotEmpty()){
		pProperty.SetActiveString(context, igdeMetaPropertyStringSet::StringRef::New(selected.GetAt(0)));
		pProperty.SetSelection(context, selected);
	}
}

void aeActionMirrorStringSet::Update(){
	SetEnabled(pProperty.IsValid(GetContext()) && pProperty.GetPropertyValue(GetContext()).IsNotEmpty()
		&& (!pSelected || pProperty.GetActiveString(GetContext())));
}

decStringSet aeActionMirrorStringSet::Mirror(const decStringSet &strings) const{
	decStringSet mirrored;
	strings.Visit([&](const decString &string){
		if(!pMatchNames.HasMatching([&](const deAnimatorRuleMirror::sMatchName &matchName){
			switch(matchName.type){
			case deAnimatorRuleMirror::emntFirst:
				if(string.BeginsWith(matchName.first)){
					mirrored.Add(matchName.second + string.GetMiddle(matchName.first.GetLength()));
					return true;
					
				}else if(string.BeginsWith(matchName.second)){
					mirrored.Add(matchName.first + string.GetMiddle(matchName.second.GetLength()));
					return true;
				}
				break;
				
			case deAnimatorRuleMirror::emntLast:
				if(string.EndsWith(matchName.first)){
					mirrored.Add(string.GetMiddle(0, -matchName.first.GetLength()) + matchName.second);
					return true;
					
				}else if(string.EndsWith(matchName.second)){
					mirrored.Add(string.GetMiddle(0, -matchName.second.GetLength()) + matchName.first);
					return true;
				}
				break;
				
			case deAnimatorRuleMirror::emntMiddle:
				if(int index = string.FindString(matchName.first); index != -1){
					mirrored.Add(string.GetMiddle(0, index) + matchName.second
						+ string.GetMiddle(index + matchName.first.GetLength()));
					return true;
					
				}else if(index = string.FindString(matchName.second); index != -1){
					mirrored.Add(string.GetMiddle(0, index) + matchName.first
						+ string.GetMiddle(index + matchName.second.GetLength()));
					return true;
				}
				break;
			}
			return false;
			
		})){
			mirrored.Add(string);
		}
	});
	return mirrored;
}
