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

#include "deoglRWorld.h"
#include "deoglWorldCompute.h"
#include "../capabilities/deoglCapabilities.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTRenderers.h"
#include "../renderthread/deoglRTLogger.h"
#include "../rendering/deoglRenderCompute.h"
#include "../shaders/paramblock/deoglSPBMapBuffer.h"

#include <dragengine/common/exceptions.h>



// Class deoglWorldCompute
////////////////////////////

// #define ENABLE_DEBUG_1
// #define ENABLE_DEBUG_2

// Constructors and Destructors
/////////////////////////////////

deoglWorldCompute::deoglWorldCompute(deoglRWorld &world) :
pWorld(world),
pFullUpdateLimit(0),
pFullUpdateFactor(0.2f),
pUpdateElementCount(0),
pForceFullUpdate(false),
pFullUpdateGeometryLimit(0),
pFullUpdateGeometryFactor(0.25f),
pUpdateElementGeometryCount(0),
pForceFullUpdateGeometry(false),
pClearGeometries(nullptr),
pClearGeometriesCount(0),
pClearGeometriesSize(0),
pClearGeometryCount(0)
{
	// WARNING potentially called from main thread. calling EnsureBuffer() is not allowed here
	
	deoglRenderThread &renderThread = world.GetRenderThread();
	const bool rowMajor = renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working();
	
	pSSBOElements.TakeOver(new deoglSPBlockSSBO(renderThread, deoglSPBlockSSBO::etStream));
	pSSBOElements->SetRowMajor(rowMajor);
	pSSBOElements->SetParameterCount(11);
	pSSBOElements->GetParameterAt(espeMinExtend).SetAll(deoglSPBParameter::evtFloat, 3, 1, 1);
	pSSBOElements->GetParameterAt(espeFlags).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBOElements->GetParameterAt(espeMaxExtend).SetAll(deoglSPBParameter::evtFloat, 3, 1, 1);
	pSSBOElements->GetParameterAt(espeUpdateIndex).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBOElements->GetParameterAt(espeLayerMask).SetAll(deoglSPBParameter::evtInt, 2, 1, 1);
	pSSBOElements->GetParameterAt(espeFirstGeometry).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBOElements->GetParameterAt(espeGeometryCount).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBOElements->GetParameterAt(espeLodFactors).SetAll(deoglSPBParameter::evtFloat, 4, 1, 1);
	pSSBOElements->GetParameterAt(espeHighestLod).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBOElements->GetParameterAt(espeCullResult).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBOElements->GetParameterAt(espeLodIndex).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBOElements->MapToStd140();
	
	pSSBOElementGeometries.TakeOver(new deoglSPBlockSSBO(renderThread, deoglSPBlockSSBO::etStream));
	pSSBOElementGeometries->SetRowMajor(rowMajor);
	pSSBOElementGeometries->SetParameterCount(9);
	pSSBOElementGeometries->GetParameterAt(espegElement).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBOElementGeometries->GetParameterAt(espegLod).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBOElementGeometries->GetParameterAt(espegRenderFilter).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBOElementGeometries->GetParameterAt(espegSkinTexture).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBOElementGeometries->GetParameterAt(espegPipelineBase).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBOElementGeometries->GetParameterAt(espegVao).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBOElementGeometries->GetParameterAt(espegInstance).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBOElementGeometries->GetParameterAt(espegSPBInstance).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBOElementGeometries->GetParameterAt(espegTUCs).SetAll(deoglSPBParameter::evtInt, 4, 1, 1);
	pSSBOElementGeometries->MapToStd140();
	
	pSharedSPBGeometries.TakeOver(new deoglSharedBlockSPB(pSSBOElementGeometries));
}

deoglWorldCompute::~deoglWorldCompute(){
	// quick dispose allows elements like components to drop out of the world quickly.
	// in this case though they do not remove themselves from the compute world properly.
	// to avoid problems remove all elements before cleaning up
	const int count = pElements.GetCount();
	int i;
	for(i=0; i<count; i++){
		deoglWorldComputeElement &element = *(deoglWorldComputeElement*)pElements.GetAt(i);
		element.SetIndex(-1);
		element.SetWorldCompute(nullptr);
		element.GetSPBGeometries() = nullptr;
	}
	pElements.RemoveAll();
	
	if(pClearGeometries){
		delete [] pClearGeometries;
	}
}



