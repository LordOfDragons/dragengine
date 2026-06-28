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
	return nullptr;
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
	return Camera(context).position;
}

// igdeCamera::MetaProperties::Rotation
igdeCamera::MetaProperties::Rotation::Rotation() :
TBase("igde.camera.rotation", "Igde.WPCamera.Rotation"){
}

igdeCamera::MetaProperties::Rotation::~Rotation() = default;

igdeMetaPropertyVectorStorageQuaternion::Storage &igdeCamera::MetaProperties::Rotation::GetStorage(
const igdeMetaContext::Ref &context) const{
	return Camera(context).rotation;
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
	return Camera(context).distance;
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
	return Camera(context).fov;
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
	return Camera(context).fovRatio;
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
	return Camera(context).imageDistance;
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
	return Camera(context).viewDistance;
}

// igdeCamera::MetaProperties::EnableHDRR
igdeCamera::MetaProperties::EnableHDRR::EnableHDRR() :
TBase("igde.camera.enableHDRR", "Igde.WPCamera.Action.EnableHDRR"){
	SetDefaultValue(true);
}

igdeCamera::MetaProperties::EnableHDRR::~EnableHDRR() = default;

igdeMetaPropertyBooleanStorage::Storage &igdeCamera::MetaProperties::EnableHDRR::GetStorage(
const igdeMetaContext::Ref &context) const{
	return Camera(context).enableHDRR;
}

// igdeCamera::MetaProperties::EnableGI
igdeCamera::MetaProperties::EnableGI::EnableGI() :
TBase("igde.camera.enableGI", "Igde.WPCamera.Action.EnableGI"){
	SetDefaultValue(true);
}

igdeCamera::MetaProperties::EnableGI::~EnableGI() = default;

