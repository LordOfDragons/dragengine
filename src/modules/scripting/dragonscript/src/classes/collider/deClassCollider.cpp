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
#include <string.h>
#include <libdscript/libdscript.h>

#include "deClassColliderBreakingListener.h"
#include "deClassColliderConstraint.h"
#include "deClassCollider.h"
#include "deClassColliderListener.h"
#include "deClassColliderCollisionTest.h"
#include "deClassColliderVolume.h"
#include "deClassColliderRig.h"
#include "deClassColliderComponent.h"
#include "../graphics/deClassBillboard.h"
#include "../graphics/deClassComponent.h"
#include "../graphics/deClassCamera.h"
#include "../graphics/deClassEnvMapProbe.h"
#include "../graphics/deClassLight.h"
#include "../graphics/deClassLumimeter.h"
#include "../math/deClassDMatrix.h"
#include "../math/deClassDVector.h"
#include "../math/deClassQuaternion.h"
#include "../math/deClassVector.h"
#include "../physics/deClassCollisionFilter.h"
#include "../physics/deClassCollisionInfo.h"
#include "../physics/deClassForceField.h"
#include "../physics/deClassTouchSensor.h"
#include "../utils/deClassShapeList.h"
#include "../world/deClassRig.h"
#include "../world/deClassWorld.h"
#include "../debug/deClassDebugDrawer.h"
#include "../sound/deClassMicrophone.h"
#include "../sound/deClassSpeaker.h"
#include "../sound/deClassSoundLevelMeter.h"
#include "../ai/deClassNavigationSpace.h"
#include "../ai/deClassNavigationBlocker.h"
#include "../ai/deClassNavigator.h"
#include "../particle/deClassParticleEmitterInstance.h"
#include "../propfield/deClassPropField.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"
#include "../../peers/dedsCollider.h"
#include "../../utils/dedsColliderListenerAdaptor.h"
#include "../../utils/dedsColliderListenerClosest.h"

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/common/shape/decShapeList.h>
#include <dragengine/resources/billboard/deBillboard.h>
#include <dragengine/resources/collider/deColliderAttachment.h>
#include <dragengine/resources/collider/deColliderBone.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deColliderConstraint.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/collider/deColliderRig.h>
#include <dragengine/resources/collider/deColliderVisitorIdentify.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderCollisionTest.h>
#include <dragengine/resources/component/deComponentBone.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/model/deModelBone.h>
#include <dragengine/resources/model/deModelFace.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelLOD.h>
#include <dragengine/resources/model/deModelVertex.h>
#include <dragengine/resources/model/deModelWeight.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/resources/camera/deCamera.h>
#include <dragengine/resources/forcefield/deForceField.h>
#include <dragengine/resources/light/deLight.h>
#include <dragengine/resources/probe/deEnvMapProbe.h>
#include <dragengine/resources/sensor/deLumimeter.h>
#include <dragengine/resources/sensor/deTouchSensor.h>
#include <dragengine/resources/sensor/deSoundLevelMeter.h>
#include <dragengine/resources/sound/deMicrophone.h>
#include <dragengine/resources/sound/deSpeaker.h>
#include <dragengine/resources/navigation/blocker/deNavigationBlocker.h>
#include <dragengine/resources/navigation/space/deNavigationSpace.h>
#include <dragengine/resources/navigation/navigator/deNavigator.h>
#include <dragengine/resources/particle/deParticleEmitterInstance.h>
#include <dragengine/resources/propfield/dePropField.h>



// native structure
struct sColNatDat{
	deCollider::Ref collider;
};



// native functions
/////////////////////

// public func new()
deClassCollider::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsCol,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
}
void deClassCollider::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sColNatDat * const nd = new (p_GetNativeData(myself)) sColNatDat;
}

// public func destructor()
deClassCollider::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsCol,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCollider::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sColNatDat*>(p_GetNativeData(myself))->~sColNatDat();
}



// management
///////////////

// public func DVector getPosition()
deClassCollider::nfGetPosition::nfGetPosition(const sInitData &init) : dsFunction(init.clsCol,
"getPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
}
void deClassCollider::nfGetPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCollider*)GetOwnerClass())->GetDS();
	
	ds.GetClassDVector()->PushDVector(rt, nd.collider->GetPosition());
}

// public func setPosition( DVector position )
deClassCollider::nfSetPosition::nfSetPosition(const sInitData &init) : dsFunction(init.clsCol,
"setPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsDVec); // position
}
void deClassCollider::nfSetPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCollider*)GetOwnerClass())->GetDS();
	const decDVector &position = ds.GetClassDVector()->GetDVector(rt->GetValue(0)->GetRealObject());
	nd.collider->SetPosition(position);
}

// public func Quaternion getOrientation()
deClassCollider::nfGetOrientation::nfGetOrientation(const sInitData &init) : dsFunction(init.clsCol,
"getOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat){
}
void deClassCollider::nfGetOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCollider*)GetOwnerClass())->GetDS();
	ds.GetClassQuaternion()->PushQuaternion(rt, nd.collider->GetOrientation());
}

// public func void setOrientation( Quaternion orientation )
deClassCollider::nfSetOrientation::nfSetOrientation(const sInitData &init) : dsFunction(init.clsCol,
"setOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsQuat); // orientation
}
void deClassCollider::nfSetOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCollider*)GetOwnerClass())->GetDS();
	const decQuaternion &orientation = ds.GetClassQuaternion()->GetQuaternion(rt->GetValue(0)->GetRealObject());
	nd.collider->SetOrientation(orientation);
}

// public func setGeometry( DVector position, Quaternion orientation )
deClassCollider::nfSetGeometry::nfSetGeometry(const sInitData &init) : dsFunction(init.clsCol,
"setGeometry", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsDVec); // position
	p_AddParameter(init.clsQuat); // orientation
}
void deClassCollider::nfSetGeometry::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCollider*)GetOwnerClass())->GetDS();
	
	const decDVector &position = ds.GetClassDVector()->GetDVector(
		rt->GetValue(0)->GetRealObject());
	const decQuaternion &orientation = ds.GetClassQuaternion()->GetQuaternion(
		rt->GetValue(1)->GetRealObject());
	
	nd.collider->SetGeometry(position, orientation);
}

// public func setGeometry( DVector position, Quaternion orientation, Scale scale )
deClassCollider::nfSetGeometry2::nfSetGeometry2(const sInitData &init) : dsFunction(init.clsCol,
"setGeometry", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsDVec); // position
	p_AddParameter(init.clsQuat); // orientation
	p_AddParameter(init.clsVec); // scale
}
void deClassCollider::nfSetGeometry2::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCollider*)GetOwnerClass())->GetDS();
	
	const decDVector &position = ds.GetClassDVector()->GetDVector(
		rt->GetValue(0)->GetRealObject());
	const decQuaternion &orientation = ds.GetClassQuaternion()->GetQuaternion(
		rt->GetValue(1)->GetRealObject());
	const decVector &scale = ds.GetClassVector()->GetVector(
		rt->GetValue(2)->GetRealObject());
	
	nd.collider->SetGeometry(position, orientation, scale);
}

// public func Vector getScale()
deClassCollider::nfGetScale::nfGetScale(const sInitData &init) : dsFunction(init.clsCol,
"getScale", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
}
void deClassCollider::nfGetScale::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCollider*)GetOwnerClass())->GetDS();
	ds.GetClassVector()->PushVector(rt, nd.collider->GetScale());
}

// public func void setScale( Vector scale )
deClassCollider::nfSetScale::nfSetScale(const sInitData &init) : dsFunction(init.clsCol,
"setScale", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // scale
}
void deClassCollider::nfSetScale::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCollider*)GetOwnerClass())->GetDS();
	const decVector &scale = ds.GetClassVector()->GetVector(rt->GetValue(0)->GetRealObject());
	nd.collider->SetScale(scale);
}

// public func Vector getView()
deClassCollider::nfGetView::nfGetView(const sInitData &init) : dsFunction(init.clsCol,
"getView", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
}
void deClassCollider::nfGetView::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCollider*)GetOwnerClass())->GetDS();
	ds.GetClassVector()->PushVector(rt, decMatrix::CreateFromQuaternion(nd.collider->GetOrientation()).TransformView());
}

// public func Vector getUp()
deClassCollider::nfGetUp::nfGetUp(const sInitData &init) : dsFunction(init.clsCol,
"getUp", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
}
void deClassCollider::nfGetUp::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCollider*)GetOwnerClass())->GetDS();
	ds.GetClassVector()->PushVector(rt, decMatrix::CreateFromQuaternion(nd.collider->GetOrientation()).TransformUp());
}

// public func Vector getRight()
deClassCollider::nfGetRight::nfGetRight(const sInitData &init) : dsFunction(init.clsCol,
"getRight", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
}
void deClassCollider::nfGetRight::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCollider*)GetOwnerClass())->GetDS();
	ds.GetClassVector()->PushVector(rt, decMatrix::CreateFromQuaternion(nd.collider->GetOrientation()).TransformRight());
}

// public func DMatrix getMatrix()
deClassCollider::nfGetMatrix::nfGetMatrix(const sInitData &init) : dsFunction(init.clsCol,
"getMatrix", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDMat){
}
void deClassCollider::nfGetMatrix::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCollider*)GetOwnerClass())->GetDS();
	
	ds.GetClassDMatrix()->PushDMatrix(rt, decDMatrix::CreateWorld(nd.collider->GetPosition(),
		nd.collider->GetOrientation(), nd.collider->GetScale()));
}

// public func DMatrix getInverseMatrix()
deClassCollider::nfGetInverseMatrix::nfGetInverseMatrix(const sInitData &init) : dsFunction(init.clsCol,
"getInverseMatrix", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDMat){
}
void deClassCollider::nfGetInverseMatrix::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCollider*)GetOwnerClass())->GetDS();
	
	ds.GetClassDMatrix()->PushDMatrix(rt, decDMatrix::CreateWorld(nd.collider->GetPosition(),
		nd.collider->GetOrientation(), nd.collider->GetScale()).QuickInvert());
}

// public func Vector getLinearVelocity()
deClassCollider::nfGetLinearVelocity::nfGetLinearVelocity(const sInitData &init) : dsFunction(init.clsCol,
"getLinearVelocity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
}
void deClassCollider::nfGetLinearVelocity::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCollider*)GetOwnerClass())->GetDS();
	ds.GetClassVector()->PushVector(rt, nd.collider->GetLinearVelocity());
}

