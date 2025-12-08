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

#include "deClassDebugDrawer.h"
#include "../graphics/deClassColor.h"
#include "../math/deClassVector.h"
#include "../math/deClassDVector.h"
#include "../math/deClassQuaternion.h"
#include "../math/deClassMatrix.h"
#include "../utils/deClassShapeList.h"
#include "../world/deClassModel.h"
#include "../world/deClassWorld.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeCapsule.h>
#include <dragengine/common/shape/decShapeCylinder.h>
#include <dragengine/common/shape/decShapeList.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerShapeFace.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelFace.h>
#include <dragengine/resources/model/deModelLOD.h>
#include <dragengine/resources/model/deModelVertex.h>



struct sDDNatDat{
	deDebugDrawer::Ref ddrawer;
};



// Constructors, Destructors
//////////////////////////////

// public func new()
deClassDebugDrawer::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsDD,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassDebugDrawer::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sDDNatDat *nd = (sDDNatDat*)p_GetNativeData(myself);
	deClassDebugDrawer &clsDD = *((deClassDebugDrawer*)GetOwnerClass());
	deDebugDrawerManager &ddmgr = *clsDD.GetDS()->GetGameEngine()->GetDebugDrawerManager();
	
	// clear ( important )
	nd->ddrawer = NULL;
	
	// create debug drawer
	nd->ddrawer = ddmgr.CreateDebugDrawer();
	if(!nd->ddrawer) DSTHROW(dueOutOfMemory);
}

// public func destructor()
deClassDebugDrawer::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsDD,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassDebugDrawer::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sDDNatDat *nd = (sDDNatDat*)p_GetNativeData(myself);
	
	if(nd->ddrawer){
		nd->ddrawer->FreeReference();
		nd->ddrawer = NULL;
	}
}



// Management
///////////////

// public func DVector getPosition()
deClassDebugDrawer::nfGetPosition::nfGetPosition(const sInitData &init) : dsFunction(init.clsDD,
"getPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
}
void deClassDebugDrawer::nfGetPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	const deDebugDrawer &ddrawer = *(((sDDNatDat*)p_GetNativeData(myself))->ddrawer);
	const deScriptingDragonScript &ds = *(((deClassDebugDrawer*)GetOwnerClass())->GetDS());
	
	ds.GetClassDVector()->PushDVector(rt, ddrawer.GetPosition());
}

// public func void setPosition( DVector position )
deClassDebugDrawer::nfSetPosition::nfSetPosition(const sInitData &init) : dsFunction(init.clsDD,
"setPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsDVec); // position
}
void deClassDebugDrawer::nfSetPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	deDebugDrawer &ddrawer = *(((sDDNatDat*)p_GetNativeData(myself))->ddrawer);
	const deScriptingDragonScript &ds = *(((deClassDebugDrawer*)GetOwnerClass())->GetDS());
	
	ddrawer.SetPosition(ds.GetClassDVector()->GetDVector(rt->GetValue(0)->GetRealObject()));
}

// public func Quaternion getOrientation()
deClassDebugDrawer::nfGetOrientation::nfGetOrientation(const sInitData &init) : dsFunction(init.clsDD,
"getOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat){
}
void deClassDebugDrawer::nfGetOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	const deDebugDrawer &ddrawer = *(((sDDNatDat*)p_GetNativeData(myself))->ddrawer);
	const deScriptingDragonScript &ds = *(((deClassDebugDrawer*)GetOwnerClass())->GetDS());
	
	ds.GetClassQuaternion()->PushQuaternion(rt, ddrawer.GetOrientation());
}

// public func void setOrientation( Quaternion orientation )
deClassDebugDrawer::nfSetOrientation::nfSetOrientation(const sInitData &init) : dsFunction(init.clsDD,
"setOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsQuat); // orientation
}
void deClassDebugDrawer::nfSetOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	deDebugDrawer &ddrawer = *(((sDDNatDat*)p_GetNativeData(myself))->ddrawer);
	const deScriptingDragonScript &ds = *(((deClassDebugDrawer*)GetOwnerClass())->GetDS());
	
	ddrawer.SetOrientation(ds.GetClassQuaternion()->GetQuaternion(rt->GetValue(0)->GetRealObject()));
}

