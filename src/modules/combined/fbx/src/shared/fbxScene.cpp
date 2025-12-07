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
#include <inttypes.h>

#include "fbxConnection.h"
#include "fbxNode.h"
#include "fbxScene.h"
#include "fbxProperty.h"
#include "fbxObjectMap.h"
#include "fbxConnectionMap.h"
#include "property/fbxPropertyString.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/deBaseModule.h>



// Definitions
////////////////

// FBX scale "1 unit = 1cm". but people seem to use it like "1 unit = 1m"... dilemma!
#define FBX_UNIT_SCALE 0.01f
// #define FBX_UNIT_SCALE 1.0f


// Class fbxScene
///////////////////

// Constructor, destructor
////////////////////////////

fbxScene::fbxScene() :
pVersion(0),
pNode(new fbxNode),
pUpAxis(eaYPos),
pFrontAxis(eaZNeg),
pCoordAxis(eaXPos),
pUnitScaleFactor(1.0f),
pNodeObjects(NULL),
pNodeConnections(NULL),
pObjectMap(NULL),
pConnectionMap(NULL){
}

fbxScene::fbxScene(decBaseFileReader &reader) :
pVersion(0),
pNode(new fbxNode),
pUpAxis(eaYPos),
pFrontAxis(eaZNeg),
pCoordAxis(eaXPos),
pUnitScaleFactor(1.0f),
pNodeObjects(NULL),
pNodeConnections(NULL),
pObjectMap(NULL),
pConnectionMap(NULL)
{
	// header
	char signature[21]; // 0-terminator at index 20 included !
	reader.Read(signature, 21);
	if(strcmp("Kaydara FBX Binary  ", signature) != 0){ // compare including 0-terminator
		DETHROW_INFO(deeInvalidFileFormat, "signature mismatch");
	}
	
	reader.SkipUShort(); // [0x1A, 0x00] (unknown but all observed files show these bytes)
	
	// written against version 7300. version 7500 introduces binary breaking changes.
	// from blender addon:
	// 
	// FBX 7500 (aka FBX2016) introduces incompatible changes at binary level:
	// - The NULL block marking end of nested stuff switches from 13 bytes long to 25 bytes long.
	// - The FBX element metadata (end_offset, prop_count and prop_length) switch from uint32 to uint64.
	pVersion = reader.ReadUInt();
	
	while(true){
		int checkEndOffset;
		
		if(pVersion < 7500){
			checkEndOffset = reader.ReadUInt();
			
		}else{
			checkEndOffset = (int)reader.ReadULong();
		}
		
		if(checkEndOffset == 0){
			reader.MovePosition(9);
			break;
		}
		
		pNode->AddNode(fbxNode::Ref::NewWith(*this, reader, checkEndOffset));
	}
	
	// footer
}

fbxScene::~fbxScene(){
	if(pConnectionMap){
		delete pConnectionMap;
	}
	if(pObjectMap){
		delete pObjectMap;
	}
}



// Loading and Saving
///////////////////////

void fbxScene::SetVersion(int version){
	pVersion = version;
}



decMatrix fbxScene::TransformMatrix(const decMatrix &matrix) const{
	/*
	// epsylonAxisX = fbxAxisX
	// epsylonAxisY = fbxAxisY
	// epsylonAxisZ = -fbxAxisZ
	
	bax = (matrix[0][0], matrix[1][0], matrix[2][0])
	bay = (matrix[0][1], matrix[1][1], matrix[2][1])
	baz = (-matrix[0][2], -matrix[1][2], -matrix[2][2])
	bpos = (matrix[0][3], matrix[1][3], matrix[2][3])
	eax = (bax[0], bax[1], -bax[2])
	eay = (bay[0], bay[1], -bay[2])
	eaz = (baz[0], baz[1], -baz[2])
	epos = (bpos[0], bpos[1], -bpos[2])
	row1 = (eax[0], eay[0], eaz[0], epos[0])
	row2 = (eax[1], eay[1], eaz[1], epos[1])
	row3 = (eax[2], eay[2], eaz[2], epos[2])
	row4 = (0.0,    0.0,    0.0,     1.0)
	*/
	
	const float scale = pUnitScaleFactor * FBX_UNIT_SCALE;
	decMatrix m;
	
	// row1 = (  matrix[0][0],  matrix[0][1], -matrix[0][2],  matrix[0][3] )
	m.a11 = matrix.a11 * scale;
	m.a12 = matrix.a12 * scale;
	m.a13 = -matrix.a13 * scale;
	m.a14 = matrix.a14 * scale;
	
	// row2 = (  matrix[1][0],  matrix[1][1], -matrix[1][2],  matrix[1][3] )
	m.a21 = matrix.a21 * scale;
	m.a22 = matrix.a22 * scale;
	m.a23 = -matrix.a23 * scale;
	m.a24 = matrix.a24 * scale;
	
	// row3 = ( -matrix[2][0], -matrix[2][1],  matrix[2][2], -matrix[2][3] )
	m.a31 = -matrix.a31 * scale;
	m.a32 = -matrix.a32 * scale;
	m.a33 = matrix.a33 * scale;
	m.a34 = -matrix.a34 * scale;
	
	return m;
}