// public func void setLinearVelocity( Vector linVelo )
deClassCollider::nfSetLinearVelocity::nfSetLinearVelocity(const sInitData &init) : dsFunction(init.clsCol,
"setLinearVelocity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // linVelo
}
void deClassCollider::nfSetLinearVelocity::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCollider*)GetOwnerClass())->GetDS();
	const decVector &linearVelocity = ds.GetClassVector()->GetVector(rt->GetValue(0)->GetRealObject());
	nd.collider->SetLinearVelocity(linearVelocity);
}

// public func Vector getAngularVelocity()
deClassCollider::nfGetAngularVelocity::nfGetAngularVelocity(const sInitData &init) : dsFunction(init.clsCol,
"getAngularVelocity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
}
void deClassCollider::nfGetAngularVelocity::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCollider*)GetOwnerClass())->GetDS();
	ds.GetClassVector()->PushVector(rt, nd.collider->GetAngularVelocity() * RAD2DEG);
}

// public func void setAngularVelocity( Vector angVelo )
deClassCollider::nfSetAngularVelocity::nfSetAngularVelocity(const sInitData &init) : dsFunction(init.clsCol,
"setAngularVelocity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // angVelo
}
void deClassCollider::nfSetAngularVelocity::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCollider*)GetOwnerClass())->GetDS();
	const decVector &angularVelocity = ds.GetClassVector()->GetVector(rt->GetValue(0)->GetRealObject());
	nd.collider->SetAngularVelocity(angularVelocity * DEG2RAD);
}

// public func float getMass()
deClassCollider::nfGetMass::nfGetMass(const sInitData &init) : dsFunction(init.clsCol,
"getMass", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassCollider::nfGetMass::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	rt->PushFloat(nd.collider->GetMass());
}

// public func void setMass( float mass )
deClassCollider::nfSetMass::nfSetMass(const sInitData &init) : dsFunction(init.clsCol,
"setMass", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // mass
}
void deClassCollider::nfSetMass::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	nd.collider->SetMass(rt->GetValue(0)->GetFloat());
}

// public func Vector getGravity()
deClassCollider::nfGetGravity::nfGetGravity(const sInitData &init) : dsFunction(init.clsCol,
"getGravity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
}
void deClassCollider::nfGetGravity::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCollider*)GetOwnerClass())->GetDS();
	ds.GetClassVector()->PushVector(rt, nd.collider->GetGravity());
}

// public func void setGravity( Vector gravity )
deClassCollider::nfSetGravity::nfSetGravity(const sInitData &init) : dsFunction(init.clsCol,
"setGravity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // gravity
}
void deClassCollider::nfSetGravity::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCollider*)GetOwnerClass())->GetDS();
	const decVector &gravity = ds.GetClassVector()->GetVector(rt->GetValue(0)->GetRealObject());
	nd.collider->SetGravity(gravity);
}

// public func bool getEnabled()
deClassCollider::nfGetEnabled::nfGetEnabled(const sInitData &init) : dsFunction(init.clsCol,
"getEnabled", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassCollider::nfGetEnabled::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	rt->PushBool(nd.collider->GetEnabled());
}

// public func setEnabled( bool enabled )
deClassCollider::nfSetEnabled::nfSetEnabled(const sInitData &init) : dsFunction(init.clsCol,
"setEnabled", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enabled
}
void deClassCollider::nfSetEnabled::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	nd.collider->SetEnabled(rt->GetValue(0)->GetBool());
}

// public func CollisionResponse getResponseType
deClassCollider::nfGetResponseType::nfGetResponseType(const sInitData &init) :
dsFunction(init.clsCol, "getResponseType", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsCollisionResponse){
}
void deClassCollider::nfGetResponseType::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	rt->PushValue(((deClassCollider*)GetOwnerClass())->GetClassCollisionResponse()
		->GetVariable(nd.collider->GetResponseType())->GetStaticValue());
}

// public func void setResponseType( CollisionResponse responseType )
deClassCollider::nfSetResponseType::nfSetResponseType(const sInitData &init) :
dsFunction(init.clsCol, "setResponseType", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCollisionResponse); // responseType
}
void deClassCollider::nfSetResponseType::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider || !rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	nd.collider->SetResponseType((deCollider::eResponseType)
		static_cast<dsClassEnumeration*>(rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
}

// public func bool getUseLocalGravity()
deClassCollider::nfGetUseLocalGravity::nfGetUseLocalGravity(const sInitData &init) : dsFunction(init.clsCol,
"getUseLocalGravity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassCollider::nfGetUseLocalGravity::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	rt->PushBool(nd.collider->GetUseLocalGravity());
}

// public func void setUseLocalGravity( bool canHit )
deClassCollider::nfSetUseLocalGravity::nfSetUseLocalGravity(const sInitData &init) : dsFunction(init.clsCol,
"setUseLocalGravity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // canHit
}
void deClassCollider::nfSetUseLocalGravity::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	nd.collider->SetUseLocalGravity(rt->GetValue(0)->GetBool());
}

// func World getParentWorld()
deClassCollider::nfGetParentWorld::nfGetParentWorld(const sInitData &init) :
dsFunction(init.clsCol, "getParentWorld", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsWorld) {
}

void deClassCollider::nfGetParentWorld::RunFunction(dsRunTime *rt, dsValue *myself) {
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	((deClassCollider*)GetOwnerClass())->GetDS().GetClassWorld()->
		PushWorld(rt, nd.collider->GetParentWorld());
}


// public func Object getOwner()
deClassCollider::nfGetOwner::nfGetOwner(const sInitData &init) : dsFunction(init.clsCol,
"getOwner", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsObj){
}
void deClassCollider::nfGetOwner::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	dedsCollider * const scrCol = (dedsCollider*)nd.collider->GetPeerScripting();
	
	if(scrCol){
		rt->PushObject(scrCol->GetOwner());
		
	}else{
		rt->PushObject(NULL);
	}
}

// public func void setOwner( Object owner )
deClassCollider::nfSetOwner::nfSetOwner(const sInitData &init) : dsFunction(init.clsCol,
"setOwner", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsObj); // owner
}
void deClassCollider::nfSetOwner::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	dedsCollider * const scrCol = (dedsCollider*)nd.collider->GetPeerScripting();
	if(scrCol){
		scrCol->SetOwner(rt->GetValue(0)->GetRealObject());
	}
}



// public func float getForceFieldDirect()
deClassCollider::nfGetForceFieldDirect::nfGetForceFieldDirect(const sInitData &init) : dsFunction(init.clsCol,
"getForceFieldDirect", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassCollider::nfGetForceFieldDirect::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	rt->PushFloat(nd.collider->GetForceFieldDirect());
}

// public func void setForceFieldDirect( float factor )
deClassCollider::nfSetForceFieldDirect::nfSetForceFieldDirect(const sInitData &init) : dsFunction(init.clsCol,
"setForceFieldDirect", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // factor
}
void deClassCollider::nfSetForceFieldDirect::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	nd.collider->SetForceFieldDirect(rt->GetValue(0)->GetFloat());
}

// public func float getForceFieldSurface()
deClassCollider::nfGetForceFieldSurface::nfGetForceFieldSurface(const sInitData &init) : dsFunction(init.clsCol,
"getForceFieldSurface", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassCollider::nfGetForceFieldSurface::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	rt->PushFloat(nd.collider->GetForceFieldSurface());
}

// public func void setForceFieldSurface( float factor )
deClassCollider::nfSetForceFieldSurface::nfSetForceFieldSurface(const sInitData &init) : dsFunction(init.clsCol,
"setForceFieldSurface", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // factor
}
void deClassCollider::nfSetForceFieldSurface::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	nd.collider->SetForceFieldSurface(rt->GetValue(0)->GetFloat());
}

// public func float getForceFieldMass()
deClassCollider::nfGetForceFieldMass::nfGetForceFieldMass(const sInitData &init) : dsFunction(init.clsCol,
"getForceFieldMass", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassCollider::nfGetForceFieldMass::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	rt->PushFloat(nd.collider->GetForceFieldMass());
}

// public func void setForceFieldMass( float factor )
deClassCollider::nfSetForceFieldMass::nfSetForceFieldMass(const sInitData &init) : dsFunction(init.clsCol,
"setForceFieldMass", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // factor
}
void deClassCollider::nfSetForceFieldMass::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	nd.collider->SetForceFieldMass(rt->GetValue(0)->GetFloat());
}

// public func float getForceFieldSpeed()
deClassCollider::nfGetForceFieldSpeed::nfGetForceFieldSpeed(const sInitData &init) : dsFunction(init.clsCol,
"getForceFieldSpeed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassCollider::nfGetForceFieldSpeed::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	rt->PushFloat(nd.collider->GetForceFieldSpeed());
}

// public func void setForceFieldSpeed( float factor )
deClassCollider::nfSetForceFieldSpeed::nfSetForceFieldSpeed(const sInitData &init) : dsFunction(init.clsCol,
"setForceFieldSpeed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // factor
}
void deClassCollider::nfSetForceFieldSpeed::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	nd.collider->SetForceFieldSpeed(rt->GetValue(0)->GetFloat());
}



// public func CollisionFilter getCollisionFilter()
deClassCollider::nfGetCollisionFilter::nfGetCollisionFilter(const sInitData &init) : dsFunction(init.clsCol,
"getCollisionFilter", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsCF){
}
void deClassCollider::nfGetCollisionFilter::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCollider*)GetOwnerClass())->GetDS();
	ds.GetClassCollisionFilter()->PushCollisionFilter(rt, nd.collider->GetCollisionFilter());
}

// public func void setCollisionFilter( CollisionFilter collisionFilter )
deClassCollider::nfSetCollisionFilter::nfSetCollisionFilter(const sInitData &init) : dsFunction(init.clsCol,
"setCollisionFilter", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCF); // collisionFilter
}
void deClassCollider::nfSetCollisionFilter::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCollider*)GetOwnerClass())->GetDS();
	const decCollisionFilter &collisionFilter = ds.GetClassCollisionFilter()->GetCollisionFilter(rt->GetValue(0)->GetRealObject());
	
	nd.collider->SetCollisionFilter(collisionFilter);
}



// Attachments
////////////////

// public func int getAttachmentCount()
deClassCollider::nfGetAttachmentCount::nfGetAttachmentCount(const sInitData &init) : dsFunction(init.clsCol,
"getAttachmentCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassCollider::nfGetAttachmentCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	rt->PushInt(nd.collider->GetAttachmentCount());
}

// public func Object getAttachedResource( int index )
deClassCollider::nfGetAttachedResource::nfGetAttachedResource(const sInitData &init) : dsFunction(init.clsCol,
"getAttachedResource", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsObj){
	p_AddParameter(init.clsInt); // index
}
void deClassCollider::nfGetAttachedResource::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	deClassCollider &clsCol = *((deClassCollider*)GetOwnerClass());
	const int index = rt->GetValue(0)->GetInt();
	clsCol.PushResource(*rt, nd.collider->GetAttachmentAt(index)->GetResource());
}

