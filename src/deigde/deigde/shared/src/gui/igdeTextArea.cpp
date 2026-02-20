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

#include "igdeTextArea.h"
#include "igdeContainer.h"
#include "igdeCommonDialogs.h"
#include "native/toolkit.h"
#include "event/igdeAction.h"
#include "event/igdeTextAreaListener.h"
#include "resources/igdeFont.h"
#include "resources/igdeTextStyle.h"
#include "resources/igdeTextSegment.h"
#include "theme/igdeGuiTheme.h"
#include "theme/propertyNames.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class igdeTextArea
///////////////////////

// Constructor, destructor
////////////////////////////

igdeTextArea::igdeTextArea(igdeEnvironment &environment, int columns, int rows,
const char *description) :
igdeWidget(environment),
pEnabled(true),
pColumns(columns),
pRows(rows),
pEditable(true),
pDescription(description),
pNativeTextArea(nullptr)
{
	if(columns < 1 || rows < 0){
		DETHROW(deeInvalidParam);
	}
}

igdeTextArea::igdeTextArea(igdeEnvironment &environment, int columns, int rows,
	bool editable, const char *description) :
igdeWidget(environment),
pEnabled(true),
pColumns(columns),
pRows(rows),
pEditable(editable),
pDescription(description),
pNativeTextArea(nullptr)
{
	if(columns < 1 || rows < 1){
		DETHROW(deeInvalidParam);
	}
}

