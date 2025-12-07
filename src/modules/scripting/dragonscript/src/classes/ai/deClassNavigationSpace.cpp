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

#include "deClassNavigationSpace.h"
#include "../math/deClassVector.h"
#include "../math/deClassDVector.h"
#include "../math/deClassQuaternion.h"
#include "../utils/deClassShapeList.h"
#include "../world/deClassWorld.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/navigation/space/deNavigationSpace.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceCorner.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceEdge.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceFace.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceRoom.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceWall.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceManager.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



struct sNavSpaceNatDat{
	deNavigationSpace *navspace;
};



// Constructors, Destructors
//////////////////////////////

// public func new()
deClassNavigationSpace::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsNavSpace,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassNavigationSpace::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sNavSpaceNatDat &nd = *((sNavSpaceNatDat*)p_GetNativeData(myself));
	const deClassNavigationSpace &clsNavSpace = *((deClassNavigationSpace*)GetOwnerClass());
	deNavigationSpaceManager &navSpaceMgr = *clsNavSpace.GetDS()->GetGameEngine()->GetNavigationSpaceManager();
	
	// clear ( important )
	nd.navspace = NULL;
	
	// create navigation space
	nd.navspace = navSpaceMgr.CreateNavigationSpace();
}

// public func new( NavigationSpace copy )
deClassNavigationSpace::nfNewCopy::nfNewCopy(const sInitData &init) : dsFunction(init.clsNavSpace,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsNavSpace); // copy
}
void deClassNavigationSpace::nfNewCopy::RunFunction(dsRunTime *rt, dsValue *myself){
	sNavSpaceNatDat &nd = *((sNavSpaceNatDat*)p_GetNativeData(myself));
	const deClassNavigationSpace &clsNavSpace = *((deClassNavigationSpace*)GetOwnerClass());
	deNavigationSpaceManager &navSpaceMgr = *clsNavSpace.GetDS()->
		GetGameEngine()->GetNavigationSpaceManager();
	
	// clear ( important )
	nd.navspace = NULL;
	
	// create copy of navigation space
	const deNavigationSpace * const copy = clsNavSpace.GetNavigationSpace(
		rt->GetValue(0)->GetRealObject());
	if(! copy){
		DSTHROW(dueNullPointer);
	}
	
	nd.navspace = navSpaceMgr.CreateNavigationSpace();
	
	try{
		nd.navspace->SetType(copy->GetType());
		nd.navspace->SetLayer(copy->GetLayer());
		nd.navspace->SetPosition(copy->GetPosition());
		nd.navspace->SetOrientation(copy->GetOrientation());
		
		nd.navspace->SetVertexCount(copy->GetVertexCount());
		if(copy->GetVertexCount() > 0){
			memcpy(nd.navspace->GetVertices(), copy->GetVertices(),
				sizeof(decVector) * copy->GetVertexCount());
		}
		
		nd.navspace->SetEdgeCount(copy->GetEdgeCount());
		if(copy->GetEdgeCount() > 0){
			memcpy(nd.navspace->GetEdges(), copy->GetEdges(),
				sizeof(deNavigationSpaceEdge) * copy->GetEdgeCount());
		}
		
		nd.navspace->SetCornerCount(copy->GetCornerCount());
		if(copy->GetCornerCount() > 0){
			memcpy(nd.navspace->GetCorners(), copy->GetCorners(),
				sizeof(deNavigationSpaceCorner) * copy->GetCornerCount());
		}
		
		nd.navspace->SetFaceCount(copy->GetFaceCount());
		if(copy->GetFaceCount() > 0){
			memcpy(nd.navspace->GetFaces(), copy->GetFaces(),
				sizeof(deNavigationSpaceFace) * copy->GetFaceCount());
		}
		
		nd.navspace->SetWallCount(copy->GetWallCount());
		if(copy->GetWallCount() > 0){
			memcpy(nd.navspace->GetWalls(), copy->GetWalls(),
				sizeof(deNavigationSpaceWall) * copy->GetWallCount());
		}
		
		nd.navspace->SetRoomCount(copy->GetRoomCount());
		if(copy->GetRoomCount() > 0){
			memcpy(nd.navspace->GetRooms(), copy->GetRooms(),
				sizeof(deNavigationSpaceRoom) * copy->GetRoomCount());
		}
		
		nd.navspace->SetSnapDistance(copy->GetSnapDistance());
		nd.navspace->SetSnapAngle(copy->GetSnapAngle());
		
		nd.navspace->SetBlockingPriority(copy->GetBlockingPriority());
		nd.navspace->GetBlockerShapeList() = copy->GetBlockerShapeList();
		nd.navspace->NotifyBlockerShapeListChanged();
		
	}catch(...){
		nd.navspace->FreeReference();
		nd.navspace = NULL;
		throw;
	}
}

