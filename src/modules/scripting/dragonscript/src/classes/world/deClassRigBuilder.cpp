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
#include "deClassRigBuilder.h"
#include "../math/deClassVector.h"
#include "../math/deClassQuaternion.h"
#include "../utils/deClassShapeList.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/rig/deRigBuilder.h>
#include <dragengine/resources/rig/deRigConstraint.h>
#include <dragengine/resources/rig/deRigManager.h>

#include <libdscript/packages/default/dsClassArray.h>
#include <libdscript/packages/default/dsClassEnumeration.h>


class deClassRigBuilder_Builder : public deRigBuilder{
	dsRunTime *pRT;
	dsValue *pMyself;
	deRig *pRig;
	
public:
	deClassRigBuilder_Builder(dsRunTime *rt, dsValue *myself) :
	pRT(rt), pMyself(myself), pRig(NULL){
	}
	
	virtual void BuildRig(deRig *rig){
		pRig = rig;
		
		try{
			pRT->RunFunction(pMyself, "buildRig", 0);
			
		}catch(const duException &e){
			pRig = NULL;
			pRT->PrintExceptionTrace();
			e.PrintError();
			DETHROW(deeInvalidParam);
			
		}catch(...){
			pRig = NULL;
			throw;
		}
		
		pRig = NULL;
	}
	
	inline deRig *GetRig() const{ return pRig; }
};


struct sMdlBldNatDat{
	deClassRigBuilder_Builder *builder;
};



// Native Functions
/////////////////////

// public constructor new()
deClassRigBuilder::nfNew::nfNew(const sInitData &init) :
dsFunction(init.clsRigBuilder, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassRigBuilder::nfNew::RunFunction(dsRunTime*, dsValue *myself){
	static_cast<sMdlBldNatDat*>(p_GetNativeData(myself))->builder = NULL;
}

// public destructor Destructor()
deClassRigBuilder::nfDestructor::nfDestructor(const sInitData &init) :
dsFunction(init.clsRigBuilder, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassRigBuilder::nfDestructor::RunFunction(dsRunTime*, dsValue*){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sMdlBldNatDat*>(p_GetNativeData(myself))->~sMdlBldNatDat();
}



// public func Rig build( String filename )
deClassRigBuilder::nfBuild::nfBuild(const sInitData &init) :
dsFunction(init.clsRigBuilder, "build", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsRig){
	p_AddParameter(init.clsString); // filename
}
void deClassRigBuilder::nfBuild::RunFunction(dsRunTime *rt, dsValue *myself){
	sMdlBldNatDat &nd = *static_cast<sMdlBldNatDat*>(p_GetNativeData(myself));
	if(nd.builder){
		DSTHROW(dueInvalidAction);
	}
	
	const deScriptingDragonScript &ds = ((deClassRigBuilder*)GetOwnerClass())->GetDS();
	const char * const filename = rt->GetValue(0)->GetString();
	deClassRigBuilder_Builder builder(rt, myself);
	deRig::Ref rig;
	
	nd.builder = &builder;
	
	try{
		rig = ds.GetGameEngine()->GetRigManager()->CreateRig(filename, builder);
		
	}catch(...){
		nd.builder = NULL;
		throw;
	}
	
	nd.builder = NULL;
	ds.GetClassRig()->PushRig(rt, rig);
}



// abstract protected func void buildRig()
deClassRigBuilder::nfBuildRig::nfBuildRig(const sInitData &init) :
dsFunction(init.clsRigBuilder, "buildRig", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE | DSTM_ABSTRACT, init.clsVoid){
}
void deClassRigBuilder::nfBuildRig::RunFunction(dsRunTime*, dsValue*){
}



// protected func void setCentralMassPoint( Vector cmp )
deClassRigBuilder::nfSetCentralMassPoint::nfSetCentralMassPoint(const sInitData &init) :
dsFunction(init.clsRigBuilder, "setCentralMassPoint", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVector); // cmp
}
void deClassRigBuilder::nfSetCentralMassPoint::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassRigBuilder_Builder * const builder = static_cast<sMdlBldNatDat*>(p_GetNativeData(myself))->builder;
	if(!builder || !builder->GetRig()){
		DSTHROW(dueInvalidAction);
	}
	
	const deScriptingDragonScript &ds = ((deClassRigBuilder*)GetOwnerClass())->GetDS();
	builder->GetRig()->SetCentralMassPoint(ds.GetClassVector()->GetVector(rt->GetValue(0)->GetRealObject()));
}

