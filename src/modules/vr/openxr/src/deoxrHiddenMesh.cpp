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

#include "deoxrHiddenMesh.h"
#include "deoxrSystem.h"
#include "deoxrInstance.h"
#include "deVROpenXR.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/model/deModelBuilder.h>
#include <dragengine/resources/model/deModelLOD.h>
#include <dragengine/resources/model/deModelFace.h>
#include <dragengine/resources/model/deModelVertex.h>
#include <dragengine/resources/model/deModelTexture.h>
#include <dragengine/resources/model/deModelTextureCoordinatesSet.h>



// Class deoxrHiddenMesh
///////////////////////////

// Constructor, destructor
////////////////////////////

deoxrHiddenMesh::deoxrHiddenMesh(deoxrSession &session,
	XrViewConfigurationType viewConfig, uint32_t viewIndex) :
pSession(session),
pViewConfig(viewConfig),
pViewIndex(viewIndex){
}

deoxrHiddenMesh::~deoxrHiddenMesh() = default;



// Management
///////////////

void deoxrHiddenMesh::SetFov(XrFovf fov){
	pFov = fov;
}



class deoxrHiddenMesh_BuildModel : public deModelBuilder {
private:
	const XrVisibilityMaskKHR &pMask;
	
public:
	deoxrHiddenMesh_BuildModel(const XrVisibilityMaskKHR &mask) : pMask(mask){}
	
	void BuildModel(deModel *model) override{
		auto lod = deModelLOD::Ref::New();
		
		model->AddTexture(deModelTexture::Ref::New("material", 256, 256));
		model->GetTextures().Last()->SetDoubleSided(true);
		
		// add vertices
		lod->GetVertices().SetCountDiscard((int)pMask.vertexCountOutput);
		lod->GetVertices().VisitIndexed([&](int i, deModelVertex &vertex){
			const XrVector2f &p = pMask.vertices[i];
			vertex.SetPosition(decVector(p.x, p.y, 0.0f));
		});
		
		lod->SetNormalCount(1);
		lod->SetTangentCount(1);
		
		// add faces
		lod->GetFaces().SetCountDiscard((int)(pMask.indexCountOutput / 3));
		lod->GetFaces().VisitIndexed([&](int i, deModelFace &face){
			face.SetTexture(0);
			
			face.SetVertex1((int)pMask.indices[i * 3]);
			face.SetVertex2((int)pMask.indices[i * 3 + 1]);
			face.SetVertex3((int)pMask.indices[i * 3 + 2]);
			
			face.SetNormal1(0);
			face.SetNormal2(0);
			face.SetNormal3(0);
			
			face.SetTangent1(0);
			face.SetTangent2(0);
			face.SetTangent3(0);
			
			face.SetTextureCoordinates1(0);
			face.SetTextureCoordinates2(0);
			face.SetTextureCoordinates3(0);
		});
		
		// add texture coordinates
		model->GetTextureCoordinatesSets().Add("default");
		
		lod->SetTextureCoordinatesCount(1);
		
		lod->GetTextureCoordinatesSets().Add({});
		deModelTextureCoordinatesSet &tcset = lod->GetTextureCoordinatesSets().Last();
		
		tcset.GetTextureCoordinates().SetAll(1, {});
		
		model->AddLOD(std::move(lod));
	}
};

void deoxrHiddenMesh::UpdateModel(){
	pModel.Clear();
	
	deVROpenXR &oxr = pSession.GetSystem().GetInstance().GetOxr();
	
	oxr.LogInfoFormat("Loading hidden mesh for view config %d for view %d", pViewConfig, pViewIndex);
	
	XrVisibilityMaskKHR mask{XR_TYPE_VISIBILITY_MASK_KHR};
	decTList<XrVector2f> vertices;
	decTList<uint32_t> indices;
	
	// fetch and process mask
	pFetchData(mask, vertices, indices);
	
	if(mask.vertexCountOutput == 0 || mask.indexCountOutput == 0){
		return;
	}
	
	pProjectVertices(mask);
	pMapVerticesToWindow(mask);
	pFitVertices(mask);
	
	// create model
	decString path;
	path.Format("/openxr/hiddenMesh/config%d_view%d.demodel", pViewConfig, pViewIndex);
	
	deoxrHiddenMesh_BuildModel builder(mask);
	pModel = oxr.GetGameEngine()->GetModelManager()->CreateModel(path, builder);
}



// Private Functions
//////////////////////

