// includes
#include <stdio.h>
#include <stdlib.h>
#include "detConvexVolume.h"
#include "dragengine/common/math/decConvexVolume.h"
#include "dragengine/common/math/decConvexVolumeFace.h"
#include "dragengine/common/math/decConvexVolumeList.h"
#include "dragengine/common/exceptions.h"

	

// Class detConvexVolume
//////////////////////////

// Constructors, destructor
/////////////////////////////

detConvexVolume::detConvexVolume(){
}

detConvexVolume::~detConvexVolume(){
}



// Testing
////////////

void detConvexVolume::Prepare(){
}

void detConvexVolume::Run(){
	TestVolumeAddVertices();
	TestFaceAddVertices();
	TestFaceSortVertices();
	TestVolumeSetToCube();
	TestSplitVolumeByFace();
	TestSplitVolumeByPlane();
	TestCropVolumeByPlane();
	TestCropVolumeByVolume();
	TestCropVolumeByVolume2();
	TestCropVolumeByVolume3();
	TestSpecial1();
}

void detConvexVolume::CleanUp(){
}

const char *detConvexVolume::GetTestName(){
	return "ConvexVolume";
}



// Private Functions
//////////////////////

void detConvexVolume::TestVolumeAddVertices(){
	decVector v1(1.0f, 5.0f, 0.0f);
	decVector v2(8.0f, 2.0f, 0.0f);
	
	SetSubTestNum(0);
	
	// create a volume
	decConvexVolume volume;
	ASSERT_TRUE(volume.GetVertexCount() == 0);
	
	// add vertices
	volume.AddVertex(v1);
	ASSERT_TRUE(volume.GetVertexCount() == 1);
	ASSERT_TRUE(volume.GetVertexAt(0).IsEqualTo(v1));
	
	volume.AddVertex(v2);
	ASSERT_TRUE(volume.GetVertexCount() == 2);
	ASSERT_TRUE(volume.GetVertexAt(0).IsEqualTo(v1));
	ASSERT_TRUE(volume.GetVertexAt(1).IsEqualTo(v2));
	
	// remove vertices
	volume.RemoveAllVertices();
	ASSERT_TRUE(volume.GetVertexCount() == 0);
}

void detConvexVolume::TestFaceAddVertices(){
	decVector normal(0.0f, 0.0f, -1.0f);
	int v1 = 0;
	int v2 = 1;
	
	SetSubTestNum(1);
	
	// create a face
	decConvexVolumeFace face;
	ASSERT_TRUE(face.GetNormal().IsEqualTo(decVector()));
	face.SetNormal(normal);
	ASSERT_TRUE(face.GetNormal().IsEqualTo(normal));
	ASSERT_TRUE(face.GetVertexCount() == 0);
	
	// add vertices
	face.AddVertex(v1);
	ASSERT_TRUE(face.GetNormal().IsEqualTo(normal));
	ASSERT_TRUE(face.GetVertexCount() == 1);
	ASSERT_TRUE(face.GetVertexAt(0) == v1);
	
	face.AddVertex(v2);
	ASSERT_TRUE(face.GetNormal().IsEqualTo(normal));
	ASSERT_TRUE(face.GetVertexCount() == 2);
	ASSERT_TRUE(face.GetVertexAt(0) == v1);
	ASSERT_TRUE(face.GetVertexAt(1) == v2);
	
	// remove vertices
	face.RemoveAllVertices();
	ASSERT_TRUE(face.GetNormal().IsEqualTo(normal));
	ASSERT_TRUE(face.GetVertexCount() == 0);
}

void detConvexVolume::TestFaceSortVertices(){
	decVector normal(0.0f, 0.0f, -1.0f);
	
	SetSubTestNum(2);
	
	// create a volume
	decConvexVolume volume;
	volume.AddVertex(decVector(1.0f, 5.0f, 0.0f));
	volume.AddVertex(decVector(3.0f, 8.0f, 0.0f));
	volume.AddVertex(decVector(6.0f, 9.0f, 0.0f));
	volume.AddVertex(decVector(8.0f, 6.0f, 0.0f));
	volume.AddVertex(decVector(8.0f, 2.0f, 0.0f));
	volume.AddVertex(decVector(7.0f, 1.0f, 0.0f));
	volume.AddVertex(decVector(2.0f, 1.0f, 0.0f));
	
	// create a face
	decConvexVolumeFace face;
	face.SetNormal(normal);
	
	// test sorting
	face.AddVertex(0);
	face.AddVertex(6);
	face.AddVertex(2);
	face.AddVertex(3);
	face.AddVertex(1);
	face.AddVertex(5);
	face.AddVertex(4);
	face.SortVertices(volume);
	ASSERT_TRUE(face.GetNormal().IsEqualTo(normal));
	ASSERT_TRUE(face.GetVertexCount() == 7);
	ASSERT_TRUE(face.GetVertexAt(0) == 0);
	ASSERT_TRUE(face.GetVertexAt(1) == 1);
	ASSERT_TRUE(face.GetVertexAt(2) == 2);
	ASSERT_TRUE(face.GetVertexAt(3) == 3);
	ASSERT_TRUE(face.GetVertexAt(4) == 4);
	ASSERT_TRUE(face.GetVertexAt(5) == 5);
	ASSERT_TRUE(face.GetVertexAt(6) == 6);
	
	// sort vertices in reverse order
	face.RemoveAllVertices();
	face.AddVertex(0);
	face.AddVertex(6);
	face.AddVertex(5);
	face.AddVertex(4);
	face.AddVertex(3);
	face.AddVertex(2);
	face.AddVertex(1);
	face.SortVertices(volume);
	ASSERT_TRUE(face.GetNormal().IsEqualTo(normal));
	ASSERT_TRUE(face.GetVertexCount() == 7);
	ASSERT_TRUE(face.GetVertexAt(0) == 0);
	ASSERT_TRUE(face.GetVertexAt(1) == 1);
	ASSERT_TRUE(face.GetVertexAt(2) == 2);
	ASSERT_TRUE(face.GetVertexAt(3) == 3);
	ASSERT_TRUE(face.GetVertexAt(4) == 4);
	ASSERT_TRUE(face.GetVertexAt(5) == 5);
	ASSERT_TRUE(face.GetVertexAt(6) == 6);
	
	// sort vertices in alternating order
	face.RemoveAllVertices();
	face.AddVertex(0);
	face.AddVertex(2);
	face.AddVertex(1);
	face.AddVertex(4);
	face.AddVertex(3);
	face.AddVertex(6);
	face.AddVertex(5);
	face.SortVertices(volume);
	ASSERT_TRUE(face.GetNormal().IsEqualTo(normal));
	ASSERT_TRUE(face.GetVertexCount() == 7);
	ASSERT_TRUE(face.GetVertexAt(0) == 0);
	ASSERT_TRUE(face.GetVertexAt(1) == 1);
	ASSERT_TRUE(face.GetVertexAt(2) == 2);
	ASSERT_TRUE(face.GetVertexAt(3) == 3);
	ASSERT_TRUE(face.GetVertexAt(4) == 4);
	ASSERT_TRUE(face.GetVertexAt(5) == 5);
	ASSERT_TRUE(face.GetVertexAt(6) == 6);
	
	// sort vertices in alternating reverse order
	face.RemoveAllVertices();
	face.AddVertex(0);
	face.AddVertex(5);
	face.AddVertex(6);
	face.AddVertex(3);
	face.AddVertex(4);
	face.AddVertex(1);
	face.AddVertex(2);
	face.SortVertices(volume);
	ASSERT_TRUE(face.GetNormal().IsEqualTo(normal));
	ASSERT_TRUE(face.GetVertexAt(0) == 0);
	ASSERT_TRUE(face.GetVertexAt(1) == 1);
	ASSERT_TRUE(face.GetVertexAt(2) == 2);
	ASSERT_TRUE(face.GetVertexAt(3) == 3);
	ASSERT_TRUE(face.GetVertexAt(4) == 4);
	ASSERT_TRUE(face.GetVertexAt(5) == 5);
	ASSERT_TRUE(face.GetVertexAt(6) == 6);
}