// protected func void setModelCollision( bool modelCollision )
deClassRigBuilder::nfSetModelCollision::nfSetModelCollision(const sInitData &init) :
dsFunction(init.clsRigBuilder, "setModelCollision", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBoolean); // modelCollision
}
void deClassRigBuilder::nfSetModelCollision::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassRigBuilder_Builder * const builder = static_cast<sMdlBldNatDat*>(p_GetNativeData(myself))->builder;
	if(!builder || !builder->GetRig()){
		DSTHROW(dueInvalidAction);
	}
	
	builder->GetRig()->SetModelCollision(rt->GetValue(0)->GetBool());
}

// protected func void addBone( String name, int parent, Vector position, Vector rotation,
// Vector centralMassPoint, bool dynamic, float mass, Vector ikLimitsLower, Vector ikLimitsUpper,
// Vector ikResistance, bool ikLockedX, bool ikLockedY, bool ikLockedZ, ShapeList shapes )
deClassRigBuilder::nfAddBone::nfAddBone(const sInitData &init) :
dsFunction(init.clsRigBuilder, "addBone", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // name
	p_AddParameter(init.clsInteger); // parent
	p_AddParameter(init.clsVector); // position
	p_AddParameter(init.clsVector); // rotation
	p_AddParameter(init.clsVector); // centralMassPoint
	p_AddParameter(init.clsBoolean); // dynamic
	p_AddParameter(init.clsFloat); // mass
	p_AddParameter(init.clsVector); // ikLimitsLower
	p_AddParameter(init.clsVector); // ikLimitsUpper
	p_AddParameter(init.clsVector); // ikResistance
	p_AddParameter(init.clsBoolean); // ikLockedX
	p_AddParameter(init.clsBoolean); // ikLockedY
	p_AddParameter(init.clsBoolean); // ikLockedZ
	p_AddParameter(init.clsShapeList); // shapes
}
void deClassRigBuilder::nfAddBone::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassRigBuilder_Builder * const builder = static_cast<sMdlBldNatDat*>(p_GetNativeData(myself))->builder;
	if(!builder || !builder->GetRig()){
		DSTHROW(dueInvalidAction);
	}
	
	const deScriptingDragonScript &ds = ((deClassRigBuilder*)GetOwnerClass())->GetDS();
	const char * const name = rt->GetValue(0)->GetString();
	const int parent = rt->GetValue(1)->GetInt();
	const decVector &position = ds.GetClassVector()->GetVector(rt->GetValue(2)->GetRealObject());
	const decVector &rotation = ds.GetClassVector()->GetVector(rt->GetValue(3)->GetRealObject());
	const decVector &centralMassPoint = ds.GetClassVector()->GetVector(rt->GetValue(4)->GetRealObject());
	const bool dynamic = rt->GetValue(5)->GetBool();
	const float mass = rt->GetValue(6)->GetFloat();
	const decVector &ikLimitsLower = ds.GetClassVector()->GetVector(rt->GetValue(7)->GetRealObject());
	const decVector &ikLimitsUpper = ds.GetClassVector()->GetVector(rt->GetValue(8)->GetRealObject());
	const decVector &ikResistance = ds.GetClassVector()->GetVector(rt->GetValue(9)->GetRealObject());
	const bool ikLockedX = rt->GetValue(10)->GetBool();
	const bool ikLockedY = rt->GetValue(11)->GetBool();
	const bool ikLockedZ = rt->GetValue(12)->GetBool();
	const decShapeList &shapes = ds.GetClassShapeList()->GetShapeList(rt->GetValue(13)->GetRealObject());
	
	deRigBone * const bone = new deRigBone(name);
	try{
		bone->SetParent(parent);
		bone->SetPosition(position);
		bone->SetRotation(rotation * DEG2RAD);
		bone->SetCentralMassPoint(centralMassPoint);
		bone->SetDynamic(dynamic);
		bone->SetMass(mass);
		bone->SetIKLimits(ikLimitsLower * DEG2RAD, ikLimitsUpper * DEG2RAD);
		bone->SetIKResistance(ikResistance);
		bone->SetIKLockedX(ikLockedX);
		bone->SetIKLockedY(ikLockedY);
		bone->SetIKLockedZ(ikLockedZ);
		bone->SetShapes(shapes);
		builder->GetRig()->AddBone(bone);
		
	}catch(...){
		delete bone;
		throw;
	}
}

