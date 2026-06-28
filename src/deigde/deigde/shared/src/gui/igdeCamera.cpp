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

#include "igdeCamera.h"
#include "igdeCommonDialogs.h"
#include "igdeUIHelper.h"
#include "../environment/igdeEnvironment.h"
#include "../loadsave/igdeLoadSaveCamera.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/camera/deCamera.h>
#include <dragengine/resources/camera/deCameraManager.h>


// Meta Context
//////////////////

// Class igdeCamera::MetaContext
//////////////////////////////////

igdeCamera::MetaContext::MetaContext(igdeCamera *camera) :
igdeMetaContext("igde.camera"),
pCamera(camera){
	SetProperties(MetaProperties::global.properties);
}

igdeCamera::MetaContext::~MetaContext() = default;

igdeCamera &igdeCamera::MetaContext::GetCameraRef() const{
	DEASSERT_NOTNULL(pCamera)
	return *pCamera;
}

igdeCamera::MetaContext::Ref igdeCamera::MetaContext::Capture() const{
	auto context = Ref::New(pCamera);
	context->pGuard = pCamera;
	return context;
}

igdeEnvironment &igdeCamera::MetaContext::GetEnvironment() const{
	return GetCameraRef().GetEnvironment();
}

igdeUndoSystem *igdeCamera::MetaContext::GetUndoSystem() const{
	return GetCameraRef().GetUndoSystem();
}

igdeClipboard *igdeCamera::MetaContext::GetClipboard() const{
	return &GetEnvironment().GetClipboard();
}


// Meta Properties
////////////////////

namespace{

class cActionCameraSetDefaultParams : public igdeMetaProperty::Action{
	igdeCamera &pCamera;
	const float pLowestIntensity, pHighestIntensity, pAdaptionTime;
	
public:
	cActionCameraSetDefaultParams(igdeCamera &camera, igdeWidget &owner,
		const igdeMetaContext::Ref &context, float lowestIntensity,
		float highestIntensity, float adaptionTime, const char *translationTag) :
	Action(owner, context, decString::Formatted("@{}", translationTag), nullptr,
		decString::Formatted("@{}.ToolTip", translationTag)),
	pCamera(camera),
	pLowestIntensity(lowestIntensity),
	pHighestIntensity(highestIntensity),
	pAdaptionTime(adaptionTime){}
	
	void OnAction() override{
		pCamera.SetDefaultParameters(pLowestIntensity, pHighestIntensity, pAdaptionTime);
	}
};

class cActionCameraLoad : public igdeMetaProperty::Action{
	igdeCamera &pCamera;
	
public:
	using Ref = deTObjectReference<cActionCameraLoad>;
	cActionCameraLoad(igdeCamera &camera, igdeWidget &owner, const igdeMetaContext::Ref &context) :
	Action(owner, context, "@Igde.WPCamera.Action.Load",
		owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiSearch),
		"@Igde.WPCamera.Action.Load.ToolTip"),
	pCamera(camera){}
	
	void OnAction() override{
		if(!igdeCommonDialogs::GetFileOpen(GetOwner(), "@Igde.WPCamera.Dialog.OpenCamera.Title",
		*GetEnvironment().GetFileSystemGame(), igdeCamera::patternCamera, igdeCamera::lastCameraFile)){
			return;
		}
		
		igdeLoadSaveCamera lscamera(GetEnvironment(), GetOwner().GetLogger(), "IGDE");
		lscamera.Load(igdeCamera::lastCameraFile, pCamera, GetEnvironment().GetFileSystemGame()->
			OpenFileForReading(decPath::CreatePathUnix(igdeCamera::lastCameraFile)));
	}
};

class cActionCameraSave : public igdeMetaProperty::Action{
	igdeCamera &pCamera;
	
public:
	using Ref = deTObjectReference<cActionCameraSave>;
	cActionCameraSave(igdeCamera &camera, igdeWidget &owner, const igdeMetaContext::Ref &context) :
	Action(owner, context, "@Igde.WPCamera.Action.Save",
		owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiSave),
		"@Igde.WPCamera.Action.Save.ToolTip"),
	pCamera(camera){}
	
	void OnAction() override{
		if(!igdeCommonDialogs::GetFileSave(GetOwner(), "@Igde.WPCamera.Dialog.SaveCamera.Title",
		*GetEnvironment().GetFileSystemGame(), igdeCamera::patternCamera, igdeCamera::lastCameraFile)){
			return;
		}
		
		igdeLoadSaveCamera lscamera(GetEnvironment(), GetOwner().GetLogger(), "IGDE");
		lscamera.Save(pCamera, GetEnvironment().GetFileSystemGame()->
			OpenFileForWriting(decPath::CreatePathUnix(igdeCamera::lastCameraFile)));
	}
};

}


