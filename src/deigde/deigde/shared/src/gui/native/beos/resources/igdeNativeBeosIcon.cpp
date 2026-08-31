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

#ifdef IGDE_TOOLKIT_BEOS

#include "igdeNativeBeosIcon.h"
#include "../beostoolkit.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/resources/image/deImage.h>

#include <translation/TranslatorRoster.h>
#include <translation/BitmapStream.h>

#include <cstring>



// BPositionIO wrapper for decBaseFileReader
class decFileReaderPositionIO : public BPositionIO{
private:
	decBaseFileReader &pReader;
	off_t pPosition;
	off_t pSize;
	
public:
	decFileReaderPositionIO(decBaseFileReader &reader) :
	pReader(reader),
	pPosition(0),
	pSize(-1){
		// Try to determine size
		try{
			const int currentPos = reader.GetPosition();
			reader.SetPositionEnd(0);
			pSize = reader.GetPosition();
			reader.SetPosition(currentPos);
			pPosition = currentPos;
		}catch(...){
			pSize = -1;
		}
	}
	
	ssize_t Read(void *buffer, size_t size) override{
		try{
			pReader.Read(buffer, (int)size);
			pPosition += size;
			return (ssize_t)size;
		}catch(...){
			return -1;
		}
	}
	
	ssize_t ReadAt(off_t pos, void *buffer, size_t size) override{
		try{
			pReader.SetPosition((int)pos);
			pReader.Read(buffer, (int)size);
			pPosition = pos + size;
			return (ssize_t)size;
		}catch(...){
			return -1;
		}
	}
	
	off_t Seek(off_t position, uint32 seekMode) override{
		try{
			switch(seekMode){
			case SEEK_SET:
				pReader.SetPosition((int)position);
				pPosition = position;
				break;
				
			case SEEK_CUR:
				pReader.MovePosition((int)position);
				pPosition += position;
				break;
				
			case SEEK_END:
				if(pSize >= 0){
					pReader.SetPosition((int)(pSize + position));
					pPosition = pSize + position;
				}else{
					return -1;
				}
				break;
				
			default:
				return -1;
			}
			return pPosition;
		}catch(...){
			return -1;
		}
	}
	
	off_t Position() const override{
		return pPosition;
	}
	
	ssize_t Write(const void*, size_t) override{
		return -1; // Read-only
	}
	
	ssize_t WriteAt(off_t, const void*, size_t) override{
		return -1; // Read-only
	}
	
	status_t SetSize(off_t) override{
		return B_NOT_ALLOWED; // Read-only
	}
};



// Internal icon data storage
class igdeNativeBeosIcon_Wrapper{
public:
	BBitmap *bitmap;
	
	igdeNativeBeosIcon_Wrapper() : bitmap(nullptr){
	}
	
	igdeNativeBeosIcon_Wrapper(deImage &image) : bitmap(nullptr){
		const int width = image.GetWidth();
		const int height = image.GetHeight();
		
		// Create BBitmap with RGBA32 format
		bitmap = new BBitmap(BRect(0, 0, width - 1, height - 1), B_RGBA32);
		if(!bitmap || !bitmap->IsValid()){
			if(bitmap){
				delete bitmap;
				bitmap = nullptr;
			}
			DETHROW(deeOutOfMemory);
		}
		
		// Copy pixel data
		CopyPixelData(image, (uint8*)bitmap->Bits());
	}
	
	~igdeNativeBeosIcon_Wrapper(){
		if(bitmap){
			delete bitmap;
		}
	}
	
