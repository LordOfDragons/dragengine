/* 
 * Drag[en]gine IGDE World Editor
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "meViewEditorAddNew.h"
#include "../meView3D.h"
#include "../meWindowMain.h"
#include "../../collisions/meCLSelect.h"
#include "../../collisions/meCLClosestElement.h"
#include "../../collisions/meCLAddDecal.h"
#include "../../collisions/meCLSnapPoint.h"
#include "../../configuration/meConfiguration.h"
#include "../../world/meCamera.h"
#include "../../world/meWorld.h"
#include "../../world/meWorldGuiParameters.h"
#include "../../world/object/meObject.h"
#include "../../world/object/meObjectSnapPoint.h"
#include "../../world/navspace/meNavigationSpace.h"
#include "../../undosys/gui/object/meUAddObject.h"
#include "../../undosys/gui/navspace/meUAddNavSpace.h"

#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/gamedefinition/class/snappoint/igdeGDCSnapPoint.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/world/deWorld.h>



// Class meViewEditorAddNew
/////////////////////////////

// Constructor, destructor
////////////////////////////

meViewEditorAddNew::meViewEditorAddNew( meView3D &view ) :
meViewEditorNavigation( view ),
pClosestElement( NULL ),
pCLSnapPoint( NULL ),
pFilterObjectsByClass( NULL ),
pAddDecal( NULL ),
pUndoAddObject( NULL ),
pUndoAddNavSpace( NULL )
{
	pCLCollider.TakeOver( view.GetWindowMain().GetEngine()->GetColliderManager()->CreateColliderVolume() );
	
	decLayerMask collisionCategory;
	collisionCategory.SetBit( meWorld::eclmEditing );
	
	decLayerMask collisionFilter;
	collisionFilter.SetBit( meWorld::eclmSnapPoint );
	
	pCLCollider->SetCollisionFilter( decCollisionFilter( collisionCategory, collisionFilter ) );
}

meViewEditorAddNew::~meViewEditorAddNew(){
	if( pAddDecal ){
		delete pAddDecal;
	}
	
	if( pCLSnapPoint ){
		delete pCLSnapPoint;
	}
	
	if( pClosestElement ){
		delete pClosestElement;
	}
	if( pFilterObjectsByClass ){
		delete pFilterObjectsByClass;
	}
}



// Management
///////////////



// Callbacks
//////////////

bool meViewEditorAddNew::OnKeyPress( deInputEvent::eKeyCodes key, bool shift, bool control ){
	return meViewEditorNavigation::OnKeyPress( key, shift, control );
}

void meViewEditorAddNew::OnLeftMouseButtonPress( int x, int y, bool shift, bool control ){
	meViewEditorNavigation::OnLeftMouseButtonPress( x, y, shift, control );
	
	const meWorldGuiParameters &guiparams = GetWorldGuiParameters();
	const meCamera &camera = GetActiveCamera();
	const int elementMode = GetElementMode();
	meWorld &world = GetWorld();
	decDVector position;
	decVector rotation;
	
	pUndoAddObject = NULL;
	pUndoAddNavSpace = NULL;
	
	if( pCLSnapPoint ){
		delete pCLSnapPoint;
		pCLSnapPoint = NULL;
	}
	
	if( elementMode == meWorldGuiParameters::eemObject ){
		if( pClosestElement ){
			DETHROW( deeInvalidParam );
		}
		
		if( guiparams.GetBrowseClass().IsEmpty() ){
			return;
		}
		
		// create closest element visitor
		pClosestElement = new meCLClosestElement( world );
		
		pFilterObjectsByClass = new meFilterObjectsByClass;
		pFilterObjectsByClass->SetRejectGhosts( true );
		//pFilterObjectsByClass->SetClassNamesFrom( guiparams.GetAddFilterObjectList() );
		//pFilterObjectsByClass->SetMatchInclusive( guiparams.GetAddFilterObjectInclusive() );
		
		// TODO: the height terrain is currently hit even if there is no texture on
		// it or it has a hole. the test has to be refined to avoid scoring a hit
		// on the height terrain if there is no texture or a hole
		pClosestElement->SetTestHeightTerrain( true );
		pClosestElement->SetTestObjects( true );
		pClosestElement->SetFilterObjects( pFilterObjectsByClass );
		
		meObject *object = NULL;
		
		try{
			object = new meObject( world.GetEnvironment() );
			object->SetClassName( guiparams.GetBrowseClass().GetString() );
			object->SetID( world.NextObjectID() );
			
			pUndoAddObject.TakeOver( new meUAddObject( &world, object ) );
			object->FreeReference();
			
		}catch( const deException &e ){
			if( object ){
				object->FreeReference();
			}
			pUndoAddObject = NULL;
			
			delete pClosestElement;
			pClosestElement = NULL;
			
			LogException( e );
			return;
		}
		
		pClosestElement->GetIgnoreObjects().Add( object );
		
		// create snap point visitor
		pCLSnapPoint = new meCLSnapPoint( world, object );
		
		decShapeList shapeList;
		pCLSnapPoint->CalcBoundingBoxShape( shapeList );
		( ( deColliderVolume& )( deCollider& )pCLCollider ).SetShapes( shapeList );
		
		// update undo
		pUndoAddObject->Redo(); // from here on we only manipulate the object parameters
		pUpdateUndo( shift, control );
		
	}else if( elementMode == meWorldGuiParameters::eemDecal ){
		if( pAddDecal ){
			DETHROW( deeInvalidParam );
		}
		
		pAddDecal = new meCLAddDecal( &GetView().GetWindowMain(), &world );
		
		pAddDecal->SetRay( camera.GetViewMatrix().GetPosition(),
			camera.GetDirectionFor( GetViewWidth(), GetViewHeight(), x, y ) );
		pAddDecal->SetAlignWithNormal( ! shift );
		
		pUpdateUndo( shift, control );
		
	}if( elementMode == meWorldGuiParameters::eemNavSpace ){
		meNavigationSpace *safeNavspace = NULL;
		meNavigationSpace *navspace = NULL;
		
		try{
			navspace = new meNavigationSpace( world.GetEnvironment() );
			
			pUndoAddNavSpace.TakeOver( new meUAddNavSpace( &world, decPoint3(), navspace ) );
			safeNavspace = navspace;
			
			navspace->FreeReference();
			navspace = NULL;
			
			pUndoAddNavSpace->Redo();
			
		}catch( const deException &e ){
			if( navspace ){
				navspace->FreeReference();
			}
			pUndoAddNavSpace = NULL;
			
			LogException( e );
			return;
		}
		
		world.GetSelectionNavigationSpace().RemoveAll();
		world.GetSelectionNavigationSpace().Add( safeNavspace );
		world.GetSelectionNavigationSpace().SetActive( safeNavspace );
		world.NotifyNavSpaceSelectionChanged();
		
		pUpdateUndo( shift, control );
	}
}

void meViewEditorAddNew::OnLeftMouseButtonRelease( int x, int y, bool shift, bool control ){
	meViewEditorNavigation::OnLeftMouseButtonRelease( x, y, shift, control );
	
	if( pUndoAddNavSpace ){
		GetWorld().GetUndoSystem()->Add( pUndoAddNavSpace, false );
		pUndoAddNavSpace = NULL;
	}
	if( pUndoAddObject ){
		GetWorld().GetUndoSystem()->Add( pUndoAddObject, false );
		pUndoAddObject = NULL;
	}
	
	if( pClosestElement ){
		delete pClosestElement;
		pClosestElement = NULL;
	}
	
	if( pAddDecal ){
		pAddDecal->Finish();
		delete pAddDecal;
		pAddDecal = NULL;
	}
	
	if( pCLSnapPoint ){
		delete pCLSnapPoint;
		pCLSnapPoint = NULL;
	}
}

void meViewEditorAddNew::OnMouseMove( int x, int y, bool shift, bool control ){
	meViewEditorNavigation::OnMouseMove( x, y, shift, control );
	
	/*if( pUndoAddObject ){
		pUndoAddObject->Undo(); // overkill
	}*/
	if( pUndoAddNavSpace ){
		pUndoAddNavSpace->Undo();
	}
	
	if( pClosestElement || pAddDecal ){
		pUpdateUndo( shift, control );
	}
}