// Management
///////////////

void deoglWorldCompute::Prepare(){
	if(pUpdateElements.GetCount() < pFullUpdateLimit && ! pForceFullUpdate
	&& pElements.GetCount() <= pSSBOElements->GetElementCount()){
		pUpdateSSBOElements();
		
	}else{
		pFullUpdateSSBOElements();
	}
	pForceFullUpdate = false;
	
	pUpdateFullUpdateLimits();
}

void deoglWorldCompute::PrepareGeometries(){
	if(pClearGeometriesCount < pFullUpdateGeometryLimit){
		pUpdateSSBOClearGeometries();
		
	}else{
		pForceFullUpdateGeometry = true;
	}
	
	if(pUpdateElementGeometries.GetCount() < pFullUpdateGeometryLimit && ! pForceFullUpdateGeometry){
		pUpdateSSBOElementGeometries();
		// pFullUpdateSSBOElementGeometries();
		
	}else{
		pFullUpdateSSBOElementGeometries();
	}
	pForceFullUpdateGeometry = false;
	
	pUpdateFullUpdateGeometryLimits();
}



int deoglWorldCompute::GetElementCount() const{
	return pElements.GetCount();
}

deoglWorldComputeElement &deoglWorldCompute::GetElementAt(int index) const{
	return *(deoglWorldComputeElement*)pElements.GetAt(index);
}

void deoglWorldCompute::AddElement(deoglWorldComputeElement *element){
	DEASSERT_NOTNULL(element);
	DEASSERT_TRUE(element->GetIndex() == -1)
	
	const int index = pElements.GetCount();
	
	pElements.Add(element);
	element->SetWorldCompute(this);
	element->SetIndex(index);
	element->SetUpdateRequired(true);
	element->SetUpdateGeometriesRequired(true);
	
	if(pUpdateElements.GetCount() < pFullUpdateLimit){
		pUpdateElements.Add(element);
		
	}else{
		pForceFullUpdate = true;
	}
	
	if(pUpdateElementGeometries.GetCount() < pFullUpdateGeometryLimit){
		pUpdateElementGeometries.Add(element);
		
	}else{
		pForceFullUpdateGeometry = true;
	}
	
#if defined ENABLE_DEBUG_1 || defined ENABLE_DEBUG_2
	pWorld.GetRenderThread().GetLogger().LogInfoFormat(
		"WorldCompute.AddElement: type=%d index=%d uec=%d ful=%d, uegc=%d fugl=%d",
		element->GetType(), index, pUpdateElements.GetCount(), pFullUpdateLimit,
		pUpdateElementGeometries.GetCount(), pFullUpdateGeometryLimit);
	pDebugPrintElements();
	pSharedSPBGeometries->DebugPrint(pWorld.GetRenderThread().GetLogger());
	pDebugPrintUpdateElements();
	pDebugPrintUpdateGeometries();
	pDebugPrintClearGeometries();
#endif
}

void deoglWorldCompute::UpdateElement(deoglWorldComputeElement *element){
	DEASSERT_NOTNULL(element);
	DEASSERT_TRUE(element->GetIndex() != -1)
	
	if(element->GetUpdateRequired()){
		return;
	}
	
	element->SetUpdateRequired(true);
	
	if(pUpdateElements.GetCount() < pFullUpdateLimit){
		pUpdateElements.Add(element);
		
	}else{
		pForceFullUpdate = true;
	}
	
#ifdef ENABLE_DEBUG_1
	pWorld.GetRenderThread().GetLogger().LogInfoFormat(
		"WorldCompute.UpdateElement: type=%d index=%d uec=%d ful=%d, uegc=%d fugl=%d",
		element->GetType(), element->GetIndex(), pUpdateElements.GetCount(), pFullUpdateLimit,
		pUpdateElementGeometries.GetCount(), pFullUpdateGeometryLimit);
	pDebugPrintElements();
	pSharedSPBGeometries->DebugPrint(pWorld.GetRenderThread().GetLogger());
	pDebugPrintUpdateElements();
	pDebugPrintUpdateGeometries();
	pDebugPrintClearGeometries();
#endif
}

