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

#include "seWPMapped.h"
#include "seWPMappedListener.h"
#include "../../skin/seSkin.h"
#include "../../skin/mapped/seMapped.h"

#include <dragengine/common/exceptions.h>


// Class seWPMappedListener
/////////////////////////////

// Constructor, destructor
////////////////////////////

seWPMappedListener::seWPMappedListener(seWPMapped &panel) :
pPanel(panel){
}

seWPMappedListener::~seWPMappedListener(){
}



// Management
///////////////

void seWPMappedListener::MappedStructureChanged(seSkin *skin){
	if(skin != pPanel.GetSkin()){
		return;
	}
	
	pPanel.UpdateMappedList();
}

void seWPMappedListener::MappedChanged(seSkin *skin, seMapped *mapped){
	if(skin != pPanel.GetSkin() || !mapped->GetActive()){
		return;
	}
	
	pPanel.UpdateMapped();
}

void seWPMappedListener::MappedNameChanged(seSkin *skin, seMapped*){
	if(skin != pPanel.GetSkin()){
		return;
	}
	
	pPanel.UpdateMappedList();
}

void seWPMappedListener::ActiveMappedChanged(seSkin *skin){
	if(skin != pPanel.GetSkin()){
		return;
	}
	
	pPanel.SelectActiveMapped();
	pPanel.UpdateMapped();
	pPanel.FitViewToCurve();
}
