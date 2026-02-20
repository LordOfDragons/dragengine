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

#include <new>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "deClassShapeList.h"
#include "../dedsHelpers.h"
#include "../file/deClassFileReader.h"
#include "../file/deClassFileWriter.h"
#include "../math/deClassQuaternion.h"
#include "../math/deClassVector.h"
#include "../math/deClassVector2.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassArray.h>
#include <libdscript/packages/default/dsClassEnumeration.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeCylinder.h>
#include <dragengine/common/shape/decShapeCapsule.h>
#include <dragengine/common/shape/decShapeHull.h>
#include <dragengine/common/shape/decShapeVisitor.h>
#include <dragengine/common/shape/decShapeVisitorIdentify.h>


struct sShaListNatDat{
	decShape::List shapeList;
};



// Constructors, Destructors
//////////////////////////////

// public func new()
deClassShapeList::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsShaList,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassShapeList::nfNew::RunFunction(dsRunTime*, dsValue *myself){
	dedsNewNativeData<sShaListNatDat>(p_GetNativeData(myself));
}

// public func new( ShapeList shapeList )
deClassShapeList::nfNew2::nfNew2(const sInitData &init) : dsFunction(init.clsShaList,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsShaList); // shapeList
}
void deClassShapeList::nfNew2::RunFunction(dsRunTime *rt, dsValue *myself){
	sShaListNatDat &nd = dedsNewNativeData<sShaListNatDat>(p_GetNativeData(myself));
	
	const deClassShapeList &clsShaList = *(static_cast<deClassShapeList*>(GetOwnerClass()));
	
	nd.shapeList = clsShaList.GetShapeList(rt->GetValue(0)->GetRealObject());
}

// public func destructor()
deClassShapeList::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsShaList,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassShapeList::nfDestructor::RunFunction(dsRunTime*, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).~sShaListNatDat();
}



// Management
///////////////

// public func int getCount()
deClassShapeList::nfGetCount::nfGetCount(const sInitData &init) : dsFunction(init.clsShaList,
"getCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassShapeList::nfGetCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const decShape::List &shapeList = dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList;
	rt->PushInt(shapeList.GetCount());
}

// public func bool empty()
deClassShapeList::nfEmpty::nfEmpty(const sInitData &init) : dsFunction(init.clsShaList,
"empty", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassShapeList::nfEmpty::RunFunction(dsRunTime *rt, dsValue *myself){
	const decShape::List &shapeList = dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList;
	rt->PushBool(shapeList.GetCount() == 0);
}

// public func bool notEmpty()
deClassShapeList::nfNotEmpty::nfNotEmpty(const sInitData &init) : dsFunction(init.clsShaList,
"notEmpty", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassShapeList::nfNotEmpty::RunFunction(dsRunTime *rt, dsValue *myself){
	const decShape::List &shapeList = dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList;
	rt->PushBool(shapeList.GetCount() > 0);
}

// public func void removeAllShapes()
deClassShapeList::nfRemoveAllShapes::nfRemoveAllShapes(const sInitData &init) : dsFunction(init.clsShaList,
"removeAllShapes", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassShapeList::nfRemoveAllShapes::RunFunction(dsRunTime*, dsValue *myself){
	decShape::List &shapeList = dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList;
	shapeList.RemoveAll();
}



// public func ShapeType getTypeAt( int index )
deClassShapeList::nfGetTypeAt::nfGetTypeAt(const sInitData &init) :
dsFunction(init.clsShaList, "getTypeAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsShapeType){
	p_AddParameter(init.clsInt); // index
}
void deClassShapeList::nfGetTypeAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const decShape::List &shapeList = dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList;
	decShapeVisitorIdentify identify;
	shapeList.GetAt(rt->GetValue(0)->GetInt())->Visit(identify);
	rt->PushValue((static_cast<deClassShapeList*>(GetOwnerClass()))->GetClassShapeType()
		->GetVariable(identify.GetType() - 1)->GetStaticValue());
}

// public func Vector getPositionAt( int index )
deClassShapeList::nfGetPositionAt::nfGetPositionAt(const sInitData &init) :
dsFunction(init.clsShaList, "getPositionAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
	p_AddParameter(init.clsInt); // index
}
void deClassShapeList::nfGetPositionAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const decShape::List &shapeList = dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList;
	const deScriptingDragonScript &ds = *((static_cast<deClassShapeList*>(GetOwnerClass()))->GetDS());
	ds.GetClassVector()->PushVector(rt, shapeList.GetAt(rt->GetValue(0)->GetInt())->GetPosition());
}

// public func Quaternion getOrientationAt( int index )
deClassShapeList::nfGetOrientationAt::nfGetOrientationAt(const sInitData &init) :
dsFunction(init.clsShaList, "getOrientationAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat){
	p_AddParameter(init.clsInt); // index
}
void deClassShapeList::nfGetOrientationAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const decShape::List &shapeList = dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList;
	const deScriptingDragonScript &ds = *((static_cast<deClassShapeList*>(GetOwnerClass()))->GetDS());
	ds.GetClassQuaternion()->PushQuaternion(rt, shapeList.GetAt(rt->GetValue(0)->GetInt())->GetOrientation());
}

// public func float getRadiusAt( int index )
deClassShapeList::nfGetRadiusAt::nfGetRadiusAt(const sInitData &init) :
dsFunction(init.clsShaList, "getRadiusAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
	p_AddParameter(init.clsInt); // index
}
void deClassShapeList::nfGetRadiusAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const decShape::List &shapeList = dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList;
	decShapeVisitorIdentify identify;
	shapeList.GetAt(rt->GetValue(0)->GetInt())->Visit(identify);
	rt->PushFloat(identify.CastToSphere().GetRadius());
}

// public func Vector getHalfExtendsAt( int index )
deClassShapeList::nfGetHalfExtendsAt::nfGetHalfExtendsAt(const sInitData &init) :
dsFunction(init.clsShaList, "getHalfExtendsAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
	p_AddParameter(init.clsInt); // index
}
void deClassShapeList::nfGetHalfExtendsAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const decShape::List &shapeList = dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList;
	const deScriptingDragonScript &ds = *((static_cast<deClassShapeList*>(GetOwnerClass()))->GetDS());
	decShapeVisitorIdentify identify;
	shapeList.GetAt(rt->GetValue(0)->GetInt())->Visit(identify);
	ds.GetClassVector()->PushVector(rt, identify.CastToBox().GetHalfExtends());
}

