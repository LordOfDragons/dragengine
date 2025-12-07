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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "igdeGDAddToListVisitor.h"
#include "../class/igdeGDClass.h"
#include "../preview/igdeGDPreviewManager.h"
#include "../skin/igdeGDSkin.h"
#include "../sky/igdeGDSky.h"
#include "../../environment/igdeEnvironment.h"
#include "../../gui/igdeIconListBox.h"
#include "../../gui/browse/igdeBrowseItemGDPreviewListener.h"
#include "../../gui/model/igdeListItem.h"
#include "../../gui/resources/igdeIcon.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>



// Class igdeGDAddToListVisitor
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDAddToListVisitor::igdeGDAddToListVisitor(igdeEnvironment &environment,
	igdeIconListBox *iconListBox, int size) :
pEnvironment(environment),
pIconListBox(iconListBox),
pSize(size){
}

igdeGDAddToListVisitor::~igdeGDAddToListVisitor(){
}



// Management
///////////////

void igdeGDAddToListVisitor::AddItemToList(const char *caption, deImage &image, void *userPointer){
	igdeListItem::Ref item;
	AddItemToList(item, caption, image, userPointer);
}

void igdeGDAddToListVisitor::AddItemToList(igdeListItem::Ref &item,
const char *caption, deImage &image, void *userPointer){
	if(!caption){
		DETHROW(deeInvalidParam);
	}
	
	/*
	const int maxCharPerLine = pSize / 4;
	const char *lastBlank = NULL;
	const char *line = caption;
	decString text;
	
	while(*caption){
		if(isblank(*caption)){
			lastBlank = caption;
		}
		
		if((int)(caption - line) >= maxCharPerLine){
			if(!text.IsEmpty()){
				text.AppendCharacter('\n');
			}
			
			decString appendText(line);
			
			if(lastBlank){
				appendText[(int)(lastBlank - line)] = '\0';
				line = lastBlank + 1;
				
			}else{
				appendText[(int)(caption - line)] = '\0';
				line = caption;
			}
			
			text.Append(appendText);
			
			// right now more than 1 line can not be drawn. cut it off
			text.Append("...");
			while(*caption){
				caption++;
			}
			line = caption;
			break;
		}
		
		caption++;
	}
	
	if(*line){
		if(!text.IsEmpty()){
			text.AppendCharacter('\n');
		}
		text.Append(line);
	}
	*/
	const char * const text = caption;
	
	igdeIcon::Ref icon;
	if(image.GetWidth() > image.GetHeight()){
		icon.TakeOver(new igdeIcon(image, pSize, pSize * image.GetHeight() / image.GetWidth()));
		
	}else{
		icon.TakeOver(new igdeIcon(image, pSize * image.GetWidth() / image.GetHeight(), pSize));
	}
	
	item.TakeOver(new igdeListItem(text, icon, userPointer));
	pIconListBox->AddItem(item);
}



// Visiting
/////////////

void igdeGDAddToListVisitor::VisitObjectClass(igdeGDClass *gdclass){
	if(!gdclass->GetCanInstantiate()){
		return;
	}
	
	if(pIconListBox->GetItemCount() >= 100){
		// too many items. FOX has problems with this. will be changed once we have a better system
		printf("Too Many List Items (%s:%d)\n", __FILE__, __LINE__);
		return;
	}
	
	deImage * const image = pEnvironment.GetGDPreviewManager()->GetPreviewObjectClass(gdclass);
	igdeListItem::Ref item;
	if(image){
		AddItemToList(item, gdclass->GetName(), *image, gdclass);
		return;
	}
	
	AddItemToList(item,  gdclass->GetName(), *pEnvironment.GetGDPreviewManager()->GetImageCreating(), gdclass);
	
	const igdeBrowseItemGDPreviewListener::Ref listener(igdeBrowseItemGDPreviewListener::Ref::NewWith(pIconListBox, item, pSize));
	pEnvironment.GetGDPreviewManager()->CreatePreviewObjectClass(
		gdclass, listener);
}

void igdeGDAddToListVisitor::VisitSkin(igdeGDSkin *gdskin){
	if(pIconListBox->GetItemCount() >= 100){
		// too many items. FOX has problems with this. will be changed once we have a better system
		printf("Too Many List Items (%s:%d)\n", __FILE__, __LINE__);
		return;
	}
	
	deImage * const image = pEnvironment.GetGDPreviewManager()->GetPreviewSkin(gdskin);
	igdeListItem::Ref item;
	if(image){
		AddItemToList(item, gdskin->GetName(), *image, gdskin);
		return;
	}
	
	AddItemToList(item,  gdskin->GetName(), *pEnvironment.GetGDPreviewManager()->GetImageCreating(), gdskin);
	
	const igdeBrowseItemGDPreviewListener::Ref listener(igdeBrowseItemGDPreviewListener::Ref::NewWith(pIconListBox, item, pSize));
	pEnvironment.GetGDPreviewManager()->CreatePreviewSkin(
		gdskin, listener);
}

void igdeGDAddToListVisitor::VisitSky(igdeGDSky *gdsky){
	if(pIconListBox->GetItemCount() >= 100){
		// too many items. FOX has problems with this. will be changed once we have a better system
		printf("Too Many List Items (%s:%d)\n", __FILE__, __LINE__);
		return;
	}
	
	deImage * const image = pEnvironment.GetGDPreviewManager()->GetPreviewSky(gdsky);
	igdeListItem::Ref item;
	if(image){
		AddItemToList(item, gdsky->GetName(), *image, gdsky);
		return;
	}
	
	AddItemToList(item,  gdsky->GetName(), *pEnvironment.GetGDPreviewManager()->GetImageCreating(), gdsky);
	
	const igdeBrowseItemGDPreviewListener::Ref listener(igdeBrowseItemGDPreviewListener::Ref::NewWith(pIconListBox, item, pSize));
	pEnvironment.GetGDPreviewManager()->CreatePreviewSky(
		gdsky, listener);
}