// igdeCamera::MetaProperties::Position
igdeCamera::MetaProperties::Position::Position() :
TBase("igde.camera.position", "Igde.WPCamera.Position"){
}

igdeCamera::MetaProperties::Position::~Position() = default;

igdeMetaPropertyDVectorStorage::Storage &igdeCamera::MetaProperties::Position::GetStorage(
const igdeMetaContext::Ref &context) const{
	return Camera(context).GetMPPosition();
}

// igdeCamera::MetaProperties::Rotation
igdeCamera::MetaProperties::Rotation::Rotation() :
TBase("igde.camera.rotation", "Igde.WPCamera.Rotation"){
}

igdeCamera::MetaProperties::Rotation::~Rotation() = default;

igdeMetaPropertyVectorStorageQuaternion::Storage &igdeCamera::MetaProperties::Rotation::GetStorage(
const igdeMetaContext::Ref &context) const{
	return Camera(context).GetMPRotation();
}

// igdeCamera::MetaProperties::Distance
igdeCamera::MetaProperties::Distance::Distance() :
TBase("igde.camera.distance", "Igde.WPCamera.OrbitDistance"){
	SetLowerLimit(0.0f);
	SetEnableLowerLimit(true);
}

igdeCamera::MetaProperties::Distance::~Distance() = default;

igdeMetaPropertyFloatStorage::Storage &igdeCamera::MetaProperties::Distance::GetStorage(
const igdeMetaContext::Ref &context) const{
	return Camera(context).GetMPDistance();
}

// igdeCamera::MetaProperties::Fov
igdeCamera::MetaProperties::Fov::Fov() :
TBase("igde.camera.fov", "Igde.WPCamera.FieldOfView"){
	SetLowerLimit(0.0f);
	SetEnableLowerLimit(true);
	SetUpperLimit(180.0f);
	SetEnableUpperLimit(true);
	SetTickSpacing(15.0f);
	SetDefaultValue(90.0f);
}

igdeCamera::MetaProperties::Fov::~Fov() = default;

igdeMetaPropertyFloatStorage::Storage &igdeCamera::MetaProperties::Fov::GetStorage(
const igdeMetaContext::Ref &context) const{
	return Camera(context).GetMPFov();
}

// igdeCamera::MetaProperties::FovRatio
igdeCamera::MetaProperties::FovRatio::FovRatio() :
TBase("igde.camera.fovRatio", "Igde.WPCamera.FieldOfViewRatio"){
	SetLowerLimit(0.01f);
	SetEnableLowerLimit(true);
	SetDefaultValue(1.0f);
}

igdeCamera::MetaProperties::FovRatio::~FovRatio() = default;

igdeMetaPropertyFloatStorage::Storage &igdeCamera::MetaProperties::FovRatio::GetStorage(
const igdeMetaContext::Ref &context) const{
	return Camera(context).GetMPFovRatio();
}

// igdeCamera::MetaProperties::ImageDistance
igdeCamera::MetaProperties::ImageDistance::ImageDistance() :
TBase("igde.camera.imageDistance", "Igde.WPCamera.ImageDistance"){
	SetLowerLimit(1e-4f);
	SetEnableLowerLimit(true);
	SetDefaultValue(0.01f);
}

igdeCamera::MetaProperties::ImageDistance::~ImageDistance() = default;

igdeMetaPropertyFloatStorage::Storage &igdeCamera::MetaProperties::ImageDistance::GetStorage(
const igdeMetaContext::Ref &context) const{
	return Camera(context).GetMPImageDistance();
}

// igdeCamera::MetaProperties::ViewDistance
igdeCamera::MetaProperties::ViewDistance::ViewDistance() :
TBase("igde.camera.viewDistance", "Igde.WPCamera.ViewDistance"){
	SetLowerLimit(1e-3f);
	SetEnableLowerLimit(true);
	SetDefaultValue(200.0f);
}

igdeCamera::MetaProperties::ViewDistance::~ViewDistance() = default;

igdeMetaPropertyFloatStorage::Storage &igdeCamera::MetaProperties::ViewDistance::GetStorage(
const igdeMetaContext::Ref &context) const{
	return Camera(context).GetMPViewDistance();
}