// public func void attachStatic( Object resource )
deClassCollider::nfAttachStatic::nfAttachStatic(const sInitData &init) : dsFunction(init.clsCol,
"attachStatic", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsObj); // resource
}
void deClassCollider::nfAttachStatic::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deClassCollider &clsCol = *((deClassCollider*)GetOwnerClass());
	
	deResource * const resource = clsCol.GetResource(*rt->GetValue(0));
	if(!resource){
		DSTHROW(dueNullPointer);
	}
	
	clsCol.AttachStatic(*nd.collider, resource);
}

// public func void attachStatic( Object resource, Vector position, Quaternion orientation )
deClassCollider::nfAttachStatic2::nfAttachStatic2(const sInitData &init) : dsFunction(init.clsCol,
"attachStatic", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsObj); // resource
	p_AddParameter(init.clsVec); // position
	p_AddParameter(init.clsQuat); // orientation
}
void deClassCollider::nfAttachStatic2::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deClassCollider &clsCol = *((deClassCollider*)GetOwnerClass());
	
	deResource * const resource = clsCol.GetResource(*rt->GetValue(0));
	const decVector &position = clsCol.GetDS().GetClassVector()->GetVector(rt->GetValue(1)->GetRealObject());
	const decQuaternion &orientation = clsCol.GetDS().GetClassQuaternion()->GetQuaternion(rt->GetValue(2)->GetRealObject());
	if(!resource){
		DSTHROW(dueNullPointer);
	}
	
	clsCol.AttachStatic(*nd.collider, resource, position, orientation,
		deClassCollider::GetResourceScale(*resource));
}

// public func void attachRelativeMovement( Object resource )
deClassCollider::nfAttachRelativeMovement::nfAttachRelativeMovement(const sInitData &init) : dsFunction(init.clsCol,
"attachRelativeMovement", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsObj); // resource
}
void deClassCollider::nfAttachRelativeMovement::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deClassCollider &clsCol = *((deClassCollider*)GetOwnerClass());
	
	deResource * const resource = clsCol.GetResource(*rt->GetValue(0));
	if(!resource){
		DSTHROW(dueNullPointer);
	}
	
	clsCol.AttachRelativeMovement(*nd.collider, resource);
}

// public func void detach( Object resource )
deClassCollider::nfDetach::nfDetach(const sInitData &init) : dsFunction(init.clsCol,
"detach", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsObj); // resource
}
void deClassCollider::nfDetach::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deClassCollider &clsCol = *((deClassCollider*)GetOwnerClass());
	
	deResource * const resource = clsCol.GetResource(*rt->GetValue(0));
	if(!resource){
		DSTHROW(dueNullPointer);
	}
	
	deColliderAttachment * const attachment = nd.collider->GetAttachmentWith(resource);
	if(attachment){
		nd.collider->RemoveAttachment(attachment);
	}
}

// public func void detachAll()
deClassCollider::nfDetachAll::nfDetachAll(const sInitData &init) : dsFunction(init.clsCol,
"detachAll", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCollider::nfDetachAll::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	nd.collider->RemoveAllAttachments();
}

// public func void attachmentsForceUpdate()
deClassCollider::nfAttachmentsForceUpdate::nfAttachmentsForceUpdate(const sInitData &init) : dsFunction(init.clsCol,
"attachmentsForceUpdate", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCollider::nfAttachmentsForceUpdate::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	nd.collider->AttachmentsForceUpdate();
}



// Constraints
////////////////

// public func int getConstraintCount()
deClassCollider::nfGetConstraintCount::nfGetConstraintCount(const sInitData &init) : dsFunction(init.clsCol,
"getConstraintCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassCollider::nfGetConstraintCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollider &collider = *(static_cast<sColNatDat*>(p_GetNativeData(myself))->collider);
	rt->PushInt(collider.GetConstraintCount());
}

// public func void addConstraint( ColliderConstraint constraint )
deClassCollider::nfAddConstraint::nfAddConstraint(const sInitData &init) : dsFunction(init.clsCol,
"addConstraint", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCCon); // constraint
}
void deClassCollider::nfAddConstraint::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deClassCollider &clsCollider = *((deClassCollider*)GetOwnerClass());
	const deScriptingDragonScript &ds = clsCollider.GetDS();
	
	const deColliderConstraint * const workConstraint = 
		ds.GetClassColliderConstraint()->GetConstraint(rt->GetValue(0)->GetRealObject());
	if(!workConstraint){
		DSTHROW_INFO(dueNullPointer, "constraint");
	}
	if(clsCollider.FindConstraint(*nd.collider, *workConstraint)){
		DSTHROW_INFO(dueInvalidParam, "constraint with same target exists already");
	}
	
	deColliderConstraint *constraint = nullptr;
	try{
		constraint = new deColliderConstraint(*workConstraint);
		nd.collider->AddConstraint(constraint);
		
	}catch(...){
		if(constraint){
			delete constraint;
		}
		throw;
	}
}

// public func bool hasConstraint(ColliderConstraint constraint)
deClassCollider::nfHasConstraint::nfHasConstraint(const sInitData &init) :
dsFunction(init.clsCol, "hasConstraint", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsCCon); // constraint
}
void deClassCollider::nfHasConstraint::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deClassCollider &clsCollider = *((deClassCollider*)GetOwnerClass());
	const deScriptingDragonScript &ds = clsCollider.GetDS();
	
	const deColliderConstraint * const constraint = 
		ds.GetClassColliderConstraint()->GetConstraint(rt->GetValue(0)->GetRealObject());
	rt->PushBool(constraint && clsCollider.FindConstraint(*nd.collider, *constraint));
}

// public func int indexOfConstraint(ColliderConstraint constraint)
deClassCollider::nfIndexOfConstraint::nfIndexOfConstraint(const sInitData &init) :
dsFunction(init.clsCol, "indexOfConstraint", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsCCon); // constraint
}
void deClassCollider::nfIndexOfConstraint::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deClassCollider &clsCollider = *((deClassCollider*)GetOwnerClass());
	const deScriptingDragonScript &ds = clsCollider.GetDS();
	int index = -1;
	
	deColliderConstraint *constraint =
		ds.GetClassColliderConstraint()->GetConstraint(rt->GetValue(0)->GetRealObject());
	if(constraint){
		constraint = clsCollider.FindConstraint(*nd.collider, *constraint);
		if(constraint){
			index = nd.collider->IndexOfConstraint(constraint);
		}
	}
	
	rt->PushInt(index);
}

// public func ColliderConstraint getConstraintAt(int index)
deClassCollider::nfGetConstraintAt::nfGetConstraintAt(const sInitData &init) :
dsFunction(init.clsCol, "getConstraintAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsCCon){
	p_AddParameter(init.clsInt); // index
}
void deClassCollider::nfGetConstraintAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCollider*)GetOwnerClass())->GetDS();
	
	const deColliderConstraint * const workConstraint =
		nd.collider->GetConstraintAt(rt->GetValue(0)->GetInt());
	
	deColliderConstraint *constraint = nullptr;
	try{
		constraint = new deColliderConstraint(*workConstraint);
		ds.GetClassColliderConstraint()->PushConstraint(rt, constraint);
		
	}catch(...){
		if(constraint){
			delete constraint;
		}
		throw;
	}
}

// public func void setConstraintAt( int index, ColliderConstraint constraint )
deClassCollider::nfSetConstraintAt::nfSetConstraintAt(const sInitData &init) : dsFunction(init.clsCol,
"addConstraint", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // index
	p_AddParameter(init.clsCCon); // constraint
}
void deClassCollider::nfSetConstraintAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deClassCollider &clsCollider = *((deClassCollider*)GetOwnerClass());
	const deScriptingDragonScript &ds = clsCollider.GetDS();
	
	const int index = rt->GetValue(0)->GetInt();
	deColliderConstraint &constraint = *nd.collider->GetConstraintAt(index);
	
	const deColliderConstraint * const workConstraint =
		ds.GetClassColliderConstraint()->GetConstraint(rt->GetValue(1)->GetRealObject());
	if(!workConstraint){
		DSTHROW_INFO(dueNullPointer, "constraint");
	}
	
	if(workConstraint->GetTargetCollider() != constraint.GetTargetCollider()
	|| workConstraint->GetTargetBone() != constraint.GetTargetBone()
	|| workConstraint->GetBone() != constraint.GetBone()){
		DSTHROW_INFO(dueInvalidParam, "new constraint target has to match old constraint target");
	}
	
	constraint = *workConstraint;
	nd.collider->NotifyConstraintChanged(index);
}

// public func void removeConstraint(ColliderConstraint constraint)
deClassCollider::nfRemoveConstraint::nfRemoveConstraint(const sInitData &init) :
dsFunction(init.clsCol, "removeConstraint", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCCon); // constraint
}
void deClassCollider::nfRemoveConstraint::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deClassCollider &clsCollider = *((deClassCollider*)GetOwnerClass());
	const deScriptingDragonScript &ds = clsCollider.GetDS();
	
	const deColliderConstraint * const workConstraint =
		ds.GetClassColliderConstraint()->GetConstraint(rt->GetValue(0)->GetRealObject());
	if(!workConstraint){
		DSTHROW_INFO(dueNullPointer, "constraint");
	}
	
	deColliderConstraint * const constraint = clsCollider.FindConstraint(*nd.collider, *workConstraint);
	if(!constraint){
		DSTHROW_INFO(dueOutOfBoundary, "constraint not found");
	}
	
	nd.collider->RemoveConstraint(constraint);
}

// public func void removeConstraintFrom( int index )
deClassCollider::nfRemoveConstraintFrom::nfRemoveConstraintFrom(const sInitData &init) : dsFunction(init.clsCol,
"removeConstraintFrom", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // index
}
void deClassCollider::nfRemoveConstraintFrom::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	nd.collider->RemoveConstraint(nd.collider->GetConstraintAt(rt->GetValue(0)->GetInt()));
}

// public func void removeAllConstraints()
deClassCollider::nfRemoveAllConstraints::nfRemoveAllConstraints(const sInitData &init) : dsFunction(init.clsCol,
"removeAllConstraints", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCollider::nfRemoveAllConstraints::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	nd.collider->RemoveAllConstraints();
}



// Ignore colliders
/////////////////////

// public func int getIgnoreColliderCount()
deClassCollider::nfGetIgnoreColliderCount::nfGetIgnoreColliderCount(const sInitData &init) : dsFunction(init.clsCol,
"getIgnoreColliderCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassCollider::nfGetIgnoreColliderCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	rt->PushInt(nd.collider->GetIgnoreColliderCount());
}

