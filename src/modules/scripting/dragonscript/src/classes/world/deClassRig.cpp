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

#include "deClassRig.h"
#include "../resources/deClassResourceListener.h"
#include "../math/deClassVector.h"
#include "../math/deClassQuaternion.h"
#include "../math/deClassMatrix.h"
#include "../collider/deClassColliderConstraint.h"
#include "../utils/deClassShapeList.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"
#include "../../resourceloader/dedsResourceLoader.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/loader/deResourceLoader.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigManager.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/rig/deRigConstraint.h>
#include <dragengine/resources/collider/deColliderConstraint.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/rig/deBaseRigModule.h>

// native structure
struct sRigNatDat{
	deRig::Ref rig;
};

// Native Functions
/////////////////////

// public func new(  String filename  )
deClassRig::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsRig,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsStr); // filename
}
void deClassRig::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sRigNatDat * const nd = new (p_GetNativeData(myself)) sRigNatDat;
	deClassRig &clsRig = *((deClassRig*)GetOwnerClass());
	deRigManager &rigMgr = *clsRig.GetDS()->GetGameEngine()->GetRigManager();
	
	// prepare
	
	// load rig
	nd->rig = rigMgr.LoadRig(rt->GetValue(0)->GetString(), "/");
	if(!nd->rig){
		DSTHROW(dueInvalidParam);
	}
}

// static public func void loadAsynchron( String filename, ResourceListener listener )
deClassRig::nfLoadAsynchron::nfLoadAsynchron(const sInitData &init) : dsFunction(init.clsRig,
"loadAsynchron", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid){
	p_AddParameter(init.clsStr); // filename
	p_AddParameter(init.clsRN); // listener
}
void deClassRig::nfLoadAsynchron::RunFunction(dsRunTime *rt, dsValue*){
	deClassRig &clsRig = *((deClassRig*)GetOwnerClass());
	
	const char * const filename = rt->GetValue(0)->GetString();
	dsRealObject * const listener = rt->GetValue(1)->GetRealObject();
	if(!listener){
		DSTHROW(dueNullPointer);
	}
	
	clsRig.GetDS()->GetResourceLoader()->AddRequest(filename, deResourceLoader::ertRig, listener);
}

// public func destructor()
deClassRig::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsRig,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassRig::nfDestructor::RunFunction(dsRunTime*, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sRigNatDat*>(p_GetNativeData(myself))->~sRigNatDat();
}



// public func String GetFilename()
deClassRig::nfGetFilename::nfGetFilename(const sInitData &init) : dsFunction(init.clsRig,
"getFilename", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassRig::nfGetFilename::RunFunction(dsRunTime *rt, dsValue *myself){
	const deRig &rig = *(static_cast<sRigNatDat*>(p_GetNativeData(myself))->rig);
	rt->PushString(rig.GetFilename());
}



// public func int getBoneCount()
deClassRig::nfGetBoneCount::nfGetBoneCount(const sInitData &init) : dsFunction(init.clsRig,
"getBoneCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassRig::nfGetBoneCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deRig &rig = *(static_cast<sRigNatDat*>(p_GetNativeData(myself))->rig);
	rt->PushInt(rig.GetBoneCount());
}

// public func int indexOfBoneNamed( String name )
deClassRig::nfIndexOfBoneNamed::nfIndexOfBoneNamed(const sInitData &init) : dsFunction(init.clsRig,
"indexOfBoneNamed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsStr); // name
}
void deClassRig::nfIndexOfBoneNamed::RunFunction(dsRunTime *rt, dsValue *myself){
	const deRig &rig = *(static_cast<sRigNatDat*>(p_GetNativeData(myself))->rig);
	rt->PushInt(rig.IndexOfBoneNamed(rt->GetValue(0)->GetString()));
}

// public func String boneGetName( int bone )
deClassRig::nfBoneGetName::nfBoneGetName(const sInitData &init) : dsFunction(init.clsRig,
"boneGetName", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
	p_AddParameter(init.clsInt); // bone
}
void deClassRig::nfBoneGetName::RunFunction(dsRunTime *rt, dsValue *myself){
	const deRig &rig = *(static_cast<sRigNatDat*>(p_GetNativeData(myself))->rig);
	const int bone = rt->GetValue(0)->GetInt();
	
	rt->PushString(rig.GetBoneAt(bone).GetName());
}

// public func Vector boneGetPosition( int bone )
deClassRig::nfBoneGetPosition::nfBoneGetPosition(const sInitData &init) : dsFunction(init.clsRig,
"boneGetPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
	p_AddParameter(init.clsInt); // bone
}
void deClassRig::nfBoneGetPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	const deRig &rig = *(static_cast<sRigNatDat*>(p_GetNativeData(myself))->rig);
	const deScriptingDragonScript &ds = *(((deClassRig*)GetOwnerClass())->GetDS());
	const int bone = rt->GetValue(0)->GetInt();
	
	ds.GetClassVector()->PushVector(rt, rig.GetBoneAt(bone).GetPosition());
}

