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

#include "mePreviewCamera.h"
#include "mePreviewCameraListener.h"
#include "../meWindowMain.h"
#include "../../configuration/meConfiguration.h"

#include <dragengine/common/exceptions.h>


// Class mePreviewCameraListener
//////////////////////////////////

// Constructor, destructor
////////////////////////////

mePreviewCameraListener::mePreviewCameraListener(mePreviewCamera &previewCamera) :
pPreviewCamera(previewCamera){
}

mePreviewCameraListener::~mePreviewCameraListener() = default;


// Notifications
//////////////////

void mePreviewCameraListener::EditingChanged(meWorld *world){
	pPreviewCamera.SetRenderWorldSize(
		pPreviewCamera.GetWindowMain().GetConfiguration().GetPreviewCameraSize());
}

void mePreviewCameraListener::ObjectAdded(meWorld *, meObject *object)
{
	if(object->GetCameraShowPreview()){
		pPreviewCamera.RebuildCameras();
	}
}

void mePreviewCameraListener::ObjectRemoved(meWorld*, meObject *object){
	if(pPreviewCamera.HasObject(object)){
		pPreviewCamera.RebuildCameras();
	}
}

void mePreviewCameraListener::ObjectClassChanged(meWorld*, meObject *object){
	if(object->GetCameraShowPreview() || pPreviewCamera.HasObject(object)){
		pPreviewCamera.RebuildCameras();
	}
}

void mePreviewCameraListener::ObjectPropertiesChanged(meWorld*, meObject *object){
	if(object->GetCameraShowPreview() || pPreviewCamera.HasObject(object)){
		pPreviewCamera.RebuildCameras();
	}
}

void mePreviewCameraListener::ObjectCameraShowPreview(meWorld*, meObject *object){
	pPreviewCamera.RebuildCameras();
}
