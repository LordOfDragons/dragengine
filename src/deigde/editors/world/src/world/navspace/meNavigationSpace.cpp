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
#include <string.h>

#include "meNavigationSpace.h"
#include "../meWorld.h"
#include "../meWorldGuiParameters.h"
#include "../../utils/meModelBuilderNavSpace.h"
#include "../../utils/meModelBuilderBox.h"
#include "../../utils/meRigBuilderMeshCollision.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>
#include <deigde/gui/wrapper/igdeWObject.h>

#include <dragengine/deEngine.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentManager.h>
#include <dragengine/resources/debug/deDebugDrawerShapeFace.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/navigation/space/deNavigationSpace.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceEdge.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceCorner.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceFace.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceWall.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceRoom.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/rig/deRigManager.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define LOGSOURCE "World Editor"


 
// Class meNavigationSpace
/////////////////////////

// Constructor, destructor
////////////////////////////

meNavigationSpace::meNavigationSpace(igdeEnvironment *environment) :
pColliderOwner(this)
{
	if(!environment){
		DETHROW(deeInvalidParam);
	}
	
	pEnvironment = environment;
	
	pWorld = NULL;
	
	pDDSNavSpace = NULL;
	pEngNavSpace = NULL;
	pEngCollider = NULL;
	pEngColComponent = NULL;
	pEngRig = NULL;
	
	pSelected = false;
	pActive = false;
	
	deEngine * const engine = GetEngine(); // after setting pEnvironment or this call fails
	
	try{
		meRigBuilderMeshCollision rigBuilder;
		pEngRig.TakeOver(engine->GetRigManager()->CreateRig("", rigBuilder));
		
		pEngCollider.TakeOver(engine->GetColliderManager()->CreateColliderComponent());
		pEngCollider->SetEnabled(true);
		pEngCollider->SetResponseType(deCollider::ertStatic);
		pEngCollider->SetUseLocalGravity(true);
		
		decLayerMask collisionCategory;
		collisionCategory.SetBit(meWorld::eclmAI);
		
		decLayerMask collisionFilter;
		collisionFilter.SetBit(meWorld::eclmEditing);
		
		pEngCollider->SetCollisionFilter(decCollisionFilter(collisionCategory, collisionFilter));
		
		environment->SetColliderUserPointer(pEngCollider, &pColliderOwner);
		
		pEngNavSpace.TakeOver(engine->GetNavigationSpaceManager()->CreateNavigationSpace());
		
		// create debug drawer and shapes
		pDebugDrawer.TakeOver(engine->GetDebugDrawerManager()->CreateDebugDrawer());
		pDebugDrawer->SetXRay(true);
		
		pDDSNavSpace = new igdeWDebugDrawerShape;
		pDDSNavSpace->SetVisible(true);
		pDDSNavSpace->SetParentDebugDrawer(pDebugDrawer);
		
		pUpdateDDSColors();
		pUpdateShapes();
		
		pObjectPlaceholder.TakeOverWith(*environment);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

meNavigationSpace::~meNavigationSpace(){
	pCleanUp();
}



// Management
///////////////

deEngine *meNavigationSpace::GetEngine() const{
	return pEnvironment->GetEngineController()->GetEngine();
}



void meNavigationSpace::SetWorld(meWorld *world){
	if(pWorld == world){
		return;
	}
	
	if(pWorld){
		deWorld &engWorld = *pWorld->GetEngineWorld();
		engWorld.RemoveCollider(pEngCollider);
		engWorld.RemoveNavigationSpace(pEngNavSpace);
		engWorld.RemoveDebugDrawer(pDebugDrawer);
		pObjectPlaceholder->SetWorld(NULL);
	}
	
	pWorld = world;
	
	if(world){
		deWorld &engWorld = *world->GetEngineWorld();
		engWorld.AddCollider(pEngCollider);
		engWorld.AddNavigationSpace(pEngNavSpace);
		engWorld.AddDebugDrawer(pDebugDrawer);
		pObjectPlaceholder->SetWorld(&engWorld);
	}
	
	ShowStateChanged();
}



void meNavigationSpace::SetPosition(const decDVector &position){
	if(pPosition.IsEqualTo(position)){
		return;
	}
	
	pPosition = position;
	
	pEngNavSpace->SetPosition(position);
	pEngCollider->SetPosition(position);
	if(pEngColComponent){
		pEngColComponent->SetPosition(position);
	}
	pDebugDrawer->SetPosition(position);
	
	NotifyGeometryChanged();
}

void meNavigationSpace::SetOrientation(const decVector &orientation){
	if(orientation.IsEqualTo(pOrientation)){
		return;
	}
	
	const decQuaternion realor = decQuaternion::CreateFromEuler(orientation * DEG2RAD);
	
	pOrientation = orientation;
	
	pEngNavSpace->SetOrientation(realor);
	pEngCollider->SetOrientation(realor);
	if(pEngColComponent){
		pEngColComponent->SetOrientation(realor);
	}
	pDebugDrawer->SetOrientation(realor);
	
	NotifyGeometryChanged();
}

void meNavigationSpace::SetFilename(const char *filename){
	if(pFilename == filename){
		return;
	}
	
	pFilename = filename;
	
	LoadFromFile();
	NotifyChanged();
}



void meNavigationSpace::SetSelected(bool selected){
	if(pSelected == selected){
		return;
	}
	
	pSelected = selected;
	pUpdateDDSColors();
}

void meNavigationSpace::SetActive(bool active){
	if(pActive == active){
		return;
	}
	
	pActive = active;
	pUpdateDDSColors();
}



void meNavigationSpace::LoadFromFile(){
	const char signatureCheck[] = "Drag[en]gine Navigation Space";
	decBaseFileReader *fileReader = NULL;
	int vertexCount = 0;
	int edgeCount = 0;
	int cornerCount = 0;
	int faceCount = 0;
	int wallCount = 0;
	int roomCount = 0;
	char signature[29];
	int version; //, flags;
	decVector position;
	decPath path;
	int i;
	
	// clear the navigation space
	pEngNavSpace->SetRoomCount(0);
	pEngNavSpace->SetWallCount(0);
	pEngNavSpace->SetFaceCount(0);
	pEngNavSpace->SetCornerCount(0);
	pEngNavSpace->SetEdgeCount(0);
	pEngNavSpace->SetVertexCount(0);
	pEngNavSpace->SetType(deNavigationSpace::estGrid);
	
	// load navigation space from file
	if(!pFilename.IsEmpty()){
		deEngine &engine = *GetEngine();
		
		path.SetFromUnix(pFilename.GetString());
		
		if(engine.GetVirtualFileSystem()->ExistsFile(path)){
			// load navigation space. if the load fails for some reason we stick to an empty space.
			try{
				fileReader = engine.GetVirtualFileSystem()->OpenFileForReading(path);
				
				fileReader->Read(&signature, 29);
				if(strncmp(&signature[0], &signatureCheck[0], 29) != 0){
					DETHROW_INFO(deeInvalidFileFormat, path.GetPathUnix());
				}
				
				version = (int)fileReader->ReadUShort();
				/*flags = ( int )*/fileReader->ReadUShort();
				
				if(version == 1){
					// read counts
					vertexCount = (int)fileReader->ReadUShort();
					edgeCount = (int)fileReader->ReadUShort();
					cornerCount = (int)fileReader->ReadUShort();
					faceCount = (int)fileReader->ReadUShort();
					wallCount = (int)fileReader->ReadUShort();
					roomCount = (int)fileReader->ReadUShort();
					
					pEngNavSpace->SetType((deNavigationSpace::eSpaceTypes)fileReader->ReadUShort());
					
					pEngNavSpace->SetVertexCount(vertexCount);
					pEngNavSpace->SetEdgeCount(edgeCount);
					pEngNavSpace->SetCornerCount(cornerCount);
					pEngNavSpace->SetFaceCount(faceCount);
					pEngNavSpace->SetWallCount(wallCount);
					pEngNavSpace->SetRoomCount(roomCount);
					
					// read vertices
					for(i=0; i<vertexCount; i++){
						position.x = fileReader->ReadFloat();
						position.y = fileReader->ReadFloat();
						position.z = fileReader->ReadFloat();
						pEngNavSpace->SetVertexAt(i, position);
					}
					
					// read edges
					for(i=0; i<edgeCount; i++){
						deNavigationSpaceEdge &edge = pEngNavSpace->GetEdgeAt(i);
						edge.SetVertex1(fileReader->ReadUShort());
						edge.SetVertex2(fileReader->ReadUShort());
						edge.SetType1(fileReader->ReadUShort());
						edge.SetType2(fileReader->ReadUShort());
					}
					
					// read corners
					for(i=0; i<cornerCount; i++){
						deNavigationSpaceCorner &corner = pEngNavSpace->GetCornerAt(i);
						corner.SetVertex(fileReader->ReadUShort());
						corner.SetType(fileReader->ReadUShort());
					}
					
					// read faces
					for(i=0; i<faceCount; i++){
						deNavigationSpaceFace &face = pEngNavSpace->GetFaceAt(i);
						face.SetCornerCount(fileReader->ReadUShort());
						face.SetType(fileReader->ReadUShort());
					}
					
					// read walls
					for(i=0; i<wallCount; i++){
						deNavigationSpaceWall &wall = pEngNavSpace->GetWallAt(i);
						wall.SetFace(fileReader->ReadUShort());
						wall.SetType(fileReader->ReadUShort());
					}
					
					// read rooms
					for(i=0; i<roomCount; i++){
						deNavigationSpaceRoom &room = pEngNavSpace->GetRoomAt(i);
						room.SetFrontWallCount(fileReader->ReadUShort());
						room.SetBackWallCount(fileReader->ReadUShort());
						room.SetType(fileReader->ReadUShort());
					}
				}
				
				fileReader->FreeReference();
				
			}catch(const deException &e){
				if(fileReader){
					fileReader->FreeReference();
				}
				
				pEngNavSpace->SetRoomCount(0);
				pEngNavSpace->SetWallCount(0);
				pEngNavSpace->SetFaceCount(0);
				pEngNavSpace->SetCornerCount(0);
				pEngNavSpace->SetEdgeCount(0);
				pEngNavSpace->SetVertexCount(0);
				pEngNavSpace->SetType(deNavigationSpace::estGrid);
				
				if(pWorld){
					pWorld->GetLogger()->LogException(LOGSOURCE, e);
				}
			}
		}
	}
	
	// if the loading failed or the navigation space is empty show a placeholder box so the
	// user can still interact with the navigation space
	if(pEngNavSpace->GetRoomCount() > 0 || pEngNavSpace->GetFaceCount() > 0 || pEngNavSpace->GetEdgeCount() > 0){
		pObjectPlaceholder->SetGDClass(NULL);
		
	}else{
		pObjectPlaceholder->SetGDClassName("IGDEPlaceholderBoxProblemNavSpace");
	}
	
	// in all cases notify the peers that the navigation space changed
	pEngNavSpace->NotifyLayoutChanged();
	
	pUpdateShapes();
	pUpdateDDSSpace();
	
	pUpdateUsedCostTypes();
}

void meNavigationSpace::SaveToFile(){
	if(!pWorld || pFilename.IsEmpty() || !pEngNavSpace){
		DETHROW(deeInvalidParam);
	}
	
	deVirtualFileSystem &vfs = *GetEngine()->GetVirtualFileSystem();
	decBaseFileWriter *fileWriter = NULL;
	const char signature[] = "Drag[en]gine Navigation Space";
	int i, count;
	decPath path;
	
	path.SetFromUnix(pFilename.GetString());
	
	try{
		fileWriter = vfs.OpenFileForWriting(path);
		
		fileWriter->Write(&signature, 29);
		fileWriter->WriteUShort(1); // version
		fileWriter->WriteUShort(0); // flags
		fileWriter->WriteUShort((unsigned short)pEngNavSpace->GetVertexCount());
		fileWriter->WriteUShort((unsigned short)pEngNavSpace->GetEdgeCount());
		fileWriter->WriteUShort((unsigned short)pEngNavSpace->GetCornerCount());
		fileWriter->WriteUShort((unsigned short)pEngNavSpace->GetFaceCount());
		fileWriter->WriteUShort((unsigned short)pEngNavSpace->GetWallCount());
		fileWriter->WriteUShort((unsigned short)pEngNavSpace->GetRoomCount());
		fileWriter->WriteUShort((unsigned short)pEngNavSpace->GetType());
		
		count = pEngNavSpace->GetVertexCount();
		for(i=0; i<count; i++){
			fileWriter->WriteVector(pEngNavSpace->GetVertexAt(i));
		}
		
		count = pEngNavSpace->GetEdgeCount();
		for(i=0; i<count; i++){
			const deNavigationSpaceEdge &edge = pEngNavSpace->GetEdgeAt(i);
			fileWriter->WriteUShort(edge.GetVertex1());
			fileWriter->WriteUShort(edge.GetVertex2());
			fileWriter->WriteUShort((unsigned short)edge.GetType1());
			fileWriter->WriteUShort((unsigned short)edge.GetType2());
		}
		
		count = pEngNavSpace->GetCornerCount();
		for(i=0; i<count; i++){
			const deNavigationSpaceCorner &corner = pEngNavSpace->GetCornerAt(i);
			fileWriter->WriteUShort(corner.GetVertex());
			fileWriter->WriteUShort(corner.GetType());
		}
		
		count = pEngNavSpace->GetFaceCount();
		for(i=0; i<count; i++){
			const deNavigationSpaceFace &face = pEngNavSpace->GetFaceAt(i);
			fileWriter->WriteUShort(face.GetCornerCount());
			fileWriter->WriteUShort((unsigned short)face.GetType());
		}
		
		count = pEngNavSpace->GetWallCount();
		for(i=0; i<count; i++){
			const deNavigationSpaceWall &wall = pEngNavSpace->GetWallAt(i);
			fileWriter->WriteUShort(wall.GetFace());
			fileWriter->WriteUShort(wall.GetType());
		}
		
		count = pEngNavSpace->GetRoomCount();
		for(i=0; i<count; i++){
			const deNavigationSpaceRoom &room = pEngNavSpace->GetRoomAt(i);
			fileWriter->WriteUShort(room.GetFrontWallCount());
			fileWriter->WriteUShort(room.GetBackWallCount());
			fileWriter->WriteUShort((unsigned short)room.GetType());
		}
		
		fileWriter->FreeReference();
		
	}catch(const deException &){
		if(fileWriter){
			fileWriter->FreeReference();
		}
		throw;
	}
}



void meNavigationSpace::NotifyChanged(){
	if(pWorld){
		pWorld->SetChanged(true);
		pWorld->NotifyNavSpaceChanged(this);
	}
}

void meNavigationSpace::NotifyGeometryChanged(){
	if(pWorld){
		pWorld->SetChanged(true);
		pWorld->NotifyNavSpaceGeometryChanged(this);
	}
}



void meNavigationSpace::ShowStateChanged(){
	if(!pWorld){
		return;
	}
	
	const meWorldGuiParameters &guiParams = pWorld->GetGuiParameters();
	const meWorldGuiParameters::eElementModes elementMode = guiParams.GetElementMode();
	
	pDDSNavSpace->SetVisible((elementMode == meWorldGuiParameters::eemNavSpace)
		|| guiParams.GetShowNavigationSpaces()
		|| (pActive && guiParams.GetShowNavigationSpacesSelected()));
}



// Private Functions
//////////////////////

void meNavigationSpace::pCleanUp(){
	SetWorld(NULL);
	
	pObjectPlaceholder = nullptr;
	
	if(pEngCollider){
		pEnvironment->SetColliderUserPointer(pEngCollider, NULL);
	}
	
	if(pDDSNavSpace){
		delete pDDSNavSpace;
	}
}



void meNavigationSpace::pUpdateShapes(){
	pEngCollider->SetComponent(NULL);
		pEngColComponent = NULL;
	}
	
	if(pEngNavSpace){
		deEngine &engine = *GetEngine();
		bool canBuild = false;
		deModel *model = NULL;
		deSkin *skin = NULL;
		
		if(pEngNavSpace->GetRoomCount() > 0 || pEngNavSpace->GetFaceCount() > 0 || pEngNavSpace->GetEdgeCount() > 0){
			if(pEngNavSpace->GetType() == deNavigationSpace::estGrid){
				canBuild = (pEngNavSpace->GetEdgeCount() > 0);
				
			}else if(pEngNavSpace->GetType() == deNavigationSpace::estMesh){
				canBuild = (pEngNavSpace->GetFaceCount() > 0);
			}
		}
		
		try{
			if(canBuild){
				meModelBuilderNavSpace builder(pEngNavSpace);
				model = engine.GetModelManager()->CreateModel("", builder);
				
			}else{
				meModelBuilderBox builder(0.25f);
				model = engine.GetModelManager()->CreateModel("", builder);
			}
			
			skin = engine.GetSkinManager()->LoadDefault();
			pEngColComponent.TakeOver(engine.GetComponentManager()->CreateComponent(model, skin));
			pEngColComponent->SetRig(pEngRig);
			
		}catch(const deException &){
			if(skin){
				skin->FreeReference();
			}
			if(model){
				model->FreeReference();
			}
			throw;
		}
		
		model->FreeReference();
		skin->FreeReference();
		
		pEngColComponent->SetPosition(pEngCollider->GetPosition());
		pEngColComponent->SetOrientation(pEngCollider->GetOrientation());
	}
	
	pEngCollider->SetComponent(pEngColComponent);
}

void meNavigationSpace::pUpdateDDSSpace(){
	pDDSNavSpace->RemoveAllFaces();
	
	if(pEngNavSpace){
		if(pEngNavSpace->GetType() == deNavigationSpace::estGrid){
			const int edgeCount = pEngNavSpace->GetEdgeCount();
			deDebugDrawerShapeFace *ddsFace = NULL;
			int e;
			
			try{
				for(e=0; e<edgeCount; e++){
					const deNavigationSpaceEdge &edge = pEngNavSpace->GetEdgeAt(e);
					const decVector &vertex1 = pEngNavSpace->GetVertexAt(edge.GetVertex1());
					const decVector &vertex2 = pEngNavSpace->GetVertexAt(edge.GetVertex2());
					
					ddsFace = new deDebugDrawerShapeFace;
					ddsFace->AddVertex(vertex1);
					ddsFace->AddVertex(vertex2);
					ddsFace->AddVertex(vertex2);
					pDDSNavSpace->AddFace(ddsFace);
				}
				
			}catch(const deException &){
				throw;
			}
			
		}else if(pEngNavSpace->GetType() == deNavigationSpace::estMesh){
			deNavigationSpaceCorner *corners = pEngNavSpace->GetCorners();
			const int faceCount = pEngNavSpace->GetFaceCount();
			deDebugDrawerShapeFace *ddsFace = NULL;
			int f, c;
			
			try{
				for(f=0; f<faceCount; f++){
					const deNavigationSpaceFace &face = pEngNavSpace->GetFaceAt(f);
					const int cornerCount = face.GetCornerCount();
					
					if(cornerCount > 2){
						ddsFace = new deDebugDrawerShapeFace;
						for(c=0; c<cornerCount; c++){
							ddsFace->AddVertex(pEngNavSpace->GetVertexAt(corners[c].GetVertex()));
						}
						ddsFace->SetNormal((ddsFace->GetVertexAt(1) - ddsFace->GetVertexAt(0)) % (ddsFace->GetVertexAt(2) - ddsFace->GetVertexAt(1)));
						pDDSNavSpace->AddFace(ddsFace);
					}
					
					corners += cornerCount;
				}
				
			}catch(const deException &){
				throw;
			}
		}
	}
}

void meNavigationSpace::pUpdateDDSColors(){
	if(pActive){
		pDDSNavSpace->SetEdgeColor(decColor(1.0f, 0.5f, 0.0f, 1.0f));
		pDDSNavSpace->SetFillColor(decColor(1.0f, 0.5f, 0.0f, 0.1f));
		
	}else if(pSelected){
		pDDSNavSpace->SetEdgeColor(decColor(1.0f, 0.0f, 0.0f, 1.0f));
		pDDSNavSpace->SetFillColor(decColor(1.0f, 0.0f, 0.0f, 0.1f));
		
	}else{
		pDDSNavSpace->SetEdgeColor(decColor(0.0f, 0.25f, 1.0f, 1.0f));
		pDDSNavSpace->SetFillColor(decColor(0.0f, 0.25f, 1.0f, 0.1f));
	}
}



void meNavigationSpace::pUpdateUsedCostTypes(){
	pUsedCostTypes.RemoveAll();
	
	if(pEngNavSpace){
		const int faceCount = pEngNavSpace->GetFaceCount();
		const int edgeCount = pEngNavSpace->GetEdgeCount();
		int i;
		
		for(i=0; i<faceCount; i++){
			const int navtype = pEngNavSpace->GetFaceAt(i).GetType();
			
			if(!pUsedCostTypes.Has(navtype)){
				pUsedCostTypes.Add(navtype);
			}
		}
		
		for(i=0; i<edgeCount; i++){
			const int navtype1 = pEngNavSpace->GetEdgeAt(i).GetType1();
			const int navtype2 = pEngNavSpace->GetEdgeAt(i).GetType2();
			
			if(!pUsedCostTypes.Has(navtype1)){
				pUsedCostTypes.Add(navtype1);
			}
			if(!pUsedCostTypes.Has(navtype2)){
				pUsedCostTypes.Add(navtype2);
			}
		}
	}
	
	if(pWorld){
		pWorld->NotifyNavSpaceUsedCostTypesChanged(this);
	}
}