fbxNode *fbxScene::FirstNodeNamed(const char *name) const{
	if(!pNodeObjects){
		DETHROW_INFO(deeInvalidParam, "missing node 'Objects'");
	}
	return pNodeObjects->FirstNodeNamed(name);
}

fbxNode *fbxScene::FirstNodeNamedOrNull(const char *name) const{
	return pNodeObjects ? pNodeObjects->FirstNodeNamedOrNull(name) : NULL;
}

void fbxScene::FindNodesNamed(decPointerList &list, const char *name) const{
	if(pNodeObjects){
		pNodeObjects->FindNodesNamed(list, name);
	}
}



void fbxScene::FindConnections(int64_t id, decPointerList &list) const{
	if(pConnectionMap){
		pConnectionMap->Get(id, list);
	}
}

fbxNode *fbxScene::NodeWithID(int64_t id) const{
	fbxNode * const node = NodeWithIDOrNull(id);
	if(node){
		return node;
	}
	
#if defined __MINGW32__ || defined __MINGW64__
	#ifdef PRId64
		#undef PRId64
	#endif
	#define PRId64 "I64u"
#endif
	decString message;
	message.Format("object not found with id %" PRId64, id);
	DETHROW_INFO(deeInvalidParam, message);
}

fbxNode *fbxScene::NodeWithIDOrNull(int64_t id) const{
	return pObjectMap ? pObjectMap->GetAt(id) : NULL;
}



decVector2 fbxScene::ConvUVFbxToDe(const decVector2 &uv){
	return decVector2(uv.x, 1.0f - uv.y);
}

fbxScene::eMappingInformationType fbxScene::ConvMappingInformationType(const fbxNode &node){
	const decString &string = node.FirstNodeNamed("MappingInformationType")->
		GetPropertyAt(0)->CastString().GetValue();
	
	if(string == "AllSame"){
		return emitAllSame;
		
	}else if(string == "ByPolygon"){
		return emitByPolygon;
		
	}else if(string == "ByVertice"){
		return emitByVertex;
		
	}else if(string == "ByPolygonVertex"){
		return emitByPolygonVertex;
		
	}else{
		DETHROW_INFO(deeInvalidParam, decString("unsupported MappingInformationType: ") + string);
	}
}

fbxScene::eReferenceInformationType fbxScene::ConvReferenceInformationType(const fbxNode &node){
	const decString &string = node.FirstNodeNamed("ReferenceInformationType")->
		GetPropertyAt(0)->CastString().GetValue();
	
	if(string == "Direct"){
		return eritDirect;
		
	}else if(string == "IndexToDirect"){
		return eritIndexToDirect;
		
	}else{
		DETHROW_INFO(deeInvalidParam, decString("unsupported ReferenceInformationType: ") + string);
	}
}

fbxScene::eRotationOrder fbxScene::ConvRotationOrder(int value){
	switch(value){
	case 0:
		return eroXYZ;
		
	case 1:
		return eroXZY;
		
	case 2:
		return eroYZX;
		
	case 3:
		return eroYXZ;
		
	case 4:
		return eroZXY;
		
	case 5:
		return eroZYX;
		
	case 6:
		return eroSphericXYZ;
		
	default:
		decString message("unsupported rotation order: ");
		message.AppendValue(value);
		DETHROW_INFO(deeInvalidParam, message);
	}
}