// igdeCamera::MetaProperties::EnableHDRR
igdeCamera::MetaProperties::EnableHDRR::EnableHDRR() :
TBase("igde.camera.enableHDRR", "Igde.WPCamera.Action.EnableHDRR"){
	SetDefaultValue(true);
}

igdeCamera::MetaProperties::EnableHDRR::~EnableHDRR() = default;

igdeMetaPropertyBooleanStorage::Storage &igdeCamera::MetaProperties::EnableHDRR::GetStorage(
const igdeMetaContext::Ref &context) const{
	return Camera(context).GetMPEnableHDRR();
}

// igdeCamera::MetaProperties::EnableGI
igdeCamera::MetaProperties::EnableGI::EnableGI() :
TBase("igde.camera.enableGI", "Igde.WPCamera.Action.EnableGI"){
	SetDefaultValue(true);
}

igdeCamera::MetaProperties::EnableGI::~EnableGI() = default;

igdeMetaPropertyBooleanStorage::Storage &igdeCamera::MetaProperties::EnableGI::GetStorage(
const igdeMetaContext::Ref &context) const{
	return Camera(context).GetMPEnableGI();
}

// igdeCamera::MetaProperties::Exposure
igdeCamera::MetaProperties::Exposure::Exposure() :
TBase("igde.camera.exposure", "Igde.WPCamera.Exposure"){
	SetLowerLimit(0.0f);
	SetEnableLowerLimit(true);
	SetDefaultValue(1.0f);
}

igdeCamera::MetaProperties::Exposure::~Exposure() = default;

igdeMetaPropertyFloatStorage::Storage &igdeCamera::MetaProperties::Exposure::GetStorage(
const igdeMetaContext::Ref &context) const{
	return Camera(context).GetMPExposure();
}

// igdeCamera::MetaProperties::LowestIntensity
igdeCamera::MetaProperties::LowestIntensity::LowestIntensity() :
TBase("igde.camera.lowestIntensity", "Igde.WPCamera.LowerIntensity"){
	SetLowerLimit(0.0f);
	SetEnableLowerLimit(true);
	SetDefaultValue(1.0f);
}

igdeCamera::MetaProperties::LowestIntensity::~LowestIntensity() = default;

igdeMetaPropertyFloatStorage::Storage &igdeCamera::MetaProperties::LowestIntensity::GetStorage(
const igdeMetaContext::Ref &context) const{
	return Camera(context).GetMPLowestIntensity();
}

// igdeCamera::MetaProperties::HighestIntensity
igdeCamera::MetaProperties::HighestIntensity::HighestIntensity() :
TBase("igde.camera.highestIntensity", "Igde.WPCamera.HigherIntensity"){
	SetLowerLimit(0.0f);
	SetEnableLowerLimit(true);
	SetDefaultValue(1.0f);
}

igdeCamera::MetaProperties::HighestIntensity::~HighestIntensity() = default;

igdeMetaPropertyFloatStorage::Storage &igdeCamera::MetaProperties::HighestIntensity::GetStorage(
const igdeMetaContext::Ref &context) const{
	return Camera(context).GetMPHighestIntensity();
}

// igdeCamera::MetaProperties::AdaptionTime
igdeCamera::MetaProperties::AdaptionTime::AdaptionTime() :
TBase("igde.camera.adaptionTime", "Igde.WPCamera.AdaptionTime"){
	SetLowerLimit(0.0f);
	SetEnableLowerLimit(true);
	SetDefaultValue(0.1f);
}

igdeCamera::MetaProperties::AdaptionTime::~AdaptionTime() = default;

igdeMetaPropertyFloatStorage::Storage &igdeCamera::MetaProperties::AdaptionTime::GetStorage(
const igdeMetaContext::Ref &context) const{
	return Camera(context).GetMPAdaptionTime();
}

// igdeCamera::MetaProperties::WhiteIntensity
igdeCamera::MetaProperties::WhiteIntensity::WhiteIntensity() :
TBase("igde.camera.whiteIntensity", "Igde.WPCamera.WhiteIntensity"){
	SetLowerLimit(0.01f);
	SetEnableLowerLimit(true);
	SetDefaultValue(2.0f);
}

igdeCamera::MetaProperties::WhiteIntensity::~WhiteIntensity() = default;