void deoglWorldCompute::RemoveElement(deoglWorldComputeElement *element){
	DEASSERT_NOTNULL(element);
	
	const int index = element->GetIndex();
	DEASSERT_TRUE(index != -1)
#if defined ENABLE_DEBUG_1 || defined ENABLE_DEBUG_2
	pWorld.GetRenderThread().GetLogger().LogInfoFormat(
		"WorldCompute.RemoveElement: type=%d spbg=%p(%d %d) index=%d(%d) ue=%d(%d %d) cgc=%d ueg=%d(%d %d)",
		element->GetType(), (const deoglSharedBlockSPBElement*)element->GetSPBGeometries(),
		element->GetSPBGeometries() ? element->GetSPBGeometries()->GetIndex() : -1,
		element->GetSPBGeometries() ? element->GetSPBGeometries()->GetCount() : -1,
		index, pElements.GetCount(), pUpdateElementCount, pFullUpdateLimit, pForceFullUpdate,
		pClearGeometriesCount, pUpdateElementGeometryCount, pFullUpdateGeometryLimit,
		pForceFullUpdateGeometry);
#endif
	
	const int last = pElements.GetCount() - 1;
	
	if(element->GetSPBGeometries()){
		// clear geometries
		if(pClearGeometriesCount < pFullUpdateGeometryLimit){
			sClearGeometries &entry = pClearGeometries[pClearGeometriesCount++];
			entry.first = element->GetSPBGeometries()->GetIndex();
			entry.count = element->GetSPBGeometries()->GetCount();
			
		}else{
			pForceFullUpdateGeometry = true;
		}
		
		element->GetSPBGeometries() = nullptr;
	}
	
	element->SetIndex(-1);
	element->SetWorldCompute(nullptr);
	element->SetUpdateRequired(false);
	element->SetUpdateGeometriesRequired(false);
	
	// removing from clear geometries is not required since clearing happens
	// before updating so no wrong results can happen
	
	int removeIndex = pUpdateElementGeometries.IndexOf(element);
	if(removeIndex != -1){
		pUpdateElementGeometries.RemoveFrom(removeIndex);
	}
	
	removeIndex = pUpdateElements.IndexOf(element);
	if(removeIndex != -1){
		pUpdateElements.RemoveFrom(removeIndex);
	}
	
	if(index < last){
		deoglWorldComputeElement * const swap = (deoglWorldComputeElement*)pElements.GetAt(last);
		pElements.SetAt(index, swap);
		swap->SetIndex(index);
		
		if(! swap->GetUpdateRequired()){
			swap->SetUpdateRequired(true);
			
			if(pUpdateElements.GetCount() < pFullUpdateLimit){
				pUpdateElements.Add(swap);
				
			}else{
				pForceFullUpdate = true;
			}
		}
		
		// checking for swap->GetSPBGeometries() to be non-nullptr is not working since an
		// upcoming GetSSBOUpdateElements() calls pCheckElementGeometryCount() which in turn
		// can change the SPB pointer. thus we have to always update the element geometries
		// even if there is the chance it is empty
		if(! swap->GetUpdateGeometriesRequired()){
			// updating geometries is required since they contain the element index
			swap->SetUpdateGeometriesRequired(true);
			
			if(pUpdateElementGeometries.GetCount() < pFullUpdateGeometryLimit){
				pUpdateElementGeometries.Add(swap);
				
			}else{
				pForceFullUpdateGeometry = true;
			}
		}
	}
	
	pElements.RemoveFrom(last);
	
#if defined ENABLE_DEBUG_1 || defined ENABLE_DEBUG_2
	pWorld.GetRenderThread().GetLogger().LogInfoFormat(
		"- ue=%d(%d %d) cgc=%d ueg=%d(%d %d)", pUpdateElements.GetCount(), pFullUpdateLimit,
		pForceFullUpdate, pClearGeometriesCount, pUpdateElementGeometries.GetCount(),
		pFullUpdateGeometryLimit, pForceFullUpdateGeometry);
	pDebugPrintElements();
	pSharedSPBGeometries->DebugPrint(pWorld.GetRenderThread().GetLogger());
	pDebugPrintUpdateElements();
	pDebugPrintUpdateGeometries();
	pDebugPrintClearGeometries();
#endif
}