// public func float getHalfHeightAt( int index )
deClassShapeList::nfGetHalfHeightAt::nfGetHalfHeightAt(const sInitData &init) :
dsFunction(init.clsShaList, "getHalfHeightAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
	p_AddParameter(init.clsInt); // index
}
void deClassShapeList::nfGetHalfHeightAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const decShape::List &shapeList = dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList;
	decShapeVisitorIdentify identify;
	shapeList.GetAt(rt->GetValue(0)->GetInt())->Visit(identify);
	switch(identify.GetType()){
	case decShapeVisitorIdentify::estCylinder:
		rt->PushFloat(identify.CastToCylinder().GetHalfHeight());
		break;
		
	case decShapeVisitorIdentify::estCapsule:
		rt->PushFloat(identify.CastToCapsule().GetHalfHeight());
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
}

// public func float getTopRadiusAt( int index )
deClassShapeList::nfGetTopRadiusAt::nfGetTopRadiusAt(const sInitData &init) :
dsFunction(init.clsShaList, "getTopRadiusAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
	p_AddParameter(init.clsInt); // index
}
void deClassShapeList::nfGetTopRadiusAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const decShape::List &shapeList = dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList;
	decShapeVisitorIdentify identify;
	shapeList.GetAt(rt->GetValue(0)->GetInt())->Visit(identify);
	switch(identify.GetType()){
	case decShapeVisitorIdentify::estCylinder:
		rt->PushFloat(identify.CastToCylinder().GetTopRadius());
		break;
		
	case decShapeVisitorIdentify::estCapsule:
		rt->PushFloat(identify.CastToCapsule().GetTopRadius());
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
}

// public func float getBottomRadiusAt( int index )
deClassShapeList::nfGetBottomRadiusAt::nfGetBottomRadiusAt(const sInitData &init) :
dsFunction(init.clsShaList, "getBottomRadiusAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
	p_AddParameter(init.clsInt); // index
}
void deClassShapeList::nfGetBottomRadiusAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const decShape::List &shapeList = dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList;
	decShapeVisitorIdentify identify;
	shapeList.GetAt(rt->GetValue(0)->GetInt())->Visit(identify);
	switch(identify.GetType()){
	case decShapeVisitorIdentify::estCylinder:
		rt->PushFloat(identify.CastToCylinder().GetBottomRadius());
		break;
		
	case decShapeVisitorIdentify::estCapsule:
		rt->PushFloat(identify.CastToCapsule().GetBottomRadius());
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
}

// public func Vector2 getTopAxisScalingAt( int index )
deClassShapeList::nfGetTopAxisScalingAt::nfGetTopAxisScalingAt(const sInitData &init) :
dsFunction(init.clsShaList, "getTopAxisScalingAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVec2){
	p_AddParameter(init.clsInt); // index
}
void deClassShapeList::nfGetTopAxisScalingAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const decShape::List &shapeList = dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList;
	const deScriptingDragonScript &ds = *((static_cast<deClassShapeList*>(GetOwnerClass()))->GetDS());
	decShapeVisitorIdentify identify;
	shapeList.GetAt(rt->GetValue(0)->GetInt())->Visit(identify);
	switch(identify.GetType()){
	case decShapeVisitorIdentify::estCylinder:
		ds.GetClassVector2()->PushVector2(rt, identify.CastToCylinder().GetTopAxisScaling());
		break;
		
	case decShapeVisitorIdentify::estCapsule:
		ds.GetClassVector2()->PushVector2(rt, identify.CastToCapsule().GetTopAxisScaling());
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
}

// public func Vector2 getBottomAxisScalingAt( int index )
deClassShapeList::nfGetBottomAxisScalingAt::nfGetBottomAxisScalingAt(const sInitData &init) :
dsFunction(init.clsShaList, "getBottomAxisScalingAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVec2){
	p_AddParameter(init.clsInt); // index
}
void deClassShapeList::nfGetBottomAxisScalingAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const decShape::List &shapeList = dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList;
	const deScriptingDragonScript &ds = *((static_cast<deClassShapeList*>(GetOwnerClass()))->GetDS());
	decShapeVisitorIdentify identify;
	shapeList.GetAt(rt->GetValue(0)->GetInt())->Visit(identify);
	switch(identify.GetType()){
	case decShapeVisitorIdentify::estCylinder:
		ds.GetClassVector2()->PushVector2(rt, identify.CastToCylinder().GetBottomAxisScaling());
		break;
		
	case decShapeVisitorIdentify::estCapsule:
		ds.GetClassVector2()->PushVector2(rt, identify.CastToCapsule().GetBottomAxisScaling());
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
}

// public func int getPointCountAt( int index )
deClassShapeList::nfGetPointCountAt::nfGetPointCountAt(const sInitData &init) :
dsFunction(init.clsShaList, "getPointCountAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // index
}
void deClassShapeList::nfGetPointCountAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const decShape::List &shapeList = dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList;
	decShapeVisitorIdentify identify;
	shapeList.GetAt(rt->GetValue(0)->GetInt())->Visit(identify);
	rt->PushInt(identify.CastToHull().GetPoints().GetCount());
}

// public func Vector getPointAt( int shape, int point )
deClassShapeList::nfGetPointAt::nfGetPointAt(const sInitData &init) :
dsFunction(init.clsShaList, "getPointAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
	p_AddParameter(init.clsInt); // shape
	p_AddParameter(init.clsInt); // point
}
void deClassShapeList::nfGetPointAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const decShape::List &shapeList = dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList;
	const deScriptingDragonScript &ds = *((static_cast<deClassShapeList*>(GetOwnerClass()))->GetDS());
	decShapeVisitorIdentify identify;
	shapeList.GetAt(rt->GetValue(0)->GetInt())->Visit(identify);
	ds.GetClassVector()->PushVector(rt, identify.CastToHull().GetPoints().GetAt(rt->GetValue(1)->GetInt()));
}



// public func void addSphere( Vector position, float radius )
deClassShapeList::nfAddSphere::nfAddSphere(const sInitData &init) : dsFunction(init.clsShaList,
"addSphere", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // position
	p_AddParameter(init.clsFlt); // radius
}
void deClassShapeList::nfAddSphere::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = *((static_cast<deClassShapeList*>(GetOwnerClass()))->GetDS());
	
	const decVector &position = ds.GetClassVector()->GetVector(rt->GetValue(0)->GetRealObject());
	const float radius = rt->GetValue(1)->GetFloat();
	
	dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList.Add(
		decShapeSphere::Ref::New(radius, position));
}