// protected func void setBoneShapeProperties( int index, Array properties )
deClassRigBuilder::nfSetBoneShapeProperties::nfSetBoneShapeProperties(const sInitData &init) :
dsFunction(init.clsRigBuilder, "setBoneShapeProperties", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // index
	p_AddParameter(init.clsArray); // properties
}
void deClassRigBuilder::nfSetBoneShapeProperties::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassRigBuilder_Builder * const builder = static_cast<sMdlBldNatDat*>(p_GetNativeData(myself))->builder;
	if(!builder || !builder->GetRig()){
		DSTHROW(dueInvalidAction);
	}
	
	deRigBone &bone = builder->GetRig()->GetBoneAt(rt->GetValue(0)->GetInt());
	
	const deScriptingDragonScript &ds = ((deClassRigBuilder*)GetOwnerClass())->GetDS();
	dsClassArray &clsArray = *((dsClassArray*)ds.GetScriptEngine()->GetClassArray());
	dsRealObject * const objProperties = rt->GetValue(1)->GetRealObject();
	const int count = clsArray.GetObjectCount(rt, objProperties);
	if(count != bone.GetShapeProperties().GetCount()){
		DSTHROW_INFO(dueInvalidParam, "properties.getCount() != bone.shapeCount");
	}
	
	decStringList properties;
	int i;
	for(i=0; i<count; i++){
		dsValue * const value = clsArray.GetObjectAt(rt, objProperties, i);
		rt->RunFunction(value, "toString", 0);
		properties.Add(rt->GetReturnString());
	}
	bone.SetShapeProperties(properties);
}

// protected func void addBoneConstraint( int bone, Vector referencePosition,
// Quaternion referenceOrientation, Vector boneOffset, float linearDamping,
// float angularDamping, float springDamping, bool isRope,
// float breakingThreshold, int parentBone )
deClassRigBuilder::nfAddBoneConstraint::nfAddBoneConstraint(const sInitData &init) :
dsFunction(init.clsRigBuilder, "addBoneConstraint", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // bone
	p_AddParameter(init.clsVector); // referencePosition
	p_AddParameter(init.clsQuaternion); // referenceOrientation
	p_AddParameter(init.clsVector); // boneOffset
	p_AddParameter(init.clsFloat); // linearDamping
	p_AddParameter(init.clsFloat); // angularDamping
	p_AddParameter(init.clsFloat); // springDamping
	p_AddParameter(init.clsBoolean); // isRope
	p_AddParameter(init.clsFloat); // breakingThreshold
	p_AddParameter(init.clsInteger); // parentBone
}
void deClassRigBuilder::nfAddBoneConstraint::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassRigBuilder_Builder * const builder = static_cast<sMdlBldNatDat*>(p_GetNativeData(myself))->builder;
	if(!builder || !builder->GetRig()){
		DSTHROW(dueInvalidAction);
	}
	
	const deScriptingDragonScript &ds = ((deClassRigBuilder*)GetOwnerClass())->GetDS();
	deRigBone &bone = builder->GetRig()->GetBoneAt(rt->GetValue(0)->GetInt());
	const decVector &referencePosition = ds.GetClassVector()->GetVector(rt->GetValue(1)->GetRealObject());
	const decQuaternion &referenceOrientation = ds.GetClassQuaternion()->GetQuaternion(rt->GetValue(2)->GetRealObject());
	const decVector &boneOffset = ds.GetClassVector()->GetVector(rt->GetValue(3)->GetRealObject());
	const float linearDamping = rt->GetValue(4)->GetFloat();
	const float angularDamping = rt->GetValue(5)->GetFloat();
	const float springDamping = rt->GetValue(6)->GetFloat();
	const bool isRope = rt->GetValue(7)->GetBool();
	const float breakingThreshold = rt->GetValue(8)->GetFloat();
	const int parentBone = rt->GetValue(9)->GetInt();
	
	deRigConstraint * const constraint = new deRigConstraint;
	try{
		constraint->SetReferencePosition(referencePosition);
		constraint->SetReferenceOrientation(referenceOrientation);
		constraint->SetBoneOffset(boneOffset);
		constraint->SetLinearDamping(linearDamping);
		constraint->SetAngularDamping(angularDamping);
		constraint->SetSpringDamping(springDamping);
		constraint->SetIsRope(isRope);
		constraint->SetBreakingThreshold(breakingThreshold);
		constraint->SetParentBone(parentBone);
		bone.AddConstraint(constraint);
		
	}catch(...){
		delete constraint;
		throw;
	}
}