// public func Quaternion boneGetOrientation( int bone )
deClassRig::nfBoneGetOrientation::nfBoneGetOrientation(const sInitData &init) : dsFunction(init.clsRig,
"boneGetOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat){
	p_AddParameter(init.clsInt); // bone
}
void deClassRig::nfBoneGetOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	const deRig &rig = *(static_cast<sRigNatDat*>(p_GetNativeData(myself))->rig);
	const deScriptingDragonScript &ds = *(((deClassRig*)GetOwnerClass())->GetDS());
	const int bone = rt->GetValue(0)->GetInt();
	
	ds.GetClassQuaternion()->PushQuaternion(rt, decQuaternion::CreateFromEuler(rig.GetBoneAt(bone).GetRotation()));
}

// public func Vector boneGetRotation(int bone)
deClassRig::nfBoneGetRotation::nfBoneGetRotation(const sInitData &init) : dsFunction(init.clsRig,
"boneGetRotation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
	p_AddParameter(init.clsInt); // bone
}
void deClassRig::nfBoneGetRotation::RunFunction(dsRunTime *rt, dsValue *myself){
	const deRig &rig = *(static_cast<sRigNatDat*>(p_GetNativeData(myself))->rig);
	const deScriptingDragonScript &ds = *(((deClassRig*)GetOwnerClass())->GetDS());
	const int bone = rt->GetValue(0)->GetInt();
	
	ds.GetClassVector()->PushVector(rt, rig.GetBoneAt(bone).GetRotation() * RAD2DEG);
}

// public func Matrix getBoneMatrix( int bone )
deClassRig::nfBoneGetMatrix::nfBoneGetMatrix(const sInitData &init) : dsFunction(init.clsRig,
"getBoneMatrix", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsMat){
	p_AddParameter(init.clsInt); // bone
}
void deClassRig::nfBoneGetMatrix::RunFunction(dsRunTime *rt, dsValue *myself){
	const deRig &rig = *(static_cast<sRigNatDat*>(p_GetNativeData(myself))->rig);
	const deScriptingDragonScript &ds = *(((deClassRig*)GetOwnerClass())->GetDS());
	const int bone = rt->GetValue(0)->GetInt();
	
	ds.GetClassMatrix()->PushMatrix(rt, rig.GetBoneAt(bone).GetMatrix());
}

// public func Matrix getBoneInverseMatrix( int bone )
deClassRig::nfBoneGetInverseMatrix::nfBoneGetInverseMatrix(const sInitData &init) : dsFunction(init.clsRig,
"getBoneInverseMatrix", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsMat){
	p_AddParameter(init.clsInt); // bone
}
void deClassRig::nfBoneGetInverseMatrix::RunFunction(dsRunTime *rt, dsValue *myself){
	const deRig &rig = *(static_cast<sRigNatDat*>(p_GetNativeData(myself))->rig);
	const deScriptingDragonScript &ds = *(((deClassRig*)GetOwnerClass())->GetDS());
	const int bone = rt->GetValue(0)->GetInt();
	
	ds.GetClassMatrix()->PushMatrix(rt, rig.GetBoneAt(bone).GetInverseMatrix());
}

