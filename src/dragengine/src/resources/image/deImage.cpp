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

#include "deImage.h"
#include "deImageManager.h"
#include "../../common/exceptions.h"
#include "../../common/file/decBaseFileReader.h"
#include "../../common/utils/decXpmImage.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/image/deBaseImageModule.h"
#include "../../systems/modules/image/deBaseImageInfo.h"
#include "../../systems/modules/graphic/deBaseGraphicImage.h"
#include "../../threading/deMutexGuard.h"



// Class deImage
//////////////////

// Constructor, destructor
////////////////////////////

deImage::deImage(deImageManager *manager, deVirtualFileSystem *vfs, const char *filename,
	TIME_SYSTEM modificationTime, int width, int height, int depth,
	int componentCount, int bitCount) :
deFileResource(manager, vfs, filename, modificationTime),
pWidth(width),
pHeight(height),
pDepth(depth),
pComponentCount(componentCount),
pBitCount(bitCount),
pRetainImageData(GetFilename().IsEmpty() ? 1 : 0),
pPeerGraphic(nullptr)
{
	DEASSERT_TRUE(width >= 1)
	DEASSERT_TRUE(height >= 1)
	DEASSERT_TRUE(depth >= 1)
	DEASSERT_TRUE(componentCount >= 1 && componentCount <= 4)
	DEASSERT_TRUE(bitCount == 8 || bitCount == 16 || bitCount == 32)
	
	pData.SetCountDiscard(width * height * depth * componentCount * (bitCount >> 3));
}

deImage::deImage(deImageManager *manager, deVirtualFileSystem *vfs, const char *filename,
	TIME_SYSTEM modificationTime, decXpmImage *image) :
deFileResource(manager, vfs, filename, modificationTime),
pWidth(0),
pHeight(0),
pDepth(1),
pComponentCount(3),
pBitCount(8),
pRetainImageData(1),
pPeerGraphic(nullptr)
{
	DEASSERT_NOTNULL(image)
	
	pWidth = image->GetWidth();
	pHeight = image->GetHeight();
	
	pData.SetCountDiscard(pWidth * pHeight * pComponentCount * (pBitCount >> 3));
	
	int x, y;
	for(y=0; y<pHeight; y++){
		const sRGBA8 * const xpmData = (sRGBA8*)image->GetData() + pWidth * (pHeight - 1 - y);
		sRGB8 * const imgData = (sRGB8*)pData.GetArrayPointer() + pWidth * y;
		
		for(x=0; x<pWidth; x++){
			imgData[x].red = xpmData[x].red;
			imgData[x].green = xpmData[x].green;
			imgData[x].blue = xpmData[x].blue;
		}
	}
}

deImage::deImage(deImageManager *manager, deVirtualFileSystem *vfs, const char *filename,
	TIME_SYSTEM modificationTime) :
deFileResource(manager, vfs, filename, modificationTime),
pWidth(0),
pHeight(0),
pDepth(0),
pComponentCount(0),
pBitCount(0),
pRetainImageData(GetFilename().IsEmpty() ? 1 : 0),
pPeerGraphic(nullptr){
}

deImage::~deImage(){
	if(pPeerGraphic){
		delete pPeerGraphic;
	}
}



// Management
///////////////

sGrayscale8 *deImage::GetDataGrayscale8(){
	DEASSERT_TRUE(pComponentCount == 1)
	DEASSERT_TRUE(pBitCount == 8)
	
	return (sGrayscale8*)pData.GetArrayPointer();
}

const sGrayscale8 *deImage::GetDataGrayscale8() const{
	DEASSERT_TRUE(pComponentCount == 1)
	DEASSERT_TRUE(pBitCount == 8)
	
	return (sGrayscale8*)pData.GetArrayPointer();
}

sGrayscale16 *deImage::GetDataGrayscale16(){
	DEASSERT_TRUE(pComponentCount == 1)
	DEASSERT_TRUE(pBitCount == 16)
	
	return (sGrayscale16*)pData.GetArrayPointer();
}

const sGrayscale16 *deImage::GetDataGrayscale16() const{
	DEASSERT_TRUE(pComponentCount == 1)
	DEASSERT_TRUE(pBitCount == 16)
	
	return (sGrayscale16*)pData.GetArrayPointer();
}

sGrayscale32 *deImage::GetDataGrayscale32(){
	DEASSERT_TRUE(pComponentCount == 1)
	DEASSERT_TRUE(pBitCount == 32)
	
	return (sGrayscale32*)pData.GetArrayPointer();
}

const sGrayscale32 *deImage::GetDataGrayscale32() const{
	DEASSERT_TRUE(pComponentCount == 1)
	DEASSERT_TRUE(pBitCount == 32)
	
	return (sGrayscale32*)pData.GetArrayPointer();
}