// public func Collider getIgnoreColliderAt( int index )
deClassCollider::nfGetIgnoreColliderAt::nfGetIgnoreColliderAt(const sInitData &init) : dsFunction(init.clsCol,
"getIgnoreColliderAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsCol){
	p_AddParameter(init.clsInt); // index
}
void deClassCollider::nfGetIgnoreColliderAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	deClassCollider &clsCol = *((deClassCollider*)GetOwnerClass());
	
	const int index = rt->GetValue(0)->GetInt();
	clsCol.PushCollider(rt, nd.collider->GetIgnoreColliderAt(index));
}

// public func bool hasIgnoreCollider( Collider collider )
deClassCollider::nfHasIgnoreCollider::nfHasIgnoreCollider(const sInitData &init) : dsFunction(init.clsCol,
"hasIgnoreCollider", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsCol); // collider
}
void deClassCollider::nfHasIgnoreCollider::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deClassCollider &clsCol = *((deClassCollider*)GetOwnerClass());
	
	deCollider * const collider = clsCol.GetCollider(rt->GetValue(0)->GetRealObject());
	rt->PushBool(nd.collider->HasIgnoreCollider(collider));
}

// public func void addIgnoreCollider( Collider collider )
deClassCollider::nfAddIgnoreCollider::nfAddIgnoreCollider(const sInitData &init) : dsFunction(init.clsCol,
"addIgnoreCollider", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCol); // collider
}
void deClassCollider::nfAddIgnoreCollider::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deClassCollider &clsCol = *((deClassCollider*)GetOwnerClass());
	deCollider * const collider = clsCol.GetCollider(rt->GetValue(0)->GetRealObject());
	nd.collider->AddIgnoreCollider(collider);
}

// public func void removeIgnoreCollider( Collider collider )
deClassCollider::nfRemoveIgnoreCollider::nfRemoveIgnoreCollider(const sInitData &init) : dsFunction(init.clsCol,
"removeIgnoreCollider", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCol); // collider
}
void deClassCollider::nfRemoveIgnoreCollider::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deClassCollider &clsCol = *((deClassCollider*)GetOwnerClass());
	deCollider * const collider = clsCol.GetCollider(rt->GetValue(0)->GetRealObject());
	nd.collider->RemoveIgnoreCollider(collider);
}

// public func void removeAllIgnoreColliders()
deClassCollider::nfRemoveAllIgnoreColliders::nfRemoveAllIgnoreColliders(const sInitData &init) : dsFunction(init.clsCol,
"removeAllIgnoreColliders", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCollider::nfRemoveAllIgnoreColliders::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	nd.collider->RemoveAllIgnoreColliders();
}



// Post physics collision tests
/////////////////////////////////

// public func int getCollisionTestCount()
deClassCollider::nfGetCollisionTestCount::nfGetCollisionTestCount(const sInitData &init) :
dsFunction(init.clsCol, "getCollisionTestCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassCollider::nfGetCollisionTestCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	rt->PushInt(nd.collider->GetCollisionTestCount());
}

// public func ColliderCollisionTest getCollisionTestAt( int index )
deClassCollider::nfGetCollisionTestAt::nfGetCollisionTestAt(const sInitData &init) :
dsFunction(init.clsCol, "getCollisionTestAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsCCT){
	p_AddParameter(init.clsInt); // index
}
void deClassCollider::nfGetCollisionTestAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCollider*)GetOwnerClass())->GetDS();
	
	const int index = rt->GetValue(0)->GetInt();
	ds.GetClassColliderCollisionTest()->PushCollisionTest(rt,
		nd.collider->GetCollisionTestAt(index), nd.collider);
}

// public func void addCollisionTest( ColliderCollisionTest collisionTest )
deClassCollider::nfAddCollisionTest::nfAddCollisionTest(const sInitData &init) :
dsFunction(init.clsCol, "addCollisionTest", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCCT); // collisionTest
}
void deClassCollider::nfAddCollisionTest::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCollider*)GetOwnerClass())->GetDS();
	deClassColliderCollisionTest &clsCCT = *ds.GetClassColliderCollisionTest();
	
	dsRealObject * const objCCT = rt->GetValue(0)->GetRealObject();
	nd.collider->AddCollisionTest(clsCCT.GetCollisionTest(objCCT));
	clsCCT.SetParentCollider(objCCT, nd.collider);
}

// public func void removeCollisionTest( ColliderCollisionTest collisionTest )
deClassCollider::nfRemoveCollisionTest::nfRemoveCollisionTest(const sInitData &init) :
dsFunction(init.clsCol, "removeCollisionTest", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCCT); // collisionTest
}
void deClassCollider::nfRemoveCollisionTest::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCollider*)GetOwnerClass())->GetDS();
	const deClassColliderCollisionTest &clsCCT = *ds.GetClassColliderCollisionTest();
	
	nd.collider->RemoveCollisionTest(clsCCT.GetCollisionTest(rt->GetValue(0)->GetRealObject()));
}

// public func void removeAllCollisionTests()
deClassCollider::nfRemoveAllCollisionTests::nfRemoveAllCollisionTests(const sInitData &init) :
dsFunction(init.clsCol, "removeAllCollisionTests", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCollider::nfRemoveAllCollisionTests::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	nd.collider->RemoveAllCollisionTests();
}



// Force and Impuls
/////////////////////

// public func void applyImpuls( Vector impuls )
deClassCollider::nfApplyImpuls::nfApplyImpuls(const sInitData &init) : dsFunction(init.clsCol,
"applyImpuls", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // impuls
}
void deClassCollider::nfApplyImpuls::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCollider*)GetOwnerClass())->GetDS();
	const decVector &impuls = ds.GetClassVector()->GetVector(rt->GetValue(0)->GetRealObject());
	nd.collider->ApplyImpuls(impuls);
}

// public func void applyImpulsAt( Vector impuls, Vector position )
deClassCollider::nfApplyImpulsAt::nfApplyImpulsAt(const sInitData &init) : dsFunction(init.clsCol,
"applyImpulsAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // impuls
	p_AddParameter(init.clsVec); // position
}
void deClassCollider::nfApplyImpulsAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCollider*)GetOwnerClass())->GetDS();
	const decVector &impuls = ds.GetClassVector()->GetVector(rt->GetValue(0)->GetRealObject());
	const decVector &position = ds.GetClassVector()->GetVector(rt->GetValue(1)->GetRealObject());
	
	nd.collider->ApplyImpulsAt(impuls, position);
}

// public func void applyTorqueImpuls( Vector torqueImpuls )
deClassCollider::nfApplyTorqueImpuls::nfApplyTorqueImpuls(const sInitData &init) : dsFunction(init.clsCol,
"applyTorqueImpuls", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // torqueImpuls
}
void deClassCollider::nfApplyTorqueImpuls::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCollider*)GetOwnerClass())->GetDS();
	const decVector &torqueImpuls = ds.GetClassVector()->GetVector(rt->GetValue(0)->GetRealObject());
	nd.collider->ApplyTorqueImpuls(torqueImpuls);
}

// public func void applyForce( Vector force )
deClassCollider::nfApplyForce::nfApplyForce(const sInitData &init) : dsFunction(init.clsCol,
"applyForce", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // force
}
void deClassCollider::nfApplyForce::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCollider*)GetOwnerClass())->GetDS();
	const decVector &force = ds.GetClassVector()->GetVector(rt->GetValue(0)->GetRealObject());
	nd.collider->ApplyForce(force);
}

// public func void applyForceAt( Vector force, Vector position )
deClassCollider::nfApplyForceAt::nfApplyForceAt(const sInitData &init) : dsFunction(init.clsCol,
"applyForceAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // force
	p_AddParameter(init.clsVec); // position
}
void deClassCollider::nfApplyForceAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCollider*)GetOwnerClass())->GetDS();
	const decVector &force = ds.GetClassVector()->GetVector(rt->GetValue(0)->GetRealObject());
	const decVector &position = ds.GetClassVector()->GetVector(rt->GetValue(1)->GetRealObject());
	
	nd.collider->ApplyForceAt(force, position);
}

// public func void applyTorque( Vector torque )
deClassCollider::nfApplyTorque::nfApplyTorque(const sInitData &init) : dsFunction(init.clsCol,
"applyTorque", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // torque
}
void deClassCollider::nfApplyTorque::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCollider*)GetOwnerClass())->GetDS();
	const decVector &torque = ds.GetClassVector()->GetVector(rt->GetValue(0)->GetRealObject());
	nd.collider->ApplyTorque(torque);
}



// Notifications
//////////////////

// public func ColliderListener getColliderListener()
deClassCollider::nfGetColliderListener::nfGetColliderListener(const sInitData &init) : dsFunction(init.clsCol,
"getColliderListener", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsCLL){
}
void deClassCollider::nfGetColliderListener::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCollider*)GetOwnerClass())->GetDS();
	dedsCollider * const scrCol = (dedsCollider*)nd.collider->GetPeerScripting();
	
	if(scrCol){
		rt->PushObject(scrCol->GetCallback(), ds.GetClassColliderListener());
		
	}else{
		rt->PushObject(NULL, ds.GetClassColliderListener());
	}
}

// public func void setColliderListener( ColliderListener listener )
deClassCollider::nfSetColliderListener::nfSetColliderListener(const sInitData &init) : dsFunction(init.clsCol,
"setColliderListener", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCLL); // listener
}
void deClassCollider::nfSetColliderListener::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	dedsCollider * const scrCol = (dedsCollider*)nd.collider->GetPeerScripting();
	if(scrCol){
		scrCol->SetCallback(rt->GetValue(0)->GetRealObject());
	}
}

// public func bool colliderListenerGetCustomCanHit()
deClassCollider::nfColliderListenerGetCustomCanHit::nfColliderListenerGetCustomCanHit(const sInitData &init) : dsFunction(init.clsCol,
"colliderListenerGetCustomCanHit", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassCollider::nfColliderListenerGetCustomCanHit::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const dedsCollider * const scrCol = (dedsCollider*)nd.collider->GetPeerScripting();
	
	if(scrCol){
		rt->PushBool(scrCol->GetEnableCanHitCallback());
		
	}else{
		rt->PushBool(false);
	}
}

// public func void colliderListenerSetCustomCanHit( bool customCanHit )
deClassCollider::nfColliderListenerSetCustomCanHit::nfColliderListenerSetCustomCanHit(const sInitData &init) : dsFunction(init.clsCol,
"colliderListenerSetCustomCanHit", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // customCanHit
}
void deClassCollider::nfColliderListenerSetCustomCanHit::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	dedsCollider * const scrCol = (dedsCollider*)nd.collider->GetPeerScripting();
	if(scrCol){
		scrCol->SetEnableCanHitCallback(rt->GetValue(0)->GetBool());
	}
}



