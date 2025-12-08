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

#include "deClassNavigator.h"
#include "deClassNavigatorPath.h"
#include "deClassNavigationInfo.h"
#include "../math/deClassVector.h"
#include "../math/deClassDVector.h"
#include "../math/deClassQuaternion.h"
#include "../collider/deClassCollider.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"
#include "../../utils/dedsNavigationInfo.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/navigation/navigator/deNavigator.h>
#include <dragengine/resources/navigation/navigator/deNavigatorManager.h>
#include <dragengine/resources/navigation/navigator/deNavigatorPath.h>
#include <dragengine/resources/navigation/navigator/deNavigatorType.h>
#include <dragengine/resources/collider/deCollider.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



struct sNavNatDat{
	deNavigator *navigator;
};



// Constructors, Destructors
//////////////////////////////

// public func new()
deClassNavigator::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsNavigator,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassNavigator::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sNavNatDat &nd = *((sNavNatDat*)p_GetNativeData(myself));
	const deClassNavigator &clsNavigator = *((deClassNavigator*)GetOwnerClass());
	deNavigatorManager &navmgr = *clsNavigator.GetDS()->GetGameEngine()->GetNavigatorManager();
	
	// clear ( important )
	nd.navigator = NULL;
	
	// create navigator
	nd.navigator = navmgr.CreateNavigator();
}

// public func destructor()
deClassNavigator::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsNavigator,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassNavigator::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sNavNatDat &nd = *((sNavNatDat*)p_GetNativeData(myself));
	
	if(nd.navigator){
		nd.navigator->FreeReference();
		nd.navigator = NULL;
	}
}



// Management
///////////////

// public func int getLayer()
deClassNavigator::nfGetLayer::nfGetLayer(const sInitData &init) : dsFunction(init.clsNavigator,
"getLayer", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}
void deClassNavigator::nfGetLayer::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNavigator &navigator = *(((const sNavNatDat *)p_GetNativeData(myself))->navigator);
	
	rt->PushInt(navigator.GetLayer());
}

// public func void setLayer( int layer )
deClassNavigator::nfSetLayer::nfSetLayer(const sInitData &init) : dsFunction(init.clsNavigator,
"setLayer", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // layer
}
void deClassNavigator::nfSetLayer::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigator &navigator = *(((const sNavNatDat *)p_GetNativeData(myself))->navigator);
	
	navigator.SetLayer(rt->GetValue(0)->GetInt());
}

// public func NavigationSpaceType getSpaceType()
deClassNavigator::nfGetSpaceType::nfGetSpaceType(const sInitData &init) : dsFunction(init.clsNavigator,
"getSpaceType", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsNavigationSpaceType){
}
void deClassNavigator::nfGetSpaceType::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNavigator &navigator = *(((const sNavNatDat *)p_GetNativeData(myself))->navigator);
	
	rt->PushValue(((deClassNavigator*)GetOwnerClass())->GetClassNavigationSpaceType()
		->GetVariable(navigator.GetSpaceType())->GetStaticValue());
}

// public func void setSpaceType( NavigationSpaceType spaceType )
deClassNavigator::nfSetSpaceType::nfSetSpaceType(const sInitData &init) : dsFunction(init.clsNavigator,
"setSpaceType", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsNavigationSpaceType); // spaceType
}
void deClassNavigator::nfSetSpaceType::RunFunction(dsRunTime *rt, dsValue *myself){
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	deNavigator &navigator = *(((const sNavNatDat *)p_GetNativeData(myself))->navigator);
	
	navigator.SetSpaceType((deNavigationSpace::eSpaceTypes)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
}

// public func float getMaxOutsideDistance()
deClassNavigator::nfGetMaxOutsideDistance::nfGetMaxOutsideDistance(const sInitData &init) : dsFunction(init.clsNavigator,
"getMaxOutsideDistance", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassNavigator::nfGetMaxOutsideDistance::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNavigator &navigator = *(((const sNavNatDat *)p_GetNativeData(myself))->navigator);
	
	rt->PushFloat(navigator.GetMaxOutsideDistance());
}

// public func void setMaxOutsideDistance( float maxDistance )
deClassNavigator::nfSetMaxOutsideDistance::nfSetMaxOutsideDistance(const sInitData &init) : dsFunction(init.clsNavigator,
"setMaxOutsideDistance", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // maxDistance
}
void deClassNavigator::nfSetMaxOutsideDistance::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigator &navigator = *(((const sNavNatDat *)p_GetNativeData(myself))->navigator);
	
	navigator.SetMaxOutsideDistance(rt->GetValue(0)->GetFloat());
}