// public func void addBox( Vector center, Vector extends )
deClassShapeList::nfAddBox::nfAddBox(const sInitData &init) : dsFunction(init.clsShaList,
"addBox", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // center
	p_AddParameter(init.clsVec); // extends
}
void deClassShapeList::nfAddBox::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = *((static_cast<deClassShapeList*>(GetOwnerClass()))->GetDS());
	
	const decVector &center = ds.GetClassVector()->GetVector(rt->GetValue(0)->GetRealObject());
	const decVector &extends = ds.GetClassVector()->GetVector(rt->GetValue(1)->GetRealObject());
	
	dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList.Add(
		decShapeBox::Ref::New(extends, center));
}

// public func void addBox( Vector center, Vector extends, Quaternion orientation )
deClassShapeList::nfAddBox2::nfAddBox2(const sInitData &init) : dsFunction(init.clsShaList,
"addBox", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // center
	p_AddParameter(init.clsVec); // extends
	p_AddParameter(init.clsQuat); // orientation
}
void deClassShapeList::nfAddBox2::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = *((static_cast<deClassShapeList*>(GetOwnerClass()))->GetDS());
	
	const decVector &center = ds.GetClassVector()->GetVector(rt->GetValue(0)->GetRealObject());
	const decVector &extends = ds.GetClassVector()->GetVector(rt->GetValue(1)->GetRealObject());
	const decQuaternion &orientation = ds.GetClassQuaternion()->GetQuaternion(rt->GetValue(2)->GetRealObject());
	
	dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList.Add(
		decShapeBox::Ref::New(extends, center, orientation));
}

// public func void addBox( Vector center, Vector extends, Vector2 tapering )
deClassShapeList::nfAddBox3::nfAddBox3(const sInitData &init) :
dsFunction(init.clsShaList, "addBox", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // center
	p_AddParameter(init.clsVec); // extends
	p_AddParameter(init.clsVec2); // tapering
}

void deClassShapeList::nfAddBox3::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = *((static_cast<deClassShapeList*>(GetOwnerClass()))->GetDS());
	
	const decVector &center = ds.GetClassVector()->GetVector(rt->GetValue(0)->GetRealObject());
	const decVector &extends = ds.GetClassVector()->GetVector(rt->GetValue(1)->GetRealObject());
	const decVector2 &tapering = ds.GetClassVector2()->GetVector2(rt->GetValue(2)->GetRealObject());
	
	dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList.Add(
		decShapeBox::Ref::New(extends, tapering, center));
}

// public func void addBox( Vector center, Vector extends, Vector2 tapering, Quaternion orientation )
deClassShapeList::nfAddBox4::nfAddBox4(const sInitData &init) :
dsFunction(init.clsShaList, "addBox", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // center
	p_AddParameter(init.clsVec); // extends
	p_AddParameter(init.clsVec2); // tapering
	p_AddParameter(init.clsQuat); // orientation
}

void deClassShapeList::nfAddBox4::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = *((static_cast<deClassShapeList*>(GetOwnerClass()))->GetDS());
	
	const decVector &center = ds.GetClassVector()->GetVector(rt->GetValue(0)->GetRealObject());
	const decVector &extends = ds.GetClassVector()->GetVector(rt->GetValue(1)->GetRealObject());
	const decVector2 &tapering = ds.GetClassVector2()->GetVector2(rt->GetValue(2)->GetRealObject());
	const decQuaternion &orientation = ds.GetClassQuaternion()->GetQuaternion(rt->GetValue(3)->GetRealObject());
	
	dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList.Add(
		decShapeBox::Ref::New(extends, tapering, center, orientation));
}



// public func void addCylinder( float halfHeight, float radius )
deClassShapeList::nfAddCylinder::nfAddCylinder(const sInitData &init) : dsFunction(init.clsShaList,
"addCylinder", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // halfHeight
	p_AddParameter(init.clsFlt); // radius
}
void deClassShapeList::nfAddCylinder::RunFunction(dsRunTime *rt, dsValue *myself){
	
	const float halfHeight(rt->GetValue(0)->GetFloat());
	const float radius(rt->GetValue(1)->GetFloat());
	
	dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList.Add(
		decShapeCylinder::Ref::New(halfHeight, radius));
}

// public func void addCylinder( float halfHeight, float radius, Vector position )
deClassShapeList::nfAddCylinder2::nfAddCylinder2(const sInitData &init) : dsFunction(init.clsShaList,
"addCylinder", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // halfHeight
	p_AddParameter(init.clsFlt); // radius
	p_AddParameter(init.clsVec); // position
}
void deClassShapeList::nfAddCylinder2::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = *((static_cast<deClassShapeList*>(GetOwnerClass()))->GetDS());
	
	const float halfHeight(rt->GetValue(0)->GetFloat());
	const float radius(rt->GetValue(1)->GetFloat());
	const decVector &position = ds.GetClassVector()->GetVector(rt->GetValue(2)->GetRealObject());
	
	dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList.Add(
		decShapeCylinder::Ref::New(halfHeight, radius, position));
}

// public func void addCylinder( float halfHeight, float radius, Vector position, Quaternion orientation )
deClassShapeList::nfAddCylinder3::nfAddCylinder3(const sInitData &init) : dsFunction(init.clsShaList,
"addCylinder", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // halfHeight
	p_AddParameter(init.clsFlt); // radius
	p_AddParameter(init.clsVec); // position
	p_AddParameter(init.clsQuat); // orientation
}
void deClassShapeList::nfAddCylinder3::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = *((static_cast<deClassShapeList*>(GetOwnerClass()))->GetDS());
	
	const float halfHeight(rt->GetValue(0)->GetFloat());
	const float radius(rt->GetValue(1)->GetFloat());
	const decVector &position = ds.GetClassVector()->GetVector(rt->GetValue(2)->GetRealObject());
	const decQuaternion &orientation = ds.GetClassQuaternion()->GetQuaternion(rt->GetValue(3)->GetRealObject());
	
	dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList.Add(
		decShapeCylinder::Ref::New(halfHeight, radius, position, orientation));
}

// public func void addCylinder( float halfHeight, float topRadius, float bottomRadius )
deClassShapeList::nfAddCylinder4::nfAddCylinder4(const sInitData &init) : dsFunction(init.clsShaList,
"addCylinder", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // halfHeight
	p_AddParameter(init.clsFlt); // radius
	p_AddParameter(init.clsFlt); // bottomRadius
}
void deClassShapeList::nfAddCylinder4::RunFunction(dsRunTime *rt, dsValue *myself){
	
	const float halfHeight(rt->GetValue(0)->GetFloat());
	const float radius(rt->GetValue(1)->GetFloat());
	const float bottomRadius(rt->GetValue(2)->GetFloat());
	
	dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList.Add(
		decShapeCylinder::Ref::New(halfHeight, radius, bottomRadius));
}