fbxScene::eWeightMode fbxScene::ConvWeightMode(const fbxNode &node){
	const decString &string = node.FirstNodeNamed("Mode")->GetPropertyAt(0)->CastString().GetValue();
	
	if(string == "Total1"){
		return ewmTotal1;
		
	}else{
		DETHROW_INFO(deeInvalidParam, decString("unsupported Mode: ") + string);
	}
}

decMatrix fbxScene::CreateRotationMatrix(const decVector &rotation, eRotationOrder rotationOrder){
	const decVector rot(-rotation.x, -rotation.y, -rotation.z);
	
	switch(rotationOrder){
	case eroXYZ:
		return decMatrix::CreateRotationX(rot.x)
			* decMatrix::CreateRotationY( rot.y )
			* decMatrix::CreateRotationZ( rot.z );
		
	case eroXZY:
		return decMatrix::CreateRotationX(rot.x)
			* decMatrix::CreateRotationY( rot.z )
			* decMatrix::CreateRotationZ( rot.y );
		
	case eroYZX:
		return decMatrix::CreateRotationX(rot.y)
			* decMatrix::CreateRotationY( rot.z )
			* decMatrix::CreateRotationZ( rot.x );
		
	case eroYXZ:
		return decMatrix::CreateRotationX(rot.y)
			* decMatrix::CreateRotationY( rot.x )
			* decMatrix::CreateRotationZ( rot.z );
		
	case eroZXY:
		return decMatrix::CreateRotationX(rot.z)
			* decMatrix::CreateRotationY( rot.x )
			* decMatrix::CreateRotationZ( rot.y );
		
	case eroZYX:
		return decMatrix::CreateRotationX(rot.z)
			* decMatrix::CreateRotationY( rot.y )
			* decMatrix::CreateRotationZ( rot.x );
		
	case eroSphericXYZ:
		return decMatrix::CreateRotationX(rot.x)
			* decMatrix::CreateRotationY( rot.y )
			* decMatrix::CreateRotationZ( rot.z );
		
	default:
		DETHROW(deeInvalidParam);
	}
}

decVector fbxScene::ConvRotation(const decVector &rotation){
	// according to FBX SDK doc: right handed system
	return decVector(-rotation.x, -rotation.y, -rotation.z);
}



