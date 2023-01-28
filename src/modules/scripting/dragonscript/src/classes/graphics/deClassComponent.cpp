/* 
 * Drag[en]gine DragonScript Script Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <libdscript/libdscript.h>

#include "deClassCamera.h"
#include "deClassColor.h"
#include "deClassComponent.h"
#include "deClassImage.h"
#include "deClassOcclusionMesh.h"
#include "../math/deClassDVector.h"
#include "../math/deClassMatrix.h"
#include "../math/deClassQuaternion.h"
#include "../math/deClassVector.h"
#include "../math/deClassTexMatrix2.h"
#include "../math/deClassDMatrix.h"
#include "../physics/deClassLayerMask.h"
#include "../world/deClassDecal.h"
#include "../world/deClassDynamicSkin.h"
#include "../world/deClassModel.h"
#include "../world/deClassRig.h"
#include "../world/deClassSkin.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentBone.h>
#include <dragengine/resources/component/deComponentManager.h>
#include <dragengine/resources/component/deComponentTexture.h>
#include <dragengine/resources/decal/deDecal.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelTexture.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkin.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/occlusionmesh/deOcclusionMesh.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassBlock.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// native structure
struct sCompNatDat{
	deComponent *component;
};

// native functions
/////////////////////

// public func new()
deClassComponent::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsCom,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassComponent::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sCompNatDat &nd = *( ( sCompNatDat* )p_GetNativeData( myself ) );
	deScriptingDragonScript &ds = *( ( ( deClassComponent* )GetOwnerClass() )->GetScriptModule() );
	
	// clear (important)
	nd.component = NULL;
	
	// create component
	nd.component = ds.GetGameEngine()->GetComponentManager()->CreateComponent();
}

// public func new( Model model, Skin skin )
deClassComponent::nfNew2::nfNew2( const sInitData &init ) : dsFunction( init.clsCom,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsMdl ); // model
	p_AddParameter( init.clsSkin ); // skin
}
void deClassComponent::nfNew2::RunFunction( dsRunTime *rt, dsValue *myself ){
	sCompNatDat &nd = *( ( sCompNatDat* )p_GetNativeData( myself ) );
	deScriptingDragonScript &ds = *( ( ( deClassComponent* )GetOwnerClass() )->GetScriptModule() );
	
	// clear (important)
	nd.component = NULL;
	
	// create component
	deModel * const model = ds.GetClassModel()->GetModel( rt->GetValue( 0 )->GetRealObject() );
	deSkin * const skin = ds.GetClassSkin()->GetSkin( rt->GetValue( 1 )->GetRealObject() );
	nd.component = ds.GetGameEngine()->GetComponentManager()->CreateComponent( model, skin );
}

// public func destructor()
deClassComponent::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsCom,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassComponent::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sCompNatDat *nd = ( sCompNatDat* )p_GetNativeData( myself );
	
	if( nd->component ){
		nd->component->FreeReference();
		nd->component = NULL;
	}
}

// public func Model getModel()
deClassComponent::nfGetModel::nfGetModel( const sInitData &init ) : dsFunction( init.clsCom,
"getModel", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsMdl ){
}
void deClassComponent::nfGetModel::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent *component = ( ( sCompNatDat* )p_GetNativeData( myself ) )->component;
	deClassComponent *clsComp = ( deClassComponent* )GetOwnerClass();
	clsComp->GetClassModel()->PushModel( rt, component->GetModel() );
}

// public func Rig getRig()
deClassComponent::nfGetRig::nfGetRig( const sInitData &init ) : dsFunction( init.clsCom,
"getRig", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsRig ){
}
void deClassComponent::nfGetRig::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent *component = ( ( sCompNatDat* )p_GetNativeData( myself ) )->component;
	deClassComponent *clsComp = ( deClassComponent* )GetOwnerClass();
	if( component->GetRig() ){
		clsComp->GetClassRig()->PushRig( rt, component->GetRig() );
	}else{
		rt->PushObject( NULL, clsComp->GetClassRig() );
	}
}

// public func Skin getSkin()
deClassComponent::nfGetSkin::nfGetSkin( const sInitData &init ) : dsFunction( init.clsCom,
"getSkin", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsSkin ){
}
void deClassComponent::nfGetSkin::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent *component = ( ( sCompNatDat* )p_GetNativeData( myself ) )->component;
	deClassComponent *clsComp = ( deClassComponent* )GetOwnerClass();
	clsComp->GetClassSkin()->PushSkin( rt, component->GetSkin() );
}

// public func DVector getPosition()
deClassComponent::nfGetPosition::nfGetPosition( const sInitData &init ) : dsFunction( init.clsCom,
"getPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec ){
}
void deClassComponent::nfGetPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent *component = ( ( sCompNatDat* )p_GetNativeData( myself ) )->component;
	deClassComponent *clsComp = ( deClassComponent* )GetOwnerClass();
	
	clsComp->GetClassDVector()->PushDVector( rt, component->GetPosition() );
}

// public func Quaternion getOrientation()
deClassComponent::nfGetOrientation::nfGetOrientation( const sInitData &init ) : dsFunction( init.clsCom,
"getOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat ){
}
void deClassComponent::nfGetOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent *component = ( ( sCompNatDat* )p_GetNativeData( myself ) )->component;
	deClassComponent *clsComp = ( deClassComponent* )GetOwnerClass();
	
	clsComp->GetClassQuaternion()->PushQuaternion( rt, component->GetOrientation() );
}

// public func Vector getScaling()
deClassComponent::nfGetScaling::nfGetScaling( const sInitData &init ) : dsFunction( init.clsCom,
"getScaling", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
}
void deClassComponent::nfGetScaling::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent *component = ( ( sCompNatDat* )p_GetNativeData( myself ) )->component;
	deClassComponent *clsComp = ( deClassComponent* )GetOwnerClass();
	clsComp->GetScriptModule()->PushVector( rt, component->GetScaling() );
}

// public func DMatrix getMatrix()
deClassComponent::nfGetMatrix::nfGetMatrix( const sInitData &init ) : dsFunction( init.clsCom,
"getMatrix", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDMat ){
}
void deClassComponent::nfGetMatrix::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	deClassDMatrix &clsDMatrix = *( ( ( deClassComponent* )GetOwnerClass() )->GetScriptModule()->GetClassDMatrix() );
	clsDMatrix.PushDMatrix( rt, component.GetMatrix() );
}

// public func DMatrix getInverseMatrix()
deClassComponent::nfGetInverseMatrix::nfGetInverseMatrix( const sInitData &init ) : dsFunction( init.clsCom,
"getInverseMatrix", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDMat ){
}
void deClassComponent::nfGetInverseMatrix::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	deClassDMatrix &clsDMatrix = *( ( ( deClassComponent* )GetOwnerClass() )->GetScriptModule()->GetClassDMatrix() );
	clsDMatrix.PushDMatrix( rt, component.GetInverseMatrix() );
}

// 	public func bool getVisible()
deClassComponent::nfGetVisible::nfGetVisible( const sInitData &init ) : dsFunction( init.clsCom,
"getVisible", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}
void deClassComponent::nfGetVisible::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent *component = ( ( sCompNatDat* )p_GetNativeData( myself ) )->component;
	rt->PushBool( component->GetVisible() );
}



// public func OcclusionMesh getOcclusionMesh()
deClassComponent::nfGetOcclusionMesh::nfGetOcclusionMesh( const sInitData &init ) : dsFunction( init.clsCom,
"getOcclusionMesh", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsOccM ){
}
void deClassComponent::nfGetOcclusionMesh::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	deClassOcclusionMesh &clsOccM = *( ( ( deClassComponent* )GetOwnerClass() )->GetScriptModule()->GetClassOcclusionMesh() );
	
	clsOccM.PushOcclusionMesh( rt, component.GetOcclusionMesh() );
}

// public func void setOcclusionMesh( OcclusionMesh occlusionMesh )
deClassComponent::nfSetOcclusionMesh::nfSetOcclusionMesh( const sInitData &init ) : dsFunction( init.clsCom,
"setOcclusionMesh", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsOccM ); // occlusionMesh
}
void deClassComponent::nfSetOcclusionMesh::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	deClassOcclusionMesh &clsOccM = *( ( ( deClassComponent* )GetOwnerClass() )->GetScriptModule()->GetClassOcclusionMesh() );
	
	component.SetOcclusionMesh( clsOccM.GetOcclusionMesh( rt->GetValue( 0 )->GetRealObject() ) );
}



// public func Model getAudioModel()
deClassComponent::nfGetAudioModel::nfGetAudioModel( const sInitData &init ) :
dsFunction( init.clsCom, "getAudioModel", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsMdl ){
}
void deClassComponent::nfGetAudioModel::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	deClassModel &clsModel = *( ( ( deClassComponent* )GetOwnerClass() )->GetScriptModule()->GetClassModel() );
	
	clsModel.PushModel( rt, component.GetAudioModel() );
}

// public func void setAudioModel( Model model )
deClassComponent::nfSetAudioModel::nfSetAudioModel( const sInitData &init ) :
dsFunction( init.clsCom, "setAudioModel", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsMdl ); // model
}
void deClassComponent::nfSetAudioModel::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	deClassModel &clsModel = *( ( ( deClassComponent* )GetOwnerClass() )->GetScriptModule()->GetClassModel() );
	
	component.SetAudioModel( clsModel.GetModel( rt->GetValue( 0 )->GetRealObject() ) );
}



// public func void setModel( Model model )
deClassComponent::nfSetModel::nfSetModel( const sInitData &init ) : dsFunction( init.clsCom,
"setModel", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsMdl ); // model
}
void deClassComponent::nfSetModel::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent *component = ( ( sCompNatDat* )p_GetNativeData( myself ) )->component;
	deClassComponent *clsComp = ( deClassComponent* )GetOwnerClass();
	
	component->SetModel( clsComp->GetClassModel()->GetModel( rt->GetValue( 0 )->GetRealObject() ) );
}

// public func void setModelKeepTextures( Model model )
deClassComponent::nfSetModelKeepTextures::nfSetModelKeepTextures( const sInitData &init ) : dsFunction( init.clsCom,
"setModelKeepTextures", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsMdl ); // model
}
void deClassComponent::nfSetModelKeepTextures::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	const deScriptingDragonScript &ds = *( ( ( deClassComponent* )GetOwnerClass() )->GetScriptModule() );
	
	component.SetModelKeepTextures( ds.GetClassModel()->GetModel( rt->GetValue( 0 )->GetRealObject() ) );
}

// public func void setSkin( Skin skin )
deClassComponent::nfSetSkin::nfSetSkin( const sInitData &init ) : dsFunction( init.clsCom,
"setSkin", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsSkin ); // skin
}
void deClassComponent::nfSetSkin::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent *component = ( ( sCompNatDat* )p_GetNativeData( myself ) )->component;
	deClassComponent *clsComp = ( deClassComponent* )GetOwnerClass();
	
	component->SetSkin( clsComp->GetClassSkin()->GetSkin( rt->GetValue( 0 )->GetRealObject() ) );
}

// public func void setRig( Rig rig )
deClassComponent::nfSetRig::nfSetRig( const sInitData &init ) : dsFunction( init.clsCom,
"setRig", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsRig ); // rig
}
void deClassComponent::nfSetRig::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent *component = ( ( sCompNatDat* )p_GetNativeData( myself ) )->component;
	deClassComponent *clsComp = ( deClassComponent* )GetOwnerClass();
	
	component->SetRig( clsComp->GetClassRig()->GetRig( rt->GetValue( 0 )->GetRealObject() ) );
}

// public func void setModelAndSkin( Model model, Skin skin )
deClassComponent::nfSetModelAndSkin::nfSetModelAndSkin( const sInitData &init ) : dsFunction( init.clsCom,
"setModelAndSkin", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsMdl ); // model
	p_AddParameter( init.clsSkin ); // skin
}
void deClassComponent::nfSetModelAndSkin::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent *component = ( ( sCompNatDat* )p_GetNativeData( myself ) )->component;
	deClassComponent *clsComp = ( deClassComponent* )GetOwnerClass();
	
	deModel * const model = clsComp->GetClassModel()->GetModel( rt->GetValue( 0 )->GetRealObject() );
	deSkin * const skin = clsComp->GetClassSkin()->GetSkin( rt->GetValue( 1 )->GetRealObject() );
	component->SetModelAndSkin( model, skin );
}

// public func void setPosition( DVector position )
deClassComponent::nfSetPosition::nfSetPosition( const sInitData &init ) : dsFunction( init.clsCom,
"setPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsDVec ); // position
}
void deClassComponent::nfSetPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent *component = ( ( sCompNatDat* )p_GetNativeData( myself ) )->component;
	deClassComponent *clsComp = ( deClassComponent* )GetOwnerClass();
	
	dsRealObject *obj = rt->GetValue( 0 )->GetRealObject();
	if( ! obj ) DSTHROW( dueNullPointer );
	
	component->SetPosition( clsComp->GetClassDVector()->GetDVector( obj ) );
}

// public func void setOrientation( Quaternion orientation )
deClassComponent::nfSetOrientation::nfSetOrientation( const sInitData &init ) : dsFunction( init.clsCom,
"setOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsQuat ); // orientation
}
void deClassComponent::nfSetOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent *component = ( ( sCompNatDat* )p_GetNativeData( myself ) )->component;
	deClassComponent *clsComp = ( deClassComponent* )GetOwnerClass();
	
	dsRealObject *obj = rt->GetValue( 0 )->GetRealObject();
	if( ! obj ) DSTHROW( dueNullPointer );
	
	component->SetOrientation( clsComp->GetClassQuaternion()->GetQuaternion( obj ) );
}

// public func void setScaling( Vector scaling )
deClassComponent::nfSetScaling::nfSetScaling( const sInitData &init ) : dsFunction( init.clsCom,
"setScaling", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsVec ); // scaling
}
void deClassComponent::nfSetScaling::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent *component = ( ( sCompNatDat* )p_GetNativeData( myself ) )->component;
	deClassComponent *clsComp = ( deClassComponent* )GetOwnerClass();
	dsRealObject *obj = rt->GetValue( 0 )->GetRealObject();
	if( ! obj ) DSTHROW( dueNullPointer );
	component->SetScaling( clsComp->GetScriptModule()->GetVector( obj ) );
}

// public func void setVisible( bool visible )
deClassComponent::nfSetVisible::nfSetVisible( const sInitData &init ) : dsFunction( init.clsCom,
"setVisible", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // visible
}
void deClassComponent::nfSetVisible::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent *component = ( ( sCompNatDat* )p_GetNativeData( myself ) )->component;
	component->SetVisible( rt->GetValue( 0 )->GetBool() );
}



// public func LayerMask getLayerMask()
deClassComponent::nfGetLayerMask::nfGetLayerMask( const sInitData &init ) : dsFunction( init.clsCom,
"getLayerMask", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsLayerMask ){
}
void deClassComponent::nfGetLayerMask::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	const deScriptingDragonScript &ds = *( ( deClassComponent* )GetOwnerClass() )->GetScriptModule();
	
	ds.GetClassLayerMask()->PushLayerMask( rt, component.GetLayerMask() );
}

// public func void setLayerMask( LayerMask layerMask )
deClassComponent::nfSetLayerMask::nfSetLayerMask( const sInitData &init ) : dsFunction( init.clsCom,
"setLayerMask", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsLayerMask ); // layerMask
}
void deClassComponent::nfSetLayerMask::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	const deScriptingDragonScript &ds = *( ( deClassComponent* )GetOwnerClass() )->GetScriptModule();
	
	component.SetLayerMask( ds.GetClassLayerMask()->GetLayerMask( rt->GetValue( 0 )->GetRealObject() ) );
}



// public func DynamicSkin getDynamicSkin()
deClassComponent::nfGetDynamicSkin::nfGetDynamicSkin( const sInitData &init ) : dsFunction( init.clsCom,
"getDynamicSkin", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDSkin ){
}
void deClassComponent::nfGetDynamicSkin::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	deClassComponent &clsComp = *( ( deClassComponent* )GetOwnerClass() );
	deClassDynamicSkin &clsDSkin = *clsComp.GetScriptModule()->GetClassDynamicSkin();
	
	clsDSkin.PushDynamicSkin( rt, component.GetDynamicSkin() );
}

// public func void setDynamicSkin( DynamicSkin dynamicSkin )
deClassComponent::nfSetDynamicSkin::nfSetDynamicSkin( const sInitData &init ) : dsFunction( init.clsCom,
"setDynamicSkin", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsDSkin ); // dynamicSkin
}
void deClassComponent::nfSetDynamicSkin::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	dsRealObject *objDSkin = rt->GetValue( 0 )->GetRealObject();
	deClassComponent &clsComp = *( ( deClassComponent* )GetOwnerClass() );
	deClassDynamicSkin &clsDSkin = *clsComp.GetScriptModule()->GetClassDynamicSkin();
	
	component.SetDynamicSkin( clsDSkin.GetDynamicSkin( objDSkin ) );
}




// public func int getBoneCount()
deClassComponent::nfGetBoneCount::nfGetBoneCount( const sInitData &init ) : dsFunction( init.clsCom,
"getBoneCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassComponent::nfGetBoneCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	rt->PushInt( component.GetBoneCount() );
}

// public func int indexOfBoneNamed( String name )
deClassComponent::nfIndexOfBoneNamed::nfIndexOfBoneNamed( const sInitData &init ) : dsFunction( init.clsCom,
"indexOfBoneNamed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
	p_AddParameter( init.clsStr ); // name
}
void deClassComponent::nfIndexOfBoneNamed::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	const char * const name = rt->GetValue( 0 )->GetString();
	const deRig * const rig = component.GetRig();
	
	if( rig ){
		rt->PushInt( rig->IndexOfBoneNamed( name ) );
		
	}else{
		rt->PushInt( -1 );
	}
}

// public func String boneGetName( int index )
deClassComponent::nfBoneGetName::nfBoneGetName( const sInitData &init ) : dsFunction( init.clsCom,
"boneGetName", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
	p_AddParameter( init.clsInt ); // bone
}
void deClassComponent::nfBoneGetName::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	const int index = rt->GetValue( 0 )->GetInt();
	const deRig * const rig = component.GetRig();
	
	if( ! rig ){
		DSTHROW( dueOutOfBoundary );
	}
	
	rt->PushString( rig->GetBoneAt( index ).GetName() );
}

// public func Vector boneGetPosition( int bone )
deClassComponent::nfBoneGetPosition::nfBoneGetPosition( const sInitData &init ) : dsFunction( init.clsCom,
"boneGetPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
	p_AddParameter( init.clsInt ); // bone
}
void deClassComponent::nfBoneGetPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	const deScriptingDragonScript &ds = *( ( ( deClassComponent* )GetOwnerClass() )->GetScriptModule() );
	
	component.PrepareBones();
	
	const deComponentBone &bone = component.GetBoneAt( rt->GetValue( 0 )->GetInt() );
	ds.GetClassVector()->PushVector( rt, bone.GetPosition() );
}

// public func void boneSetPosition( int bone, Vector position )
deClassComponent::nfBoneSetPosition::nfBoneSetPosition( const sInitData &init ) : dsFunction( init.clsCom,
"boneSetPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // bone
	p_AddParameter( init.clsVec ); // position
}
void deClassComponent::nfBoneSetPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	const deScriptingDragonScript &ds = *( ( ( deClassComponent* )GetOwnerClass() )->GetScriptModule() );
	
	deComponentBone &bone = component.GetBoneAt( rt->GetValue( 0 )->GetInt() );
	const decVector &position = ds.GetClassVector()->GetVector( rt->GetValue( 1 )->GetRealObject() );
	
	bone.SetPosition( position );
	component.InvalidateBones();
}

// public func Quaternion boneGetRotation( int bone )
deClassComponent::nfBoneGetRotation::nfBoneGetRotation( const sInitData &init ) :
dsFunction( init.clsCom, "boneGetRotation", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat ){
	p_AddParameter( init.clsInt ); // bone
}
void deClassComponent::nfBoneGetRotation::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	const deScriptingDragonScript &ds = *( ( ( deClassComponent* )GetOwnerClass() )->GetScriptModule() );
	
	component.PrepareBones();
	
	const deComponentBone &bone = component.GetBoneAt( rt->GetValue( 0 )->GetInt() );
	ds.GetClassQuaternion()->PushQuaternion( rt, bone.GetRotation() );
}

// public func void boneSetRotation( int index, Quaternion rotation )
deClassComponent::nfBoneSetRotation::nfBoneSetRotation( const sInitData &init ) :
dsFunction( init.clsCom, "boneSetRotation", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // bone
	p_AddParameter( init.clsQuat ); // rotation
}
void deClassComponent::nfBoneSetRotation::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	const deScriptingDragonScript &ds = *( ( ( deClassComponent* )GetOwnerClass() )->GetScriptModule() );
	
	deComponentBone &bone = component.GetBoneAt( rt->GetValue( 0 )->GetInt() );
	const decQuaternion &rotation = ds.GetClassQuaternion()->GetQuaternion( rt->GetValue( 1 )->GetRealObject() );
	
	bone.SetRotation( rotation );
	component.InvalidateBones();
}

// public func Matrix boneGetMatrix( int bone )
deClassComponent::nfBoneGetMatrix::nfBoneGetMatrix( const sInitData &init ) : dsFunction( init.clsCom,
"boneGetMatrix", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsMat ){
	p_AddParameter( init.clsInt ); // bone
}
void deClassComponent::nfBoneGetMatrix::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	const deScriptingDragonScript &ds = *( ( ( deClassComponent* )GetOwnerClass() )->GetScriptModule() );
	
	component.PrepareBones();
	
	const deComponentBone &bone = component.GetBoneAt( rt->GetValue( 0 )->GetInt() );
	ds.GetClassMatrix()->PushMatrix( rt, bone.GetMatrix() );
}

// public func Matrix boneGetInverseMatrix( int bone )
deClassComponent::nfBoneGetInverseMatrix::nfBoneGetInverseMatrix( const sInitData &init ) : dsFunction( init.clsCom,
"boneGetInverseMatrix", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsMat ){
	p_AddParameter( init.clsInt ); // bone
}
void deClassComponent::nfBoneGetInverseMatrix::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	const deScriptingDragonScript &ds = *( ( ( deClassComponent* )GetOwnerClass() )->GetScriptModule() );
	
	component.PrepareBones();
	
	deComponentBone &bone = component.GetBoneAt( rt->GetValue( 0 )->GetInt() );
	ds.GetClassMatrix()->PushMatrix( rt, bone.GetInverseMatrix() );
}

// public func Matrix getBoneOriginMatrix( int bone )
deClassComponent::nfGetBoneOriginMatrix::nfGetBoneOriginMatrix( const sInitData &init ) : dsFunction( init.clsCom,
"getBoneOriginMatrix", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsMat ){
	p_AddParameter( init.clsInt ); // bone
}
void deClassComponent::nfGetBoneOriginMatrix::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	const deScriptingDragonScript &ds = *( ( ( deClassComponent* )GetOwnerClass() )->GetScriptModule() );
	
	component.PrepareBones();
	
	const deComponentBone &bone = component.GetBoneAt( rt->GetValue( 0 )->GetInt() );
	
	if( bone.GetParentBone() == -1 ){
		ds.GetClassMatrix()->PushMatrix( rt, bone.GetOriginalMatrix() );
		
	}else{
		ds.GetClassMatrix()->PushMatrix( rt, bone.GetOriginalMatrix()
			* component.GetBoneAt( bone.GetParentBone() ).GetMatrix() );
	}
}

// public func Matrix getBoneOriginInverseMatrix( int bone )
deClassComponent::nfGetBoneOriginInverseMatrix::nfGetBoneOriginInverseMatrix( const sInitData &init ) : dsFunction( init.clsCom,
"getBoneOriginInverseMatrix", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsMat ){
	p_AddParameter( init.clsInt ); // bone
}
void deClassComponent::nfGetBoneOriginInverseMatrix::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	const deScriptingDragonScript &ds = *( ( ( deClassComponent* )GetOwnerClass() )->GetScriptModule() );
	
	component.PrepareBones();
	
	const deComponentBone &bone = component.GetBoneAt( rt->GetValue( 0 )->GetInt() );
	
	if( bone.GetParentBone() == -1 ){
		ds.GetClassMatrix()->PushMatrix( rt, bone.GetInverseOriginalMatrix() );
		
	}else{
		ds.GetClassMatrix()->PushMatrix( rt,
			component.GetBoneAt( bone.GetParentBone() ).GetInverseMatrix()
			* bone.GetInverseOriginalMatrix() );
	}
}

// public func Vector getBonePosition( String boneName )
deClassComponent::nfGetBonePosition::nfGetBonePosition( const sInitData &init ) : dsFunction( init.clsCom,
"getBonePosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
	p_AddParameter( init.clsStr ); // boneName
}
void deClassComponent::nfGetBonePosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	deClassComponent &clsComp = *( ( deClassComponent* )GetOwnerClass() );
	const deRig * const rig = component.GetRig();
	
	if( rig ){
		const char * const boneName = rt->GetValue( 0 )->GetString();
		const int boneIndex = rig->IndexOfBoneNamed( boneName );
		
		if( boneIndex != -1 ){
			component.PrepareBones();
			clsComp.GetScriptModule()->PushVector( rt, component.GetBoneAt( boneIndex ).GetPosition() );
			return;
		}
	}
	
	clsComp.GetScriptModule()->PushVector( rt, decVector() );
}

// public func Quaternion getBoneRotation( String boneName )
deClassComponent::nfGetBoneRotation::nfGetBoneRotation( const sInitData &init ) : dsFunction( init.clsCom,
"getBoneRotation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat ){
	p_AddParameter( init.clsStr ); // boneName
}
void deClassComponent::nfGetBoneRotation::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	const deScriptingDragonScript &ds = *( ( ( deClassComponent* )GetOwnerClass() )->GetScriptModule() );
	const deRig * const rig = component.GetRig();
	
	if( rig ){
		const char * const boneName = rt->GetValue( 0 )->GetString();
		const int boneIndex = rig->IndexOfBoneNamed( boneName );
		
		if( boneIndex != -1 ){
			component.PrepareBones();
			ds.GetClassQuaternion()->PushQuaternion( rt, component.GetBoneAt( boneIndex ).GetRotation() );
			return;
		}
	}
	
	ds.GetClassQuaternion()->PushQuaternion( rt, decQuaternion() );
}

// public func Matrix getBoneMatrix( String boneName )
deClassComponent::nfGetBoneMatrix::nfGetBoneMatrix( const sInitData &init ) : dsFunction( init.clsCom,
"getBoneMatrix", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsMat ){
	p_AddParameter( init.clsStr ); // boneName
}
void deClassComponent::nfGetBoneMatrix::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	deClassMatrix &clsMatrix = *( ( ( deClassComponent* )GetOwnerClass() )->GetScriptModule()->GetClassMatrix() );
	const deRig * const rig = component.GetRig();
	
	if( rig ){
		const char * const boneName = rt->GetValue( 0 )->GetString();
		const int boneIndex = rig->IndexOfBoneNamed( boneName );
		
		if( boneIndex != -1 ){
			component.PrepareBones();
			clsMatrix.PushMatrix( rt, component.GetBoneAt( boneIndex ).GetMatrix() );
			return;
		}
	}
	
	clsMatrix.PushMatrix( rt, decMatrix() );
}

// public func Matrix getBoneInverseMatrix( String boneName )
deClassComponent::nfGetBoneInverseMatrix::nfGetBoneInverseMatrix( const sInitData &init ) : dsFunction( init.clsCom,
"getBoneInverseMatrix", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsMat ){
	p_AddParameter( init.clsStr ); // boneName
}
void deClassComponent::nfGetBoneInverseMatrix::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	deClassMatrix &clsMatrix = *( ( ( deClassComponent* )GetOwnerClass() )->GetScriptModule()->GetClassMatrix() );
	const deRig * const rig = component.GetRig();
	
	if( rig ){
		const char * const boneName = rt->GetValue( 0 )->GetString();
		const int boneIndex = rig->IndexOfBoneNamed( boneName );
		
		if( boneIndex != -1 ){
			component.PrepareBones();
			clsMatrix.PushMatrix( rt, component.GetBoneAt( boneIndex ).GetInverseMatrix() );
			return;
		}
	}
	
	clsMatrix.PushMatrix( rt, decMatrix() );
}

// public func void setBonePosition( String boneName, Vector position )
deClassComponent::nfSetBonePosition::nfSetBonePosition( const sInitData &init ) :
dsFunction( init.clsCom, "setBonePosition", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsStr ); // boneName
	p_AddParameter( init.clsVec ); // position
}
void deClassComponent::nfSetBonePosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	const deScriptingDragonScript &ds = *( ( ( deClassComponent* )GetOwnerClass() )->GetScriptModule() );
	
	const decVector &position = ds.GetClassVector()->GetVector( rt->GetValue( 1 )->GetRealObject() );
	const char *boneName = rt->GetValue( 0 )->GetString();
	deRig * const rig = component.GetRig();
	
	if( rig ){
		const int boneIndex = rig->IndexOfBoneNamed( boneName );
		
		if( boneIndex != -1 ){
			component.GetBoneAt( boneIndex ).SetPosition( position );
			component.InvalidateBones();
		}
	}
}

// public func void setBoneRotation( String boneName, Quaternion rotation )
deClassComponent::nfSetBoneRotation::nfSetBoneRotation( const sInitData &init ) : dsFunction( init.clsCom,
"setBoneRotation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsStr ); // boneName
	p_AddParameter( init.clsQuat ); // rotation
}
void deClassComponent::nfSetBoneRotation::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	const deScriptingDragonScript &ds = *( ( ( deClassComponent* )GetOwnerClass() )->GetScriptModule() );
	
	const decQuaternion &rotation = ds.GetClassQuaternion()->GetQuaternion( rt->GetValue( 1 )->GetRealObject() );
	const char *boneName = rt->GetValue( 0 )->GetString();
	deRig * const rig = component.GetRig();
	
	if( rig ){
		const int boneIndex = rig->IndexOfBoneNamed( boneName );
		
		if( boneIndex != -1 ){
			component.GetBoneAt( boneIndex ).SetRotation( rotation );
			component.InvalidateBones();
		}
	}
}



// public func void updateBones()
deClassComponent::nfUpdateBones::nfUpdateBones( const sInitData &init ) : dsFunction( init.clsCom,
"updateBones", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassComponent::nfUpdateBones::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent *component = ( ( sCompNatDat* )p_GetNativeData( myself ) )->component;
	component->PrepareBones();
}

// public func void copyBonesToComponent( Component component )
deClassComponent::nfCopyBonesToComponent::nfCopyBonesToComponent( const sInitData &init ) : dsFunction( init.clsCom,
"copyBonesToComponent", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCom ); // component
}
void deClassComponent::nfCopyBonesToComponent::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	const deClassComponent &clsComp = *( ( deClassComponent* )GetOwnerClass() );
	
	deComponent * const otherComponent = clsComp.GetComponent( rt->GetValue( 0 )->GetRealObject() );
	if( ! otherComponent ){
		DSTHROW( dueNullPointer );
	}
	
	component.CopyBonesToComponent( *otherComponent );
}



// Textures
/////////////

// public func int indexOfTextureNamed( String name )
deClassComponent::nfIndexOfTextureNamed::nfIndexOfTextureNamed( const sInitData &init ) : dsFunction( init.clsCom,
"indexOfTextureNamed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
	p_AddParameter( init.clsStr ); // name
}
void deClassComponent::nfIndexOfTextureNamed::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	const char *name = rt->GetValue( 0 )->GetString();
	
	rt->PushInt( component.GetModel() ? component.GetModel()->IndexOfTextureNamed( name ) : -1 );
}

// public func int indexOfTextureClosestTo( Vector position, float radius )
deClassComponent::nfIndexOfTextureClosedTo::nfIndexOfTextureClosedTo( const sInitData &init ) : dsFunction( init.clsCom,
"indexOfTextureClosestTo", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
	p_AddParameter( init.clsVec ); // position
	p_AddParameter( init.clsFlt ); // radius
}
void deClassComponent::nfIndexOfTextureClosedTo::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	const deScriptingDragonScript &ds = *( ( ( deClassComponent* )GetOwnerClass() )->GetScriptModule() );
	const decVector &vector = ds.GetClassVector()->GetVector( rt->GetValue( 0 )->GetRealObject() );
	const float radius = rt->GetValue( 1 )->GetFloat();
	
	rt->PushInt( component.IndexOfTextureClosestTo( vector, radius ) );
}

// public func int getTextureCount()
deClassComponent::nfGetTextureCount::nfGetTextureCount( const sInitData &init ) : dsFunction( init.clsCom,
"getTextureCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassComponent::nfGetTextureCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	rt->PushInt( component.GetTextureCount() );
}

// public func String getTextureNameAt(int index)
deClassComponent::nfGetTextureNameAt::nfGetTextureNameAt( const sInitData &init ) :
dsFunction( init.clsCom, "getTextureNameAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
	p_AddParameter( init.clsInt ); // index
}
void deClassComponent::nfGetTextureNameAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	const int index = rt->GetValue( 0 )->GetInt();
	
	const deModel * const model = component.GetModel();
	if( ! model ){
		DSTHROW_INFO( dueNullPointer, "model" );
	}
	
	rt->PushString( model->GetTextureAt( index )->GetName() );
}

// public func Skin getTextureSkinAt( int index )
deClassComponent::nfGetTextureSkinAt::nfGetTextureSkinAt( const sInitData &init ) : dsFunction( init.clsCom,
"getTextureSkinAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsSkin ){
	p_AddParameter( init.clsInt ); // index
}
void deClassComponent::nfGetTextureSkinAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	const deScriptingDragonScript &ds = *( ( ( deClassComponent* )GetOwnerClass() )->GetScriptModule() );
	const int index = rt->GetValue( 0 )->GetInt();
	
	ds.GetClassSkin()->PushSkin( rt, component.GetTextureAt( index ).GetSkin() );
}

// public func void setTextureSkinAt( int index, Skin skin, int skinTexture )
deClassComponent::nfSetTextureSkinAt::nfSetTextureSkinAt( const sInitData &init ) : dsFunction( init.clsCom,
"setTextureSkinAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // index
	p_AddParameter( init.clsSkin ); // skin
	p_AddParameter( init.clsInt ); // skinTexture
}
void deClassComponent::nfSetTextureSkinAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	const deScriptingDragonScript &ds = *( ( ( deClassComponent* )GetOwnerClass() )->GetScriptModule() );
	
	int index = rt->GetValue( 0 )->GetInt();
	deSkin *skin = ds.GetClassSkin()->GetSkin( rt->GetValue( 1 )->GetRealObject() );
	int skinTexture = rt->GetValue( 2 )->GetInt();
	
	deComponentTexture &ctex = component.GetTextureAt( index );
	ctex.SetSkin( skin );
	ctex.SetTexture( skinTexture );
	
	component.NotifyTextureChanged( index );
}

// public func int getTextureTextureAt( int index )
deClassComponent::nfGetTextureTextureAt::nfGetTextureTextureAt( const sInitData &init ) :
dsFunction( init.clsCom, "getTextureTextureAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
	p_AddParameter( init.clsInt ); // index
}
void deClassComponent::nfGetTextureTextureAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	const int index = rt->GetValue( 0 )->GetInt();
	
	rt->PushInt( component.GetTextureAt( index ).GetTexture() );
}

// public func TexMatrix2 getTextureTransformAt( int index )
deClassComponent::nfGetTextureTransformAt::nfGetTextureTransformAt( const sInitData &init ): dsFunction( init.clsCom,
"getTextureTransformAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsTexMat2 ){
	p_AddParameter( init.clsInt ); // index
}
void deClassComponent::nfGetTextureTransformAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	const deScriptingDragonScript &ds = *( ( ( deClassComponent* )GetOwnerClass() )->GetScriptModule() );
	const int index = rt->GetValue( 0 )->GetInt();
	
	ds.GetClassTexMatrix2()->PushTexMatrix( rt, component.GetTextureAt( index ).GetTransform() );
}

// public func void setTextureTransformAt( int index, TexMatrix2 transform )
deClassComponent::nfSetTextureTransformAt::nfSetTextureTransformAt( const sInitData &init ): dsFunction( init.clsCom,
"setTextureTransformAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // index
	p_AddParameter( init.clsTexMat2 ); // transform
}
void deClassComponent::nfSetTextureTransformAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	const deScriptingDragonScript &ds = *( ( ( deClassComponent* )GetOwnerClass() )->GetScriptModule() );
	
	const int index = rt->GetValue( 0 )->GetInt();
	const decTexMatrix2 &transform = ds.GetClassTexMatrix2()->GetTexMatrix( rt->GetValue( 1 )->GetRealObject() );
	
	deComponentTexture &ctex = component.GetTextureAt( index );
	ctex.SetTransform( transform );
	
	component.NotifyTextureChanged( index );
}

// public func DynamicSkin getTextureDynamicSkinAt( int index )
deClassComponent::nfGetTextureDynamicSkinAt::nfGetTextureDynamicSkinAt( const sInitData &init ) : dsFunction( init.clsCom,
"getTextureDynamicSkinAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDSkin ){
	p_AddParameter( init.clsInt ); // index
}
void deClassComponent::nfGetTextureDynamicSkinAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	const deScriptingDragonScript &ds = *( ( ( deClassComponent* )GetOwnerClass() )->GetScriptModule() );
	const int index = rt->GetValue( 0 )->GetInt();
	
	ds.GetClassDynamicSkin()->PushDynamicSkin( rt, component.GetTextureAt( index ).GetDynamicSkin() );
}

// public func void setTextureDynamicSkinAt( int index, DynamicSkin dynamicSkin )
deClassComponent::nfSetTextureDynamicSkinAt::nfSetTextureDynamicSkinAt( const sInitData &init ) : dsFunction( init.clsCom,
"setTextureDynamicSkinAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // index
	p_AddParameter( init.clsDSkin ); // dynamicSkin
}
void deClassComponent::nfSetTextureDynamicSkinAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	const deScriptingDragonScript &ds = *( ( ( deClassComponent* )GetOwnerClass() )->GetScriptModule() );
	
	int index = rt->GetValue( 0 )->GetInt();
	deDynamicSkin *dynamicSkin = ds.GetClassDynamicSkin()->GetDynamicSkin( rt->GetValue( 1 )->GetRealObject() );
	
	component.GetTextureAt( index ).SetDynamicSkin( dynamicSkin );
	
	component.NotifyTextureChanged( index );
}



// Decals
///////////

// public func void addDecal( Decal decal )
deClassComponent::nfAddDecal::nfAddDecal( const sInitData &init ) :
dsFunction(init.clsCom, "addDecal", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsDec ); // decal
}
void deClassComponent::nfAddDecal::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	deScriptingDragonScript &ds = *( ( deClassComponent* )GetOwnerClass() )->GetScriptModule();
	
	deDecal * const decal = ds.GetClassDecal()->GetDecal( rt->GetValue( 0 )->GetRealObject() );
	component.AddDecal( decal );
}

// public func void removeDecal( Decal decal )
deClassComponent::nfRemoveDecal::nfRemoveDecal( const sInitData &init ) :
dsFunction(init.clsCom, "removeDecal", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter(init.clsDec); // decal
}
void deClassComponent::nfRemoveDecal::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	deScriptingDragonScript &ds = *( ( deClassComponent* )GetOwnerClass() )->GetScriptModule();
	
	deDecal * const decal = ds.GetClassDecal()->GetDecal( rt->GetValue( 0 )->GetRealObject() );
	component.RemoveDecal( decal );
}

// public func void removeAllDecals()
deClassComponent::nfRemoveAllDecals::nfRemoveAllDecals( const sInitData &init ) :
dsFunction( init.clsCom, "removeAllDecals", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassComponent::nfRemoveAllDecals::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	component.RemoveAllDecals();
}

// public func int getDecalCount()
deClassComponent::nfGetDecalCount::nfGetDecalCount( const sInitData &init ) :
dsFunction( init.clsCom, "getDecalCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassComponent::nfGetDecalCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	rt->PushInt( component.GetDecalCount() );
}

// public func void forEachDecal( Block ablock )
deClassComponent::nfForEachDecal::nfForEachDecal( const sInitData &init ) :
dsFunction( init.clsCom, "forEachDecal", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBlock ); // ablock
}
void deClassComponent::nfForEachDecal::RunFunction( dsRunTime *rt, dsValue *myself ){
	dsValue * const valueBlock = rt->GetValue( 0 );
	if( ! valueBlock->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	const deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	deDecal *decal = component.GetRootDecal();
	if( ! decal ){
		return;
	}
	
	const int funcIndexRun = ( ( dsClassBlock* )rt->GetEngine()->GetClassBlock() )->GetFuncIndexRun1();
	deScriptingDragonScript &ds = *( ( deClassComponent* )GetOwnerClass() )->GetScriptModule();
	deClassDecal &clsDecal = *ds.GetClassDecal();
	
	while( decal ){
		clsDecal.PushDecal( rt, decal );
		rt->RunFunctionFast( valueBlock, funcIndexRun ); // Object run( Decal )
		decal = decal->GetLLComponentNext();
	}
}



// public func int hashCode()
deClassComponent::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsCom, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}

void deClassComponent::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent *component = ( ( sCompNatDat* )p_GetNativeData( myself ) )->component;
	// hash code = memory location
	rt->PushInt( ( int )( intptr_t )component );
}

// public func bool equals( Object obj )
deClassComponent::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsCom, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // obj
}
void deClassComponent::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	deComponent *component = ( ( sCompNatDat* )p_GetNativeData( myself ) )->component;
	deClassComponent *clsComp = ( deClassComponent* )GetOwnerClass();
	dsValue *obj = rt->GetValue( 0 );
	if( ! p_IsObjOfType( obj, clsComp ) ){
		rt->PushBool( false );
	}else{
		deComponent *otherComp = ( ( sCompNatDat* )p_GetNativeData( obj ) )->component;
		rt->PushBool( component == otherComp );
	}
}



// Hints
//////////

// public func ComponentHintMovement getHintMovement()
deClassComponent::nfGetHintMovement::nfGetHintMovement( const sInitData &init ) :
dsFunction( init.clsCom, "getHintMovement", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsComponentHintMovement ){
}
void deClassComponent::nfGetHintMovement::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	rt->PushValue( ( ( deClassComponent* )GetOwnerClass() )->GetClassComponentHintMovement()
		->GetVariable( component.GetHintMovement() )->GetStaticValue() );
}

// public func void setHintMovement( ComponentHintMovement hint )
deClassComponent::nfSetHintMovement::nfSetHintMovement( const sInitData &init ) :
dsFunction( init.clsCom, "setHintMovement", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsComponentHintMovement ); // type
}
void deClassComponent::nfSetHintMovement::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	deComponent &component = *( ( ( sCompNatDat* )p_GetNativeData( myself ) )->component );
	component.SetHintMovement( ( deComponent::eMovementHints )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
}



// Class deClassComponent
///////////////////////////

// Constructor, destructor
////////////////////////////

deClassComponent::deClassComponent( deEngine *GameEngine, deScriptingDragonScript *ScrMgr ) :
dsClass( "Component", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ){
	if( ! GameEngine || ! ScrMgr ) DSTHROW( dueInvalidParam );
	// prepare
	pGameEngine = GameEngine;
	pScrMgr = ScrMgr;
	// store information into parser info
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	// do the rest
	p_SetNativeDataSize( sizeof( sCompNatDat ) );
}

deClassComponent::~deClassComponent(){
}



// Management
///////////////

void deClassComponent::CreateClassMembers( dsEngine *engine ){
	pClsComponentHintMovement = engine->GetClass( "Dragengine.Scenery.ComponentHintMovement" );
	
	sInitData init;
	pClsMdl = pScrMgr->GetClassModel();
	pClsSkin = pScrMgr->GetClassSkin();
	pClsImg = pScrMgr->GetClassImage();
	pClsRig = pScrMgr->GetClassRig();
	pClsDec = pScrMgr->GetClassDecal();
	pClsClr = pScrMgr->GetClassColor();
	pClsCam = pScrMgr->GetClassCamera();
	pClsVec = pScrMgr->GetClassVector();
	pClsDVec = pScrMgr->GetClassDVector();
	pClsQuat = pScrMgr->GetClassQuaternion();
	init.clsComponentHintMovement = pClsComponentHintMovement;
	
	init.clsCom = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	init.clsVec = pClsVec;
	init.clsDVec = pClsDVec;
	init.clsQuat = pClsQuat;
	init.clsMat = pScrMgr->GetClassMatrix();
	init.clsDMat = pScrMgr->GetClassDMatrix();
	init.clsMdl = pClsMdl;
	init.clsSkin = pClsSkin;
	init.clsImg = pClsImg;
	init.clsRig = pClsRig;
	init.clsDec = pClsDec;
	init.clsClr = pClsClr;
	init.clsCam = pClsCam;
	init.clsDSkin = pScrMgr->GetClassDynamicSkin();
	init.clsTexMat2 = pScrMgr->GetClassTexMatrix2();
	init.clsOccM = pScrMgr->GetClassOcclusionMesh();
	init.clsLayerMask = pScrMgr->GetClassLayerMask();
	init.clsBlock = engine->GetClassBlock();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfNew2( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetModel( init ) );
	AddFunction( new nfGetRig( init ) );
	AddFunction( new nfGetSkin( init ) );
	AddFunction( new nfGetPosition( init ) );
	AddFunction( new nfGetOrientation( init ) );
	AddFunction( new nfGetScaling( init ) );
	AddFunction( new nfGetMatrix( init ) );
	AddFunction( new nfGetInverseMatrix( init ) );
	AddFunction( new nfGetVisible( init ) );
	AddFunction( new nfSetVisible( init ) );
	
	AddFunction( new nfGetBoneCount( init ) );
	AddFunction( new nfIndexOfBoneNamed( init ) );
	AddFunction( new nfBoneGetName( init ) );
	AddFunction( new nfBoneGetPosition( init ) );
	AddFunction( new nfBoneSetPosition( init ) );
	AddFunction( new nfBoneGetRotation( init ) );
	AddFunction( new nfBoneSetRotation( init ) );
	AddFunction( new nfBoneGetMatrix( init ) );
	AddFunction( new nfBoneGetInverseMatrix( init ) );
	AddFunction( new nfGetBonePosition( init ) );
	AddFunction( new nfGetBoneRotation( init ) );
	AddFunction( new nfGetBoneMatrix( init ) );
	AddFunction( new nfGetBoneInverseMatrix( init ) );
	AddFunction( new nfGetBoneOriginMatrix( init ) );
	AddFunction( new nfGetBoneOriginInverseMatrix( init ) );
	AddFunction( new nfSetBonePosition( init ) );
	AddFunction( new nfSetBoneRotation( init ) );
	
	AddFunction( new nfGetOcclusionMesh( init ) );
	AddFunction( new nfSetOcclusionMesh( init ) );
	
	AddFunction( new nfGetAudioModel( init ) );
	AddFunction( new nfSetAudioModel( init ) );
	
	AddFunction( new nfSetModel( init ) );
	AddFunction( new nfSetModelKeepTextures( init ) );
	AddFunction( new nfSetSkin( init ) );
	AddFunction( new nfSetRig( init ) );
	AddFunction( new nfSetModelAndSkin( init ) );
	AddFunction( new nfSetPosition( init ) );
	AddFunction( new nfSetOrientation( init ) );
	AddFunction( new nfSetScaling( init ) );
	
	AddFunction( new nfGetLayerMask( init ) );
	AddFunction( new nfSetLayerMask( init ) );
	
	AddFunction( new nfGetDynamicSkin( init ) );
	AddFunction( new nfSetDynamicSkin( init ) );
	
	AddFunction( new nfUpdateBones( init ) );
	AddFunction( new nfCopyBonesToComponent( init ) );
	
	AddFunction( new nfIndexOfTextureNamed( init ) );
	AddFunction( new nfIndexOfTextureClosedTo( init ) );
	AddFunction( new nfGetTextureCount( init ) );
	AddFunction( new nfGetTextureNameAt( init ) );
	AddFunction( new nfGetTextureSkinAt( init ) );
	AddFunction( new nfGetTextureTextureAt( init ) );
	AddFunction( new nfSetTextureSkinAt( init ) );
	AddFunction( new nfGetTextureTransformAt( init ) );
	AddFunction( new nfSetTextureTransformAt( init ) );
	AddFunction( new nfGetTextureDynamicSkinAt( init ) );
	AddFunction( new nfSetTextureDynamicSkinAt( init ) );
	
	AddFunction( new nfAddDecal( init ) );
	AddFunction( new nfRemoveDecal( init ) );
	AddFunction( new nfRemoveAllDecals( init ) );
	AddFunction( new nfGetDecalCount( init ) );
	AddFunction( new nfForEachDecal( init ) );
	
	AddFunction( new nfGetHintMovement( init ) );
	AddFunction( new nfSetHintMovement( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

deComponent *deClassComponent::GetComponent( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sCompNatDat* )p_GetNativeData( myself->GetBuffer() ) )->component;
}

void deClassComponent::PushComponent( dsRunTime *rt, deComponent *component ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! component ){
		rt->PushObject( NULL, this );
		return;
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sCompNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->component = component;
	component->AddReference();
}