// public func float getDefaultFixCost()
deClassNavigator::nfGetDefaultFixCost::nfGetDefaultFixCost(const sInitData &init) : dsFunction(init.clsNavigator,
"getDefaultFixCost", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassNavigator::nfGetDefaultFixCost::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNavigator &navigator = *(((const sNavNatDat *)p_GetNativeData(myself))->navigator);
	
	rt->PushFloat(navigator.GetDefaultFixCost());
}

// public func void setDefaultFixCost( float cost )
deClassNavigator::nfSetDefaultFixCost::nfSetDefaultFixCost(const sInitData &init) : dsFunction(init.clsNavigator,
"setDefaultFixCost", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // cost
}
void deClassNavigator::nfSetDefaultFixCost::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigator &navigator = *(((const sNavNatDat *)p_GetNativeData(myself))->navigator);
	
	navigator.SetDefaultFixCost(rt->GetValue(0)->GetFloat());
}

// public func float getDefaultCostPerMeter()
deClassNavigator::nfGetDefaultCostPerMeter::nfGetDefaultCostPerMeter(const sInitData &init) : dsFunction(init.clsNavigator,
"getDefaultCostPerMeter", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassNavigator::nfGetDefaultCostPerMeter::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNavigator &navigator = *(((const sNavNatDat *)p_GetNativeData(myself))->navigator);
	
	rt->PushFloat(navigator.GetDefaultCostPerMeter());
}

// public func void setDefaultCostPerMeter( float costPerMeter )
deClassNavigator::nfSetDefaultCostPerMeter::nfSetDefaultCostPerMeter(const sInitData &init) : dsFunction(init.clsNavigator,
"setDefaultCostPerMeter", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // costPerMeter
}
void deClassNavigator::nfSetDefaultCostPerMeter::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigator &navigator = *(((const sNavNatDat *)p_GetNativeData(myself))->navigator);
	
	navigator.SetDefaultCostPerMeter(rt->GetValue(0)->GetFloat());
}

// public func float getBlockingCost()
deClassNavigator::nfGetBlockingCost::nfGetBlockingCost(const sInitData &init) : dsFunction(init.clsNavigator,
"getBlockingCost", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassNavigator::nfGetBlockingCost::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNavigator &navigator = *(((const sNavNatDat *)p_GetNativeData(myself))->navigator);
	
	rt->PushFloat(navigator.GetBlockingCost());
}

// public func void setBlockingCost( float cost )
deClassNavigator::nfSetBlockingCost::nfSetBlockingCost(const sInitData &init) : dsFunction(init.clsNavigator,
"setBlockingCost", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // cost
}
void deClassNavigator::nfSetBlockingCost::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigator &navigator = *(((const sNavNatDat *)p_GetNativeData(myself))->navigator);
	
	navigator.SetBlockingCost(rt->GetValue(0)->GetFloat());
}



// public func float getTypeFixCost( int type )
deClassNavigator::nfGetTypeFixCost::nfGetTypeFixCost(const sInitData &init) : dsFunction(init.clsNavigator,
"getTypeFixCost", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
	p_AddParameter(init.clsInteger); // type
}
void deClassNavigator::nfGetTypeFixCost::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNavigator &navigator = *(((const sNavNatDat *)p_GetNativeData(myself))->navigator);
	const int type = rt->GetValue(0)->GetInt();
	
	const int index = navigator.IndexOfTypeWith(type);
	if(index == -1){
		rt->PushFloat(navigator.GetDefaultFixCost());
		
	}else{
		rt->PushFloat(navigator.GetTypeAt(index)->GetFixCost());
	}
}