// public func destructor()
deClassNavigationSpace::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsNavSpace,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassNavigationSpace::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sNavSpaceNatDat &nd = *((sNavSpaceNatDat*)p_GetNativeData(myself));
	
	if(nd.navspace){
		nd.navspace->FreeReference();
		nd.navspace = NULL;
	}
}



// Management
///////////////

// public func DVector getPosition()
deClassNavigationSpace::nfGetPosition::nfGetPosition(const sInitData &init) : dsFunction(init.clsNavSpace,
"getPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
}
void deClassNavigationSpace::nfGetPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	const deScriptingDragonScript &ds = *(((deClassNavigationSpace*)GetOwnerClass())->GetDS());
	
	ds.GetClassDVector()->PushDVector(rt, navspace.GetPosition());
}

// public func void setPosition( DVector position )
deClassNavigationSpace::nfSetPosition::nfSetPosition(const sInitData &init) : dsFunction(init.clsNavSpace,
"setPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsDVec); // position
}
void deClassNavigationSpace::nfSetPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	const deScriptingDragonScript &ds = *(((deClassNavigationSpace*)GetOwnerClass())->GetDS());
	
	dsRealObject * const objPosition = rt->GetValue(0)->GetRealObject();
	
	if(! objPosition){
		DSTHROW(dueNullPointer);
	}
	
	navspace.SetPosition(ds.GetClassDVector()->GetDVector(objPosition));
}

// public func Quaternion getOrientation()
deClassNavigationSpace::nfGetOrientation::nfGetOrientation(const sInitData &init) : dsFunction(init.clsNavSpace,
"getOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat){
}
void deClassNavigationSpace::nfGetOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	const deScriptingDragonScript &ds = *(((deClassNavigationSpace*)GetOwnerClass())->GetDS());
	
	ds.GetClassQuaternion()->PushQuaternion(rt, navspace.GetOrientation());
}

// public func void setOrientation( Quaternion orientation )
deClassNavigationSpace::nfSetOrientation::nfSetOrientation(const sInitData &init) : dsFunction(init.clsNavSpace,
"setOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsQuat); // orientation
}
void deClassNavigationSpace::nfSetOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	const deScriptingDragonScript &ds = *(((deClassNavigationSpace*)GetOwnerClass())->GetDS());
	
	dsRealObject * const objOrientation = rt->GetValue(0)->GetRealObject();
	
	if(! objOrientation){
		DSTHROW(dueNullPointer);
	}
	
	navspace.SetOrientation(ds.GetClassQuaternion()->GetQuaternion(objOrientation));
}

// public func void int getLayer()
deClassNavigationSpace::nfGetLayer::nfGetLayer(const sInitData &init) : dsFunction(init.clsNavSpace,
"getLayer", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassNavigationSpace::nfGetLayer::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	
	rt->PushInt(navspace.GetLayer());
}

// public func void setLayer( int layer )
deClassNavigationSpace::nfSetLayer::nfSetLayer(const sInitData &init) : dsFunction(init.clsNavSpace,
"setLayer", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // layer
}
void deClassNavigationSpace::nfSetLayer::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	
	navspace.SetLayer(rt->GetValue(0)->GetInt());
}

// public func void NavigationSpaceType getType()
deClassNavigationSpace::nfGetType::nfGetType(const sInitData &init) : dsFunction(init.clsNavSpace,
"getType", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsNavigationSpaceType){
}
void deClassNavigationSpace::nfGetType::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	
	rt->PushValue(((deClassNavigationSpace*)GetOwnerClass())->GetClassNavigationSpaceType()
		->GetVariable(navspace.GetType())->GetStaticValue());
}