void detConvexVolume::TestVolumeSetToCube(){
	decVector halfSize(0.5f, 1.0f, 2.0f);
	decConvexVolumeFace *convexFace;
	int vertex[8];
	
	SetSubTestNum(3);
	
	// create a volume and set it to cube
	decConvexVolume volume;
	volume.SetToCube(halfSize);
	
	ASSERT_TRUE(volume.GetVertexCount() == 8);
	
	vertex[0] = volume.IndexOfVertex(decVector(-halfSize.x, halfSize.y, -halfSize.z));
	ASSERT_FALSE(vertex[0] == -1);
	vertex[1] = volume.IndexOfVertex(decVector(halfSize.x, halfSize.y, -halfSize.z));
	ASSERT_FALSE(vertex[1] == -1);
	vertex[2] = volume.IndexOfVertex(decVector(halfSize.x, -halfSize.y, -halfSize.z));
	ASSERT_FALSE(vertex[2] == -1);
	vertex[3] = volume.IndexOfVertex(decVector(-halfSize.x, -halfSize.y, -halfSize.z));
	ASSERT_FALSE(vertex[3] == -1);
	vertex[4] = volume.IndexOfVertex(decVector(-halfSize.x, halfSize.y, halfSize.z));
	ASSERT_FALSE(vertex[4] == -1);
	vertex[5] = volume.IndexOfVertex(decVector(halfSize.x, halfSize.y, halfSize.z));
	ASSERT_FALSE(vertex[5] == -1);
	vertex[6] = volume.IndexOfVertex(decVector(halfSize.x, -halfSize.y, halfSize.z));
	ASSERT_FALSE(vertex[6] == -1);
	vertex[7] = volume.IndexOfVertex(decVector(-halfSize.x, -halfSize.y, halfSize.z));
	ASSERT_FALSE(vertex[7] == -1);
	
	convexFace = pGetFaceWithNormal(volume, decVector(0.0f, 0.0f, -1.0f));
	ASSERT_TRUE(convexFace != NULL);
	ASSERT_TRUE(convexFace->GetVertexCount() == 4);
	ASSERT_TRUE(convexFace->HasVertex(vertex[0]));
	ASSERT_TRUE(convexFace->HasVertex(vertex[1]));
	ASSERT_TRUE(convexFace->HasVertex(vertex[2]));
	ASSERT_TRUE(convexFace->HasVertex(vertex[3]));
	
	convexFace = pGetFaceWithNormal(volume, decVector(0.0f, 0.0f, 1.0f));
	ASSERT_TRUE(convexFace != NULL);
	ASSERT_TRUE(convexFace->GetVertexCount() == 4);
	ASSERT_TRUE(convexFace->HasVertex(vertex[4]));
	ASSERT_TRUE(convexFace->HasVertex(vertex[5]));
	ASSERT_TRUE(convexFace->HasVertex(vertex[6]));
	ASSERT_TRUE(convexFace->HasVertex(vertex[7]));
	
	convexFace = pGetFaceWithNormal(volume, decVector(1.0f, 0.0f, 0.0f));
	ASSERT_TRUE(convexFace != NULL);
	ASSERT_TRUE(convexFace->GetVertexCount() == 4);
	ASSERT_TRUE(convexFace->HasVertex(vertex[1]));
	ASSERT_TRUE(convexFace->HasVertex(vertex[2]));
	ASSERT_TRUE(convexFace->HasVertex(vertex[5]));
	ASSERT_TRUE(convexFace->HasVertex(vertex[6]));
	
	convexFace = pGetFaceWithNormal(volume, decVector(-1.0f, 0.0f, 0.0f));
	ASSERT_TRUE(convexFace != NULL);
	ASSERT_TRUE(convexFace->GetVertexCount() == 4);
	ASSERT_TRUE(convexFace->HasVertex(vertex[0]));
	ASSERT_TRUE(convexFace->HasVertex(vertex[3]));
	ASSERT_TRUE(convexFace->HasVertex(vertex[4]));
	ASSERT_TRUE(convexFace->HasVertex(vertex[7]));
	
	convexFace = pGetFaceWithNormal(volume, decVector(0.0f, 1.0f, 0.0f));
	ASSERT_TRUE(convexFace != NULL);
	ASSERT_TRUE(convexFace->GetVertexCount() == 4);
	ASSERT_TRUE(convexFace->HasVertex(vertex[0]));
	ASSERT_TRUE(convexFace->HasVertex(vertex[1]));
	ASSERT_TRUE(convexFace->HasVertex(vertex[4]));
	ASSERT_TRUE(convexFace->HasVertex(vertex[5]));
	
	convexFace = pGetFaceWithNormal(volume, decVector(0.0f, -1.0f, 0.0f));
	ASSERT_TRUE(convexFace != NULL);
	ASSERT_TRUE(convexFace->GetVertexCount() == 4);
	ASSERT_TRUE(convexFace->HasVertex(vertex[3]));
	ASSERT_TRUE(convexFace->HasVertex(vertex[2]));
	ASSERT_TRUE(convexFace->HasVertex(vertex[7]));
	ASSERT_TRUE(convexFace->HasVertex(vertex[6]));
}

void detConvexVolume::TestSplitVolumeByFace(){
	decConvexVolume splitVolume;
	decVector splitNormal;
	int result;
	
	SetSubTestNum(4);
	
	// create a volume list with a cube volume
	decConvexVolumeList list;
	list.SetToCube(decVector(1.0f, 1.0f, 1.0f));
	
	result = pIsVolumeCube(list);
	ASSERT_TRUE(result == 0);
	
	
	
	// create split plane that cuts right through
	splitNormal.Set(4.0f, 0.0f, 1.0f);
	splitNormal.Normalize();
	pSetSplitVolume(splitVolume, decVector(0.0f, 0.5f, -1.0f), decVector(-0.5f, 0.5f, 1.0f),
		decVector(-0.5f, -0.5f, 1.0f), decVector(0.0f, -0.5f, -1.0f), splitNormal);
	
	list.SplitByFace(splitVolume, 0);
	
	result = pIsVolumeCubeSplit(list, splitNormal);
	ASSERT_TRUE(result == 0);
	
	// create split plane that cuts right through ( inversed normal )
	pInvertSplitFace(splitVolume);
	
	list.SetToCube(decVector(1.0f, 1.0f, 1.0f));
	list.SplitByFace(splitVolume, 0);
	
	result = pIsVolumeCubeSplit(list, splitNormal);
	ASSERT_TRUE(result == 0);
	
	
	
	// create split face which does not cut due to split plane normal separating
	splitNormal.Set(1.0f, 0.0f, 2.0f);
	splitNormal.Normalize();
	pSetSplitVolume(splitVolume, decVector(0.5f, 0.5f, -2.0f), decVector(-1.5f, 0.5f, -1.0f),
		decVector(-1.5f, -0.5f, -1.0f), decVector(0.5f, -0.5f, -2.0f), splitNormal);
	
	list.SetToCube(decVector(1.0f, 1.0f, 1.0f));
	list.SplitByFace(splitVolume, 0);
	
	result = pIsVolumeCube(list);
	ASSERT_TRUE(result == 0);
	
	// create split face which does not cut due to split plane normal separating ( inversed normal )
	pInvertSplitFace(splitVolume);
	
	list.SetToCube(decVector(1.0f, 1.0f, 1.0f));
	list.SplitByFace(splitVolume, 0);
	
	result = pIsVolumeCube(list);
	ASSERT_TRUE(result == 0);
	
	
	
	// create split face which does not cut due to split plane normal separating
	// and is in the same plane as a cube face
	splitNormal.Set(0.0f, 0.0f, 1.0f);
	pSetSplitVolume(splitVolume, decVector(1.0f, 0.5f, -1.0f), decVector(-1.0f, 0.5f, -1.0f),
		decVector(-1.0f, -0.5f, -1.0f), decVector(1.0f, -0.5f, -1.0f), splitNormal);
	
	list.SetToCube(decVector(1.0f, 1.0f, 1.0f));
	list.SplitByFace(splitVolume, 0);
	
	result = pIsVolumeCube(list);
	ASSERT_TRUE(result == 0);
	
	// create split face which does not cut due to split plane normal separating
	// and is in the same plane as a cube face ( inversed normal )
	pInvertSplitFace(splitVolume);
	
	list.SetToCube(decVector(1.0f, 1.0f, 1.0f));
	list.SplitByFace(splitVolume, 0);
	
	result = pIsVolumeCube(list);
	ASSERT_TRUE(result == 0);
	
	
	
	// create split face which does not cut due to a volume plane normal separating
	splitNormal.Set(2.0f, 0.0f, 1.0f);
	splitNormal.Normalize();
	pSetSplitVolume(splitVolume, decVector(0.0f, 0.5f, -2.5f), decVector(-0.5f, 0.5f, -1.5f),
		decVector(-0.5f, -0.5f, -1.5f), decVector(0.0f, -0.5f, -2.5f), splitNormal);
	
	list.SetToCube(decVector(1.0f, 1.0f, 1.0f));
	list.SplitByFace(splitVolume, 0);
	
	result = pIsVolumeCube(list);
	ASSERT_TRUE(result == 0);
	
	// create split face which does not cut due to a volume plane normal separating
	pInvertSplitFace(splitVolume);
	
	list.SetToCube(decVector(1.0f, 1.0f, 1.0f));
	list.SplitByFace(splitVolume, 0);
	
	result = pIsVolumeCube(list);
	ASSERT_TRUE(result == 0);
	
	
	/*
	printf("\n\nDEBUG:\n");
	printf("Split Normal: %g,%g,%g\n", splitNormal.x, splitNormal.y, splitNormal.z);
	decConvexVolume *cv;
	decConvexVolumeFace *cvf;
	int a,b,c;
	for(a=0; a<list.GetVolumeCount(); a++){
		cv = list.GetVolumeAt(a);
		printf("  Volume %i\n", a);
		for(b=0; b<cv->GetVertexCount(); b++){
			const decVector &cvv = cv->GetVertexAt(b);
			printf("    Vertex %i: %g,%g,%g\n", b, cvv.x, cvv.y, cvv.z);
		}
		for(b=0; b<cv->GetFaceCount(); b++){
			cvf = cv->GetFaceAt(b);
			printf("    Face %i\n", b);
			const decVector &cvfn = cvf->GetNormal();
			printf("      Normal: %g,%g,%g\n", cvfn.x, cvfn.y, cvfn.z);
			for(c=0; c<cvf->GetVertexCount(); c++){
				printf("      Vertex %i: %i\n", c, cvf->GetVertexAt(c));
			}
		}
	}
	*/
	
}