igdeMetaPropertyFloatStorage::Storage &igdeCamera::MetaProperties::WhiteIntensity::GetStorage(
const igdeMetaContext::Ref &context) const{
	return Camera(context).GetMPWhiteIntensity();
}

// igdeCamera::MetaProperties::BloomIntensity
igdeCamera::MetaProperties::BloomIntensity::BloomIntensity() :
TBase("igde.camera.bloomIntensity", "Igde.WPCamera.BloomIntensity"){
	SetLowerLimit(0.0f);
	SetEnableLowerLimit(true);
	SetDefaultValue(1.0f);
}

igdeCamera::MetaProperties::BloomIntensity::~BloomIntensity() = default;

igdeMetaPropertyFloatStorage::Storage &igdeCamera::MetaProperties::BloomIntensity::GetStorage(
const igdeMetaContext::Ref &context) const{
	return Camera(context).GetMPBloomIntensity();
}

// igdeCamera::MetaProperties::BloomStrength
igdeCamera::MetaProperties::BloomStrength::BloomStrength() :
TBase("igde.camera.bloomStrength", "Igde.WPCamera.BloomStrength"){
	SetLowerLimit(0.0f);
	SetEnableLowerLimit(true);
	SetDefaultValue(0.25f);
}

igdeCamera::MetaProperties::BloomStrength::~BloomStrength() = default;

igdeMetaPropertyFloatStorage::Storage &igdeCamera::MetaProperties::BloomStrength::GetStorage(
const igdeMetaContext::Ref &context) const{
	return Camera(context).GetMPBloomStrength();
}

// igdeCamera::MetaProperties::BloomBlend
igdeCamera::MetaProperties::BloomBlend::BloomBlend() :
TBase("igde.camera.bloomBlend", "Igde.WPCamera.BloomBlend"){
	SetLowerLimit(0.0f);
	SetEnableLowerLimit(true);
	SetUpperLimit(1.0f);
	SetEnableUpperLimit(true);
	SetDefaultValue(1.0f);
}

igdeCamera::MetaProperties::BloomBlend::~BloomBlend() = default;

igdeMetaPropertyFloatStorage::Storage &igdeCamera::MetaProperties::BloomBlend::GetStorage(
const igdeMetaContext::Ref &context) const{
	return Camera(context).GetMPBloomBlend();
}

// igdeCamera::MetaProperties::BloomSize
igdeCamera::MetaProperties::BloomSize::BloomSize() :
TBase("igde.camera.bloomSize", "Igde.WPCamera.BloomSize"){
	SetLowerLimit(0.0f);
	SetEnableLowerLimit(true);
	SetUpperLimit(1.0f);
	SetEnableUpperLimit(true);
	SetDefaultValue(0.25f);
}

igdeCamera::MetaProperties::BloomSize::~BloomSize() = default;

igdeMetaPropertyFloatStorage::Storage &igdeCamera::MetaProperties::BloomSize::GetStorage(
const igdeMetaContext::Ref &context) const{
	return Camera(context).GetMPBloomSize();
}

// igdeCamera::MetaProperties::ToneMapCurve
igdeCamera::MetaProperties::ToneMapCurve::ToneMapCurve() :
TBase("igde.camera.toneMapCurve", "Igde.WPCamera.ToneMapCurve"){
	SetClampMin(decVector2(0.0f, 0.0f));
	SetClampMax(decVector2(3.0f, 1.0f));
	SetClamp(true);
	// default is empty curve
}

igdeCamera::MetaProperties::ToneMapCurve::~ToneMapCurve() = default;

igdeMetaPropertyCurveBezierStorage::Storage &igdeCamera::MetaProperties::ToneMapCurve::GetStorage(
const igdeMetaContext::Ref &context) const{
	return Camera(context).GetMPToneMapCurve();
}

// Properties

igdeCamera::MetaProperties igdeCamera::MetaProperties::global;

igdeCamera::MetaProperties::MetaProperties() :
position(deTObjectReference<Position>::New()),
rotation(deTObjectReference<Rotation>::New()),
distance(deTObjectReference<Distance>::New()),
enableHDRR(deTObjectReference<EnableHDRR>::New()),
enableGI(deTObjectReference<EnableGI>::New()),

