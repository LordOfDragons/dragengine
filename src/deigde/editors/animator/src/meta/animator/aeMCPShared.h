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

#ifndef _AEMCPSHARED_H_
#define _AEMCPSHARED_H_

#include <deigde/meta/property/igdeMetaPropertyStringSet.h>

#include <dragengine/common/collection/decTList.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleMirror.h>


/**
 * Mirror string set.
 * 
 * The default match names are:
 * - ".left", ".right", deAnimatorRuleMirror::emntLast
 * - " left", " right", deAnimatorRuleMirror::emntLast
 * - ".L", ".R", deAnimatorRuleMirror::emntLast
 * - " L", " R", deAnimatorRuleMirror::emntLast
 * - ".l", ".r", deAnimatorRuleMirror::emntLast
 * - " l", " r", deAnimatorRuleMirror::emntLast
 */
class aeActionMirrorStringSet : public igdeMetaProperty::Action{
public:
	using MatchNameList = decTList<deAnimatorRuleMirror::sMatchName>;
	
private:
	igdeMetaPropertyStringSetStorage &pProperty;
	
protected:
	MatchNameList pMatchNames;
	bool pSelected = false;
	
public:
	aeActionMirrorStringSet(igdeMetaPropertyStringSetStorage &property,
		igdeWidget &owner, const igdeMetaContext::Ref &context);
	
	void OnAction() override;
	void Update() override;
	
	decStringSet Mirror(const decStringSet &strings) const;
};

#endif