void detConvexVolume::TestSplitVolumeByPlane(){
	decVector splitNormal, splitPosition;
	decConvexVolumeList list;
	int result;
	
	SetSubTestNum(5);
	
	// create split plane that cuts right through
	splitNormal.Set(4.0f, 0.0f, 1.0f);
	splitNormal.Normalize();
	splitPosition.Set(decVector(0.0f, 0.5f, -1.0f));
	
	list.SetToCube(decVector(1.0f, 1.0f, 1.0f));
	list.SplitByPlane(splitNormal, splitPosition, false, NULL);
	
	result = pIsVolumeCubeSplit(list, splitNormal);
	ASSERT_TRUE(result == 0);
	
	// create split plane that cuts right through ( inversed normal )
	list.SetToCube(decVector(1.0f, 1.0f, 1.0f));
	list.SplitByPlane(-splitNormal, splitPosition, false, NULL);
	
	result = pIsVolumeCubeSplit(list, splitNormal);
	ASSERT_TRUE(result == 0);
	
	
	
	// create split face which does not cut due to split plane normal separating
	splitNormal.Set(1.0f, 0.0f, 2.0f);
	splitNormal.Normalize();
	splitPosition.Set(0.5f, 0.5f, -2.0f);
	
	list.SetToCube(decVector(1.0f, 1.0f, 1.0f));
	list.SplitByPlane(splitNormal, splitPosition, false, NULL);
	
	result = pIsVolumeCube(list);
	ASSERT_TRUE(result == 0);
	
	// create split face which does not cut due to split plane normal separating ( inversed normal )
	list.SetToCube(decVector(1.0f, 1.0f, 1.0f));
	list.SplitByPlane(-splitNormal, splitPosition, false, NULL);
	
	result = pIsVolumeCube(list);
	ASSERT_TRUE(result == 0);
}

void detConvexVolume::TestCropVolumeByPlane(){
	decVector splitNormal, splitPosition;
	decConvexVolumeList list;
	int result;
	
	SetSubTestNum(6);
	
	// create split plane that cuts right through
	splitNormal.Set(4.0f, 0.0f, 1.0f);
	splitNormal.Normalize();
	splitPosition.Set(decVector(0.0f, 0.5f, -1.0f));
	
	list.SetToCube(decVector(1.0f, 1.0f, 1.0f));
	list.SplitByPlane(splitNormal, splitPosition, true, NULL);
	
	result = pIsVolumeCubeCropped(list, splitNormal);
	ASSERT_TRUE(result == 0);
	
	// create split plane that cuts right through ( inversed normal )
	list.SetToCube(decVector(1.0f, 1.0f, 1.0f));
	list.SplitByPlane(-splitNormal, splitPosition, true, NULL);
	
	result = pIsVolumeCubeCroppedInverse(list, splitNormal);
	ASSERT_TRUE(result == 0);
	
	
	
	// create split face which does not cut due to split plane normal separating
	splitNormal.Set(1.0f, 0.0f, 2.0f);
	splitNormal.Normalize();
	splitPosition.Set(0.5f, 0.5f, -2.0f);
	
	list.SetToCube(decVector(1.0f, 1.0f, 1.0f));
	list.SplitByPlane(splitNormal, splitPosition, true, NULL);
	
	result = pIsVolumeCube(list);
	ASSERT_TRUE(result == 0);
	
	// create split face which does not cut due to split plane normal separating.
	// the volume is on the cut side so it is completely cut away
	list.SetToCube(decVector(1.0f, 1.0f, 1.0f));
	list.SplitByPlane(-splitNormal, splitPosition, true, NULL);
	
	ASSERT_TRUE(list.GetVolumeCount() == 0);
}

void detConvexVolume::TestCropVolumeByVolume(){
	decConvexVolume splitVolume;
	decConvexVolumeList list;
	decVector boxFaceFrom[6];
	decVector boxFaceTo[6];
	decVector boxCropFrom;
	decVector boxCropTo;
	decVector boxCubeFrom;
	decVector boxCubeTo;
	decVector testVector;
	int b, x, y, z;
	int counter;
	
	SetSubTestNum(7);
	
	// create volumes
	list.SetToCube(decVector(1.0f, 1.0f, 1.0f));
	splitVolume.SetToCube(decVector(0.25f, 0.25f, 0.25f));
	
	// split by volume
	list.SplitByVolume(splitVolume);
	
	// test if the volumes are kind of cubes
	ASSERT_TRUE(list.GetVolumeCount() == 6);
	ASSERT_TRUE(pIsKindOfCube(*list.GetVolumeAt(0)));
	ASSERT_TRUE(pIsKindOfCube(*list.GetVolumeAt(1)));
	ASSERT_TRUE(pIsKindOfCube(*list.GetVolumeAt(2)));
	ASSERT_TRUE(pIsKindOfCube(*list.GetVolumeAt(3)));
	ASSERT_TRUE(pIsKindOfCube(*list.GetVolumeAt(4)));
	ASSERT_TRUE(pIsKindOfCube(*list.GetVolumeAt(5)));
	
	// set the collision boxes
	for(b=0; b<6; b++){
		pBoxFromCube(*list.GetVolumeAt(b), boxFaceFrom[b], boxFaceTo[b]);
	}
	boxCubeFrom.Set(-1.0f, -1.0f, -1.0f);
	boxCubeTo.Set(1.0f, 1.0f, 1.0f);
	boxCropFrom.Set(-0.25f, -0.25f, -0.25f);
	boxCropTo.Set(0.25f, 0.25f, 0.25f);
	
	// test points across the entire original including a margin
	// to lie in exactly one volume or none at all if inside the
	// crop volume
	float base = -2.505f;
	float scale = 0.025f;
	int end = 200;
	for(x=0; x<end; x++){
		testVector.x = base + scale * (float)x;
		for(y=0; y<end; y++){
			testVector.y = base + scale * (float)y;
			for(z=0; z<end; z++){
				testVector.z = base + scale * (float)z;
				
				counter = 0;
				for(b=0; b<6; b++){
					if(testVector > boxFaceFrom[b] && testVector < boxFaceTo[b]){
						counter++;
					}
				}
				
				if(testVector > boxCropFrom && testVector < boxCropTo){
					ASSERT_TRUE(counter == 0);
					
				}else if(testVector > boxCubeFrom && testVector < boxCubeTo){
					ASSERT_TRUE(counter == 1);
					
				}else{
					ASSERT_TRUE(counter == 0);
				}
			}
		}
	}
}