// protected func void setBoneConstraintDof( int bone, int constraint, ColliderConstraintDof dof,
// float lowerLimit, float upperLimit, float staticFriction, float kinematicFriction, float springStiffness )
deClassRigBuilder::nfSetBoneConstraintDof::nfSetBoneConstraintDof(const sInitData &init) :
dsFunction(init.clsRigBuilder, "setBoneConstraintDof", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // bone
	p_AddParameter(init.clsInteger); // constraint
	p_AddParameter(init.clsColliderConstraintDof); // dof
	p_AddParameter(init.clsFloat); // lowerLimit
	p_AddParameter(init.clsFloat); // upperLimit
	p_AddParameter(init.clsFloat); // staticFriction
	p_AddParameter(init.clsFloat); // kinematicFriction
	p_AddParameter(init.clsFloat); // springStiffness
}
void deClassRigBuilder::nfSetBoneConstraintDof::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassRigBuilder_Builder * const builder = static_cast<sMdlBldNatDat*>(p_GetNativeData(myself))->builder;
	if(!builder || !builder->GetRig()){
		DSTHROW(dueInvalidAction);
	}
	if(!rt->GetValue(2)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	deRigBone &bone = builder->GetRig()->GetBoneAt(rt->GetValue(0)->GetInt());
	deRigConstraint &constraint = bone.GetConstraintAt(rt->GetValue(1)->GetInt());
	const deColliderConstraint::eDegreesOfFreedom dofEnum = (deColliderConstraint::eDegreesOfFreedom)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 2 )->GetRealObject() );
	deColliderConstraintDof &dof = constraint.GetDof(dofEnum);
	
	switch(dofEnum){
	case deColliderConstraint::edofLinearX:
	case deColliderConstraint::edofLinearY:
	case deColliderConstraint::edofLinearZ:
		dof.SetLowerLimit(rt->GetValue(3)->GetFloat());
		dof.SetUpperLimit(rt->GetValue(4)->GetFloat());
		break;
		
	case deColliderConstraint::edofAngularX:
	case deColliderConstraint::edofAngularY:
	case deColliderConstraint::edofAngularZ:
		dof.SetLowerLimit(rt->GetValue(3)->GetFloat() * DEG2RAD);
		dof.SetUpperLimit(rt->GetValue(4)->GetFloat() * DEG2RAD);
		break;
	}
	
	dof.SetStaticFriction(rt->GetValue(5)->GetFloat());
	dof.SetKinematicFriction(rt->GetValue(6)->GetFloat());
	dof.SetSpringStiffness(rt->GetValue(7)->GetFloat());
}

// protected func void setRootBone( int bone )
deClassRigBuilder::nfSetRootBone::nfSetRootBone(const sInitData &init) :
dsFunction(init.clsRigBuilder, "setRootBone", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // bone
}
void deClassRigBuilder::nfSetRootBone::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassRigBuilder_Builder * const builder = static_cast<sMdlBldNatDat*>(p_GetNativeData(myself))->builder;
	if(!builder || !builder->GetRig()){
		DSTHROW(dueInvalidAction);
	}
	
	builder->GetRig()->SetRootBone(rt->GetValue(0)->GetInt());
}