// public func void setTypeFixCost( int type, float fixCost )
deClassNavigator::nfSetTypeFixCost::nfSetTypeFixCost(const sInitData &init) : dsFunction(init.clsNavigator,
"setTypeFixCost", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // type
	p_AddParameter(init.clsFloat); // fixCost
}
void deClassNavigator::nfSetTypeFixCost::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigator &navigator = *(((const sNavNatDat *)p_GetNativeData(myself))->navigator);
	const int type = rt->GetValue(0)->GetInt();
	const float fixCost = rt->GetValue(1)->GetFloat();
	
	int index = navigator.IndexOfTypeWith(type);
	if(index == -1){
		index = navigator.GetTypeCount();
		navigator.AddType(type);
	}
	
	navigator.GetTypeAt(index)->SetFixCost(fixCost);
	navigator.NotifyTypesChanged();
}

// public func float getTypeCostPerMeter( int type )
deClassNavigator::nfGetTypeCostPerMeter::nfGetTypeCostPerMeter(const sInitData &init) : dsFunction(init.clsNavigator,
"getTypeCostPerMeter", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
	p_AddParameter(init.clsInteger); // type
}
void deClassNavigator::nfGetTypeCostPerMeter::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNavigator &navigator = *(((const sNavNatDat *)p_GetNativeData(myself))->navigator);
	const int type = rt->GetValue(0)->GetInt();
	
	const int index = navigator.IndexOfTypeWith(type);
	if(index == -1){
		rt->PushFloat(navigator.GetDefaultCostPerMeter());
		
	}else{
		rt->PushFloat(navigator.GetTypeAt(index)->GetCostPerMeter());
	}
}

// public func void setTypeCostPerMeter( int type, float costPerMeter )
deClassNavigator::nfSetTypeCostPerMeter::nfSetTypeCostPerMeter(const sInitData &init) : dsFunction(init.clsNavigator,
"setTypeCostPerMeter", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // type
	p_AddParameter(init.clsFloat); // costPerMeter
}
void deClassNavigator::nfSetTypeCostPerMeter::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigator &navigator = *(((const sNavNatDat *)p_GetNativeData(myself))->navigator);
	const int type = rt->GetValue(0)->GetInt();
	const float costPerMeter = rt->GetValue(1)->GetFloat();
	
	int index = navigator.IndexOfTypeWith(type);
	if(index == -1){
		index = navigator.GetTypeCount();
		navigator.AddType(type);
	}
	
	navigator.GetTypeAt(index)->SetCostPerMeter(costPerMeter);
	navigator.NotifyTypesChanged();
}

// public func void removeType( int type )
deClassNavigator::nfRemoveType::nfRemoveType(const sInitData &init) : dsFunction(init.clsNavigator,
"removeType", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // type
}
void deClassNavigator::nfRemoveType::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigator &navigator = *(((const sNavNatDat *)p_GetNativeData(myself))->navigator);
	const int type = rt->GetValue(0)->GetInt();
	
	if(navigator.HasTypeWith(type)){
		navigator.RemoveTypeWith(type);
		navigator.NotifyTypesChanged();
	}
}

// public func void removeAllTypes()
deClassNavigator::nfRemoveAllTypes::nfRemoveAllTypes(const sInitData &init) : dsFunction(init.clsNavigator,
"removeAllTypes", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassNavigator::nfRemoveAllTypes::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigator &navigator = *(((const sNavNatDat *)p_GetNativeData(myself))->navigator);
	
	if(navigator.GetTypeCount() > 0){
		navigator.RemoveAllTypes();
		navigator.NotifyTypesChanged();
	}
}