// public func void addCylinder( float halfHeight, float topRadius,
// float bottomRadius, Vector position )
deClassShapeList::nfAddCylinder5::nfAddCylinder5(const sInitData &init) : dsFunction(init.clsShaList,
"addCylinder", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // halfHeight
	p_AddParameter(init.clsFlt); // radius
	p_AddParameter(init.clsFlt); // bottomRadius
	p_AddParameter(init.clsVec); // position
}
void deClassShapeList::nfAddCylinder5::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = *((static_cast<deClassShapeList*>(GetOwnerClass()))->GetDS());
	
	const float halfHeight(rt->GetValue(0)->GetFloat());
	const float radius(rt->GetValue(1)->GetFloat());
	const float bottomRadius(rt->GetValue(2)->GetFloat());
	const decVector &position = ds.GetClassVector()->GetVector(rt->GetValue(3)->GetRealObject());
	
	dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList.Add(
		decShapeCylinder::Ref::New(halfHeight, radius, bottomRadius, position));
}

// public func void addCylinder( float halfHeight, float topRadius,
// float bottomRadius, Vector position, Quaternion orientation )
deClassShapeList::nfAddCylinder6::nfAddCylinder6(const sInitData &init) : dsFunction(init.clsShaList,
"addCylinder", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // halfHeight
	p_AddParameter(init.clsFlt); // radius
	p_AddParameter(init.clsFlt); // bottomRadius
	p_AddParameter(init.clsVec); // position
	p_AddParameter(init.clsQuat); // orientation
}
void deClassShapeList::nfAddCylinder6::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = *((static_cast<deClassShapeList*>(GetOwnerClass()))->GetDS());
	
	const float halfHeight(rt->GetValue(0)->GetFloat());
	const float radius(rt->GetValue(1)->GetFloat());
	const float bottomRadius(rt->GetValue(2)->GetFloat());
	const decVector &position = ds.GetClassVector()->GetVector(rt->GetValue(3)->GetRealObject());
	const decQuaternion &orientation = ds.GetClassQuaternion()->GetQuaternion(rt->GetValue(4)->GetRealObject());
	
	dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList.Add(
		decShapeCylinder::Ref::New(halfHeight, radius, bottomRadius, position, orientation));
}

// public func void addCylinder( float halfHeight, float topRadius, float bottomRadius,
// Vector2 topAxisScaling, Vector2 bottomAxisScaling )
deClassShapeList::nfAddCylinder7::nfAddCylinder7(const sInitData &init) : dsFunction(init.clsShaList,
"addCylinder", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // halfHeight
	p_AddParameter(init.clsFlt); // radius
	p_AddParameter(init.clsFlt); // bottomRadius
	p_AddParameter(init.clsVec2); // topAxisScaling
	p_AddParameter(init.clsVec2); // bottomAxisScaling
}
void deClassShapeList::nfAddCylinder7::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = *((static_cast<deClassShapeList*>(GetOwnerClass()))->GetDS());
	
	const float halfHeight(rt->GetValue(0)->GetFloat());
	const float radius(rt->GetValue(1)->GetFloat());
	const float bottomRadius(rt->GetValue(2)->GetFloat());
	const decVector2 &topAxisScaling = ds.GetClassVector2()->GetVector2(rt->GetValue(3)->GetRealObject());
	const decVector2 &bottomAxisScaling = ds.GetClassVector2()->GetVector2(rt->GetValue(4)->GetRealObject());
	
	dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList.Add(
		decShapeCylinder::Ref::New(halfHeight, radius, bottomRadius, topAxisScaling, bottomAxisScaling));
}

// public func void addCylinder( float halfHeight, float topRadius, float bottomRadius,
// Vector2 topAxisScaling, Vector2 bottomAxisScaling, Vector position )
deClassShapeList::nfAddCylinder8::nfAddCylinder8(const sInitData &init) : dsFunction(init.clsShaList,
"addCylinder", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // halfHeight
	p_AddParameter(init.clsFlt); // radius
	p_AddParameter(init.clsFlt); // bottomRadius
	p_AddParameter(init.clsVec2); // topAxisScaling
	p_AddParameter(init.clsVec2); // bottomAxisScaling
	p_AddParameter(init.clsVec); // position
}
void deClassShapeList::nfAddCylinder8::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = *((static_cast<deClassShapeList*>(GetOwnerClass()))->GetDS());
	
	const float halfHeight(rt->GetValue(0)->GetFloat());
	const float radius(rt->GetValue(1)->GetFloat());
	const float bottomRadius(rt->GetValue(2)->GetFloat());
	const decVector2 &topAxisScaling = ds.GetClassVector2()->GetVector2(rt->GetValue(3)->GetRealObject());
	const decVector2 &bottomAxisScaling = ds.GetClassVector2()->GetVector2(rt->GetValue(4)->GetRealObject());
	const decVector &position = ds.GetClassVector()->GetVector(rt->GetValue(5)->GetRealObject());
	
	dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList.Add(
		decShapeCylinder::Ref::New(halfHeight, radius, bottomRadius,
			topAxisScaling, bottomAxisScaling, position));
}

// public func void addCylinder( float halfHeight, float topRadius, float bottomRadius, Vector2 topAxisScaling,
// Vector2 bottomAxisScaling, Vector position, Quaternion orientation )
deClassShapeList::nfAddCylinder9::nfAddCylinder9(const sInitData &init) : dsFunction(init.clsShaList,
"addCylinder", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // halfHeight
	p_AddParameter(init.clsFlt); // radius
	p_AddParameter(init.clsFlt); // bottomRadius
	p_AddParameter(init.clsVec2); // topAxisScaling
	p_AddParameter(init.clsVec2); // bottomAxisScaling
	p_AddParameter(init.clsVec); // position
	p_AddParameter(init.clsQuat); // orientation
}
void deClassShapeList::nfAddCylinder9::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = *((static_cast<deClassShapeList*>(GetOwnerClass()))->GetDS());
	
	const float halfHeight(rt->GetValue(0)->GetFloat());
	const float radius(rt->GetValue(1)->GetFloat());
	const float bottomRadius(rt->GetValue(2)->GetFloat());
	const decVector2 &topAxisScaling = ds.GetClassVector2()->GetVector2(rt->GetValue(3)->GetRealObject());
	const decVector2 &bottomAxisScaling = ds.GetClassVector2()->GetVector2(rt->GetValue(4)->GetRealObject());
	const decVector &position = ds.GetClassVector()->GetVector(rt->GetValue(5)->GetRealObject());
	const decQuaternion &orientation = ds.GetClassQuaternion()->GetQuaternion(rt->GetValue(6)->GetRealObject());
	
	dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList.Add(
		decShapeCylinder::Ref::New(halfHeight, radius, bottomRadius, topAxisScaling,
			bottomAxisScaling, position, orientation));
}



