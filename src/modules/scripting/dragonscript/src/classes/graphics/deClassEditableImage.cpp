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
#include <stdint.h>
#include <string.h>

#include "deClassImage.h"
#include "deClassEditableImage.h"
#include "deClassColor.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassArray.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>



// Native Structure
/////////////////////

struct sImgPixNatDat{
	class Mutator{
	protected:
		Mutator(){}
		
	public:
		virtual ~Mutator(){}
		virtual decColor GetAt(int index) const = 0;
		virtual void SetAt(int index, const decColor &color) = 0;
	};
	
	
	class MutatorGrayscale8 : public Mutator{
	private:
		sGrayscale8 *pData;
		const float pFactor, pFactor2;
		
	public:
		MutatorGrayscale8(deImage &image) : pData(image.GetDataGrayscale8()),
			pFactor(1.0f / 255.0f), pFactor2(255.0f){}
		
		virtual decColor GetAt(int index) const{
			const float value = pFactor * pData[index].value;
			return decColor(value, value, value);
		}
		
		virtual void SetAt(int index, const decColor &color){
			pData[index].value = (unsigned char)(pFactor2 * decMath::clamp(color.r, 0.0f, 1.0f));
		}
	};
	
	class MutatorGrayscale16 : public Mutator{
	private:
		sGrayscale16 *pData;
		const float pFactor, pFactor2;
		
	public:
		MutatorGrayscale16(deImage &image) : pData(image.GetDataGrayscale16()),
			pFactor(1.0f / 65535.0f), pFactor2(65535.0f){}
		
		virtual decColor GetAt(int index) const{
			const float value = pFactor * pData[index].value;
			return decColor(value, value, value);
		}
		
		virtual void SetAt(int index, const decColor &color){
			pData[index].value = (unsigned short)(pFactor2 * decMath::clamp(color.r, 0.0f, 1.0f));
		}
	};
	
	class MutatorGrayscale32 : public Mutator{
	private:
		sGrayscale32 *pData;
		
	public:
		MutatorGrayscale32(deImage &image) : pData(image.GetDataGrayscale32()){}
		
		virtual decColor GetAt(int index) const{
			const float &value = pData[index].value;
			return decColor(value, value, value);
		}
		
		virtual void SetAt(int index, const decColor &color){
			pData[index].value = color.r;
		}
	};
	
	
	class MutatorGrayscaleAlpha8 : public Mutator{
	private:
		sGrayscaleAlpha8 *pData;
		const float pFactor, pFactor2;
		
	public:
		MutatorGrayscaleAlpha8(deImage &image) : pData(image.GetDataGrayscaleAlpha8()),
			pFactor(1.0f / 255.0f), pFactor2(255.0f){}
		
		virtual decColor GetAt(int index) const{
			const float value = pFactor * pData[index].value;
			return decColor(value, value, value, pFactor * pData[index].alpha);
		}
		
		virtual void SetAt(int index, const decColor &color){
			pData[index].value = (unsigned char)(pFactor2 * decMath::clamp(color.r, 0.0f, 1.0f));
			pData[index].alpha = (unsigned char)(pFactor2 * decMath::clamp(color.a, 0.0f, 1.0f));
		}
	};
	
	class MutatorGrayscaleAlpha16 : public Mutator{
	private:
		sGrayscaleAlpha16 *pData;
		const float pFactor, pFactor2;
		
	public:
		MutatorGrayscaleAlpha16(deImage &image) : pData(image.GetDataGrayscaleAlpha16()),
			pFactor(1.0f / 65535.0f), pFactor2(65535.0f){}
		
		virtual decColor GetAt(int index) const{
			const float value = pFactor * pData[index].value;
			return decColor(value, value, value, pFactor * pData[index].alpha);
		}
		
		virtual void SetAt(int index, const decColor &color){
			pData[index].value = (unsigned short)(pFactor2 * decMath::clamp(color.r, 0.0f, 1.0f));
			pData[index].alpha = (unsigned short)(pFactor2 * decMath::clamp(color.a, 0.0f, 1.0f));
		}
	};
	
	class MutatorGrayscaleAlpha32 : public Mutator{
	private:
		sGrayscaleAlpha32 *pData;
		
	public:
		MutatorGrayscaleAlpha32(deImage &image) : pData(image.GetDataGrayscaleAlpha32()){}
		
		virtual decColor GetAt(int index) const{
			const float &value = pData[index].value;
			return decColor(value, value, value, pData[index].alpha);
		}
		