// public func void findPath( NavigatorPath path, DVector start, DVector goal )
deClassNavigator::nfFindPath::nfFindPath(const sInitData &init) : dsFunction(init.clsNavigator,
"findPath", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsNavPath); // path
	p_AddParameter(init.clsDVector); // start
	p_AddParameter(init.clsDVector); // goal
}
void deClassNavigator::nfFindPath::RunFunction(dsRunTime *rt, dsValue *myself){
	deNavigator &navigator = *(((const sNavNatDat *)p_GetNativeData(myself))->navigator);
	deScriptingDragonScript &ds = *(((deClassNavigator*)GetOwnerClass())->GetDS());
	
	deNavigatorPath &path = ds.GetClassNavigatorPath()->GetNavigatorPath(rt->GetValue(0)->GetRealObject());
	const decDVector &start = ds.GetClassDVector()->GetDVector(rt->GetValue(1)->GetRealObject());
	const decDVector &goal = ds.GetClassDVector()->GetDVector(rt->GetValue(2)->GetRealObject());
	
	navigator.FindPath(path, start, goal);
}

// public func NavigationInfo nearestPoint( DVector point, float radius )
deClassNavigator::nfNearestPoint::nfNearestPoint(const sInitData &init) : dsFunction(init.clsNavigator,
"nearestPoint", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsNavInfo){
	p_AddParameter(init.clsDVector); // point
	p_AddParameter(init.clsFloat); // radius
}
void deClassNavigator::nfNearestPoint::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNavigator &navigator = *(((const sNavNatDat *)p_GetNativeData(myself))->navigator);
	deScriptingDragonScript &ds = *(((deClassNavigator*)GetOwnerClass())->GetDS());
	
	const decDVector &point = ds.GetClassDVector()->GetDVector(rt->GetValue(0)->GetRealObject());
	const float radius = rt->GetValue(1)->GetFloat();
	
	decDVector nearestPoint;
	int nearestType;
	
	if(!navigator.NearestPoint(point, radius, nearestPoint, nearestType)){
		ds.GetClassNavigationInfo()->PushNavigationInfo(rt, NULL);
		return;
	}
	
	dedsNavigationInfo *navinfo = NULL;
	
	try{
		navinfo = new dedsNavigationInfo(ds);
		navinfo->SetPosition(nearestPoint);
		navinfo->SetCostType(nearestType);
		
		ds.GetClassNavigationInfo()->PushNavigationInfo(rt, navinfo);
		navinfo->FreeReference();
		
	}catch(const duException &){
		if(navinfo){
			navinfo->FreeReference();
		}
		throw;
	}
}

// public func NavigationInfo lineCollide( DVector origin, Vector direction )
deClassNavigator::nfLineCollide::nfLineCollide(const sInitData &init) :
dsFunction(init.clsNavigator, "lineCollide", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsNavInfo){
	p_AddParameter(init.clsDVector); // origin
	p_AddParameter(init.clsVector); // direction
}
void deClassNavigator::nfLineCollide::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNavigator &navigator = *(((const sNavNatDat *)p_GetNativeData(myself))->navigator);
	deScriptingDragonScript &ds = *(((deClassNavigator*)GetOwnerClass())->GetDS());
	
	const decDVector &origin = ds.GetClassDVector()->GetDVector(rt->GetValue(0)->GetRealObject());
	const decVector &direction = ds.GetClassVector()->GetVector(rt->GetValue(1)->GetRealObject());
	
	float distance;
	
	if(!navigator.LineCollide(origin, direction, distance)){
		ds.GetClassNavigationInfo()->PushNavigationInfo(rt, NULL);
		return;
	}
	
	dedsNavigationInfo *navinfo = NULL;
	
	try{
		navinfo = new dedsNavigationInfo(ds);
		navinfo->SetPosition(origin + decDVector(direction * distance));
		navinfo->SetPathFactor(distance);
		
		ds.GetClassNavigationInfo()->PushNavigationInfo(rt, navinfo);
		navinfo->FreeReference();
		
	}catch(const duException &){
		if(navinfo){
			navinfo->FreeReference();
		}
		throw;
	}
}