// public func Vector getScale()
deClassDebugDrawer::nfGetScale::nfGetScale(const sInitData &init) : dsFunction(init.clsDD,
"getScale", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
}
void deClassDebugDrawer::nfGetScale::RunFunction(dsRunTime *rt, dsValue *myself){
	const deDebugDrawer &ddrawer = *(((sDDNatDat*)p_GetNativeData(myself))->ddrawer);
	const deScriptingDragonScript &ds = *(((deClassDebugDrawer*)GetOwnerClass())->GetDS());
	
	ds.GetClassVector()->PushVector(rt, ddrawer.GetScale());
}

// public func void setScale( Vector scale )
deClassDebugDrawer::nfSetScale::nfSetScale(const sInitData &init) : dsFunction(init.clsDD,
"setScale", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // scale
}
void deClassDebugDrawer::nfSetScale::RunFunction(dsRunTime *rt, dsValue *myself){
	deDebugDrawer &ddrawer = *(((sDDNatDat*)p_GetNativeData(myself))->ddrawer);
	const deScriptingDragonScript &ds = *(((deClassDebugDrawer*)GetOwnerClass())->GetDS());
	
	ddrawer.SetScale(ds.GetClassVector()->GetVector(rt->GetValue(0)->GetRealObject()));
}

// func World getParentWorld()
deClassDebugDrawer::nfGetParentWorld::nfGetParentWorld(const sInitData &init) :
dsFunction(init.clsDD, "getParentWorld", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsWorld){
}
void deClassDebugDrawer::nfGetParentWorld::RunFunction(dsRunTime *rt, dsValue *myself){
	const deDebugDrawer &ddrawer = *(((sDDNatDat*)p_GetNativeData(myself))->ddrawer);
	deClassWorld &clsWorld = *(((deClassDebugDrawer*)GetOwnerClass())->GetDS()->GetClassWorld());
	clsWorld.PushWorld(rt, ddrawer.GetParentWorld());
}