// public func Vector boneGetCentralMassPoint( int bone )
deClassRig::nfBoneGetCentralMassPoint::nfBoneGetCentralMassPoint(const sInitData &init) : dsFunction(init.clsRig,
"boneGetCentralMassPoint", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
	p_AddParameter(init.clsInt); // bone
}
void deClassRig::nfBoneGetCentralMassPoint::RunFunction(dsRunTime *rt, dsValue *myself){
	const deRig &rig = *(static_cast<sRigNatDat*>(p_GetNativeData(myself))->rig);
	const deScriptingDragonScript &ds = *(((deClassRig*)GetOwnerClass())->GetDS());
	const int bone = rt->GetValue(0)->GetInt();
	
	ds.GetClassVector()->PushVector(rt, rig.GetBoneAt(bone).GetCentralMassPoint());
}

// public func float boneGetMass( int bone )
deClassRig::nfBoneGetMass::nfBoneGetMass(const sInitData &init) : dsFunction(init.clsRig,
"boneGetMass", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
	p_AddParameter(init.clsInt); // bone
}
void deClassRig::nfBoneGetMass::RunFunction(dsRunTime *rt, dsValue *myself){
	const deRig &rig = *(static_cast<sRigNatDat*>(p_GetNativeData(myself))->rig);
	const int bone = rt->GetValue(0)->GetInt();
	
	rt->PushFloat(rig.GetBoneAt(bone).GetMass());
}

// public func bool boneGetDynamic( int bone )
deClassRig::nfBoneGetDynamic::nfBoneGetDynamic(const sInitData &init) : dsFunction(init.clsRig,
"boneGetDynamic", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsInt); // bone
}
void deClassRig::nfBoneGetDynamic::RunFunction(dsRunTime *rt, dsValue *myself){
	const deRig &rig = *(static_cast<sRigNatDat*>(p_GetNativeData(myself))->rig);
	const int bone = rt->GetValue(0)->GetInt();
	
	rt->PushBool(rig.GetBoneAt(bone).GetDynamic());
}

// public func Vector boneGetIKLimitsLower( int bone )
deClassRig::nfBoneGetIKLimitsLower::nfBoneGetIKLimitsLower(const sInitData &init) : dsFunction(init.clsRig,
"boneGetIKLimitsLower", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
	p_AddParameter(init.clsInt); // bone
}
void deClassRig::nfBoneGetIKLimitsLower::RunFunction(dsRunTime *rt, dsValue *myself){
	const deRig &rig = *(static_cast<sRigNatDat*>(p_GetNativeData(myself))->rig);
	const deScriptingDragonScript &ds = *(((deClassRig*)GetOwnerClass())->GetDS());
	const int bone = rt->GetValue(0)->GetInt();
	
	ds.GetClassVector()->PushVector(rt, rig.GetBoneAt(bone).GetIKLimitsLower() * RAD2DEG);
}

// public func Vector boneGetIKLimitsUpper( int bone )
deClassRig::nfBoneGetIKLimitsUpper::nfBoneGetIKLimitsUpper(const sInitData &init) : dsFunction(init.clsRig,
"boneGetIKLimitsUpper", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
	p_AddParameter(init.clsInt); // bone
}
void deClassRig::nfBoneGetIKLimitsUpper::RunFunction(dsRunTime *rt, dsValue *myself){
	const deRig &rig = *(static_cast<sRigNatDat*>(p_GetNativeData(myself))->rig);
	const deScriptingDragonScript &ds = *(((deClassRig*)GetOwnerClass())->GetDS());
	const int bone = rt->GetValue(0)->GetInt();
	
	ds.GetClassVector()->PushVector(rt, rig.GetBoneAt(bone).GetIKLimitsUpper() * RAD2DEG);
}

// public func Vector boneGetIKResistance( int bone )
deClassRig::nfBoneGetIKResistance::nfBoneGetIKResistance(const sInitData &init) : dsFunction(init.clsRig,
"boneGetIKResistance", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
	p_AddParameter(init.clsInt); // bone
}
void deClassRig::nfBoneGetIKResistance::RunFunction(dsRunTime *rt, dsValue *myself){
	const deRig &rig = *(static_cast<sRigNatDat*>(p_GetNativeData(myself))->rig);
	const deScriptingDragonScript &ds = *(((deClassRig*)GetOwnerClass())->GetDS());
	const int bone = rt->GetValue(0)->GetInt();
	
	ds.GetClassVector()->PushVector(rt, rig.GetBoneAt(bone).GetIKResistance());
}