void detConvexVolume::TestCropVolumeByVolume2(){
	decConvexVolume splitVolume;
	decConvexVolumeList list;
	
	SetSubTestNum(8);
	
	// another test but this time the crop encloses the entire volume
	list.SetToCube(decVector(1.0f, 1.0f, 1.0f));
	splitVolume.SetToCube(decVector(1.25f, 1.25f, 1.25f));
	
	list.SplitByVolume(splitVolume);
	
	ASSERT_TRUE(list.GetVolumeCount() == 0);
}

void detConvexVolume::TestCropVolumeByVolume3(){
	decConvexVolume splitVolume;
	decConvexVolumeList list;
	decVector boxFaceFrom[3];
	decVector boxFaceTo[3];
	decVector boxCropFrom;
	decVector boxCropTo;
	decVector boxCubeFrom;
	decVector boxCubeTo;
	decVector testVector;
	int b, x, y, z;
	int counter;
	
	SetSubTestNum(9);
	
	// create volumes. the split volume now only cuts of a corner
	list.SetToCube(decVector(1.0f, 1.0f, 1.0f));
	splitVolume.SetToCube(decVector(0.25f, 0.25f, 0.25f));
	splitVolume.Move(decVector(1.0f, 1.0f, 1.0f));
	
	// split by volume
	list.SplitByVolume(splitVolume);
	
	// test if the volumes are kind of cubes
	ASSERT_TRUE(list.GetVolumeCount() == 3);
	ASSERT_TRUE(pIsKindOfCube(*list.GetVolumeAt(0)));
	ASSERT_TRUE(pIsKindOfCube(*list.GetVolumeAt(1)));
	ASSERT_TRUE(pIsKindOfCube(*list.GetVolumeAt(2)));
	
	// set the collision boxes
	for(b=0; b<3; b++){
		pBoxFromCube(*list.GetVolumeAt(b), boxFaceFrom[b], boxFaceTo[b]);
	}
	boxCubeFrom.Set(-1.0f, -1.0f, -1.0f);
	boxCubeTo.Set(1.0f, 1.0f, 1.0f);
	boxCropFrom.Set(0.75f, 0.75f, 0.75f);
	boxCropTo.Set(1.25f, 1.25f, 1.25f);
	
	// test points across the entire original including a margin
	// to lie in exactly one volume or none at all if inside the
	// crop volume
	float base = -2.505f;
	float scale = 0.025f;
	int end = 200;
	for(x=0; x<end; x++){
		testVector.x = base + scale * (float)x;
		for(y=0; y<end; y++){
			testVector.y = base + scale * (float)y;
			for(z=0; z<end; z++){
				testVector.z = base + scale * (float)z;
				
				counter = 0;
				for(b=0; b<3; b++){
					if(testVector > boxFaceFrom[b] && testVector < boxFaceTo[b]){
						counter++;
					}
				}
				
				if(testVector > boxCropFrom && testVector < boxCropTo){
					ASSERT_TRUE(counter == 0);
					
				}else if(testVector > boxCubeFrom && testVector < boxCubeTo){
					ASSERT_TRUE(counter == 1);
					
				}else{
					ASSERT_TRUE(counter == 0);
				}
			}
		}
	}
}

void detConvexVolume::TestFaceValidity(){
	/*
	decVector normal(0.0f, 0.0f, -1.0f);
	decVector invalidNormal;
	decVector v1(1.0f, 5.0f, 0.0f);
	decVector v2(8.0f, 2.0f, 0.0f);
	decVector v3(6.0f, 9.0f, 0.0f);
	decVector v4(8.0f, 6.0f, 0.0f);
	decVector v5(2.0f, 1.0f, 0.0f);
	decVector v6(7.0f, 1.0f, 0.0f);
	decVector v7(3.0f, 8.0f, 0.0f);
	*/
//	SetSubTestNum( 4 );
	/*
	// test for invalid normal
	decConvexVolumeFace face1(invalidNormal);
	ASSERT_FALSE(face1.IsValid());
	
	// test with not enough vertices
	decConvexVolumeFace face2(normal);
	ASSERT_FALSE(face2.IsValid());
	
	face2.AddVertex(v1);
	ASSERT_FALSE(face2.IsValid());
	
	face2.AddVertex(v2);
	ASSERT_FALSE(face2.IsValid());
	
	// test with incorrect ordering
	face2.AddVertex(v3);
	ASSERT_FALSE(face2.IsValid());
	
	// test with valid ordering
	face2.AddVertex(v4);
	face2.AddVertex(v5);
	face2.AddVertex(v6);
	face2.AddVertex(v7);
	face2.SortVertices();
	ASSERT_TRUE(face2.IsValid());
	*/
}

void detConvexVolume::TestVolume(){
	//int a = 1;
	
//	SetSubTestNum( 5 );
	
	/*
	// create test elements
	decDefaultConvexVolume octree(decVector(0.0f, 0.0f, 0.0f), decVector(10.0f, 8.0f, 12.0f));
	
	// add element to the this.pConvexVolume
	octree.InsertIntoTree(&a, decVector(1.0f, 0.5f, 2.0f), decVector(0.6f, 0.8f, 1.2f));
	
	// check the state
	ASSERT_TRUE(!octree.GetNodeAt(decConvexVolume::eoXposYposZpos));
	ASSERT_TRUE(!octree.GetNodeAt(decConvexVolume::eoXnegYposZpos));
	ASSERT_TRUE(!octree.GetNodeAt(decConvexVolume::eoXnegYnegZpos));
	ASSERT_TRUE(!octree.GetNodeAt(decConvexVolume::eoXposYnegZpos));
	ASSERT_TRUE(!octree.GetNodeAt(decConvexVolume::eoXposYposZneg));
	ASSERT_TRUE(!octree.GetNodeAt(decConvexVolume::eoXnegYposZneg));
	ASSERT_TRUE(!octree.GetNodeAt(decConvexVolume::eoXnegYnegZneg));
	ASSERT_TRUE(!octree.GetNodeAt(decConvexVolume::eoXposYnegZneg));
	ASSERT_TRUE(octree.GetElementCount() == 1);
	ASSERT_TRUE(octree.GetElementAt(0) == &a);
	*/
}

