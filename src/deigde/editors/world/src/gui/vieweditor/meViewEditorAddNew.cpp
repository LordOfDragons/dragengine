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
#include <dragengine/logger/deLogger.h>



// Class meViewEditorAddNew
/////////////////////////////

// Constructor, destructor
////////////////////////////

meViewEditorAddNew::meViewEditorAddNew(meView3D &view) :
meViewEditorNavigation(view),
pClosestElement(nullptr),
pCLSnapPoint(nullptr),
pFilterObjectsByClass(nullptr),
pAddDecal(nullptr),
pUndoAddObject(nullptr),
pUndoAddNavSpace(nullptr)
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
	
	pUndoAddObject = nullptr;
	pUndoAddNavSpace = nullptr;
	
	if( pCLSnapPoint ){
		delete pCLSnapPoint;
		pCLSnapPoint = nullptr;
	}
	
	if( elementMode == meWorldGuiParameters::eemObject ){
		if(guiparams.GetBrowseClass().IsEmpty()){
			return;
		}
		
		pCreateClosestElementVisitor();
		
		meObject *object = nullptr;
		
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
			pUndoAddObject = nullptr;
			
			delete pClosestElement;
			pClosestElement = nullptr;
			
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
		pCreateClosestElementVisitor();
		
		const meNavigationSpace::Ref navspace(meNavigationSpace::Ref::New(
			new meNavigationSpace(world.GetEnvironment())));
		
		try{
			pUndoAddNavSpace.TakeOver(new meUAddNavSpace(&world, decPoint3(), navspace));
			pUndoAddNavSpace->Redo(); // from here on we only manipulate the object parameters
			
		}catch(const deException &e){
			pUndoAddNavSpace = nullptr;
			LogException( e );
			return;
		}
		
		world.GetSelectionNavigationSpace().RemoveAll();
		world.GetSelectionNavigationSpace().Add(navspace);
		world.GetSelectionNavigationSpace().SetActive(navspace);
		world.NotifyNavSpaceSelectionChanged();
		
		pUpdateUndo(shift, control);
	}
}

void meViewEditorAddNew::OnLeftMouseButtonRelease( int x, int y, bool shift, bool control ){
	meViewEditorNavigation::OnLeftMouseButtonRelease( x, y, shift, control );
	
	if( pUndoAddNavSpace ){
		GetWorld().GetUndoSystem()->Add( pUndoAddNavSpace, false );
		pUndoAddNavSpace = nullptr;
	}
	if( pUndoAddObject ){
		GetWorld().GetUndoSystem()->Add( pUndoAddObject, false );
		pUndoAddObject = nullptr;
	}
	
	if( pClosestElement ){
		delete pClosestElement;
		pClosestElement = nullptr;
	}
	
	if( pAddDecal ){
		pAddDecal->Finish();
		delete pAddDecal;
		pAddDecal = nullptr;
	}
	
	if( pCLSnapPoint ){
		delete pCLSnapPoint;
		pCLSnapPoint = nullptr;
	}
}

void meViewEditorAddNew::OnMouseMove( int x, int y, bool shift, bool control ){
	meViewEditorNavigation::OnMouseMove( x, y, shift, control );
	
	/*if( pUndoAddObject ){
		pUndoAddObject->Undo(); // overkill
	}*/
	
	if(pClosestElement || pAddDecal){
		pUpdateUndo(shift, control);
	}
}