// public func ColliderBreakingListener getBreakingListener()
deClassCollider::nfGetBreakingListener::nfGetBreakingListener(const sInitData &init) : dsFunction(init.clsCol,
"getBreakingListener", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsCBL){
}
void deClassCollider::nfGetBreakingListener::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCollider*)GetOwnerClass())->GetDS();
	dedsCollider * const scrCol = (dedsCollider*)nd.collider->GetPeerScripting();
	
	if(scrCol){
		rt->PushObject(scrCol->GetCallbackBreaking(), ds.GetClassColliderBreakingListener());
		
	}else{
		rt->PushObject(NULL, ds.GetClassColliderBreakingListener());
	}
}

// public func void setBreakingListener( ColliderBreakingListener listener )
deClassCollider::nfSetBreakingListener::nfSetBreakingListener(const sInitData &init) : dsFunction(init.clsCol,
"setBreakingListener", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCBL); // listener
}
void deClassCollider::nfSetBreakingListener::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	dedsCollider * const scrCol = (dedsCollider*)nd.collider->GetPeerScripting();
	if(scrCol){
		scrCol->SetCallbackBreaking(rt->GetValue(0)->GetRealObject());
	}
}



// Collision Detection
////////////////////////

// public func bool pointInside( DVector point )
deClassCollider::nfPointInside::nfPointInside(const sInitData &init) : dsFunction(init.clsCol,
"pointInside", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsDVec); // point
}
void deClassCollider::nfPointInside::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	deClassCollider * const clsCol = (deClassCollider*)GetOwnerClass();
	const deScriptingDragonScript &ds = clsCol->GetDS();
	
	const decDVector &point = ds.GetClassDVector()->GetDVector(rt->GetValue(0)->GetRealObject());
	
	rt->PushBool(nd.collider->PointInside(point));
}

// public func void rayHits( DVector rayOrigin, Vector rayDirection, ColliderListener listener )
deClassCollider::nfRayHits::nfRayHits(const sInitData &init) : dsFunction(init.clsCol,
"rayHits", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsDVec); // rayOrigin
	p_AddParameter(init.clsVec); // rayDirection
	p_AddParameter(init.clsCLL); // listener
}
void deClassCollider::nfRayHits::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	deClassCollider * const clsCol = (deClassCollider*)GetOwnerClass();
	deScriptingDragonScript &ds = clsCol->GetDS();
	
	const decDVector &rayOrigin = ds.GetClassDVector()->GetDVector(rt->GetValue(0)->GetRealObject());
	const decVector &rayDirection = ds.GetClassVector()->GetVector(rt->GetValue(1)->GetRealObject());
	dsRealObject * const objListener = rt->GetValue(2)->GetRealObject();
	
	dedsColliderListenerAdaptor &rayHits = ds.GetColliderListenerAdaptor();
	rayHits.SetListener(objListener);
	rayHits.Reset();
	
	nd.collider->RayHits(rayOrigin, rayDirection, &rayHits);
	
	rayHits.SetListener(NULL);
	rayHits.Reset();
}

// public func void rayHitsClosest( DVector rayOrigin, Vector rayDirection, ColliderListener listener )
deClassCollider::nfRayHitsClosest::nfRayHitsClosest(const sInitData &init) : dsFunction(init.clsCol,
"rayHitsClosest", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsDVec); // rayOrigin
	p_AddParameter(init.clsVec); // rayDirection
	p_AddParameter(init.clsCLL); // listener
}
void deClassCollider::nfRayHitsClosest::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	deClassCollider * const clsCol = (deClassCollider*)GetOwnerClass();
	deScriptingDragonScript &ds = clsCol->GetDS();
	
	const decDVector &rayOrigin = ds.GetClassDVector()->GetDVector(rt->GetValue(0)->GetRealObject());
	const decVector &rayDirection = ds.GetClassVector()->GetVector(rt->GetValue(1)->GetRealObject());
	dsRealObject * const objListener = rt->GetValue(2)->GetRealObject();
	
	dedsColliderListenerClosest &hitsClosest = ds.GetColliderListenerClosest();
	hitsClosest.SetListener(objListener);
	hitsClosest.Reset();
	
	nd.collider->RayHits(rayOrigin, rayDirection, &hitsClosest);
	
	hitsClosest.NotifyListener();
	hitsClosest.SetListener(NULL);
	hitsClosest.Reset();
}

// public func void colliderHits( Collider collider, ColliderListener listener )
deClassCollider::nfColliderHits::nfColliderHits(const sInitData &init) : dsFunction(init.clsCol,
"colliderHits", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCol); // collider
	p_AddParameter(init.clsCLL); // listener
}
void deClassCollider::nfColliderHits::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	deClassCollider * const clsCol = (deClassCollider*)GetOwnerClass();
	deScriptingDragonScript &ds = clsCol->GetDS();
	
	deCollider * const collider = clsCol->GetCollider(rt->GetValue(0)->GetRealObject());
	dsRealObject * const objListener = rt->GetValue(1)->GetRealObject();
	if(!collider || !objListener){
		DSTHROW(dueNullPointer);
	}
	
	dedsColliderListenerAdaptor &adapter = ds.GetColliderListenerAdaptor();
	adapter.SetListener(objListener);
	adapter.Reset();
	
	nd.collider->ColliderHits(collider, &adapter);
	
	adapter.SetListener(NULL);
	adapter.Reset();
}

// public func void colliderMoveHits( Collider collider, Vector displacement, ColliderListener listener )
deClassCollider::nfColliderMoveHits::nfColliderMoveHits(const sInitData &init) : dsFunction(init.clsCol,
"colliderMoveHits", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCol); // collider
	p_AddParameter(init.clsVec); // displacement
	p_AddParameter(init.clsCLL); // listener
}
void deClassCollider::nfColliderMoveHits::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	deClassCollider * const clsCol = (deClassCollider*)GetOwnerClass();
	deScriptingDragonScript &ds = clsCol->GetDS();
	
	deCollider * const collider = clsCol->GetCollider(rt->GetValue(0)->GetRealObject());
	const decVector &displacement = ds.GetVector(rt->GetValue(1)->GetRealObject());
	dsRealObject * const objListener = rt->GetValue(2)->GetRealObject();
	
	if(!collider || !objListener){
		DSTHROW(dueNullPointer);
	}
	
	dedsColliderListenerAdaptor &adapter = ds.GetColliderListenerAdaptor();
	adapter.SetListener(objListener);
	adapter.Reset();
	
	nd.collider->ColliderMoveHits(collider, displacement, &adapter);
	
	adapter.SetListener(NULL);
	adapter.Reset();
}

// public func void colliderMoveHitsClosest( Collider collider, Vector displacement, ColliderListener listener )
deClassCollider::nfColliderMoveHitsClosest::nfColliderMoveHitsClosest(const sInitData &init) :
dsFunction(init.clsCol, "colliderMoveHitsClosest", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCol); // collider
	p_AddParameter(init.clsVec); // displacement
	p_AddParameter(init.clsCLL); // listener
}
void deClassCollider::nfColliderMoveHitsClosest::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	deClassCollider * const clsCol = (deClassCollider*)GetOwnerClass();
	deScriptingDragonScript &ds = clsCol->GetDS();
	
	deCollider * const collider = clsCol->GetCollider(rt->GetValue(0)->GetRealObject());
	const decVector &displacement = ds.GetVector(rt->GetValue(1)->GetRealObject());
	dsRealObject * const objListener = rt->GetValue(2)->GetRealObject();
	
	if(!collider || !objListener){
		DSTHROW(dueNullPointer);
	}
	
	dedsColliderListenerClosest &hitsClosest = ds.GetColliderListenerClosest();
	hitsClosest.SetListener(objListener);
	hitsClosest.Reset();
	
	nd.collider->ColliderMoveHits(collider, displacement, &hitsClosest);
	
	hitsClosest.NotifyListener();
	hitsClosest.SetListener(NULL);
	hitsClosest.Reset();
}

// public func void colliderRotateHits( Collider collider, Vector rotation, ColliderListener listener )
deClassCollider::nfColliderRotateHits::nfColliderRotateHits(const sInitData &init) : dsFunction(init.clsCol,
"colliderRotateHits", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCol); // collider
	p_AddParameter(init.clsVec); // rotation
	p_AddParameter(init.clsCLL); // listener
}
void deClassCollider::nfColliderRotateHits::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	deClassCollider * const clsCol = (deClassCollider*)GetOwnerClass();
	deScriptingDragonScript &ds = clsCol->GetDS();
	
	deCollider * const collider = clsCol->GetCollider(rt->GetValue(0)->GetRealObject());
	const decVector rotation(ds.GetClassVector()->GetVector(rt->GetValue(1)->GetRealObject()) * DEG2RAD);
	dsRealObject * const objListener = rt->GetValue(2)->GetRealObject();
	
	if(!collider || !objListener){
		DSTHROW(dueNullPointer);
	}
	
	dedsColliderListenerAdaptor &adapter = ds.GetColliderListenerAdaptor();
	adapter.SetListener(objListener);
	adapter.Reset();
	
	nd.collider->ColliderRotateHits(collider, rotation, &adapter);
	
	adapter.SetListener(NULL);
	adapter.Reset();
}

// public func void colliderRotateHitsClosest( Collider collider, Vector rotation, ColliderListener listener )
deClassCollider::nfColliderRotateHitsClosest::nfColliderRotateHitsClosest(const sInitData &init) :
dsFunction(init.clsCol, "colliderRotateHitsClosest", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCol); // collider
	p_AddParameter(init.clsVec); // rotation
	p_AddParameter(init.clsCLL); // listener
}
void deClassCollider::nfColliderRotateHitsClosest::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	deClassCollider * const clsCol = (deClassCollider*)GetOwnerClass();
	deScriptingDragonScript &ds = clsCol->GetDS();
	
	deCollider * const collider = clsCol->GetCollider(rt->GetValue(0)->GetRealObject());
	const decVector rotation(ds.GetClassVector()->GetVector(rt->GetValue(1)->GetRealObject()) * DEG2RAD);
	dsRealObject * const objListener = rt->GetValue(2)->GetRealObject();
	
	if(!collider || !objListener){
		DSTHROW(dueNullPointer);
	}
	
	dedsColliderListenerClosest &hitsClosest = ds.GetColliderListenerClosest();
	hitsClosest.SetListener(objListener);
	hitsClosest.Reset();
	
	nd.collider->ColliderRotateHits(collider, rotation, &hitsClosest);
	
	hitsClosest.NotifyListener();
	hitsClosest.SetListener(NULL);
	hitsClosest.Reset();
}

