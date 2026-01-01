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

#include "meWCEntry.h"
#include "meWindowChangelog.h"
#include "meWindowChangelogListener.h"
#include "../meWindowMain.h"
#include "../../world/meWorld.h"
#include "../../world/terrain/meHeightTerrain.h"
#include "../../world/terrain/meHeightTerrainSector.h"
#include "../../world/terrain/meHeightTerrainTexture.h"
#include "../../world/terrain/meHeightTerrainNavSpace.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/igdeApplication.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeIconListBox.h>
#include <deigde/gui/event/igdeIconListBoxListener.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/gui/model/igdeListItemSorter.h>

#include <dragengine/common/exceptions.h>



// Sorting
////////////

namespace {

class cChangelogSorter : public igdeListItemSorter{
public:
	typedef deTObjectReference<cChangelogSorter> Ref;
	
	cChangelogSorter(){}
	
	virtual bool Precedes(const igdeListItem &item1, const igdeListItem &item2){
		const meWCEntry &entry1 = (const meWCEntry &)item1;
		const meWCEntry &entry2 = (const meWCEntry &)item2;
		const bool worldType1 = entry1.GetType() == meWCEntry::eetWorld;
		const bool worldType2 = entry2.GetType() == meWCEntry::eetWorld;
		
		if(worldType1 && !worldType2){
			return true;
		}
		if(worldType2 && !worldType1){
			return false;
		}
		
		const decPoint3 difference(entry2.GetSector() - entry1.GetSector());
		if(difference.x < 0){
			return true;
			
		}else if(difference.x > 0){
			return false;
			
		}else if(difference.y < 0){
			return true;
			
		}else if(difference.y > 0){
			return false;
			
		}else{
			return difference.z <= 0;
		}
	}
};

class cListChangelog : public igdeIconListBoxListener{
	meWindowChangelog &pWindow;
	
public:
	typedef deTObjectReference<cListChangelog> Ref;
	cListChangelog(meWindowChangelog &window) : pWindow(window){}
	
	virtual void AddContextMenuEntries(igdeIconListBox*, igdeMenuCascade&){
		(void)pWindow;
	}
};

}



// Class meWindowChangelog
/////////////////////////////

// Constructor, destructor
////////////////////////////

meWindowChangelog::meWindowChangelog(meWindowMain &windowMain) :
igdeContainerBorder(windowMain.GetEnvironment()),
pWindowMain(windowMain)
{
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	pListener = meWindowChangelogListener::Ref::New(*this);
	
	igdeUIHelper::sColumnHeader headers[3] = {
		igdeUIHelper::sColumnHeader("Sector", nullptr, igdeApplication::app().DisplayScaled(50)),
		igdeUIHelper::sColumnHeader("What", nullptr, igdeApplication::app().DisplayScaled(150)),
		igdeUIHelper::sColumnHeader("Filename", nullptr, igdeApplication::app().DisplayScaled(380))};
	helper.IconListBox(
		igdeApplication::app().DisplayScaled(decPoint(100, 150)),
		headers, 3, "Changes", pListChanges, cListChangelog::Ref::New(*this));
	AddChild(pListChanges, igdeContainerBorder::eaCenter);
	
	pListChanges->SetSorter(cChangelogSorter::Ref::New());
}

meWindowChangelog::~meWindowChangelog(){
	if(pWorld){
		pWorld->RemoveNotifier(pListener);
		pWorld = nullptr;
	}
}



// Management
///////////////

void meWindowChangelog::SetWorld(meWorld *world){
	if(world == pWorld){
		return;
	}
	
	pListChanges->RemoveAllItems();
	
	if(pWorld){
		pWorld->RemoveNotifier(pListener);
	}
	
	pWorld = world;
	
	if(world){
		world->AddNotifier(pListener);
	}
	
	UpdateChangelog();
}



