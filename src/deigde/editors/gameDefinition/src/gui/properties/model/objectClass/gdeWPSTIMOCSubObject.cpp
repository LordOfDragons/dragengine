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
#include <stdlib.h>
#include <string.h>

#include "gdeWPSTIMOCSubObject.h"
#include "../../../gdeWindowMain.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

gdeWPSTIMOCSubObject::gdeWPSTIMOCSubObject(gdeWPSTreeModel &tree, eTypes type,
	gdeObjectClass *objectClass, int index) :
gdeWPSTreeItemModel(tree, type),
pObjectClass(NULL),
pIndex(index)
{
	if(!objectClass){
		DETHROW(deeInvalidParam);
	}
	
	SetIcon(GetWindowMain().GetEnvironment().GetStockIcon(igdeEnvironment::esiNew));
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeWPSTIMOCSubObject::~gdeWPSTIMOCSubObject(){
}



// Management
///////////////

bool gdeWPSTIMOCSubObject::IsValid() const{
	return true;
}

void gdeWPSTIMOCSubObject::ValidateObjectClassName(){
}

int gdeWPSTIMOCSubObject::Compare(const gdeWPSTreeItemModel &item) const{
	const gdeWPSTIMOCSubObject &other = (const gdeWPSTIMOCSubObject &)item;
	
	if(GetType() < item.GetType()){
		return -1;
		
	}else if(GetType() > item.GetType()){
		return 1;
		
	}else if(pIndex < other.pIndex){
		return -1;
		
	}else if(pIndex > other.pIndex){
		return 1;
		
	}else{
		return 0;
	}
}

void gdeWPSTIMOCSubObject::SelectBestMatching(const char *string){
	if(GetParent()){
		((gdeWPSTreeItemModel*)GetParent())->SelectBestMatching(string);
	}
}