void detConvexVolume::TestSpecial1(){
	SetSubTestNum(10);
	
	// special test
	decConvexVolumeList cvl1, cvl2;
	decConvexVolume *cv = NULL;
	decConvexVolumeFace *cvf = NULL;
	int i, j;
	
	try{
		// volume to split
		cv = new decConvexVolume;
		const decVector v[4] = {
			decVector(-4.299999,0.000000,14.300001),
			decVector(-4.299997,0.000000,25.000000),
			decVector(4.300003,0.000000,25.000000),
			decVector(4.300001,0.000000,14.300001)};
		for(i=0; i<4; i++){
			cv->AddVertex(v[i]);
		}
		cvf = new decConvexVolumeFace;
		for(i=0; i<4; i++){
			cvf->AddVertex(i);
		}
		cvf->SetNormal(((v[1]-v[0])%(v[2]-v[1])).Normalized());
		cv->AddFace(cvf);
		cvf = NULL;
		cvl1.AddVolume(cv);
		cv = NULL;
		
		// splitter volume
		cv = new decConvexVolume;
		const decVector v2[8] = {
			decVector(-0.338018,1.300000,22.241497),
			decVector(-1.931929,1.300000,22.102049),
			decVector(-2.071378,1.300000,23.695961),
			decVector(-0.477467,1.300000,23.835409),
			decVector(-0.338018,-0.300000,22.241497),
			decVector(-1.931929,-0.300000,22.102049),
			decVector(-2.071378,-0.300000,23.695961),
			decVector(-0.477467,-0.300000,23.835409)};
		for(i=0; i<8; i++){
			cv->AddVertex(v2[i]);
		}
		const int f2[6][4] = {{0,1,2,3}, {7,6,5,4}, {6,7,3,2}, {4,5,1,0}, {5,6,2,1}, {7,4,0,3}};
		for(i=0; i<6; i++){
			cvf = new decConvexVolumeFace;
			for(j=0; j<4; j++){
				cvf->AddVertex(f2[i][j]);
			}
			cvf->SetNormal(((v2[f2[i][1]]-v2[f2[i][0]])%(v2[f2[i][2]]-v2[f2[i][1]])).Normalized());
			cv->AddFace(cvf);
			cvf = NULL;
		}
		cvl2.AddVolume(cv);
		cv = NULL;
		
	}catch(const deException &){
		if(cvf){
			delete cvf;
		}
		if(cv){
			delete cv;
		}
		throw;
	}
	
	// split and test result
	cvl1.SplitByVolume(*cvl2.GetVolumeAt(0));
	
	ASSERT_EQUAL(cvl1.GetVolumeCount(), 4);
	ASSERT_EQUAL(cvl1.GetVolumeAt(0)->GetVertexCount(), 4);
	ASSERT_EQUAL(cvl1.GetVolumeAt(0)->GetFaceCount(), 1);
	ASSERT_EQUAL(cvl1.GetVolumeAt(0)->GetFaceAt(0)->GetVertexCount(), 4);
	ASSERT_EQUAL(cvl1.GetVolumeAt(1)->GetVertexCount(), 4);
	ASSERT_EQUAL(cvl1.GetVolumeAt(1)->GetFaceCount(), 1);
	ASSERT_EQUAL(cvl1.GetVolumeAt(1)->GetFaceAt(0)->GetVertexCount(), 4);
	ASSERT_EQUAL(cvl1.GetVolumeAt(2)->GetVertexCount(), 4);
	ASSERT_EQUAL(cvl1.GetVolumeAt(2)->GetFaceCount(), 1);
	ASSERT_EQUAL(cvl1.GetVolumeAt(2)->GetFaceAt(0)->GetVertexCount(), 4);
	ASSERT_EQUAL(cvl1.GetVolumeAt(3)->GetVertexCount(), 4);
	ASSERT_EQUAL(cvl1.GetVolumeAt(3)->GetFaceCount(), 1);
	ASSERT_EQUAL(cvl1.GetVolumeAt(3)->GetFaceAt(0)->GetVertexCount(), 4);
	
	int testSetFound[4] = {0, 0, 0, 0};
	
	for(i=0; i<4; i++){
		cv = cvl1.GetVolumeAt(i);
		cvf = cv->GetFaceAt(0);
		
		if(cv->GetVertexAt(cvf->GetVertexAt(0)).IsEqualTo(decVector(-4.299997,0.000000,23.500980), THRESHOLD_EQUAL_FLOAT)
		&& cv->GetVertexAt(cvf->GetVertexAt(1)).IsEqualTo(decVector(-4.299997,0.000000,25.000000), THRESHOLD_EQUAL_FLOAT)
		&& cv->GetVertexAt(cvf->GetVertexAt(2)).IsEqualTo(decVector(4.300003,0.000000,25.000000), THRESHOLD_EQUAL_FLOAT)
		&& cv->GetVertexAt(cvf->GetVertexAt(3)).IsEqualTo(decVector(4.300003,0.000000,24.253380), THRESHOLD_EQUAL_FLOAT)){
			testSetFound[0]++;
			continue;
		}
		
		if(cv->GetVertexAt(cvf->GetVertexAt(0)).IsEqualTo(decVector(-4.299999,0.000000,14.300001), THRESHOLD_EQUAL_FLOAT)
		&& cv->GetVertexAt(cvf->GetVertexAt(1)).IsEqualTo(decVector(-4.299998,0.000000,21.894873), THRESHOLD_EQUAL_FLOAT)
		&& cv->GetVertexAt(cvf->GetVertexAt(2)).IsEqualTo(decVector(4.300003,0.000000,22.647270), THRESHOLD_EQUAL_FLOAT)
		&& cv->GetVertexAt(cvf->GetVertexAt(3)).IsEqualTo(decVector(4.300001,0.000000,14.300001), THRESHOLD_EQUAL_FLOAT)){
			testSetFound[1]++;
			continue;
		}
		
		if(cv->GetVertexAt(cvf->GetVertexAt(0)).IsEqualTo(decVector(-4.299998,0.000000,21.894873), THRESHOLD_EQUAL_FLOAT)
		&& cv->GetVertexAt(cvf->GetVertexAt(1)).IsEqualTo(decVector(-4.299997,0.000000,23.500980), THRESHOLD_EQUAL_FLOAT)
		&& cv->GetVertexAt(cvf->GetVertexAt(2)).IsEqualTo(decVector(-2.071378,0.000000,23.695959), THRESHOLD_EQUAL_FLOAT)
		&& cv->GetVertexAt(cvf->GetVertexAt(3)).IsEqualTo(decVector(-1.931929,0.000000,22.102051), THRESHOLD_EQUAL_FLOAT)){
			testSetFound[2]++;
			continue;
		}
		
		if(cv->GetVertexAt(cvf->GetVertexAt(0)).IsEqualTo(decVector(-0.477467,0.000000,23.835407), THRESHOLD_EQUAL_FLOAT)
		&& cv->GetVertexAt(cvf->GetVertexAt(1)).IsEqualTo(decVector(4.300003,0.000000,24.253380), THRESHOLD_EQUAL_FLOAT)
		&& cv->GetVertexAt(cvf->GetVertexAt(2)).IsEqualTo(decVector(4.300003,0.000000,22.647270), THRESHOLD_EQUAL_FLOAT)
		&& cv->GetVertexAt(cvf->GetVertexAt(3)).IsEqualTo(decVector(-0.338018,0.000000,22.241499), THRESHOLD_EQUAL_FLOAT)){
			testSetFound[3]++;
			continue;
		}
	}
	
	ASSERT_EQUAL(testSetFound[0], 1);
	ASSERT_EQUAL(testSetFound[1], 1);
	ASSERT_EQUAL(testSetFound[2], 1);
	ASSERT_EQUAL(testSetFound[3], 1);
}



decConvexVolumeFace *detConvexVolume::pGetFaceWithNormal(const decConvexVolume &volume, const decVector &normal) const{
	int i, count = volume.GetFaceCount();
	decConvexVolumeFace *face;
	
	for(i=0; i<count; i++){
		face = volume.GetFaceAt(i);
		if(normal.IsEqualTo(face->GetNormal())) return face;
	}
	
	return NULL;
}

decConvexVolume *detConvexVolume::pGetVolumeWithVertex(const decConvexVolumeList &list, const decVector &vertex) const{
	int i, count = list.GetVolumeCount();
	decConvexVolume *volume;
	
	for(i=0; i<count; i++){
		volume = list.GetVolumeAt(i);
		if(volume->HasVertex(vertex)) return volume;
	}
	
	return NULL;
}