// public func void setType( NavigationSpaceType type )
deClassNavigationSpace::nfSetType::nfSetType(const sInitData &init) : dsFunction(init.clsNavSpace,
"setType", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsNavigationSpaceType); // type
}
void deClassNavigationSpace::nfSetType::RunFunction(dsRunTime *rt, dsValue *myself){
	if(! rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	
	navspace.SetType((deNavigationSpace::eSpaceTypes)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
}

// public func void int getBlockingPriority()
deClassNavigationSpace::nfGetBlockingPriority::nfGetBlockingPriority(const sInitData &init) : dsFunction(init.clsNavSpace,
"getBlockingPriority", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassNavigationSpace::nfGetBlockingPriority::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	
	rt->PushInt(navspace.GetBlockingPriority());
}

// public func void setBlockingPriority( int priority )
deClassNavigationSpace::nfSetBlockingPriority::nfSetBlockingPriority(const sInitData &init) : dsFunction(init.clsNavSpace,
"setBlockingPriority", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // priority
}
void deClassNavigationSpace::nfSetBlockingPriority::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	
	navspace.SetBlockingPriority(rt->GetValue(0)->GetInt());
}

// func World getParentWorld()
deClassNavigationSpace::nfGetParentWorld::nfGetParentWorld(const sInitData &init) :
dsFunction(init.clsNavSpace, "getParentWorld", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsWorld){
}

void deClassNavigationSpace::nfGetParentWorld::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNavigationSpace &navspace = *((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace;
	const deScriptingDragonScript &ds = *((deClassNavigationSpace *)GetOwnerClass())->GetDS();
	
	ds.GetClassWorld()->PushWorld(rt, navspace.GetParentWorld());
}

// public func int getVertexCount()
deClassNavigationSpace::nfGetVertexCount::nfGetVertexCount(const sInitData &init) : dsFunction(init.clsNavSpace,
"getVertexCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassNavigationSpace::nfGetVertexCount::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	
	rt->PushInt(navspace.GetVertexCount());
}

// public func void setVertexCount( int count )
deClassNavigationSpace::nfSetVertexCount::nfSetVertexCount(const sInitData &init) : dsFunction(init.clsNavSpace,
"setVertexCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // count
}
void deClassNavigationSpace::nfSetVertexCount::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	
	navspace.SetVertexCount(rt->GetValue(0)->GetInt());
}

// public func Vector getVertexAt( int index )
deClassNavigationSpace::nfGetVertexAt::nfGetVertexAt(const sInitData &init) : dsFunction(init.clsNavSpace,
"getVertexAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
	p_AddParameter(init.clsInt); // index
}
void deClassNavigationSpace::nfGetVertexAt::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	const deScriptingDragonScript &ds = *(((deClassNavigationSpace*)GetOwnerClass())->GetDS());
	const int index = rt->GetValue(0)->GetInt();
	
	ds.GetClassVector()->PushVector(rt, navspace.GetVertexAt(index));
}

// public func void setVertexAt( int index, Vector position )
deClassNavigationSpace::nfSetVertexAt::nfSetVertexAt(const sInitData &init) : dsFunction(init.clsNavSpace,
"setVertexAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // index
	p_AddParameter(init.clsVec); // position
}
void deClassNavigationSpace::nfSetVertexAt::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	const deScriptingDragonScript &ds = *(((deClassNavigationSpace*)GetOwnerClass())->GetDS());
	const int index = rt->GetValue(0)->GetInt();
	dsRealObject * const objPosition = rt->GetValue(1)->GetRealObject();
	
	if(! objPosition){
		DSTHROW(dueNullPointer);
	}
	
	navspace.SetVertexAt(index, ds.GetClassVector()->GetVector(objPosition));
}



// public func int getEdgeCount()
deClassNavigationSpace::nfGetEdgeCount::nfGetEdgeCount(const sInitData &init) : dsFunction(init.clsNavSpace,
"getEdgeCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassNavigationSpace::nfGetEdgeCount::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	
	rt->PushInt(navspace.GetEdgeCount());
}

// public func void setEdgeCount( int count )
deClassNavigationSpace::nfSetEdgeCount::nfSetEdgeCount(const sInitData &init) : dsFunction(init.clsNavSpace,
"setEdgeCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // count
}
void deClassNavigationSpace::nfSetEdgeCount::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	
	navspace.SetEdgeCount(rt->GetValue(0)->GetInt());
}