// protected func void setShapes( ShapeList shapes )
deClassRigBuilder::nfSetShapes::nfSetShapes(const sInitData &init) :
dsFunction(init.clsRigBuilder, "setShapes", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsShapeList); // shapes
}
void deClassRigBuilder::nfSetShapes::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassRigBuilder_Builder * const builder = static_cast<sMdlBldNatDat*>(p_GetNativeData(myself))->builder;
	if(!builder || !builder->GetRig()){
		DSTHROW(dueInvalidAction);
	}
	
	const deScriptingDragonScript &ds = ((deClassRigBuilder*)GetOwnerClass())->GetDS();
	builder->GetRig()->SetShapes(ds.GetClassShapeList()->GetShapeList(rt->GetValue(0)->GetRealObject()));
}

// protected func void setShapeProperties( Array properties )
deClassRigBuilder::nfSetShapeProperties::nfSetShapeProperties(const sInitData &init) :
dsFunction(init.clsRigBuilder, "setShapeProperties", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsArray); // properties
}
void deClassRigBuilder::nfSetShapeProperties::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassRigBuilder_Builder * const builder = static_cast<sMdlBldNatDat*>(p_GetNativeData(myself))->builder;
	if(!builder || !builder->GetRig()){
		DSTHROW(dueInvalidAction);
	}
	
	const deScriptingDragonScript &ds = ((deClassRigBuilder*)GetOwnerClass())->GetDS();
	dsClassArray &clsArray = *((dsClassArray*)ds.GetScriptEngine()->GetClassArray());
	dsRealObject * const objProperties = rt->GetValue(0)->GetRealObject();
	const int count = clsArray.GetObjectCount(rt, objProperties);
	if(count != builder->GetRig()->GetShapeProperties().GetCount()){
		DSTHROW_INFO(dueInvalidParam, "properties.getCount() != rig.shapeCount");
	}
	
	decStringList properties;
	int i;
	for(i=0; i<count; i++){
		dsValue * const value = clsArray.GetObjectAt(rt, objProperties, i);
		rt->RunFunction(value, "toString", 0);
		properties.Add(rt->GetReturnString());
	}
	builder->GetRig()->SetShapeProperties(properties);
}



// Class deClassRigBuilder
////////////////////////////

// Constructor, destructor
////////////////////////////

deClassRigBuilder::deClassRigBuilder(deScriptingDragonScript &ds) :
dsClass("RigBuilder", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_ABSTRACT),
pDS(ds)
{
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sMdlBldNatDat));
}

deClassRigBuilder::~deClassRigBuilder(){
}



// Management
///////////////

void deClassRigBuilder::CreateClassMembers(dsEngine *engine){
	pClsColliderConstraintDof = engine->GetClass("Dragengine.Scenery.ColliderConstraintDof");
	
	sInitData init;
	init.clsRigBuilder = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBoolean = engine->GetClassBool();
	init.clsString = engine->GetClassString();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsObject = engine->GetClassObject();
	init.clsRig = pDS.GetClassRig();
	init.clsVector = pDS.GetClassVector();
	init.clsQuaternion = pDS.GetClassQuaternion();
	init.clsShapeList = pDS.GetClassShapeList();
	init.clsColliderConstraintDof = pClsColliderConstraintDof;
	init.clsArray = engine->GetClassArray();
	
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfBuild(init));
	AddFunction(new nfBuildRig(init));
	AddFunction(new nfSetCentralMassPoint(init));
	AddFunction(new nfSetModelCollision(init));
	AddFunction(new nfAddBone(init));
	AddFunction(new nfSetBoneShapeProperties(init));
	AddFunction(new nfAddBoneConstraint(init));
	AddFunction(new nfSetBoneConstraintDof(init));
	AddFunction(new nfSetRootBone(init));
	AddFunction(new nfSetShapes(init));
	AddFunction(new nfSetShapeProperties(init));
}