fov(deTObjectReference<Fov>::New()),
fovRatio(deTObjectReference<FovRatio>::New()),
imageDistance(deTObjectReference<ImageDistance>::New()),
viewDistance(deTObjectReference<ViewDistance>::New()),
groupInternal(deTObjectReference<igdeMetaPropertyGroup>::New("igde.camera.groupInternal",
	"Igde.WPCamera.InternalParameters", decTObjectOrderedSet<igdeMetaProperty>(devctag,
		fov, fovRatio, imageDistance, viewDistance), true)),

exposure(deTObjectReference<Exposure>::New()),
lowestIntensity(deTObjectReference<LowestIntensity>::New()),
highestIntensity(deTObjectReference<HighestIntensity>::New()),
adaptionTime(deTObjectReference<AdaptionTime>::New()),
groupExposure(deTObjectReference<igdeMetaPropertyGroup>::New("igde.camera.groupExposure",
	"Igde.WPCamera.ExposureControls", decTObjectOrderedSet<igdeMetaProperty>(devctag,
		exposure, lowestIntensity, highestIntensity, adaptionTime), true)),

whiteIntensity(deTObjectReference<WhiteIntensity>::New()),
toneMapCurve(deTObjectReference<ToneMapCurve>::New()),
groupToneMap(deTObjectReference<igdeMetaPropertyGroup>::New("igde.camera.groupToneMap",
	"Igde.WPCamera.ToneMapping", decTObjectOrderedSet<igdeMetaProperty>(devctag,
		whiteIntensity, toneMapCurve), true)),

bloomIntensity(deTObjectReference<BloomIntensity>::New()),
bloomStrength(deTObjectReference<BloomStrength>::New()),
bloomBlend(deTObjectReference<BloomBlend>::New()),
bloomSize(deTObjectReference<BloomSize>::New()),
groupBloom(deTObjectReference<igdeMetaPropertyGroup>::New("igde.camera.groupBloom",
	"Igde.WPCamera.BloomOverbright", decTObjectOrderedSet<igdeMetaProperty>(devctag,
		bloomIntensity, bloomStrength, bloomBlend, bloomSize), true)),

properties(igdeMetaContext::PropertyList::Ref::New(decTObjectOrderedSet<igdeMetaProperty>(devctag,
	position, rotation, distance, enableHDRR, enableGI,
	groupInternal, groupExposure, groupToneMap, groupBloom))){
}


// Class igdeCamera
/////////////////////

decString igdeCamera::lastCameraFile("Camera.decamera");

const igdeFilePattern::List igdeCamera::patternCamera(igdeFilePattern::List(devctag,
	igdeFilePattern::Ref::New("@Igde.LoadSaveSystem.FilePattern.Camera", "*.decamera", ".decamera")));

// Constructor, destructor
////////////////////////////