sGrayscaleAlpha8 *deImage::GetDataGrayscaleAlpha8(){
	DEASSERT_TRUE(pComponentCount == 2)
	DEASSERT_TRUE(pBitCount == 8)
	
	return (sGrayscaleAlpha8*)pData.GetArrayPointer();
}

const sGrayscaleAlpha8 *deImage::GetDataGrayscaleAlpha8() const{
	DEASSERT_TRUE(pComponentCount == 2)
	DEASSERT_TRUE(pBitCount == 8)
	
	return (sGrayscaleAlpha8*)pData.GetArrayPointer();
}

sGrayscaleAlpha16 *deImage::GetDataGrayscaleAlpha16(){
	DEASSERT_TRUE(pComponentCount == 2)
	DEASSERT_TRUE(pBitCount == 16)
	
	return (sGrayscaleAlpha16*)pData.GetArrayPointer();
}

const sGrayscaleAlpha16 *deImage::GetDataGrayscaleAlpha16() const{
	DEASSERT_TRUE(pComponentCount == 2)
	DEASSERT_TRUE(pBitCount == 16)
	
	return (sGrayscaleAlpha16*)pData.GetArrayPointer();
}

sGrayscaleAlpha32 *deImage::GetDataGrayscaleAlpha32(){
	DEASSERT_TRUE(pComponentCount == 2)
	DEASSERT_TRUE(pBitCount == 32)
	
	return (sGrayscaleAlpha32*)pData.GetArrayPointer();
}

const sGrayscaleAlpha32 *deImage::GetDataGrayscaleAlpha32() const{
	DEASSERT_TRUE(pComponentCount == 2)
	DEASSERT_TRUE(pBitCount == 32)
	
	return (sGrayscaleAlpha32*)pData.GetArrayPointer();
}

sRGB8 *deImage::GetDataRGB8(){
	DEASSERT_TRUE(pComponentCount == 3)
	DEASSERT_TRUE(pBitCount == 8)
	
	return (sRGB8*)pData.GetArrayPointer();
}

const sRGB8 *deImage::GetDataRGB8() const{
	DEASSERT_TRUE(pComponentCount == 3)
	DEASSERT_TRUE(pBitCount == 8)
	
	return (sRGB8*)pData.GetArrayPointer();
}

sRGB16 *deImage::GetDataRGB16(){
	DEASSERT_TRUE(pComponentCount == 3)
	DEASSERT_TRUE(pBitCount == 16)
	
	return (sRGB16*)pData.GetArrayPointer();
}

const sRGB16 *deImage::GetDataRGB16() const{
	DEASSERT_TRUE(pComponentCount == 3)
	DEASSERT_TRUE(pBitCount == 16)
	
	return (sRGB16*)pData.GetArrayPointer();
}

sRGB32 *deImage::GetDataRGB32(){
	DEASSERT_TRUE(pComponentCount == 3)
	DEASSERT_TRUE(pBitCount == 32)
	
	return (sRGB32*)pData.GetArrayPointer();
}

const sRGB32 *deImage::GetDataRGB32() const{
	DEASSERT_TRUE(pComponentCount == 3)
	DEASSERT_TRUE(pBitCount == 32)
	
	return (sRGB32*)pData.GetArrayPointer();
}

sRGBA8 *deImage::GetDataRGBA8(){
	DEASSERT_TRUE(pComponentCount == 4)
	DEASSERT_TRUE(pBitCount == 8)
	
	return (sRGBA8*)pData.GetArrayPointer();
}

const sRGBA8 *deImage::GetDataRGBA8() const{
	DEASSERT_TRUE(pComponentCount == 4)
	DEASSERT_TRUE(pBitCount == 8)
	
	return (sRGBA8*)pData.GetArrayPointer();
}

sRGBA16 *deImage::GetDataRGBA16(){
	DEASSERT_TRUE(pComponentCount == 4)
	DEASSERT_TRUE(pBitCount == 16)
	
	return (sRGBA16*)pData.GetArrayPointer();
}

const sRGBA16 *deImage::GetDataRGBA16() const{
	DEASSERT_TRUE(pComponentCount == 4)
	DEASSERT_TRUE(pBitCount == 16)
	
	return (sRGBA16*)pData.GetArrayPointer();
}

sRGBA32 *deImage::GetDataRGBA32(){
	DEASSERT_TRUE(pComponentCount == 4)
	DEASSERT_TRUE(pBitCount == 32)
	
	return (sRGBA32*)pData.GetArrayPointer();
}