int deoglWorldCompute::GetElementGeometryCount() const{
	return pSharedSPBGeometries->GetSize() - pSharedSPBGeometries->GetFreeElementCountAtEnd();
	// return GetElementCount() > 0 ? pSSBOElementGeometries->GetElementCount() : 0;
}

void deoglWorldCompute::UpdateElementGeometries(deoglWorldComputeElement *element){
	DEASSERT_NOTNULL(element);
	DEASSERT_TRUE(element->GetIndex() != -1)
	
	if(element->GetUpdateGeometriesRequired()){
		return;
	}
	
	element->SetUpdateGeometriesRequired(true);
	
	if(pUpdateElementGeometries.GetCount() < pFullUpdateGeometryLimit){
		pUpdateElementGeometries.Add(element);
		
	}else{
		pForceFullUpdateGeometry = true;
	}
	
	// no clearing geometries here since pCheckElementGeometryCount() will do this if required
	
#ifdef ENABLE_DEBUG_1
	pWorld.GetRenderThread().GetLogger().LogInfoFormat(
		"WorldCompute.UpdateElementGeometries: type=%d index=%d", element->GetType(), element->GetIndex());
	pDebugPrintElements();
	pSharedSPBGeometries->DebugPrint(pWorld.GetRenderThread().GetLogger());
	pDebugPrintUpdateElements();
	pDebugPrintUpdateGeometries();
	pDebugPrintClearGeometries();
#endif
}



// Private Functions
//////////////////////

void deoglWorldCompute::pUpdateSSBOElements(){
	const int count = pUpdateElements.GetCount();
	
	const deoglRenderCompute &renderCompute = pWorld.GetRenderThread().GetRenderers().GetCompute();
	deoglSPBlockSSBO &ssbo = renderCompute.GetSSBOUpdateElements();
	
	if(count > ssbo.GetElementCount()){
		ssbo.SetElementCount(count);
	}
	
	const deoglSPBMapBuffer mapped(ssbo);
	deoglWorldComputeElement::sDataElement * const data =
		(deoglWorldComputeElement::sDataElement*)ssbo.GetMappedBuffer();
	int i;
	
	memset(data, 0, ssbo.GetElementStride() * count);
	
	for(i=0; i<count; i++){
		deoglWorldComputeElement &element = *((deoglWorldComputeElement*)pUpdateElements.GetAt(i));
		pUpdateSSBOElement(element, data[i]);
		data[i].updateIndex = (uint32_t)element.GetIndex();
	}
	
#if defined ENABLE_DEBUG_1 || defined ENABLE_DEBUG_2
	pWorld.GetRenderThread().GetLogger().LogInfoFormat("WorldCompute.pUpdateSSBOElements: %d", count);
#endif
#ifdef ENABLE_DEBUG_1
	pDebugPrintElements();
	pSharedSPBGeometries->DebugPrint(pWorld.GetRenderThread().GetLogger());
	pDebugPrintUpdateElements();
#endif
	
	pUpdateElementCount = count;
	pUpdateElements.RemoveAll();
}

void deoglWorldCompute::pFullUpdateSSBOElements(){
	pUpdateElementCount = 0;
	pUpdateElements.RemoveAll();
	
	const int count = pElements.GetCount();
	if(count == 0){
		pSSBOElements->EnsureBuffer(); // since we can not init in constructor
		return;
	}
	
	if(count > pSSBOElements->GetElementCount()){
		pSSBOElements->SetElementCount(count + 100);
	}
	
	const deoglSPBMapBuffer mapped(pSSBOElements);
	deoglWorldComputeElement::sDataElement * const data =
		(deoglWorldComputeElement::sDataElement*)pSSBOElements->GetMappedBuffer();
	
	memset(data, 0, pSSBOElements->GetElementStride() * count);
	
	int i;
	for(i=0; i<count; i++){
		pUpdateSSBOElement(*(deoglWorldComputeElement*)pElements.GetAt(i), data[i]);
	}
	
#if defined ENABLE_DEBUG_1 || defined ENABLE_DEBUG_2
	pWorld.GetRenderThread().GetLogger().LogInfoFormat("WorldCompute.pFullUpdateSSBOElements %d", count);
#endif
#ifdef ENABLE_DEBUG_1
	pDebugPrintElements();
	pSharedSPBGeometries->DebugPrint(pWorld.GetRenderThread().GetLogger());
#endif
}