// public func void addCapsule( float halfHeight, float radius )
deClassShapeList::nfAddCapsule::nfAddCapsule(const sInitData &init) : dsFunction(init.clsShaList,
"addCapsule", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // halfHeight
	p_AddParameter(init.clsFlt); // radius
}
void deClassShapeList::nfAddCapsule::RunFunction(dsRunTime *rt, dsValue *myself){
	
	const float halfHeight(rt->GetValue(0)->GetFloat());
	const float radius(rt->GetValue(1)->GetFloat());
	
	dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList.Add(
		decShapeCapsule::Ref::New(halfHeight, radius));
}

// public func void addCapsule( float halfHeight, float radius, Vector position )
deClassShapeList::nfAddCapsule2::nfAddCapsule2(const sInitData &init) : dsFunction(init.clsShaList,
"addCapsule", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // halfHeight
	p_AddParameter(init.clsFlt); // radius
	p_AddParameter(init.clsVec); // position
}
void deClassShapeList::nfAddCapsule2::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = *((static_cast<deClassShapeList*>(GetOwnerClass()))->GetDS());
	
	const float halfHeight(rt->GetValue(0)->GetFloat());
	const float radius(rt->GetValue(1)->GetFloat());
	const decVector &position = ds.GetClassVector()->GetVector(rt->GetValue(2)->GetRealObject());
	
	dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList.Add(
		decShapeCapsule::Ref::New(halfHeight, radius, position));
}

// public func void addCapsule( float halfHeight, float radius, Vector position, Quaternion orientation )
deClassShapeList::nfAddCapsule3::nfAddCapsule3(const sInitData &init) : dsFunction(init.clsShaList,
"addCapsule", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // halfHeight
	p_AddParameter(init.clsFlt); // radius
	p_AddParameter(init.clsVec); // position
	p_AddParameter(init.clsQuat); // orientation
}
void deClassShapeList::nfAddCapsule3::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = *((static_cast<deClassShapeList*>(GetOwnerClass()))->GetDS());
	
	const float halfHeight(rt->GetValue(0)->GetFloat());
	const float radius(rt->GetValue(1)->GetFloat());
	const decVector &position = ds.GetClassVector()->GetVector(rt->GetValue(2)->GetRealObject());
	const decQuaternion &orientation = ds.GetClassQuaternion()->GetQuaternion(rt->GetValue(3)->GetRealObject());
	
	dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList.Add(
		decShapeCapsule::Ref::New(halfHeight, radius, position, orientation));
}

// public func void addCapsule( float halfHeight, float topRadius, float bottomRadius )
deClassShapeList::nfAddCapsule4::nfAddCapsule4(const sInitData &init) : dsFunction(init.clsShaList,
"addCapsule", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // halfHeight
	p_AddParameter(init.clsFlt); // radius
	p_AddParameter(init.clsFlt); // bottomRadius
}
void deClassShapeList::nfAddCapsule4::RunFunction(dsRunTime *rt, dsValue *myself){
	
	const float halfHeight(rt->GetValue(0)->GetFloat());
	const float radius(rt->GetValue(1)->GetFloat());
	const float bottomRadius(rt->GetValue(2)->GetFloat());
	
	dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList.Add(
		decShapeCapsule::Ref::New(halfHeight, radius, bottomRadius));
}

// public func void addCapsule( float halfHeight, float topRadius,
// float bottomRadius, Vector position )
deClassShapeList::nfAddCapsule5::nfAddCapsule5(const sInitData &init) : dsFunction(init.clsShaList,
"addCapsule", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // halfHeight
	p_AddParameter(init.clsFlt); // radius
	p_AddParameter(init.clsFlt); // bottomRadius
	p_AddParameter(init.clsVec); // position
}
void deClassShapeList::nfAddCapsule5::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = *((static_cast<deClassShapeList*>(GetOwnerClass()))->GetDS());
	
	const float halfHeight(rt->GetValue(0)->GetFloat());
	const float radius(rt->GetValue(1)->GetFloat());
	const float bottomRadius(rt->GetValue(2)->GetFloat());
	const decVector &position = ds.GetClassVector()->GetVector(rt->GetValue(3)->GetRealObject());
	
	dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList.Add(
		decShapeCapsule::Ref::New(halfHeight, radius, bottomRadius, position));
}

// public func void addCapsule( float halfHeight, float topRadius,
// float bottomRadius, Vector position, Quaternion orientation )
deClassShapeList::nfAddCapsule6::nfAddCapsule6(const sInitData &init) : dsFunction(init.clsShaList,
"addCapsule", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // halfHeight
	p_AddParameter(init.clsFlt); // radius
	p_AddParameter(init.clsFlt); // bottomRadius
	p_AddParameter(init.clsVec); // position
	p_AddParameter(init.clsQuat); // orientation
}
void deClassShapeList::nfAddCapsule6::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = *((static_cast<deClassShapeList*>(GetOwnerClass()))->GetDS());
	
	const float halfHeight(rt->GetValue(0)->GetFloat());
	const float radius(rt->GetValue(1)->GetFloat());
	const float bottomRadius(rt->GetValue(2)->GetFloat());
	const decVector &position = ds.GetClassVector()->GetVector(rt->GetValue(3)->GetRealObject());
	const decQuaternion &orientation = ds.GetClassQuaternion()->GetQuaternion(rt->GetValue(4)->GetRealObject());
	
	dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList.Add(
		decShapeCapsule::Ref::New(halfHeight, radius, bottomRadius, position, orientation));
}

