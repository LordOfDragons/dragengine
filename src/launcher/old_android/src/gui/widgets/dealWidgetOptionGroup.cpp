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
 
#include <stdio.h>

#include "dealWidgetOptionGroup.h"
#include "dealWidgetOptionBox.h"
#include "../../common/exceptions.h"




// Class dealWidgetOptionGroup
////////////////////////////////

// Constructors, destructors
//////////////////////////////

dealWidgetOptionGroup::dealWidgetOptionGroup(){
}

dealWidgetOptionGroup::~dealWidgetOptionGroup(){
	RemoveAll();
}



// Management
///////////////

int dealWidgetOptionGroup::GetCount() const{
	return pWidgets.GetCount();
}

dealWidgetOptionBox *dealWidgetOptionGroup::GetAt(int index) const{
	return (dealWidgetOptionBox*)pWidgets.GetAt(index);
}

void dealWidgetOptionGroup::Add(dealWidgetOptionBox *widget){
	if(!widget || pWidgets.Has(widget)){
		DETHROW(deeInvalidParam);
	}
	
	if(GetSelected()){
		widget->SetSelected(false);
	}
	
	pWidgets.Add(widget);
	//widget->SetGroup( this );
}

void dealWidgetOptionGroup::Remove(dealWidgetOptionBox *widget){
	if(!pWidgets.Has(widget)){
		DETHROW(deeInvalidParam);
	}
	
	//widget->SetGroup( NULL );
	pWidgets.Remove(widget);
}

void dealWidgetOptionGroup::RemoveAll(){
	while(pWidgets.GetCount() > 0){
		dealWidgetOptionBox * const widget = (dealWidgetOptionBox*)pWidgets.GetAt(pWidgets.GetCount() - 1);
		//widget->SetGroup( NULL );
		pWidgets.Remove(widget);
	}
}

dealWidgetOptionBox *dealWidgetOptionGroup::GetSelected() const{
	const int count = pWidgets.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		dealWidgetOptionBox * const option = (dealWidgetOptionBox*)pWidgets.GetAt(i);
		if(option->GetSelected()){
			return option;
		}
	}
	
	return NULL;
}

void dealWidgetOptionGroup::Select(dealWidgetOptionBox *option){
	const int count = pWidgets.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		dealWidgetOptionBox * const option2 = (dealWidgetOptionBox*)pWidgets.GetAt(i);
		option2->SetSelected(option2 == option);
	}
}