// public NavigationInfo pathCollideRay( NavigatorPath path, Collider collider )
deClassNavigator::nfPathCollideRay::nfPathCollideRay(const sInitData &init) : dsFunction(init.clsNavigator,
"pathCollideRay", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsNavInfo){
	p_AddParameter(init.clsNavPath); // path
	p_AddParameter(init.clsCollider); // collider
}
void deClassNavigator::nfPathCollideRay::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNavigator &navigator = *(((const sNavNatDat *)p_GetNativeData(myself))->navigator);
	deScriptingDragonScript &ds = *(((deClassNavigator*)GetOwnerClass())->GetDS());
	
	const deNavigatorPath &path = ds.GetClassNavigatorPath()->GetNavigatorPath(rt->GetValue(0)->GetRealObject());
	deCollider * const collider = ds.GetClassCollider()->GetCollider(rt->GetValue(1)->GetRealObject());
	if(!collider){
		DSTHROW(dueNullPointer);
	}
	
	int hitAfterPoint;
	float hitDistance;
	if(navigator.PathCollideRay(path, *collider, hitAfterPoint, hitDistance)){
		dedsNavigationInfo *navinfo = NULL;
		
		try{
			navinfo = new dedsNavigationInfo(ds);
			navinfo->SetPathIndex(hitAfterPoint);
			navinfo->SetPathFactor(hitDistance);
			
			ds.GetClassNavigationInfo()->PushNavigationInfo(rt, navinfo);
			navinfo->FreeReference();
			
		}catch(const duException &){
			if(navinfo){
				navinfo->FreeReference();
			}
			throw;
		}
		
	}else{
		ds.GetClassNavigationInfo()->PushNavigationInfo(rt, NULL);
	}
}

// public NavigationInfo pathCollideRay( NavigatorPath path, Collider collider,
// DVector startPosition, int nextPoint, float maxDistance )
deClassNavigator::nfPathCollideRay2::nfPathCollideRay2(const sInitData &init) : dsFunction(init.clsNavigator,
"pathCollideRay", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsNavInfo){
	p_AddParameter(init.clsNavPath); // path
	p_AddParameter(init.clsCollider); // collider
	p_AddParameter(init.clsDVector); // startPosition
	p_AddParameter(init.clsInteger); // nextPoint
	p_AddParameter(init.clsFloat); // maxDistance
}
void deClassNavigator::nfPathCollideRay2::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNavigator &navigator = *(((const sNavNatDat *)p_GetNativeData(myself))->navigator);
	deScriptingDragonScript &ds = *(((deClassNavigator*)GetOwnerClass())->GetDS());
	
	const deNavigatorPath &path = ds.GetClassNavigatorPath()->GetNavigatorPath(rt->GetValue(0)->GetRealObject());
	deCollider * const collider = ds.GetClassCollider()->GetCollider(rt->GetValue(1)->GetRealObject());
	const decDVector &startPosition = ds.GetClassDVector()->GetDVector(rt->GetValue(2)->GetRealObject());
	const int nextPoint = rt->GetValue(3)->GetInt();
	const float maxDistance = rt->GetValue(4)->GetFloat();
	if(!collider){
		DSTHROW(dueNullPointer);
	}
	
	int hitAfterPoint;
	float hitDistance;
	if(navigator.PathCollideRay(path, *collider, startPosition, nextPoint, maxDistance, hitAfterPoint, hitDistance)){
		dedsNavigationInfo *navinfo = NULL;
		
		try{
			navinfo = new dedsNavigationInfo(ds);
			navinfo->SetPathIndex(hitAfterPoint);
			navinfo->SetPathFactor(hitDistance);
			
			ds.GetClassNavigationInfo()->PushNavigationInfo(rt, navinfo);
			navinfo->FreeReference();
			
		}catch(const duException &){
			if(navinfo){
				navinfo->FreeReference();
			}
			throw;
		}
		
	}else{
		ds.GetClassNavigationInfo()->PushNavigationInfo(rt, NULL);
	}
}

