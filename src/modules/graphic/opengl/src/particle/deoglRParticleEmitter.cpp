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
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "deoglRParticleEmitter.h"
#include "deoglRParticleEmitterType.h"

#include "../renderthread/deoglRenderThread.h"
#include "../vbo/deoglVBOLayout.h"
#include "../vbo/deoglVBOAttribute.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/errortracing/deErrorTrace.h>
#include <dragengine/errortracing/deErrorTracePoint.h>
#include <dragengine/errortracing/deErrorTraceValue.h>



// Class deoglRParticleEmitter
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRParticleEmitter::deoglRParticleEmitter(deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pVBOLayoutShared(NULL),
pVBOLayoutLocal(NULL){
	LEAK_CHECK_CREATE(renderThread, ParticleEmitter);
}

deoglRParticleEmitter::~deoglRParticleEmitter(){
	LEAK_CHECK_FREE(pRenderThread, ParticleEmitter);
	if(pVBOLayoutLocal){
		delete pVBOLayoutLocal;
	}
	if(pVBOLayoutShared){
		delete pVBOLayoutShared;
	}
}



// Management
///////////////

deoglVBOLayout *deoglRParticleEmitter::GetVBOLayoutShared(){
	if(!pVBOLayoutShared){
		pVBOLayoutShared = new deoglVBOLayout;
		
		// name        | offset | type  | components
		// ------------+--------+-------+------------------------------------------------------------------
		// attribute 0 |      0 | float | lifetime, position.x, position.y, position.z
		// attribute 1 |     16 | float | linearVelocity, angularVelocity, castSize, castEmissivity
		// attribute 2 |     32 | uchar | linearDirection.x, linearDirection.y, linearDirection.z, rotation
		// attribute 3 |     36 | uchar | castRed, castGreen, castBlue, castTransparency
		pVBOLayoutShared->SetAttributeCount(4);
		pVBOLayoutShared->SetStride(40); // best performance with multiple of 32/64?
		pVBOLayoutShared->SetSize(0);
		pVBOLayoutShared->SetIndexType(deoglVBOLayout::eitUnsignedShort);
		
		deoglVBOAttribute &attribute0 = pVBOLayoutShared->GetAttributeAt(0);
		attribute0.SetComponentCount(4);
		attribute0.SetDataType(deoglVBOAttribute::edtFloat);
		attribute0.SetOffset(0);
		
		deoglVBOAttribute &attribute1 = pVBOLayoutShared->GetAttributeAt(1);
		attribute1.SetComponentCount(4);
		attribute1.SetDataType(deoglVBOAttribute::edtFloat);
		attribute1.SetOffset(16);
		
		deoglVBOAttribute &attribute2 = pVBOLayoutShared->GetAttributeAt(2);
		attribute2.SetComponentCount(4);
		attribute2.SetDataType(deoglVBOAttribute::edtUByte);
		attribute2.SetOffset(32);
		
		deoglVBOAttribute &attribute3 = pVBOLayoutShared->GetAttributeAt(3);
		attribute3.SetComponentCount(4);
		attribute3.SetDataType(deoglVBOAttribute::edtUByte);
		attribute3.SetOffset(36);
	}
	
	return pVBOLayoutShared;
}

deoglVBOLayout *deoglRParticleEmitter::GetVBOLayoutLocal(){
	if(!pVBOLayoutLocal){
		pVBOLayoutLocal = new deoglVBOLayout;
		
		// name        | offset | type  | components
		// ------------+--------+-------+------------------------------------------------------------------
		// attribute 0 |      0 | float | beamLocation
		pVBOLayoutLocal->SetAttributeCount(1);
		pVBOLayoutLocal->SetStride(4);
		pVBOLayoutLocal->SetSize(0);
		pVBOLayoutLocal->SetIndexType(deoglVBOLayout::eitUnsignedShort);
		
		deoglVBOAttribute &attribute0 = pVBOLayoutLocal->GetAttributeAt(0);
		attribute0.SetComponentCount(1);
		attribute0.SetDataType(deoglVBOAttribute::edtFloat);
		attribute0.SetOffset(0);
	}
	
	return pVBOLayoutLocal;
}

void deoglRParticleEmitter::PrepareForRender(){
	const int count = pTypes.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((deoglRParticleEmitterType*)pTypes.GetAt(i))->PrepareForRender();
	}
}



int deoglRParticleEmitter::GetTypeCount() const{
	return pTypes.GetCount();
}

deoglRParticleEmitterType &deoglRParticleEmitter::GetTypeAt(int index) const{
	return *((deoglRParticleEmitterType*)pTypes.GetAt(index));
}

void deoglRParticleEmitter::RemoveAllTypes(){
	pTypes.RemoveAll();
}

void deoglRParticleEmitter::AddType(deoglRParticleEmitterType *type){
	if(!type){
		DETHROW(deeInvalidParam);
	}
	pTypes.Add(type);
}