	static void CopyPixelData(deImage &image, uint8 *bitmapData){
		const int width = image.GetWidth();
		const int height = image.GetHeight();
		const int bytesPerRow = bitmap_bytes_per_row(width);
		int x, y;
		
		if(image.GetBitCount() != 8){
			DETHROW(deeInvalidParam);
		}
		
		if(image.GetComponentCount() == 1){
			image.RetainImageData();
			const sGrayscale8 * const dataIn = image.GetDataGrayscale8();
			for(y=0; y<height; y++){
				uint8 *row = bitmapData + y * bytesPerRow;
				for(x=0; x<width; x++){
					const sGrayscale8 &pixel = dataIn[y * width + x];
					row[x * 4 + 0] = pixel.value; // B
					row[x * 4 + 1] = pixel.value; // G
					row[x * 4 + 2] = pixel.value; // R
					row[x * 4 + 3] = 255;         // A
				}
			}
			image.ReleaseImageData();
			
		}else if(image.GetComponentCount() == 3){
			image.RetainImageData();
			const sRGB8 * const dataIn = image.GetDataRGB8();
			for(y=0; y<height; y++){
				uint8 *row = bitmapData + y * bytesPerRow;
				for(x=0; x<width; x++){
					const sRGB8 &pixel = dataIn[y * width + x];
					row[x * 4 + 0] = pixel.blue;  // B
					row[x * 4 + 1] = pixel.green; // G
					row[x * 4 + 2] = pixel.red;   // R
					row[x * 4 + 3] = 255;         // A
				}
			}
			image.ReleaseImageData();
			
		}else if(image.GetComponentCount() == 4){
			image.RetainImageData();
			const sRGBA8 * const dataIn = image.GetDataRGBA8();
			for(y=0; y<height; y++){
				uint8 *row = bitmapData + y * bytesPerRow;
				for(x=0; x<width; x++){
					const sRGBA8 &pixel = dataIn[y * width + x];
					row[x * 4 + 0] = pixel.blue;  // B
					row[x * 4 + 1] = pixel.green; // G
					row[x * 4 + 2] = pixel.red;   // R
					row[x * 4 + 3] = pixel.alpha; // A
				}
			}
			image.ReleaseImageData();
			
		}else{
			DETHROW(deeInvalidParam);
		}
	}
	
private:
	// Not copyable
	igdeNativeBeosIcon_Wrapper(const igdeNativeBeosIcon_Wrapper&) = delete;
	igdeNativeBeosIcon_Wrapper &operator=(const igdeNativeBeosIcon_Wrapper&) = delete;
	
	static int bitmap_bytes_per_row(int width){
		// BBitmap pads rows to 4-byte boundaries
		return ((width * 4) + 3) & ~3;
	}
};



// Class igdeNativeBeosIcon
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeBeosIcon::igdeNativeBeosIcon(){
}

igdeNativeBeosIcon::~igdeNativeBeosIcon(){
}



// Static management
/////////////////////

void *igdeNativeBeosIcon::CreateNativeIcon(deImage &image){
	// DELint-Allow-NewWithoutRef
	return new igdeNativeBeosIcon_Wrapper(image);
}

void *igdeNativeBeosIcon::CreateNativeIconPNG(decBaseFileReader &reader){
	BBitmap *bitmap = nullptr;
	BTranslatorRoster *roster = BTranslatorRoster::Default();
	
	if(roster){
		try{
			// Wrap the file reader in a BPositionIO
			decFileReaderPositionIO io(reader);
			
			// Create a BBitmapStream to receive the decoded bitmap
			BBitmapStream stream;
			
			// Translate the PNG data to a BBitmap
			if(roster->Translate(&io, nullptr, nullptr, &stream, B_TRANSLATOR_BITMAP) == B_OK){
				// Extract the bitmap from the stream
				if(stream.DetachBitmap(&bitmap) == B_OK && bitmap && bitmap->IsValid()){
					// Success - wrap the bitmap
					igdeNativeBeosIcon_Wrapper *wrapper = new igdeNativeBeosIcon_Wrapper();
					wrapper->bitmap = bitmap;
					// DELint-Allow-NewWithoutRef
					return wrapper;
				}
				
				// Failed to get bitmap from stream
				if(bitmap){
					delete bitmap;
					bitmap = nullptr;
				}
			}
		}catch(...){
			// Translation failed, fall back to default icon
			if(bitmap){
				delete bitmap;
				bitmap = nullptr;
			}
		}
	}
	
	// Fallback: create a simple 32x32 default icon
	igdeNativeBeosIcon_Wrapper *wrapper = new igdeNativeBeosIcon_Wrapper();
	wrapper->bitmap = new BBitmap(BRect(0, 0, 31, 31), B_RGBA32);
	if(wrapper->bitmap && wrapper->bitmap->IsValid()){
		// Fill with a default gray color
		const int bytesPerRow = ((32 * 4) + 3) & ~3;
		uint8 *bits = (uint8*)wrapper->bitmap->Bits();
		for(int y = 0; y < 32; y++){
			uint8 *row = bits + y * bytesPerRow;
			for(int x = 0; x < 32; x++){
				row[x * 4 + 0] = 192; // B
				row[x * 4 + 1] = 192; // G
				row[x * 4 + 2] = 192; // R
				row[x * 4 + 3] = 255; // A
			}
		}
	}
	
	// DELint-Allow-NewWithoutRef
	return wrapper;
}