int detConvexVolume::pIsVolumeCube(const decConvexVolumeList &list) const{
	decConvexVolumeFace *convexFace;
	decConvexVolume *volume;
	int vertex[8];
	
	if(list.GetVolumeCount() != 1) return 1;
	
	volume = list.GetVolumeAt(0);
	if(volume->GetVertexCount() != 8) return 10;
	
	vertex[0] = volume->IndexOfVertex(decVector(-1.0f, 1.0f, -1.0f));
	if(vertex[0] == -1) return 11;
	vertex[1] = volume->IndexOfVertex(decVector(1.0f, 1.0f, -1.0f));
	if(vertex[1] == -1) return 12;
	vertex[2] = volume->IndexOfVertex(decVector(1.0f, -1.0f, -1.0f));
	if(vertex[2] == -1) return 13;
	vertex[3] = volume->IndexOfVertex(decVector(-1.0f, -1.0f, -1.0f));
	if(vertex[3] == -1) return 14;
	vertex[4] = volume->IndexOfVertex(decVector(-1.0f, 1.0f, 1.0f));
	if(vertex[4] == -1) return 15;
	vertex[5] = volume->IndexOfVertex(decVector(1.0f, 1.0f, 1.0f));
	if(vertex[5] == -1) return 16;
	vertex[6] = volume->IndexOfVertex(decVector(1.0f, -1.0f, 1.0f));
	if(vertex[6] == -1) return 17;
	vertex[7] = volume->IndexOfVertex(decVector(-1.0f, -1.0f, 1.0f));
	if(vertex[7] == -1) return 18;
	
	convexFace = pGetFaceWithNormal(*volume, decVector(0.0f, 0.0f, -1.0f));
	if(!convexFace) return 20;
	if(convexFace->GetVertexCount() != 4) return 21;
	if(!convexFace->HasVertex(vertex[0])) return 22;
	if(!convexFace->HasVertex(vertex[1])) return 23;
	if(!convexFace->HasVertex(vertex[2])) return 24;
	if(!convexFace->HasVertex(vertex[3])) return 25;
	
	convexFace = pGetFaceWithNormal(*volume, decVector(0.0f, 0.0f, 1.0f));
	if(!convexFace) return 30;
	if(convexFace->GetVertexCount() != 4) return 31;
	if(!convexFace->HasVertex(vertex[4])) return 32;
	if(!convexFace->HasVertex(vertex[5])) return 33;
	if(!convexFace->HasVertex(vertex[6])) return 34;
	if(!convexFace->HasVertex(vertex[7])) return 35;
	
	convexFace = pGetFaceWithNormal(*volume, decVector(1.0f, 0.0f, 0.0f));
	if(!convexFace) return 40;
	if(convexFace->GetVertexCount() != 4) return 41;
	if(!convexFace->HasVertex(vertex[1])) return 42;
	if(!convexFace->HasVertex(vertex[2])) return 43;
	if(!convexFace->HasVertex(vertex[5])) return 44;
	if(!convexFace->HasVertex(vertex[6])) return 45;
	
	convexFace = pGetFaceWithNormal(*volume, decVector(-1.0f, 0.0f, 0.0f));
	if(!convexFace) return 50;
	if(convexFace->GetVertexCount() != 4) return 51;
	if(!convexFace->HasVertex(vertex[0])) return 52;
	if(!convexFace->HasVertex(vertex[3])) return 53;
	if(!convexFace->HasVertex(vertex[4])) return 54;
	if(!convexFace->HasVertex(vertex[7])) return 55;
	
	convexFace = pGetFaceWithNormal(*volume, decVector(0.0f, 1.0f, 0.0f));
	if(!convexFace) return 60;
	if(convexFace->GetVertexCount() != 4) return 61;
	if(!convexFace->HasVertex(vertex[0])) return 62;
	if(!convexFace->HasVertex(vertex[1])) return 63;
	if(!convexFace->HasVertex(vertex[4])) return 64;
	if(!convexFace->HasVertex(vertex[5])) return 65;
	
	convexFace = pGetFaceWithNormal(*volume, decVector(0.0f, -1.0f, 0.0f));
	if(!convexFace) return 70;
	if(convexFace->GetVertexCount() != 4) return 71;
	if(!convexFace->HasVertex(vertex[3])) return 72;
	if(!convexFace->HasVertex(vertex[2])) return 73;
	if(!convexFace->HasVertex(vertex[7])) return 74;
	if(!convexFace->HasVertex(vertex[6])) return 75;
	
	return 0;
}

int detConvexVolume::pIsVolumeCubeSplit(const decConvexVolumeList &list, const decVector &splitNormal) const{
	decConvexVolumeFace *testFace;
	decConvexVolume *testVolume;
	int vertex[8];
	
	testVolume = pGetVolumeWithVertex(list, decVector(1.0f, 1.0f, 1.0f));
	if(!testVolume) return 1;
	
	if(testVolume->GetVertexCount() != 8) return 10;
	
	vertex[0] = testVolume->IndexOfVertex(decVector(1.0f, 1.0f, 1.0f));
	if(vertex[0] == -1) return 11;
	vertex[1] = testVolume->IndexOfVertex(decVector(1.0f, -1.0f, 1.0f));
	if(vertex[1] == -1) return 12;
	vertex[2] = testVolume->IndexOfVertex(decVector(1.0f, 1.0f, -1.0f));
	if(vertex[2] == -1) return 13;
	vertex[3] = testVolume->IndexOfVertex(decVector(1.0f, -1.0f, -1.0f));
	if(vertex[3] == -1) return 14;
	vertex[4] = testVolume->IndexOfVertex(decVector(-0.5f, 1.0f, 1.0f));
	if(vertex[4] == -1) return 15;
	vertex[5] = testVolume->IndexOfVertex(decVector(-0.5f, -1.0f, 1.0f));
	if(vertex[5] == -1) return 16;
	vertex[6] = testVolume->IndexOfVertex(decVector(0.0f, 1.0f, -1.0f));
	if(vertex[6] == -1) return 17;
	vertex[7] = testVolume->IndexOfVertex(decVector(0.0f, -1.0f, -1.0f));
	if(vertex[7] == -1) return 18;
	
	testFace = pGetFaceWithNormal(*testVolume, decVector(1.0f, 0.0f, 0.0f));
	if(!testFace) return 20;
	if(testFace->GetVertexCount() != 4) return 21;
	if(!testFace->HasVertex(vertex[2])) return 22;
	if(!testFace->HasVertex(vertex[0])) return 23;
	if(!testFace->HasVertex(vertex[1])) return 24;
	if(!testFace->HasVertex(vertex[3])) return 25;
	
	testFace = pGetFaceWithNormal(*testVolume, decVector(0.0f, 0.0f, 1.0f));
	if(!testFace) return 30;
	if(testFace->GetVertexCount() != 4) return 31;
	if(!testFace->HasVertex(vertex[0])) return 32;
	if(!testFace->HasVertex(vertex[4])) return 33;
	if(!testFace->HasVertex(vertex[5])) return 34;
	if(!testFace->HasVertex(vertex[1])) return 35;
	
	testFace = pGetFaceWithNormal(*testVolume, decVector(0.0f, 0.0f, -1.0f));
	if(!testFace) return 40;
	if(testFace->GetVertexCount() != 4) return 41;
	if(!testFace->HasVertex(vertex[6])) return 42;
	if(!testFace->HasVertex(vertex[2])) return 43;
	if(!testFace->HasVertex(vertex[3])) return 44;
	if(!testFace->HasVertex(vertex[7])) return 45;
	
	testFace = pGetFaceWithNormal(*testVolume, decVector(0.0f, 1.0f, 0.0f));
	if(!testFace) return 50;
	if(testFace->GetVertexCount() != 4) return 51;
	if(!testFace->HasVertex(vertex[4])) return 52;
	if(!testFace->HasVertex(vertex[0])) return 53;
	if(!testFace->HasVertex(vertex[2])) return 54;
	if(!testFace->HasVertex(vertex[6])) return 55;
	
	testFace = pGetFaceWithNormal(*testVolume, decVector(0.0f, -1.0f, 0.0f));
	if(!testFace) return 60;
	if(testFace->GetVertexCount() != 4) return 61;
	if(!testFace->HasVertex(vertex[5])) return 62;
	if(!testFace->HasVertex(vertex[1])) return 63;
	if(!testFace->HasVertex(vertex[3])) return 64;
	if(!testFace->HasVertex(vertex[7])) return 65;
	
	testFace = pGetFaceWithNormal(*testVolume, -splitNormal);
	if(!testFace) return 70;
	if(testFace->GetVertexCount() != 4) return 71;
	if(!testFace->HasVertex(vertex[4])) return 72;
	if(!testFace->HasVertex(vertex[6])) return 73;
	if(!testFace->HasVertex(vertex[7])) return 74;
	if(!testFace->HasVertex(vertex[5])) return 75;
	
	
	
	testVolume = pGetVolumeWithVertex(list, decVector(-1.0f, -1.0f, -1.0f));
	if(!testVolume) return 101;
	
	if(testVolume->GetVertexCount() != 8) return 110;
	
	vertex[0] = testVolume->IndexOfVertex(decVector(-1.0f, 1.0f, 1.0f));
	if(vertex[0] == -1) return 111;
	vertex[1] = testVolume->IndexOfVertex(decVector(-1.0f, -1.0f, 1.0f));
	if(vertex[1] == -1) return 112;
	vertex[2] = testVolume->IndexOfVertex(decVector(-1.0f, 1.0f, -1.0f));
	if(vertex[2] == -1) return 113;
	vertex[3] = testVolume->IndexOfVertex(decVector(-1.0f, -1.0f, -1.0f));
	if(vertex[3] == -1) return 114;
	vertex[4] = testVolume->IndexOfVertex(decVector(-0.5f, 1.0f, 1.0f));
	if(vertex[4] == -1) return 115;
	vertex[5] = testVolume->IndexOfVertex(decVector(-0.5f, -1.0f, 1.0f));
	if(vertex[5] == -1) return 116;
	vertex[6] = testVolume->IndexOfVertex(decVector(0.0f, 1.0f, -1.0f));
	if(vertex[6] == -1) return 117;
	vertex[7] = testVolume->IndexOfVertex(decVector(0.0f, -1.0f, -1.0f));
	if(vertex[7] == -1) return 118;
	
	testFace = pGetFaceWithNormal(*testVolume, decVector(-1.0f, 0.0f, 0.0f));
	if(!testFace) return 120;
	if(testFace->GetVertexCount() != 4) return 121;
	if(!testFace->HasVertex(vertex[2])) return 122;
	if(!testFace->HasVertex(vertex[0])) return 123;
	if(!testFace->HasVertex(vertex[1])) return 124;
	if(!testFace->HasVertex(vertex[3])) return 125;
	
	testFace = pGetFaceWithNormal(*testVolume, decVector(0.0f, 0.0f, 1.0f));
	if(!testFace) return 130;
	if(testFace->GetVertexCount() != 4) return 131;
	if(!testFace->HasVertex(vertex[0])) return 132;
	if(!testFace->HasVertex(vertex[4])) return 133;
	if(!testFace->HasVertex(vertex[5])) return 134;
	if(!testFace->HasVertex(vertex[1])) return 135;
	
	testFace = pGetFaceWithNormal(*testVolume, decVector(0.0f, 0.0f, -1.0f));
	if(!testFace) return 140;
	if(testFace->GetVertexCount() != 4) return 141;
	if(!testFace->HasVertex(vertex[6])) return 142;
	if(!testFace->HasVertex(vertex[2])) return 143;
	if(!testFace->HasVertex(vertex[3])) return 144;
	if(!testFace->HasVertex(vertex[7])) return 145;
	
	testFace = pGetFaceWithNormal(*testVolume, decVector(0.0f, 1.0f, 0.0f));
	if(!testFace) return 150;
	if(testFace->GetVertexCount() != 4) return 151;
	if(!testFace->HasVertex(vertex[4])) return 152;
	if(!testFace->HasVertex(vertex[0])) return 153;
	if(!testFace->HasVertex(vertex[2])) return 154;
	if(!testFace->HasVertex(vertex[6])) return 155;
	
	testFace = pGetFaceWithNormal(*testVolume, decVector(0.0f, -1.0f, 0.0f));
	if(!testFace) return 160;
	if(testFace->GetVertexCount() != 4) return 161;
	if(!testFace->HasVertex(vertex[5])) return 162;
	if(!testFace->HasVertex(vertex[1])) return 163;
	if(!testFace->HasVertex(vertex[3])) return 164;
	if(!testFace->HasVertex(vertex[7])) return 165;
	
	testFace = pGetFaceWithNormal(*testVolume, splitNormal);
	if(!testFace) return 170;
	if(testFace->GetVertexCount() != 4) return 171;
	if(!testFace->HasVertex(vertex[4])) return 172;
	if(!testFace->HasVertex(vertex[6])) return 173;
	if(!testFace->HasVertex(vertex[7])) return 174;
	if(!testFace->HasVertex(vertex[5])) return 175;
	
	return 0;
}