		virtual void SetAt(int index, const decColor &color){
			pData[index].value = color.r;
			pData[index].alpha = color.a;
		}
	};
	
	
	class MutatorRGB8 : public Mutator{
	private:
		sRGB8 *pData;
		const float pFactor, pFactor2;
		
	public:
		MutatorRGB8(deImage &image) : pData(image.GetDataRGB8()),
			pFactor(1.0f / 255.0f), pFactor2(255.0f){}
		
		virtual decColor GetAt(int index) const{
			return decColor(pFactor * pData[index].red,
				pFactor * pData[index].green,
				pFactor * pData[index].blue);
		}
		
		virtual void SetAt(int index, const decColor &color){
			pData[index].red = (unsigned char)(pFactor2 * decMath::clamp(color.r, 0.0f, 1.0f));
			pData[index].green = (unsigned char)(pFactor2 * decMath::clamp(color.g, 0.0f, 1.0f));
			pData[index].blue = (unsigned char)(pFactor2 * decMath::clamp(color.b, 0.0f, 1.0f));
		}
	};
	
	class MutatorRGB16 : public Mutator{
	private:
		sRGB16 *pData;
		const float pFactor, pFactor2;
		
	public:
		MutatorRGB16(deImage &image) : pData(image.GetDataRGB16()),
			pFactor(1.0f / 65535.0f), pFactor2(65535.0f){}
		
		virtual decColor GetAt(int index) const{
			return decColor(pFactor * pData[index].red,
				pFactor * pData[index].green,
				pFactor * pData[index].blue);
		}
		
		virtual void SetAt(int index, const decColor &color){
			pData[index].red = (unsigned short)(pFactor2 * decMath::clamp(color.r, 0.0f, 1.0f));
			pData[index].green = (unsigned short)(pFactor2 * decMath::clamp(color.g, 0.0f, 1.0f));
			pData[index].blue = (unsigned short)(pFactor2 * decMath::clamp(color.b, 0.0f, 1.0f));
		}
	};
	
	class MutatorRGB32 : public Mutator{
	private:
		sRGB32 *pData;
		
	public:
		MutatorRGB32(deImage &image) : pData(image.GetDataRGB32()){}
		
		virtual decColor GetAt(int index) const{
			return decColor(pData[index].red, pData[index].green, pData[index].blue);
		}
		
		virtual void SetAt(int index, const decColor &color){
			pData[index].red = color.r;
			pData[index].green = color.g;
			pData[index].blue = color.b;
		}
	};
	
	
	class MutatorRGBA8 : public Mutator{
	private:
		sRGBA8 *pData;
		const float pFactor, pFactor2;
		
	public:
		MutatorRGBA8(deImage &image) : pData(image.GetDataRGBA8()),
			pFactor(1.0f / 255.0f), pFactor2(255.0f){}
		
		virtual decColor GetAt(int index) const{
			return decColor(pFactor * pData[index].red,
				pFactor * pData[index].green,
				pFactor * pData[index].blue,
				pFactor * pData[index].alpha);
		}
		
		virtual void SetAt(int index, const decColor &color){
			pData[index].red = (unsigned char)(pFactor2 * decMath::clamp(color.r, 0.0f, 1.0f));
			pData[index].green = (unsigned char)(pFactor2 * decMath::clamp(color.g, 0.0f, 1.0f));
			pData[index].blue = (unsigned char)(pFactor2 * decMath::clamp(color.b, 0.0f, 1.0f));
			pData[index].alpha = (unsigned char)(pFactor2 * decMath::clamp(color.a, 0.0f, 1.0f));
		}
	};
	
	class MutatorRGBA16 : public Mutator{
	private:
		sRGBA16 *pData;
		const float pFactor, pFactor2;
		
	public:
		MutatorRGBA16(deImage &image) : pData(image.GetDataRGBA16()),
			pFactor(1.0f / 65535.0f), pFactor2(65535.0f){}
		
		virtual decColor GetAt(int index) const{
			return decColor(pFactor * pData[index].red,
				pFactor * pData[index].green,
				pFactor * pData[index].blue,
				pFactor * pData[index].alpha);
		}
		
		virtual void SetAt(int index, const decColor &color){
			pData[index].red = (unsigned short)(pFactor2 * decMath::clamp(color.r, 0.0f, 1.0f));
			pData[index].green = (unsigned short)(pFactor2 * decMath::clamp(color.g, 0.0f, 1.0f));
			pData[index].blue = (unsigned short)(pFactor2 * decMath::clamp(color.b, 0.0f, 1.0f));
			pData[index].alpha = (unsigned short)(pFactor2 * decMath::clamp(color.a, 0.0f, 1.0f));
		}
	};
	