void deoxrHiddenMesh::pFetchData(XrVisibilityMaskKHR &mask,
decTList<XrVector2f> &vertices, decTList<uint32_t> &indices) const{
	deoxrInstance &instance = pSession.GetSystem().GetInstance();
	
	// get counts
	OXR_CHECK(instance.xrGetVisibilityMaskKHR(pSession.GetSession(),
		pViewConfig, pViewIndex, XR_VISIBILITY_MASK_TYPE_HIDDEN_TRIANGLE_MESH_KHR, &mask));
	
	if(mask.vertexCountOutput == 0 || mask.indexCountOutput == 0){
		return;
	}
	
	// allocate temporary memory and get data
	vertices.SetCountDiscard(mask.vertexCountOutput);
	mask.vertices = vertices.GetArrayPointer();
	
	indices.SetCountDiscard(mask.indexCountOutput);
	mask.indices = indices.GetArrayPointer();
	
	mask.vertexCapacityInput = mask.vertexCountOutput;
	mask.indexCapacityInput = mask.indexCountOutput;
	
	OXR_CHECK(instance.xrGetVisibilityMaskKHR(pSession.GetSession(),
		pViewConfig, pViewIndex, XR_VISIBILITY_MASK_TYPE_HIDDEN_TRIANGLE_MESH_KHR, &mask));
}

void deoxrHiddenMesh::pProjectVertices(XrVisibilityMaskKHR &mask) const{
	// calculate scaling. according to specification the returned triangle mesh
	// is an actual 3d mesh with the implicit z coordinate -1 . the graphic module
	// expects the mesh though in image space. we have to calculate the projection
	// to the image plane to apply the correct scaling to fix the values
	// 
	// since z is -1 (or rather 1 in dragengine coordinate system) the division by
	// w is not required. we only have to deal with the x and y scaling which is:
	// 
	//   idx = 1 / (projection.right - projection.left)
	//   idy = 1 / (projection.bottom - projection.top)
	//   sx = projection.right + projection.left
	//   sy = projection.bottom + projection.top
	//   
	//   a11 = 2 * idx
	//   a13 = -sx * idx
	//   a22 = 2 * idy
	//   a23 = -sy * idy
	// 
	//   x' = x*a11 + z*a13 = x*a11 + a13
	//   y' = y*a22 + z*a23 = y*a22 + a23
	// 
	// keep in mind openxr uses angles for projection so the projection values above
	// are actually tan(x) (and up/down is flipped).
	// 
	// no idea why they made it so complicated. it is a view hidden mesh. a 2d mesh
	// would have been more than enough
	const float projLeft = tanf(pFov.angleLeft);
	const float projRight = tanf(pFov.angleRight);
	const float projTop = tanf(-pFov.angleUp);
	const float projBottom = tanf(-pFov.angleDown);
	
	const float idx = 1.0f / (projRight - projLeft);
	const float idy = 1.0f / (projBottom - projTop);
	const float sx = projRight + projLeft;
	const float sy = projBottom + projTop;
	
	const decVector2 scale(2.0f * idx, 2.0f * idy);
	const decVector2 offset(-sx * idx, -sy * idy);
	
	// project vertices
	uint32_t i;
	for(i=0; i<mask.vertexCountOutput; i++){
		XrVector2f &p = mask.vertices[i];
		p.x = p.x * scale.x + offset.x;
		p.y = p.y * scale.y + offset.y;
	}
}

void deoxrHiddenMesh::pMapVerticesToWindow(XrVisibilityMaskKHR &mask) const{
	uint32_t i;
	for(i=0; i<mask.vertexCountOutput; i++){
		XrVector2f &p = mask.vertices[i];
		p.x = p.x * 0.5f + 0.5f;
		p.y = p.y * 0.5f + 0.5f;
	}
}

void deoxrHiddenMesh::pFitVertices(XrVisibilityMaskKHR &mask) const{
	// hidden mask returned by openxr are often slightly off by a pixel or two.
	// this leaves small gaps which are rendered which is suboptimal.
	// stretch the mask to fit tightly into the window.
	float minX = 0.5f, maxX = 0.5f, minY = 0.5f, maxY = 0.5f;
	uint32_t i;
	
	for(i=0; i<mask.vertexCountOutput; i++){
		const XrVector2f &p = mask.vertices[i];
		
		if(p.x < minX){
			minX = p.x;
			
		}else if(p.x > maxX){
			maxX = p.x;
		}
		
		if(p.y < minY){
			minY = p.y;
			
		}else if(p.y > maxY){
			maxY = p.y;
		}
	}
	
	const float scaleX = 1.0f / (maxX - minX);
	const float scaleY = 1.0f / (maxY - minY);
	const float offsetX = scaleX * minX;
	const float offsetY = scaleY * minY;
	
	for(i=0; i<mask.vertexCountOutput; i++){
		XrVector2f &p = mask.vertices[i];
		p.x = p.x * scaleX - offsetX;
		p.y = p.y * scaleY - offsetY;
	}
}