// public func bool boneGetIKLocked( int bone, int axis )
deClassRig::nfBoneGetIKLocked::nfBoneGetIKLocked(const sInitData &init) : dsFunction(init.clsRig,
"boneGetIKLocked", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsInt); // bone
	p_AddParameter(init.clsInt); // axis
}
void deClassRig::nfBoneGetIKLocked::RunFunction(dsRunTime *rt, dsValue *myself){
	const deRig &rig = *(static_cast<sRigNatDat*>(p_GetNativeData(myself))->rig);
	const int bone = rt->GetValue(0)->GetInt();
	const int axis = rt->GetValue(1)->GetInt();
	
	if(axis == 0){
		rt->PushBool(rig.GetBoneAt(bone).GetIKLockedX());
		
	}else if(axis == 1){
		rt->PushBool(rig.GetBoneAt(bone).GetIKLockedY());
		
	}else if(axis == 2){
		rt->PushBool(rig.GetBoneAt(bone).GetIKLockedZ());
		
	}else{
		DSTHROW(dueInvalidParam);
	}
}

// public func int boneGetConstraintCount( int bone )
deClassRig::nfBoneGetConstraintCount::nfBoneGetConstraintCount(const sInitData &init) : dsFunction(init.clsRig,
"boneGetConstraintCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // bone
}
void deClassRig::nfBoneGetConstraintCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deRig &rig = *(static_cast<sRigNatDat*>(p_GetNativeData(myself))->rig);
	const int bone = rt->GetValue(0)->GetInt();
	
	rt->PushInt(rig.GetBoneAt(bone).GetConstraintCount());
}

// public func ColliderConstraint boneGetConstraintAt( int bone, int constraint )
deClassRig::nfBoneGetConstraintAt::nfBoneGetConstraintAt(const sInitData &init) : dsFunction(init.clsRig,
"boneGetConstraintAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsCCon){
	p_AddParameter(init.clsInt); // bone
	p_AddParameter(init.clsInt); // constraint
}
void deClassRig::nfBoneGetConstraintAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deRig &rig = *(static_cast<sRigNatDat*>(p_GetNativeData(myself))->rig);
	const deScriptingDragonScript &ds = *(((deClassRig*)GetOwnerClass())->GetDS());
	const int bone = rt->GetValue(0)->GetInt();
	const int constraint = rt->GetValue(1)->GetInt();
	
	const deRigBone &rigBone = rig.GetBoneAt(bone);
	const deRigConstraint &rigConstraint = rigBone.GetConstraintAt(constraint);
	decMatrix bcMatrix, bcRotMatrix;
	
	deColliderConstraint *bc = NULL;
	try{
		bc = new deColliderConstraint;
		
		bcRotMatrix.SetFromQuaternion(rigConstraint.GetReferenceOrientation());
		bc->SetPosition1(rigConstraint.GetReferencePosition() + bcRotMatrix * rigConstraint.GetBoneOffset());
		bc->SetOrientation1(decQuaternion()/*rigConstraint.GetReferenceOrientation()*/);
		
		bcMatrix.SetRT(rigBone.GetRotation(), rigBone.GetPosition());
		
		bc->SetPosition2(bcMatrix * rigConstraint.GetReferencePosition());
		bc->SetOrientation2((bcRotMatrix * bcMatrix).ToQuaternion());
		
		bc->GetDofLinearX() = rigConstraint.GetDofLinearX();
		bc->GetDofLinearY() = rigConstraint.GetDofLinearY();
		bc->GetDofLinearZ() = rigConstraint.GetDofLinearZ();
		bc->GetDofAngularX() = rigConstraint.GetDofAngularX();
		bc->GetDofAngularY() = rigConstraint.GetDofAngularY();
		bc->GetDofAngularZ() = rigConstraint.GetDofAngularZ();
		
		bc->SetLinearDamping(rigConstraint.GetLinearDamping());
		bc->SetAngularDamping(rigConstraint.GetAngularDamping());
		bc->SetSpringDamping(rigConstraint.GetSpringDamping());
		
		bc->SetIsRope(rigConstraint.GetIsRope());
		bc->SetBreakingThreshold(rigConstraint.GetBreakingThreshold());
		
		bc->SetBone(rigConstraint.GetParentBone());
		
		ds.GetClassColliderConstraint()->PushConstraint(rt, bc);
		
	}catch(...){
		if(bc){
			delete bc;
		}
		throw;
	}
}