// public func bool getVisible()
deClassDebugDrawer::nfGetVisible::nfGetVisible(const sInitData &init) : dsFunction(init.clsDD,
"getVisible", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassDebugDrawer::nfGetVisible::RunFunction(dsRunTime *rt, dsValue *myself){
	const deDebugDrawer &ddrawer = *(((sDDNatDat*)p_GetNativeData(myself))->ddrawer);
	rt->PushBool(ddrawer.GetVisible());
}

// public func void setVisible( bool visible )
deClassDebugDrawer::nfSetVisible::nfSetVisible(const sInitData &init) : dsFunction(init.clsDD,
"setVisible", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // visible
}
void deClassDebugDrawer::nfSetVisible::RunFunction(dsRunTime *rt, dsValue *myself){
	deDebugDrawer &ddrawer = *(((sDDNatDat*)p_GetNativeData(myself))->ddrawer);
	ddrawer.SetVisible(rt->GetValue(0)->GetBool());
}

// public func bool getXRay()
deClassDebugDrawer::nfGetXRay::nfGetXRay(const sInitData &init) : dsFunction(init.clsDD,
"getXRay", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassDebugDrawer::nfGetXRay::RunFunction(dsRunTime *rt, dsValue *myself){
	const deDebugDrawer &ddrawer = *(((sDDNatDat*)p_GetNativeData(myself))->ddrawer);
	rt->PushBool(ddrawer.GetXRay());
}

// public func void setXRay( bool xray )
deClassDebugDrawer::nfSetXRay::nfSetXRay(const sInitData &init) : dsFunction(init.clsDD,
"setXRay", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // xray
}
void deClassDebugDrawer::nfSetXRay::RunFunction(dsRunTime *rt, dsValue *myself){
	deDebugDrawer &ddrawer = *(((sDDNatDat*)p_GetNativeData(myself))->ddrawer);
	ddrawer.SetXRay(rt->GetValue(0)->GetBool());
}



// public func int getShapeCount()
deClassDebugDrawer::nfGetShapeCount::nfGetShapeCount(const sInitData &init) : dsFunction(init.clsDD,
"getShapeCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassDebugDrawer::nfGetShapeCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deDebugDrawer &ddrawer = *(((sDDNatDat*)p_GetNativeData(myself))->ddrawer);
	rt->PushInt(ddrawer.GetShapeCount());
}

// public func void setShapeCount( int count )
deClassDebugDrawer::nfSetShapeCount::nfSetShapeCount(const sInitData &init) : dsFunction(init.clsDD,
"setShapeCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // count
}
void deClassDebugDrawer::nfSetShapeCount::RunFunction(dsRunTime *rt, dsValue *myself){
	deDebugDrawer &ddrawer = *(((sDDNatDat*)p_GetNativeData(myself))->ddrawer);
	deDebugDrawerShape *ddshape = NULL;
	int count = rt->GetValue(0)->GetInt();
	
	if(count < 0){
		DSTHROW(dueInvalidParam);
	}
	
	ddrawer.RemoveAllShapes();
	
	try{
		for(; count>0; count--){
			ddshape = new deDebugDrawerShape;
			ddrawer.AddShape(ddshape);
			ddshape = NULL;
		}
		
	}catch(...){
		if(ddshape){
			delete ddshape;
		}
		throw;
	}
}



// public func void shapeSetPosition( int shape, Vector position )
deClassDebugDrawer::nfShapeSetPosition::nfShapeSetPosition(const sInitData &init) : dsFunction(init.clsDD,
"shapeSetPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // shape
	p_AddParameter(init.clsVec); // position
}
void deClassDebugDrawer::nfShapeSetPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	deDebugDrawer &ddrawer = *(((sDDNatDat*)p_GetNativeData(myself))->ddrawer);
	const deScriptingDragonScript &ds = *(((deClassDebugDrawer*)GetOwnerClass())->GetDS());
	
	deDebugDrawerShape &ddshape = *ddrawer.GetShapeAt(rt->GetValue(0)->GetInt());
	const decVector &position = ds.GetClassVector()->GetVector(rt->GetValue(1)->GetRealObject());
	
	ddshape.SetPosition(position);
	ddrawer.NotifyShapeGeometryChanged();
}

// public func void shapeSetOrientation( int shape, Quaternion orientation )
deClassDebugDrawer::nfShapeSetOrientation::nfShapeSetOrientation(const sInitData &init) : dsFunction(init.clsDD,
"shapeSetOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // shape
	p_AddParameter(init.clsQuat); // orientation
}
void deClassDebugDrawer::nfShapeSetOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	deDebugDrawer &ddrawer = *(((sDDNatDat*)p_GetNativeData(myself))->ddrawer);
	const deScriptingDragonScript &ds = *(((deClassDebugDrawer*)GetOwnerClass())->GetDS());
	
	deDebugDrawerShape &ddshape = *ddrawer.GetShapeAt(rt->GetValue(0)->GetInt());
	const decQuaternion &orientation = ds.GetClassQuaternion()->GetQuaternion(rt->GetValue(1)->GetRealObject());
	
	ddshape.SetOrientation(orientation);
	ddrawer.NotifyShapeGeometryChanged();
}

// public func void shapeSetScale( int shape, Vector scale )
deClassDebugDrawer::nfShapeSetScale::nfShapeSetScale(const sInitData &init) : dsFunction(init.clsDD,
"shapeSetScale", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // shape
	p_AddParameter(init.clsVec); // scale
}
void deClassDebugDrawer::nfShapeSetScale::RunFunction(dsRunTime *rt, dsValue *myself){
	deDebugDrawer &ddrawer = *(((sDDNatDat*)p_GetNativeData(myself))->ddrawer);
	const deScriptingDragonScript &ds = *(((deClassDebugDrawer*)GetOwnerClass())->GetDS());
	
	deDebugDrawerShape &ddshape = *ddrawer.GetShapeAt(rt->GetValue(0)->GetInt());
	const decVector &scale = ds.GetClassVector()->GetVector(rt->GetValue(1)->GetRealObject());
	
	ddshape.SetScale(scale);
	ddrawer.NotifyShapeGeometryChanged();
}