// public func int getEdgeVertex1( int edge )
deClassNavigationSpace::nfGetEdgeVertex1::nfGetEdgeVertex1(const sInitData &init) : dsFunction(init.clsNavSpace,
"getEdgeVertex1", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // edge
}
void deClassNavigationSpace::nfGetEdgeVertex1::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	deNavigationSpaceEdge &edge = navspace.GetEdgeAt(rt->GetValue(0)->GetInt());
	
	rt->PushInt((int)edge.GetVertex1());
}

// public func int getEdgeVertex2( int edge )
deClassNavigationSpace::nfGetEdgeVertex2::nfGetEdgeVertex2(const sInitData &init) : dsFunction(init.clsNavSpace,
"getEdgeVertex2", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // edge
}
void deClassNavigationSpace::nfGetEdgeVertex2::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	deNavigationSpaceEdge &edge = navspace.GetEdgeAt(rt->GetValue(0)->GetInt());
	
	rt->PushInt((int)edge.GetVertex2());
}

// public func int getEdgeType1( int edge )
deClassNavigationSpace::nfGetEdgeType1::nfGetEdgeType1(const sInitData &init) : dsFunction(init.clsNavSpace,
"getEdgeType1", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // edge
}
void deClassNavigationSpace::nfGetEdgeType1::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	deNavigationSpaceEdge &edge = navspace.GetEdgeAt(rt->GetValue(0)->GetInt());
	
	rt->PushInt((int)edge.GetType1());
}

// public func int getEdgeType2( int edge )
deClassNavigationSpace::nfGetEdgeType2::nfGetEdgeType2(const sInitData &init) : dsFunction(init.clsNavSpace,
"getEdgeType2", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // edge
}
void deClassNavigationSpace::nfGetEdgeType2::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	deNavigationSpaceEdge &edge = navspace.GetEdgeAt(rt->GetValue(0)->GetInt());
	
	rt->PushInt((int)edge.GetType2());
}

// public func void setEdgeAt( int edge, int vertex1, int vertex2, int type1, int type2 )
deClassNavigationSpace::nfSetEdgeAt::nfSetEdgeAt(const sInitData &init) : dsFunction(init.clsNavSpace,
"setEdgeAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // edge
	p_AddParameter(init.clsInt); // vertex1
	p_AddParameter(init.clsInt); // vertex2
	p_AddParameter(init.clsInt); // type1
	p_AddParameter(init.clsInt); // type2
}
void deClassNavigationSpace::nfSetEdgeAt::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	deNavigationSpaceEdge &edge = navspace.GetEdgeAt(rt->GetValue(0)->GetInt());
	
	edge.SetVertex1((unsigned short)rt->GetValue(1)->GetInt());
	edge.SetVertex2((unsigned short)rt->GetValue(2)->GetInt());
	edge.SetType1((unsigned short)rt->GetValue(3)->GetInt());
	edge.SetType2((unsigned short)rt->GetValue(4)->GetInt());
}



// public func int getCornerCount()
deClassNavigationSpace::nfGetCornerCount::nfGetCornerCount(const sInitData &init) : dsFunction(init.clsNavSpace,
"getCornerCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassNavigationSpace::nfGetCornerCount::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	
	rt->PushInt(navspace.GetCornerCount());
}

// public func void setCornerCount( int count )
deClassNavigationSpace::nfSetCornerCount::nfSetCornerCount(const sInitData &init) : dsFunction(init.clsNavSpace,
"setCornerCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // count
}
void deClassNavigationSpace::nfSetCornerCount::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	
	navspace.SetCornerCount(rt->GetValue(0)->GetInt());
}

// public func int getCornerVertex( int corner )
deClassNavigationSpace::nfGetCornerVertex::nfGetCornerVertex(const sInitData &init) : dsFunction(init.clsNavSpace,
"getCornerVertex", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // corner
}
void deClassNavigationSpace::nfGetCornerVertex::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	deNavigationSpaceCorner &corner = navspace.GetCornerAt(rt->GetValue(0)->GetInt());
	
	rt->PushInt((int)corner.GetVertex());
}