void deoglWorldCompute::pUpdateSSBOElement(deoglWorldComputeElement &element,
deoglWorldComputeElement::sDataElement &data){
	element.UpdateData(data);
	element.SetUpdateRequired(false);
	
	pCheckElementGeometryCount(element, data);
	
	if(element.GetSPBGeometries()){
		data.firstGeometry = (uint32_t)element.GetSPBGeometries()->GetIndex();
	}
}

void deoglWorldCompute::pCheckElementGeometryCount(deoglWorldComputeElement &element,
deoglWorldComputeElement::sDataElement &data){
	// check if geometry count changed
	int oldFirstIndex = 0;
	int oldGeometryCount = 0;
	
	if(element.GetSPBGeometries()){
		oldFirstIndex = element.GetSPBGeometries()->GetIndex();
		oldGeometryCount = element.GetSPBGeometries()->GetCount();
	}
	
	const int geometryCount = data.geometryCount;
	if(geometryCount == oldGeometryCount){
		return;
	}
	
	if(geometryCount == 0){
		if(oldGeometryCount > 0){
			// clear old geometries
			if(pClearGeometriesCount < pFullUpdateGeometryLimit){
				sClearGeometries &entry = pClearGeometries[pClearGeometriesCount++];
				entry.first = oldFirstIndex;
				entry.count = oldGeometryCount;
				
			}else{
				pForceFullUpdateGeometry = true;
			}
		}
		
		element.GetSPBGeometries() = nullptr;
		return;
	}
	
	if(geometryCount < oldGeometryCount){
		// if the geometry count is less we can keep using the same spb element. this does
		// waste some slots but chances are the geometry count increses again soon most
		// probably back to the old count. this is typically the case with outline geometries.
		// 
		// we do not have to clear the geometries since updating element does already
		return;
	}
	
	// first drop the spb element. this returns it to the spb. this is required since the
	// returned spb element could be merged producing a larger empty space we can consume
	element.GetSPBGeometries() = nullptr;
	element.GetSPBGeometries() = pSharedSPBGeometries->GetElement(geometryCount);
	
	// if new element can be obtained clear old geometries range. updating element will
	// write the new geometry range. furthermore UpdateElementGeometries() will be called
	// too so we do not need to add updating the new geometry range here
	if(element.GetSPBGeometries()){
		if(oldGeometryCount > 0){
			// clear old geometries
			if(pClearGeometriesCount < pFullUpdateGeometryLimit){
				sClearGeometries &entry = pClearGeometries[pClearGeometriesCount++];
				entry.first = oldFirstIndex;
				entry.count = oldGeometryCount;
				
			}else{
				pForceFullUpdateGeometry = true;
			}
		}
		
	// if no new element can be obtained we have to grow the spb. this requires a full
	// update so we can stop trying to do partial updates in the future
	}else{
		pSSBOElementGeometries->SetElementCount(pSSBOElementGeometries->GetElementCount() + geometryCount + 100);
		pForceFullUpdateGeometry = true;
		
		element.GetSPBGeometries() = pSharedSPBGeometries->GetElement(geometryCount);
	}
}