// public func void shapeSetShapes( int shape, ShapeList shapeList )
deClassDebugDrawer::nfShapeSetShapes::nfShapeSetShapes(const sInitData &init) : dsFunction(init.clsDD,
"shapeSetShapes", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // shape
	p_AddParameter(init.clsShapeList); // shapeList
}
void deClassDebugDrawer::nfShapeSetShapes::RunFunction(dsRunTime *rt, dsValue *myself){
	deDebugDrawer &ddrawer = *(((sDDNatDat*)p_GetNativeData(myself))->ddrawer);
	const deScriptingDragonScript &ds = *(((deClassDebugDrawer*)GetOwnerClass())->GetDS());
	
	deDebugDrawerShape &ddshape = *ddrawer.GetShapeAt(rt->GetValue(0)->GetInt());
	const decShapeList &shapeList = ds.GetClassShapeList()->GetShapeList(rt->GetValue(1)->GetRealObject());
	
	ddshape.GetShapeList() = shapeList;
	ddrawer.NotifyShapeContentChanged();
}

// public func void shapeRemoveAllShapes( int shape )
deClassDebugDrawer::nfShapeRemoveAllShapes::nfShapeRemoveAllShapes(const sInitData &init) : dsFunction(init.clsDD,
"shapeRemoveAllShapes", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // shape
}
void deClassDebugDrawer::nfShapeRemoveAllShapes::RunFunction(dsRunTime *rt, dsValue *myself){
	deDebugDrawer &ddrawer = *(((sDDNatDat*)p_GetNativeData(myself))->ddrawer);
	deDebugDrawerShape &ddshape = *ddrawer.GetShapeAt(rt->GetValue(0)->GetInt());
	
	ddshape.GetShapeList().RemoveAll();
	ddrawer.NotifyShapeContentChanged();
}



// public func void shapeSetEdgeColor( int shape, Color color )
deClassDebugDrawer::nfShapeSetEdgeColor::nfShapeSetEdgeColor(const sInitData &init) : dsFunction(init.clsDD,
"shapeSetEdgeColor", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // shape
	p_AddParameter(init.clsClr); // color
}
void deClassDebugDrawer::nfShapeSetEdgeColor::RunFunction(dsRunTime *rt, dsValue *myself){
	deDebugDrawer &ddrawer = *(((sDDNatDat*)p_GetNativeData(myself))->ddrawer);
	const deScriptingDragonScript &ds = *(((deClassDebugDrawer*)GetOwnerClass())->GetDS());
	
	deDebugDrawerShape &ddshape = *ddrawer.GetShapeAt(rt->GetValue(0)->GetInt());
	const decColor &color = ds.GetClassColor()->GetColor(rt->GetValue(1)->GetRealObject());
	
	ddshape.SetEdgeColor(color);
	ddrawer.NotifyShapeColorChanged();
}

// public func void shapeSetFillColor( int shape, Color color )
deClassDebugDrawer::nfShapeSetFillColor::nfShapeSetFillColor(const sInitData &init) : dsFunction(init.clsDD,
"shapeSetFillColor", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // shape
	p_AddParameter(init.clsClr); // color
}
void deClassDebugDrawer::nfShapeSetFillColor::RunFunction(dsRunTime *rt, dsValue *myself){
	deDebugDrawer &ddrawer = *(((sDDNatDat*)p_GetNativeData(myself))->ddrawer);
	const deScriptingDragonScript &ds = *(((deClassDebugDrawer*)GetOwnerClass())->GetDS());
	
	deDebugDrawerShape &ddshape = *ddrawer.GetShapeAt(rt->GetValue(0)->GetInt());
	const decColor &color = ds.GetClassColor()->GetColor(rt->GetValue(1)->GetRealObject());
	
	ddshape.SetFillColor(color);
	ddrawer.NotifyShapeColorChanged();
}

