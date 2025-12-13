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

#include <stdlib.h>
#include <string.h>

#include "deovrRenderModel.h"
#include "deVROpenVR.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/model/deModelBuilder.h>
#include <dragengine/resources/model/deModelLOD.h>
#include <dragengine/resources/model/deModelFace.h>
#include <dragengine/resources/model/deModelVertex.h>
#include <dragengine/resources/model/deModelTexture.h>
#include <dragengine/resources/model/deModelTextureCoordinatesSet.h>



// Class deovrRenderModel
///////////////////////////

// Constructor, destructor
////////////////////////////

deovrRenderModel::deovrRenderModel(deVROpenVR &ovr, const char *name) :
pOvr(ovr),
pName(name),
pTextureID(vr::INVALID_TEXTURE_ID)
{
	pLoadModel();
}

deovrRenderModel::~deovrRenderModel(){
}



// Management
///////////////



// Private Functions
//////////////////////

class deovrRenderModel_BuildModel : public deModelBuilder {
private:
	const vr::RenderModel_t &pRenderModel;
	
public:
	deovrRenderModel_BuildModel(const vr::RenderModel_t &renderModel) :
	pRenderModel(renderModel){}
	
	virtual void BuildModel(deModel *model){
		deModelLOD * const lod = new deModelLOD;
		model->AddLOD(lod);
		
		model->AddTexture(new deModelTexture("material", 256, 256));
		
		// add vertices
		int i;
		const int vertexCount = (int)pRenderModel.unVertexCount;
		
		lod->SetVertexCount(vertexCount);
		deModelVertex * const vertices = lod->GetVertices();
		
		for(i=0; i<vertexCount; i++){
			const vr::HmdVector3_t &p = pRenderModel.rVertexData[i].vPosition;
			vertices[i].SetPosition(decVector(p.v[0], p.v[1], -p.v[2]));
			// we can not use normals directly. instead we would have to match normals
			// along face boundaries to figure out the normal sets to use. for the time
			// being do full smoothing
		}
		
		lod->SetNormalCount(vertexCount);
		lod->SetTangentCount(vertexCount);
		
		// add faces
		const int faceCount = (int)pRenderModel.unTriangleCount;
		const uint16_t *corners = pRenderModel.rIndexData;
		
		lod->SetFaceCount(faceCount);
		deModelFace * const faces = lod->GetFaces();
		
		for(i=0; i<faceCount; i++){
			const int v3 = (int)*(corners++);
			const int v2 = (int)*(corners++);
			const int v1 = (int)*(corners++);
			
			faces[i].SetTexture(0);
			
			faces[i].SetVertex1(v1);
			faces[i].SetVertex2(v2);
			faces[i].SetVertex3(v3);
			
			faces[i].SetNormal1(v1);
			faces[i].SetNormal2(v2);
			faces[i].SetNormal3(v3);
			
			faces[i].SetTangent1(v1);
			faces[i].SetTangent2(v2);
			faces[i].SetTangent3(v3);
			
			faces[i].SetTextureCoordinates1(v1);
			faces[i].SetTextureCoordinates2(v2);
			faces[i].SetTextureCoordinates3(v3);
		}
		
		// add texture coordinates
		model->GetTextureCoordinatesSetList().Add("default");
		
		lod->SetTextureCoordinatesCount(vertexCount);
		
		lod->SetTextureCoordinatesSetCount(1);
		deModelTextureCoordinatesSet &tcset = lod->GetTextureCoordinatesSetAt(0);
		
		tcset.SetTextureCoordinatesCount(vertexCount);
		decVector2 * const tcs = tcset.GetTextureCoordinates();
		
		for(i=0; i<vertexCount; i++){
			const vr::RenderModel_Vertex_t &v = pRenderModel.rVertexData[i];
			tcs[i].Set(v.rfTextureCoord[0], v.rfTextureCoord[1]);
		}
	}
};

void deovrRenderModel::pLoadModel(){
	vr::IVRRenderModels &vrrm = pOvr.GetVRRenderModels();
	vr::RenderModel_t *renderModel = nullptr;
	vr::EVRRenderModelError error;
	
	pOvr.LogInfoFormat("Loading render model '%s'", pName.GetString());
	decTimer timer;
	
	while(true){
		error = vrrm.LoadRenderModel_Async(pName, &renderModel);
		if(error == vr::VRRenderModelError_None){
			break;
		}
		if(error != vr::VRRenderModelError_Loading){
			pOvr.LogErrorFormat("Failed loading render model '%s': %d", pName.GetString(), error);
			DETHROW_INFO(deeInvalidAction, "Failed loading render model");
		}
		// sleep a bit... but not here. well cache this later ( /cache/global/renderModels/<name>.demodel )
	}
	
	decPath path(decPath::CreatePathUnix("/openvr/renderModels"));
	path.AddComponent(pName + ".demodel");
	
	try{
		deovrRenderModel_BuildModel builder(*renderModel);
		pModel = pOvr.GetGameEngine()->GetModelManager()->CreateModel(path.GetPathUnix(), builder);
		
	}catch(const deException &){
		vrrm.FreeRenderModel(renderModel);
		throw;
	}
	
	pTextureID = renderModel->diffuseTextureId;
	
	vrrm.FreeRenderModel(renderModel);
	
	pOvr.LogInfoFormat("Loading render model '%s' finished in %.1fms",
		pName.GetString(), timer.GetElapsedTime() * 1e3f);
}