void deoglWorldCompute::pUpdateSSBOElementGeometries(){
		// decTimer timer;
	const int elementCount = pUpdateElementGeometries.GetCount();
	int i;
	
	pUpdateElementGeometryCount = 0;
	
	for(i=0; i<elementCount; i++){
		const deoglWorldComputeElement &element = *((deoglWorldComputeElement*)pUpdateElementGeometries.GetAt(i));
		if(element.GetSPBGeometries()){
			pUpdateElementGeometryCount += element.GetSPBGeometries()->GetCount();
		}
	}
	
	if(pUpdateElementGeometryCount == 0){
		for(i=0; i<elementCount; i++){
			// has to be always done since it is possible a previous GetSSBOUpdateElements() called
			// pCheckElementGeometryCount() which in turn could drop the SPB
			((deoglWorldComputeElement*)pUpdateElementGeometries.GetAt(i))->SetUpdateGeometriesRequired(false);
		}
		
		pUpdateElementGeometries.RemoveAll();
		return;
	}
	
	const deoglRenderCompute &renderCompute = pWorld.GetRenderThread().GetRenderers().GetCompute();
	deoglSPBlockSSBO &ssboData = renderCompute.GetSSBOUpdateElementGeometries();
	deoglSPBlockSSBO &ssboIndex = renderCompute.GetSSBOUpdateIndices();
	
	if(pUpdateElementGeometryCount > ssboData.GetElementCount()){
		ssboData.SetElementCount(pUpdateElementGeometryCount);
	}
	
	const int countIndex = (pUpdateElementGeometryCount - 1) / 4 + 1;
	if(countIndex > ssboIndex.GetElementCount()){
		ssboIndex.SetElementCount(countIndex);
	}
	
	const deoglSPBMapBuffer mappedData(ssboData, 0, pUpdateElementGeometryCount);
	const deoglSPBMapBuffer mappedIndex(ssboIndex, 0, countIndex);
	deoglWorldComputeElement::sDataElementGeometry * const dataData =
		(deoglWorldComputeElement::sDataElementGeometry*)ssboData.GetMappedBuffer();
	uint32_t * const dataIndex = (uint32_t*)ssboIndex.GetMappedBuffer();
	
	ssboData.Clear();
	ssboIndex.Clear();
	
	int j, nextIndex = 0;
	
	for(i=0; i<elementCount; i++){
		deoglWorldComputeElement &element = *((deoglWorldComputeElement*)pUpdateElementGeometries.GetAt(i));
		// has to be always done since it is possible a previous GetSSBOUpdateElements() called
		// pCheckElementGeometryCount() which in turn could drop the SPB
		element.SetUpdateGeometriesRequired(false);
		
		if(! element.GetSPBGeometries()){
			continue;
		}
		
		element.UpdateDataGeometries(dataData + nextIndex);
		
		const deoglSharedBlockSPBElement &spbel = element.GetSPBGeometries();
		const int firstGeometry = spbel.GetIndex();
		const int geometryCount = spbel.GetCount();
		
		for(j=0; j<geometryCount; j++){
			dataIndex[nextIndex++] = (uint32_t)(firstGeometry + j);
		}
	}
		// pWorld.GetRenderThread().GetLogger().LogInfoFormat("pUpdateSSBOElementGeometries: Write %dys (%d)", (int)(timer.GetElapsedTime() * 1e6f), pUpdateElementGeometryCount);
		// mappedData.Unmap();
		// mappedIndex.Unmap();
		// pWorld.GetRenderThread().GetLogger().LogInfoFormat("pUpdateSSBOElementGeometries: Upload %dys", (int)(timer.GetElapsedTime() * 1e6f));
	
#if defined ENABLE_DEBUG_1 || defined ENABLE_DEBUG_2
	pWorld.GetRenderThread().GetLogger().LogInfoFormat(
		"WorldCompute.pUpdateSSBOElementGeometries %d", pUpdateElementGeometryCount);
#endif
#ifdef ENABLE_DEBUG_1
	pDebugPrintElements();
	pSharedSPBGeometries->DebugPrint(pWorld.GetRenderThread().GetLogger());
	pDebugPrintUpdateGeometries();
#endif
	
	pUpdateElementGeometries.RemoveAll();
	
	// pWorld.GetRenderThread().GetLogger().LogInfo( "WorldCompute.UpdateElementGeometries" );
	// pDebugPrintElements();
	// pSharedSPBGeometries->DebugPrint( pWorld.GetRenderThread().GetLogger() );
}