// public NavigationInfo pathCollideShape( NavigatorPath path, Collider collider, Collider agent )
deClassNavigator::nfPathCollideShape::nfPathCollideShape(const sInitData &init) : dsFunction(init.clsNavigator,
"pathCollideShape", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsNavInfo){
	p_AddParameter(init.clsNavPath); // path
	p_AddParameter(init.clsCollider); // collider
	p_AddParameter(init.clsCollider); // agent
}
void deClassNavigator::nfPathCollideShape::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNavigator &navigator = *(((const sNavNatDat *)p_GetNativeData(myself))->navigator);
	deScriptingDragonScript &ds = *(((deClassNavigator*)GetOwnerClass())->GetDS());
	
	const deNavigatorPath &path = ds.GetClassNavigatorPath()->GetNavigatorPath(rt->GetValue(0)->GetRealObject());
	deCollider * const collider = ds.GetClassCollider()->GetCollider(rt->GetValue(1)->GetRealObject());
	deCollider * const agent = ds.GetClassCollider()->GetCollider(rt->GetValue(2)->GetRealObject());
	if(!collider || !agent){
		DSTHROW(dueNullPointer);
	}
	
	int hitAfterPoint;
	float hitDistance;
	if(navigator.PathCollideShape(path, *collider,  *agent, hitAfterPoint, hitDistance)){
		dedsNavigationInfo *navinfo = NULL;
		
		try{
			navinfo = new dedsNavigationInfo(ds);
			navinfo->SetPathIndex(hitAfterPoint);
			navinfo->SetPathFactor(hitDistance);
			
			ds.GetClassNavigationInfo()->PushNavigationInfo(rt, navinfo);
			navinfo->FreeReference();
			
		}catch(const duException &){
			if(navinfo){
				navinfo->FreeReference();
			}
			throw;
		}
		
	}else{
		ds.GetClassNavigationInfo()->PushNavigationInfo(rt, NULL);
	}
}

// public NavigationInfo pathCollideShape( NavigatorPath path, Collider collider,
// Collider agent, DVector startPosition, int nextPoint, float maxDistance )
deClassNavigator::nfPathCollideShape2::nfPathCollideShape2(const sInitData &init) : dsFunction(init.clsNavigator,
"pathCollideShape", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsNavInfo){
	p_AddParameter(init.clsNavPath); // path
	p_AddParameter(init.clsCollider); // collider
	p_AddParameter(init.clsCollider); // agent
	p_AddParameter(init.clsDVector); // startPosition
	p_AddParameter(init.clsInteger); // nextPoint
	p_AddParameter(init.clsFloat); // maxDistance
}
void deClassNavigator::nfPathCollideShape2::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNavigator &navigator = *(((const sNavNatDat *)p_GetNativeData(myself))->navigator);
	deScriptingDragonScript &ds = *(((deClassNavigator*)GetOwnerClass())->GetDS());
	
	const deNavigatorPath &path = ds.GetClassNavigatorPath()->GetNavigatorPath(rt->GetValue(0)->GetRealObject());
	deCollider * const collider = ds.GetClassCollider()->GetCollider(rt->GetValue(1)->GetRealObject());
	deCollider * const agent = ds.GetClassCollider()->GetCollider(rt->GetValue(2)->GetRealObject());
	const decDVector &startPosition = ds.GetClassDVector()->GetDVector(rt->GetValue(3)->GetRealObject());
	const int nextPoint = rt->GetValue(4)->GetInt();
	const float maxDistance = rt->GetValue(5)->GetFloat();
	if(!collider || !agent){
		DSTHROW(dueNullPointer);
	}
	
	int hitAfterPoint;
	float hitDistance;
	if(navigator.PathCollideShape(path, *collider, *agent, startPosition,
	nextPoint, maxDistance, hitAfterPoint, hitDistance)){
		dedsNavigationInfo *navinfo = NULL;
		
		try{
			navinfo = new dedsNavigationInfo(ds);
			navinfo->SetPathIndex(hitAfterPoint);
			navinfo->SetPathFactor(hitDistance);
			
			ds.GetClassNavigationInfo()->PushNavigationInfo(rt, navinfo);
			navinfo->FreeReference();
			
		}catch(const duException &){
			if(navinfo){
				navinfo->FreeReference();
			}
			throw;
		}
		
	}else{
		ds.GetClassNavigationInfo()->PushNavigationInfo(rt, NULL);
	}
}



// public func int hashCode()
deClassNavigator::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsNavigator, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}

void deClassNavigator::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNavigator * const navigator = ((const sNavNatDat *)p_GetNativeData(myself))->navigator;
	
	rt->PushInt((int)(intptr_t)navigator);
}