void meViewEditorAddNew::pUpdateUndo( bool shift, bool control ){
	const meCamera &camera = GetActiveCamera();
	const int elementMode = GetElementMode();
	const decPoint& pt = GetDragCurrent();
	
	decDVector rayDirection = camera.GetDirectionFor( GetViewWidth(), GetViewHeight(), pt.x, pt.y );
	const decDVector rayPosition = camera.GetViewMatrix().GetPosition();
	
	if( elementMode == meWorldGuiParameters::eemObject ){
		if(!pUndoAddObject || !pUndoAddObject->GetObject()){
			return;
		}
		
		meObject &undoObject = *pUndoAddObject->GetObject();
		sCastIntoWorldParams params{};
		params.originalMatrix = undoObject.GetObjectMatrix();
		params.rayDirection = rayDirection;
		params.rayPosition = rayPosition;
		params.shift = shift;
		params.control = control;
		params.useSnapPoint = true;
		pCastIntoWorld(params);
		
		undoObject.SetPosition(params.position);
		undoObject.SetRotation(params.rotation);
		//pUndoAddObject->RedoAction(); // overkill
		
		GetWorld().NotifyObjectGeometryChanged( &undoObject );
		
	}else if(elementMode == meWorldGuiParameters::eemNavSpace){
		if(!pUndoAddNavSpace || !pUndoAddNavSpace->GetNavSpace()){
			return;
		}
		
		meNavigationSpace &undoNavSpace = *pUndoAddNavSpace->GetNavSpace();
		sCastIntoWorldParams params{};
		params.originalMatrix.SetRT(decDVector(undoNavSpace.GetOrientation() * DEG2RAD),
			undoNavSpace.GetPosition());
		params.rayDirection = rayDirection;
		params.rayPosition = rayPosition;
		params.shift = shift;
		params.control = control;
		params.useSnapPoint = false;
		pCastIntoWorld(params);
		
		undoNavSpace.SetPosition(params.position);
		undoNavSpace.SetOrientation(params.rotation);
		
		GetWorld().NotifyNavSpaceGeometryChanged(&undoNavSpace);
		
	}else if(elementMode == meWorldGuiParameters::eemDecal){
		DEASSERT_NOTNULL(pAddDecal)
		
		rayDirection *= 50.0; // 50m
		
		pAddDecal->SetRay(rayPosition, rayDirection);
		pAddDecal->SetAlignWithNormal(!shift);
		
		decLayerMask collisionCategory;
		collisionCategory.SetBit(meWorld::eclmEditing);
		
		decLayerMask collisionFilter;
		collisionFilter.SetBit(meWorld::eclmObjects);
		collisionFilter.SetBit(meWorld::eclmHeightTerrains);
		
		pAddDecal->Reset();
		RayTestCollision(pAddDecal, rayPosition, rayDirection,
			decCollisionFilter(collisionCategory, collisionFilter));
		pAddDecal->RunAction();
	}
}

void meViewEditorAddNew::pCreateClosestElementVisitor(){
	DEASSERT_NULL(pClosestElement)
	
	pClosestElement = new meCLClosestElement(GetWorld());
	
	pFilterObjectsByClass = new meFilterObjectsByClass;
	pFilterObjectsByClass->SetRejectGhosts(true);
	//pFilterObjectsByClass->SetClassNamesFrom(guiparams.GetAddFilterObjectList());
	//pFilterObjectsByClass->SetMatchInclusive(guiparams.GetAddFilterObjectInclusive());
	
	// TODO: the height terrain is currently hit even if there is no texture on
	// it or it has a hole. the test has to be refined to avoid scoring a hit
	// on the height terrain if there is no texture or a hole
	pClosestElement->SetTestHeightTerrain( true );
	pClosestElement->SetTestObjects( true );
	pClosestElement->SetFilterObjects( pFilterObjectsByClass );
}