void deoglWorldCompute::pFullUpdateSSBOElementGeometries(){
	pClearGeometryCount = 0;
	pUpdateElementGeometryCount = 0;
	pUpdateElementGeometries.RemoveAll();
	
	const int elementCount = pElements.GetCount();
	if(elementCount == 0){
		pSSBOElementGeometries->EnsureBuffer(); // since we can not init in constructor
		return;
	}
	
		// decTimer timer;
	const deoglSPBMapBuffer mapped(pSSBOElementGeometries);
	deoglWorldComputeElement::sDataElementGeometry * const data =
		(deoglWorldComputeElement::sDataElementGeometry*)pSSBOElementGeometries->GetMappedBuffer();
	
	pSSBOElementGeometries->Clear();
	
	int i;
	for(i=0; i<elementCount; i++){
		deoglWorldComputeElement &element = *((deoglWorldComputeElement*)pElements.GetAt(i));
		if(element.GetSPBGeometries()){
			element.UpdateDataGeometries(data + element.GetSPBGeometries()->GetIndex());
		}
		
		// has to be always done since it is possible a previous GetSSBOUpdateElements() called
		// pCheckElementGeometryCount() which in turn could drop the SPB
		element.SetUpdateGeometriesRequired(false);
	}
		// pWorld.GetRenderThread().GetLogger().LogInfoFormat( "pFullUpdateSSBOElementGeometries: Write %dys", (int)(timer.GetElapsedTime() * 1e6f) );
		// mapped.Unmap();
		// pWorld.GetRenderThread().GetLogger().LogInfoFormat( "pFullUpdateSSBOElementGeometries: Upload %dys", (int)(timer.GetElapsedTime() * 1e6f) );
	
	// pWorld.GetRenderThread().GetLogger().LogInfo( "WorldCompute.FullUpdateSSBOElementGeometries" );
	// pDebugPrintElements();
	// pSharedSPBGeometries->DebugPrint( pWorld.GetRenderThread().GetLogger() );
	
#if defined ENABLE_DEBUG_1 || defined ENABLE_DEBUG_2
	pWorld.GetRenderThread().GetLogger().LogInfoFormat(
		"WorldCompute.pUpdateFullUpdateGeometryLimits %d", elementCount);
#endif
#ifdef ENABLE_DEBUG_1
	pDebugPrintElements();
	pSharedSPBGeometries->DebugPrint(pWorld.GetRenderThread().GetLogger());
#endif
}



void deoglWorldCompute::pUpdateSSBOClearGeometries(){
	pClearGeometryCount = 0;
	
	int i;
	for(i=0; i<pClearGeometriesCount; i++){
		pClearGeometryCount += pClearGeometries[i].count;
	}
	
	if(pClearGeometryCount == 0){
		pClearGeometriesCount = 0;
		return;
	}
	
	const deoglRenderCompute &renderCompute = pWorld.GetRenderThread().GetRenderers().GetCompute();
	deoglSPBlockSSBO &ssbo = renderCompute.GetSSBOClearGeometries();
	
	if(pClearGeometryCount > ssbo.GetElementCount()){
		ssbo.SetElementCount(pClearGeometryCount);
	}
	
	const deoglSPBMapBuffer mapped(ssbo, 0, pClearGeometryCount);
	uint32_t * const data = (uint32_t*)ssbo.GetMappedBuffer();
	int j, nextIndex = 0;
	
	for(i=0; i<pClearGeometriesCount; i++){
		const sClearGeometries &entry = pClearGeometries[i];
		for(j=0; j<entry.count; j++){
			data[nextIndex++] = (uint32_t)(entry.first + j);
		}
	}
	pClearGeometriesCount = 0;
	
#if defined ENABLE_DEBUG_1 || defined ENABLE_DEBUG_2
	pWorld.GetRenderThread().GetLogger().LogInfoFormat("WorldCompute.pClearGeometries %d", nextIndex);
#endif
#ifdef ENABLE_DEBUG_1
	pDebugPrintElements();
	pSharedSPBGeometries->DebugPrint(pWorld.GetRenderThread().GetLogger());
	pDebugPrintClearGeometries();
#endif
}