	class MutatorRGBA32 : public Mutator{
	private:
		sRGBA32 *pData;
		
	public:
		MutatorRGBA32(deImage &image) : pData(image.GetDataRGBA32()){}
		
		virtual decColor GetAt(int index) const{
			return decColor(pData[index].red, pData[index].green, pData[index].blue, pData[index].alpha);
		}
		
		virtual void SetAt(int index, const decColor &color){
			pData[index].red = color.r;
			pData[index].green = color.g;
			pData[index].blue = color.b;
			pData[index].alpha = color.a;
		}
	};
	
	
	deImage *image;
	Mutator *mutator;
	int strideZ;
	int strideY;
};



// Native Functions
/////////////////////

// public func new( int width, int height, int depth, int componentCount, int bitCount )
deClassEditableImage::nfNew::nfNew(const sInitData &init) :
dsFunction(init.clsEditableImage, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // width
	p_AddParameter(init.clsInteger); // height
	p_AddParameter(init.clsInteger); // depth
	p_AddParameter(init.clsInteger); // componentCount
	p_AddParameter(init.clsInteger); // bitCount
}
void deClassEditableImage::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sImgPixNatDat &nd = *((sImgPixNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassEditableImage*)GetOwnerClass())->GetDS();
	
	// clear ( important )
	nd.image = NULL;
	nd.mutator = NULL;
	nd.strideZ = 0;
	nd.strideY = 0;
	
	// create image. this automatically retains the image data
	const int width = rt->GetValue(0)->GetInt();
	const int height = rt->GetValue(1)->GetInt();
	const int depth = rt->GetValue(2)->GetInt();
	const int componentCount = rt->GetValue(3)->GetInt();
	const int bitCount = rt->GetValue(4)->GetInt();
	
	nd.image = ds.GetGameEngine()->GetImageManager()->
		CreateImage(width, height, depth, componentCount, bitCount);
	
	nd.strideZ = width * height;
	nd.strideY = width;
	
	// create mutator
	switch(nd.image->GetComponentCount()){
	case 1:
		switch(nd.image->GetBitCount()){
		case 8:
			nd.mutator = new sImgPixNatDat::MutatorGrayscale8(*nd.image);
			break;
			
		case 16:
			nd.mutator = new sImgPixNatDat::MutatorGrayscale16(*nd.image);
			break;
			
		case 32:
			nd.mutator = new sImgPixNatDat::MutatorGrayscale32(*nd.image);
			break;
			
		default:
			DSTHROW(dueInvalidParam);
		}
		break;
		
	case 2:
		switch(nd.image->GetBitCount()){
		case 8:
			nd.mutator = new sImgPixNatDat::MutatorGrayscaleAlpha8(*nd.image);
			break;
			
		case 16:
			nd.mutator = new sImgPixNatDat::MutatorGrayscaleAlpha16(*nd.image);
			break;
			
		case 32:
			nd.mutator = new sImgPixNatDat::MutatorGrayscaleAlpha32(*nd.image);
			break;
			
		default:
			DSTHROW(dueInvalidParam);
		}
		break;
		
	case 3:
		switch(nd.image->GetBitCount()){
		case 8:
			nd.mutator = new sImgPixNatDat::MutatorRGB8(*nd.image);
			break;
			
		case 16:
			nd.mutator = new sImgPixNatDat::MutatorRGB16(*nd.image);
			break;
			
		case 32:
			nd.mutator = new sImgPixNatDat::MutatorRGB32(*nd.image);
			break;
			
		default:
			DSTHROW(dueInvalidParam);
		}
		break;
		
	case 4:
		switch(nd.image->GetBitCount()){
		case 8:
			nd.mutator = new sImgPixNatDat::MutatorRGBA8(*nd.image);
			break;
			
		case 16:
			nd.mutator = new sImgPixNatDat::MutatorRGBA16(*nd.image);
			break;
			
		case 32:
			nd.mutator = new sImgPixNatDat::MutatorRGBA32(*nd.image);
			break;
			
		default:
			DSTHROW(dueInvalidParam);
		}
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
}

// public func destructor()
deClassEditableImage::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsEditableImage,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassEditableImage::nfDestructor::RunFunction(dsRunTime*, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sImgPixNatDat &nd = *((sImgPixNatDat*)p_GetNativeData(myself));
	
	if(nd.mutator){
		delete nd.mutator;
		nd.mutator = NULL;
	}
	
	if(nd.image){
		try{
			nd.image->ReleaseImageData();
			
		}catch(...){
			// no exception allowed here
		}
		
		nd.image->FreeReference();
		nd.image = NULL;
	}
}



