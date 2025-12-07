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
#include "deClassImagePixels.h"
#include "deClassColor.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassArray.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/image/deImage.h>



// Native Structure
/////////////////////

struct sImgPixNatDat{
	class Getter{
	protected:
		Getter(){}
		
	public:
		virtual ~Getter(){}
		virtual decColor GetAt(int index) const = 0;
	};
	
	
	class GetterGrayscale8 : public Getter{
	private:
		const sGrayscale8 *pData;
		const float pFactor;
		
	public:
		GetterGrayscale8(deImage &image) : pData(image.GetDataGrayscale8()), pFactor(1.0f / 255.0f){}
		virtual decColor GetAt(int index) const{
			const float value = pFactor * pData[index].value;
			return decColor(value, value, value);
		}
	};
	
	class GetterGrayscale16 : public Getter{
	private:
		const sGrayscale16 *pData;
		const float pFactor;
		
	public:
		GetterGrayscale16(deImage &image) : pData(image.GetDataGrayscale16()), pFactor(1.0f / 65535.0f){}
		virtual decColor GetAt(int index) const{
			const float value = pFactor * pData[index].value;
			return decColor(value, value, value);
		}
	};
	
	class GetterGrayscale32 : public Getter{
	private:
		const sGrayscale32 *pData;
		
	public:
		GetterGrayscale32(deImage &image) : pData(image.GetDataGrayscale32()){}
		virtual decColor GetAt(int index) const{
			const float &value = pData[index].value;
			return decColor(value, value, value);
		}
	};
	
	
	class GetterGrayscaleAlpha8 : public Getter{
	private:
		const sGrayscaleAlpha8 *pData;
		const float pFactor;
		
	public:
		GetterGrayscaleAlpha8(deImage &image) : pData(image.GetDataGrayscaleAlpha8()), pFactor(1.0f / 255.0f){}
		virtual decColor GetAt(int index) const{
			const float value = pFactor * pData[index].value;
			return decColor(value, value, value, pFactor * pData[index].alpha);
		}
	};
	
	class GetterGrayscaleAlpha16 : public Getter{
	private:
		const sGrayscaleAlpha16 *pData;
		const float pFactor;
		
	public:
		GetterGrayscaleAlpha16(deImage &image) : pData(image.GetDataGrayscaleAlpha16()), pFactor(1.0f / 65535.0f){}
		virtual decColor GetAt(int index) const{
			const float value = pFactor * pData[index].value;
			return decColor(value, value, value, pFactor * pData[index].alpha);
		}
	};
	
	class GetterGrayscaleAlpha32 : public Getter{
	private:
		const sGrayscaleAlpha32 *pData;
		
	public:
		GetterGrayscaleAlpha32(deImage &image) : pData(image.GetDataGrayscaleAlpha32()){}
		virtual decColor GetAt(int index) const{
			const float &value = pData[index].value;
			return decColor(value, value, value, pData[index].alpha);
		}
	};
	
	
	class GetterRGB8 : public Getter{
	private:
		const sRGB8 *pData;
		const float pFactor;
		
	public:
		GetterRGB8(deImage &image) : pData(image.GetDataRGB8()), pFactor(1.0f / 255.0f){}
		virtual decColor GetAt(int index) const{
			return decColor(pFactor * pData[index].red,
				pFactor * pData[index].green,
				pFactor * pData[index].blue);
		}
	};
	
	class GetterRGB16 : public Getter{
	private:
		const sRGB16 *pData;
		const float pFactor;
		
	public:
		GetterRGB16(deImage &image) : pData(image.GetDataRGB16()), pFactor(1.0f / 65535.0f){}
		virtual decColor GetAt(int index) const{
			return decColor(pFactor * pData[index].red,
				pFactor * pData[index].green,
				pFactor * pData[index].blue);
		}
	};
	
	class GetterRGB32 : public Getter{
	private:
		const sRGB32 *pData;
		
	public:
		GetterRGB32(deImage &image) : pData(image.GetDataRGB32()){}
		virtual decColor GetAt(int index) const{
			return decColor(pData[index].red, pData[index].green, pData[index].blue);
		}
	};
	
	
	class GetterRGBA8 : public Getter{
	private:
		const sRGBA8 *pData;
		const float pFactor;
		
	public:
		GetterRGBA8(deImage &image) : pData(image.GetDataRGBA8()), pFactor(1.0f / 255.0f){}
		virtual decColor GetAt(int index) const{
			return decColor(pFactor * pData[index].red,
				pFactor * pData[index].green,
				pFactor * pData[index].blue,
				pFactor * pData[index].alpha);
		}
	};
	
	class GetterRGBA16 : public Getter{
	private:
		const sRGBA16 *pData;
		const float pFactor;
		