// public func void addCapsule( float halfHeight, float topRadius, float bottomRadius,
// Vector2 topAxisScaling, Vector2 bottomAxisScaling )
deClassShapeList::nfAddCapsule7::nfAddCapsule7(const sInitData &init) : dsFunction(init.clsShaList,
"addCapsule", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // halfHeight
	p_AddParameter(init.clsFlt); // radius
	p_AddParameter(init.clsFlt); // bottomRadius
	p_AddParameter(init.clsVec2); // topAxisScaling
	p_AddParameter(init.clsVec2); // bottomAxisScaling
}
void deClassShapeList::nfAddCapsule7::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = *((static_cast<deClassShapeList*>(GetOwnerClass()))->GetDS());
	
	const float halfHeight(rt->GetValue(0)->GetFloat());
	const float radius(rt->GetValue(1)->GetFloat());
	const float bottomRadius(rt->GetValue(2)->GetFloat());
	const decVector2 &topAxisScaling = ds.GetClassVector2()->GetVector2(rt->GetValue(3)->GetRealObject());
	const decVector2 &bottomAxisScaling = ds.GetClassVector2()->GetVector2(rt->GetValue(4)->GetRealObject());
	
	dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList.Add(
		decShapeCapsule::Ref::New(halfHeight, radius, bottomRadius, topAxisScaling, bottomAxisScaling));
}

// public func void addCapsule( float halfHeight, float topRadius, float bottomRadius,
// Vector2 topAxisScaling, Vector2 bottomAxisScaling, Vector position )
deClassShapeList::nfAddCapsule8::nfAddCapsule8(const sInitData &init) : dsFunction(init.clsShaList,
"addCapsule", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // halfHeight
	p_AddParameter(init.clsFlt); // radius
	p_AddParameter(init.clsFlt); // bottomRadius
	p_AddParameter(init.clsVec2); // topAxisScaling
	p_AddParameter(init.clsVec2); // bottomAxisScaling
	p_AddParameter(init.clsVec); // position
}
void deClassShapeList::nfAddCapsule8::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = *((static_cast<deClassShapeList*>(GetOwnerClass()))->GetDS());
	
	const float halfHeight(rt->GetValue(0)->GetFloat());
	const float radius(rt->GetValue(1)->GetFloat());
	const float bottomRadius(rt->GetValue(2)->GetFloat());
	const decVector2 &topAxisScaling = ds.GetClassVector2()->GetVector2(rt->GetValue(3)->GetRealObject());
	const decVector2 &bottomAxisScaling = ds.GetClassVector2()->GetVector2(rt->GetValue(4)->GetRealObject());
	const decVector &position = ds.GetClassVector()->GetVector(rt->GetValue(5)->GetRealObject());
	
	dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList.Add(
		decShapeCapsule::Ref::New(halfHeight, radius, bottomRadius,
			topAxisScaling, bottomAxisScaling, position));
}

// public func void addCapsule( float halfHeight, float topRadius, float bottomRadius, Vector2 topAxisScaling,
// Vector2 bottomAxisScaling, Vector position, Quaternion orientation )
deClassShapeList::nfAddCapsule9::nfAddCapsule9(const sInitData &init) : dsFunction(init.clsShaList,
"addCapsule", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // halfHeight
	p_AddParameter(init.clsFlt); // radius
	p_AddParameter(init.clsFlt); // bottomRadius
	p_AddParameter(init.clsVec2); // topAxisScaling
	p_AddParameter(init.clsVec2); // bottomAxisScaling
	p_AddParameter(init.clsVec); // position
	p_AddParameter(init.clsQuat); // orientation
}
void deClassShapeList::nfAddCapsule9::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = *((static_cast<deClassShapeList*>(GetOwnerClass()))->GetDS());
	
	const float halfHeight(rt->GetValue(0)->GetFloat());
	const float radius(rt->GetValue(1)->GetFloat());
	const float bottomRadius(rt->GetValue(2)->GetFloat());
	const decVector2 &topAxisScaling = ds.GetClassVector2()->GetVector2(rt->GetValue(3)->GetRealObject());
	const decVector2 &bottomAxisScaling = ds.GetClassVector2()->GetVector2(rt->GetValue(4)->GetRealObject());
	const decVector &position = ds.GetClassVector()->GetVector(rt->GetValue(5)->GetRealObject());
	const decQuaternion &orientation = ds.GetClassQuaternion()->GetQuaternion(rt->GetValue(6)->GetRealObject());
	
	dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList.Add(
		decShapeCapsule::Ref::New(halfHeight, radius, bottomRadius, topAxisScaling,
			bottomAxisScaling, position, orientation));
}



// public func void addHull( Array points )
deClassShapeList::nfAddHull::nfAddHull(const sInitData &init) : dsFunction(init.clsShaList,
"addHull", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsArray); // points
}
void deClassShapeList::nfAddHull::RunFunction(dsRunTime *rt, dsValue *myself){
	decShapeHull::Ref hull = decShapeHull::Ref::New();
	(static_cast<deClassShapeList*>(GetOwnerClass()))->HullAddPoints(*rt, *hull, *rt->GetValue(0));
	dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList.Add(std::move(hull));
}

// public func void addHull( Array points, Vector position )
deClassShapeList::nfAddHull2::nfAddHull2(const sInitData &init) : dsFunction(init.clsShaList,
"addHull", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsArray); // points
	p_AddParameter(init.clsVec); // position
}
void deClassShapeList::nfAddHull2::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = *((static_cast<deClassShapeList*>(GetOwnerClass()))->GetDS());
	
	const decVector &position = ds.GetClassVector()->GetVector(rt->GetValue(2)->GetRealObject());
	
	decShapeHull::Ref hull = decShapeHull::Ref::New(position);
	(static_cast<deClassShapeList*>(GetOwnerClass()))->HullAddPoints(*rt, *hull, *rt->GetValue(0));
	dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList.Add(std::move(hull));
}

// public func void addHull( Array points, Vector position, Quaternion orientation )
deClassShapeList::nfAddHull3::nfAddHull3(const sInitData &init) : dsFunction(init.clsShaList,
"addHull", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsArray); // points
	p_AddParameter(init.clsVec); // position
	p_AddParameter(init.clsQuat); // orientation
}
void deClassShapeList::nfAddHull3::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = *((static_cast<deClassShapeList*>(GetOwnerClass()))->GetDS());
	
	const decVector &position = ds.GetClassVector()->GetVector(rt->GetValue(2)->GetRealObject());
	const decQuaternion &orientation = ds.GetClassQuaternion()->GetQuaternion(rt->GetValue(3)->GetRealObject());
	
	decShapeHull::Ref hull = decShapeHull::Ref::New(position, orientation);
	(static_cast<deClassShapeList*>(GetOwnerClass()))->HullAddPoints(*rt, *hull, *rt->GetValue(0));
	dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList.Add(std::move(hull));
}



// File Handling
//////////////////