// public func Image getImage()
deClassEditableImage::nfGetImage::nfGetImage(const sInitData &init) :
dsFunction(init.clsEditableImage, "getImage", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsImage){
}
void deClassEditableImage::nfGetImage::RunFunction(dsRunTime *rt, dsValue *myself){
	const sImgPixNatDat &nd = *((sImgPixNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassEditableImage*)GetOwnerClass())->GetDS();
	
	ds.GetClassImage()->PushImage(rt, nd.image);
}

// public func int getWidth()
deClassEditableImage::nfGetWidth::nfGetWidth(const sInitData &init) :
dsFunction(init.clsEditableImage, "getWidth", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}
void deClassEditableImage::nfGetWidth::RunFunction(dsRunTime *rt, dsValue *myself){
	const sImgPixNatDat &nd = *((sImgPixNatDat*)p_GetNativeData(myself));
	
	rt->PushInt(nd.image->GetWidth());
}

// public func int getHeight()
deClassEditableImage::nfGetHeight::nfGetHeight(const sInitData &init) :
dsFunction(init.clsEditableImage, "getHeight", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}
void deClassEditableImage::nfGetHeight::RunFunction(dsRunTime *rt, dsValue *myself){
	const sImgPixNatDat &nd = *((sImgPixNatDat*)p_GetNativeData(myself));
	
	rt->PushInt(nd.image->GetHeight());
}

// public func int getDepth()
deClassEditableImage::nfGetDepth::nfGetDepth(const sInitData &init) :
dsFunction(init.clsEditableImage, "getDepth", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}
void deClassEditableImage::nfGetDepth::RunFunction(dsRunTime *rt, dsValue *myself){
	const sImgPixNatDat &nd = *((sImgPixNatDat*)p_GetNativeData(myself));
	
	rt->PushInt(nd.image->GetDepth());
}



// public func Color getAt( int x, int y )
deClassEditableImage::nfGetAt::nfGetAt(const sInitData &init) :
dsFunction(init.clsEditableImage, "getAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsColor){
	p_AddParameter(init.clsInteger); // x
	p_AddParameter(init.clsInteger); // y
}
void deClassEditableImage::nfGetAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const sImgPixNatDat &nd = *((sImgPixNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassEditableImage*)GetOwnerClass())->GetDS();
	
	const int x = rt->GetValue(0)->GetInt();
	const int y = rt->GetValue(1)->GetInt();
	
	if(x < 0){
		DSTHROW_INFO(dueInvalidParam, "x < 0");
	}
	if(x >= nd.image->GetWidth()){
		DSTHROW_INFO(dueInvalidParam, "x >= width");
	}
	if(y < 0){
		DSTHROW_INFO(dueInvalidParam, "y < 0");
	}
	if(y >= nd.image->GetHeight()){
		DSTHROW_INFO(dueInvalidParam, "y >= height");
	}
	
	ds.GetClassColor()->PushColor(rt, nd.mutator->GetAt(nd.strideY * y + x));
}

// public func Color getAt( int x, int y, int z )
deClassEditableImage::nfGetAt2::nfGetAt2(const sInitData &init) :
dsFunction(init.clsEditableImage, "getAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsColor){
	p_AddParameter(init.clsInteger); // x
	p_AddParameter(init.clsInteger); // y
	p_AddParameter(init.clsInteger); // z
}
void deClassEditableImage::nfGetAt2::RunFunction(dsRunTime *rt, dsValue *myself){
	const sImgPixNatDat &nd = *((sImgPixNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassEditableImage*)GetOwnerClass())->GetDS();
	
	const int x = rt->GetValue(0)->GetInt();
	const int y = rt->GetValue(1)->GetInt();
	const int z = rt->GetValue(2)->GetInt();
	
	if(x < 0){
		DSTHROW_INFO(dueInvalidParam, "x < 0");
	}
	if(x >= nd.image->GetWidth()){
		DSTHROW_INFO(dueInvalidParam, "x >= width");
	}
	if(y < 0){
		DSTHROW_INFO(dueInvalidParam, "y < 0");
	}
	if(y >= nd.image->GetHeight()){
		DSTHROW_INFO(dueInvalidParam, "y >= height");
	}
	if(z < 0){
		DSTHROW_INFO(dueInvalidParam, "z < 0");
	}
	if(z >= nd.image->GetDepth()){
		DSTHROW_INFO(dueInvalidParam, "z >= depth");
	}
	
	ds.GetClassColor()->PushColor(rt, nd.mutator->GetAt(nd.strideZ * z + nd.strideY * y + x));
}

