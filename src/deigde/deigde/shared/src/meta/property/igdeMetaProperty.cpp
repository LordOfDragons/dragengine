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

#include "igdeMetaProperty.h"
#include "../igdeMetaContext.h"
#include "../../environment/igdeEnvironment.h"
#include "../../localization/igdeTranslationManager.h"


// Class igdeMetaProperty::Action
///////////////////////////////////

igdeMetaProperty::Action::~Action() = default;

void igdeMetaProperty::Action::SetContext(const ContextRef &context){
	pContext = context;
}

igdeEnvironment &igdeMetaProperty::Action::GetEnvironment() const{
	return pOwner.GetEnvironment();
}

igdeTranslationManager &igdeMetaProperty::Action::GetTranslationManager() const{
	return GetEnvironment().GetTranslationManager();
}

decUnicodeString igdeMetaProperty::Action::Translate(const decString &entryName) const{
	return GetTranslationManager().Translate(entryName);
}

decUnicodeString igdeMetaProperty::Action::Translate(const char *entryName) const{
	return GetTranslationManager().Translate(entryName);
}

decUnicodeString igdeMetaProperty::Action::TranslateIf(const decString &text) const{
	return GetTranslationManager().TranslateIf(text);
}

decUnicodeString igdeMetaProperty::Action::TranslateIf(const char *text) const{
	return GetTranslationManager().TranslateIf(text);
}

decString igdeMetaProperty::Action::BuildUndoInfo(const igdeMetaProperty &property) const{
	return property.RealUndoInfo(pContext, *this);
}


// Class igdeMetaProperty::ActionHelp
///////////////////////////////////////

igdeMetaProperty::ActionHelp::ActionHelp(igdeEnvironment &environment, const igdeMetaProperty &property) :
igdeAction("@Igde.MetaProperty.Action.Help",
	environment.GetStockIcon(igdeEnvironment::esiSearch),
	"@Igde.MetaProperty.Action.Help.ToolTip"),
pEnvironment(environment),
pProperty(property){
}

igdeMetaProperty::ActionHelp::~ActionHelp() = default;

void igdeMetaProperty::ActionHelp::OnAction(){
	auto url = pProperty.GetHelpUrl();
	if(!url.IsEmpty()){
		pEnvironment.OpenUrl(pProperty.GetHelpUrl());
	}
}


// Class igdeMetaProperty
///////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaProperty::igdeMetaProperty(const char *id, const char *name, const char *description) :
pId(id),
pLabel(name),
pDescription(description){
}

igdeMetaProperty::igdeMetaProperty(const char *id, const char *translationTag) :
pId(id),
pLabel(decString::Formatted("@{0}", translationTag)),
pDescription(decString::Formatted("@{0}.ToolTip", translationTag)),
pFilter(decString::Formatted("@{0}.FilterString", translationTag)),
pUndoInfo(decString::Formatted("@{0}.UndoInfo", translationTag)){
}

igdeMetaProperty::~igdeMetaProperty() = default;


// Management
///////////////

void igdeMetaProperty::SetLabel(const char *label){
	pLabel = label;
}

void igdeMetaProperty::SetDescription(const char *description){
	pDescription = description;
}

void igdeMetaProperty::SetFilter(const char *filter){
	pFilter = filter;
}

void igdeMetaProperty::SetUndoInfo(const char *undoInfo){
	pUndoInfo = undoInfo;
}

void igdeMetaProperty::SetHideLabel(bool hideLabel){
	pHideLabel = hideLabel;
}

void igdeMetaProperty::SetCanHideGroup(bool canHideGroup){
	pCanHideGroup = canHideGroup;
}

void igdeMetaProperty::SetClipboardDataTypeName(const char *name){
	pClipboardDataTypeName = name;
}

void igdeMetaProperty::SetHelpUrl(const char *url){
	pHelpUrl = url;
}


decString igdeMetaProperty::RealFilter(const igdeMetaContext &context) const{
	const auto &tm = context.GetEnvironment().GetTranslationManager();
	if(pFilter.IsEmpty()){
		return tm.TranslateIf(pLabel).ToUTF8();
	}
	
	auto filter = tm.TranslateIf(pFilter, decUnicodeString());
	return (filter.IsEmpty() ? tm.TranslateIf(pLabel) : filter).ToUTF8();
}

decString igdeMetaProperty::RealUndoInfo(const igdeMetaContext &context) const{
	const auto &tm = context.GetEnvironment().GetTranslationManager();
	if(pUndoInfo.IsEmpty()){
		return tm.TranslateIf(pLabel).ToUTF8();
	}
	
	const auto undo = tm.TranslateIf(pUndoInfo, decUnicodeString());
	return (undo.IsEmpty() ? tm.TranslateIf(pLabel) : undo).ToUTF8();
}

decString igdeMetaProperty::RealUndoInfo(const igdeMetaContext &context, const igdeAction &action) const{
	return RealUndoInfo(context, action.GetText());
}

decString igdeMetaProperty::RealUndoInfo(const igdeMetaContext &context, const char *text) const{
	return RealUndoInfo(context) + ": " + context.GetEnvironment().
		GetTranslationManager().TranslateIf(text).ToUTF8();
}


void igdeMetaProperty::SetWidgetState(const igdeMetaPropertyWidgetState::Ref &widgetState){
	pWidgetState = widgetState;
}


void igdeMetaProperty::AddContextMenuEntries(igdeMenuCascade&, const igdeMetaContext::Ref&, igdeWidget&){
}