void meViewEditorAddNew::pCastIntoWorld(sCastIntoWorldParams &params){
	DEASSERT_NOTNULL(pClosestElement)
	
	const meWorldGuiParameters &guiparams = GetWorldGuiParameters();
	bool applySnapping = params.useSnapPoint;
	
	if(params.control){
		params.rayDirection *= 3.0; // 3m
		params.position = params.rayPosition + params.rayDirection;
		
	}else{
		params.rayDirection *= 50.0; // 50m
		
		decLayerMask collisionCategory;
		collisionCategory.SetBit(meWorld::eclmEditing);
		
		decLayerMask collisionFilter;
		collisionFilter.SetBit(meWorld::eclmObjects);
		collisionFilter.SetBit(meWorld::eclmHeightTerrains);
		if(params.useSnapPoint){
			collisionFilter.SetBit(meWorld::eclmSnapPoint);
		}
		
		pClosestElement->Reset();
		pClosestElement->SetTestSnapPoints(params.useSnapPoint
			&& pCLCollider && pCLSnapPoint && pCLSnapPoint->IsSnappingEnabled());
		
		RayTestCollision(pClosestElement, params.rayPosition, params.rayDirection,
			decCollisionFilter(collisionCategory, collisionFilter));
		
		if(pClosestElement->GetHitObject()){
			params.position = params.rayPosition
				+ params.rayDirection * (double)pClosestElement->GetHitDistance();
			
			decVector view(GetMatrixView().TransformView()),
				up(params.shift ? pClosestElement->GetHitNormal() : decVector(0.0f, 1.0f, 0.0f));
			
			if(fabsf(up * view) > cosf(5.0f * DEG2RAD)){
				up = GetMatrixView().TransformUp();
			}
			
			view = (up % view) % up;
			params.rotation = decMatrix::CreateVU(view, up).GetEulerAngles() * RAD2DEG;
			
		}else if(pClosestElement->GetHitSnapPoint()){
			const meObjectSnapPoint &hitSnapPoint = *pClosestElement->GetHitSnapPoint();
			const igdeGDCSnapPoint &gdcSnapPoint = *hitSnapPoint.GetSnapPoint();
			
			const decDMatrix matrixTarget(
				decDMatrix::CreateRT(
					decDVector(gdcSnapPoint.GetRotation() * DEG2RAD),
					decDVector(gdcSnapPoint.GetPosition()))
				* hitSnapPoint.GetObject()->GetObjectMatrix());
			
			const decDMatrix matrixSource(params.originalMatrix);
			
			const decDVector targetPosition(matrixTarget.GetPosition());
			const decDVector sourcePosition(matrixSource.GetPosition());
			
			if(gdcSnapPoint.GetSnapToRotation()){
				const decDMatrix matrix(params.originalMatrix
					* decDMatrix::CreateTranslation(-sourcePosition)
					* decDMatrix::CreateFromQuaternion(
						matrixTarget.ToQuaternion() * matrixSource.ToQuaternion().Conjugate())
					* decDMatrix::CreateTranslation(targetPosition));
				
				params.position = matrix.GetPosition();
				params.rotation = matrix.GetEulerAngles() * RAD2DEG;
				
			}else{
				params.position = params.originalMatrix.GetPosition()
					+ (targetPosition - sourcePosition);
			}
			
			applySnapping = false;
			
		}else{
			params.rayDirection *= 3.0; // 3m
			params.position = params.rayPosition + params.rayDirection;
		}
	}
	
	if(guiparams.GetAddRandomizeYAxis()){
		params.rotation.y += decMath::random(-180.0f, 180.0f);
	}
	
	// snapping used only if no snap point is hit
	if(applySnapping && pCLSnapPoint && pCLSnapPoint->IsSnappingEnabled() && pCLCollider){
		pCLCollider->SetPosition(params.originalMatrix.GetPosition());
		pCLCollider->SetOrientation(params.originalMatrix.ToQuaternion());
		
		pCLSnapPoint->Reset();
		ColliderTestCollision(pCLSnapPoint, pCLCollider);
		
		if(pCLSnapPoint->GetTargetSnapPoint()){
			if(pCLSnapPoint->GetTargetSnapPoint()->GetSnapToRotation()){
				const decDMatrix matrix(params.originalMatrix * pCLSnapPoint->CalcCorrectionMatrix());
				params.position = matrix.GetPosition();
				params.rotation = matrix.GetEulerAngles() * RAD2DEG;
				
			}else{
				params.position = params.originalMatrix.GetPosition()
					+ pCLSnapPoint->CalcCorrectionMatrix().GetPosition();
			}
		}
	}
}