// write shapes visitor
class VisitorWriteShapes : public decShapeVisitor{
private:
	decBaseFileWriter &pWriter;
	
public:
	const static unsigned char typeShere = 0;
	const static unsigned char typeBox = 1;
	const static unsigned char typeCylinder = 2;
	const static unsigned char typeCapsule = 3;
	const static unsigned char typeHull = 4;
	
	explicit VisitorWriteShapes(decBaseFileWriter &writer) : pWriter(writer){}
	
	void VisitShapeSphere(decShapeSphere &sphere) override{
		pWriter.WriteByte(typeShere);
		pWriter.WriteVector(sphere.GetPosition());
		pWriter.WriteFloat(sphere.GetRadius());
		pWriter.WriteVector2(sphere.GetAxisScaling());
	}
	
	void VisitShapeBox(decShapeBox &box) override{
		pWriter.WriteByte(typeBox);
		pWriter.WriteVector(box.GetPosition());
		pWriter.WriteQuaternion(box.GetOrientation());
		pWriter.WriteVector(box.GetHalfExtends());
		pWriter.WriteVector2(box.GetTapering());
	}
	
	void VisitShapeCylinder(decShapeCylinder &cylinder) override{
		pWriter.WriteByte(typeCylinder);
		pWriter.WriteVector(cylinder.GetPosition());
		pWriter.WriteQuaternion(cylinder.GetOrientation());
		pWriter.WriteFloat(cylinder.GetTopRadius());
		pWriter.WriteFloat(cylinder.GetBottomRadius());
		pWriter.WriteFloat(cylinder.GetHalfHeight());
		pWriter.WriteVector2(cylinder.GetTopAxisScaling());
		pWriter.WriteVector2(cylinder.GetBottomAxisScaling());
	}
	
	void VisitShapeCapsule(decShapeCapsule &capsule) override{
		pWriter.WriteByte(typeCapsule);
		pWriter.WriteVector(capsule.GetPosition());
		pWriter.WriteQuaternion(capsule.GetOrientation());
		pWriter.WriteFloat(capsule.GetTopRadius());
		pWriter.WriteFloat(capsule.GetBottomRadius());
		pWriter.WriteFloat(capsule.GetHalfHeight());
		pWriter.WriteVector2(capsule.GetTopAxisScaling());
		pWriter.WriteVector2(capsule.GetBottomAxisScaling());
	}
	
	void VisitShapeHull(decShapeHull &hull) override{
		pWriter.WriteByte(typeHull);
		pWriter.WriteVector(hull.GetPosition());
		pWriter.WriteQuaternion(hull.GetOrientation());
		pWriter.WriteUShort((uint16_t)hull.GetPoints().GetCount());
		hull.GetPoints().Visit([&](const decVector &point){
			pWriter.WriteVector(point);
		});
	}
};

// static public func ShapeList readFromFile( FileReader reader )
deClassShapeList::nfReadFromFile::nfReadFromFile(const sInitData &init) : dsFunction(init.clsShaList,
"readFromFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsShaList){
	p_AddParameter(init.clsFileReader); // reader
}
void deClassShapeList::nfReadFromFile::RunFunction(dsRunTime *rt, dsValue*){
	deClassShapeList &clsShapeList = *(static_cast<deClassShapeList*>(GetOwnerClass()));
	const deClassFileReader &clsFileReader = *clsShapeList.GetDS()->GetClassFileReader();
	decBaseFileReader * const reader = clsFileReader.GetFileReader(rt->GetValue(0)->GetRealObject());
	if(!reader){
		DSTHROW(dueNullPointer);
	}
	
	switch(reader->ReadByte()){ // version
	case 0:{
		const int count = reader->ReadUShort();
		decShape::List shapeList;
		
		for(int i=0; i<count; i++){
			switch(reader->ReadByte()){
			case VisitorWriteShapes::typeShere:{
				const decVector position(reader->ReadVector());
				const float radius = reader->ReadFloat();
				const decVector2 axisScaling(reader->ReadVector2());
				shapeList.Add(decShapeSphere::Ref::New(radius, axisScaling, position));
				}break;
				
			case VisitorWriteShapes::typeBox:{
				const decVector position(reader->ReadVector());
				const decQuaternion orientation(reader->ReadQuaternion());
				const decVector halfExtends(reader->ReadVector());
				const decVector2 tapering(reader->ReadVector2());
				shapeList.Add(decShapeBox::Ref::New(halfExtends, tapering, position, orientation));
				}break;
				
			case VisitorWriteShapes::typeCylinder:{
				const decVector position(reader->ReadVector());
				const decQuaternion orientation(reader->ReadQuaternion());
				const float topRadius = reader->ReadFloat();
				const float bottomRadius = reader->ReadFloat();
				const float halfHeight = reader->ReadFloat();
				const decVector2 topAxisScaling(reader->ReadVector2());
				const decVector2 bottomAxisScaling(reader->ReadVector2());
				shapeList.Add(decShapeCylinder::Ref::New(halfHeight, topRadius, bottomRadius,
					topAxisScaling, bottomAxisScaling, position, orientation));
				}break;
				
			case VisitorWriteShapes::typeCapsule:{
				const decVector position(reader->ReadVector());
				const decQuaternion orientation(reader->ReadQuaternion());
				const float topRadius = reader->ReadFloat();
				const float bottomRadius = reader->ReadFloat();
				const float halfHeight = reader->ReadFloat();
				const decVector2 topAxisScaling(reader->ReadVector2());
				const decVector2 bottomAxisScaling(reader->ReadVector2());
				shapeList.Add(decShapeCapsule::Ref::New(halfHeight, topRadius, bottomRadius,
					topAxisScaling, bottomAxisScaling, position, orientation));
				}break;
				
			case VisitorWriteShapes::typeHull:{
				const decVector position(reader->ReadVector());
				const decQuaternion orientation(reader->ReadQuaternion());
				decShapeHull::Ref shapeHull = decShapeHull::Ref::New(position, orientation);
				const int pointCount = reader->ReadUShort();
				shapeHull->SetPointCount(pointCount);
				for(int j=0; j<pointCount; j++){
					shapeHull->SetPointAt(j, reader->ReadVector());
				}
				shapeList.Add(std::move(shapeHull));
				}break;
			}
		}
		
		clsShapeList.PushShapeList(rt, shapeList);
		}break;
		
	default:
		DSTHROW_INFO(dueInvalidParam, "unsupported version");
	}
}