// public func void getRange( Array pixels, int x, int y, int width, int height )
deClassEditableImage::nfGetRange::nfGetRange(const sInitData &init) :
dsFunction(init.clsEditableImage, "getRange", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsArray); // pixels
	p_AddParameter(init.clsInteger); // x
	p_AddParameter(init.clsInteger); // y
	p_AddParameter(init.clsInteger); // width
	p_AddParameter(init.clsInteger); // height
}
void deClassEditableImage::nfGetRange::RunFunction(dsRunTime *rt, dsValue *myself){
	const sImgPixNatDat &nd = *((sImgPixNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassEditableImage*)GetOwnerClass())->GetDS();
	dsClassArray &clsArray = *((dsClassArray*)ds.GetScriptEngine()->GetClassArray());
	deClassColor &clsColor = *ds.GetClassColor();
	
	dsRealObject * const pixels = rt->GetValue(0)->GetRealObject();
	if(!pixels){
		DSTHROW_INFO(dueNullPointer, "pixels");
	}
	
	const int x = rt->GetValue(1)->GetInt();
	const int y = rt->GetValue(2)->GetInt();
	const int width = rt->GetValue(3)->GetInt();
	const int height = rt->GetValue(4)->GetInt();
	
	if(x < 0){
		DSTHROW_INFO(dueInvalidParam, "x < 0");
	}
	if(width < 0){
		DSTHROW_INFO(dueInvalidParam, "width < 0");
	}
	if(x + width > nd.image->GetWidth()){
		DSTHROW_INFO(dueInvalidParam, "x + width > image.width");
	}
	if(y < 0){
		DSTHROW_INFO(dueInvalidParam, "y < 0");
	}
	if(height < 0){
		DSTHROW_INFO(dueInvalidParam, "height < 0");
	}
	if(y + height > nd.image->GetHeight()){
		DSTHROW_INFO(dueInvalidParam, "y + height > image.height");
	}
	
	const int count = clsArray.GetObjectCount(rt, pixels);
	if(count != width * height){
		DSTHROW_INFO(dueInvalidParam, "pixels.count != width * height");
	}
	
	const int pstrideY = width;
	int ix, iy;
	
	for(iy=0; iy<height; iy++){
		const int offsetY = nd.strideY * (y + iy) + x;
		const int poffsetY = pstrideY * iy;
		
		for(ix=0; ix<width; ix++){
			clsColor.PushColor(rt, nd.mutator->GetAt(offsetY + ix));
			clsArray.SetObjectAt(rt, pixels, poffsetY + ix, rt->GetValue(0)->GetRealObject());
			rt->RemoveValues(1);
		}
	}
}

// public func void getRange( Array pixels, int x, int y, int z, int width, int height, int depth )
deClassEditableImage::nfGetRange2::nfGetRange2(const sInitData &init) :
dsFunction(init.clsEditableImage, "getRange", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsArray); // pixels
	p_AddParameter(init.clsInteger); // x
	p_AddParameter(init.clsInteger); // y
	p_AddParameter(init.clsInteger); // z
	p_AddParameter(init.clsInteger); // width
	p_AddParameter(init.clsInteger); // height
	p_AddParameter(init.clsInteger); // depth
}
void deClassEditableImage::nfGetRange2::RunFunction(dsRunTime *rt, dsValue *myself){
	const sImgPixNatDat &nd = *((sImgPixNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassEditableImage*)GetOwnerClass())->GetDS();
	dsClassArray &clsArray = *((dsClassArray*)ds.GetScriptEngine()->GetClassArray());
	deClassColor &clsColor = *ds.GetClassColor();
	
	dsRealObject * const pixels = rt->GetValue(0)->GetRealObject();
	if(!pixels){
		DSTHROW_INFO(dueNullPointer, "pixels");
	}
	
	const int x = rt->GetValue(1)->GetInt();
	const int y = rt->GetValue(2)->GetInt();
	const int z = rt->GetValue(3)->GetInt();
	const int width = rt->GetValue(4)->GetInt();
	const int height = rt->GetValue(5)->GetInt();
	const int depth = rt->GetValue(6)->GetInt();
	
	if(x < 0){
		DSTHROW_INFO(dueInvalidParam, "x < 0");
	}
	if(width < 0){
		DSTHROW_INFO(dueInvalidParam, "width < 0");
	}
	if(x + width > nd.image->GetWidth()){
		DSTHROW_INFO(dueInvalidParam, "x + width > image.width");
	}
	if(y < 0){
		DSTHROW_INFO(dueInvalidParam, "y < 0");
	}
	if(height < 0){
		DSTHROW_INFO(dueInvalidParam, "height < 0");
	}
	if(y + height > nd.image->GetHeight()){
		DSTHROW_INFO(dueInvalidParam, "y + height > image.height");
	}
	if(z < 0){
		DSTHROW_INFO(dueInvalidParam, "z < 0");
	}
	if(depth < 0){
		DSTHROW_INFO(dueInvalidParam, "depth < 0");
	}
	if(z + depth > nd.image->GetDepth()){
		DSTHROW_INFO(dueInvalidParam, "z + depth > image.depth");
	}
	
	const int count = clsArray.GetObjectCount(rt, pixels);
	if(count != width * height * depth){
		DSTHROW_INFO(dueInvalidParam, "pixels.count != width * height * depth");
	}
	
	const int pstrideZ = width * height;
	const int pstrideY = width;
	int ix, iy, iz;
	
	for(iz=0; iz<depth; iz++){
		const int offsetZ = nd.strideZ * (z + iz) + nd.strideY * y + x;
		const int poffsetZ = pstrideZ * iz;
		
		for(iy=0; iy<height; iy++){
			const int offsetY = offsetZ + nd.strideY * iy;
			const int poffsetY = poffsetZ + pstrideY * iy;
			
			for(ix=0; ix<width; ix++){
				clsColor.PushColor(rt, nd.mutator->GetAt(offsetY + ix));
				clsArray.SetObjectAt(rt, pixels, poffsetY + ix, rt->GetValue(0)->GetRealObject());
				rt->RemoveValues(1);
			}
		}
	}
}