// public func void shapeAddFace(int shape)
deClassDebugDrawer::nfShapeAddFace::nfShapeAddFace(const sInitData &init) :
dsFunction(init.clsDD, "shapeAddFace", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // shape
}
void deClassDebugDrawer::nfShapeAddFace::RunFunction(dsRunTime *rt, dsValue *myself){
	deDebugDrawer &ddrawer = *(((sDDNatDat*)p_GetNativeData(myself))->ddrawer);
	deDebugDrawerShape &ddshape = *ddrawer.GetShapeAt(rt->GetValue(0)->GetInt());
	ddshape.AddFace(new deDebugDrawerShapeFace);
}

// public func void shapeFaceAddVertex(int shape, int face, Vector vertex)
deClassDebugDrawer::nfShapeFaceAddVertex::nfShapeFaceAddVertex(const sInitData &init) :
dsFunction(init.clsDD, "shapeFaceAddVertex", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // shape
	p_AddParameter(init.clsInt); // face
	p_AddParameter(init.clsVec); // vertex
}
void deClassDebugDrawer::nfShapeFaceAddVertex::RunFunction(dsRunTime *rt, dsValue *myself){
	deDebugDrawer &ddrawer = *(((sDDNatDat*)p_GetNativeData(myself))->ddrawer);
	const deScriptingDragonScript &ds = *(((deClassDebugDrawer*)GetOwnerClass())->GetDS());
	
	deDebugDrawerShape &ddshape = *ddrawer.GetShapeAt(rt->GetValue(0)->GetInt());
	deDebugDrawerShapeFace &ddsface = *ddshape.GetFaceAt(rt->GetValue(1)->GetInt());
	ddsface.AddVertex(ds.GetClassVector()->GetVector(rt->GetValue(2)->GetRealObject()));
}

// public func void shapeFaceCalculateNormal(int shape, int face)
deClassDebugDrawer::nfShapeFaceCalculateNormal::nfShapeFaceCalculateNormal(const sInitData &init) :
dsFunction(init.clsDD, "shapeFaceCalculateNormal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // shape
	p_AddParameter(init.clsInt); // face
}
void deClassDebugDrawer::nfShapeFaceCalculateNormal::RunFunction(dsRunTime *rt, dsValue *myself){
	deDebugDrawer &ddrawer = *(((sDDNatDat*)p_GetNativeData(myself))->ddrawer);
	deDebugDrawerShape &ddshape = *ddrawer.GetShapeAt(rt->GetValue(0)->GetInt());
	ddshape.GetFaceAt(rt->GetValue(1)->GetInt())->CalculateNormal();
}

// public func void shapeRemoveAllFaces(int shape)
deClassDebugDrawer::nfShapeRemoveAllFaces::nfShapeRemoveAllFaces(const sInitData &init) :
dsFunction(init.clsDD, "shapeRemoveAllFaces", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // shape
}
void deClassDebugDrawer::nfShapeRemoveAllFaces::RunFunction(dsRunTime *rt, dsValue *myself){
	deDebugDrawer &ddrawer = *(((sDDNatDat*)p_GetNativeData(myself))->ddrawer);
	ddrawer.GetShapeAt(rt->GetValue(0)->GetInt())->RemoveAllFaces();
	ddrawer.NotifyShapeContentChanged();
}

// public func void shapeFinishedFaces(int shape)
deClassDebugDrawer::nfShapeFinishedFaces::nfShapeFinishedFaces(const sInitData &init) :
dsFunction(init.clsDD, "shapeFinishedFaces", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // shape
}
void deClassDebugDrawer::nfShapeFinishedFaces::RunFunction(dsRunTime *rt, dsValue *myself){
	deDebugDrawer &ddrawer = *(((sDDNatDat*)p_GetNativeData(myself))->ddrawer);
	ddrawer.NotifyShapeContentChanged();
}