// public func Vector boneConstraintGetReferencePosition(int bone, int constraint)
deClassRig::nfBoneConstraintGetReferencePosition::nfBoneConstraintGetReferencePosition(const sInitData &init) :
dsFunction(init.clsRig, "boneConstraintGetReferencePosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
	p_AddParameter(init.clsInt); // bone
	p_AddParameter(init.clsInt); // constraint
}
void deClassRig::nfBoneConstraintGetReferencePosition::RunFunction(dsRunTime *rt, dsValue *myself){
	const deRig &rig = *(static_cast<sRigNatDat*>(p_GetNativeData(myself))->rig);
	const deScriptingDragonScript &ds = *(((deClassRig*)GetOwnerClass())->GetDS());
	const int bone = rt->GetValue(0)->GetInt();
	const int constraint = rt->GetValue(1)->GetInt();
	
	ds.GetClassVector()->PushVector(rt, rig.GetBoneAt(bone).GetConstraintAt(constraint).GetReferencePosition());
}

// public func Quaternion boneConstraintGetReferenceOrientation(int bone, int constraint)
deClassRig::nfBoneConstraintGetReferenceOrientation::nfBoneConstraintGetReferenceOrientation(const sInitData &init) :
dsFunction(init.clsRig, "boneConstraintGetReferenceOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat){
	p_AddParameter(init.clsInt); // bone
	p_AddParameter(init.clsInt); // constraint
}
void deClassRig::nfBoneConstraintGetReferenceOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	const deRig &rig = *(static_cast<sRigNatDat*>(p_GetNativeData(myself))->rig);
	const deScriptingDragonScript &ds = *(((deClassRig*)GetOwnerClass())->GetDS());
	const int bone = rt->GetValue(0)->GetInt();
	const int constraint = rt->GetValue(1)->GetInt();
	
	ds.GetClassQuaternion()->PushQuaternion(rt, rig.GetBoneAt(bone).GetConstraintAt(constraint).GetReferenceOrientation());
}

// public func Vector boneConstraintGetBoneOffset(int bone, int constraint)
deClassRig::nfBoneConstraintGetBoneOffset::nfBoneConstraintGetBoneOffset(const sInitData &init) :
dsFunction(init.clsRig, "boneConstraintGetBoneOffset", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
	p_AddParameter(init.clsInt); // bone
	p_AddParameter(init.clsInt); // constraint
}
void deClassRig::nfBoneConstraintGetBoneOffset::RunFunction(dsRunTime *rt, dsValue *myself){
	const deRig &rig = *(static_cast<sRigNatDat*>(p_GetNativeData(myself))->rig);
	const deScriptingDragonScript &ds = *(((deClassRig*)GetOwnerClass())->GetDS());
	const int bone = rt->GetValue(0)->GetInt();
	const int constraint = rt->GetValue(1)->GetInt();
	
	ds.GetClassVector()->PushVector(rt, rig.GetBoneAt(bone).GetConstraintAt(constraint).GetBoneOffset());
}

// public func ShapeList boneGetShapes( int bone )
deClassRig::nfBoneGetShapes::nfBoneGetShapes(const sInitData &init) : dsFunction(init.clsRig,
"boneGetShapes", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsShapeList){
	p_AddParameter(init.clsInt); // bone
}
void deClassRig::nfBoneGetShapes::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = *(((deClassRig*)GetOwnerClass())->GetDS());
	const deRig &rig = *(static_cast<sRigNatDat*>(p_GetNativeData(myself))->rig);
	const int bone = rt->GetValue(0)->GetInt();
	
	ds.GetClassShapeList()->PushShapeList(rt, rig.GetBoneAt(bone).GetShapes());
}