// public func void writeToFile( FileWriter writer )
deClassShapeList::nfWriteToFile::nfWriteToFile(const sInitData &init) : dsFunction(init.clsShaList,
"writeToFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFileWriter); // writer
}
void deClassShapeList::nfWriteToFile::RunFunction(dsRunTime *rt, dsValue *myself){
	const decShape::List &shapeList = dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList;
	const deClassShapeList &clsShapeList = *(static_cast<deClassShapeList*>(GetOwnerClass()));
	const deClassFileWriter &clsFileWriter = *clsShapeList.GetDS()->GetClassFileWriter();
	decBaseFileWriter * const writer = clsFileWriter.GetFileWriter(rt->GetValue(0)->GetRealObject());
	
	if(!writer){
		DSTHROW(dueNullPointer);
	}
	
	const int count = shapeList.GetCount();
	VisitorWriteShapes visitor(*writer);
	int i;
	
	writer->WriteByte(0); // version
	writer->WriteUShort((unsigned short)count);
	for(i=0; i<count; i++){
		shapeList.GetAt(i)->Visit(visitor);
	}
}



// Special
////////////

// public func int hashCode()
deClassShapeList::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsShaList, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}

void deClassShapeList::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	const decShape::List &shapeList = dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList;
	
	rt->PushInt((int)(intptr_t)&shapeList);
}

// public func bool equals( Object object )
deClassShapeList::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsShaList, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // object
}
void deClassShapeList::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	const decShape::List &shapeList = dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself)).shapeList;
	deClassShapeList * const clsShaList = static_cast<deClassShapeList*>(GetOwnerClass());
	dsValue * const object = rt->GetValue(0);
	
	if(!p_IsObjOfType(object, clsShaList)){
		rt->PushBool(false);
		
	}else{
		const decShape::List &otherShapeList = dedsGetNativeData<sShaListNatDat>(p_GetNativeData(object)).shapeList;
		rt->PushBool(&shapeList == &otherShapeList);
	}
}



// Class deClassShapeList
///////////////////////////

// Constructor
////////////////

deClassShapeList::deClassShapeList(deScriptingDragonScript *ds) :
dsClass("ShapeList", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(nullptr),
pClsShapeType(nullptr)
{
	if(!ds){
		DSTHROW(dueInvalidParam);
	}
	
	pDS = ds;
	
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(dedsNativeDataSize<sShaListNatDat>());
}

deClassShapeList::~deClassShapeList(){
}



// Management
///////////////

void deClassShapeList::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	pClsShapeType = engine->GetClass("Dragengine.Scenery.ShapeType");
	
	init.clsShaList = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	init.clsVec = pDS->GetClassVector();
	init.clsQuat = pDS->GetClassQuaternion();
	init.clsVec2 = pDS->GetClassVector2();
	init.clsArray = engine->GetClassArray();
	init.clsFileReader = pDS->GetClassFileReader();
	init.clsFileWriter = pDS->GetClassFileWriter();
	init.clsShapeType = pClsShapeType;
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfNew2(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetCount(init));
	AddFunction(new nfEmpty(init));
	AddFunction(new nfNotEmpty(init));
	AddFunction(new nfRemoveAllShapes(init));
	
	AddFunction(new nfGetTypeAt(init));
	AddFunction(new nfGetPositionAt(init));
	AddFunction(new nfGetRadiusAt(init));
	AddFunction(new nfGetHalfExtendsAt(init));
	AddFunction(new nfGetOrientationAt(init));
	AddFunction(new nfGetHalfHeightAt(init));
	AddFunction(new nfGetTopRadiusAt(init));
	AddFunction(new nfGetBottomRadiusAt(init));
	AddFunction(new nfGetTopAxisScalingAt(init));
	AddFunction(new nfGetBottomAxisScalingAt(init));
	AddFunction(new nfGetPointCountAt(init));
	AddFunction(new nfGetPointAt(init));
	
	AddFunction(new nfAddSphere(init));
	
	AddFunction(new nfAddBox(init));
	AddFunction(new nfAddBox2(init));
	AddFunction(new nfAddBox3(init));
	AddFunction(new nfAddBox4(init));
	
	AddFunction(new nfAddCylinder(init));
	AddFunction(new nfAddCylinder2(init));
	AddFunction(new nfAddCylinder3(init));
	AddFunction(new nfAddCylinder4(init));
	AddFunction(new nfAddCylinder5(init));
	AddFunction(new nfAddCylinder6(init));
	AddFunction(new nfAddCylinder7(init));
	AddFunction(new nfAddCylinder8(init));
	AddFunction(new nfAddCylinder9(init));
	
	AddFunction(new nfAddCapsule(init));
	AddFunction(new nfAddCapsule2(init));
	AddFunction(new nfAddCapsule3(init));
	AddFunction(new nfAddCapsule4(init));
	AddFunction(new nfAddCapsule5(init));
	AddFunction(new nfAddCapsule6(init));
	AddFunction(new nfAddCapsule7(init));
	AddFunction(new nfAddCapsule8(init));
	AddFunction(new nfAddCapsule9(init));
	
	AddFunction(new nfAddHull(init));
	AddFunction(new nfAddHull2(init));
	AddFunction(new nfAddHull3(init));
	
	AddFunction(new nfReadFromFile(init));
	AddFunction(new nfWriteToFile(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

const decShape::List &deClassShapeList::GetShapeList(dsRealObject *myself) const{
	if(!myself){
		DSTHROW(dueNullPointer);
	}
	
	return dedsGetNativeData<sShaListNatDat>(p_GetNativeData(myself->GetBuffer())).shapeList;
}

void deClassShapeList::PushShapeList(dsRunTime *rt, const decShape::List &shapeList){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	rt->CreateObjectNakedOnStack(this);
	sShaListNatDat &nd = dedsNewNativeData<sShaListNatDat>(p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()));
	
	try{
		nd.shapeList = shapeList;
		
	}catch(...){
		rt->RemoveValues(1); // remove pushed object
		throw;
	}
}

void deClassShapeList::HullAddPoints(dsRunTime &rt, decShapeHull &hull, dsValue &pointsArray) const{
	if(!pointsArray.GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	int count = 0;
	rt.RunFunction(&pointsArray, "getCount", 0);
	count = rt.GetReturnInt();
	if(count == 0){
		return;
	}
	
	const deClassVector &clsVector = *pDS->GetClassVector();
	int i;
	
	hull.SetPointCount(count);
	
	for(i=0; i<count; i++){
		rt.PushInt(i);
		rt.RunFunction(&pointsArray, "getAt", 1);
		hull.SetPointAt(i, clsVector.GetVector(rt.GetReturnRealObject()));
	}
}