// public func void colliderMoveRotateHits( Collider collider, Vector displacement, Vector rotation, ColliderListener listener )
deClassCollider::nfColliderMoveRotateHits::nfColliderMoveRotateHits(const sInitData &init) : dsFunction(init.clsCol,
"colliderMoveRotateHits", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCol); // collider
	p_AddParameter(init.clsVec); // displacement
	p_AddParameter(init.clsVec); // rotation
	p_AddParameter(init.clsCLL); // listener
}
void deClassCollider::nfColliderMoveRotateHits::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	deClassCollider * const clsCol = (deClassCollider*)GetOwnerClass();
	deScriptingDragonScript &ds = clsCol->GetDS();
	
	deCollider * const collider = clsCol->GetCollider(rt->GetValue(0)->GetRealObject());
	const decVector &displacement = ds.GetClassVector()->GetVector(rt->GetValue(1)->GetRealObject());
	const decVector rotation(ds.GetClassVector()->GetVector(rt->GetValue(2)->GetRealObject()) * DEG2RAD);
	dsRealObject * const objListener = rt->GetValue(3)->GetRealObject();
	
	if(!collider || !objListener){
		DSTHROW(dueNullPointer);
	}
	
	dedsColliderListenerAdaptor &adapter = ds.GetColliderListenerAdaptor();
	adapter.SetListener(objListener);
	adapter.Reset();
	
	nd.collider->ColliderMoveRotateHits(collider, displacement, rotation, &adapter);
	
	adapter.SetListener(NULL);
	adapter.Reset();
}

// public func void colliderMoveRotateHitsClosest( Collider collider, Vector displacement, ColliderListener listener )
deClassCollider::nfColliderMoveRotateHitsClosest::nfColliderMoveRotateHitsClosest(const sInitData &init) :
dsFunction(init.clsCol, "colliderMoveRotateHitsClosest", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCol); // collider
	p_AddParameter(init.clsVec); // displacement
	p_AddParameter(init.clsVec); // rotation
	p_AddParameter(init.clsCLL); // listener
}
void deClassCollider::nfColliderMoveRotateHitsClosest::RunFunction(dsRunTime *rt, dsValue *myself){
	const sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself));
	if(!nd.collider){
		DSTHROW(dueNullPointer);
	}
	
	deClassCollider * const clsCol = (deClassCollider*)GetOwnerClass();
	deScriptingDragonScript &ds = clsCol->GetDS();
	
	deCollider * const collider = clsCol->GetCollider(rt->GetValue(0)->GetRealObject());
	const decVector &displacement = ds.GetClassVector()->GetVector(rt->GetValue(1)->GetRealObject());
	const decVector rotation(ds.GetClassVector()->GetVector(rt->GetValue(2)->GetRealObject()) * DEG2RAD);
	dsRealObject * const objListener = rt->GetValue(3)->GetRealObject();
	
	if(!collider || !objListener){
		DSTHROW(dueNullPointer);
	}
	
	dedsColliderListenerClosest &hitsClosest = ds.GetColliderListenerClosest();
	hitsClosest.SetListener(objListener);
	hitsClosest.Reset();
	
	nd.collider->ColliderMoveRotateHits(collider, displacement, rotation, &hitsClosest);
	
	hitsClosest.NotifyListener();
	hitsClosest.SetListener(NULL);
	hitsClosest.Reset();
}



// public func int hashCode()
deClassCollider::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsCol, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}

void deClassCollider::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	deCollider *collider = static_cast<sColNatDat*>(p_GetNativeData(myself))->collider;
	// hash code = memory location
	rt->PushInt((int)(intptr_t)collider);
}

// public func bool equals( Object obj )
deClassCollider::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsCol, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // obj
}
void deClassCollider::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	deCollider * const collider = static_cast<sColNatDat*>(p_GetNativeData(myself))->collider;
	deClassCollider * const clsCol = (deClassCollider*)GetOwnerClass();
	dsValue * const obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsCol)){
		rt->PushBool(false);
		
	}else{
		deCollider * const otherCol = static_cast<sColNatDat*>(p_GetNativeData(obj))->collider;
		rt->PushBool(collider == otherCol);
	}
}

// public static func bool equals( Collider collider1, Collider collider2 )
deClassCollider::nfEquals2::nfEquals2(const sInitData &init) :
dsFunction(init.clsCol, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsBool){
	p_AddParameter(init.clsCol); // collider1
	p_AddParameter(init.clsCol); // collider2
}
void deClassCollider::nfEquals2::RunFunction(dsRunTime *rt, dsValue *myself){
	const deClassCollider &clsCol = *((deClassCollider*)GetOwnerClass());
	deCollider * const collider1 = clsCol.GetCollider(rt->GetValue(0)->GetRealObject());
	deCollider * const collider2 = clsCol.GetCollider(rt->GetValue(1)->GetRealObject());
	
	rt->PushBool(collider1 == collider2);
}



// Class deClassCollider
//////////////////////////

// Constructor, Destructor
////////////////////////////

deClassCollider::deClassCollider(deScriptingDragonScript &ds) :
dsClass("Collider", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS(ds){
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sColNatDat));
}

deClassCollider::~deClassCollider(){
}



// Management
///////////////