int detConvexVolume::pIsVolumeCubeCropped(const decConvexVolumeList &list, const decVector &splitNormal) const{
	decConvexVolumeFace *testFace;
	decConvexVolume *testVolume;
	int vertex[8];
	
	testVolume = pGetVolumeWithVertex(list, decVector(1.0f, 1.0f, 1.0f));
	if(!testVolume) return 1;
	
	if(testVolume->GetVertexCount() != 8) return 10;
	
	vertex[0] = testVolume->IndexOfVertex(decVector(1.0f, 1.0f, 1.0f));
	if(vertex[0] == -1) return 11;
	vertex[1] = testVolume->IndexOfVertex(decVector(1.0f, -1.0f, 1.0f));
	if(vertex[1] == -1) return 12;
	vertex[2] = testVolume->IndexOfVertex(decVector(1.0f, 1.0f, -1.0f));
	if(vertex[2] == -1) return 13;
	vertex[3] = testVolume->IndexOfVertex(decVector(1.0f, -1.0f, -1.0f));
	if(vertex[3] == -1) return 14;
	vertex[4] = testVolume->IndexOfVertex(decVector(-0.5f, 1.0f, 1.0f));
	if(vertex[4] == -1) return 15;
	vertex[5] = testVolume->IndexOfVertex(decVector(-0.5f, -1.0f, 1.0f));
	if(vertex[5] == -1) return 16;
	vertex[6] = testVolume->IndexOfVertex(decVector(0.0f, 1.0f, -1.0f));
	if(vertex[6] == -1) return 17;
	vertex[7] = testVolume->IndexOfVertex(decVector(0.0f, -1.0f, -1.0f));
	if(vertex[7] == -1) return 18;
	
	testFace = pGetFaceWithNormal(*testVolume, decVector(1.0f, 0.0f, 0.0f));
	if(!testFace) return 20;
	if(testFace->GetVertexCount() != 4) return 21;
	if(!testFace->HasVertex(vertex[2])) return 22;
	if(!testFace->HasVertex(vertex[0])) return 23;
	if(!testFace->HasVertex(vertex[1])) return 24;
	if(!testFace->HasVertex(vertex[3])) return 25;
	
	testFace = pGetFaceWithNormal(*testVolume, decVector(0.0f, 0.0f, 1.0f));
	if(!testFace) return 30;
	if(testFace->GetVertexCount() != 4) return 31;
	if(!testFace->HasVertex(vertex[0])) return 32;
	if(!testFace->HasVertex(vertex[4])) return 33;
	if(!testFace->HasVertex(vertex[5])) return 34;
	if(!testFace->HasVertex(vertex[1])) return 35;
	
	testFace = pGetFaceWithNormal(*testVolume, decVector(0.0f, 0.0f, -1.0f));
	if(!testFace) return 40;
	if(testFace->GetVertexCount() != 4) return 41;
	if(!testFace->HasVertex(vertex[6])) return 42;
	if(!testFace->HasVertex(vertex[2])) return 43;
	if(!testFace->HasVertex(vertex[3])) return 44;
	if(!testFace->HasVertex(vertex[7])) return 45;
	
	testFace = pGetFaceWithNormal(*testVolume, decVector(0.0f, 1.0f, 0.0f));
	if(!testFace) return 50;
	if(testFace->GetVertexCount() != 4) return 51;
	if(!testFace->HasVertex(vertex[4])) return 52;
	if(!testFace->HasVertex(vertex[0])) return 53;
	if(!testFace->HasVertex(vertex[2])) return 54;
	if(!testFace->HasVertex(vertex[6])) return 55;
	
	testFace = pGetFaceWithNormal(*testVolume, decVector(0.0f, -1.0f, 0.0f));
	if(!testFace) return 60;
	if(testFace->GetVertexCount() != 4) return 61;
	if(!testFace->HasVertex(vertex[5])) return 62;
	if(!testFace->HasVertex(vertex[1])) return 63;
	if(!testFace->HasVertex(vertex[3])) return 64;
	if(!testFace->HasVertex(vertex[7])) return 65;
	
	testFace = pGetFaceWithNormal(*testVolume, -splitNormal);
	if(!testFace) return 70;
	if(testFace->GetVertexCount() != 4) return 71;
	if(!testFace->HasVertex(vertex[4])) return 72;
	if(!testFace->HasVertex(vertex[6])) return 73;
	if(!testFace->HasVertex(vertex[7])) return 74;
	if(!testFace->HasVertex(vertex[5])) return 75;
	
	return 0;
}