// public func void clear( Color color )
deClassEditableImage::nfClear::nfClear(const sInitData &init) :
dsFunction(init.clsEditableImage, "clear", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsColor); // color
}
void deClassEditableImage::nfClear::RunFunction(dsRunTime *rt, dsValue *myself){
	const sImgPixNatDat &nd = *((sImgPixNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassEditableImage*)GetOwnerClass())->GetDS();
	
	const decColor &color = ds.GetClassColor()->GetColor(rt->GetValue(0)->GetRealObject());
	const int count = nd.image->GetWidth() * nd.image->GetHeight() * nd.image->GetDepth();
	int i;
	
	for(i=0; i<count; i++){
		nd.mutator->SetAt(i, color);
	}
}

// public func void setAt( int x, int y, Color color )
deClassEditableImage::nfSetAt::nfSetAt(const sInitData &init) :
dsFunction(init.clsEditableImage, "setAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // x
	p_AddParameter(init.clsInteger); // y
	p_AddParameter(init.clsColor); // color
}
void deClassEditableImage::nfSetAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const sImgPixNatDat &nd = *((sImgPixNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassEditableImage*)GetOwnerClass())->GetDS();
	
	const int x = rt->GetValue(0)->GetInt();
	const int y = rt->GetValue(1)->GetInt();
	const decColor &color = ds.GetClassColor()->GetColor(rt->GetValue(2)->GetRealObject());
	
	if(x < 0){
		DSTHROW_INFO(dueInvalidParam, "x < 0");
	}
	if(x >= nd.image->GetWidth()){
		DSTHROW_INFO(dueInvalidParam, "x >= width");
	}
	if(y < 0){
		DSTHROW_INFO(dueInvalidParam, "y < 0");
	}
	if(y >= nd.image->GetHeight()){
		DSTHROW_INFO(dueInvalidParam, "y >= height");
	}
	
	nd.mutator->SetAt(nd.strideY * y + x, color);
}

// public func void setAt( int x, int y, int z, Color color )
deClassEditableImage::nfSetAt2::nfSetAt2(const sInitData &init) :
dsFunction(init.clsEditableImage, "setAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // x
	p_AddParameter(init.clsInteger); // y
	p_AddParameter(init.clsInteger); // z
	p_AddParameter(init.clsColor); // color
}
void deClassEditableImage::nfSetAt2::RunFunction(dsRunTime *rt, dsValue *myself){
	const sImgPixNatDat &nd = *((sImgPixNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassEditableImage*)GetOwnerClass())->GetDS();
	
	const int x = rt->GetValue(0)->GetInt();
	const int y = rt->GetValue(1)->GetInt();
	const int z = rt->GetValue(2)->GetInt();
	const decColor &color = ds.GetClassColor()->GetColor(rt->GetValue(3)->GetRealObject());
	
	if(x < 0){
		DSTHROW_INFO(dueInvalidParam, "x < 0");
	}
	if(x >= nd.image->GetWidth()){
		DSTHROW_INFO(dueInvalidParam, "x >= width");
	}
	if(y < 0){
		DSTHROW_INFO(dueInvalidParam, "y < 0");
	}
	if(y >= nd.image->GetHeight()){
		DSTHROW_INFO(dueInvalidParam, "y >= height");
	}
	if(z < 0){
		DSTHROW_INFO(dueInvalidParam, "z < 0");
	}
	if(z >= nd.image->GetDepth()){
		DSTHROW_INFO(dueInvalidParam, "z >= depth");
	}
	
	nd.mutator->SetAt(nd.strideZ * z + nd.strideY * y + x, color);
}