	public:
		GetterRGBA16(deImage &image) : pData(image.GetDataRGBA16()), pFactor(1.0f / 65535.0f){}
		virtual decColor GetAt(int index) const{
			return decColor(pFactor * pData[index].red,
				pFactor * pData[index].green,
				pFactor * pData[index].blue,
				pFactor * pData[index].alpha);
		}
	};
	
	class GetterRGBA32 : public Getter{
	private:
		const sRGBA32 *pData;
		
	public:
		GetterRGBA32(deImage &image) : pData(image.GetDataRGBA32()){}
		virtual decColor GetAt(int index) const{
			return decColor(pData[index].red, pData[index].green, pData[index].blue, pData[index].alpha);
		}
	};
	
	
	deImage *image;
	Getter *getter;
	int strideZ;
	int strideY;
};



// Native Functions
/////////////////////

// public func new( Image image )
deClassImagePixels::nfNew::nfNew(const sInitData &init) :
dsFunction(init.clsImagePixels, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsImage); // image
}
void deClassImagePixels::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sImgPixNatDat &nd = *((sImgPixNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassImagePixels*)GetOwnerClass())->GetDS();
	
	// clear ( important )
	nd.image = NULL;
	nd.getter = NULL;
	nd.strideZ = 0;
	nd.strideY = 0;
	
	// check parameters
	deImage * const image = ds.GetClassImage()->GetImage(rt->GetValue(0)->GetRealObject());
	if(!image){
		DSTHROW(dueNullPointer);
	}
	
	// retain image
	image->RetainImageData();
	
	// create getter
	switch(image->GetComponentCount()){
	case 1:
		switch(image->GetBitCount()){
		case 8:
			nd.getter = new sImgPixNatDat::GetterGrayscale8(*image);
			break;
			
		case 16:
			nd.getter = new sImgPixNatDat::GetterGrayscale16(*image);
			break;
			
		case 32:
			nd.getter = new sImgPixNatDat::GetterGrayscale32(*image);
			break;
			
		default:
			DSTHROW(dueInvalidParam);
		}
		break;
		
	case 2:
		switch(image->GetBitCount()){
		case 8:
			nd.getter = new sImgPixNatDat::GetterGrayscaleAlpha8(*image);
			break;
			
		case 16:
			nd.getter = new sImgPixNatDat::GetterGrayscaleAlpha16(*image);
			break;
			
		case 32:
			nd.getter = new sImgPixNatDat::GetterGrayscaleAlpha32(*image);
			break;
			
		default:
			DSTHROW(dueInvalidParam);
		}
		break;
		
	case 3:
		switch(image->GetBitCount()){
		case 8:
			nd.getter = new sImgPixNatDat::GetterRGB8(*image);
			break;
			
		case 16:
			nd.getter = new sImgPixNatDat::GetterRGB16(*image);
			break;
			
		case 32:
			nd.getter = new sImgPixNatDat::GetterRGB32(*image);
			break;
			
		default:
			DSTHROW(dueInvalidParam);
		}
		break;
		
	case 4:
		switch(image->GetBitCount()){
		case 8:
			nd.getter = new sImgPixNatDat::GetterRGBA8(*image);
			break;
			
		case 16:
			nd.getter = new sImgPixNatDat::GetterRGBA16(*image);
			break;
			
		case 32:
			nd.getter = new sImgPixNatDat::GetterRGBA32(*image);
			break;
			
		default:
			DSTHROW(dueInvalidParam);
		}
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
	
	// store
	nd.image = image;
	image->AddReference();
	
	nd.strideZ = image->GetWidth() * image->GetHeight();
	nd.strideY = image->GetWidth();
}