igdeCamera::igdeCamera(igdeEnvironment &environment, deEngine *engine) :
pEnvironment(environment),
pUndoSystem(nullptr),
pMetaContext(MetaContext::Ref::New(this)),
pName("Camera"),
pEngine(engine),
pMPPosition(MetaProperties::global.position, pMetaContext),
pMPRotation(MetaProperties::global.rotation, pMetaContext),
pMPDistance(MetaProperties::global.distance, pMetaContext),
pMPFov(MetaProperties::global.fov, pMetaContext),
pMPFovRatio(MetaProperties::global.fovRatio, pMetaContext),
pMPImageDistance(MetaProperties::global.imageDistance, pMetaContext),
pMPViewDistance(MetaProperties::global.viewDistance, pMetaContext),
pMPEnableHDRR(MetaProperties::global.enableHDRR, pMetaContext),
pMPEnableGI(MetaProperties::global.enableGI, pMetaContext),
pMPExposure(MetaProperties::global.exposure, pMetaContext),
pMPLowestIntensity(MetaProperties::global.lowestIntensity, pMetaContext),
pMPHighestIntensity(MetaProperties::global.highestIntensity, pMetaContext),
pMPAdaptionTime(MetaProperties::global.adaptionTime, pMetaContext),
pMPWhiteIntensity(MetaProperties::global.whiteIntensity, pMetaContext),
pMPBloomIntensity(MetaProperties::global.bloomIntensity, pMetaContext),
pMPBloomStrength(MetaProperties::global.bloomStrength, pMetaContext),
pMPBloomBlend(MetaProperties::global.bloomBlend, pMetaContext),
pMPBloomSize(MetaProperties::global.bloomSize, pMetaContext),
pMPToneMapCurve(MetaProperties::global.toneMapCurve, pMetaContext)
{
	DEASSERT_NOTNULL(engine)
	
	pEngCamera = engine->GetCameraManager()->CreateCamera();
	
	pMPFov.SetValue(pEngCamera->GetFov() * RAD2DEG, false);
	pMPFovRatio.SetValue(pEngCamera->GetFovRatio(), false);
	pMPImageDistance.SetValue(pEngCamera->GetImageDistance(), false);
	pMPViewDistance.SetValue(pEngCamera->GetViewDistance(), false);
	
	pEngCamera->SetEnableGI(pMPEnableGI);
	pEngCamera->SetEnableHDRR(pMPEnableHDRR);
	
	pMPExposure.SetValue(pEngCamera->GetExposure(), false);
	pMPLowestIntensity.SetValue(pEngCamera->GetLowestIntensity(), false);
	pMPHighestIntensity.SetValue(pEngCamera->GetHighestIntensity(), false);
	pMPAdaptionTime.SetValue(pEngCamera->GetAdaptionTime(), false);
	
	pMPWhiteIntensity.SetValue(pEngCamera->GetWhiteIntensity(), false);
	pMPBloomIntensity.SetValue(pEngCamera->GetBloomIntensity(), false);
	pMPBloomStrength.SetValue(pEngCamera->GetBloomStrength(), false);
	pMPBloomBlend.SetValue(pEngCamera->GetBloomBlend(), false);
	pMPBloomSize.SetValue(pEngCamera->GetBloomSize(), false);
	pMPToneMapCurve.SetValue(pEngCamera->GetToneMapCurve(), false);
	
	pMPPosition.onValueChanged = [this](){
		pUpdateViewMatrix();
		pUpdateCameraPosition();
		GeometryChanged();
		onChanged();
	};
	pMPRotation.onValueChanged = pMPPosition.onValueChanged;
	
	pMPDistance.onValueChanged = [this](){
		pUpdateViewMatrix();
		pUpdateCameraPosition();
		onChanged();
	};
	
	pMPFov.onValueChanged = [this](){
		pEngCamera->SetFov(pMPFov * DEG2RAD);
		GeometryChanged();
		onChanged();
	};
	
	pMPFovRatio.onValueChanged = [this](){
		pEngCamera->SetFovRatio(pMPFovRatio);
		GeometryChanged();
		onChanged();
	};
	
	pMPImageDistance.onValueChanged = [this](){
		pEngCamera->SetImageDistance(pMPImageDistance);
		GeometryChanged();
		onChanged();
	};
	
	pMPViewDistance.onValueChanged = [this](){
		pEngCamera->SetViewDistance(pMPViewDistance);
		GeometryChanged();
		onChanged();
	};
	
	pMPEnableHDRR.onValueChanged = [this](){
		pEngCamera->SetEnableHDRR(pMPEnableHDRR);
		AdaptionChanged();
		onChanged();
	};
	
	pMPEnableGI.onValueChanged = [this](){
		pEngCamera->SetEnableGI(pMPEnableGI);
		AdaptionChanged();
		onChanged();
	};
	
	pMPExposure.onValueChanged = [this](){
		pEngCamera->SetExposure(pMPExposure);
		AdaptionChanged();
		onChanged();
	};
	
	pMPLowestIntensity.onValueChanged = [this](){
		pEngCamera->SetLowestIntensity(pMPLowestIntensity);
		AdaptionChanged();
		onChanged();
	};
	
	pMPHighestIntensity.onValueChanged = [this](){
		pEngCamera->SetHighestIntensity(pMPHighestIntensity);
		AdaptionChanged();
		onChanged();
	};
	
	pMPAdaptionTime.onValueChanged = [this](){
		pEngCamera->SetAdaptionTime(pMPAdaptionTime);
		AdaptionChanged();
		onChanged();
	};
	
	pMPWhiteIntensity.onValueChanged = [this](){
		pEngCamera->SetWhiteIntensity(pMPWhiteIntensity);
		AdaptionChanged();
		onChanged();
	};
	
	pMPBloomIntensity.onValueChanged = [this](){
		pEngCamera->SetBloomIntensity(pMPBloomIntensity);
		AdaptionChanged();
		onChanged();
	};
	
	pMPBloomStrength.onValueChanged = [this](){
		pEngCamera->SetBloomStrength(pMPBloomStrength);
		AdaptionChanged();
		onChanged();
	};
	
	pMPBloomBlend.onValueChanged = [this](){
		pEngCamera->SetBloomBlend(pMPBloomBlend);
		AdaptionChanged();
		onChanged();
	};
	
	pMPBloomSize.onValueChanged = [this](){
		pEngCamera->SetBloomSize(pMPBloomSize);
		AdaptionChanged();
		onChanged();
	};
	
	pMPToneMapCurve.onValueChanged = [this](){
		pEngCamera->SetToneMapCurve(pMPToneMapCurve);
		AdaptionChanged();
		onChanged();
	};
}