// public func void setRange( int x, int y, int width, int height, Array pixels )
deClassEditableImage::nfSetRange::nfSetRange(const sInitData &init) :
dsFunction(init.clsEditableImage, "setRange", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // x
	p_AddParameter(init.clsInteger); // y
	p_AddParameter(init.clsInteger); // width
	p_AddParameter(init.clsInteger); // height
	p_AddParameter(init.clsArray); // pixels
}
void deClassEditableImage::nfSetRange::RunFunction(dsRunTime *rt, dsValue *myself){
	sImgPixNatDat &nd = *((sImgPixNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassEditableImage*)GetOwnerClass())->GetDS();
	dsClassArray &clsArray = *((dsClassArray*)ds.GetScriptEngine()->GetClassArray());
	deClassColor &clsColor = *ds.GetClassColor();
	
	const int x = rt->GetValue(0)->GetInt();
	const int y = rt->GetValue(1)->GetInt();
	const int width = rt->GetValue(2)->GetInt();
	const int height = rt->GetValue(3)->GetInt();
	
	dsRealObject * const pixels = rt->GetValue(4)->GetRealObject();
	if(!pixels){
		DSTHROW_INFO(dueNullPointer, "pixels");
	}
	
	if(x < 0){
		DSTHROW_INFO(dueInvalidParam, "x < 0");
	}
	if(width < 0){
		DSTHROW_INFO(dueInvalidParam, "width < 0");
	}
	if(x + width > nd.image->GetWidth()){
		DSTHROW_INFO(dueInvalidParam, "x + width > image.width");
	}
	if(y < 0){
		DSTHROW_INFO(dueInvalidParam, "y < 0");
	}
	if(height < 0){
		DSTHROW_INFO(dueInvalidParam, "height < 0");
	}
	if(y + height > nd.image->GetHeight()){
		DSTHROW_INFO(dueInvalidParam, "y + height > image.height");
	}
	
	const int count = clsArray.GetObjectCount(rt, pixels);
	if(count != width * height){
		DSTHROW_INFO(dueInvalidParam, "pixels.count != width * height");
	}
	
	const int pstrideY = width;
	int ix, iy;
	
	for(iy=0; iy<height; iy++){
		const int offsetY = nd.strideY * (y + iy) + x;
		const int poffsetY = pstrideY * iy;
		
		for(ix=0; ix<width; ix++){
			const dsValue &value = *clsArray.GetObjectAt(rt, pixels, poffsetY + ix);
			if(!value.GetRealObject()){
				DSTHROW_INFO(dueNullPointer, "pixels contains null instance");
			}
			if(value.GetRealType() != &clsColor){
				DSTHROW_INFO(dseInvalidCast, ErrorCastInfo(&value, &clsColor));
			}
			
			nd.mutator->SetAt(offsetY + ix, clsColor.GetColor(value.GetRealObject()));
		}
	}
}