igdeMetaPropertyBooleanStorage::Storage &igdeCamera::MetaProperties::EnableGI::GetStorage(
const igdeMetaContext::Ref &context) const{
	return Camera(context).enableGI;
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
	return Camera(context).exposure;
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
	return Camera(context).lowestIntensity;
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
	return Camera(context).highestIntensity;
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
	return Camera(context).adaptionTime;
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
	return Camera(context).whiteIntensity;
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
	return Camera(context).bloomIntensity;
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
	return Camera(context).bloomStrength;
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
	return Camera(context).bloomBlend;
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
	return Camera(context).bloomSize;
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
	return Camera(context).toneMapCurve;
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
pMetaContext(MetaContext::Ref::New(this)),
pName("Camera"),
pEngine(engine),
position(MetaProperties::global.position, pMetaContext),
rotation(MetaProperties::global.rotation, pMetaContext),
distance(MetaProperties::global.distance, pMetaContext),
fov(MetaProperties::global.fov, pMetaContext),
fovRatio(MetaProperties::global.fovRatio, pMetaContext),
imageDistance(MetaProperties::global.imageDistance, pMetaContext),
viewDistance(MetaProperties::global.viewDistance, pMetaContext),
enableHDRR(MetaProperties::global.enableHDRR, pMetaContext),
enableGI(MetaProperties::global.enableGI, pMetaContext),
exposure(MetaProperties::global.exposure, pMetaContext),
lowestIntensity(MetaProperties::global.lowestIntensity, pMetaContext),
highestIntensity(MetaProperties::global.highestIntensity, pMetaContext),
adaptionTime(MetaProperties::global.adaptionTime, pMetaContext),
whiteIntensity(MetaProperties::global.whiteIntensity, pMetaContext),
bloomIntensity(MetaProperties::global.bloomIntensity, pMetaContext),
bloomStrength(MetaProperties::global.bloomStrength, pMetaContext),
bloomBlend(MetaProperties::global.bloomBlend, pMetaContext),
bloomSize(MetaProperties::global.bloomSize, pMetaContext),
toneMapCurve(MetaProperties::global.toneMapCurve, pMetaContext)
{
	DEASSERT_NOTNULL(engine)
	
	pEngCamera = engine->GetCameraManager()->CreateCamera();
	
	fov.SetValue(pEngCamera->GetFov() * RAD2DEG, false);
	fovRatio.SetValue(pEngCamera->GetFovRatio(), false);
	imageDistance.SetValue(pEngCamera->GetImageDistance(), false);
	viewDistance.SetValue(pEngCamera->GetViewDistance(), false);
	
	pEngCamera->SetEnableGI(enableGI);
	pEngCamera->SetEnableHDRR(enableHDRR);
	
	exposure.SetValue(pEngCamera->GetExposure(), false);
	lowestIntensity.SetValue(pEngCamera->GetLowestIntensity(), false);
	highestIntensity.SetValue(pEngCamera->GetHighestIntensity(), false);
	adaptionTime.SetValue(pEngCamera->GetAdaptionTime(), false);
	
	whiteIntensity.SetValue(pEngCamera->GetWhiteIntensity(), false);
	bloomIntensity.SetValue(pEngCamera->GetBloomIntensity(), false);
	bloomStrength.SetValue(pEngCamera->GetBloomStrength(), false);
	bloomBlend.SetValue(pEngCamera->GetBloomBlend(), false);
	bloomSize.SetValue(pEngCamera->GetBloomSize(), false);
	toneMapCurve.SetValue(pEngCamera->GetToneMapCurve(), false);
	
	position.onValueChanged = [this](){
		pUpdateViewMatrix();
		pUpdateCameraPosition();
		GeometryChanged();
		onChanged();
	};
	rotation.onValueChanged = position.onValueChanged;
	
	distance.onValueChanged = [this](){
		pUpdateViewMatrix();
		pUpdateCameraPosition();
		onChanged();
	};
	
	fov.onValueChanged = [this](){
		pEngCamera->SetFov(fov * DEG2RAD);
		GeometryChanged();
		onChanged();
	};
	
	fovRatio.onValueChanged = [this](){
		pEngCamera->SetFovRatio(fovRatio);
		GeometryChanged();
		onChanged();
	};
	
	imageDistance.onValueChanged = [this](){
		pEngCamera->SetImageDistance(imageDistance);
		GeometryChanged();
		onChanged();
	};
	
	viewDistance.onValueChanged = [this](){
		pEngCamera->SetViewDistance(viewDistance);
		GeometryChanged();
		onChanged();
	};
	
	enableHDRR.onValueChanged = [this](){
		pEngCamera->SetEnableHDRR(enableHDRR);
		AdaptionChanged();
		onChanged();
	};
	
	enableGI.onValueChanged = [this](){
		pEngCamera->SetEnableGI(enableGI);
		AdaptionChanged();
		onChanged();
	};
	
	exposure.onValueChanged = [this](){
		pEngCamera->SetExposure(exposure);
		AdaptionChanged();
		onChanged();
	};
	
	lowestIntensity.onValueChanged = [this](){
		pEngCamera->SetLowestIntensity(lowestIntensity);
		AdaptionChanged();
		onChanged();
	};
	
	highestIntensity.onValueChanged = [this](){
		pEngCamera->SetHighestIntensity(highestIntensity);
		AdaptionChanged();
		onChanged();
	};
	
	adaptionTime.onValueChanged = [this](){
		pEngCamera->SetAdaptionTime(adaptionTime);
		AdaptionChanged();
		onChanged();
	};
	
	whiteIntensity.onValueChanged = [this](){
		pEngCamera->SetWhiteIntensity(whiteIntensity);
		AdaptionChanged();
		onChanged();
	};
	
	bloomIntensity.onValueChanged = [this](){
		pEngCamera->SetBloomIntensity(bloomIntensity);
		AdaptionChanged();
		onChanged();
	};
	
	bloomStrength.onValueChanged = [this](){
		pEngCamera->SetBloomStrength(bloomStrength);
		AdaptionChanged();
		onChanged();
	};
	
	bloomBlend.onValueChanged = [this](){
		pEngCamera->SetBloomBlend(bloomBlend);
		AdaptionChanged();
		onChanged();
	};
	
	bloomSize.onValueChanged = [this](){
		pEngCamera->SetBloomSize(bloomSize);
		AdaptionChanged();
		onChanged();
	};
	
	toneMapCurve.onValueChanged = [this](){
		pEngCamera->SetToneMapCurve(toneMapCurve);
		AdaptionChanged();
		onChanged();
	};
}

igdeCamera::~igdeCamera(){
	pCleanUp();
}



// Management
///////////////

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
	position = value;
}

void igdeCamera::SetOrientation(const decVector &value){
	rotation = value;
}

void igdeCamera::SetFov(float value){
	fov = value;
}

void igdeCamera::SetFovRatio(float value){
	fovRatio = value;
}

void igdeCamera::SetImageDistance(float value){
	imageDistance = value;
}