void deoglWorldCompute::pUpdateFullUpdateLimits(){
	pFullUpdateLimit = decMath::max(pFullUpdateLimit,
		(int)(pFullUpdateFactor * (float)pElements.GetCount()));
}

void deoglWorldCompute::pUpdateFullUpdateGeometryLimits(){
	// this is count of elements not geometries
	pFullUpdateGeometryLimit = decMath::max(pFullUpdateGeometryLimit,
		(int)(pFullUpdateGeometryFactor * (float)pElements.GetCount()));
	
	// ensure clear geometries array is large enough
	if(pClearGeometriesSize < pFullUpdateGeometryLimit){
		if(pClearGeometries){
			delete [] pClearGeometries;
			pClearGeometries = nullptr;
		}
		
		pClearGeometries = new sClearGeometries[pFullUpdateGeometryLimit];
		pClearGeometriesSize = pFullUpdateGeometryLimit;
		pClearGeometriesCount = 0; // should be 0 already
	}
}

void deoglWorldCompute::pDebugPrintElements(){
	deoglRTLogger &logger = pWorld.GetRenderThread().GetLogger();
	const int count = pElements.GetCount();
	decStringList list;
	decString string;
	int i;
	
	list.Add("{El}");
	for(i=0; i<count; i++){
		const deoglWorldComputeElement &element = *(deoglWorldComputeElement*)pElements.GetAt(i);
			if(element.GetType() == deoglWorldComputeElement::eetHeightTerrainSectorCluster) continue;
		string.Format("[%d:%d(%p),%d,%d]", i, element.GetType(), element.GetOwner(),
			element.GetSPBGeometries() ? element.GetSPBGeometries()->GetIndex() : -1,
			element.GetSPBGeometries() ? element.GetSPBGeometries()->GetCount() : -1);
		list.Add(string);
	}
	logger.LogInfo(list.Join(" "));
}

void deoglWorldCompute::pDebugPrintUpdateElements(){
	deoglRTLogger &logger = pWorld.GetRenderThread().GetLogger();
	const int count = pUpdateElements.GetCount();
	decStringList list;
	decString string;
	int i;
	
	list.Add("{UpEl}");
	for(i=0; i<count; i++){
		deoglWorldComputeElement &element = *((deoglWorldComputeElement*)pUpdateElements.GetAt(i));
		string.Format("[%d:%d(%p),%d,%d]", i, element.GetType(), element.GetOwner(),
			element.GetSPBGeometries() ? element.GetSPBGeometries()->GetIndex() : -1,
			element.GetSPBGeometries() ? element.GetSPBGeometries()->GetCount() : -1);
		list.Add(string);
	}
	logger.LogInfo(list.Join(" "));
}

void deoglWorldCompute::pDebugPrintUpdateGeometries(){
	deoglRTLogger &logger = pWorld.GetRenderThread().GetLogger();
	const int count = pUpdateElementGeometries.GetCount();
	decStringList list;
	decString string;
	int i;
	
	list.Add("{UpGeo}");
	for(i=0; i<count; i++){
		const deoglWorldComputeElement &element = *((deoglWorldComputeElement*)pUpdateElementGeometries.GetAt(i));
		string.Format("[%d:%d(%p),%d,%d]", i, element.GetType(), element.GetOwner(),
			element.GetSPBGeometries() ? element.GetSPBGeometries()->GetIndex() : -1,
			element.GetSPBGeometries() ? element.GetSPBGeometries()->GetCount() : -1);
		list.Add(string);
	}
	logger.LogInfo(list.Join(" "));
}

void deoglWorldCompute::pDebugPrintClearGeometries(){
	deoglRTLogger &logger = pWorld.GetRenderThread().GetLogger();
	const int count = pClearGeometriesCount;
	decStringList list;
	decString string;
	int i;
	
	list.Add("{ClrGeo}");
	for(i=0; i<count; i++){
		string.Format("[%d:%d,%d]", i, pClearGeometries[i].first, pClearGeometries[i].count);
		list.Add(string);
	}
	logger.LogInfo(list.Join(" "));
}