// public func int getCornerType( int corner )
deClassNavigationSpace::nfGetCornerType::nfGetCornerType(const sInitData &init) : dsFunction(init.clsNavSpace,
"getCornerType", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // corner
}
void deClassNavigationSpace::nfGetCornerType::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	deNavigationSpaceCorner &corner = navspace.GetCornerAt(rt->GetValue(0)->GetInt());
	
	rt->PushInt((int)corner.GetType());
}

// public func void setCornerAt( int corner, int vertex, int type )
deClassNavigationSpace::nfSetCornerAt::nfSetCornerAt(const sInitData &init) : dsFunction(init.clsNavSpace,
"setCornerAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // edge
	p_AddParameter(init.clsInt); // vertex
	p_AddParameter(init.clsInt); // type
}
void deClassNavigationSpace::nfSetCornerAt::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	deNavigationSpaceCorner &corner = navspace.GetCornerAt(rt->GetValue(0)->GetInt());
	
	corner.SetVertex((unsigned short)rt->GetValue(1)->GetInt());
	corner.SetType((unsigned short)rt->GetValue(2)->GetInt());
}



// public func int getFaceCount()
deClassNavigationSpace::nfGetFaceCount::nfGetFaceCount(const sInitData &init) : dsFunction(init.clsNavSpace,
"getFaceCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassNavigationSpace::nfGetFaceCount::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	
	rt->PushInt(navspace.GetFaceCount());
}

// public func void setFaceCount( int count )
deClassNavigationSpace::nfSetFaceCount::nfSetFaceCount(const sInitData &init) : dsFunction(init.clsNavSpace,
"setFaceCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // count
}
void deClassNavigationSpace::nfSetFaceCount::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	
	navspace.SetFaceCount(rt->GetValue(0)->GetInt());
}

// public func int getFaceCornerCount( int face )
deClassNavigationSpace::nfGetFaceCornerCount::nfGetFaceCornerCount(const sInitData &init) : dsFunction(init.clsNavSpace,
"getFaceCornerCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // index
}
void deClassNavigationSpace::nfGetFaceCornerCount::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	deNavigationSpaceFace &face = navspace.GetFaceAt(rt->GetValue(0)->GetInt());
	
	rt->PushInt((int)face.GetCornerCount());
}

// public func int getFaceType( int face )
deClassNavigationSpace::nfGetFaceType::nfGetFaceType(const sInitData &init) : dsFunction(init.clsNavSpace,
"getFaceType", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // face
}
void deClassNavigationSpace::nfGetFaceType::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	deNavigationSpaceFace &face = navspace.GetFaceAt(rt->GetValue(0)->GetInt());
	
	rt->PushInt((int)face.GetType());
}

// public func void setFaceAt( int face, int cornerCount, int type )
deClassNavigationSpace::nfSetFaceAt::nfSetFaceAt(const sInitData &init) : dsFunction(init.clsNavSpace,
"setFaceAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // face
	p_AddParameter(init.clsInt); // cornerCount
	p_AddParameter(init.clsInt); // type
}
void deClassNavigationSpace::nfSetFaceAt::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	deNavigationSpaceFace &face = navspace.GetFaceAt(rt->GetValue(0)->GetInt());
	
	face.SetCornerCount((unsigned short)rt->GetValue(1)->GetInt());
	face.SetType((unsigned short)rt->GetValue(2)->GetInt());
}



// public func int getWallCount()
deClassNavigationSpace::nfGetWallCount::nfGetWallCount(const sInitData &init) : dsFunction(init.clsNavSpace,
"getWallCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassNavigationSpace::nfGetWallCount::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	
	rt->PushInt(navspace.GetWallCount());
}

// public func void setWallCount( int count )
deClassNavigationSpace::nfSetWallCount::nfSetWallCount(const sInitData &init) : dsFunction(init.clsNavSpace,
"setWallCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // count
}
void deClassNavigationSpace::nfSetWallCount::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	
	navspace.SetWallCount(rt->GetValue(0)->GetInt());
}

// public func int getWallFace( int wall )
deClassNavigationSpace::nfGetWallFace::nfGetWallFace(const sInitData &init) : dsFunction(init.clsNavSpace,
"getWallFace", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // wall
}
void deClassNavigationSpace::nfGetWallFace::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	deNavigationSpaceWall &wall = navspace.GetWallAt(rt->GetValue(0)->GetInt());
	
	rt->PushInt((int)wall.GetFace());
}

