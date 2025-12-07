// include only once
#ifndef _DETCONVEXVOLUME_H_
#define _DETCONVEXVOLUME_H_

// includes
#include "../detCase.h"
#include "dragengine/common/math/decMath.h"

// predefinitions
class decConvexVolumeFace;
class decConvexVolume;
class decConvexVolumeList;



// class detConvexVolume
class detConvexVolume : public detCase{
public:
	detConvexVolume();
	~detConvexVolume();
	void Prepare();
	void Run();
	void CleanUp();
	const char *GetTestName();
	
private:
	void TestVolumeAddVertices();
	void TestFaceAddVertices();
	void TestFaceSortVertices();
	void TestVolumeSetToCube();
	void TestSplitVolumeByFace();
	void TestSplitVolumeByPlane();
	void TestCropVolumeByPlane();
	void TestCropVolumeByVolume();
	void TestCropVolumeByVolume2();
	void TestCropVolumeByVolume3();
	void TestFaceValidity();
	void TestVolume();
	void TestSpecial1();
	
	decConvexVolumeFace *pGetFaceWithNormal(const decConvexVolume &volume, const decVector &normal) const;
	decConvexVolume *pGetVolumeWithVertex(const decConvexVolumeList &list, const decVector &vertex) const;
	int pIsVolumeCube(const decConvexVolumeList &list) const;
	int pIsVolumeCubeSplit(const decConvexVolumeList &list, const decVector &splitNormal) const;
	int pIsVolumeCubeCropped(const decConvexVolumeList &list, const decVector &splitNormal) const;
	int pIsVolumeCubeCroppedInverse(const decConvexVolumeList &list, const decVector &splitNormal) const;
	int pIsKindOfCube(const decConvexVolume &volume) const;
	void pSetSplitVolume(decConvexVolume &volume, const decVector &v1, const decVector &v2, const decVector &v3, const decVector &v4, const decVector &normal) const;
	void pInvertSplitFace(decConvexVolume &volume);
	void pBoxFromCube(const decConvexVolume &volume, decVector &boxFrom, decVector &boxTo);
};

// end of include only once
#endif