void igdeCamera::SetViewDistance(float value){
	viewDistance = value;
}

void igdeCamera::SetEnableHDRR(bool value){
	enableHDRR = value;
}

void igdeCamera::SetExposure(float value){
	exposure = value;
}

void igdeCamera::SetLowestIntensity(float value){
	lowestIntensity = value;
}

void igdeCamera::SetHighestIntensity(float value){
	highestIntensity = value;
}

void igdeCamera::SetAdaptionTime(float value){
	adaptionTime = value;
}

void igdeCamera::SetEnableGI(bool value){
	enableGI = value;
}

void igdeCamera::SetWhiteIntensity(float value){
	whiteIntensity = value;
}

void igdeCamera::SetBloomIntensity(float value){
	bloomIntensity = value;
}

void igdeCamera::SetBloomStrength(float value){
	bloomStrength = value;
}

void igdeCamera::SetBloomBlend(float value){
	bloomBlend = value;
}

void igdeCamera::SetBloomSize(float value){
	bloomSize = value;
}

void igdeCamera::SetToneMapCurve(const decCurveBezier &curve){
	toneMapCurve = curve;
}

void igdeCamera::SetDistance(float value){
	distance = value;
}

void igdeCamera::Reset(){
	SetPosition(decDVector());
}

decVector igdeCamera::GetDirectionFor(int width, int height, int x, int y) const{
	float aspectRatio = (float)width / (float)height;
	int halfHeight = height / 2;
	int halfWidth = width / 2;
	decVector direction;
	
	direction.x = tanf(fov * DEG2RAD * 0.5f) * ((float)(x - halfWidth) / (float)halfWidth);
	direction.y = tanf(fov * DEG2RAD * fovRatio * 0.5f)
		* ( ( float )( halfHeight - y ) / ( float )halfHeight ) / aspectRatio;
	direction.z = 1.0f;
	
	direction.Normalize();
	return pViewMatrix.TransformNormal(direction).ToVector();
}

void igdeCamera::SetDefaultParameters(float alowestIntensity, float ahighestIntensity, float aadaptionTime){
	const deCamera::Ref camera(pEngine->GetCameraManager()->CreateCamera());
	
	fov.SetValue(camera->GetFov() * RAD2DEG, false);
	fovRatio.SetValue(camera->GetFovRatio(), false);
	imageDistance.SetValue(camera->GetImageDistance(), false);
	viewDistance.SetValue(camera->GetViewDistance(), false);
	
	enableGI.SetValue(true, false);
	enableHDRR.SetValue(true, false);
	exposure.SetValue(camera->GetExposure(), false);
	lowestIntensity.SetValue(alowestIntensity, false);
	highestIntensity.SetValue(ahighestIntensity, false);
	adaptionTime.SetValue(aadaptionTime, false);
	
	whiteIntensity.SetValue(camera->GetWhiteIntensity(), false);
	bloomIntensity.SetValue(camera->GetBloomIntensity(), false);
	bloomStrength.SetValue(camera->GetBloomStrength(), false);
	bloomBlend.SetValue(camera->GetBloomBlend(), false);
	bloomSize.SetValue(camera->GetBloomSize(), false);
	toneMapCurve.SetValue(camera->GetToneMapCurve(), false);
	
	pEngCamera->SetFov(fov * DEG2RAD);
	pEngCamera->SetFovRatio(fovRatio);
	pEngCamera->SetImageDistance(imageDistance);
	pEngCamera->SetViewDistance(viewDistance);
	pEngCamera->SetEnableHDRR(enableHDRR);
	pEngCamera->SetExposure(exposure);
	pEngCamera->SetLowestIntensity(lowestIntensity);
	pEngCamera->SetHighestIntensity(highestIntensity);
	pEngCamera->SetAdaptionTime(adaptionTime);
	pEngCamera->SetEnableGI(enableGI);
	pEngCamera->SetWhiteIntensity(whiteIntensity);
	pEngCamera->SetBloomIntensity(bloomIntensity);
	pEngCamera->SetBloomStrength(bloomStrength);
	pEngCamera->SetBloomBlend(bloomBlend);
	pEngCamera->SetBloomSize(bloomSize);
	pEngCamera->SetToneMapCurve(toneMapCurve);
	
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
	pViewMatrix = decDMatrix::CreateTranslation(0.0, 0.0, -(double)distance)
		* decDMatrix::CreateWorld(position, rotation);
}