igdeCamera::~igdeCamera(){
	pCleanUp();
}



// Management
///////////////

void igdeCamera::SetUndoSystem(igdeUndoSystem *undoSystem){
	pUndoSystem = undoSystem;
}

void igdeCamera::SetEngineWorld(deWorld *world){
	if(world == pEngWorld){
		return;
	}
	
	if(pEngWorld){
		pEngWorld->RemoveCamera(pEngCamera);
	}
	
	pEngWorld = world;
	
	if(world){
		pEngWorld->AddCamera(pEngCamera);
	}
	
	WorldChanged();
}

void igdeCamera::SetName(const char *value){
	pName = value;
}

void igdeCamera::SetPosition(const decDVector &value){
	pMPPosition = value;
}

void igdeCamera::SetOrientation(const decVector &value){
	pMPRotation = value;
}

void igdeCamera::SetFov(float value){
	pMPFov = value;
}

void igdeCamera::SetFovRatio(float value){
	pMPFovRatio = value;
}

void igdeCamera::SetImageDistance(float value){
	pMPImageDistance = value;
}

void igdeCamera::SetViewDistance(float value){
	pMPViewDistance = value;
}

void igdeCamera::SetEnableHDRR(bool value){
	pMPEnableHDRR = value;
}

void igdeCamera::SetExposure(float value){
	pMPExposure = value;
}

void igdeCamera::SetLowestIntensity(float value){
	pMPLowestIntensity = value;
}

void igdeCamera::SetHighestIntensity(float value){
	pMPHighestIntensity = value;
}

void igdeCamera::SetAdaptionTime(float value){
	pMPAdaptionTime = value;
}

void igdeCamera::SetEnableGI(bool value){
	pMPEnableGI = value;
}

void igdeCamera::SetWhiteIntensity(float value){
	pMPWhiteIntensity = value;
}

void igdeCamera::SetBloomIntensity(float value){
	pMPBloomIntensity = value;
}

void igdeCamera::SetBloomStrength(float value){
	pMPBloomStrength = value;
}

void igdeCamera::SetBloomBlend(float value){
	pMPBloomBlend = value;
}

void igdeCamera::SetBloomSize(float value){
	pMPBloomSize = value;
}

void igdeCamera::SetToneMapCurve(const decCurveBezier &curve){
	pMPToneMapCurve = curve;
}

void igdeCamera::SetDistance(float value){
	pMPDistance = value;
}

void igdeCamera::Reset(){
	SetPosition(decDVector());
}

decVector igdeCamera::GetDirectionFor(int width, int height, int x, int y) const{
	float aspectRatio = (float)width / (float)height;
	int halfHeight = height / 2;
	int halfWidth = width / 2;
	decVector direction;
	
	direction.x = tanf(pMPFov * DEG2RAD * 0.5f) * ((float)(x - halfWidth) / (float)halfWidth);
	direction.y = tanf(pMPFov * DEG2RAD * pMPFovRatio * 0.5f)
		* ( ( float )( halfHeight - y ) / ( float )halfHeight ) / aspectRatio;
	direction.z = 1.0f;
	
	direction.Normalize();
	return pViewMatrix.TransformNormal(direction).ToVector();
}