void *igdeNativeBeosIcon::DuplicateNativeIcon(void *native){
	if(!native){
		return nullptr;
	}
	
	igdeNativeBeosIcon_Wrapper *src = (igdeNativeBeosIcon_Wrapper*)native;
	igdeNativeBeosIcon_Wrapper *dst = new igdeNativeBeosIcon_Wrapper();
	
	if(src->bitmap && src->bitmap->IsValid()){
		BRect bounds = src->bitmap->Bounds();
		dst->bitmap = new BBitmap(bounds, src->bitmap->ColorSpace());
		if(dst->bitmap && dst->bitmap->IsValid()){
			// Copy bitmap data - both bitmaps have same size, so this is safe
			const int32 length = src->bitmap->BitsLength();
			// DELint-Allow-memcpy
			memcpy(dst->bitmap->Bits(), src->bitmap->Bits(), length);
		}
	}
	
	return dst;
}

void igdeNativeBeosIcon::DestroyNativeIcon(void *native){
	if(native){
		delete (igdeNativeBeosIcon_Wrapper*)native;
	}
}



// Management
///////////////

decPoint igdeNativeBeosIcon::GetSize(void *native){
	if(!native){
		return decPoint(32, 32);
	}
	
	igdeNativeBeosIcon_Wrapper *wrapper = (igdeNativeBeosIcon_Wrapper*)native;
	if(wrapper->bitmap && wrapper->bitmap->IsValid()){
		BRect bounds = wrapper->bitmap->Bounds();
		return decPoint((int)(bounds.Width() + 1), (int)(bounds.Height() + 1));
	}
	
	return decPoint(32, 32);
}

void igdeNativeBeosIcon::Scale(const decPoint &size, void *native){
	if(!native){
		return;
	}
	
	igdeNativeBeosIcon_Wrapper *wrapper = (igdeNativeBeosIcon_Wrapper*)native;
	if(!wrapper->bitmap || !wrapper->bitmap->IsValid()){
		return;
	}
	
	BRect bounds = wrapper->bitmap->Bounds();
	const int currentWidth = (int)(bounds.Width() + 1);
	const int currentHeight = (int)(bounds.Height() + 1);
	
	if(currentWidth == size.x && currentHeight == size.y){
		return; // Already correct size
	}
	
	// Create new scaled bitmap
	BBitmap *newBitmap = new BBitmap(BRect(0, 0, size.x - 1, size.y - 1),
		wrapper->bitmap->ColorSpace());
	
	if(newBitmap && newBitmap->IsValid()){
		// Simple nearest-neighbor scaling
		const int srcBytesPerRow = ((currentWidth * 4) + 3) & ~3;
		const int dstBytesPerRow = ((size.x * 4) + 3) & ~3;
		const uint8 *srcBits = (uint8*)wrapper->bitmap->Bits();
		uint8 *dstBits = (uint8*)newBitmap->Bits();
		
		for(int y = 0; y < size.y; y++){
			const int srcY = (y * currentHeight) / size.y;
			const uint8 *srcRow = srcBits + srcY * srcBytesPerRow;
			uint8 *dstRow = dstBits + y * dstBytesPerRow;
			
			for(int x = 0; x < size.x; x++){
				const int srcX = (x * currentWidth) / size.x;
				dstRow[x * 4 + 0] = srcRow[srcX * 4 + 0];
				dstRow[x * 4 + 1] = srcRow[srcX * 4 + 1];
				dstRow[x * 4 + 2] = srcRow[srcX * 4 + 2];
				dstRow[x * 4 + 3] = srcRow[srcX * 4 + 3];
			}
		}
		
		delete wrapper->bitmap;
		wrapper->bitmap = newBitmap;
	}else{
		if(newBitmap){
			delete newBitmap;
		}
	}
}

void igdeNativeBeosIcon::UpdatePixels(void *native, deImage &image){
	if(!native){
		return;
	}
	
	igdeNativeBeosIcon_Wrapper *wrapper = (igdeNativeBeosIcon_Wrapper*)native;
	if(!wrapper->bitmap || !wrapper->bitmap->IsValid()){
		return;
	}
	
	BRect bounds = wrapper->bitmap->Bounds();
	const int width = (int)(bounds.Width() + 1);
	const int height = (int)(bounds.Height() + 1);
	
	if(image.GetWidth() != width || image.GetHeight() != height || image.GetBitCount() != 8){
		DETHROW(deeInvalidParam);
	}
	
	// Update bitmap pixels
	igdeNativeBeosIcon_Wrapper::CopyPixelData(image, (uint8*)wrapper->bitmap->Bits());
}

#endif