igdeTextArea::~igdeTextArea(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeTextArea::SetEnabled(bool enabled){
	if(pEnabled == enabled){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}

void igdeTextArea::SetColumns(int columns){
	if(columns < 1){
		DETHROW(deeInvalidParam);
	}
	
	if(pColumns == columns){
		return;
	}
	
	pColumns = columns;
	OnColumnsChanged();
}

void igdeTextArea::SetRows(int rows){
	if(rows < 1){
		DETHROW(deeInvalidParam);
	}
	
	if(pRows == rows){
		return;
	}
	
	pRows = rows;
	OnRowsChanged();
}

void igdeTextArea::SetEditable(bool editable){
	if(pEditable == editable){
		return;
	}
	
	pEditable = editable;
	OnEditableChanged();
}

void igdeTextArea::SetDescription(const char *description){
	if(pDescription == description){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}



void igdeTextArea::AddStyle(igdeTextStyle *style){
	if(!style || pStyles.HasNamed(style->GetName())){
		DETHROW(deeInvalidParam);
	}
	
	pStyles.Add(style);
	StylesChanged();
}

void igdeTextArea::RemoveAllStyles(){
	if(pStyles.IsEmpty()){
		return;
	}
	
	pStyles.RemoveAll();
	StylesChanged();
}

void igdeTextArea::StylesChanged(){
	OnStylesChanged();
}



void igdeTextArea::SetText(const char *text){
	if(pText == text){
		return;
	}
	
	pSegments.RemoveAll();
	pText = text;
	OnTextChanged();
}

void igdeTextArea::AppendText(const char *text){
	pText += text;
	OnTextChanged();
}

void igdeTextArea::AppendText(const char *text, const char *style){
	AppendText(text, style, nullptr);
}

void igdeTextArea::AppendText(const char *text, const char *style, igdeAction *action){
	if(!style){
		DETHROW(deeInvalidParam);
	}
	
	if(style[0] == 0 && !action){
		AppendText(text);
		return;
	}
	
	const int begin = pText.GetLength();
	pText += text;
	const int end = pText.GetLength() - 1;
	
	pSegments.Add(igdeTextSegment::Ref::New(begin, end, style, action));
	
	OnTextChanged();
}

void igdeTextArea::DeleteText(int begin, int end){
	if(begin < 0 || begin > pText.GetLength() || end < begin){
		DETHROW(deeInvalidParam);
	}
	
	if(begin == end){
		return;
	}
	
	const int count = pSegments.GetCount();
	const int length = end - begin;
	int i;
	
	for(i=count-1; i>=0; i--){
		const igdeTextSegment &segment = pSegments.GetAt(i);
		
		if(segment.GetBegin() >= begin && segment.GetEnd() < end){
			pSegments.RemoveFrom(i);
			
		}else if(segment.GetBegin() < begin && segment.GetEnd() >= end){
			pSegments.SetAt(i, igdeTextSegment::Ref::New(segment.GetBegin(),
				segment.GetEnd() - length, segment.GetStyle(), segment.GetAction()));
			
		}else if(segment.GetBegin() < begin && segment.GetEnd() >= begin){
			pSegments.SetAt(i, igdeTextSegment::Ref::New(segment.GetBegin(),
				begin - 1, segment.GetStyle(), segment.GetAction()));
			
		}else if(segment.GetEnd() >= end && segment.GetBegin() < end){
			pSegments.SetAt(i, igdeTextSegment::Ref::New(end,
				segment.GetEnd(), segment.GetStyle(), segment.GetAction()));
			
		}else if(segment.GetBegin() >= end){
			pSegments.SetAt(i, igdeTextSegment::Ref::New(segment.GetBegin() - length,
				segment.GetEnd() - length, segment.GetStyle(), segment.GetAction()));
		}
	}
	
	pText = pText.GetLeft(begin) + pText.GetMiddle(end);
	OnTextChanged();
}

void igdeTextArea::ClearText(){
	if(pText.IsEmpty()){
		return;
	}
	
	pSegments.RemoveAll();
	pText.Empty();
	OnTextChanged();
}



int igdeTextArea::GetCursorPosition() const{
	return pNativeTextArea ? pNativeTextArea->GetCursorPosition() : 0;
}

void igdeTextArea::SetCursorPosition(int position){
	if(!pNativeTextArea){
		return;
	}
	
	if(position < 0 || position >= pText.GetLength()){
		DETHROW(deeInvalidParam);
	}
	
	pNativeTextArea->SetCursorPosition(position);
}

decPoint igdeTextArea::GetCursorCoordinate() const{
	return pNativeTextArea ? decPoint(pNativeTextArea->GetCursorColumn(),
		pNativeTextArea->GetCursorRow()) : decPoint();
}

void igdeTextArea::SetCursorCoordinate(const decPoint &coordinate){
	if(pNativeTextArea){
		pNativeTextArea->SetCursorRow(coordinate.y);
		pNativeTextArea->SetCursorColumn(coordinate.x);
	}
}



int igdeTextArea::GetTopLine() const{
	if(!pNativeTextArea){
		return 0;
	}
	
	return pNativeTextArea->GetTopLine();
}

void igdeTextArea::SetTopLine(int line){
	if(pNativeTextArea){
		pNativeTextArea->SetTopLine(line);
	}
}

int igdeTextArea::GetBottomLine() const{
	return pNativeTextArea ? pNativeTextArea->GetBottomLine() : 0;
}

void igdeTextArea::SetBottomLine(int line){
	if(pNativeTextArea){
		pNativeTextArea->SetBottomLine(line);
	}
}

int igdeTextArea::GetLineCount() const{
	return pNativeTextArea ? pNativeTextArea->GetLineCount() : 0;
}

void igdeTextArea::Focus(){
	if(pNativeTextArea){
		pNativeTextArea->Focus();
	}
}



const igdeTextSegment *igdeTextArea::GetSegmentWith(int offset) const{
	return pSegments.FindOrDefault([&](const igdeTextSegment &segment){
		return offset >= segment.GetBegin() && offset <= segment.GetEnd();
	});
}

void igdeTextArea::SetTextSegment(int begin, int end, const char *style, igdeAction *action){
	bool changed = pClearSegment(begin, end);
	
	if(style[0] || action){
		pSegments.Add(igdeTextSegment::Ref::New(begin, end, style, action));
		changed = true;
	}
	
	if(changed){
		OnTextChanged();
	}
}

void igdeTextArea::ClearTextSegment(int begin, int end){
	if(pClearSegment(begin, end)){
		OnTextChanged();
	}
}



void igdeTextArea::AddListener(igdeTextAreaListener *listener){
	DEASSERT_NOTNULL(listener)
	pListeners.Add(listener);
}

void igdeTextArea::RemoveListener(igdeTextAreaListener *listener){
	pListeners.Remove(listener);
}
void igdeTextArea::NotifyTextChanged(){
	const auto listeners(pListeners);
	listeners.Visit([&](igdeTextAreaListener &l){
		l.OnTextChanged(this);
	});
}

void igdeTextArea::NotifyTextChanging(){
	const auto listeners(pListeners);
	listeners.Visit([&](igdeTextAreaListener &l){
		l.OnTextChanging(this);
	});
}


void igdeTextArea::CreateNativeWidget(){
	if(GetNativeWidget()){
		return;
	}
	
	igdeNativeTextArea * const native = igdeNativeTextArea::CreateNativeWidget(*this);
	SetNativeWidget(native);
	pNativeTextArea = native;
	native->PostCreateNativeWidget();
}

void igdeTextArea::DestroyNativeWidget(){
	if(!GetNativeWidget()){
		return;
	}
	
	((igdeNativeTextArea*)GetNativeWidget())->DestroyNativeWidget();
	DropNativeWidget();
}

void igdeTextArea::DropNativeWidget(){
	pNativeTextArea = nullptr;
	igdeWidget::DropNativeWidget();
}


void igdeTextArea::OnTextChanged(){
	if(pNativeTextArea){
		pNativeTextArea->UpdateText();
	}
}

void igdeTextArea::OnColumnsChanged(){
	if(pNativeTextArea){
		pNativeTextArea->UpdateColumns();
	}
}

void igdeTextArea::OnRowsChanged(){
	if(pNativeTextArea){
		pNativeTextArea->UpdateRows();
	}
}

void igdeTextArea::OnEnabledChanged(){
	if(pNativeTextArea){
		pNativeTextArea->UpdateEnabled();
	}
}

void igdeTextArea::OnEditableChanged(){
	if(pNativeTextArea){
		pNativeTextArea->UpdateEditable();
	}
}

void igdeTextArea::OnDescriptionChanged(){
	if(pNativeTextArea){
		pNativeTextArea->UpdateDescription();
	}
}

void igdeTextArea::OnStylesChanged(){
	if(pNativeTextArea){
		pNativeTextArea->UpdateStyles();
	}
}

bool igdeTextArea::pClearSegment(int begin, int end){
	if(begin < 0 || end >= pText.GetLength()){
		DETHROW(deeInvalidParam);
	}
	
	const int count = pSegments.GetCount();
	bool changed = false;
	int i;
	
	for(i=count-1; i>=0; i--){
		const igdeTextSegment &segment = pSegments.GetAt(i);
		
		if(segment.GetBegin() >= begin && segment.GetEnd() <= end){
			pSegments.RemoveFrom(i);
			changed = true;
			
		}else if(segment.GetBegin() < begin && segment.GetEnd() > end){
			pSegments.Add(igdeTextSegment::Ref::New(end + 1, segment.GetEnd(),
				segment.GetStyle(), segment.GetAction()));
			pSegments.SetAt(i, igdeTextSegment::Ref::New(segment.GetBegin(), begin - 1,
				segment.GetStyle(), segment.GetAction()));
			changed = true;
			
		}else if(segment.GetBegin() < begin && segment.GetEnd() >= begin){
			pSegments.SetAt(i, igdeTextSegment::Ref::New(segment.GetBegin(), begin - 1,
				segment.GetStyle(), segment.GetAction()));
			changed = true;
			
		}else if(segment.GetBegin() <= end && segment.GetEnd() > end){
			pSegments.SetAt(i, igdeTextSegment::Ref::New(end + 1, segment.GetEnd(),
				segment.GetStyle(), segment.GetAction()));
			changed = true;
		}
		// otherwise segment is fully before start or after end so not affected
	}
	
	return changed;
}

void igdeTextArea::OnNativeWidgetLanguageChanged(){
	OnDescriptionChanged();
}
