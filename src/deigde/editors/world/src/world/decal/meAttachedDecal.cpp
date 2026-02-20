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

#include "meDecal.h"
#include "meAttachedDecal.h"
#include "../object/meObject.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/decal/deDecal.h>
#include <dragengine/resources/decal/deDecalManager.h>
#include <dragengine/common/exceptions.h>
#include <deigde/gui/wrapper/igdeWObject.h>



// Class meAttachedDecal
//////////////////////////

// Constructor, destructor
////////////////////////////

meAttachedDecal::meAttachedDecal(deEngine *engine, meDecal *decal) :
pEngine(engine),
pDecal(decal)
{
	if(!engine || !decal){
		DETHROW(deeInvalidParam);
	}
	
	pEngDecal = engine->GetDecalManager()->CreateDecal();
}

meAttachedDecal::~meAttachedDecal(){
	RemoveFromParent();
}



// Management
///////////////

void meAttachedDecal::SetParentObject(meObject *object){
	if(object == pParentObject){
		return;
	}
	
	RemoveFromParent();
	
	if(pParentObject){
		pParentObject = nullptr;
	}
	
	pParentObject = object;
	AttachToParent();
}



void meAttachedDecal::AttachToParent(){
	RemoveFromParent(); // just to make sure
	
	if(!pParentObject){
		return;
	}
	
	deComponent * const engComponent = pParentObject->GetObjectWrapper()->GetComponent();
	if(!engComponent){
		return;
	}
	
	const decDMatrix matrix(
		decDMatrix::CreateRT(pDecal->GetRotation() * DEG2RAD, pDecal->GetPosition())
		* decDMatrix::CreateRT( pParentObject->GetRotation() * DEG2RAD, pParentObject->GetPosition() ).Invert() );
	const decVector size(decVector(0.001f, 0.001f, 0.001f).Largest(pDecal->GetSize()));
	
	pEngDecal->SetPosition(matrix.GetPosition().ToVector());
	pEngDecal->SetOrientation(matrix.ToQuaternion());
	pEngDecal->SetSize(size);
	
	engComponent->AddDecal(pEngDecal);
}

void meAttachedDecal::RemoveFromParent(){
	if(pEngDecal->GetParentComponent()){
		pEngDecal->GetParentComponent()->RemoveDecal(pEngDecal);
	}
}