// public func int getWallType( int wall )
deClassNavigationSpace::nfGetWallType::nfGetWallType(const sInitData &init) : dsFunction(init.clsNavSpace,
"getWallType", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // wall
}
void deClassNavigationSpace::nfGetWallType::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	deNavigationSpaceWall &wall = navspace.GetWallAt(rt->GetValue(0)->GetInt());
	
	rt->PushInt((int)wall.GetType());
}

// public func void setWallAt( int wall, int face, int type )
deClassNavigationSpace::nfSetWallAt::nfSetWallAt(const sInitData &init) : dsFunction(init.clsNavSpace,
"setWallAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // wall
	p_AddParameter(init.clsInt); // face
	p_AddParameter(init.clsInt); // type
}
void deClassNavigationSpace::nfSetWallAt::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	deNavigationSpaceWall &wall = navspace.GetWallAt(rt->GetValue(0)->GetInt());
	
	wall.SetFace((unsigned short)rt->GetValue(1)->GetInt());
	wall.SetType((unsigned short)rt->GetValue(2)->GetInt());
}



// public func int getRoomCount()
deClassNavigationSpace::nfGetRoomCount::nfGetRoomCount(const sInitData &init) : dsFunction(init.clsNavSpace,
"getRoomCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassNavigationSpace::nfGetRoomCount::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	
	rt->PushInt(navspace.GetRoomCount());
}

// public func void setRoomCount( int count )
deClassNavigationSpace::nfSetRoomCount::nfSetRoomCount(const sInitData &init) : dsFunction(init.clsNavSpace,
"setRoomCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // count
}
void deClassNavigationSpace::nfSetRoomCount::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	
	navspace.SetRoomCount(rt->GetValue(0)->GetInt());
}

// public func int getRoomFrontWallCount( int room )
deClassNavigationSpace::nfGetRoomFrontWallCount::nfGetRoomFrontWallCount(const sInitData &init) : dsFunction(init.clsNavSpace,
"getRoomFrontWallCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // room
}
void deClassNavigationSpace::nfGetRoomFrontWallCount::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	deNavigationSpaceRoom &room = navspace.GetRoomAt(rt->GetValue(0)->GetInt());
	
	rt->PushInt((int)room.GetFrontWallCount());
}

// public func int getRoomBackWallCount( int room )
deClassNavigationSpace::nfGetRoomBackWallCount::nfGetRoomBackWallCount(const sInitData &init) : dsFunction(init.clsNavSpace,
"getRoomBackWallCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // room
}
void deClassNavigationSpace::nfGetRoomBackWallCount::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	deNavigationSpaceRoom &room = navspace.GetRoomAt(rt->GetValue(0)->GetInt());
	
	rt->PushInt((int)room.GetBackWallCount());
}

// public func int getRoomType()
deClassNavigationSpace::nfGetRoomType::nfGetRoomType(const sInitData &init) : dsFunction(init.clsNavSpace,
"getRoomType", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // room
}
void deClassNavigationSpace::nfGetRoomType::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	deNavigationSpaceRoom &room = navspace.GetRoomAt(rt->GetValue(0)->GetInt());
	
	rt->PushInt((int)room.GetType());
}

// public func void setRoomAt( int room, int frontWallCount, int backWallCount, int type )
deClassNavigationSpace::nfSetRoomAt::nfSetRoomAt(const sInitData &init) : dsFunction(init.clsNavSpace,
"setRoomAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // room
	p_AddParameter(init.clsInt); // frontWallCount
	p_AddParameter(init.clsInt); // backWallCount
	p_AddParameter(init.clsInt); // type
}
void deClassNavigationSpace::nfSetRoomAt::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	deNavigationSpaceRoom &room = navspace.GetRoomAt(rt->GetValue(0)->GetInt());
	
	room.SetFrontWallCount((unsigned short)rt->GetValue(1)->GetInt());
	room.SetBackWallCount((unsigned short)rt->GetValue(2)->GetInt());
	room.SetType((unsigned short)rt->GetValue(3)->GetInt());
}