const sRGBA32 *deImage::GetDataRGBA32() const{
	DEASSERT_TRUE(pComponentCount == 4)
	DEASSERT_TRUE(pBitCount == 32)
	
	return (sRGBA32*)pData.GetArrayPointer();
}

void deImage::NotifyImageDataChanged(){
	if(pPeerGraphic){
		pPeerGraphic->ImageDataChanged();
	}
}



void deImage::RetainImageData(){
	const deMutexGuard guard(pMutex);
	
	if(pRetainImageData > 0){
		pRetainImageData++;
		return;
	}
	
	// dynamic images are always retained
	if(GetFilename().IsEmpty()){
		return;
	}
	
	// load image data
	if(pData.IsEmpty()){
		deImageManager &manager = *((deImageManager*)GetResourceManager());
// 		manager.LogInfoFormat( "Retain image '%s'", GetFilename().GetString() );
		deBaseImageInfo *info = nullptr;
		
		try{
			deBaseImageModule * const module = (deBaseImageModule*)manager.GetModuleSystem()->
				GetModuleAbleToLoad(deModuleSystem::emtImage, GetFilename());
			
			const decBaseFileReader::Ref fileReader(
				manager.OpenFileForReading(*GetVirtualFileSystem(), GetFilename()));
			
			info = module->InitLoadImage(fileReader);
			if(!info){
				DETHROW(deeInvalidParam);
			}
			
			if(info->GetWidth() != pWidth || info->GetHeight() != pHeight || info->GetDepth() != pDepth
			|| info->GetComponentCount() != pComponentCount || info->GetBitCount() != pBitCount){
				DETHROW(deeInvalidParam);
			}
			
			pData.SetCountDiscard(pWidth * pHeight * pDepth * pComponentCount * (pBitCount >> 3));
			
			module->LoadImage(fileReader, *this, *info);
			delete info;
			
		}catch(const deException &){
			if(info){
				delete info;
			}
			throw;
		}
	}
	
	pRetainImageData++; // to make sure it is incremented before notifying peers
	
	if(pPeerGraphic){
		pPeerGraphic->ImageDataRestored();
	}
}

void deImage::ReleaseImageData(){
	const deMutexGuard guard(pMutex);
	
	if(pRetainImageData == 0){
		DETHROW(deeInvalidParam);
	}
	
	pRetainImageData--;
	if(pRetainImageData > 0){
		return;
	}
	
	// dynamic images are always retained
	if(GetFilename().IsEmpty()){
		return;
	}
	
	// unload image data
	pData.SetCountDiscard(0);
	pData.CompactCapacity();
// 		( ( deImageManager* )GetResourceManager() )->LogInfoFormat(
// 			"Release image '%s'", GetFilename().GetString() );
	
	if(pPeerGraphic){
		pPeerGraphic->ImageDataReleased();
	}
}



// System Peers
/////////////////

void deImage::SetPeerGraphic(deBaseGraphicImage *peer){
	if(peer == pPeerGraphic){
		return;
	}
	
	if(pPeerGraphic){
		if(!GetAsynchron() && pPeerGraphic->RetainImageData()){
			ReleaseImageData();
		}
		delete pPeerGraphic;
	}
	
	pPeerGraphic = peer;
	
	if(peer){
		if(!GetAsynchron() && peer->RetainImageData()){
			RetainImageData();
		}
	}
}



// Special
////////////

void deImage::FinalizeConstruction(int width, int height, int depth, int componentCount, int bitCount){
	DEASSERT_TRUE(width >= 1)
	DEASSERT_TRUE(height >= 1)
	DEASSERT_TRUE(depth >= 1)
	DEASSERT_TRUE(componentCount >= 1 && componentCount <= 4)
	DEASSERT_TRUE(bitCount == 8 || bitCount == 16 || bitCount == 32)
	
	pWidth = width;
	pHeight = height;
	pDepth = depth;
	pComponentCount = componentCount;
	pBitCount = bitCount;
	
	pData.SetCountDiscard(width * height * depth * componentCount * (bitCount >> 3));
}

void deImage::PeersRetainImageData(){
	const deMutexGuard guard(pMutex);
	
	if(GetAsynchron()){
		DETHROW(deeInvalidParam);
	}
	
	if(pPeerGraphic && pPeerGraphic->RetainImageData()){
		pRetainImageData++;
	}
	
	// dynamic images are always retained
	if(GetFilename().IsEmpty()){
		return;
	}
	
	// unload image data if retain count is zero
	if(pRetainImageData == 0){
// 		( ( deImageManager* )GetResourceManager() )->LogInfoFormat(
// 			"(PRID) Release image '%s'", GetFilename().GetString() );
		pData.SetCountDiscard(0);
		pData.CompactCapacity();
	}
}