// public func String boneShapeGetProperty( int bone, int shape )
deClassRig::nfBoneShapeGetProperty::nfBoneShapeGetProperty(const sInitData &init) : dsFunction(init.clsRig,
"boneShapeGetProperty", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
	p_AddParameter(init.clsInt); // bone
	p_AddParameter(init.clsInt); // shape
}
void deClassRig::nfBoneShapeGetProperty::RunFunction(dsRunTime *rt, dsValue *myself){
	const deRig &rig = *(static_cast<sRigNatDat*>(p_GetNativeData(myself))->rig);
	const int shape = rt->GetValue(1)->GetInt();
	const int bone = rt->GetValue(0)->GetInt();
	
	rt->PushString(rig.GetBoneAt(bone).GetShapeProperties().GetAt(shape));
}

// public func int boneGetParent( int bone )
deClassRig::nfBoneGetParent::nfBoneGetParent(const sInitData &init) : dsFunction(init.clsRig,
"boneGetParent", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // bone
}
void deClassRig::nfBoneGetParent::RunFunction(dsRunTime *rt, dsValue *myself){
	const deRig &rig = *(static_cast<sRigNatDat*>(p_GetNativeData(myself))->rig);
	const int bone = rt->GetValue(0)->GetInt();
	
	rt->PushInt(rig.GetBoneAt(bone).GetParent());
}

// public func int getRootBone()
deClassRig::nfGetRootBone::nfGetRootBone(const sInitData &init) : dsFunction(init.clsRig,
"getRootBone", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassRig::nfGetRootBone::RunFunction(dsRunTime *rt, dsValue *myself){
	const deRig &rig = *(static_cast<sRigNatDat*>(p_GetNativeData(myself))->rig);
	rt->PushInt(rig.GetRootBone());
}



// public func ShapeList getShapes()
deClassRig::nfGetShapes::nfGetShapes(const sInitData &init) : dsFunction(init.clsRig,
"getShapes", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsShapeList){
}
void deClassRig::nfGetShapes::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = *(((deClassRig*)GetOwnerClass())->GetDS());
	const deRig &rig = *(static_cast<sRigNatDat*>(p_GetNativeData(myself))->rig);
	
	ds.GetClassShapeList()->PushShapeList(rt, rig.GetShapes());
}

// public func String shapeGetProperty( int shape )
deClassRig::nfShapeGetProperty::nfShapeGetProperty(const sInitData &init) : dsFunction(init.clsRig,
"shapeGetProperty", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
	p_AddParameter(init.clsInt); // shape
}
void deClassRig::nfShapeGetProperty::RunFunction(dsRunTime *rt, dsValue *myself){
	const deRig &rig = *(static_cast<sRigNatDat*>(p_GetNativeData(myself))->rig);
	const int shape = rt->GetValue(0)->GetInt();
	
	rt->PushString(rig.GetShapeProperties().GetAt(shape));
}

// public func Vector getCentralMassPoint()
deClassRig::nfGetCentralMassPoint::nfGetCentralMassPoint(const sInitData &init) :
dsFunction(init.clsRig, "getCentralMassPoint", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
}
void deClassRig::nfGetCentralMassPoint::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = *(((deClassRig*)GetOwnerClass())->GetDS());
	const deRig &rig = *(static_cast<sRigNatDat*>(p_GetNativeData(myself))->rig);
	
	ds.GetClassVector()->PushVector(rt, rig.GetCentralMassPoint());
}



// public func void save( String filename )
deClassRig::nfSave::nfSave(const sInitData &init) : dsFunction(init.clsRig,
"save", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsStr); // filename
}
void deClassRig::nfSave::RunFunction(dsRunTime *rt, dsValue *myself){
	const deRig &rig = *(static_cast<sRigNatDat*>(p_GetNativeData(myself))->rig);
	deScriptingDragonScript &ds = *(((deClassRig*)GetOwnerClass())->GetDS());
	const char * const filename = rt->GetValue(0)->GetString();
	const deEngine &engine = *ds.GetGameEngine();
	
	deBaseRigModule * const module = (deBaseRigModule*)engine.GetModuleSystem()->
		GetModuleAbleToLoad(deModuleSystem::emtRig, filename);
	if(!module){
		DSTHROW_INFO(dueInvalidParam, "no module found to handle filename");
	}
	
	module->SaveRig(engine.GetRigManager()->OpenFileForWriting(
		*engine.GetVirtualFileSystem(), filename), rig);
}



