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

#include "gdeWPSOCCamera.h"
#include "gdeWPSOCCameraListener.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/objectClass/camera/gdeOCCamera.h"

#include <dragengine/common/exceptions.h>



// Class gdeWPSOCCameraListener
/////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPSOCCameraListener::gdeWPSOCCameraListener(gdeWPSOCCamera &panel) :
pPanel(panel){
}

gdeWPSOCCameraListener::~gdeWPSOCCameraListener(){
}



// Management
///////////////

void gdeWPSOCCameraListener::OCPropertyNameChanged(gdeGameDefinition*,
gdeObjectClass *objectClass, gdeProperty*){
	if(pPanel.GetObjectClass() != objectClass){
		return;
	}
	pPanel.UpdatePropertyList();
}

void gdeWPSOCCameraListener::OCPropertiesChanged(gdeGameDefinition*, gdeObjectClass *objectClass){
	if(pPanel.GetObjectClass() != objectClass){
		return;
	}
	pPanel.UpdatePropertyList();
}

void gdeWPSOCCameraListener::ActiveObjectClassChanged(gdeGameDefinition*){
	pPanel.UpdatePropertyList();
}



void gdeWPSOCCameraListener::OCCamerasChanged(gdeGameDefinition*, gdeObjectClass *objectClass){
	if(pPanel.GetObjectClass() != objectClass){
		return;
	}
	pPanel.UpdateCamera();
}

void gdeWPSOCCameraListener::OCCameraChanged(gdeGameDefinition*,
gdeObjectClass *objectClass, gdeOCCamera *camera){
	if(pPanel.GetObjectClass() != objectClass){
		return;
	}
	if(pPanel.GetCamera() != camera){
		return;
	}
	pPanel.UpdateCamera();
}

void gdeWPSOCCameraListener::ActiveOCCameraChanged(gdeGameDefinition*){
	pPanel.UpdateCamera();
}