void meWindowChangelog::UpdateChangelog(){
	pListChanges->RemoveAllItems();
	
	if(!pWorld){
		return;
	}
	
	meHeightTerrain * const hterrain = pWorld->GetHeightTerrain();
	
	try{
		if(pWorld->GetChanged()){
			const igdeListItem::Ref refEntry(meWCEntry::Ref::New(*this, meWCEntry::eetWorld));
			meWCEntry &entry = (meWCEntry&)(igdeListItem&)refEntry;
			entry.SetWorld(pWorld);
			entry.UpdateText();
			
			pListChanges->AddItem(refEntry);
		}
		
		if(pWorld->GetDepChanged()){
			if(pWorld->GetHeightTerrain()->GetChanged()){
				const igdeListItem::Ref refEntry(meWCEntry::Ref::New(*this, meWCEntry::eetHeightTerrain));
				meWCEntry &entry = (meWCEntry&)(igdeListItem&)refEntry;
				entry.SetWorld(pWorld);
				entry.UpdateText();
				pListChanges->AddItem(refEntry);
			}
			
			hterrain->GetSectors().Visit([&](meHeightTerrainSector &htsector){
				const decPoint &scoord = htsector.GetCoordinates();
				
				if(htsector.GetHeightImageChanged()){
					const igdeListItem::Ref refEntry(meWCEntry::Ref::New(*this, meWCEntry::eetHTHeight));
					meWCEntry &entry = (meWCEntry&)(igdeListItem&)refEntry;
					entry.SetWorld(pWorld);
					entry.SetSector(decPoint3(scoord.x, 0, scoord.y));
					entry.UpdateText();
					pListChanges->AddItem(refEntry);
				}
				
				if(htsector.GetVisibilityChanged()){
					const igdeListItem::Ref refEntry(meWCEntry::Ref::New(*this, meWCEntry::eetHTVisibility));
					meWCEntry &entry = (meWCEntry&)(igdeListItem&)refEntry;
					entry.SetWorld(pWorld);
					entry.SetSector(decPoint3(scoord.x, 0, scoord.y));
					entry.UpdateText();
					pListChanges->AddItem(refEntry);
				}
				
				if(htsector.GetPFCacheChanged()){
					const igdeListItem::Ref refEntry(meWCEntry::Ref::New(*this, meWCEntry::eetHTPFCache));
					meWCEntry &entry = (meWCEntry&)(igdeListItem&)refEntry;
					entry.SetWorld(pWorld);
					entry.SetSector(decPoint3(scoord.x, 0, scoord.y));
					entry.UpdateText();
					pListChanges->AddItem(refEntry);
				}
				
				if(pWorld->GetHeightTerrain()->GetDepChanged()){
					htsector.GetTextures().Visit([&](meHeightTerrainTexture &httexture){
						if(!httexture.GetMaskChanged()){
							return;
						}
						
						const igdeListItem::Ref refEntry(meWCEntry::Ref::New(*this, meWCEntry::eetHTTextureMask));
						meWCEntry &entry = (meWCEntry&)(igdeListItem&)refEntry;
						entry.SetHTTexture(&httexture);
						entry.SetSector(decPoint3(scoord.x, 0, scoord.y));
						entry.UpdateText();
						pListChanges->AddItem(refEntry);
					});
				}
				
				htsector.GetNavSpaces().Visit([&](meHeightTerrainNavSpace *navspace){
					if(!navspace->GetNavSpaceChanged()){
						return;
					}
					
					const igdeListItem::Ref refEntry(meWCEntry::Ref::New(*this, meWCEntry::eetHTNavSpace));
					meWCEntry &entry = (meWCEntry&)(igdeListItem&)refEntry;
					entry.SetHTNavSpace(navspace);
					entry.SetSector(decPoint3(scoord.x, 0, scoord.y));
					entry.UpdateText();
					pListChanges->AddItem(refEntry);
				});
			});
		}
		
	}catch(const deException &e){
		pWindowMain.DisplayException(e);
	}
	
	pListChanges->SortItems();
}



void meWindowChangelog::SaveEntry(meWCEntry*){
}

void meWindowChangelog::SaveAllEntries(){
}
