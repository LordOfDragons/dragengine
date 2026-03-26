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

#ifndef _DEOXRBODYTRACKER_H_
#define _DEOXRBODYTRACKER_H_

#include "deoxrBasics.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputDevicePose.h>
#include <dragengine/resources/rig/deRig.h>

class deoxrSession;


/**
 * Oxr body tracker.
 * 
 * Supports XR_FB_body_tracking (upper body, ebcUpperBody) and
 * XR_META_body_tracking_full_body (full body, ebcFullBody).
 * Hand bones are excluded as these are handled by deoxrHandTracker.
 */
class deoxrBodyTracker : public deObject{
public:
	/** Reference. */
	using Ref = deTObjectReference<deoxrBodyTracker>;
	
	struct sBoneMapping{
		int constOxr, constDe, parentBone;
		deInputDevicePose *bone;
		XrBodyJointLocationFB *location;
		const char *name;
	};
	
	
private:
	deoxrSession &pSession;
	
	const deInputDevice::eBoneConfigurations pBodyConfiguration;
	XrBodyTrackerFB pBodyTracker;
	
	decTList<XrBodyJointLocationFB> pJointLocations;
	
	XrBodyJointsLocateInfoFB pLocateInfo;
	XrBodyJointLocationsFB pLocations;
	
	decTList<deInputDevicePose> pPoseBones;
	decTList<sBoneMapping> pMapBoneXrToDe;
	
	deRig::Ref pBodyRig;
	uint32_t pLastSkeletonChangedCount;
	bool pSkeletonDirty;
	
	decVector pRootPosition;
	decQuaternion pRootOrientation;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create body tracker. */
	deoxrBodyTracker(deoxrSession &session, deInputDevice::eBoneConfigurations bodyConfiguration);
	
protected:
	/** Clean up body tracker. */
	~deoxrBodyTracker() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Session. */
	inline deoxrSession &GetSession() const{ return pSession; }
	
	/** Body configuration. */
	inline deInputDevice::eBoneConfigurations GetBodyConfiguration() const{ return pBodyConfiguration; }
	
	/** Body tracker. */
	inline XrBodyTrackerFB GetBodyTracker() const{ return pBodyTracker; }
	
	/** Body rig or nullptr if not yet fetched. */
	inline const deRig::Ref &GetBodyRig() const{ return pBodyRig; }
	
	/** Skeleton dirty flag indicating the rig has changed and needs to be propagated. */
	inline bool GetSkeletonDirty() const{ return pSkeletonDirty; }
	
	/** Clear skeleton dirty flag. */
	void SetSkeletonDirty(bool dirty);
	
	/** Locate body joints. */
	void Locate();
	
	/** Count of pose bones. */
	inline int GetPoseBoneCount() const{ return pPoseBones.GetCount(); }
	
	/** Pose bone at index. */
	deInputDevicePose &GetPoseBoneAt(int index);
	const deInputDevicePose &GetPoseBoneAt(int index) const;
	
	/** Log bone poses. */
	void LogPoseBones(const char *prefix) const;
	
	/** Reference pose changed. */
	void ReferencePoseChanged();
	
	/** Root position. */
	inline const decVector &GetRootPosition() const{ return pRootPosition; }
	
	/** Root orientation. */
	inline const decQuaternion &GetRootOrientation() const{ return pRootOrientation; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pSetBoneMapping(int index, int constDe, int constOxr, const char *name, int parentBone);
	void pFetchSkeleton();
};

#endif