// public func void setRange( int x, int y, int z, int width, int height, int depth, Array pixels )
deClassEditableImage::nfSetRange2::nfSetRange2(const sInitData &init) :
dsFunction(init.clsEditableImage, "setRange", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // x
	p_AddParameter(init.clsInteger); // y
	p_AddParameter(init.clsInteger); // z
	p_AddParameter(init.clsInteger); // width
	p_AddParameter(init.clsInteger); // height
	p_AddParameter(init.clsInteger); // depth
	p_AddParameter(init.clsArray); // pixels
}
void deClassEditableImage::nfSetRange2::RunFunction(dsRunTime *rt, dsValue *myself){
	const sImgPixNatDat &nd = *((sImgPixNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassEditableImage*)GetOwnerClass())->GetDS();
	dsClassArray &clsArray = *((dsClassArray*)ds.GetScriptEngine()->GetClassArray());
	deClassColor &clsColor = *ds.GetClassColor();
	
	const int x = rt->GetValue(0)->GetInt();
	const int y = rt->GetValue(1)->GetInt();
	const int z = rt->GetValue(2)->GetInt();
	const int width = rt->GetValue(3)->GetInt();
	const int height = rt->GetValue(4)->GetInt();
	const int depth = rt->GetValue(5)->GetInt();
	
	dsRealObject * const pixels = rt->GetValue(6)->GetRealObject();
	if(!pixels){
		DSTHROW_INFO(dueNullPointer, "pixels");
	}
	
	if(x < 0){
		DSTHROW_INFO(dueInvalidParam, "x < 0");
	}
	if(width < 0){
		DSTHROW_INFO(dueInvalidParam, "width < 0");
	}
	if(x + width > nd.image->GetWidth()){
		DSTHROW_INFO(dueInvalidParam, "x + width > image.width");
	}
	if(y < 0){
		DSTHROW_INFO(dueInvalidParam, "y < 0");
	}
	if(height < 0){
		DSTHROW_INFO(dueInvalidParam, "height < 0");
	}
	if(y + height > nd.image->GetHeight()){
		DSTHROW_INFO(dueInvalidParam, "y + height > image.height");
	}
	if(z < 0){
		DSTHROW_INFO(dueInvalidParam, "z < 0");
	}
	if(depth < 0){
		DSTHROW_INFO(dueInvalidParam, "depth < 0");
	}
	if(z + depth > nd.image->GetDepth()){
		DSTHROW_INFO(dueInvalidParam, "z + depth > image.depth");
	}
	
	const int count = clsArray.GetObjectCount(rt, pixels);
	if(count != width * height * depth){
		DSTHROW_INFO(dueInvalidParam, "pixels.count != width * height * depth");
	}
	
	const int pstrideZ = width * height;
	const int pstrideY = width;
	int ix, iy, iz;
	
	for(iz=0; iz<depth; iz++){
		const int offsetZ = nd.strideZ * (z + iz) + nd.strideY * y + x;
		const int poffsetZ = pstrideZ * iz;
		
		for(iy=0; iy<height; iy++){
			const int offsetY = offsetZ + nd.strideY * iy;
			const int poffsetY = poffsetZ + pstrideY * iy;
			
			for(ix=0; ix<width; ix++){
				const dsValue &value = *clsArray.GetObjectAt(rt, pixels, poffsetY + ix);
				if(!value.GetRealObject()){
					DSTHROW_INFO(dueNullPointer, "pixels contains null instance");
				}
				if(value.GetRealType() != &clsColor){
					DSTHROW_INFO(dseInvalidCast, ErrorCastInfo(&value, &clsColor));
				}
				
				nd.mutator->SetAt(offsetY + ix, clsColor.GetColor(value.GetRealObject()));
			}
		}
	}
}

// public func void contentChanged()
deClassEditableImage::nfContentChanged::nfContentChanged(const sInitData &init) :
dsFunction(init.clsEditableImage, "contentChanged", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassEditableImage::nfContentChanged::RunFunction(dsRunTime*, dsValue *myself){
	const sImgPixNatDat &nd = *((sImgPixNatDat*)p_GetNativeData(myself));
	nd.image->NotifyImageDataChanged();
}



// Class deClassEditableImage
///////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassEditableImage::deClassEditableImage(deScriptingDragonScript &ds) :
dsClass("EditableImage", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS(ds){
	GetParserInfo()->SetParent(DENS_GUI);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sImgPixNatDat));
}

deClassEditableImage::~deClassEditableImage(){
}



// Management
///////////////

void deClassEditableImage::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	init.clsEditableImage = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsString = engine->GetClassString();
	init.clsInteger = engine->GetClassInt();
	init.clsObject = engine->GetClassObject();
	init.clsImage = pDS.GetClassImage();
	init.clsColor = pDS.GetClassColor();
	init.clsArray = engine->GetClassArray();
	
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetImage(init));
	AddFunction(new nfGetWidth(init));
	AddFunction(new nfGetHeight(init));
	AddFunction(new nfGetDepth(init));
	
	AddFunction(new nfGetAt(init));
	AddFunction(new nfGetAt2(init));
	AddFunction(new nfGetRange(init));
	AddFunction(new nfGetRange2(init));
	
	AddFunction(new nfClear(init));
	AddFunction(new nfSetAt(init));
	AddFunction(new nfSetAt2(init));
	AddFunction(new nfSetRange(init));
	AddFunction(new nfSetRange2(init));
	AddFunction(new nfContentChanged(init));
	
	CalcMemberOffsets();
}