// public func destructor()
deClassImagePixels::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsImagePixels,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassImagePixels::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sImgPixNatDat &nd = *((sImgPixNatDat*)p_GetNativeData(myself));
	
	if(nd.getter){
		delete nd.getter;
		nd.getter = NULL;
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
deClassImagePixels::nfGetImage::nfGetImage(const sInitData &init) :
dsFunction(init.clsImagePixels, "getImage", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsImage){
}
void deClassImagePixels::nfGetImage::RunFunction(dsRunTime *rt, dsValue *myself){
	const sImgPixNatDat &nd = *((sImgPixNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassImagePixels*)GetOwnerClass())->GetDS();
	
	ds.GetClassImage()->PushImage(rt, nd.image);
}

// public func int getWidth()
deClassImagePixels::nfGetWidth::nfGetWidth(const sInitData &init) :
dsFunction(init.clsImagePixels, "getWidth", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}
void deClassImagePixels::nfGetWidth::RunFunction(dsRunTime *rt, dsValue *myself){
	const sImgPixNatDat &nd = *((sImgPixNatDat*)p_GetNativeData(myself));
	
	rt->PushInt(nd.image->GetWidth());
}

// public func int getHeight()
deClassImagePixels::nfGetHeight::nfGetHeight(const sInitData &init) :
dsFunction(init.clsImagePixels, "getHeight", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}
void deClassImagePixels::nfGetHeight::RunFunction(dsRunTime *rt, dsValue *myself){
	const sImgPixNatDat &nd = *((sImgPixNatDat*)p_GetNativeData(myself));
	
	rt->PushInt(nd.image->GetHeight());
}

// public func int getDepth()
deClassImagePixels::nfGetDepth::nfGetDepth(const sInitData &init) :
dsFunction(init.clsImagePixels, "getDepth", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}
void deClassImagePixels::nfGetDepth::RunFunction(dsRunTime *rt, dsValue *myself){
	const sImgPixNatDat &nd = *((sImgPixNatDat*)p_GetNativeData(myself));
	
	rt->PushInt(nd.image->GetDepth());
}

// public func Color getAt( int x, int y )
deClassImagePixels::nfGetAt::nfGetAt(const sInitData &init) :
dsFunction(init.clsImagePixels, "getAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsColor){
	p_AddParameter(init.clsInteger); // x
	p_AddParameter(init.clsInteger); // y
}
void deClassImagePixels::nfGetAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const sImgPixNatDat &nd = *((sImgPixNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassImagePixels*)GetOwnerClass())->GetDS();
	
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
	
	ds.GetClassColor()->PushColor(rt, nd.getter->GetAt(nd.strideY * y + x));
}

// public func Color getAt( int x, int y, int z )
deClassImagePixels::nfGetAt2::nfGetAt2(const sInitData &init) :
dsFunction(init.clsImagePixels, "getAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsColor){
	p_AddParameter(init.clsInteger); // x
	p_AddParameter(init.clsInteger); // y
	p_AddParameter(init.clsInteger); // z
}
void deClassImagePixels::nfGetAt2::RunFunction(dsRunTime *rt, dsValue *myself){
	const sImgPixNatDat &nd = *((sImgPixNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassImagePixels*)GetOwnerClass())->GetDS();
	
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
	
	ds.GetClassColor()->PushColor(rt, nd.getter->GetAt(nd.strideZ * z + nd.strideY * y + x));
}

// public func void getRange( Array pixels, int x, int y, int width, int height )
deClassImagePixels::nfGetRange::nfGetRange(const sInitData &init) :
dsFunction(init.clsImagePixels, "getRange", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsArray); // pixels
	p_AddParameter(init.clsInteger); // x
	p_AddParameter(init.clsInteger); // y
	p_AddParameter(init.clsInteger); // width
	p_AddParameter(init.clsInteger); // height
}
void deClassImagePixels::nfGetRange::RunFunction(dsRunTime *rt, dsValue *myself){
	const sImgPixNatDat &nd = *((sImgPixNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassImagePixels*)GetOwnerClass())->GetDS();
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
			clsColor.PushColor(rt, nd.getter->GetAt(offsetY + ix));
			clsArray.SetObjectAt(rt, pixels, poffsetY + ix, rt->GetValue(0)->GetRealObject());
			rt->RemoveValues(1);
		}
	}
}

// public func void getRange( Array pixels, int x, int y, int z, int width, int height, int depth )
deClassImagePixels::nfGetRange2::nfGetRange2(const sInitData &init) :
dsFunction(init.clsImagePixels, "getRange", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsArray); // pixels
	p_AddParameter(init.clsInteger); // x
	p_AddParameter(init.clsInteger); // y
	p_AddParameter(init.clsInteger); // z
	p_AddParameter(init.clsInteger); // width
	p_AddParameter(init.clsInteger); // height
	p_AddParameter(init.clsInteger); // depth
}
void deClassImagePixels::nfGetRange2::RunFunction(dsRunTime *rt, dsValue *myself){
	const sImgPixNatDat &nd = *((sImgPixNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassImagePixels*)GetOwnerClass())->GetDS();
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
				clsColor.PushColor(rt, nd.getter->GetAt(offsetY + ix));
				clsArray.SetObjectAt(rt, pixels, poffsetY + ix, rt->GetValue(0)->GetRealObject());
				rt->RemoveValues(1);
			}
		}
	}
}



// Class deClassImagePixels
/////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassImagePixels::deClassImagePixels(deScriptingDragonScript &ds) :
dsClass("ImagePixels", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS(ds){
	GetParserInfo()->SetParent(DENS_GUI);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sImgPixNatDat));
}

deClassImagePixels::~deClassImagePixels(){
}



// Management
///////////////

void deClassImagePixels::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	init.clsImagePixels = this;
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
	
	CalcMemberOffsets();
}
