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
#include "../meWindowMain.h"
#include "../../world/meWorld.h"
#include "../../world/terrain/meHeightTerrain.h"
#include "../../world/terrain/meHeightTerrainSector.h"
#include "../../world/terrain/meHeightTerrainTexture.h"
#include "../../world/terrain/meHeightTerrainNavSpace.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/localization/igdeTranslationManager.h>

#include <dragengine/common/exceptions.h>



// Class meWCEntry
////////////////////

// Constructor, destructor
////////////////////////////

meWCEntry::meWCEntry(meWindowChangelog &windowChangelog, eElementTypes type) :
igdeListItem(""),
pWindowChangelog(windowChangelog),
pType(type)
{
	if(type < eetWorld || type > eetHTPFCache){
		DETHROW(deeInvalidParam);
	}
}

meWCEntry::~meWCEntry(){
	SetHTNavSpace(nullptr);
	SetHTTexture(nullptr);
	SetWorld(nullptr);
}



// Management
///////////////

void meWCEntry::SetSector(const decPoint3 &sector){
	pSector = sector;
}

void meWCEntry::SetWorld(meWorld *world){
	pWorld = world;
}

void meWCEntry::SetHTTexture(meHeightTerrainTexture *texture){
	pHTTexture = texture;
}

void meWCEntry::SetHTNavSpace(meHeightTerrainNavSpace *navspace){
	pHTNavSpace = navspace;
}



void meWCEntry::UpdateText(){
	const char * const textUnsaved = "< not saved yet >";
	const char * const textRemoved = "< removed >";
	decStringList &details = GetDetails();
	decString text;
	
	details.RemoveAll();
	
	switch(pType){
	case eetWorld:
		text = "-";
		details.Add(pWorld->GetWindowMain().Translate("World.WCEntry.World").ToUTF8());
		
		if(pWorld->GetSaved()){
			details.Add(pWorld->GetFilePath());
			
		}else{
			details.Add(textUnsaved);
		}
		break;
		
	case eetHeightTerrain: {
		text = "-";
		details.Add(pWorld->GetWindowMain().Translate("World.WCEntry.HeightTerrain").ToUTF8());
		
		if(pWorld->GetHeightTerrain()->GetSaved()){
			if(pWorld->GetHeightTerrain()->GetPathHT().IsEmpty()){
				details.Add(textRemoved);
				
			}else{
				details.Add(pWorld->GetHeightTerrain()->GetPathHT());
			}
			
		}else{
			details.Add(textUnsaved);
		}
		} break;
		
	case eetHTHeight: {
		const meHeightTerrainSector * const htsector =
			pWorld->GetHeightTerrain()->GetSectorWith(decPoint(pSector.x, pSector.z));
		
		text.Format("(%d,%d)", pSector.x, pSector.z);
		details.Add(pWorld->GetWindowMain().Translate("World.WCEntry.HeightImage").ToUTF8());
		
		if(htsector && htsector->GetHeightImageSaved()){
			if(htsector->GetPathHeightImage().IsEmpty()){
				details.Add(textRemoved);
				
			}else{
				details.Add(htsector->GetPathHeightImage());
			}
			
		}else{
			details.Add(textUnsaved);
		}
		} break;
		
	case eetHTVisibility: {
		const meHeightTerrainSector * const htsector =
			pWorld->GetHeightTerrain()->GetSectorWith(decPoint(pSector.x, pSector.z));
		
		text.Format("(%d,%d)", pSector.x, pSector.z);
		details.Add(pWorld->GetWindowMain().Translate("World.WCEntry.VisibilityImage").ToUTF8());
		
		if(htsector && htsector->GetVisibilitySaved()){
			if(htsector->GetPathVisibilityImage().IsEmpty()){
				details.Add(textRemoved);
				
			}else{
				details.Add(htsector->GetPathVisibilityImage());
			}
			
		}else{
			details.Add(textUnsaved);
		}
		} break;
		
	case eetHTTextureMask:
		text.Format("(%d,%d)", pSector.x, pSector.z);
		details.Add(pWorld->GetWindowMain().Translate("World.WCEntry.TextureMask").ToUTF8());
		
		if(pHTTexture->GetMaskSaved()){
			if(pHTTexture->GetPathMask().IsEmpty()){
				details.Add(textRemoved);
				
			}else{
				details.Add(pHTTexture->GetPathMask());
			}
			
		}else{
			details.Add(textUnsaved);
		}
		break;
		
	case meWCEntry::eetHTNavSpace:
		text = "-";
		details.Add(pWorld->GetWindowMain().Translate("World.WCEntry.NavigationSpace").ToUTF8());
		
		if(pHTNavSpace->GetNavSpaceSaved()){
			if(pHTNavSpace->GetPathNavSpace().IsEmpty()){
				details.Add(textRemoved);
				
			}else{
				details.Add(pHTNavSpace->GetPathNavSpace());
			}
			
		}else{
			details.Add(textUnsaved);
		}
		break;
		
	case eetHTPFCache: {
		const meHeightTerrainSector * const htsector =
			pWorld->GetHeightTerrain()->GetSectorWith(decPoint(pSector.x, pSector.z));
		
		text.Format("(%d,%d)", pSector.x, pSector.z);
		details.Add(pWorld->GetWindowMain().Translate("World.WCEntry.PropFieldCache").ToUTF8());
		
		if(htsector && htsector->GetPFCacheSaved()){
			if(htsector->GetPathPFCacheImage().IsEmpty()){
				details.Add(textRemoved);
				
			}else{
				details.Add(htsector->GetPathPFCacheImage());
			}
			
		}else{
			details.Add(textUnsaved);
		}
		} break;
	}
	
	SetText(text);
}