// public func float getSnapDistance()
deClassNavigationSpace::nfGetSnapDistance::nfGetSnapDistance(const sInitData &init) : dsFunction(init.clsNavSpace,
"getSnapDistance", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassNavigationSpace::nfGetSnapDistance::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	rt->PushFloat(navspace.GetSnapDistance());
}

// public func void setSnapDistance( float distance )
deClassNavigationSpace::nfSetSnapDistance::nfSetSnapDistance(const sInitData &init) : dsFunction(init.clsNavSpace,
"setSnapDistance", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // distance
}
void deClassNavigationSpace::nfSetSnapDistance::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	navspace.SetSnapDistance(rt->GetValue(0)->GetFloat());
}

// public func float getSnapAngle()
deClassNavigationSpace::nfGetSnapAngle::nfGetSnapAngle(const sInitData &init) : dsFunction(init.clsNavSpace,
"getSnapAngle", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassNavigationSpace::nfGetSnapAngle::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	rt->PushFloat(navspace.GetSnapAngle() / DEG2RAD);
}

// public func void setSnapAngle( float angle )
deClassNavigationSpace::nfSetSnapAngle::nfSetSnapAngle(const sInitData &init) : dsFunction(init.clsNavSpace,
"setSnapAngle", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // angle
}
void deClassNavigationSpace::nfSetSnapAngle::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	navspace.SetSnapAngle(rt->GetValue(0)->GetFloat() * DEG2RAD);
}



// public func void layoutChanged()
deClassNavigationSpace::nfLayoutChanged::nfLayoutChanged(const sInitData &init) : dsFunction(init.clsNavSpace,
"layoutChanged", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassNavigationSpace::nfLayoutChanged::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	
	navspace.NotifyLayoutChanged();
}



// public func ShapeList getBlockerShapeList()
deClassNavigationSpace::nfGetBlockerShapeList::nfGetBlockerShapeList(const sInitData &init) : dsFunction(init.clsNavSpace,
"getBlockerShapeList", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsShaList){
}
void deClassNavigationSpace::nfGetBlockerShapeList::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	const deScriptingDragonScript &ds = *(((deClassNavigationSpace*)GetOwnerClass())->GetDS());
	
	ds.GetClassShapeList()->PushShapeList(rt, navspace.GetBlockerShapeList());
}

// public func void setBlockerShapeList( ShapeList shapeList )
deClassNavigationSpace::nfSetBlockerShapeList::nfSetBlockerShapeList(const sInitData &init) : dsFunction(init.clsNavSpace,
"setBlockerShapeList", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsShaList); // shapeList
}
void deClassNavigationSpace::nfSetBlockerShapeList::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigationSpace &navspace = *(((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace);
	const deScriptingDragonScript &ds = *(((deClassNavigationSpace*)GetOwnerClass())->GetDS());
	dsRealObject * const objShapeList = rt->GetValue(0)->GetRealObject();
	
	navspace.GetBlockerShapeList() = ds.GetClassShapeList()->GetShapeList(objShapeList);
	navspace.NotifyBlockerShapeListChanged();
}



// public func int hashCode()
deClassNavigationSpace::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsNavSpace, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}

void deClassNavigationSpace::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNavigationSpace * const navspace = ((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace;
	
	rt->PushInt((int)(intptr_t)navspace);
}

// public func bool equals( Object object )
deClassNavigationSpace::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsNavSpace, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // object
}
void deClassNavigationSpace::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNavigationSpace * const navspace = ((const sNavSpaceNatDat *)p_GetNativeData(myself))->navspace;
	deClassNavigationSpace * const clsNavSpace = (deClassNavigationSpace*)GetOwnerClass();
	dsValue * const object = rt->GetValue(0);
	
	if(! p_IsObjOfType(object, clsNavSpace)){
		rt->PushBool(false);
		
	}else{
		const deNavigationSpace * const otherNavSpace = ((const sNavSpaceNatDat *)p_GetNativeData(object))->navspace;
		rt->PushBool(navspace == otherNavSpace);
	}
}



// Class deClassNavigationSpace
////////////////////////////////

// Constructor
////////////////

deClassNavigationSpace::deClassNavigationSpace(deScriptingDragonScript *ds) :
dsClass("NavigationSpace", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED){
	if(! ds) DSTHROW(dueInvalidParam);
	
	pDS = ds;
	
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sNavSpaceNatDat));
}

