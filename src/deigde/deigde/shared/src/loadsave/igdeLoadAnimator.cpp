/* 
 * Drag[en]gine IGDE
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
#include <stdlib.h>
#include <string.h>

#include "igdeLoadAnimator.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/curve/decCurveBezierPoint.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlDocumentReference.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/animation/deAnimationManager.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorReference.h>
#include <dragengine/resources/animator/deAnimatorManager.h>
#include <dragengine/resources/animator/deAnimatorLink.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/controller/deAnimatorControllerTarget.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleBoneTransformator.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimation.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimationDifference.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimationSelect.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleStateManipulator.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleStateSnapshot.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleInverseKinematic.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleForeignState.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleSubAnimator.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleLimit.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleGroup.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleTrackTo.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleMirror.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigManager.h>



// Class igdeLoadAnimator
///////////////////////////

// Constructor, destructor
////////////////////////////

igdeLoadAnimator::igdeLoadAnimator( igdeEnvironment &environment, deLogger *logger, const char *loggerSource ) :
igdeBaseXML( logger, loggerSource ),
pEnvironment( environment ),
pName( "Drag[en]gine Animator" ),
pPattern( "*.deanimator" ),
pDefaultExtension( ".deanimator" ){
}

igdeLoadAnimator::~igdeLoadAnimator(){
}



// Management
///////////////

void igdeLoadAnimator::SetName( const char *name ){
	pName = name;
}

void igdeLoadAnimator::SetPattern( const char *pattern ){
	pPattern = pattern;
}

void igdeLoadAnimator::SetDefaultExtension( const char *extension ){
	pDefaultExtension = extension;
}



// Loading and saving
///////////////////////

void igdeLoadAnimator::Load( const decString &pathAnimator, deAnimator &animator, decBaseFileReader &reader ){
	decPath basePath;
	basePath.SetFromUnix( pathAnimator.GetString() );
	if( basePath.GetComponentCount() > 1 ){
		basePath.RemoveLastComponent();
		
	}else{
		basePath.SetFromUnix( "/" );
	}
	
	decXmlDocumentReference xmlDoc;
	xmlDoc.TakeOver( new decXmlDocument );
	
	decXmlParser( GetLogger() ).ParseXml( &reader, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag *root = xmlDoc->GetRoot();
	if( ! root || strcmp( root->GetName(), "animator" ) != 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pReadAnimator( *root, basePath.GetPathUnix(), animator );
}



// Private Functions
//////////////////////

void igdeLoadAnimator::pReadAnimator( const decXmlElementTag &root, const char *basePath, deAnimator &animator ){
	const int elementCount = root.GetElementCount();
	deAnimation *animation = NULL;
	deRig *rig = NULL;
	int e;
	
	for( e=0; e<elementCount; e++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( e );
		if( ! tag ){
			continue;
		}
		
		if( strcmp( tag->GetName(), "rig" ) == 0 ){
			if( strlen( GetCDataString( *tag ) ) > 0 ){
				rig = NULL;
				
				try{
					rig = animator.GetEngine()->GetRigManager()->LoadRig( GetCDataString( *tag ), basePath );
					animator.SetRig( rig );
					rig->FreeReference();
					
				}catch( const deException &e ){
					if( rig ){
						rig->FreeReference();
					}
					LogWarnGenericProblemTag( root, tag->GetName(), "Failed loading resource file" );
				}
			}
			
		}else if( strcmp( tag->GetName(), "animation" ) == 0 ){
			if( strlen( GetCDataString( *tag ) ) > 0 ){
				animation = NULL;
				
				try{
					animation = animator.GetEngine()->GetAnimationManager()->LoadAnimation( GetCDataString( *tag ), basePath );
					animator.SetAnimation( animation );
					animation->FreeReference();
					
				}catch( const deException &e ){
					if( animation ){
						animation->FreeReference();
					}
					LogWarnGenericProblemTag( root, tag->GetName(), "Failed loading resource file" );
				}
			}
			
		}else if( strcmp( tag->GetName(), "bone" ) == 0 ){
			animator.GetListBones().Add( GetCDataString( *tag ) );
			animator.NotifyBonesChanged();
			
		}else if( strcmp( tag->GetName(), "controller" ) == 0 ){
			pReadController( *tag, animator );
			
		}else if( strcmp( tag->GetName(), "link" ) == 0 ){
			pReadLink( *tag, animator );
			
		}else{
			deAnimatorRule * const rule = pReadRule( *tag, basePath, animator );
			if( rule ){
				animator.AddRule( rule );
				rule->FreeReference();
			}
		}
	}
}

void igdeLoadAnimator::pReadController( const decXmlElementTag &root, deAnimator &animator ){
	const int elementCount = root.GetElementCount();
	deAnimatorController *controller = NULL;
	int e;
	
	try{
		controller = new deAnimatorController;
		
		for( e=0; e<elementCount; e++ ){
			const decXmlElementTag * const tag = root.GetElementIfTag( e );
			if( ! tag ){
				continue;
			}
			
			if( strcmp( tag->GetName(), "name" ) == 0 ){
				controller->SetName( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "clamp" ) == 0 ){
				controller->SetClamp( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "frozen" ) == 0 ){
				controller->SetFrozen( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "limits" ) == 0 ){
				controller->SetValueRange( GetAttributeFloat( *tag, "min" ), GetAttributeFloat( *tag, "max" ) );
			}
		}
		
		animator.AddController( controller );
		
	}catch( const deException & ){
		if( controller ){
			delete controller;
		}
		throw;
	}
}

void igdeLoadAnimator::pReadLink( const decXmlElementTag &root, deAnimator &animator ){
	const int elementCount = root.GetElementCount();
	deAnimatorLink *link = NULL;
	int e;
	
	try{
		link = new deAnimatorLink;
		
		for( e=0; e<elementCount; e++ ){
			const decXmlElementTag * const tag = root.GetElementIfTag( e );
			if( ! tag ){
				continue;
			}
			
			if( tag->GetName() == "controller" ){
				link->SetController( GetCDataInt( *tag ) );
				
			}else if( tag->GetName() == "curve" ){
				decCurveBezier curve;
				ReadCurveBezier( *tag, curve );
				link->SetCurve( curve );
				
			}else if( tag->GetName() == "repeat" ){
				link->SetRepeat( GetCDataInt( *tag ) );
				
			}else if( tag->GetName() == "bone" ){
				link->SetBone( GetCDataString( *tag ) );
				
			}else if( tag->GetName() == "boneParameter" ){
				const char * const name = GetCDataString( *tag );
				
				if( strcmp( name, "positionX" ) == 0 ){
					link->SetBoneParameter( deAnimatorLink::ebpPositionX );
					
				}else if( strcmp( name, "positionY" ) == 0 ){
					link->SetBoneParameter( deAnimatorLink::ebpPositionY );
					
				}else if( strcmp( name, "positionZ" ) == 0 ){
					link->SetBoneParameter( deAnimatorLink::ebpPositionZ );
					
				}else if( strcmp( name, "rotationX" ) == 0 ){
					link->SetBoneParameter( deAnimatorLink::ebpRotationX );
					
				}else if( strcmp( name, "rotationY" ) == 0 ){
					link->SetBoneParameter( deAnimatorLink::ebpRotationY );
					
				}else if( strcmp( name, "rotationZ" ) == 0 ){
					link->SetBoneParameter( deAnimatorLink::ebpRotationZ );
					
				}else if( strcmp( name, "scaleX" ) == 0 ){
					link->SetBoneParameter( deAnimatorLink::ebpScaleX );
					
				}else if( strcmp( name, "scaleY" ) == 0 ){
					link->SetBoneParameter( deAnimatorLink::ebpScaleY );
					
				}else if( strcmp( name, "scaleZ" ) == 0 ){
					link->SetBoneParameter( deAnimatorLink::ebpScaleZ );
					
				}else{
					LogErrorUnknownValue( *tag, name );
				}
				
			}else if( tag->GetName() == "boneLimits" ){
				link->SetBoneValueRange( GetAttributeFloat( *tag, "min" ), GetAttributeFloat( *tag, "max" ) );
				
			}else if( tag->GetName() == "wrapY" ){
				link->SetWrapY( GetCDataBool( *tag ) );
			}
		}
		
		animator.AddLink( link );
		
	}catch( const deException & ){
		if( link ){
			delete link;
		}
		throw;
	}
}

deAnimatorRule *igdeLoadAnimator::pReadRule( const decXmlElementTag &root,
const char *basePath, deAnimator &animator ){
	if( strcmp( root.GetName(), "ruleAnimation" ) == 0 ){
		return pReadRuleAnimation( root, animator );
		
	}else if( strcmp( root.GetName(), "ruleAnimationDifference" ) == 0 ){
		return pReadRuleAnimationDifference( root, animator );
		
	}else if( strcmp( root.GetName(), "ruleAnimationSelect" ) == 0 ){
		return pReadRuleAnimationSelect( root, animator );
		
	}else if( strcmp( root.GetName(), "ruleBoneTransformator" ) == 0 ){
		return pReadRuleBoneTransformator( root, animator );
		
	}else if( strcmp( root.GetName(), "ruleStateManipulator" ) == 0 ){
		return pReadRuleStateManipulator( root, animator );
		
	}else if( strcmp( root.GetName(), "ruleStateSnapshot" ) == 0 ){
		return pReadRuleStateSnapshot( root, animator );
		
	}else if( strcmp( root.GetName(), "ruleInverseKinematic" ) == 0 ){
		return pReadRuleInverseKinematic( root, animator );
		
	}else if( strcmp( root.GetName(), "ruleForeignState" ) == 0 ){
		return pReadRuleForeignState( root, animator );
		
	}else if( strcmp( root.GetName(), "ruleGroup" ) == 0 ){
		return pReadRuleGroup( root, basePath, animator );
		
	}else if( strcmp( root.GetName(), "ruleSubAnimator" ) == 0 ){
		return pReadRuleSubAnimator( root, basePath, animator );
		
	}else if( strcmp( root.GetName(), "ruleLimit" ) == 0 ){
		return pReadRuleLimit( root, animator );
		
	}else if( strcmp( root.GetName(), "ruleMirror" ) == 0 ){
		return pReadRuleMirror( root, animator );
		
	}else{
		return NULL;
	}
}

deAnimatorRule * igdeLoadAnimator::pReadRuleAnimation( const decXmlElementTag &root, deAnimator &animator ){
	const int elementCount = root.GetElementCount();
	deAnimatorRuleAnimation *rule = NULL;
	const char *name;
	int e;
	
	try{
		rule = new deAnimatorRuleAnimation;
		
		for( e=0; e<elementCount; e++ ){
			const decXmlElementTag * const tag = root.GetElementIfTag( e );
			if( ! tag ){
				continue;
			}
			
			if( strcmp( tag->GetName(), "moveName" ) == 0 ){
				rule->SetMoveName( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "moveTime" ) == 0 ){
				rule->SetMoveTime( GetCDataFloat( *tag ) );
				
			}else if( strcmp( tag->GetName(), "enablePosition" ) == 0 ){
				rule->SetEnablePosition( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "enableOrientation" ) == 0 ){
				rule->SetEnableOrientation( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "enableSize" ) == 0 ){
				rule->SetEnableSize( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "target" ) == 0 ){
				name = GetAttributeString( *tag, "name" );
				
				if( strcmp( name, "blendFactor" ) == 0 ){
					pReadControllerTarget( *tag, animator, rule->GetTargetBlendFactor() );
					
				}else if( strcmp( name, "moveTime" ) == 0 ){
					pReadControllerTarget( *tag, animator, rule->GetTargetMoveTime() );
					
				}else{
					LogErrorUnknownValue( *tag, name );
				}
				
			}else{
				pReadRuleCommon( *tag, animator, *rule );
			}
		}
		
	}catch( const deException & ){
		if( rule ){
			rule->FreeReference();
		}
		throw;
	}
	
	return rule;
}

deAnimatorRule * igdeLoadAnimator::pReadRuleAnimationDifference( const decXmlElementTag &root, deAnimator &animator ){
	const int elementCount = root.GetElementCount();
	deAnimatorRuleAnimationDifference *rule = NULL;
	const char *name;
	int e;
	
	try{
		rule = new deAnimatorRuleAnimationDifference;
		
		for( e=0; e<elementCount; e++ ){
			const decXmlElementTag * const tag = root.GetElementIfTag( e );
			if( ! tag ){
				continue;
			}
			
			if( strcmp( tag->GetName(), "leadingMoveName" ) == 0 ){
				rule->SetLeadingMoveName( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "leadingMoveTime" ) == 0 ){
				rule->SetLeadingMoveTime( GetCDataFloat( *tag ) );
				
			}else if( strcmp( tag->GetName(), "referenceMoveName" ) == 0 ){
				rule->SetReferenceMoveName( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "referenceMoveTime" ) == 0 ){
				rule->SetReferenceMoveTime( GetCDataFloat( *tag ) );
				
			}else if( strcmp( tag->GetName(), "enablePosition" ) == 0 ){
				rule->SetEnablePosition( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "enableOrientation" ) == 0 ){
				rule->SetEnableOrientation( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "enableSize" ) == 0 ){
				rule->SetEnableSize( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "target" ) == 0 ){
				name = GetAttributeString( *tag, "name" );
				
				if( strcmp( name, "blendFactor" ) == 0 ){
					pReadControllerTarget( *tag, animator, rule->GetTargetBlendFactor() );
					
				}else if( strcmp( name, "leadingMoveTime" ) == 0 ){
					pReadControllerTarget( *tag, animator, rule->GetTargetLeadingMoveTime() );
					
				}else if( strcmp( name, "referenceMoveTime" ) == 0 ){
					pReadControllerTarget( *tag, animator, rule->GetTargetReferenceMoveTime() );
					
				}else{
					LogErrorUnknownValue( *tag, name );
				}
				
			}else{
				pReadRuleCommon( *tag, animator, *rule );
			}
		}
		
	}catch( const deException & ){
		if( rule ){
			rule->FreeReference();
		}
		throw;
	}
	
	return rule;
}

deAnimatorRule * igdeLoadAnimator::pReadRuleAnimationSelect( const decXmlElementTag &root,
deAnimator &animator ){
	const int elementCount = root.GetElementCount();
	deAnimatorRuleAnimationSelect *rule = NULL;
	const char *name;
	int e;
	
	try{
		rule = new deAnimatorRuleAnimationSelect;
		
		for( e=0; e<elementCount; e++ ){
			const decXmlElementTag * const tag = root.GetElementIfTag( e );
			if( ! tag ){
				continue;
			}
			
			if( strcmp( tag->GetName(), "move" ) == 0 ){
				rule->GetMoves().Add( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "enablePosition" ) == 0 ){
				rule->SetEnablePosition( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "enableOrientation" ) == 0 ){
				rule->SetEnableOrientation( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "enableSize" ) == 0 ){
				rule->SetEnableSize( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "target" ) == 0 ){
				name = GetAttributeString( *tag, "name" );
				
				if( strcmp( name, "blendFactor" ) == 0 ){
					pReadControllerTarget( *tag, animator, rule->GetTargetBlendFactor() );
					
				}else if( strcmp( name, "moveTime" ) == 0 ){
					pReadControllerTarget( *tag, animator, rule->GetTargetMoveTime() );
					
				}else if( strcmp( name, "selection" ) == 0 ){
					pReadControllerTarget( *tag, animator, rule->GetTargetSelect() );
					
				}else{
					LogErrorUnknownValue( *tag, name );
				}
				
			}else{
				pReadRuleCommon( *tag, animator, *rule );
			}
		}
		
	}catch( const deException & ){
		if( rule ){
			rule->FreeReference();
		}
		throw;
	}
	
	return rule;
}

deAnimatorRule * igdeLoadAnimator::pReadRuleBoneTransformator( const decXmlElementTag &root, deAnimator &animator ){
	const int elementCount = root.GetElementCount();
	deAnimatorRuleBoneTransformator *rule = NULL;
	decVector vector;
	const char *name;
	int e;
	
	try{
		rule = new deAnimatorRuleBoneTransformator;
		
		for( e=0; e<elementCount; e++ ){
			const decXmlElementTag * const tag = root.GetElementIfTag( e );
			if( ! tag ){
				continue;
			}
			
			if( strcmp( tag->GetName(), "minimumTranslation" ) == 0 ){
				vector.SetZero();
				ReadVector( *tag, vector );
				rule->SetMinimumTranslation( vector );
				
			}else if( strcmp( tag->GetName(), "maximumTranslation" ) == 0 ){
				vector.SetZero();
				ReadVector( *tag, vector );
				rule->SetMaximumTranslation( vector );
				
			}else if( strcmp( tag->GetName(), "minimumRotation" ) == 0 ){
				vector.SetZero();
				ReadVector( *tag, vector );
				rule->SetMinimumRotation( vector * DEG2RAD );
				
			}else if( strcmp( tag->GetName(), "maximumRotation" ) == 0 ){
				vector.SetZero();
				ReadVector( *tag, vector );
				rule->SetMaximumRotation( vector * DEG2RAD );
				
			}else if( strcmp( tag->GetName(), "minimumScaling" ) == 0 ){
				vector.SetZero();
				ReadVector( *tag, vector );
				rule->SetMinimumScaling( vector );
				
			}else if( strcmp( tag->GetName(), "maximumScaling" ) == 0 ){
				vector.SetZero();
				ReadVector( *tag, vector );
				rule->SetMaximumScaling( vector );
				
			}else if( strcmp( tag->GetName(), "axis" ) == 0 ){
				vector.SetZero();
				ReadVector( *tag, vector );
				rule->SetAxis( vector );
				
			}else if( strcmp( tag->GetName(), "minimumAngle" ) == 0 ){
				rule->SetMinimumAngle( GetCDataFloat( *tag ) * DEG2RAD );
				
			}else if( strcmp( tag->GetName(), "maximumAngle" ) == 0 ){
				rule->SetMaximumAngle( GetCDataFloat( *tag ) * DEG2RAD );
				
			}else if( strcmp( tag->GetName(), "cframe" ) == 0 ){
				name = GetCDataString( *tag );
				
				if( strcmp( name, "local" ) == 0 ){
					rule->SetCoordinateFrame( deAnimatorRuleBoneTransformator::ecfBoneLocal );
					
				}else if( strcmp( name, "component" ) == 0 ){
					rule->SetCoordinateFrame( deAnimatorRuleBoneTransformator::ecfComponent );
					
				}else if( strcmp( name, "target" ) == 0 ){
					rule->SetCoordinateFrame( deAnimatorRuleBoneTransformator::ecfTargetBone );
					
				}else{
					LogErrorUnknownValue( *tag, name );
				}
				
			}else if( strcmp( tag->GetName(), "enablePosition" ) == 0 ){
				rule->SetEnablePosition( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "enableOrientation" ) == 0 ){
				rule->SetEnableOrientation( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "enableSize" ) == 0 ){
				rule->SetEnableSize( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "useAxis" ) == 0 ){
				rule->SetUseAxis( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "targetBone" ) == 0 ){
				rule->SetTargetBone( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "target" ) == 0 ){
				name = GetAttributeString( *tag, "name" );
				
				if( strcmp( name, "blendFactor" ) == 0 ){
					pReadControllerTarget( *tag, animator, rule->GetTargetBlendFactor() );
					
				}else if( strcmp( name, "translation" ) == 0 ){
					pReadControllerTarget( *tag, animator, rule->GetTargetTranslation() );
					
				}else if( strcmp( name, "rotation" ) == 0 ){
					pReadControllerTarget( *tag, animator, rule->GetTargetRotation() );
					
				}else if( strcmp( name, "scaling" ) == 0 ){
					pReadControllerTarget( *tag, animator, rule->GetTargetScaling() );
					
				}else{
					LogErrorUnknownValue( *tag, name );
				}
				
			}else{
				pReadRuleCommon( *tag, animator, *rule );
			}
		}
		
	}catch( const deException & ){
		if( rule ){
			rule->FreeReference();
		}
		throw;
	}
	
	return rule;
}

deAnimatorRule * igdeLoadAnimator::pReadRuleStateManipulator( const decXmlElementTag &root, deAnimator &animator ){
	const int elementCount = root.GetElementCount();
	deAnimatorRuleStateManipulator *rule = NULL;
	decVector vector;
	const char *name;
	int e;
	
	try{
		rule = new deAnimatorRuleStateManipulator;
		
		for( e=0; e<elementCount; e++ ){
			const decXmlElementTag * const tag = root.GetElementIfTag( e );
			if( ! tag ){
				continue;
			}
			
			if( strcmp( tag->GetName(), "positionMinimum" ) == 0 ){
				vector.SetZero();
				ReadVector( *tag, vector );
				rule->SetMinimumPosition( vector );
				
			}else if( strcmp( tag->GetName(), "positionMaximum" ) == 0 ){
				vector.SetZero();
				ReadVector( *tag, vector );
				rule->SetMaximumPosition( vector );
				
			}else if( strcmp( tag->GetName(), "rotationMinimum" ) == 0 ){
				vector.SetZero();
				ReadVector( *tag, vector );
				rule->SetMinimumRotation( vector * DEG2RAD );
				
			}else if( strcmp( tag->GetName(), "rotationMaximum" ) == 0 ){
				vector.SetZero();
				ReadVector( *tag, vector );
				rule->SetMaximumRotation( vector * DEG2RAD );
				
			}else if( strcmp( tag->GetName(), "sizeMinimum" ) == 0 ){
				vector.SetZero();
				ReadVector( *tag, vector );
				rule->SetMinimumSize( vector );
				
			}else if( strcmp( tag->GetName(), "sizeMaximum" ) == 0 ){
				vector.SetZero();
				ReadVector( *tag, vector );
				rule->SetMaximumSize( vector );
				
			}else if( strcmp( tag->GetName(), "enablePosition" ) == 0 ){
				rule->SetEnablePosition( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "enableOrientation" ) == 0 ){
				rule->SetEnableRotation( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "enableSize" ) == 0 ){
				rule->SetEnableSize( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "target" ) == 0 ){
				name = GetAttributeString( *tag, "name" );
				
				if( strcmp( name, "blendFactor" ) == 0 ){
					pReadControllerTarget( *tag, animator, rule->GetTargetBlendFactor() );
					
				}else if( strcmp( name, "position" ) == 0 ){
					pReadControllerTarget( *tag, animator, rule->GetTargetPosition() );
					
				}else if( strcmp( name, "orientation" ) == 0 ){
					pReadControllerTarget( *tag, animator, rule->GetTargetRotation() );
					
				}else if( strcmp( name, "size" ) == 0 ){
					pReadControllerTarget( *tag, animator, rule->GetTargetSize() );
					
				}else{
					LogErrorUnknownValue( *tag, name );
				}
				
			}else{
				pReadRuleCommon( *tag, animator, *rule );
			}
		}
		
	}catch( const deException & ){
		if( rule ){
			rule->FreeReference();
		}
		throw;
	}
	
	return rule;
}

deAnimatorRule * igdeLoadAnimator::pReadRuleStateSnapshot( const decXmlElementTag &root, deAnimator &animator ){
	const int elementCount = root.GetElementCount();
	deAnimatorRuleStateSnapshot *rule = NULL;
	const char *name;
	int e;
	
	try{
		rule = new deAnimatorRuleStateSnapshot;
		
		for( e=0; e<elementCount; e++ ){
			const decXmlElementTag * const tag = root.GetElementIfTag( e );
			if( ! tag ){
				continue;
			}
			
			if( strcmp( tag->GetName(), "useLastState" ) == 0 ){
				rule->SetUseLastState( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "id" ) == 0 ){
				rule->SetID( GetCDataInt( *tag ) );
				
			}else if( strcmp( tag->GetName(), "enablePosition" ) == 0 ){
				rule->SetEnablePosition( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "enableOrientation" ) == 0 ){
				rule->SetEnableOrientation( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "enableSize" ) == 0 ){
				rule->SetEnableSize( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "target" ) == 0 ){
				name = GetAttributeString( *tag, "name" );
				
				if( strcmp( name, "blendFactor" ) == 0 ){
					pReadControllerTarget( *tag, animator, rule->GetTargetBlendFactor() );
					
				}else{
					LogErrorUnknownValue( *tag, name );
				}
				
			}else{
				pReadRuleCommon( *tag, animator, *rule );
			}
		}
		
	}catch( const deException & ){
		if( rule ){
			rule->FreeReference();
		}
		throw;
	}
	
	return rule;
}

deAnimatorRule * igdeLoadAnimator::pReadRuleInverseKinematic( const decXmlElementTag &root, deAnimator &animator ){
	const int elementCount = root.GetElementCount();
	deAnimatorRuleInverseKinematic *rule = NULL;
	decVector vector;
	int e;
	
	try{
		rule = new deAnimatorRuleInverseKinematic;
		
		for( e=0; e<elementCount; e++ ){
			const decXmlElementTag * const tag = root.GetElementIfTag( e );
			if( ! tag ){
				continue;
			}
			
			const decString &tagName = tag->GetName();
			if( tagName == "goalPosition" ){
				vector.SetZero();
				ReadVector( *tag, vector );
				rule->SetGoalPosition( vector );
				
			}else if( tagName == "goalOrientation" ){
				vector.SetZero();
				ReadVector( *tag, vector );
				rule->SetGoalOrientation( decMatrix::CreateRotation( vector * DEG2RAD ).ToQuaternion() );
				
			}else if( tagName == "localPosition" ){
				vector.SetZero();
				ReadVector( *tag, vector );
				rule->SetLocalPosition( vector );
				
			}else if( tagName == "localOrientation" ){
				vector.SetZero();
				ReadVector( *tag, vector );
				rule->SetLocalOrientation( decMatrix::CreateRotation( vector * DEG2RAD ).ToQuaternion() );
				
			}else if( tagName == "adjustOrientation" ){
				rule->SetAdjustOrientation( GetCDataBool( *tag ) );
				
			}else if( tagName == "solverBone" ){
				rule->SetSolverBone( GetCDataString( *tag ) );
				
			}else if( tagName == "useSolverBone" ){
				rule->SetUseSolverBone( GetCDataBool( *tag ) );
				
			}else if( tagName == "reachRange" ){
				rule->SetReachRange( GetCDataFloat( *tag ) );
				
			}else if( tagName == "reachBone" ){
				rule->SetReachBone( GetCDataString( *tag ) );
				
			}else if ( tagName == "reachCenter" ){
				vector.SetZero();
				ReadVector( *tag, vector );
				rule->SetReachCenter( vector );
				
			}else if( tagName == "guidePosition" ){
				vector.SetZero();
				ReadVector( *tag, vector );
				rule->SetGuidePosition( vector );
				
			}else if( tagName == "guideBone" ){
				rule->SetGuideBone( GetCDataString( *tag ) );
				
			}else if( tagName == "useGuideSolverBone" ){
				rule->SetUseGuideSolverBone( GetCDataBool( *tag ) );
				
			}else if( tagName == "guideSolverBone" ){
				rule->SetGuideSolverBone( GetCDataString( *tag ) );
				
			}else if( tagName == "target" ){
				const decString &name = GetAttributeString( *tag, "name" );
				
				if( name == "blendFactor" ){
					pReadControllerTarget( *tag, animator, rule->GetTargetBlendFactor() );
					
				}else if( name == "goalPosition" ){
					pReadControllerTarget( *tag, animator, rule->GetTargetGoalPosition() );
					
				}else if( name == "goalOrientation" ){
					pReadControllerTarget( *tag, animator, rule->GetTargetGoalOrientation() );
					
				}else if( name == "localPosition" ){
					pReadControllerTarget( *tag, animator, rule->GetTargetLocalPosition() );
					
				}else if( name == "localOrientation" ){
					pReadControllerTarget( *tag, animator, rule->GetTargetLocalOrientation() );
					
				}else if( name == "reachRange" ){
					pReadControllerTarget( *tag, animator, rule->GetTargetReachRange() );
					
				}else if( name == "reachCenter" ){
					pReadControllerTarget( *tag, animator, rule->GetTargetReachCenter() );
					
				}else if( name == "guidePosition" ){
					pReadControllerTarget( *tag, animator, rule->GetTargetGuidePosition() );
					
				}else{
					LogErrorUnknownValue( *tag, name );
				}
				
			}else{
				pReadRuleCommon( *tag, animator, *rule );
			}
		}
		
	}catch( const deException & ){
		if( rule ){
			rule->FreeReference();
		}
		throw;
	}
	
	return rule;
}

deAnimatorRule * igdeLoadAnimator::pReadRuleForeignState( const decXmlElementTag &root, deAnimator &animator ){
	const int elementCount = root.GetElementCount();
	deAnimatorRuleForeignState *rule = NULL;
	decVector vector;
	const char *name;
	int e;
	
	try{
		rule = new deAnimatorRuleForeignState;
		
		for( e=0; e<elementCount; e++ ){
			const decXmlElementTag * const tag = root.GetElementIfTag( e );
			if( ! tag ){
				continue;
			}
			
			if( strcmp( tag->GetName(), "scalePosition" ) == 0 ){
				rule->SetScalePosition( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "scaleOrientation" ) == 0 ){
				rule->SetScaleOrientation( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "scaleSize" ) == 0 ){
				rule->SetScaleSize( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "enablePosition" ) == 0 ){
				rule->SetEnablePosition( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "enableOrientation" ) == 0 ){
				rule->SetEnableOrientation( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "enableSize" ) == 0 ){
				rule->SetEnableSize( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "foreignBone" ) == 0 ){
				rule->SetForeignBone( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "srcCFrame" ) == 0 ){
				name = tag->GetFirstData()->GetData();
				
				if( strcmp( name, "local" ) == 0 ){
					rule->SetSourceCoordinateFrame( deAnimatorRuleForeignState::ecfBoneLocal );
					
				}else if( strcmp( name, "component" ) == 0 ){
					rule->SetSourceCoordinateFrame( deAnimatorRuleForeignState::ecfComponent );
					
				}else{
					LogErrorUnknownValue( *tag, name );
				}
				
			}else if( strcmp( tag->GetName(), "destCFrame" ) == 0 ){
				name = tag->GetFirstData()->GetData();
				
				if( strcmp( name, "local" ) == 0 ){
					rule->SetDestCoordinateFrame( deAnimatorRuleForeignState::ecfBoneLocal );
					
				}else if( strcmp( name, "component" ) == 0 ){
					rule->SetDestCoordinateFrame( deAnimatorRuleForeignState::ecfComponent );
					
				}else{
					LogErrorUnknownValue( *tag, name );
				}
				
			}else if( strcmp( tag->GetName(), "target" ) == 0 ){
				name = GetAttributeString( *tag, "name" );
				
				if( strcmp( name, "blendFactor" ) == 0 ){
					pReadControllerTarget( *tag, animator, rule->GetTargetBlendFactor() );
					
				}else if( strcmp( name, "position" ) == 0 ){
					pReadControllerTarget( *tag, animator, rule->GetTargetPosition() );
					
				}else if( strcmp( name, "orientation" ) == 0 ){
					pReadControllerTarget( *tag, animator, rule->GetTargetOrientation() );
					
				}else if( strcmp( name, "size" ) == 0 ){
					pReadControllerTarget( *tag, animator, rule->GetTargetSize() );
					
				}else{
					LogErrorUnknownValue( *tag, name );
				}
				
			}else{
				pReadRuleCommon( *tag, animator, *rule );
			}
		}
		
	}catch( const deException & ){
		if( rule ){
			rule->FreeReference();
		}
		throw;
	}
	
	return rule;
}

deAnimatorRule * igdeLoadAnimator::pReadRuleMirror( const decXmlElementTag &root, deAnimator &animator ){
	const deAnimatorRuleMirror::Ref rule( deAnimatorRuleMirror::Ref::New( new deAnimatorRuleMirror ) );
	const int elementCount = root.GetElementCount();
	decVector vector;
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( tag->GetName() == "enablePosition" ){
			rule->SetEnablePosition( GetCDataBool( *tag ) );
			
		}else if( tag->GetName() == "enableOrientation" ){
			rule->SetEnableOrientation( GetCDataBool( *tag ) );
			
		}else if( tag->GetName() == "enableSize" ){
			rule->SetEnableSize( GetCDataBool( *tag ) );
			
		}else if( tag->GetName() == "mirrorAxis" ){
			const decString &name = tag->GetFirstData()->GetData();
			
			if( name == "x" ){
				rule->SetMirrorAxis( deAnimatorRuleMirror::emaX );
				
			}else if( name == "y" ){
				rule->SetMirrorAxis( deAnimatorRuleMirror::emaY );
				
			}else if( name == "z" ){
				rule->SetMirrorAxis( deAnimatorRuleMirror::emaZ );
				
			}else{
				LogErrorUnknownValue( *tag, name );
			}
			
		}else if( strcmp( tag->GetName(), "mirrorBone" ) == 0 ){
			rule->SetMirrorBone( GetCDataString( *tag ) );
			
		}else if( strcmp( tag->GetName(), "matchName" ) == 0 ){
			const decString &first = GetAttributeString( *tag, "first" );
			const decString &second = GetAttributeString( *tag, "second" );
			const decString &strType = GetAttributeString( *tag, "type" );
			
			deAnimatorRuleMirror::eMatchNameType type;
			if( strType == "first" ){
				type = deAnimatorRuleMirror::emntFirst;
				
			}else if( strType == "last" ){
				type = deAnimatorRuleMirror::emntLast;
				
			}else if( strType == "middle" ){
				type = deAnimatorRuleMirror::emntMiddle;
				
			}else{
				LogErrorUnknownValue( *tag, strType );
				DETHROW( deeInvalidParam ); // LogErrorUnknownValue does throw but compiler does not see it
			}
			
			rule->AddMatchName( first, second, type );
			
		}else if( strcmp( tag->GetName(), "target" ) == 0 ){
			const decString &name = GetAttributeString( *tag, "name" );
			
			if( name == "blendFactor" ){
				pReadControllerTarget( *tag, animator, rule->GetTargetBlendFactor() );
				
			}else{
				LogErrorUnknownValue( *tag, name );
			}
			
		}else{
			pReadRuleCommon( *tag, animator, *rule );
		}
	}
	
	return rule;
}

deAnimatorRule *igdeLoadAnimator::pReadRuleGroup( const decXmlElementTag &root,
const char *basePath, deAnimator &animator ){
	const int elementCount = root.GetElementCount();
	deAnimatorRuleGroup *rule = NULL;
	int i;
	
	try{
		rule = new deAnimatorRuleGroup;
		
		for( i=0; i<elementCount; i++ ){
			const decXmlElementTag * const tag = root.GetElementIfTag( i );
			if( ! tag ){
				continue;
			}
			
			if( pReadRuleCommon( *tag, animator, *rule ) ){
				continue;
			}
			
			if( strcmp( tag->GetName(), "applicationType" ) == 0 ){
				const char * const name = tag->GetFirstData()->GetData();
				
				if( strcmp( name, "all" ) == 0 ){
					rule->SetApplicationType( deAnimatorRuleGroup::eatAll );
					
				}else if( strcmp( name, "selection" ) == 0 ){
					rule->SetApplicationType( deAnimatorRuleGroup::eatSelect );
					
				}else{
					LogErrorUnknownValue( *tag, name );
				}
				
			}else if( strcmp( tag->GetName(), "useCurrentState" ) == 0 ){
				rule->SetUseCurrentState( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "enablePosition" ) == 0 ){
				rule->SetEnablePosition( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "enableOrientation" ) == 0 ){
				rule->SetEnableOrientation( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "enableSize" ) == 0 ){
				rule->SetEnableSize( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "target" ) == 0 ){
				const char * const name = GetAttributeString( *tag, "name" );
				
				if( strcmp( name, "blendFactor" ) == 0 ){
					pReadControllerTarget( *tag, animator, rule->GetTargetBlendFactor() );
					
				}else if( strcmp( name, "selection" ) == 0 ){
					pReadControllerTarget( *tag, animator, rule->GetTargetSelect() );
					
				}else{
					LogErrorUnknownValue( *tag, name );
				}
				
			}else{
				deAnimatorRule * const srule = pReadRule( *tag, basePath, animator );
				if( srule ){
					rule->AddRule( srule );
					srule->FreeReference();
				}
			}
		}
		
	}catch( const deException & ){
		if( rule ){
			rule->FreeReference();
		}
		throw;
	}
	
	return rule;
}

deAnimatorRule * igdeLoadAnimator::pReadRuleSubAnimator( const decXmlElementTag &root,
const char *basePath, deAnimator &animator ){
	deEngine &engine = *animator.GetEngine();
	deVirtualFileSystem &vfs = *engine.GetVirtualFileSystem();
	const int elementCount = root.GetElementCount();
	deAnimatorRuleSubAnimator *rule = NULL;
	bool hasConnections = false;
	decString pathAnimator;
	decVector vector;
	const char *name;
	int e, target;
	
	try{
		rule = new deAnimatorRuleSubAnimator;
		
		for( e=0; e<elementCount; e++ ){
			const decXmlElementTag * const tag = root.GetElementIfTag( e );
			if( ! tag ){
				continue;
			}
			
			if( strcmp( tag->GetName(), "pathAnimator" ) == 0 ){
				pathAnimator = GetCDataString( *tag );
				if( pathAnimator.IsEmpty() ){
					continue;
				}
				
				try{
					const decPath realPath( decPath::AbsolutePathNative( pathAnimator, basePath ) );
					decBaseFileReaderReference reader;
					reader.TakeOver( vfs.OpenFileForReading( realPath ) );
					deAnimatorReference subAnimator;
					subAnimator.TakeOver( animator.GetEngine()->GetAnimatorManager()->CreateAnimator() );
					Load( realPath.GetPathUnix(), subAnimator, reader );
					rule->SetSubAnimator( subAnimator );
					
				}catch( const deException &e ){
					LogWarnExceptionTag( *tag, e );
					LogWarnGenericProblemTag( root, tag->GetName(), "Failed loading animator" );
				}
				
			}else if( strcmp( tag->GetName(), "connection" ) == 0 ){
				hasConnections = true;
				target = GetAttributeInt( *tag, "target" );
				if( target >= 0 && target < rule->GetConnectionCount() ){
					rule->SetConnectionAt( target, GetAttributeInt( *tag, "controller" ) );
				}
				
			}else if( strcmp( tag->GetName(), "enablePosition" ) == 0 ){
				rule->SetEnablePosition( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "enableOrientation" ) == 0 ){
				rule->SetEnableOrientation( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "enableSize" ) == 0 ){
				rule->SetEnableSize( GetCDataBool( *tag ) );
				
			}else if( strcmp( tag->GetName(), "target" ) == 0 ){
				name = GetAttributeString( *tag, "name" );
				
				if( strcmp( name, "blendFactor" ) == 0 ){
					pReadControllerTarget( *tag, animator, rule->GetTargetBlendFactor() );
					
				}else{
					LogErrorUnknownValue( *tag, name );
				}
				
			}else{
				pReadRuleCommon( *tag, animator, *rule );
			}
		}
		
		if( ! hasConnections ){
			rule->SetMatchingConnections( animator );
		}
		
	}catch( const deException & ){
		if( rule ){
			rule->FreeReference();
		}
		throw;
	}
	
	return rule;
}

deAnimatorRule * igdeLoadAnimator::pReadRuleLimit( const decXmlElementTag &root, deAnimator &animator ){
	const int elementCount = root.GetElementCount();
	deAnimatorRuleLimit *rule = NULL;
	int i;
	
	try{
		rule = new deAnimatorRuleLimit;
		
		for( i=0; i<elementCount; i++ ){
			const decXmlElementTag * const tag = root.GetElementIfTag( i );
			if( ! tag ){
				continue;
			}
			
			if( strcmp( tag->GetName(), "minimumPosition" ) == 0 ){
				decVector vector;
				ReadVector( *tag, vector );
				rule->SetMinimumPosition( vector );
				
			}else if( strcmp( tag->GetName(), "maximumPosition" ) == 0 ){
				decVector vector;
				ReadVector( *tag, vector );
				rule->SetMaximumPosition( vector );
				
			}else if( strcmp( tag->GetName(), "minimumRotation" ) == 0 ){
				decVector vector;
				ReadVector( *tag, vector );
				rule->SetMinimumRotation( vector );
				
			}else if( strcmp( tag->GetName(), "maximumRotation" ) == 0 ){
				decVector vector;
				ReadVector( *tag, vector );
				rule->SetMaximumRotation( vector );
				
			}else if( strcmp( tag->GetName(), "minimumScaling" ) == 0 ){
				decVector vector;
				ReadVector( *tag, vector );
				rule->SetMinimumScaling( vector );
				
			}else if( strcmp( tag->GetName(), "maximumScaling" ) == 0 ){
				decVector vector;
				ReadVector( *tag, vector );
				rule->SetMaximumScaling( vector );
				
			}else if( strcmp( tag->GetName(), "cframe" ) == 0 ){
				const char * const name = tag->GetFirstData()->GetData();
				
				if( strcmp( name, "local" ) == 0 ){
					rule->SetCoordinateFrame( deAnimatorRuleLimit::ecfBoneLocal );
					
				}else if( strcmp( name, "component" ) == 0 ){
					rule->SetCoordinateFrame( deAnimatorRuleLimit::ecfComponent );
					
				}else if( strcmp( name, "target" ) == 0 ){
					rule->SetCoordinateFrame( deAnimatorRuleLimit::ecfTargetBone );
					
				}else{
					LogErrorUnknownValue( *tag, name );
				}
				
			}else if( strcmp( tag->GetName(), "enablePosMin" ) == 0 ){
				rule->SetEnablePositionXMin( GetAttributeInt( *tag, "x" ) != 0 );
				rule->SetEnablePositionYMin( GetAttributeInt( *tag, "y" ) != 0 );
				rule->SetEnablePositionZMin( GetAttributeInt( *tag, "z" ) != 0 );
				
			}else if( strcmp( tag->GetName(), "enablePosMax" ) == 0 ){
				rule->SetEnablePositionXMax( GetAttributeInt( *tag, "x" ) != 0 );
				rule->SetEnablePositionYMax( GetAttributeInt( *tag, "y" ) != 0 );
				rule->SetEnablePositionZMax( GetAttributeInt( *tag, "z" ) != 0 );
				
			}else if( strcmp( tag->GetName(), "enableRotMin" ) == 0 ){
				rule->SetEnableRotationXMin( GetAttributeInt( *tag, "x" ) != 0 );
				rule->SetEnableRotationYMin( GetAttributeInt( *tag, "y" ) != 0 );
				rule->SetEnableRotationZMin( GetAttributeInt( *tag, "z" ) != 0 );
				
			}else if( strcmp( tag->GetName(), "enableRotMax" ) == 0 ){
				rule->SetEnableRotationXMax( GetAttributeInt( *tag, "x" ) != 0 );
				rule->SetEnableRotationYMax( GetAttributeInt( *tag, "y" ) != 0 );
				rule->SetEnableRotationZMax( GetAttributeInt( *tag, "z" ) != 0 );
				
			}else if( strcmp( tag->GetName(), "enableScaleMin" ) == 0 ){
				rule->SetEnableScalingXMin( GetAttributeInt( *tag, "x" ) != 0 );
				rule->SetEnableScalingYMin( GetAttributeInt( *tag, "y" ) != 0 );
				rule->SetEnableScalingZMin( GetAttributeInt( *tag, "z" ) != 0 );
				
			}else if( strcmp( tag->GetName(), "enableScaleMax" ) == 0 ){
				rule->SetEnableScalingXMax( GetAttributeInt( *tag, "x" ) != 0 );
				rule->SetEnableScalingYMax( GetAttributeInt( *tag, "y" ) != 0 );
				rule->SetEnableScalingZMax( GetAttributeInt( *tag, "z" ) != 0 );
				
			}else if( strcmp( tag->GetName(), "targetBone" ) == 0 ){
				rule->SetTargetBone( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "target" ) == 0 ){
				const char * const name = GetAttributeString( *tag, "name" );
				
				if( strcmp( name, "blendFactor" ) == 0 ){
					pReadControllerTarget( *tag, animator, rule->GetTargetBlendFactor() );
					
				}else{
					LogErrorUnknownValue( *tag, name );
				}
				
			}else{
				pReadRuleCommon( *tag, animator, *rule );
			}
		}
		
	}catch( const deException & ){
		if( rule ){
			rule->FreeReference();
		}
		throw;
	}
	
	return rule;
}

deAnimatorRule *igdeLoadAnimator::pReadRuleTrackTo( const decXmlElementTag &root, deAnimator &animator ){
	const int elementCount = root.GetElementCount();
	deAnimatorRuleTrackTo *rule = NULL;
	int i;
	
	try{
		rule = new deAnimatorRuleTrackTo;
		
		for( i=0; i<elementCount; i++ ){
			const decXmlElementTag * const tag = root.GetElementIfTag( i );
			if( ! tag ){
				continue;
			}
			
			if( strcmp( tag->GetName(), "trackBone" ) == 0 ){
				if( tag->GetFirstData() ){
					rule->SetTrackBone( GetCDataString( *tag ) );
				}
				
			}else if( strcmp( tag->GetName(), "trackAxis" ) == 0 ){
				const char * const name = GetCDataString( *tag );
				
				if( strcmp( name, "posX" ) == 0 ){
					rule->SetTrackAxis( deAnimatorRuleTrackTo::etaPosX );
					
				}else if( strcmp( name, "posY" ) == 0 ){
					rule->SetTrackAxis( deAnimatorRuleTrackTo::etaPosY );
					
				}else if( strcmp( name, "posZ" ) == 0 ){
					rule->SetTrackAxis( deAnimatorRuleTrackTo::etaPosZ );
					
				}else if( strcmp( name, "negX" ) == 0 ){
					rule->SetTrackAxis( deAnimatorRuleTrackTo::etaNegX );
					
				}else if( strcmp( name, "negY" ) == 0 ){
					rule->SetTrackAxis( deAnimatorRuleTrackTo::etaNegY );
					
				}else if( strcmp( name, "negZ" ) == 0 ){
					rule->SetTrackAxis( deAnimatorRuleTrackTo::etaNegZ );
					
				}else{
					LogErrorUnknownValue( *tag, name );
				}
				
			}else if( strcmp( tag->GetName(), "upAxis" ) == 0 ){
				const char * const name = GetCDataString( *tag );
				
				if( strcmp( name, "posX" ) == 0 ){
					rule->SetUpAxis( deAnimatorRuleTrackTo::etaPosX );
					
				}else if( strcmp( name, "posY" ) == 0 ){
					rule->SetUpAxis( deAnimatorRuleTrackTo::etaPosY );
					
				}else if( strcmp( name, "posZ" ) == 0 ){
					rule->SetUpAxis( deAnimatorRuleTrackTo::etaPosZ );
					
				}else if( strcmp( name, "negX" ) == 0 ){
					rule->SetUpAxis( deAnimatorRuleTrackTo::etaNegX );
					
				}else if( strcmp( name, "negY" ) == 0 ){
					rule->SetUpAxis( deAnimatorRuleTrackTo::etaNegY );
					
				}else if( strcmp( name, "negZ" ) == 0 ){
					rule->SetUpAxis( deAnimatorRuleTrackTo::etaNegZ );
					
				}else{
					LogErrorUnknownValue( *tag, name );
				}
				
			}else if( strcmp( tag->GetName(), "upTarget" ) == 0 ){
				const char * const name = GetCDataString( *tag );
				
				if( strcmp( name, "worldX" ) == 0 ){
					rule->SetUpTarget( deAnimatorRuleTrackTo::eutWorldX );
					
				}else if( strcmp( name, "worldY" ) == 0 ){
					rule->SetUpTarget( deAnimatorRuleTrackTo::eutWorldY );
					
				}else if( strcmp( name, "worldZ" ) == 0 ){
					rule->SetUpTarget( deAnimatorRuleTrackTo::eutWorldZ );
					
				}else if( strcmp( name, "componentX" ) == 0 ){
					rule->SetUpTarget( deAnimatorRuleTrackTo::eutComponentX );
					
				}else if( strcmp( name, "componentY" ) == 0 ){
					rule->SetUpTarget( deAnimatorRuleTrackTo::eutComponentY );
					
				}else if( strcmp( name, "componentZ" ) == 0 ){
					rule->SetUpTarget( deAnimatorRuleTrackTo::eutComponentZ );
					
				}else if( strcmp( name, "trackBoneX" ) == 0 ){
					rule->SetUpTarget( deAnimatorRuleTrackTo::eutTrackBoneX );
					
				}else if( strcmp( name, "trackBoneY" ) == 0 ){
					rule->SetUpTarget( deAnimatorRuleTrackTo::eutTrackBoneY );
					
				}else if( strcmp( name, "trackBoneZ" ) == 0 ){
					rule->SetUpTarget( deAnimatorRuleTrackTo::eutTrackBoneZ );
					
				}else if( strcmp( name, "controller" ) == 0 ){
					rule->SetUpTarget( deAnimatorRuleTrackTo::eutController );
					
				}else{
					LogErrorUnknownValue( *tag, name );
				}
				
			}else if( strcmp( tag->GetName(), "target" ) == 0 ){
				const char * const name = GetAttributeString( *tag, "name" );
				
				if( strcmp( name, "blendFactor" ) == 0 ){
					pReadControllerTarget( *tag, animator, rule->GetTargetBlendFactor() );
					
				}else if( strcmp( name, "position" ) == 0 ){
					pReadControllerTarget( *tag, animator, rule->GetTargetPosition() );
					
				}else if( strcmp( name, "up" ) == 0 ){
					pReadControllerTarget( *tag, animator, rule->GetTargetUp() );
					
				}else{
					LogErrorUnknownValue( *tag, name );
				}
				
			}else{
				pReadRuleCommon( *tag, animator, *rule );
			}
		}
		
	}catch( const deException & ){
		if( rule ){
			rule->FreeReference();
		}
		throw;
	}
	
	return rule;
}

bool igdeLoadAnimator::pReadRuleCommon( const decXmlElementTag &root, deAnimator &animator, deAnimatorRule &rule ){
	if( strcmp( root.GetName(), "name" ) == 0 ){
		return true;
		
	}else if( strcmp( root.GetName(), "enabled" ) == 0 ){
		rule.SetEnabled( GetCDataInt( root ) != 0 );
		return true;
		
	}else if( strcmp( root.GetName(), "blendMode" ) == 0 ){
		const char * const blendMode = GetCDataString( root );
		
		if( strcmp( blendMode, "blend" ) == 0 ){
			rule.SetBlendMode( deAnimatorRule::ebmBlend );
			
		}else if( strcmp( blendMode, "overlay" ) == 0 ){
			rule.SetBlendMode( deAnimatorRule::ebmOverlay );
			
		}else{
			LogErrorUnknownValue( root, blendMode );
		}
		
		return true;
		
	}else if( strcmp( root.GetName(), "blendFactor" ) == 0 ){
		rule.SetBlendFactor( GetCDataFloat( root ) );
		return true;
		
	}else if( strcmp( root.GetName(), "bone" ) == 0 ){
		rule.GetListBones().Add( GetCDataString( root ) );
		return true;
	}
	
	return false;
}

void igdeLoadAnimator::pReadControllerTarget( const decXmlElementTag &root, deAnimator &animator, deAnimatorControllerTarget &target ){
	const int elementCount = root.GetElementCount();
	int e;
	
	for( e=0; e<elementCount; e++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( e );
		if( ! tag ){
			continue;
		}
		
		if( strcmp( tag->GetName(), "link" ) == 0 ){
			target.AddLink( GetCDataInt( *tag ) );
		}
	}
}