void igdeCamera::SetDefaultParameters(float alowestIntensity, float ahighestIntensity, float aadaptionTime){
	const deCamera::Ref camera(pEngine->GetCameraManager()->CreateCamera());
	
	pMPFov.SetValue(camera->GetFov() * RAD2DEG, false);
	pMPFovRatio.SetValue(camera->GetFovRatio(), false);
	pMPImageDistance.SetValue(camera->GetImageDistance(), false);
	pMPViewDistance.SetValue(camera->GetViewDistance(), false);
	
	pMPEnableGI.SetValue(true, false);
	pMPEnableHDRR.SetValue(true, false);
	pMPExposure.SetValue(camera->GetExposure(), false);
	pMPLowestIntensity.SetValue(alowestIntensity, false);
	pMPHighestIntensity.SetValue(ahighestIntensity, false);
	pMPAdaptionTime.SetValue(aadaptionTime, false);
	
	pMPWhiteIntensity.SetValue(camera->GetWhiteIntensity(), false);
	pMPBloomIntensity.SetValue(camera->GetBloomIntensity(), false);
	pMPBloomStrength.SetValue(camera->GetBloomStrength(), false);
	pMPBloomBlend.SetValue(camera->GetBloomBlend(), false);
	pMPBloomSize.SetValue(camera->GetBloomSize(), false);
	pMPToneMapCurve.SetValue(camera->GetToneMapCurve(), false);
	
	pEngCamera->SetFov(pMPFov * DEG2RAD);
	pEngCamera->SetFovRatio(pMPFovRatio);
	pEngCamera->SetImageDistance(pMPImageDistance);
	pEngCamera->SetViewDistance(pMPViewDistance);
	pEngCamera->SetEnableHDRR(pMPEnableHDRR);
	pEngCamera->SetExposure(pMPExposure);
	pEngCamera->SetLowestIntensity(pMPLowestIntensity);
	pEngCamera->SetHighestIntensity(pMPHighestIntensity);
	pEngCamera->SetAdaptionTime(pMPAdaptionTime);
	pEngCamera->SetEnableGI(pMPEnableGI);
	pEngCamera->SetWhiteIntensity(pMPWhiteIntensity);
	pEngCamera->SetBloomIntensity(pMPBloomIntensity);
	pEngCamera->SetBloomStrength(pMPBloomStrength);
	pEngCamera->SetBloomBlend(pMPBloomBlend);
	pEngCamera->SetBloomSize(pMPBloomSize);
	pEngCamera->SetToneMapCurve(pMPToneMapCurve);
	
	GeometryChanged();
	AdaptionChanged();
	onChanged();
}

void igdeCamera::AddContextMenuEntries(igdeMenuCascade &menu, igdeWidget &owner){
	auto &env = menu.GetEnvironment();
	auto &helper = env.GetUIHelper();
	if(menu.GetChildren().IsNotEmpty()){
		helper.Separator(menu);
	}
	auto submenu = igdeMenuCascade::Ref::New(env, "@Igde.WPCamera.Action.SetDefault",
		env.GetStockIcon(igdeEnvironment::esiSearch));
	helper.MenuCommand(submenu, deTObjectReference<cActionCameraSetDefaultParams>::New(
		*this, owner, pMetaContext, 1.0f, 1.0f, 0.1f, "Igde.WPCamera.Action.SetDefaultIndoor"));
	helper.MenuCommand(submenu, deTObjectReference<cActionCameraSetDefaultParams>::New(
		*this, owner, pMetaContext, 20.0f, 20.0f, 0.1f, "Igde.WPCamera.Action.SetDefaultDay"));
	helper.MenuCommand(submenu, deTObjectReference<cActionCameraSetDefaultParams>::New(
		*this, owner, pMetaContext, 0.1f, 0.1f, 0.1f, "Igde.WPCamera.Action.SetDefaultNight"));
	helper.MenuCommand(submenu, deTObjectReference<cActionCameraSetDefaultParams>::New(
		*this, owner, pMetaContext, 1.0f, 20.0f, 0.1f, "Igde.WPCamera.Action.SetDefaultDynamic"));
	menu.AddChild(submenu);
	
	helper.MenuCommand(menu, deTObjectReference<cActionCameraLoad>::New(*this, owner, pMetaContext));
	helper.MenuCommand(menu, deTObjectReference<cActionCameraSave>::New(*this, owner, pMetaContext));
}


void igdeCamera::WorldChanged(){
}

void igdeCamera::GeometryChanged(){
}

void igdeCamera::ParameterChanged(){
}

void igdeCamera::AdaptionChanged(){
}



// Private Functions
//////////////////////

void igdeCamera::pCleanUp(){
	SetEngineWorld(nullptr);
	
	if(pMetaContext){
		pMetaContext->Dispose();
	}
}

void igdeCamera::pUpdateCameraPosition(){
	pEngCamera->SetPosition(pViewMatrix.GetPosition());
	pEngCamera->SetOrientation(pViewMatrix.ToQuaternion());
}

void igdeCamera::pUpdateViewMatrix(){
	pViewMatrix = decDMatrix::CreateTranslation(0.0, 0.0, -(double)pMPDistance)
		* decDMatrix::CreateWorld(pMPPosition, pMPRotation);
}