void deClassCollider::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	pClsCollisionResponse = engine->GetClass("Dragengine.Scenery.CollisionResponse");
	
	init.clsCol = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	init.clsVec = pDS.GetClassVector();
	init.clsDVec = pDS.GetClassDVector();
	init.clsQuat = pDS.GetClassQuaternion();
	init.clsCLL = pDS.GetClassColliderListener();
	init.clsCCon = pDS.GetClassColliderConstraint();
	init.clsCI = pDS.GetClassCollisionInfo();
	init.clsCF = pDS.GetClassCollisionFilter();
	init.clsDMat = pDS.GetClassDMatrix();
	init.clsCBL = pDS.GetClassColliderBreakingListener();
	init.clsCCT = pDS.GetClassColliderCollisionTest();
	init.clsCollisionResponse = pClsCollisionResponse;
	init.clsWorld = pDS.GetClassWorld();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetPosition(init));
	AddFunction(new nfSetPosition(init));
	AddFunction(new nfGetOrientation(init));
	AddFunction(new nfSetOrientation(init));
	AddFunction(new nfSetGeometry(init));
	AddFunction(new nfSetGeometry2(init));
	AddFunction(new nfGetScale(init));
	AddFunction(new nfSetScale(init));
	AddFunction(new nfGetView(init));
	AddFunction(new nfGetUp(init));
	AddFunction(new nfGetRight(init));
	AddFunction(new nfGetMatrix(init));
	AddFunction(new nfGetInverseMatrix(init));
	AddFunction(new nfGetLinearVelocity(init));
	AddFunction(new nfSetLinearVelocity(init));
	AddFunction(new nfGetAngularVelocity(init));
	AddFunction(new nfSetAngularVelocity(init));
	AddFunction(new nfGetMass(init));
	AddFunction(new nfSetMass(init));
	AddFunction(new nfGetGravity(init));
	AddFunction(new nfSetGravity(init));
	AddFunction(new nfGetEnabled(init));
	AddFunction(new nfSetEnabled(init));
	AddFunction(new nfGetResponseType(init));
	AddFunction(new nfSetResponseType(init));
	AddFunction(new nfGetUseLocalGravity(init));
	AddFunction(new nfSetUseLocalGravity(init));
	AddFunction(new nfGetParentWorld(init));
	
	AddFunction(new nfGetOwner(init));
	AddFunction(new nfSetOwner(init));
	
	AddFunction(new nfGetForceFieldDirect(init));
	AddFunction(new nfSetForceFieldDirect(init));
	AddFunction(new nfGetForceFieldSurface(init));
	AddFunction(new nfSetForceFieldSurface(init));
	AddFunction(new nfGetForceFieldMass(init));
	AddFunction(new nfSetForceFieldMass(init));
	AddFunction(new nfGetForceFieldSpeed(init));
	AddFunction(new nfSetForceFieldSpeed(init));
	
	AddFunction(new nfGetCollisionFilter(init));
	AddFunction(new nfSetCollisionFilter(init));
	
	AddFunction(new nfGetAttachmentCount(init));
	AddFunction(new nfGetAttachedResource(init));
	AddFunction(new nfAttachStatic(init));
	AddFunction(new nfAttachStatic2(init));
	AddFunction(new nfAttachRelativeMovement(init));
	AddFunction(new nfDetach(init));
	AddFunction(new nfDetachAll(init));
	AddFunction(new nfAttachmentsForceUpdate(init));
	
	AddFunction(new nfGetConstraintCount(init));
	AddFunction(new nfAddConstraint(init));
	AddFunction(new nfHasConstraint(init));
	AddFunction(new nfIndexOfConstraint(init));
	AddFunction(new nfGetConstraintAt(init));
	AddFunction(new nfSetConstraintAt(init));
	AddFunction(new nfRemoveConstraint(init));
	AddFunction(new nfRemoveConstraintFrom(init));
	AddFunction(new nfRemoveAllConstraints(init));
	
	AddFunction(new nfGetIgnoreColliderCount(init));
	AddFunction(new nfGetIgnoreColliderAt(init));
	AddFunction(new nfHasIgnoreCollider(init));
	AddFunction(new nfAddIgnoreCollider(init));
	AddFunction(new nfRemoveIgnoreCollider(init));
	AddFunction(new nfRemoveAllIgnoreColliders(init));
	
	AddFunction(new nfGetCollisionTestCount(init));
	AddFunction(new nfGetCollisionTestAt(init));
	AddFunction(new nfAddCollisionTest(init));
	AddFunction(new nfRemoveCollisionTest(init));
	AddFunction(new nfRemoveAllCollisionTests(init));
	
	AddFunction(new nfApplyImpuls(init));
	AddFunction(new nfApplyImpulsAt(init));
	AddFunction(new nfApplyTorqueImpuls(init));
	AddFunction(new nfApplyForce(init));
	AddFunction(new nfApplyForceAt(init));
	AddFunction(new nfApplyTorque(init));
	
	AddFunction(new nfGetColliderListener(init));
	AddFunction(new nfSetColliderListener(init));
	AddFunction(new nfColliderListenerGetCustomCanHit(init));
	AddFunction(new nfColliderListenerSetCustomCanHit(init));
	
	AddFunction(new nfGetBreakingListener(init));
	AddFunction(new nfSetBreakingListener(init));
	
	AddFunction(new nfPointInside(init));
	AddFunction(new nfRayHits(init));
	AddFunction(new nfRayHitsClosest(init));
	AddFunction(new nfColliderHits(init));
	AddFunction(new nfColliderMoveHits(init));
	AddFunction(new nfColliderMoveHitsClosest(init));
	AddFunction(new nfColliderRotateHits(init));
	AddFunction(new nfColliderRotateHitsClosest(init));
	AddFunction(new nfColliderMoveRotateHits(init));
	AddFunction(new nfColliderMoveRotateHitsClosest(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfEquals2(init));
	AddFunction(new nfHashCode(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}



deCollider *deClassCollider::GetCollider(dsRealObject *myself) const {
	if(!myself){
		return NULL;
	}
	
	return static_cast<sColNatDat*>(p_GetNativeData(myself->GetBuffer()))->collider;
}

void deClassCollider::AssignCollider(dsRealObject *myself, deCollider *collider){
	if(!myself){
		DSTHROW(dueInvalidParam);
	}
	
	sColNatDat &nd = *static_cast<sColNatDat*>(p_GetNativeData(myself->GetBuffer()));
	
	if(collider == nd.collider){
		return;
	}
	
	if(nd.collider){
		nd.collider->FreeReference();
	}
	
	nd.collider = collider;
	
	if(collider){
		collider->AddReference();
	}
}

void deClassCollider::PushCollider(dsRunTime *rt, deCollider *collider){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!collider){
		rt->PushObject(NULL, this);
		return;
	}
	
	deColliderVisitorIdentify identify;
	collider->Visit(identify);
	
	switch(identify.GetType()){
	case deColliderVisitorIdentify::ectVolume:
		pDS.GetClassColliderVolume()->PushCollider(rt, &identify.CastToVolume());
		break;
		
	case deColliderVisitorIdentify::ectRig:
		pDS.GetClassColliderRig()->PushCollider(rt, &identify.CastToRig());
		break;
		
	case deColliderVisitorIdentify::ectComponent:
		pDS.GetClassColliderComponent()->PushCollider(rt, &identify.CastToComponent());
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
}



// For member use only
////////////////////////

deResource *deClassCollider::GetResource(dsValue &myself) const{
	if(myself.GetType()->GetPrimitiveType() != DSPT_OBJECT){
		return NULL;
	}
	
	dsRealObject * const realObject = myself.GetRealObject();
	if(!realObject){
		return NULL;
	}
	
	dsClass * const sclass = realObject->GetType();
	
	if(sclass == pDS.GetClassCollider()){
		return pDS.GetClassCollider()->GetCollider(realObject);
		
	}else if(sclass == pDS.GetClassColliderVolume()){
		return pDS.GetClassColliderVolume()->GetCollider(realObject);
		
	}else if(sclass == pDS.GetClassColliderRig()){
		return pDS.GetClassColliderRig()->GetCollider(realObject);
		
	}else if(sclass == pDS.GetClassColliderComponent()){
		return pDS.GetClassColliderComponent()->GetCollider(realObject);
		
	}else if(sclass == pDS.GetClassBillboard()){
		return pDS.GetClassBillboard()->GetBillboard(realObject);
		
	}else if(sclass == pDS.GetClassCamera()){
		return pDS.GetClassCamera()->GetCamera(realObject);
		
	}else if(sclass == pDS.GetClassComponent()){
		return pDS.GetClassComponent()->GetComponent(realObject);
		
	}else if(sclass == pDS.GetClassDebugDrawer()){
		return pDS.GetClassDebugDrawer()->GetDebugDrawer(realObject);
		
	}else if(sclass == pDS.GetClassEnvMapProbe()){
		return pDS.GetClassEnvMapProbe()->GetEnvMapProbe(realObject);
		
	}else if(sclass == pDS.GetClassForceField()){
		return pDS.GetClassForceField()->GetForceField(realObject);
		
	}else if(sclass == pDS.GetClassLight()){
		return pDS.GetClassLight()->GetLight(realObject);
		
	}else if(sclass == pDS.GetClassLumimeter()){
		return pDS.GetClassLumimeter()->GetLumimeter(realObject);
		
	}else if(sclass == pDS.GetClassMicrophone()){
		return pDS.GetClassMicrophone()->GetMicrophone(realObject);
		
	}else if(sclass == pDS.GetClassNavigationSpace()){
		return pDS.GetClassNavigationSpace()->GetNavigationSpace(realObject);
		
	}else if(sclass == pDS.GetClassNavigationBlocker()){
		return pDS.GetClassNavigationBlocker()->GetNavigationBlocker(realObject);
		
	}else if(sclass == pDS.GetClassNavigator()){
		return pDS.GetClassNavigator()->GetNavigator(realObject);
		
	}else if(sclass == pDS.GetClassParticleEmitterInstance()){
		return pDS.GetClassParticleEmitterInstance()->GetInstance(realObject);
		
	}else if(sclass == pDS.GetClassPropField()){
		return pDS.GetClassPropField()->GetPropField(realObject);
		
	}else if(sclass == pDS.GetClassSpeaker()){
		return pDS.GetClassSpeaker()->GetSpeaker(realObject);
		
	}else if(sclass == pDS.GetClassTouchSensor()){
		return pDS.GetClassTouchSensor()->GetTouchSensor(realObject);
		
	}else if(sclass == pDS.GetClassSoundLevelMeter()){
		return pDS.GetClassSoundLevelMeter()->GetSoundLevelMeter(realObject);
	}
	
	return NULL;
}

void deClassCollider::PushResource(dsRunTime &rt, deResource *resource){
	if(!resource){
		rt.PushObject(NULL, pDS.GetScriptEngine()->GetClassObject());
		return;
	}
	
	switch(resource->GetResourceManager()->GetResourceType()){
	case deResourceManager::ertCollider:
		PushCollider(&rt, (deCollider*)resource);
		break;
		
	case deResourceManager::ertBillboard:
		pDS.GetClassBillboard()->PushBillboard(&rt, (deBillboard*)resource);
		break;
		
	case deResourceManager::ertCamera:
		pDS.GetClassCamera()->PushCamera(&rt, (deCamera*)resource);
		break;
		
	case deResourceManager::ertComponent:
		pDS.GetClassComponent()->PushComponent(&rt, (deComponent*)resource);
		break;
		
	case deResourceManager::ertDebugDrawer:
		pDS.GetClassDebugDrawer()->PushDebugDrawer(&rt, (deDebugDrawer*)resource);
		break;
		
	case deResourceManager::ertEnvMapProbe:
		pDS.GetClassEnvMapProbe()->PushEnvMapProbe(&rt, (deEnvMapProbe*)resource);
		break;
		
	case deResourceManager::ertForceField:
		pDS.GetClassForceField()->PushForceField(&rt, (deForceField*)resource);
		break;
		
	case deResourceManager::ertLight:
		pDS.GetClassLight()->PushLight(&rt, (deLight*)resource);
		break;
		
	case deResourceManager::ertLumimeter:
		pDS.GetClassLumimeter()->PushLumimeter(&rt, (deLumimeter*)resource);
		break;
		
	case deResourceManager::ertMicrophone:
		pDS.GetClassMicrophone()->PushMicrophone(&rt, (deMicrophone*)resource);
		break;
		
	case deResourceManager::ertNavigationSpace:
		pDS.GetClassNavigationSpace()->PushNavigationSpace(&rt, (deNavigationSpace*)resource);
		break;
		
	case deResourceManager::ertNavigationBlocker:
		pDS.GetClassNavigationBlocker()->PushNavigationBlocker(&rt, (deNavigationBlocker*)resource);
		break;
		
	case deResourceManager::ertNavigator:
		pDS.GetClassNavigator()->PushNavigator(&rt, (deNavigator*)resource);
		break;
		
	case deResourceManager::ertParticleEmitterInstance:
		pDS.GetClassParticleEmitterInstance()->PushInstance(&rt, (deParticleEmitterInstance*)resource);
		break;
		
	case deResourceManager::ertPropField:
		pDS.GetClassPropField()->PushPropField(&rt, (dePropField*)resource);
		break;
		
	case deResourceManager::ertSpeaker:
		pDS.GetClassSpeaker()->PushSpeaker(&rt, (deSpeaker*)resource);
		break;
		
	case deResourceManager::ertTouchSensor:
		pDS.GetClassTouchSensor()->PushTouchSensor(&rt, (deTouchSensor*)resource);
		break;
		
	case deResourceManager::ertSoundLevelMeter:
		pDS.GetClassSoundLevelMeter()->PushSoundLevelMeter(&rt, (deSoundLevelMeter*)resource);
		break;
		
	default:
		rt.PushObject(NULL, pDS.GetScriptEngine()->GetClassObject());
	}
}

decDMatrix deClassCollider::GetResourceMatrix(const deResource &resource){
	switch(resource.GetResourceManager()->GetResourceType()){
	case deResourceManager::ertCollider:{
		const deCollider &collider = (const deCollider &)resource;
		return decDMatrix::CreateWorld(collider.GetPosition(),
			collider.GetOrientation(), collider.GetScale());
		};
		
	case deResourceManager::ertBillboard:{
		const deBillboard &billboard = (const deBillboard &)resource;
		return decDMatrix::CreateTranslation(billboard.GetPosition());
		};
		
	case deResourceManager::ertCamera:{
		const deCamera &camera = (const deCamera &)resource;
		return decDMatrix::CreateWorld(camera.GetPosition(), camera.GetOrientation());
		};
		
	case deResourceManager::ertComponent:{
		const deComponent &component = (const deComponent &)resource;
		return decDMatrix::CreateWorld(component.GetPosition(),
			component.GetOrientation(), component.GetScaling());
		};
		
	case deResourceManager::ertDebugDrawer:{
		const deDebugDrawer &debugDrawer = (const deDebugDrawer &)resource;
		return decDMatrix::CreateWorld(debugDrawer.GetPosition(),
			debugDrawer.GetOrientation(), debugDrawer.GetScale());
		};
		
	case deResourceManager::ertEnvMapProbe:{
		const deEnvMapProbe &envMapProbe = (const deEnvMapProbe &)resource;
		return decDMatrix::CreateWorld(envMapProbe.GetPosition(), envMapProbe.GetOrientation());
		};
		
	case deResourceManager::ertForceField:{
		const deForceField &forceField = (const deForceField &)resource;
		return decDMatrix::CreateWorld(forceField.GetPosition(), forceField.GetOrientation());
		};
		
	case deResourceManager::ertLight:{
		const deLight &light = (const deLight &)resource;
		return decDMatrix::CreateWorld(light.GetPosition(), light.GetOrientation());
		};
		
	case deResourceManager::ertLumimeter:{
		const deLumimeter &lumimeter = (const deLumimeter &)resource;
		return decDMatrix::CreateTranslation(lumimeter.GetPosition());
		};
		
	case deResourceManager::ertMicrophone:{
		const deMicrophone &microphone = (const deMicrophone &)resource;
		return decDMatrix::CreateWorld(microphone.GetPosition(), microphone.GetOrientation());
		};
		
	case deResourceManager::ertNavigationSpace:{
		const deNavigationSpace &navigationSpace = (const deNavigationSpace &)resource;
		return decDMatrix::CreateWorld(navigationSpace.GetPosition(), navigationSpace.GetOrientation());
		};
		
	case deResourceManager::ertNavigationBlocker:{
		const deNavigationBlocker &navigationBlocker = (const deNavigationBlocker &)resource;
		return decDMatrix::CreateWorld(navigationBlocker.GetPosition(),
			navigationBlocker.GetOrientation(), navigationBlocker.GetScaling());
		};
		
	case deResourceManager::ertNavigator:{
		return decDMatrix(); // because navigator path is in world space
		};
		
	case deResourceManager::ertParticleEmitterInstance:{
		const deParticleEmitterInstance &instance = (const deParticleEmitterInstance &)resource;
		return decDMatrix::CreateWorld(instance.GetPosition(), instance.GetOrientation());
		};
		
	case deResourceManager::ertPropField:{
		const dePropField &propField = (const dePropField &)resource;
		return decDMatrix::CreateTranslation(propField.GetPosition());
		};
		
	case deResourceManager::ertSpeaker:{
		const deSpeaker &speaker = (const deSpeaker &)resource;
		return decDMatrix::CreateWorld(speaker.GetPosition(), speaker.GetOrientation());
		};
		
	case deResourceManager::ertTouchSensor:{
		const deTouchSensor &touchSensor = (const deTouchSensor &)resource;
		return decDMatrix::CreateWorld(touchSensor.GetPosition(), touchSensor.GetOrientation());
		};
		
	case deResourceManager::ertSoundLevelMeter:{
		const deSoundLevelMeter &soundLevelMeter = (const deSoundLevelMeter &)resource;
		return decDMatrix::CreateWorld(soundLevelMeter.GetPosition(), soundLevelMeter.GetOrientation());
		};
		
	default:
		return decDMatrix();
	}
}

decVector deClassCollider::GetResourceScale(const deResource &resource){
	switch(resource.GetResourceManager()->GetResourceType()){
	case deResourceManager::ertCollider:{
		const deCollider &collider = (const deCollider &)resource;
		return collider.GetScale();
		};
		
	case deResourceManager::ertComponent:{
		const deComponent &component = (const deComponent &)resource;
		return component.GetScaling();
		};
		
	case deResourceManager::ertDebugDrawer:{
		const deDebugDrawer &debugDrawer = (const deDebugDrawer &)resource;
		return debugDrawer.GetScale();
		};
		
	case deResourceManager::ertNavigationBlocker:{
		const deNavigationBlocker &navigationBlocker = (const deNavigationBlocker &)resource;
		return navigationBlocker.GetScaling();
		};
		
	default:
		return decVector(1.0, 1.0, 1.0);
	}
}

void deClassCollider::AttachStatic(deCollider &collider, deResource *resource) const{
	const decDMatrix matrix(GetResourceMatrix(*resource).QuickMultiply(
		decDMatrix::CreateWorld(collider.GetPosition(),
			collider.GetOrientation(), collider.GetScale()).QuickInvert()));
	
	AttachStatic(collider, resource, matrix.GetPosition(), matrix.Normalized().ToQuaternion(), matrix.GetScale());
}

void deClassCollider::AttachStatic(deCollider &collider, deResource *resource,
const decVector &position, const decQuaternion &orientation, const decVector &scale) const{
	deColliderAttachment *attachment = NULL;
	
	try{
		attachment = new deColliderAttachment(resource);
		attachment->SetPosition(position);
		attachment->SetOrientation(orientation);
		attachment->SetScaling(scale);
		attachment->SetAttachType(deColliderAttachment::eatStatic);
		collider.AddAttachment(attachment);
		
	}catch(...){
		if(attachment){
			delete attachment;
		}
		throw;
	}
}

void deClassCollider::AttachRig(deCollider &collider, deResource *resource) const{
	deColliderAttachment *attachment = NULL;
	
	try{
		attachment = new deColliderAttachment(resource);
		attachment->SetAttachType(deColliderAttachment::eatRig);
		collider.AddAttachment(attachment);
		
	}catch(...){
		if(attachment){
			delete attachment;
		}
		throw;
	}
}

void deClassCollider::AttachBone(deCollider &collider, deResource *resource, const char *targetBone) const{
	deColliderVisitorIdentify identify;
	decDMatrix matrix;
	
	collider.Visit(identify);
	
	if(identify.IsComponent()){
		const deColliderComponent &colliderComponent = identify.CastToComponent();
		deComponent * const component = colliderComponent.GetComponent();
		
		if(component){
			const deRig * const rig = component->GetRig();
			
			if(rig){
				const int index = rig->IndexOfBoneNamed(targetBone);
				
				if(index != -1){
					component->PrepareBones();
					matrix = component->GetBoneAt(index).GetMatrix();
				}
			}
		}
	}
	
	matrix = GetResourceMatrix(*resource).QuickMultiply(
		matrix.QuickMultiply(decDMatrix::CreateWorld(collider.GetPosition(),
			collider.GetOrientation(), collider.GetScale())).QuickInvert());
	
	AttachBone(collider, resource, targetBone, matrix.GetPosition(),
		matrix.Normalized().ToQuaternion(), matrix.GetScale());
}

void deClassCollider::AttachBone(deCollider &collider, deResource *resource, const char *targetBone,
const decVector &position, const decQuaternion &orientation, const decVector &scale) const{
	deColliderAttachment *attachment = NULL;
	
	try{
		attachment = new deColliderAttachment(resource);
		attachment->SetPosition(position);
		attachment->SetOrientation(orientation);
		attachment->SetScaling(scale);
		attachment->SetTrackBone(targetBone);
		attachment->SetAttachType(deColliderAttachment::eatBone);
		collider.AddAttachment(attachment);
		
	}catch(...){
		if(attachment){
			delete attachment;
		}
		throw;
	}
}

void deClassCollider::AttachWeight(deCollider &collider, deResource *resource, const deCollisionInfo &colInfo) const{
	if(resource->GetResourceManager()->GetResourceType() != deResourceManager::ertCollider){
		pDS.LogWarnFormat("Collider.AttachWeight: Resource type %i can not be attached using "
			"bone weights. Fall back to static.", resource->GetResourceManager()->GetResourceType());
		AttachStatic(collider, resource);
		return;
	}
	
	deCollider * const attachCollider = (deCollider*)resource;
	
	deColliderAttachment *attachment = NULL;
	deColliderVisitorIdentify identify;
	
	collider.Visit(identify);
	
	// if collider component attach using weighting or bone
	if(identify.IsComponent()){
		deColliderComponent &colliderComponent = identify.CastToComponent();
		
		deComponent * const component = colliderComponent.GetComponent();
		if(!component){
			pDS.LogWarn("Collider.AttachWeight: Component is null. Fall back to static.");
			AttachStatic(collider, resource);
			return;
		}
		
		const deRig * const rig = component->GetRig();
		if(!rig){
			pDS.LogWarn("Collider.AttachWeight: Rig is null. Fall back to static.");
			AttachStatic(collider, resource);
			return;
		}
		
		const int bone = colInfo.GetBone();
		const int face = colInfo.GetFace();
		
		if(bone != -1){
			if(bone >= 0 && bone < component->GetBoneCount()){
				component->PrepareBones();
				const decDMatrix matrix(GetResourceMatrix(*resource)
					.QuickMultiply(decDMatrix(component->GetBoneAt(bone).GetMatrix())
						.QuickMultiply(decDMatrix::CreateWorld(collider.GetPosition(),
							collider.GetOrientation())).QuickInvert()));
				AttachBone(collider, resource, rig->GetBoneAt(bone).GetName(),
					matrix.GetPosition(), matrix.Normalized().ToQuaternion(), matrix.GetScale());
				
			}else{
				pDS.LogWarnFormat("Collider.AttachWeight: bone(%i) outside range(%i). "
					"Fall back to static.", bone, component->GetBoneCount());
				AttachStatic(collider, attachCollider);
			}
			
		}else if(face >= 0){
			const deModel * const model = component->GetModel();
			if(!model){
				pDS.LogWarn("Collider.AttachWeight: Model is null. Fall back to static.");
				AttachStatic(collider, resource);
				return;
			}
			
			const deModelLOD &lod = *model->GetLODAt(0);
			if(face >= lod.GetFaceCount()){
				pDS.LogWarnFormat("Collider.AttachWeight: face(%i) outside range(%i). "
					"Fall back to static.", face, lod.GetFaceCount());
				AttachStatic(collider, attachCollider);
				return;
			}
			
			const decDMatrix matrix(GetResourceMatrix(*resource)
				.QuickMultiply(decDMatrix::CreateWorld(collider.GetPosition(),
					collider.GetOrientation(), collider.GetScale())).QuickInvert());
			
			try{
				attachment = new deColliderAttachment(resource);
				attachment->SetAttachType(deColliderAttachment::eatStatic);
				attachment->SetPosition(matrix.GetPosition().ToVector());
				attachment->SetOrientation(matrix.Normalized().ToQuaternion());
				attachment->SetScaling(matrix.GetScale());
				colliderComponent.InitWeightAttachment(*attachment, face);
				collider.AddAttachment(attachment);
				
			}catch(...){
				if(attachment){
					delete attachment;
				}
				throw;
			}
			
		}else{
			pDS.LogWarnFormat("Collider.AttachWeight: bone(%i) and fase(%i) invalid. "
				"Fall back to static.", bone, face);
			AttachStatic(collider, attachCollider);
		}
		
	}else{
		pDS.LogWarn("Collider.AttachWeight: Collider is not of type component or rig. Fall back to static.");
		AttachStatic(collider, attachCollider);
	}
}

void deClassCollider::AttachRelativeMovement(deCollider &collider, deResource *resource) const{
	deColliderAttachment *attachment = NULL;
	
	try{
		attachment = new deColliderAttachment(resource);
		attachment->SetAttachType(deColliderAttachment::eatRelativeMovement);
		collider.AddAttachment(attachment);
		
	}catch(...){
		if(attachment){
			delete attachment;
		}
		throw;
	}
}

deColliderConstraint *deClassCollider::FindConstraint(deCollider &collider,
const deColliderConstraint &constraint) const{
	const int count = collider.GetConstraintCount();
	int i;
	
	for(i=0; i<count; i++){
		deColliderConstraint * const checkConstraint = collider.GetConstraintAt(i);
		if(checkConstraint->GetTargetCollider() == constraint.GetTargetCollider()
		&& checkConstraint->GetTargetBone() == constraint.GetTargetBone()
		&& checkConstraint->GetBone() == constraint.GetBone()){
			return checkConstraint;
		}
	}
	
	return nullptr;
}