// public func void shapeSetFacesFromModel(int shape, Model model, Matrix matrix)
deClassDebugDrawer::nfShapeSetFacesFromModel::nfShapeSetFacesFromModel(const sInitData &init) :
dsFunction(init.clsDD, "shapeSetFacesFromModel", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // shape
	p_AddParameter(init.clsModel); // model
	p_AddParameter(init.clsMatrix); // matrix
}
void deClassDebugDrawer::nfShapeSetFacesFromModel::RunFunction(dsRunTime *rt, dsValue *myself){
	deDebugDrawer &ddrawer = *(((sDDNatDat*)p_GetNativeData(myself))->ddrawer);
	const deScriptingDragonScript &ds = *(((deClassDebugDrawer*)GetOwnerClass())->GetDS());
	
	deDebugDrawerShape &ddshape = *ddrawer.GetShapeAt(rt->GetValue(0)->GetInt());
	const deModel * const model = ds.GetClassModel()->GetModel(rt->GetValue(1)->GetRealObject());
	if(!model){
		DSTHROW_INFO(dueNullPointer, "model");
	}
	
	const decMatrix &matrix = ds.GetClassMatrix()->GetMatrix(rt->GetValue(2)->GetRealObject());
	
	const deModelLOD &lod = *model->GetLODAt(0);
	const deModelVertex * const vertices = lod.GetVertices();
	const deModelFace * const faces = lod.GetFaces();
	deDebugDrawerShapeFace *ddsface = nullptr;
	const int faceCount = lod.GetFaceCount();
	int i;
	
	ddshape.RemoveAllFaces();
	
	try{
		for(i=0; i<faceCount; i++){
			const deModelFace &face = faces[i];
			ddsface = new deDebugDrawerShapeFace;
			ddsface->AddVertex(matrix * vertices[face.GetVertex1()].GetPosition());
			ddsface->AddVertex(matrix * vertices[face.GetVertex2()].GetPosition());
			ddsface->AddVertex(matrix * vertices[face.GetVertex3()].GetPosition());
			ddsface->CalculateNormal();
			ddshape.AddFace(ddsface);
			ddsface = nullptr;
		}
		
	}catch(...){
		if(ddsface){
			delete ddsface;
		}
		throw;
	}
	
	ddrawer.NotifyShapeContentChanged();
}

// public func void shapeSetFacesFromModel(int shape, Model model, String texture, Matrix matrix)
deClassDebugDrawer::nfShapeSetFacesFromModel2::nfShapeSetFacesFromModel2(const sInitData &init) :
dsFunction(init.clsDD, "shapeSetFacesFromModel", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // shape
	p_AddParameter(init.clsModel); // model
	p_AddParameter(init.clsStr); // texture
	p_AddParameter(init.clsMatrix); // matrix
}
void deClassDebugDrawer::nfShapeSetFacesFromModel2::RunFunction(dsRunTime *rt, dsValue *myself){
	deDebugDrawer &ddrawer = *(((sDDNatDat*)p_GetNativeData(myself))->ddrawer);
	const deScriptingDragonScript &ds = *(((deClassDebugDrawer*)GetOwnerClass())->GetDS());
	
	deDebugDrawerShape &ddshape = *ddrawer.GetShapeAt(rt->GetValue(0)->GetInt());
	const deModel * const model = ds.GetClassModel()->GetModel(rt->GetValue(1)->GetRealObject());
	if(!model){
		DSTHROW_INFO(dueNullPointer, "model");
	}
	
	ddshape.RemoveAllFaces();
	
	const int texture = model->IndexOfTextureNamed(rt->GetValue(2)->GetString());
	if(texture == -1){
		return;
	}
	
	const decMatrix &matrix = ds.GetClassMatrix()->GetMatrix(rt->GetValue(3)->GetRealObject());
	
	const deModelLOD &lod = *model->GetLODAt(0);
	const deModelVertex * const vertices = lod.GetVertices();
	const deModelFace * const faces = lod.GetFaces();
	deDebugDrawerShapeFace *ddsface = nullptr;
	const int faceCount = lod.GetFaceCount();
	int i;
	
	try{
		for(i=0; i<faceCount; i++){
			const deModelFace &face = faces[i];
			if(face.GetTexture() != texture){
				continue;
			}
			
			ddsface = new deDebugDrawerShapeFace;
			ddsface->AddVertex(matrix * vertices[face.GetVertex1()].GetPosition());
			ddsface->AddVertex(matrix * vertices[face.GetVertex2()].GetPosition());
			ddsface->AddVertex(matrix * vertices[face.GetVertex3()].GetPosition());
			ddsface->CalculateNormal();
			ddshape.AddFace(ddsface);
			ddsface = nullptr;
		}
		
	}catch(...){
		if(ddsface){
			delete ddsface;
		}
		throw;
	}
	
	ddrawer.NotifyShapeContentChanged();
}