// public func int hashCode()
deClassRig::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsRig, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}

void deClassRig::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	const deRig * const rig = static_cast<sRigNatDat*>(p_GetNativeData(myself))->rig;
	
	// hash code = memory location
	rt->PushInt((int)(intptr_t)rig);
}

// public func bool equals( Object obj )
deClassRig::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsRig, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // obj
}
void deClassRig::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	const deRig * const rig = static_cast<sRigNatDat*>(p_GetNativeData(myself))->rig;
	deClassRig * const clsRig = (deClassRig*)GetOwnerClass();
	dsValue * const obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsRig)){
		rt->PushBool(false);
		
	}else{
		const deRig * const rigOther = static_cast<sRigNatDat*>(p_GetNativeData(obj))->rig;
		rt->PushBool(rig == rigOther);
	}
}




// Class deClassRig
/////////////////////

// Constructor, Destructor
////////////////////////////

deClassRig::deClassRig(deScriptingDragonScript *ds) :
dsClass("Rig", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED){
	if(!ds){
		DSTHROW(dueInvalidParam);
	}
	
	pDS = ds;
	
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sRigNatDat));
}

deClassRig::~deClassRig(){
}


// Management
///////////////

void deClassRig::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	init.clsRig = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsObj = engine->GetClassObject();
	init.clsRN = pDS->GetClassResourceListener();
	init.clsVec = pDS->GetClassVector();
	init.clsQuat = pDS->GetClassQuaternion();
	init.clsCCon = pDS->GetClassColliderConstraint();
	init.clsShapeList = pDS->GetClassShapeList();
	init.clsMat = pDS->GetClassMatrix();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfLoadAsynchron(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetFilename(init));
	
	AddFunction(new nfGetBoneCount(init));
	AddFunction(new nfIndexOfBoneNamed(init));
	AddFunction(new nfBoneGetName(init));
	AddFunction(new nfBoneGetPosition(init));
	AddFunction(new nfBoneGetOrientation(init));
	AddFunction(new nfBoneGetRotation(init));
	AddFunction(new nfBoneGetMatrix(init));
	AddFunction(new nfBoneGetInverseMatrix(init));
	AddFunction(new nfBoneGetCentralMassPoint(init));
	AddFunction(new nfBoneGetMass(init));
	AddFunction(new nfBoneGetDynamic(init));
	AddFunction(new nfBoneGetIKLimitsLower(init));
	AddFunction(new nfBoneGetIKLimitsUpper(init));
	AddFunction(new nfBoneGetIKResistance(init));
	AddFunction(new nfBoneGetIKLocked(init));
	AddFunction(new nfBoneGetConstraintCount(init));
	AddFunction(new nfBoneGetConstraintAt(init));
	AddFunction(new nfBoneConstraintGetReferencePosition(init));
	AddFunction(new nfBoneConstraintGetReferenceOrientation(init));
	AddFunction(new nfBoneConstraintGetBoneOffset(init));
	AddFunction(new nfBoneGetShapes(init));
	AddFunction(new nfBoneShapeGetProperty(init));
	AddFunction(new nfBoneGetParent(init));
	
	AddFunction(new nfGetRootBone(init));
	AddFunction(new nfGetShapes(init));
	AddFunction(new nfShapeGetProperty(init));
	AddFunction(new nfGetCentralMassPoint(init));
	
	AddFunction(new nfSave(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
}

deRig *deClassRig::GetRig(dsRealObject *myself) const{
	if(!myself){
		return NULL;
	}
	
	return static_cast<sRigNatDat*>(p_GetNativeData(myself->GetBuffer()))->rig;
}

void deClassRig::PushRig(dsRunTime *rt, deRig *rig){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!rig){
		rt->PushObject(NULL, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	static_cast<sRigNatDat*>(p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()))->rig = rig;
	rig->AddReference();
}