void fbxScene::Prepare(deBaseModule &module){
	pNode->Prepare(module);
	
	const fbxNode * const settings = pNode->FirstNodeNamedOrNull("GlobalSettings");
	if(settings){
		const int upAxis = settings->GetPropertyInt("UpAxis", 2);
		const int frontAxis = settings->GetPropertyInt("FrontAxis", 1);
		const int coordAxis = settings->GetPropertyInt("CoordAxis", 0);
		const int upAxisSign = settings->GetPropertyInt("UpAxisSign", 1);
		const int frontAxisSign = settings->GetPropertyInt("FrontAxisSign", 1);
		const int coordAxisSign = settings->GetPropertyInt("CoordAxisSign", 1);
		
		pUpAxis = pGetAxis(upAxis, upAxisSign);
		pFrontAxis = pGetAxis(frontAxis, frontAxisSign);
		pCoordAxis = pGetAxis(coordAxis, coordAxisSign);
		pUnitScaleFactor = settings->GetPropertyFloat("UnitScaleFactor", 1.0f);
		
		switch(upAxis){
		case 0:
			pTransformation.a12 = (float)upAxisSign; pTransformation.a22 = 0.0f; pTransformation.a32 = 0.0f;
			break;
			
		case 1:
			pTransformation.a12 = 0.0f; pTransformation.a22 = (float)upAxisSign; pTransformation.a32 = 0.0f;
			break;
			
		case 2:
			pTransformation.a12 = 0.0f; pTransformation.a22 = 0.0f; pTransformation.a32 = (float)upAxisSign;
			break;
		}
		
		switch(frontAxis){
		case 0:
			pTransformation.a13 = (float)frontAxisSign; pTransformation.a23 = 0.0f; pTransformation.a33 = 0.0f;
			break;
			
		case 1:
			pTransformation.a13 = 0.0f; pTransformation.a23 = (float)frontAxisSign; pTransformation.a33 = 0.0f;
			break;
			
		case 2:
			pTransformation.a13 = 0.0f; pTransformation.a23 = 0.0f; pTransformation.a33 = (float)frontAxisSign;
			break;
		}
		
		switch(coordAxis){
		case 0:
			pTransformation.a11 = (float)coordAxisSign; pTransformation.a21 = 0.0f; pTransformation.a31 = 0.0f;
			break;
			
		case 1:
			pTransformation.a11 = 0.0f; pTransformation.a21 = (float)coordAxisSign; pTransformation.a31 = 0.0f;
			break;
			
		case 2:
			pTransformation.a11 = 0.0f; pTransformation.a21 = 0.0f; pTransformation.a31 = (float)coordAxisSign;
			break;
		}
	}
	
	const float scale = pUnitScaleFactor * FBX_UNIT_SCALE;
	pTransformation = pTransformation * decMatrix::CreateScale(scale, scale, scale);
	
	pAxisTransformation.a11 = -1.0f; pAxisTransformation.a21 = 0.0f; pAxisTransformation.a31 = 0.0f;
	pAxisTransformation.a12 =  0.0f; pAxisTransformation.a22 = 1.0f; pAxisTransformation.a32 = 0.0f;
	pAxisTransformation.a13 =  0.0f; pAxisTransformation.a23 = 0.0f; pAxisTransformation.a33 = 1.0f;
	pTransformation = pTransformation.QuickMultiply(pAxisTransformation);
	
	pNodeObjects = pNode->FirstNodeNamed("Objects");
	if(pNodeObjects){
		const int count = pNodeObjects->GetNodeCount();
		if(count > 0){
			pObjectMap = new fbxObjectMap(count);
			
			int i;
			for(i=0; i<count; i++){
				pObjectMap->Add(pNodeObjects->GetNodeAt(i));
			}
		}
	}
	
	pNodeConnections = pNode->FirstNodeNamed("Connections");
	const int count = pNodeConnections->GetNodeCount();
	fbxConnection::Ref connection;
	int i;
	
	pConnectionMap = new fbxConnectionMap(count);
	
	for(i=0; i<count; i++){
		fbxNode &nodeConnection = *pNodeConnections->GetNodeAt(i);
		if(nodeConnection.GetName() != "C"){
			DETHROW_INFO(deeInvalidFileFormat, "node inside 'Connections' is not named 'C'");
		}
		
		const decString &type = nodeConnection.GetPropertyAt(0)->CastString().GetValue();
		if(type == "OO"){
			connection.TakeOverWith(
				nodeConnection.GetPropertyAt(1)->GetValueAsLong(),
				nodeConnection.GetPropertyAt(2)->GetValueAsLong());
			pConnections.Add(connection);
			pConnectionMap->Add(connection);
			
		}else if(type == "OP"){
			connection.TakeOverWith(
				nodeConnection.GetPropertyAt(1)->GetValueAsLong(),
				nodeConnection.GetPropertyAt(2)->GetValueAsLong(),
				nodeConnection.GetPropertyAt(3)->CastString().GetValue());
			pConnections.Add(connection);
			pConnectionMap->Add(connection);
		}
	}
}

void fbxScene::Save(decBaseFileWriter &writer){
}

void fbxScene::DebugPrintStructure(deBaseModule &module, bool verbose) const{
	module.LogInfoFormat("FBX Scene (version %d.%d):", pVersion / 1000, pVersion % 1000);
	module.LogInfoFormat("Axes: up=%d front=%d coord=%d", pUpAxis, pFrontAxis, pCoordAxis);
	module.LogInfoFormat("UnitScaleFactor: %g", pUnitScaleFactor);
	module.LogInfoFormat("Transformation: [%g,%g,%g,%g] [%g,%g,%g,%g] [%g,%g,%g,%g]",
		pTransformation.a11, pTransformation.a12, pTransformation.a13, pTransformation.a14,
		pTransformation.a21, pTransformation.a22, pTransformation.a23, pTransformation.a24,
		pTransformation.a31, pTransformation.a32, pTransformation.a33, pTransformation.a34);
	pNode->DebugPrintStructure(module, "", verbose);
}

fbxScene::eAxis fbxScene::pGetAxis(int axisType, int axisSign) const{
	switch(axisType){
	case 0:
		return axisSign > 0 ? eaXPos : eaXNeg;
		
	case 1:
		return axisSign > 0 ? eaYPos : eaYNeg;
		
	case 2:
		return axisSign > 0 ? eaZPos : eaZNeg;
		
	default:
		DETHROW_INFO(deeInvalidParam, "invalid axis type");
	}
}