int detConvexVolume::pIsVolumeCubeCroppedInverse(const decConvexVolumeList &list, const decVector &splitNormal) const{
	decConvexVolumeFace *testFace;
	decConvexVolume *testVolume;
	int vertex[8];
	
	testVolume = pGetVolumeWithVertex(list, decVector(-1.0f, -1.0f, -1.0f));
	if(!testVolume) return 01;
	
	if(testVolume->GetVertexCount() != 8) return 10;
	
	vertex[0] = testVolume->IndexOfVertex(decVector(-1.0f, 1.0f, 1.0f));
	if(vertex[0] == -1) return 11;
	vertex[1] = testVolume->IndexOfVertex(decVector(-1.0f, -1.0f, 1.0f));
	if(vertex[1] == -1) return 12;
	vertex[2] = testVolume->IndexOfVertex(decVector(-1.0f, 1.0f, -1.0f));
	if(vertex[2] == -1) return 13;
	vertex[3] = testVolume->IndexOfVertex(decVector(-1.0f, -1.0f, -1.0f));
	if(vertex[3] == -1) return 14;
	vertex[4] = testVolume->IndexOfVertex(decVector(-0.5f, 1.0f, 1.0f));
	if(vertex[4] == -1) return 15;
	vertex[5] = testVolume->IndexOfVertex(decVector(-0.5f, -1.0f, 1.0f));
	if(vertex[5] == -1) return 16;
	vertex[6] = testVolume->IndexOfVertex(decVector(0.0f, 1.0f, -1.0f));
	if(vertex[6] == -1) return 17;
	vertex[7] = testVolume->IndexOfVertex(decVector(0.0f, -1.0f, -1.0f));
	if(vertex[7] == -1) return 18;
	
	testFace = pGetFaceWithNormal(*testVolume, decVector(-1.0f, 0.0f, 0.0f));
	if(!testFace) return 20;
	if(testFace->GetVertexCount() != 4) return 21;
	if(!testFace->HasVertex(vertex[2])) return 22;
	if(!testFace->HasVertex(vertex[0])) return 23;
	if(!testFace->HasVertex(vertex[1])) return 24;
	if(!testFace->HasVertex(vertex[3])) return 25;
	
	testFace = pGetFaceWithNormal(*testVolume, decVector(0.0f, 0.0f, 1.0f));
	if(!testFace) return 30;
	if(testFace->GetVertexCount() != 4) return 31;
	if(!testFace->HasVertex(vertex[0])) return 32;
	if(!testFace->HasVertex(vertex[4])) return 33;
	if(!testFace->HasVertex(vertex[5])) return 34;
	if(!testFace->HasVertex(vertex[1])) return 35;
	
	testFace = pGetFaceWithNormal(*testVolume, decVector(0.0f, 0.0f, -1.0f));
	if(!testFace) return 40;
	if(testFace->GetVertexCount() != 4) return 41;
	if(!testFace->HasVertex(vertex[6])) return 42;
	if(!testFace->HasVertex(vertex[2])) return 43;
	if(!testFace->HasVertex(vertex[3])) return 44;
	if(!testFace->HasVertex(vertex[7])) return 45;
	
	testFace = pGetFaceWithNormal(*testVolume, decVector(0.0f, 1.0f, 0.0f));
	if(!testFace) return 50;
	if(testFace->GetVertexCount() != 4) return 51;
	if(!testFace->HasVertex(vertex[4])) return 52;
	if(!testFace->HasVertex(vertex[0])) return 53;
	if(!testFace->HasVertex(vertex[2])) return 54;
	if(!testFace->HasVertex(vertex[6])) return 55;
	
	testFace = pGetFaceWithNormal(*testVolume, decVector(0.0f, -1.0f, 0.0f));
	if(!testFace) return 60;
	if(testFace->GetVertexCount() != 4) return 61;
	if(!testFace->HasVertex(vertex[5])) return 62;
	if(!testFace->HasVertex(vertex[1])) return 63;
	if(!testFace->HasVertex(vertex[3])) return 64;
	if(!testFace->HasVertex(vertex[7])) return 65;
	
	testFace = pGetFaceWithNormal(*testVolume, splitNormal);
	if(!testFace) return 70;
	if(testFace->GetVertexCount() != 4) return 71;
	if(!testFace->HasVertex(vertex[4])) return 72;
	if(!testFace->HasVertex(vertex[6])) return 73;
	if(!testFace->HasVertex(vertex[7])) return 74;
	if(!testFace->HasVertex(vertex[5])) return 75;
	
	return 0;
}

int detConvexVolume::pIsKindOfCube(const decConvexVolume &volume) const{
	int f, faceCount = volume.GetFaceCount();
	decConvexVolumeFace *convexFace;
	decVector normals[6] = {
		decVector(1.0f, 0.0f, 0.0f),
		decVector(-1.0f, 0.0f, 0.0f),
		decVector(0.0f, 1.0f, 0.0f),
		decVector(0.0f, -1.0f, 0.0f),
		decVector(0.0f, 0.0f, 1.0f),
		decVector(0.0f, 0.0f, -1.0f)};
	bool foundNormal[6] = {false, false, false, false, false, false};
	int errorBase, n;
	
	if(volume.GetVertexCount() != 8) return 1;
	if(faceCount != 6) return 2;
	
	for(f=0; f<faceCount; f++){
		errorBase = (f + 1) * 10;
		convexFace = volume.GetFaceAt(f);
		const decVector &normal = convexFace->GetNormal();
		
		if(convexFace->GetVertexCount() != 4) return errorBase;
		
		for(n=0; n<6; n++){
			if(normal.IsEqualTo(normals[n])){
				if(foundNormal[n]) return errorBase + 1;
				foundNormal[n] = true;
			}
		}
		
		if(n == 6) return errorBase + 2;
	}
	
	for(n=0; n<6; n++){
		if(!foundNormal[n]) return 3;
	}
	
	return 0;
}

void detConvexVolume::pSetSplitVolume(decConvexVolume &volume, const decVector &v1, const decVector &v2, const decVector &v3, const decVector &v4, const decVector &normal) const{
	// clear the volume
	volume.SetEmpty();
	
	// add the vertices
	volume.AddVertex(v1);
	volume.AddVertex(v2);
	volume.AddVertex(v3);
	volume.AddVertex(v4);
	
	// create test face
	decConvexVolumeFace *face = new decConvexVolumeFace;
	if(!face) DETHROW(deeOutOfMemory);
	
	// set face normal
	face->SetNormal(normal);
	
	// add vertices to face
	face->AddVertex(0);
	face->AddVertex(1);
	face->AddVertex(2);
	face->AddVertex(3);
	
	// add face to volume
	volume.AddFace(face);
}

void detConvexVolume::pInvertSplitFace(decConvexVolume &volume){
	decConvexVolumeFace *face = volume.GetFaceAt(0);
	int v, vertex[4];
	
	for(v=0; v<4; v++){
		vertex[v] = face->GetVertexAt(v);
	}
	
	face->RemoveAllVertices();
	
	for(v=3; v>=0; v--){
		face->AddVertex(vertex[v]);
	}
	
	face->SetNormal(-face->GetNormal());
}

void detConvexVolume::pBoxFromCube(const decConvexVolume &volume, decVector &boxFrom, decVector &boxTo){
	int v, vertexCount = volume.GetVertexCount();
	
	boxFrom = volume.GetVertexAt(0);
	boxTo = boxFrom;
	for(v=1; v<vertexCount; v++){
		const decVector &position = volume.GetVertexAt(v);
		
		if(position.x < boxFrom.x){
			boxFrom.x = position.x;
		}else if(position.x > boxTo.x){
			boxTo.x = position.x;
		}
		
		if(position.y < boxFrom.y){
			boxFrom.y = position.y;
		}else if(position.y > boxTo.y){
			boxTo.y = position.y;
		}
		
		if(position.z < boxFrom.z){
			boxFrom.z = position.z;
		}else if(position.z > boxTo.z){
			boxTo.z = position.z;
		}
	}
}