// public func bool equals( Object object )
deClassNavigator::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsNavigator, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObject); // object
}
void deClassNavigator::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNavigator * const navigator = ((const sNavNatDat *)p_GetNativeData(myself))->navigator;
	deClassNavigator * const clsNavigator = (deClassNavigator*)GetOwnerClass();
	dsValue * const object = rt->GetValue(0);
	
	if(!p_IsObjOfType(object, clsNavigator)){
		rt->PushBool(false);
		
	}else{
		const deNavigator * const otherNavigator = ((const sNavNatDat *)p_GetNativeData(object))->navigator;
		rt->PushBool(navigator == otherNavigator);
	}
}



// Class deClassNavigator
///////////////////////////

// Constructor
////////////////

deClassNavigator::deClassNavigator(deScriptingDragonScript *ds) :
dsClass("Navigator", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED){
	if(!ds){
		DSTHROW(dueInvalidParam);
	}
	
	pDS = ds;
	
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sNavNatDat));
}

deClassNavigator::~deClassNavigator(){
}



// Management
///////////////

void deClassNavigator::CreateClassMembers(dsEngine *engine){
	pClsNavigationSpaceType = engine->GetClass("Dragengine.Scenery.NavigationSpaceType");
	
	sInitData init;
	init.clsNavigator = this;
	init.clsBool = engine->GetClassBool();
	init.clsFloat = engine->GetClassFloat();
	init.clsInteger = engine->GetClassInt();
	init.clsObject = engine->GetClassObject();
	init.clsString = engine->GetClassString();
	init.clsVoid = engine->GetClassVoid();
	init.clsCollider = pDS->GetClassCollider();
	init.clsDVector = pDS->GetClassDVector();
	init.clsNavInfo = pDS->GetClassNavigationInfo();
	init.clsQuaternion = pDS->GetClassQuaternion();
	init.clsVector = pDS->GetClassVector();
	init.clsNavPath = pDS->GetClassNavigatorPath();
	init.clsNavigationSpaceType = pClsNavigationSpaceType;
	
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetLayer(init));
	AddFunction(new nfSetLayer(init));
	AddFunction(new nfGetSpaceType(init));
	AddFunction(new nfSetSpaceType(init));
	AddFunction(new nfGetMaxOutsideDistance(init));
	AddFunction(new nfSetMaxOutsideDistance(init));
	
	AddFunction(new nfGetDefaultFixCost(init));
	AddFunction(new nfSetDefaultFixCost(init));
	AddFunction(new nfGetDefaultCostPerMeter(init));
	AddFunction(new nfSetDefaultCostPerMeter(init));
	AddFunction(new nfGetBlockingCost(init));
	AddFunction(new nfSetBlockingCost(init));
	
	AddFunction(new nfGetTypeFixCost(init));
	AddFunction(new nfSetTypeFixCost(init));
	AddFunction(new nfGetTypeCostPerMeter(init));
	AddFunction(new nfSetTypeCostPerMeter(init));
	AddFunction(new nfRemoveType(init));
	AddFunction(new nfRemoveAllTypes(init));
	
	AddFunction(new nfFindPath(init));
	
	AddFunction(new nfNearestPoint(init));
	AddFunction(new nfLineCollide(init));
	
	AddFunction(new nfPathCollideRay(init));
	AddFunction(new nfPathCollideRay2(init));
	AddFunction(new nfPathCollideShape(init));
	AddFunction(new nfPathCollideShape2(init));
	
	AddFunction(new nfHashCode(init));
	AddFunction(new nfEquals(init));
	
	CalcMemberOffsets();
}

deNavigator *deClassNavigator::GetNavigator(dsRealObject *myself) const{
	if(!myself){
		return NULL;
	}
	
	return ((const sNavNatDat *)p_GetNativeData(myself->GetBuffer()))->navigator;
}

void deClassNavigator::PushNavigator(dsRunTime *rt, deNavigator *navigator){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!navigator){
		rt->PushObject(NULL, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	((sNavNatDat*)p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()))->navigator = navigator;
	navigator->AddReference();
}