deClassNavigationSpace::~deClassNavigationSpace(){
}



// Management
///////////////

void deClassNavigationSpace::CreateClassMembers(dsEngine *engine){
	pClsNavigationSpaceType = engine->GetClass("Dragengine.Scenery.NavigationSpaceType");
	
	sInitData init;
	init.clsNavSpace = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsVec = pDS->GetClassVector();
	init.clsDVec = pDS->GetClassDVector();
	init.clsQuat = pDS->GetClassQuaternion();
	init.clsShaList = pDS->GetClassShapeList();
	init.clsNavigationSpaceType = pClsNavigationSpaceType;
	init.clsWorld = pDS->GetClassWorld();
	
	AddFunction(new nfNew(init));
	AddFunction(new nfNewCopy(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetPosition(init));
	AddFunction(new nfSetPosition(init));
	AddFunction(new nfGetOrientation(init));
	AddFunction(new nfSetOrientation(init));
	AddFunction(new nfGetLayer(init));
	AddFunction(new nfSetLayer(init));
	AddFunction(new nfGetType(init));
	AddFunction(new nfSetType(init));
	AddFunction(new nfGetBlockingPriority(init));
	AddFunction(new nfSetBlockingPriority(init));
	AddFunction(new nfGetParentWorld(init));
	
	AddFunction(new nfGetVertexCount(init));
	AddFunction(new nfSetVertexCount(init));
	AddFunction(new nfGetVertexAt(init));
	AddFunction(new nfSetVertexAt(init));
	
	AddFunction(new nfGetEdgeCount(init));
	AddFunction(new nfSetEdgeCount(init));
	AddFunction(new nfGetEdgeVertex1(init));
	AddFunction(new nfGetEdgeVertex2(init));
	AddFunction(new nfGetEdgeType1(init));
	AddFunction(new nfGetEdgeType2(init));
	AddFunction(new nfSetEdgeAt(init));
	
	AddFunction(new nfGetCornerCount(init));
	AddFunction(new nfSetCornerCount(init));
	AddFunction(new nfGetCornerVertex(init));
	AddFunction(new nfGetCornerType(init));
	AddFunction(new nfSetCornerAt(init));
	
	AddFunction(new nfGetFaceCount(init));
	AddFunction(new nfSetFaceCount(init));
	AddFunction(new nfGetFaceCornerCount(init));
	AddFunction(new nfGetFaceType(init));
	AddFunction(new nfSetFaceAt(init));
	
	AddFunction(new nfGetWallCount(init));
	AddFunction(new nfSetWallCount(init));
	AddFunction(new nfGetWallFace(init));
	AddFunction(new nfGetWallType(init));
	AddFunction(new nfSetWallAt(init));
	
	AddFunction(new nfGetRoomCount(init));
	AddFunction(new nfSetRoomCount(init));
	AddFunction(new nfGetRoomFrontWallCount(init));
	AddFunction(new nfGetRoomBackWallCount(init));
	AddFunction(new nfGetRoomType(init));
	AddFunction(new nfSetRoomAt(init));
	
	AddFunction(new nfGetSnapDistance(init));
	AddFunction(new nfSetSnapDistance(init));
	AddFunction(new nfGetSnapAngle(init));
	AddFunction(new nfSetSnapAngle(init));
	
	AddFunction(new nfLayoutChanged(init));
	
	AddFunction(new nfGetBlockerShapeList(init));
	AddFunction(new nfSetBlockerShapeList(init));
	
	AddFunction(new nfHashCode(init));
	AddFunction(new nfEquals(init));
	
	CalcMemberOffsets();
}

deNavigationSpace *deClassNavigationSpace::GetNavigationSpace(dsRealObject *myself) const{
	if(! myself){
		return NULL;
	}
	
	return ((const sNavSpaceNatDat *)p_GetNativeData(myself->GetBuffer()))->navspace;
}

void deClassNavigationSpace::PushNavigationSpace(dsRunTime *rt, deNavigationSpace *navspace){
	if(! rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(! navspace){
		rt->PushObject(NULL, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	((sNavSpaceNatDat*)p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()))->navspace = navspace;
	navspace->AddReference();
}
