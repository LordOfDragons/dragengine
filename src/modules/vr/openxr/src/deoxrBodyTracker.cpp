/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "deVROpenXR.h"
#include "deoxrBodyTracker.h"
#include "deoxrSession.h"
#include "deoxrUtils.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/rig/deRigBuilder.h>
#include <dragengine/resources/rig/deRigManager.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class deoxrBodyTracker
///////////////////////////

deoxrBodyTracker::deoxrBodyTracker(deoxrSession &session,
	deInputDevice::eBoneConfigurations bodyConfiguration) :
pSession(session),
pBodyConfiguration(bodyConfiguration),
pBodyTracker(XR_NULL_HANDLE),
pLocateInfo{},
pLocations{},
pLastSkeletonChangedCount(0),
pSkeletonDirty(false)
{
	deoxrInstance &instance = session.GetSystem().GetInstance();
	
	try{
		// create body tracker
		XrBodyTrackerCreateInfoFB createInfo{XR_TYPE_BODY_TRACKER_CREATE_INFO_FB};
		createInfo.bodyJointSet = bodyConfiguration == deInputDevice::ebcFullBody
			? XR_BODY_JOINT_SET_FULL_BODY_META : XR_BODY_JOINT_SET_DEFAULT_FB;
		
		OXR_CHECK(instance.xrCreateBodyTrackerFB(session.GetSession(), &createInfo, &pBodyTracker));
		
		// allocate joint locations
		if(bodyConfiguration == deInputDevice::ebcFullBody){
			pJointLocations.SetAll(XR_FULL_BODY_JOINT_COUNT_META, {});
			
		}else{
			pJointLocations.SetAll(XR_BODY_JOINT_COUNT_FB, {});
		}
		pJointLocations.Visit([](XrBodyJointLocationFB &loc){
			loc.pose.orientation.w = 1.0f;
		});
		
		// initialize structures used to fetch joints
		pLocateInfo.type = XR_TYPE_BODY_JOINTS_LOCATE_INFO_FB;
		pLocateInfo.baseSpace = session.GetMainSpace()->GetSpace();
		
		pLocations.type = XR_TYPE_BODY_JOINT_LOCATIONS_FB;
		pLocations.jointCount = pJointLocations.GetCount();
		pLocations.jointLocations = pJointLocations.GetArrayPointer();
		
		// create bone pose arrays and bone mapping table.
		// upper body and full body share the same 17 upper body bones (1-17 in both extensions).
		// full body adds 14 leg/foot bones (70-83 in META extension, mapped to indices 17-30).
		// hand joints (18-69 in both extensions) are excluded as they are handled by deoxrHandTracker.
		if(bodyConfiguration == deInputDevice::ebcFullBody){
			pPoseBones.SetAll(deInputDevice::FullBodyBoneCount, {});
			pMapBoneXrToDe.SetAll(deInputDevice::FullBodyBoneCount, {});
			
			// upper body bones (same indices as in FB extension)
			pSetBoneMapping(0, deInputDevice::efbbHips, XR_FULL_BODY_JOINT_HIPS_META, "hips", -1);
			pSetBoneMapping(1, deInputDevice::efbbSpineLower, XR_FULL_BODY_JOINT_SPINE_LOWER_META, "spineLower", 0);
			pSetBoneMapping(2, deInputDevice::efbbSpineMiddle, XR_FULL_BODY_JOINT_SPINE_MIDDLE_META, "spineMiddle", 1);
			pSetBoneMapping(3, deInputDevice::efbbSpineUpper, XR_FULL_BODY_JOINT_SPINE_UPPER_META, "spineUpper", 2);
			pSetBoneMapping(4, deInputDevice::efbbChest, XR_FULL_BODY_JOINT_CHEST_META, "chest", 3);
			pSetBoneMapping(5, deInputDevice::efbbNeck, XR_FULL_BODY_JOINT_NECK_META, "neck", 4);
			pSetBoneMapping(6, deInputDevice::efbbHead, XR_FULL_BODY_JOINT_HEAD_META, "head", 5);
			pSetBoneMapping(7, deInputDevice::efbbLeftShoulder, XR_FULL_BODY_JOINT_LEFT_SHOULDER_META, "shoulder.l", 4);
			pSetBoneMapping(8, deInputDevice::efbbLeftScapula, XR_FULL_BODY_JOINT_LEFT_SCAPULA_META, "scapula.l", 7);
			pSetBoneMapping(9, deInputDevice::efbbLeftArmUpper, XR_FULL_BODY_JOINT_LEFT_ARM_UPPER_META, "armUpper.l", 8);
			pSetBoneMapping(10, deInputDevice::efbbLeftArmLower, XR_FULL_BODY_JOINT_LEFT_ARM_LOWER_META, "armLower.l", 9);
			pSetBoneMapping(11, deInputDevice::efbbLeftHandWristTwist, XR_FULL_BODY_JOINT_LEFT_HAND_WRIST_TWIST_META, "handWristTwist.l", 10);
			pSetBoneMapping(12, deInputDevice::efbbLeftHandWrist, XR_FULL_BODY_JOINT_LEFT_HAND_WRIST_META, "handWrist.l", 11);
			pSetBoneMapping(13, deInputDevice::efbbRightShoulder, XR_FULL_BODY_JOINT_RIGHT_SHOULDER_META, "shoulder.r", 4);
			pSetBoneMapping(14, deInputDevice::efbbRightScapula, XR_FULL_BODY_JOINT_RIGHT_SCAPULA_META, "scapula.r", 13);
			pSetBoneMapping(15, deInputDevice::efbbRightArmUpper, XR_FULL_BODY_JOINT_RIGHT_ARM_UPPER_META, "armUpper.r", 14);
			pSetBoneMapping(16, deInputDevice::efbbRightArmLower, XR_FULL_BODY_JOINT_RIGHT_ARM_LOWER_META, "armLower.r", 15);
			pSetBoneMapping(17, deInputDevice::efbbRightHandWristTwist, XR_FULL_BODY_JOINT_RIGHT_HAND_WRIST_TWIST_META, "handWristTwist.r", 16);
			pSetBoneMapping(18, deInputDevice::efbbRightHandWrist, XR_FULL_BODY_JOINT_RIGHT_HAND_WRIST_META, "handWrist.r", 17);
			
			// leg/foot bones (META extension joints 70-83)
			pSetBoneMapping(19, deInputDevice::efbbLeftUpperLeg, XR_FULL_BODY_JOINT_LEFT_UPPER_LEG_META, "upperLeg.l", 0);
			pSetBoneMapping(20, deInputDevice::efbbLeftLowerLeg, XR_FULL_BODY_JOINT_LEFT_LOWER_LEG_META, "lowerLeg.l", 19);
			pSetBoneMapping(21, deInputDevice::efbbLeftFootAnkleTwist, XR_FULL_BODY_JOINT_LEFT_FOOT_ANKLE_TWIST_META, "footAnkleTwist.l", 20);
			pSetBoneMapping(22, deInputDevice::efbbLeftFootAnkle, XR_FULL_BODY_JOINT_LEFT_FOOT_ANKLE_META, "footAnkle.l", 21);
			pSetBoneMapping(23, deInputDevice::efbbLeftFootSubtalar, XR_FULL_BODY_JOINT_LEFT_FOOT_SUBTALAR_META, "footSubtalar.l", 22);
			pSetBoneMapping(24, deInputDevice::efbbLeftFootTransverse, XR_FULL_BODY_JOINT_LEFT_FOOT_TRANSVERSE_META, "footTransverse.l", 23);
			pSetBoneMapping(25, deInputDevice::efbbLeftFootBall, XR_FULL_BODY_JOINT_LEFT_FOOT_BALL_META, "footBall.l", 24);
			pSetBoneMapping(26, deInputDevice::efbbRightUpperLeg, XR_FULL_BODY_JOINT_RIGHT_UPPER_LEG_META, "upperLeg.r", 0);
			pSetBoneMapping(27, deInputDevice::efbbRightLowerLeg, XR_FULL_BODY_JOINT_RIGHT_LOWER_LEG_META, "lowerLeg.r", 26);
			pSetBoneMapping(28, deInputDevice::efbbRightFootAnkleTwist, XR_FULL_BODY_JOINT_RIGHT_FOOT_ANKLE_TWIST_META, "footAnkleTwist.r", 27);
			pSetBoneMapping(29, deInputDevice::efbbRightFootAnkle, XR_FULL_BODY_JOINT_RIGHT_FOOT_ANKLE_META, "footAnkle.r", 28);
			pSetBoneMapping(30, deInputDevice::efbbRightFootSubtalar, XR_FULL_BODY_JOINT_RIGHT_FOOT_SUBTALAR_META, "footSubtalar.r", 29);
			pSetBoneMapping(31, deInputDevice::efbbRightFootTransverse, XR_FULL_BODY_JOINT_RIGHT_FOOT_TRANSVERSE_META, "footTransverse.r", 30);
			pSetBoneMapping(32, deInputDevice::efbbRightFootBall, XR_FULL_BODY_JOINT_RIGHT_FOOT_BALL_META, "footBall.r", 31);
			
		}else{
			pPoseBones.SetAll(deInputDevice::UpperBodyBoneCount, {});
			pMapBoneXrToDe.SetAll(deInputDevice::UpperBodyBoneCount, {});
			
			pSetBoneMapping(0, deInputDevice::eubbHips, XR_BODY_JOINT_HIPS_FB, "hips", -1);
			pSetBoneMapping(1, deInputDevice::eubbSpineLower, XR_BODY_JOINT_SPINE_LOWER_FB, "spineLower", 0);
			pSetBoneMapping(2, deInputDevice::eubbSpineMiddle, XR_BODY_JOINT_SPINE_MIDDLE_FB, "spineMiddle", 1);
			pSetBoneMapping(3, deInputDevice::eubbSpineUpper, XR_BODY_JOINT_SPINE_UPPER_FB, "spineUpper", 2);
			pSetBoneMapping(4, deInputDevice::eubbChest, XR_BODY_JOINT_CHEST_FB, "chest", 3);
			pSetBoneMapping(5, deInputDevice::eubbNeck, XR_BODY_JOINT_NECK_FB, "neck", 4);
			pSetBoneMapping(6, deInputDevice::eubbHead, XR_BODY_JOINT_HEAD_FB, "head", 5);
			pSetBoneMapping(7, deInputDevice::eubbLeftShoulder, XR_BODY_JOINT_LEFT_SHOULDER_FB, "shoulder.l", 4);
			pSetBoneMapping(8, deInputDevice::eubbLeftScapula, XR_BODY_JOINT_LEFT_SCAPULA_FB, "scapula.l", 7);
			pSetBoneMapping(9, deInputDevice::eubbLeftArmUpper, XR_BODY_JOINT_LEFT_ARM_UPPER_FB, "armUpper.l", 8);
			pSetBoneMapping(10, deInputDevice::eubbLeftArmLower, XR_BODY_JOINT_LEFT_ARM_LOWER_FB, "armLower.l", 9);
			pSetBoneMapping(11, deInputDevice::eubbLeftHandWristTwist, XR_BODY_JOINT_LEFT_HAND_WRIST_TWIST_FB, "handWristTwist.l", 10);
			pSetBoneMapping(12, deInputDevice::eubbLeftHandWrist, XR_BODY_JOINT_LEFT_HAND_WRIST_FB, "handWrist.l", 11);
			pSetBoneMapping(13, deInputDevice::eubbRightShoulder, XR_BODY_JOINT_RIGHT_SHOULDER_FB, "shoulder.r", 4);
			pSetBoneMapping(14, deInputDevice::eubbRightScapula, XR_BODY_JOINT_RIGHT_SCAPULA_FB, "scapula.r", 13);
			pSetBoneMapping(15, deInputDevice::eubbRightArmUpper, XR_BODY_JOINT_RIGHT_ARM_UPPER_FB, "armUpper.r", 14);
			pSetBoneMapping(16, deInputDevice::eubbRightArmLower, XR_BODY_JOINT_RIGHT_ARM_LOWER_FB, "armLower.r", 15);
			pSetBoneMapping(17, deInputDevice::eubbRightHandWristTwist, XR_BODY_JOINT_RIGHT_HAND_WRIST_TWIST_FB, "handWristTwist.r", 16);
			pSetBoneMapping(18, deInputDevice::eubbRightHandWrist, XR_BODY_JOINT_RIGHT_HAND_WRIST_FB, "handWrist.r", 17);
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoxrBodyTracker::~deoxrBodyTracker(){
	pCleanUp();
}



// Management
///////////////

// #define FLAGS_POS_VALID (XR_SPACE_LOCATION_POSITION_TRACKED_BIT | XR_SPACE_LOCATION_POSITION_VALID_BIT)
// #define FLAGS_ROT_VALID (XR_SPACE_LOCATION_ORIENTATION_TRACKED_BIT | XR_SPACE_LOCATION_ORIENTATION_VALID_BIT)

// not checking the tracket bit since some devices (like the Quest) do not set this bit correctly
#define FLAGS_POS_VALID XR_SPACE_LOCATION_POSITION_VALID_BIT
#define FLAGS_ROT_VALID XR_SPACE_LOCATION_ORIENTATION_VALID_BIT

void deoxrBodyTracker::SetSkeletonDirty(bool dirty){
	pSkeletonDirty = dirty;
}

void deoxrBodyTracker::Locate(){
	const deoxrInstance &instance = pSession.GetSystem().GetInstance();
	
	pLocateInfo.time = pSession.GetPredictedDisplayTime();
	
	if(!XR_SUCCEEDED(instance.xrLocateBodyJointsFB(pBodyTracker, &pLocateInfo, &pLocations))
	|| !pLocations.isActive){
		return;
	}
	
	const decMatrix rootMatrix = deoxrUtils::Convert(pLocations.jointLocations[0].pose);
	pRootPosition = rootMatrix.GetPosition();
	pRootOrientation = rootMatrix.ToQuaternion();
	
	const decMatrix invRootMatrix = rootMatrix.QuickInvert();
	
	if(pLocations.skeletonChangedCount != pLastSkeletonChangedCount){
		pLastSkeletonChangedCount = pLocations.skeletonChangedCount;
		pFetchSkeleton();
		pSkeletonDirty = true;
	}
	
	pMapBoneXrToDe.Visit([&](const sBoneMapping &mapping){
		const decMatrix matrix(decMatrix::CreateWorld(
			deoxrUtils::Convert(mapping.location->pose.position),
			deoxrUtils::Convert(mapping.location->pose.orientation)) * invRootMatrix);
		
		if((mapping.location->locationFlags & FLAGS_POS_VALID) == FLAGS_POS_VALID){
			mapping.bone->SetPosition(matrix.GetPosition());
		}
		
		if((mapping.location->locationFlags & FLAGS_ROT_VALID) == FLAGS_ROT_VALID){
			mapping.bone->SetOrientation(matrix.ToQuaternion());
		}
	});
}

deInputDevicePose &deoxrBodyTracker::GetPoseBoneAt(int index){
	return pPoseBones[index];
}

const deInputDevicePose &deoxrBodyTracker::GetPoseBoneAt(int index) const{
	return pPoseBones[index];
}

void deoxrBodyTracker::LogPoseBones(const char *prefix) const{
	deVROpenXR &oxr = pSession.GetSystem().GetInstance().GetOxr();
	pPoseBones.VisitIndexed([&](int i, const deInputDevicePose &pose){
		const decVector &p = pose.GetPosition();
		const decVector o(pose.GetOrientation().GetEulerAngles() * RAD2DEG);
		const decVector &lv = pose.GetLinearVelocity();
		const decVector av(pose.GetAngularVelocity() * RAD2DEG);
		oxr.LogInfoFormat("%sBone %02d: pos=(%.3f,%.3f,%.3f) rot=(% 3f,% 3f,% 3f)"
			" lv=(% 3.1f,% 3.1f,% 3.1f) av=(% 3f,% 3f,% 3f)",
			prefix, i, p.x, p.y, p.z, o.x, o.y, o.z, lv.x, lv.y, lv.z, av.x, av.y, av.z);
	});
}

void deoxrBodyTracker::ReferencePoseChanged(){
	pLocateInfo.baseSpace = pSession.GetMainSpace()->GetSpace();
}



// Private Functions
//////////////////////

namespace {

class cBodyRigBuilder : public deRigBuilder{
private:
	const decTList<deoxrBodyTracker::sBoneMapping> &pMappings;
	const decTList<XrBodySkeletonJointFB> &pSkeletonJoints;
	
public:
	cBodyRigBuilder(const decTList<deoxrBodyTracker::sBoneMapping> &mappings,
		const decTList<XrBodySkeletonJointFB> &skeletonJoints) :
	pMappings(mappings), pSkeletonJoints(skeletonJoints){
	}
	
	void BuildRig(deRig *rig) override{
		decMatrix rootMatrix;
		const int skepRootIndex = pIndexOfOxrJoint(XR_BODY_JOINT_ROOT_FB);
		if(skepRootIndex != -1){
			rootMatrix = deoxrUtils::Convert(pSkeletonJoints[skepRootIndex].pose);
		}
		
		const decMatrix invRootMatrix = rootMatrix.QuickInvert();
		
		pMappings.VisitIndexed([&](int, const deoxrBodyTracker::sBoneMapping &mapping){
			auto bone = deRigBone::Ref::New(mapping.name);
			bone->SetParent(mapping.parentBone);
			
			decMatrix boneMatrix;
			
			const int skelIndex = pIndexOfOxrJoint(mapping.constOxr);
			if(skelIndex != -1){
				bool hasParent = false;
				
				if(mapping.parentBone != -1){
					const int skelParentIndex = pIndexOfOxrJoint(pMappings[mapping.parentBone].constOxr);
					if(skelParentIndex != -1){
						boneMatrix = deoxrUtils::Convert(pSkeletonJoints[skelIndex].pose)
							* deoxrUtils::Convert(pSkeletonJoints[skelParentIndex].pose).QuickInvert();
						hasParent = true;
					}
				}
				
				if(!hasParent){
					boneMatrix = deoxrUtils::Convert(pSkeletonJoints[skelIndex].pose) * invRootMatrix;
				}
			}
			
			bone->SetPosition(boneMatrix.GetPosition());
			bone->SetRotation(boneMatrix.GetEulerAngles());
			
			rig->AddBone(std::move(bone));
		});
	}
	
private:
	int pIndexOfOxrJoint(int constOxr) const{
		return pSkeletonJoints.IndexOfMatching([&](const XrBodySkeletonJointFB &joint){
			return joint.joint == constOxr;
		});
	}
};

}

void deoxrBodyTracker::pFetchSkeleton(){
	auto &instance = pSession.GetSystem().GetInstance();
	decTList<XrBodySkeletonJointFB> skeletonJoints(pJointLocations.GetCount(), {});
	
	XrBodySkeletonFB skeleton{XR_TYPE_BODY_SKELETON_FB};
	skeleton.jointCount = skeletonJoints.GetCount();
	skeleton.joints = skeletonJoints.GetArrayPointer();
	
	if(!XR_SUCCEEDED(instance.xrGetBodySkeletonFB(pBodyTracker, &skeleton))){
		return;
	}
	
	cBodyRigBuilder builder(pMapBoneXrToDe, skeletonJoints);
	pBodyRig = instance.GetOxr().GetGameEngine()->GetRigManager()->CreateRig("", builder);
}

void deoxrBodyTracker::pCleanUp(){
	if(pBodyTracker){
		pSession.GetSystem().GetInstance().xrDestroyBodyTrackerFB(pBodyTracker);
		pBodyTracker = XR_NULL_HANDLE;
	}
}

void deoxrBodyTracker::pSetBoneMapping(int index, int constDe, int constOxr, const char *name, int parentBone){
	sBoneMapping &m = pMapBoneXrToDe[index];
	m.constOxr = constOxr;
	m.constDe = constDe;
	m.location = &pJointLocations[constOxr];
	m.bone = &pPoseBones[constDe];
	m.parentBone = parentBone;
	m.name = name;
}