void meViewEditorAddNew::pUpdateUndo( bool shift, bool control ){
	const meCamera &camera = GetActiveCamera();
	const int elementMode = GetElementMode();
	const decPoint& pt = GetDragCurrent();
	
	decDVector rayDirection = camera.GetDirectionFor( GetViewWidth(), GetViewHeight(), pt.x, pt.y );
	const decDVector rayPosition = camera.GetViewMatrix().GetPosition();
	
	if( elementMode == meWorldGuiParameters::eemObject ){
		if( ! pUndoAddObject || ! ( ( meUAddObject& )( igdeUndo& )pUndoAddObject ).GetObject() ){
			return;
		}
		
		meObject &undoObject = *( ( meUAddObject& )( igdeUndo& )pUndoAddObject ).GetObject();
		bool applySnapping = true;
		decDVector position;
		decVector rotation;
		
		if( ! pClosestElement || ! pCLSnapPoint ){
			DETHROW( deeInvalidParam );
		}
		
		if( control ){
			rayDirection *= 3.0; // 3m
			position = rayPosition + rayDirection;
			
		}else{
			rayDirection *= 50.0; // 50m
			
			decLayerMask collisionCategory;
			collisionCategory.SetBit( meWorld::eclmEditing );
			
			decLayerMask collisionFilter;
			collisionFilter.SetBit( meWorld::eclmObjects );
			collisionFilter.SetBit( meWorld::eclmHeightTerrains );
			collisionFilter.SetBit( meWorld::eclmSnapPoint );
			
			pClosestElement->Reset();
			pClosestElement->SetTestSnapPoints( pCLSnapPoint->IsSnappingEnabled() );
			RayTestCollision( pClosestElement, rayPosition, rayDirection,
				decCollisionFilter( collisionCategory, collisionFilter ) );
			
			if( pClosestElement->GetHitObject() ){
				position = rayPosition + rayDirection * ( double )pClosestElement->GetHitDistance();
				
				if( shift ){
					const decVector &normal = pClosestElement->GetHitNormal();
					
					rotation.x = acosf( normal.y ) / DEG2RAD;
					rotation.y = atan2f( normal.x, -normal.z ) / DEG2RAD;
					rotation.z = 0.0f;
				}
				
			}else if( pClosestElement->GetHitSnapPoint() ){
				const meObjectSnapPoint &hitSnapPoint = *pClosestElement->GetHitSnapPoint();
				const igdeGDCSnapPoint &gdcSnapPoint = *hitSnapPoint.GetSnapPoint();
				
				const decDMatrix matrixTarget(
					decDMatrix::CreateRT(
						decDVector( gdcSnapPoint.GetRotation() * DEG2RAD ),
						decDVector( gdcSnapPoint.GetPosition() ) )
					* hitSnapPoint.GetObject()->GetObjectMatrix() );
				
				const decDMatrix matrixSource( undoObject.GetObjectMatrix() );
				
				const decDVector targetPosition( matrixTarget.GetPosition() );
				const decDVector sourcePosition( matrixSource.GetPosition() );
				
				if( gdcSnapPoint.GetSnapToRotation() ){
					const decDMatrix matrix( undoObject.GetObjectMatrix()
						* decDMatrix::CreateTranslation( -sourcePosition )
						* decDMatrix::CreateFromQuaternion(
							matrixTarget.ToQuaternion() * matrixSource.ToQuaternion().Conjugate() )
						* decDMatrix::CreateTranslation( targetPosition ) );
					
					position = matrix.GetPosition();
					rotation = matrix.GetEulerAngles() / DEG2RAD;
					
				}else{
					position = undoObject.GetPosition() + ( targetPosition - sourcePosition );
				}
				
				applySnapping = false;
				
			}else{
				rayDirection *= 3.0; // 3m
				position = rayPosition + rayDirection;
			}
		}
		
		// update undo
		undoObject.SetPosition( position );
		undoObject.SetRotation( rotation );
		//pUndoAddObject->RedoAction(); // overkill
		
		// snapping used only if no snap point is hit
		if( applySnapping && pCLSnapPoint->IsSnappingEnabled() ){
			pCLCollider->SetPosition( undoObject.GetPosition() );
			pCLCollider->SetOrientation( decQuaternion::CreateFromEuler(
				undoObject.GetRotation() * DEG2RAD ) );
			
			pCLSnapPoint->Reset();
			ColliderTestCollision( pCLSnapPoint, pCLCollider );
			
			if( pCLSnapPoint->GetTargetSnapPoint() ){
				if( pCLSnapPoint->GetTargetSnapPoint()->GetSnapToRotation() ){
					const decDMatrix matrix( undoObject.GetObjectMatrix()
						* pCLSnapPoint->CalcCorrectionMatrix() );
					undoObject.SetPosition( matrix.GetPosition() );
					undoObject.SetRotation( matrix.GetEulerAngles() / DEG2RAD );
					
				}else{
					undoObject.SetPosition( undoObject.GetPosition()
						+ pCLSnapPoint->CalcCorrectionMatrix().GetPosition() );
				}
			}
		}
		
		// notify object geometry changed
		GetWorld().NotifyObjectGeometryChanged( &undoObject );
		
	}else if( elementMode == meWorldGuiParameters::eemDecal ){
		if( ! pAddDecal ){
			DETHROW( deeInvalidParam );
		}
		
		rayDirection *= 50.0; // 50m
		
		pAddDecal->SetRay( rayPosition, rayDirection );
		pAddDecal->SetAlignWithNormal( ! shift );
		
		decLayerMask collisionCategory;
		collisionCategory.SetBit( meWorld::eclmEditing );
		
		decLayerMask collisionFilter;
		collisionFilter.SetBit( meWorld::eclmObjects );
		collisionFilter.SetBit( meWorld::eclmHeightTerrains );
		
		pAddDecal->Reset();
		RayTestCollision( pAddDecal, rayPosition, rayDirection, decCollisionFilter( collisionCategory, collisionFilter ) );
		pAddDecal->RunAction();
	}
}