// Class deClassDebugDrawer
/////////////////////////////

// Constructor
////////////////

deClassDebugDrawer::deClassDebugDrawer(deScriptingDragonScript *ds) :
dsClass("DebugDrawer", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED){
	if(!ds){
		DSTHROW(dueInvalidParam);
	}
	
	pDS = ds;
	
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sDDNatDat));
}

deClassDebugDrawer::~deClassDebugDrawer(){
}



// Management
///////////////

void deClassDebugDrawer::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	init.clsDD = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	init.clsClr = pDS->GetClassColor();
	init.clsVec = pDS->GetClassVector();
	init.clsDVec = pDS->GetClassDVector();
	init.clsQuat = pDS->GetClassQuaternion();
	init.clsShapeList = pDS->GetClassShapeList();
	init.clsModel = pDS->GetClassModel();
	init.clsMatrix = pDS->GetClassMatrix();
	init.clsWorld = pDS->GetClassWorld();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetPosition(init));
	AddFunction(new nfSetPosition(init));
	AddFunction(new nfGetOrientation(init));
	AddFunction(new nfSetOrientation(init));
	AddFunction(new nfGetScale(init));
	AddFunction(new nfSetScale(init));
	AddFunction(new nfGetParentWorld(init));
	
	AddFunction(new nfGetVisible(init));
	AddFunction(new nfSetVisible(init));
	AddFunction(new nfGetXRay(init));
	AddFunction(new nfSetXRay(init));
	
	AddFunction(new nfGetShapeCount(init));
	AddFunction(new nfSetShapeCount(init));
	
	AddFunction(new nfShapeSetPosition(init));
	AddFunction(new nfShapeSetOrientation(init));
	AddFunction(new nfShapeSetScale(init));
	
	AddFunction(new nfShapeSetShapes(init));
	AddFunction(new nfShapeRemoveAllShapes(init));
	
	AddFunction(new nfShapeSetEdgeColor(init));
	AddFunction(new nfShapeSetFillColor(init));
	
	AddFunction(new nfShapeAddFace(init));
	AddFunction(new nfShapeFaceAddVertex(init));
	AddFunction(new nfShapeFaceCalculateNormal(init));
	AddFunction(new nfShapeRemoveAllFaces(init));
	AddFunction(new nfShapeFinishedFaces(init));
	AddFunction(new nfShapeSetFacesFromModel(init));
	AddFunction(new nfShapeSetFacesFromModel2(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

deDebugDrawer* deClassDebugDrawer::GetDebugDrawer(dsRealObject *myself) const{
	if(!myself){
		return NULL;
	}
	
	return ((sDDNatDat*)p_GetNativeData(myself->GetBuffer()))->ddrawer;
}

void deClassDebugDrawer::PushDebugDrawer(dsRunTime *rt, deDebugDrawer::Ref debugDrawer){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!debugDrawer){
		rt->PushObject(NULL, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	((sDDNatDat*)p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()))->ddrawer = debugDrawer;
}
