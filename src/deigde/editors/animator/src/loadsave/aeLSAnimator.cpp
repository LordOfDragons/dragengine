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

#include "aeLSAnimator.h"
#include "aeLoadSaveSystem.h"
#include "../animator/aeAnimator.h"
#include "../animator/controller/aeController.h"
#include "../animator/controller/aeControllerTarget.h"
#include "../animator/link/aeLink.h"
#include "../animator/locomotion/aeAnimatorLocomotion.h"
#include "../animator/locomotion/aeAnimatorLocomotionLeg.h"
#include "../animator/rule/aeRuleAnimation.h"
#include "../animator/rule/aeRuleAnimationDifference.h"
#include "../animator/rule/aeRuleAnimationSelect.h"
#include "../animator/rule/aeRuleBoneTransformator.h"
#include "../animator/rule/aeRuleStateManipulator.h"
#include "../animator/rule/aeRuleStateSnapshot.h"
#include "../animator/rule/aeRuleInverseKinematic.h"
#include "../animator/rule/aeRuleForeignState.h"
#include "../animator/rule/aeRuleSubAnimator.h"
#include "../animator/rule/aeRuleTrackTo.h"
#include "../animator/rule/aeRuleLimit.h"
#include "../animator/rule/aeRuleGroup.h"
#include "../animator/rule/aeRuleMirror.h"
#include "../gui/aeWindowMain.h"

#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/curve/decCurveBezierPoint.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationManager.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/controller/deAnimatorControllerTarget.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorManager.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleBoneTransformator.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleForeignState.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleTrackTo.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleMirror.h>



// Definitions
////////////////

#define LOGSOURCE "Animator Editor"



// Class aeLSAnimator
///////////////////////

// Constructor, destructor
////////////////////////////

aeLSAnimator::aeLSAnimator(aeLoadSaveSystem *lssys) :
igdeBaseXML(lssys->GetWindowMain()->GetLogger(), "aeLSAnimator")
{
	pLSSys = lssys;
	
	pName = "Drag[en]gine XML Animator";
	pPattern = "*.deanimator";
}

aeLSAnimator::~aeLSAnimator(){
}



// Management
///////////////

void aeLSAnimator::SetName(const char *name){
	if(!name) DETHROW(deeInvalidParam);
	
	pName = name;
}

void aeLSAnimator::SetPattern(const char *pattern){
	if(!pattern) DETHROW(deeInvalidParam);
	
	pPattern = pattern;
}



// Loading and saving
///////////////////////

void aeLSAnimator::LoadAnimator(aeAnimator *animator, decBaseFileReader *file){
	if(!animator || !file){
		DETHROW(deeInvalidParam);
	}
	
	decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::New());
	
	decXmlParser(pLSSys->GetWindowMain()->GetEnvironment().GetLogger()).ParseXml(file, xmlDoc);
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if(!root){
		DETHROW(deeInvalidParam);
	}
	if(strcmp(root->GetName(), "animator") != 0){
		DETHROW(deeInvalidParam);
	}
	
	pLoadAnimator(root, *animator);
}

void aeLSAnimator::SaveAnimator(aeAnimator *animator, decBaseFileWriter *file){
	if(!animator || !file) DETHROW(deeInvalidParam);
	int i, count;
	
	decXmlWriter writer(file);
	
	// write header
	writer.WriteXMLDeclaration();
	writer.WriteOpeningTag("animator", false, true);
	
	// display section
	pSaveDisplay(writer, *animator);
	
	// animator properties
	writer.WriteDataTagString("rig", animator->GetRigPath());
	writer.WriteDataTagString("animation", animator->GetAnimationPath());
	
	// write bones
	const decStringSet &boneList = animator->GetListBones();
	
	count = boneList.GetCount();
	for(i=0; i<count; i++){
		writer.WriteDataTagString("bone", boneList.GetAt(i));
	}
	
	// write vertex position sets
	const decStringSet &vpsList = animator->GetListVertexPositionSets();
	
	count = vpsList.GetCount();
	for(i=0; i<count; i++){
		writer.WriteDataTagString("vertexPositionSet", vpsList.GetAt(i));
	}
	
	// write controllers
	count = animator->GetControllers().GetCount();
	for(i=0; i<count; i++){
		writer.WriteNewline();
		pSaveController(writer, *animator->GetControllers().GetAt(i));
	}
	
	// write links
	count = animator->GetLinks().GetCount();
	for(i=0; i<count; i++){
		writer.WriteNewline();
		pSaveLink(writer, *animator, *animator->GetLinks().GetAt(i));
	}
	
	// write rules
	count = animator->GetRules().GetCount();
	for(i=0; i<count; i++){
		writer.WriteNewline();
		pSaveRule(writer, *animator, *animator->GetRules().GetAt(i));
	}
	
	// write footer
	writer.WriteClosingTag("animator", true);
}



// Private functions
//////////////////////

void aeLSAnimator::pSaveDisplay(decXmlWriter &writer, const aeAnimator &animator){
	bool saveExplainingComments = false;
	
	if(saveExplainingComments){
		writer.WriteComment("Contains optional display information only relevant to the animator editor.");
	}
	writer.WriteOpeningTag("display");
	
	if(saveExplainingComments) writer.WriteComment("Path to model to use.");
	writer.WriteDataTagString("model", animator.GetDisplayModelPath());
	
	if(saveExplainingComments) writer.WriteComment("Path to skin to use.");
	writer.WriteDataTagString("skin", animator.GetDisplaySkinPath());
	
	if(saveExplainingComments) writer.WriteComment("Path to rig to use.");
	writer.WriteDataTagString("rig", animator.GetDisplayRigPath());
	
//	if( saveExplainingComments ) writer.WriteComment( "Path to animation to use." );
//	writer.WriteDataTagString( "animation", animator.GetAnimationPath() );
	
	pSaveLocomotion(writer, animator);
	
	writer.WriteClosingTag("display");
}

void aeLSAnimator::pSaveLocomotion(decXmlWriter &writer, const aeAnimator &animator){
	const aeAnimatorLocomotion &locomotion = animator.GetLocomotion();
	int l, legCount = locomotion.GetLegCount();
	
	writer.WriteOpeningTag("locomotion");
	
	switch(locomotion.GetLocomotionType()){
		case aeAnimatorLocomotion::eltNatural:
			//writer.WriteDataTagString( "locomotionType", "natural" );
			break;
			
		case aeAnimatorLocomotion::eltFPS:
			writer.WriteDataTagString("locomotionType", "fps");
			break;
			
		case aeAnimatorLocomotion::eltVehicle:
			writer.WriteDataTagString("locomotionType", "vehicle");
			break;
		
		default:
			break;
	}
	
	if(fabsf(locomotion.GetLimitLookDown() - -90.0f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("limitLookDown", locomotion.GetLimitLookDown());
	}
	if(fabsf(locomotion.GetLimitLookUp() - 90.0f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("limitLookUp", locomotion.GetLimitLookUp());
	}
	if(fabsf(locomotion.GetLimitLookLeft() - -90.0f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("limitLookLeft", locomotion.GetLimitLookLeft());
	}
	if(fabsf(locomotion.GetLimitLookRight() - 90.0f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("limitLookRight", locomotion.GetLimitLookRight());
	}
	
	if(fabsf(locomotion.GetLookUpDown().GetAdjustTime()) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("adjustTimeUpDown", locomotion.GetLookUpDown().GetAdjustTime());
	}
	if(fabsf(locomotion.GetLookLeftRight().GetAdjustTime()) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("adjustTimeLeftRight", locomotion.GetLookLeftRight().GetAdjustTime());
	}
	if(fabsf(locomotion.GetStance().GetAdjustTime() - 0.5f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("adjustTimeStance", locomotion.GetStance().GetAdjustTime());
	}
	if(fabsf(locomotion.GetAdjustTimeTurnIP() - 1.0f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("adjustTimeTurnIP", locomotion.GetAdjustTimeTurnIP());
	}
	if(fabsf(locomotion.GetOrientation().GetAdjustTime() - 1.0f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("adjustTimeOrientation", locomotion.GetOrientation().GetAdjustTime());
	}
	if(fabsf(locomotion.GetLinearVelocity().GetAdjustTime() - 0.5f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("adjustTimeVelocity", locomotion.GetLinearVelocity().GetAdjustTime());
	}
	
	if(fabsf(locomotion.GetWalkSpeed() - 1.25f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("speedWalk", locomotion.GetWalkSpeed());
	}
	if(fabsf(locomotion.GetRunSpeed() - 4.0f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("speedRun", locomotion.GetRunSpeed());
	}
	
	if(fabsf(locomotion.GetLegBlendTime()) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("legBlendTime", locomotion.GetLegBlendTime());
	}
	if(locomotion.GetUseLegPairCount() != 1){
		writer.WriteDataTagInt("useLegPairs", locomotion.GetUseLegPairCount());
	}
	
	for(l=0; l<legCount; l++){
		const aeAnimatorLocomotionLeg &leg = *locomotion.GetLegAt(l);
		const decVector &pdposStand = leg.GetPutDownPositionStand();
		const decVector &pdposWalk = leg.GetPutDownPositionWalk();
		const decVector &pdposRun = leg.GetPutDownPositionRun();
		
		const bool hasLiftOffTime = fabsf(leg.GetLiftOffTime()) > FLOAT_SAFE_EPSILON;
		const bool hasPutDownTime = fabsf(leg.GetPutDownTime() - 0.5f) > FLOAT_SAFE_EPSILON;
		const bool hasPdposStand = !pdposStand.IsZero();
		const bool hasPdposWalk = !pdposWalk.IsZero();
		const bool hasPdposRun = !pdposRun.IsZero();
		
		if(!hasLiftOffTime && !hasPutDownTime && !hasPdposStand && !hasPdposWalk && !hasPdposRun){
			continue;
		}
		
		writer.WriteOpeningTag("leg");
		
		//writer.WriteDataTagString( "name", leg.GetName() );
		if(hasLiftOffTime){
			writer.WriteDataTagFloat("liftOffTime", leg.GetLiftOffTime());
		}
		if(hasPutDownTime){
			writer.WriteDataTagFloat("putDownTime", leg.GetPutDownTime());
		}
		
		if(hasPdposStand){
			writer.WriteOpeningTagStart("putDownPositionStand");
			writer.WriteAttributeFloat("x", pdposStand.x);
			writer.WriteAttributeFloat("y", pdposStand.y);
			writer.WriteAttributeFloat("z", pdposStand.z);
			writer.WriteOpeningTagEnd(true);
		}
		
		if(hasPdposWalk){
			writer.WriteOpeningTagStart("putDownPositionWalk");
			writer.WriteAttributeFloat("x", pdposWalk.x);
			writer.WriteAttributeFloat("y", pdposWalk.y);
			writer.WriteAttributeFloat("z", pdposWalk.z);
			writer.WriteOpeningTagEnd(true);
		}
		
		if(hasPdposRun){
			writer.WriteOpeningTagStart("putDownPositionRun");
			writer.WriteAttributeFloat("x", pdposRun.x);
			writer.WriteAttributeFloat("y", pdposRun.y);
			writer.WriteAttributeFloat("z", pdposRun.z);
			writer.WriteOpeningTagEnd(true);
		}
		
		writer.WriteClosingTag("leg");
	}
	
	writer.WriteClosingTag("locomotion");
}

void aeLSAnimator::pSaveController(decXmlWriter &writer, const aeController &controller){
	writer.WriteOpeningTag("controller");
	
	writer.WriteDataTagString("name", controller.GetName());
	
	writer.WriteOpeningTagStart("limits");
	writer.WriteAttributeFloat("min", controller.GetMinimumValue());
	writer.WriteAttributeFloat("max", controller.GetMaximumValue());
	writer.WriteOpeningTagEnd(true);
	
	if(controller.GetFrozen()){
		writer.WriteDataTagBool("frozen", controller.GetFrozen());
	}
	if(!controller.GetClamp()){
		writer.WriteDataTagBool("clamp", controller.GetClamp());
	}
	
	switch(controller.GetLocomotionAttribute()){
	case aeAnimatorLocomotion::eaElapsedTime:
		writer.WriteDataTagString("locomotionAttribute", "elapsedTime");
		break;
		
	case aeAnimatorLocomotion::eaLookUpDown:
		writer.WriteDataTagString("locomotionAttribute", "lookUpDown");
		break;
		
	case aeAnimatorLocomotion::eaLookLeftRight:
		writer.WriteDataTagString("locomotionAttribute", "lookLeftRight");
		break;
		
	case aeAnimatorLocomotion::eaMovingSpeed:
		writer.WriteDataTagString("locomotionAttribute", "movingSpeed");
		break;
		
	case aeAnimatorLocomotion::eaMovingDirection:
		writer.WriteDataTagString("locomotionAttribute", "movingDirection");
		break;
		
	case aeAnimatorLocomotion::eaRelativeMovingSpeed:
		writer.WriteDataTagString("locomotionAttribute", "relativeMovingSpeed");
		break;
		
	case aeAnimatorLocomotion::eaTurningSpeed:
		writer.WriteDataTagString("locomotionAttribute", "turningSpeed");
		break;
		
	case aeAnimatorLocomotion::eaStance:
		writer.WriteDataTagString("locomotionAttribute", "stance");
		break;
		
	case aeAnimatorLocomotion::eaDisplacement:
		writer.WriteDataTagString("locomotionAttribute", "displacement");
		break;
		
	case aeAnimatorLocomotion::eaRelativeDisplacement:
		writer.WriteDataTagString("locomotionAttribute", "relativeDisplacement");
		break;
		
	case aeAnimatorLocomotion::eaBodyTiltOffset:
		writer.WriteDataTagString("locomotionAttribute", "bodyTiltOffset");
		break;
		
	case aeAnimatorLocomotion::eaBodyTiltUpDown:
		writer.WriteDataTagString("locomotionAttribute", "bodyTiltUpDown");
		break;
		
	case aeAnimatorLocomotion::eaBodyTiltRightLeft:
		writer.WriteDataTagString("locomotionAttribute", "bodyTiltLeftRight");
		break;
		
	case aeAnimatorLocomotion::eaTimeTurnIP:
		writer.WriteDataTagString("locomotionAttribute", "timeTurnIP");
		break;
		
	case aeAnimatorLocomotion::eaLegGroundPosition:
		writer.WriteDataTagString("locomotionAttribute", "legGroundPosition");
		break;
		
	case aeAnimatorLocomotion::eaLegGroundNormal:
		writer.WriteDataTagString("locomotionAttribute", "legGroundNormal");
		break;
		
	case aeAnimatorLocomotion::eaLegInfluence:
		writer.WriteDataTagString("locomotionAttribute", "legInfluence");
		break;
		
	case aeAnimatorLocomotion::eaLegPosition:
		writer.WriteDataTagString("locomotionAttribute", "legPosition");
		break;
		
	case aeAnimatorLocomotion::eaLegOrientation:
		writer.WriteDataTagString("locomotionAttribute", "legOrientation");
		break;
	}
	
	if(controller.GetLocomotionLeg() > 0){
		writer.WriteDataTagInt("locomotionLeg", controller.GetLocomotionLeg());
	}
	
	switch(controller.GetVectorSimulation()){
	case aeController::evsPosition:
		writer.WriteDataTagString("vectorSimulation", "position");
		break;
		
	case aeController::evsRotation:
		writer.WriteDataTagString("vectorSimulation", "rotation");
		break;
		
	case aeController::evsNone:
		break;
	}
	
	if(fabsf(controller.GetDefaultValue()) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("value", controller.GetDefaultValue());
	}
	if(!controller.GetDefaultVector().IsZero()){
		WriteVector(writer, "vector", controller.GetDefaultVector());
	}
	
	writer.WriteClosingTag("controller");
}

void aeLSAnimator::pSaveLink(decXmlWriter &writer, const aeAnimator &animator, const aeLink &link){
	int controllerIndex = -1;
	
	if(link.GetController()){
		controllerIndex = animator.GetControllers().IndexOf(link.GetController());
	}
	
	writer.WriteOpeningTag("link");
	
	writer.WriteDataTagString("name", link.GetName().GetString());
	writer.WriteDataTagInt("controller", controllerIndex);
	if(link.GetRepeat() != 1){
		writer.WriteDataTagInt("repeat", link.GetRepeat());
	}
	if(!link.GetBone().IsEmpty()){
		writer.WriteDataTagString("bone", link.GetBone());
	}
	
	switch(link.GetBoneParameter()){
	case deAnimatorLink::ebpPositionX:
		writer.WriteDataTagString("boneParameter", "positionX");
		break;
		
	case deAnimatorLink::ebpPositionY:
		writer.WriteDataTagString("boneParameter", "positionY");
		break;
		
	case deAnimatorLink::ebpPositionZ:
		//writer.WriteDataTagString( "boneParameter", "positionZ" ); // default
		break;
		
	case deAnimatorLink::ebpRotationX:
		writer.WriteDataTagString("boneParameter", "rotationX");
		break;
		
	case deAnimatorLink::ebpRotationY:
		writer.WriteDataTagString("boneParameter", "rotationY");
		break;
		
	case deAnimatorLink::ebpRotationZ:
		writer.WriteDataTagString("boneParameter", "rotationZ");
		break;
		
	case deAnimatorLink::ebpScaleX:
		writer.WriteDataTagString("boneParameter", "scaleX");
		break;
		
	case deAnimatorLink::ebpScaleY:
		writer.WriteDataTagString("boneParameter", "scaleY");
		break;
		
	case deAnimatorLink::ebpScaleZ:
		writer.WriteDataTagString("boneParameter", "scaleZ");
		break;
	}
	
	if(fabsf(link.GetBoneMinimum()) > FLOAT_SAFE_EPSILON
	|| fabsf(link.GetBoneMaximum() - 1.0f) > FLOAT_SAFE_EPSILON){
		writer.WriteOpeningTagStart("boneLimits");
		writer.WriteAttributeFloat("min", link.GetBoneMinimum());
		writer.WriteAttributeFloat("max", link.GetBoneMaximum());
		writer.WriteOpeningTagEnd(true);
	}
	
	if(!link.GetVertexPositionSet().IsEmpty()){
		writer.WriteDataTagString("vertexPositionSet", link.GetVertexPositionSet());
	}
	
	if(fabsf(link.GetVertexPositionSetMinimum()) > FLOAT_SAFE_EPSILON
	|| fabsf(link.GetVertexPositionSetMaximum() - 1.0f) > FLOAT_SAFE_EPSILON){
		writer.WriteOpeningTagStart("vertexPositionSetLimits");
		writer.WriteAttributeFloat("min", link.GetVertexPositionSetMinimum());
		writer.WriteAttributeFloat("max", link.GetVertexPositionSetMaximum());
		writer.WriteOpeningTagEnd(true);
	}
	
	if(link.GetWrapY()){
		writer.WriteDataTagBool("wrapY", link.GetWrapY());
	}
	
	const decCurveBezier &curve = link.GetCurve();
	if(curve.GetInterpolationMode() != decCurveBezier::eimLinear || curve.GetPointCount() != 2
	|| !curve.GetPointAt(0).GetPoint().IsEqualTo(decVector2(0.0f, 0.0f))
	|| !curve.GetPointAt(1).GetPoint().IsEqualTo(decVector2(1.0f, 1.0f))){
		WriteCurveBezier(writer, "curve", link.GetCurve());
	}
	
	writer.WriteClosingTag("link");
}

void aeLSAnimator::pSaveRuleCommon(decXmlWriter &writer, const aeAnimator &animator, const aeRule &rule){
	int i, count;
	
	writer.WriteDataTagString("name", rule.GetName());
	if(!rule.GetEnabled()){
		writer.WriteDataTagBool("enabled", rule.GetEnabled());
	}
	
	if(rule.GetBlendMode() == deAnimatorRule::ebmBlend){
		//writer.WriteDataTagString( "blendMode", "blend" ); // default value
		
	}else if(rule.GetBlendMode() == deAnimatorRule::ebmOverlay){
		writer.WriteDataTagString("blendMode", "overlay");
		
	}else{
		DETHROW(deeInvalidParam);
	}
	
	if(fabsf(rule.GetBlendFactor() - 1.0f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("blendFactor", rule.GetBlendFactor());
	}
	if(rule.GetInvertBlendFactor()){
		writer.WriteDataTagBool("invertBlendFactor", rule.GetInvertBlendFactor());
	}
	
	pSaveControllerTarget(writer, animator, rule.GetTargetBlendFactor(), "blendFactor");
	
	const decStringSet &boneList = rule.GetListBones();
	count = boneList.GetCount();
	for(i=0; i<count; i++){
		writer.WriteDataTagString("bone", boneList.GetAt(i));
	}
	
	const decStringSet &vpsList = rule.GetListVertexPositionSets();
	count = vpsList.GetCount();
	for(i=0; i<count; i++){
		writer.WriteDataTagString("vertexPositionSet", vpsList.GetAt(i));
	}
}

void aeLSAnimator::pSaveRule(decXmlWriter &writer, const aeAnimator &animator, const aeRule &rule){
	if(rule.GetType() == deAnimatorRuleVisitorIdentify::ertAnimation){
		pSaveRuleAnimation(writer, animator, (const aeRuleAnimation &)rule);
		
	}else if(rule.GetType() == deAnimatorRuleVisitorIdentify::ertAnimationDifference){
		pSaveRuleAnimationDifference(writer, animator, (const aeRuleAnimationDifference &)rule);
		
	}else if(rule.GetType() == deAnimatorRuleVisitorIdentify::ertAnimationSelect){
		pSaveRuleAnimationSelect(writer, animator, (const aeRuleAnimationSelect &)rule);
		
	}else if(rule.GetType() == deAnimatorRuleVisitorIdentify::ertBoneTransformator){
		pSaveRuleBoneTransformator(writer, animator, (const aeRuleBoneTransformator &)rule);
		
	}else if(rule.GetType() == deAnimatorRuleVisitorIdentify::ertStateManipulator){
		pSaveRuleStateManipulator(writer, animator, (const aeRuleStateManipulator &)rule);
		
	}else if(rule.GetType() == deAnimatorRuleVisitorIdentify::ertStateSnapshot){
		pSaveRuleStateSnapshot(writer, animator, (const aeRuleStateSnapshot &)rule);
		
	}else if(rule.GetType() == deAnimatorRuleVisitorIdentify::ertInverseKinematic){
		pSaveRuleInverseKinematic(writer, animator, (const aeRuleInverseKinematic &)rule);
		
	}else if(rule.GetType() == deAnimatorRuleVisitorIdentify::ertForeignState){
		pSaveRuleForeignState(writer, animator, (const aeRuleForeignState &)rule);
		
	}else if(rule.GetType() == deAnimatorRuleVisitorIdentify::ertGroup){
		pSaveRuleGroup(writer, animator, (const aeRuleGroup &)rule);
		
	}else if(rule.GetType() == deAnimatorRuleVisitorIdentify::ertSubAnimator){
		pSaveRuleSubAnimator(writer, animator, (const aeRuleSubAnimator &)rule);
		
	}else if(rule.GetType() == deAnimatorRuleVisitorIdentify::ertTrackTo){
		pSaveRuleTrackTo(writer, animator, (const aeRuleTrackTo &)rule);
		
	}else if(rule.GetType() == deAnimatorRuleVisitorIdentify::ertLimit){
		pSaveRuleLimit(writer, animator, (const aeRuleLimit &)rule);
		
	}else if(rule.GetType() == deAnimatorRuleVisitorIdentify::ertMirror){
		pSaveRuleMirror(writer, animator, (const aeRuleMirror &)rule);
	}
}

void aeLSAnimator::pSaveControllerTarget(decXmlWriter &writer, const aeAnimator &animator,
const aeControllerTarget &target, const char *name){
	const int linkCount = target.GetLinkCount();
	if(linkCount == 0){
		return;
	}
	
	int l, linkIndex;
	aeLink *link;
	
	writer.WriteOpeningTagStart("target");
	writer.WriteAttributeString("name", name);
	writer.WriteOpeningTagEnd();
	
	for(l=0; l<linkCount; l++){
		link = target.GetLinkAt(l);
		
		linkIndex = animator.GetLinks().IndexOf(link);
		
		writer.WriteDataTagInt("link", linkIndex);
	}
	
	writer.WriteClosingTag("target");
}

void aeLSAnimator::pSaveRuleAnimation(decXmlWriter &writer, const aeAnimator &animator,
const aeRuleAnimation &rule){
	writer.WriteOpeningTag("ruleAnimation");
	
	pSaveRuleCommon(writer, animator, rule);
	
	writer.WriteDataTagString("moveName", rule.GetMoveName());
	writer.WriteDataTagFloat("moveTime", rule.GetMoveTime());
	
	if(!rule.GetEnablePosition()){
		writer.WriteDataTagBool("enablePosition", rule.GetEnablePosition());
	}
	if(!rule.GetEnableOrientation()){
		writer.WriteDataTagBool("enableOrientation", rule.GetEnableOrientation());
	}
	if(rule.GetEnableSize()){
		writer.WriteDataTagBool("enableSize", rule.GetEnableSize());
	}
	if(!rule.GetEnableVertexPositionSet()){
		writer.WriteDataTagBool("enableVertexPositionSet", rule.GetEnableVertexPositionSet());
	}
	
	pSaveControllerTarget(writer, animator, rule.GetTargetMoveTime(), "moveTime");
	
	writer.WriteClosingTag("ruleAnimation");
}

void aeLSAnimator::pSaveRuleAnimationDifference(decXmlWriter &writer, const aeAnimator &animator,
const aeRuleAnimationDifference &rule){
	writer.WriteOpeningTag("ruleAnimationDifference");
	
	pSaveRuleCommon(writer, animator, rule);
	
	writer.WriteDataTagString("leadingMoveName", rule.GetLeadingMoveName());
	writer.WriteDataTagFloat("leadingMoveTime", rule.GetLeadingMoveTime());
	writer.WriteDataTagString("referenceMoveName", rule.GetReferenceMoveName());
	writer.WriteDataTagFloat("referenceMoveTime", rule.GetReferenceMoveTime());
	
	if(!rule.GetEnablePosition()){
		writer.WriteDataTagBool("enablePosition", rule.GetEnablePosition());
	}
	if(!rule.GetEnableOrientation()){
		writer.WriteDataTagBool("enableOrientation", rule.GetEnableOrientation());
	}
	if(rule.GetEnableSize()){
		writer.WriteDataTagBool("enableSize", rule.GetEnableSize());
	}
	if(!rule.GetEnableVertexPositionSet()){
		writer.WriteDataTagBool("enableVertexPositionSet", rule.GetEnableVertexPositionSet());
	}
	
	pSaveControllerTarget(writer, animator, rule.GetTargetLeadingMoveTime(), "leadingMoveTime");
	pSaveControllerTarget(writer, animator, rule.GetTargetReferenceMoveTime(), "referenceMoveTime");
	
	writer.WriteClosingTag("ruleAnimationDifference");
}

void aeLSAnimator::pSaveRuleAnimationSelect(decXmlWriter &writer, const aeAnimator &animator,
const aeRuleAnimationSelect &rule){
	writer.WriteOpeningTag("ruleAnimationSelect");
	
	pSaveRuleCommon(writer, animator, rule);
	
	const decStringList &moves = rule.GetMoves();
	const int moveCount = moves.GetCount();
	int i;
	
	for(i=0; i<moveCount; i++){
		writer.WriteDataTagString("move", moves.GetAt(i));
	}
	
	if(!rule.GetEnablePosition()){
		writer.WriteDataTagBool("enablePosition", rule.GetEnablePosition());
	}
	if(!rule.GetEnableOrientation()){
		writer.WriteDataTagBool("enableOrientation", rule.GetEnableOrientation());
	}
	if(rule.GetEnableSize()){
		writer.WriteDataTagBool("enableSize", rule.GetEnableSize());
	}
	if(!rule.GetEnableVertexPositionSet()){
		writer.WriteDataTagBool("enableVertexPositionSet", rule.GetEnableVertexPositionSet());
	}
	
	pSaveControllerTarget(writer, animator, rule.GetTargetMoveTime(), "moveTime");
	pSaveControllerTarget(writer, animator, rule.GetTargetSelect(), "selection");
	
	writer.WriteClosingTag("ruleAnimationSelect");
}

void aeLSAnimator::pSaveRuleBoneTransformator(decXmlWriter &writer, const aeAnimator &animator,
const aeRuleBoneTransformator &rule){
	writer.WriteOpeningTag("ruleBoneTransformator");
	
	pSaveRuleCommon(writer, animator, rule);
	
	if(!rule.GetMinimumTranslation().IsZero()){
		writer.WriteOpeningTagStart("minimumTranslation");
		writer.WriteAttributeFloat("x", rule.GetMinimumTranslation().x);
		writer.WriteAttributeFloat("y", rule.GetMinimumTranslation().y);
		writer.WriteAttributeFloat("z", rule.GetMinimumTranslation().z);
		writer.WriteOpeningTagEnd(true);
	}
	
	if(!rule.GetMaximumTranslation().IsZero()){
		writer.WriteOpeningTagStart("maximumTranslation");
		writer.WriteAttributeFloat("x", rule.GetMaximumTranslation().x);
		writer.WriteAttributeFloat("y", rule.GetMaximumTranslation().y);
		writer.WriteAttributeFloat("z", rule.GetMaximumTranslation().z);
		writer.WriteOpeningTagEnd(true);
	}
	
	if(!rule.GetMinimumRotation().IsZero()){
		writer.WriteOpeningTagStart("minimumRotation");
		writer.WriteAttributeFloat("x", rule.GetMinimumRotation().x);
		writer.WriteAttributeFloat("y", rule.GetMinimumRotation().y);
		writer.WriteAttributeFloat("z", rule.GetMinimumRotation().z);
		writer.WriteOpeningTagEnd(true);
	}
	
	if(!rule.GetMaximumRotation().IsZero()){
		writer.WriteOpeningTagStart("maximumRotation");
		writer.WriteAttributeFloat("x", rule.GetMaximumRotation().x);
		writer.WriteAttributeFloat("y", rule.GetMaximumRotation().y);
		writer.WriteAttributeFloat("z", rule.GetMaximumRotation().z);
		writer.WriteOpeningTagEnd(true);
	}
	
	if(!rule.GetMinimumScaling().IsEqualTo(decVector(1.0f, 1.0f, 1.0f))){
		writer.WriteOpeningTagStart("minimumScaling");
		writer.WriteAttributeFloat("x", rule.GetMinimumScaling().x);
		writer.WriteAttributeFloat("y", rule.GetMinimumScaling().y);
		writer.WriteAttributeFloat("z", rule.GetMinimumScaling().z);
		writer.WriteOpeningTagEnd(true);
	}
	
	if(!rule.GetMaximumScaling().IsEqualTo(decVector(1.0f, 1.0f, 1.0f))){
		writer.WriteOpeningTagStart("maximumScaling");
		writer.WriteAttributeFloat("x", rule.GetMaximumScaling().x);
		writer.WriteAttributeFloat("y", rule.GetMaximumScaling().y);
		writer.WriteAttributeFloat("z", rule.GetMaximumScaling().z);
		writer.WriteOpeningTagEnd(true);
	}
	
	if(!rule.GetAxis().IsEqualTo(decVector(0.0f, 0.0f, 1.0f))){
		writer.WriteOpeningTagStart("axis");
		writer.WriteAttributeFloat("x", rule.GetAxis().x);
		writer.WriteAttributeFloat("y", rule.GetAxis().y);
		writer.WriteAttributeFloat("z", rule.GetAxis().z);
		writer.WriteOpeningTagEnd(true);
	}
	
	if(fabsf(rule.GetMinimumAngle()) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("minimumAngle", rule.GetMinimumAngle());
	}
	if(fabsf(rule.GetMaximumAngle()) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("maximumAngle", rule.GetMaximumAngle());
	}
	
	switch(rule.GetCoordinateFrame()){
	case deAnimatorRuleBoneTransformator::ecfBoneLocal:
		writer.WriteDataTagString("cframe", "local");
		break;
		
	case deAnimatorRuleBoneTransformator::ecfComponent:
		writer.WriteDataTagString("cframe", "component");
		break;
		
	case deAnimatorRuleBoneTransformator::ecfTargetBone:
		writer.WriteDataTagString("cframe", "target");
		break;
	}
	
	if(rule.GetEnablePosition()){
		writer.WriteDataTagBool("enablePosition", rule.GetEnablePosition());
	}
	if(!rule.GetEnableOrientation()){
		writer.WriteDataTagBool("enableOrientation", rule.GetEnableOrientation());
	}
	if(rule.GetEnableSize()){
		writer.WriteDataTagBool("enableSize", rule.GetEnableSize());
	}
	if(rule.GetUseAxis()){
		writer.WriteDataTagBool("useAxis", rule.GetUseAxis());
	}
	if(!rule.GetTargetBone().IsEmpty()){
		writer.WriteDataTagString("targetBone", rule.GetTargetBone());
	}
	
	if(!rule.GetInputBone().IsEmpty()){
		writer.WriteDataTagString("inputBone", rule.GetInputBone());
	}
	
	switch(rule.GetInputSource()){
	case deAnimatorRuleBoneTransformator::eisTargetBlend:
		// writer.WriteDataTagString("inputSource", "targetBlend");
		break;
		
	case deAnimatorRuleBoneTransformator::eisTargetDirect:
		writer.WriteDataTagString("inputSource", "targetDirect");
		break;
		
	case deAnimatorRuleBoneTransformator::eisBoneState:
		writer.WriteDataTagString("inputSource", "boneState");
		break;
		
	case deAnimatorRuleBoneTransformator::eisBoneStateInverse:
		writer.WriteDataTagString("inputSource", "boneStateInverse");
		break;
	}
	
	pSaveControllerTarget(writer, animator, rule.GetTargetTranslation(), "translation");
	pSaveControllerTarget(writer, animator, rule.GetTargetRotation(), "rotation");
	pSaveControllerTarget(writer, animator, rule.GetTargetScaling(), "scaling");
	
	writer.WriteClosingTag("ruleBoneTransformator");
}

void aeLSAnimator::pSaveRuleStateManipulator(decXmlWriter &writer, const aeAnimator &animator,
const aeRuleStateManipulator &rule){
	writer.WriteOpeningTag("ruleStateManipulator");
	
	pSaveRuleCommon(writer, animator, rule);
	
	writer.WriteOpeningTagStart("positionMinimum");
	writer.WriteAttributeFloat("x", rule.GetMinimumPosition().x);
	writer.WriteAttributeFloat("y", rule.GetMinimumPosition().y);
	writer.WriteAttributeFloat("z", rule.GetMinimumPosition().z);
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteOpeningTagStart("positionMaximum");
	writer.WriteAttributeFloat("x", rule.GetMaximumPosition().x);
	writer.WriteAttributeFloat("y", rule.GetMaximumPosition().y);
	writer.WriteAttributeFloat("z", rule.GetMaximumPosition().z);
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteOpeningTagStart("rotationMinimum");
	writer.WriteAttributeFloat("x", rule.GetMinimumRotation().x);
	writer.WriteAttributeFloat("y", rule.GetMinimumRotation().y);
	writer.WriteAttributeFloat("z", rule.GetMinimumRotation().z);
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteOpeningTagStart("rotationMaximum");
	writer.WriteAttributeFloat("x", rule.GetMaximumRotation().x);
	writer.WriteAttributeFloat("y", rule.GetMaximumRotation().y);
	writer.WriteAttributeFloat("z", rule.GetMaximumRotation().z);
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteOpeningTagStart("sizeMinimum");
	writer.WriteAttributeFloat("x", rule.GetMinimumSize().x);
	writer.WriteAttributeFloat("y", rule.GetMinimumSize().y);
	writer.WriteAttributeFloat("z", rule.GetMinimumSize().z);
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteOpeningTagStart("sizeMaximum");
	writer.WriteAttributeFloat("x", rule.GetMaximumSize().x);
	writer.WriteAttributeFloat("y", rule.GetMaximumSize().y);
	writer.WriteAttributeFloat("z", rule.GetMaximumSize().z);
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteDataTagFloat("vertexPositionSetMinimum", rule.GetMinimumVertexPositionSet());
	writer.WriteDataTagFloat("vertexPositionSetMaximum", rule.GetMaximumVertexPositionSet());
	
	if(rule.GetEnablePosition()){
		writer.WriteDataTagBool("enablePosition", rule.GetEnablePosition());
	}
	if(!rule.GetEnableOrientation()){
		writer.WriteDataTagBool("enableOrientation", rule.GetEnableOrientation());
	}
	if(rule.GetEnableSize()){
		writer.WriteDataTagBool("enableSize", rule.GetEnableSize());
	}
	if(!rule.GetEnableVertexPositionSet()){
		writer.WriteDataTagBool("enableVertexPositionSet", rule.GetEnableVertexPositionSet());
	}
	
	pSaveControllerTarget(writer, animator, rule.GetTargetPosition(), "position");
	pSaveControllerTarget(writer, animator, rule.GetTargetRotation(), "orientation");
	pSaveControllerTarget(writer, animator, rule.GetTargetSize(), "size");
	pSaveControllerTarget(writer, animator, rule.GetTargetVertexPositionSet(), "vertexPositionSet");
	
	writer.WriteClosingTag("ruleStateManipulator");
}

void aeLSAnimator::pSaveRuleStateSnapshot(decXmlWriter &writer, const aeAnimator &animator,
const aeRuleStateSnapshot &rule){
	writer.WriteOpeningTag("ruleStateSnapshot");
	
	pSaveRuleCommon(writer, animator, rule);
	
	if(!rule.GetUseLastState()){
		writer.WriteDataTagBool("useLastState", rule.GetUseLastState());
	}
	
	if(!rule.GetEnablePosition()){
		writer.WriteDataTagBool("enablePosition", rule.GetEnablePosition());
	}
	if(!rule.GetEnableOrientation()){
		writer.WriteDataTagBool("enableOrientation", rule.GetEnableOrientation());
	}
	if(rule.GetEnableSize()){
		writer.WriteDataTagBool("enableSize", rule.GetEnableSize());
	}
	if(!rule.GetEnableVertexPositionSet()){
		writer.WriteDataTagBool("enableVertexPositionSet", rule.GetEnableVertexPositionSet());
	}
	
	writer.WriteClosingTag("ruleStateSnapshot");
}

void aeLSAnimator::pSaveRuleInverseKinematic(decXmlWriter &writer, const aeAnimator &animator,
const aeRuleInverseKinematic &rule){
	writer.WriteOpeningTag("ruleInverseKinematic");
	
	pSaveRuleCommon(writer, animator, rule);
	
	const decVector &goalPosition = rule.GetGoalPosition();
	writer.WriteOpeningTagStart("goalPosition");
	writer.WriteAttributeFloat("x", goalPosition.x);
	writer.WriteAttributeFloat("y", goalPosition.y);
	writer.WriteAttributeFloat("z", goalPosition.z);
	writer.WriteOpeningTagEnd(true);
	
	const decVector &goalOrientation = rule.GetGoalOrientation();
	writer.WriteOpeningTagStart("goalOrientation");
	writer.WriteAttributeFloat("x", goalOrientation.x);
	writer.WriteAttributeFloat("y", goalOrientation.y);
	writer.WriteAttributeFloat("z", goalOrientation.z);
	writer.WriteOpeningTagEnd(true);
	
	const decVector &localPosition = rule.GetLocalPosition();
	writer.WriteOpeningTagStart("localPosition");
	writer.WriteAttributeFloat("x", localPosition.x);
	writer.WriteAttributeFloat("y", localPosition.y);
	writer.WriteAttributeFloat("z", localPosition.z);
	writer.WriteOpeningTagEnd(true);
	
	const decVector &localOrientation = rule.GetLocalOrientation();
	writer.WriteOpeningTagStart("localOrientation");
	writer.WriteAttributeFloat("x", localOrientation.x);
	writer.WriteAttributeFloat("y", localOrientation.y);
	writer.WriteAttributeFloat("z", localOrientation.z);
	writer.WriteOpeningTagEnd(true);
	
	if(rule.GetAdjustOrientation()){
		writer.WriteDataTagBool("adjustOrientation", rule.GetAdjustOrientation());
	}
	
	if(!rule.GetSolverBone().IsEmpty()){
		writer.WriteDataTagString("solverBone", rule.GetSolverBone());
	}
	if(rule.GetUseSolverBone()){
		writer.WriteDataTagBool("useSolverBone", rule.GetUseSolverBone());
	}
	
	writer.WriteDataTagFloat("reachRange", rule.GetReachRange());
	if(!rule.GetReachBone().IsEmpty()){
		writer.WriteDataTagString("reachBone", rule.GetReachBone());
	}
	
	const decVector &reachCenter = rule.GetReachCenter();
	if(!reachCenter.IsZero()){
		writer.WriteOpeningTagStart("reachCenter");
		writer.WriteAttributeFloat("x", reachCenter.x);
		writer.WriteAttributeFloat("y", reachCenter.y);
		writer.WriteAttributeFloat("z", reachCenter.z);
		writer.WriteOpeningTagEnd(true);
	}
	
	pSaveControllerTarget(writer, animator, rule.GetTargetGoalPosition(), "goalPosition");
	pSaveControllerTarget(writer, animator, rule.GetTargetGoalOrientation(), "goalOrientation");
	pSaveControllerTarget(writer, animator, rule.GetTargetLocalPosition(), "localPosition");
	pSaveControllerTarget(writer, animator, rule.GetTargetLocalOrientation(), "localOrientation");
	pSaveControllerTarget(writer, animator, rule.GetTargetReachRange(), "reachRange");
	pSaveControllerTarget(writer, animator, rule.GetTargetReachCenter(), "reachCenter");
	
	writer.WriteClosingTag("ruleInverseKinematic");
}

void aeLSAnimator::pSaveRuleForeignState(decXmlWriter &writer, const aeAnimator &animator,
const aeRuleForeignState &rule){
	writer.WriteOpeningTag("ruleForeignState");
	
	pSaveRuleCommon(writer, animator, rule);
	
	writer.WriteDataTagString("foreignBone", rule.GetForeignBone());
	writer.WriteDataTagString("foreignVertexPositionSet", rule.GetForeignVertexPositionSet());
	writer.WriteDataTagFloat("scalePosition", rule.GetScalePosition());
	writer.WriteDataTagFloat("scaleOrientation", rule.GetScaleOrientation());
	writer.WriteDataTagFloat("scaleSize", rule.GetScaleSize());
	writer.WriteDataTagFloat("scaleVertexPositionSet", rule.GetScaleVertexPositionSet());
	
	if(!rule.GetEnablePosition()){
		writer.WriteDataTagBool("enablePosition", rule.GetEnablePosition());
	}
	if(!rule.GetEnableOrientation()){
		writer.WriteDataTagBool("enableOrientation", rule.GetEnableOrientation());
	}
	if(rule.GetEnableSize()){
		writer.WriteDataTagBool("enableSize", rule.GetEnableSize());
	}
	if(!rule.GetEnableVertexPositionSet()){
		writer.WriteDataTagBool("enableVertexPositionSet", rule.GetEnableVertexPositionSet());
	}
	
	if(rule.GetSourceCoordinateFrame() == deAnimatorRuleForeignState::ecfBoneLocal){
		writer.WriteDataTagString("srcCFrame", "local");
		
	}else{ // deAnimatorRuleForeignState::ecfComponent
		writer.WriteDataTagString("srcCFrame", "component");
	}
	
	if(rule.GetDestCoordinateFrame() == deAnimatorRuleForeignState::ecfBoneLocal){
		writer.WriteDataTagString("destCFrame", "local");
		
	}else{ // deAnimatorRuleForeignState::ecfComponent
		writer.WriteDataTagString("destCFrame", "component");
	}
	
	pSaveControllerTarget(writer, animator, rule.GetTargetPosition(), "position");
	pSaveControllerTarget(writer, animator, rule.GetTargetOrientation(), "orientation");
	pSaveControllerTarget(writer, animator, rule.GetTargetSize(), "size");
	pSaveControllerTarget(writer, animator, rule.GetTargetVertexPositionSet(), "vertexPositionSet");
	
	writer.WriteClosingTag("ruleForeignState");
}


void aeLSAnimator::pSaveRuleMirror(decXmlWriter &writer, const aeAnimator &animator,
const aeRuleMirror &rule){
	writer.WriteOpeningTag("ruleMirror");
	
	pSaveRuleCommon(writer, animator, rule);
	
	switch(rule.GetMirrorAxis()){
	case deAnimatorRuleMirror::emaX:
		writer.WriteDataTagString("mirrorAxis", "x");
		break;
		
	case deAnimatorRuleMirror::emaY:
		writer.WriteDataTagString("mirrorAxis", "y");
		break;
		
	case deAnimatorRuleMirror::emaZ:
		writer.WriteDataTagString("mirrorAxis", "z");
		break;
	}
	
	if(!rule.GetMirrorBone().IsEmpty()){
		writer.WriteDataTagString("mirrorBone", rule.GetMirrorBone());
	}
	
	const int count = rule.GetMatchNameCount();
	int i;
	for(i=0; i<count; i++){
		const aeRuleMirror::cMatchName &matchName = *rule.GetMatchNameAt(i);
		writer.WriteOpeningTagStart("matchName");
		writer.WriteAttributeString("first", matchName.GetFirst());
		writer.WriteAttributeString("second", matchName.GetSecond());
		
		switch(matchName.GetType()){
		case deAnimatorRuleMirror::emntFirst:
			writer.WriteAttributeString("type", "first");
			break;
			
		case deAnimatorRuleMirror::emntLast:
			writer.WriteAttributeString("type", "last");
			break;
			
		case deAnimatorRuleMirror::emntMiddle:
			writer.WriteAttributeString("type", "middle");
			break;
		}
		
		writer.WriteOpeningTagEnd(true);
	}
	
	if(!rule.GetEnablePosition()){
		writer.WriteDataTagBool("enablePosition", rule.GetEnablePosition());
	}
	if(!rule.GetEnableOrientation()){
		writer.WriteDataTagBool("enableOrientation", rule.GetEnableOrientation());
	}
	if(rule.GetEnableSize()){
		writer.WriteDataTagBool("enableSize", rule.GetEnableSize());
	}
	if(!rule.GetEnableVertexPositionSet()){
		writer.WriteDataTagBool("enableVertexPositionSet", rule.GetEnableVertexPositionSet());
	}
	
	writer.WriteClosingTag("ruleMirror");
}

void aeLSAnimator::pSaveRuleGroup(decXmlWriter &writer, const aeAnimator &animator,
const aeRuleGroup &rule){
	writer.WriteOpeningTag("ruleGroup");
	
	pSaveRuleCommon(writer, animator, rule);
	
	if(!rule.GetEnablePosition()){
		writer.WriteDataTagBool("enablePosition", rule.GetEnablePosition());
	}
	if(!rule.GetEnableOrientation()){
		writer.WriteDataTagBool("enableOrientation", rule.GetEnableOrientation());
	}
	if(rule.GetEnableSize()){
		writer.WriteDataTagBool("enableSize", rule.GetEnableSize());
	}
	if(!rule.GetEnableVertexPositionSet()){
		writer.WriteDataTagBool("enableVertexPositionSet", rule.GetEnableVertexPositionSet());
	}
	
	const int count = rule.GetRules().GetCount();
	int i;
	for(i=0; i<count; i++){
		writer.WriteNewline();
		pSaveRule(writer, animator, *rule.GetRules().GetAt(i));
	}
	
	if(rule.GetUseCurrentState()){
		writer.WriteDataTagBool("useCurrentState", rule.GetUseCurrentState());
	}
	
	switch(rule.GetApplicationType()){
	case deAnimatorRuleGroup::eatAll:
		writer.WriteDataTagString("applicationType", "all");
		break;
		
	case deAnimatorRuleGroup::eatSelect:
		writer.WriteDataTagString("applicationType", "selection");
		break;
	}
	
	pSaveControllerTarget(writer, animator, rule.GetTargetSelect(), "selection");
	
	writer.WriteClosingTag("ruleGroup");
}

void aeLSAnimator::pSaveRuleSubAnimator(decXmlWriter &writer, const aeAnimator &animator,
const aeRuleSubAnimator &rule){
	writer.WriteOpeningTag("ruleSubAnimator");
	
	pSaveRuleCommon(writer, animator, rule);
	
	writer.WriteDataTagString("pathAnimator", rule.GetPathSubAnimator());
	
	const int connectionCount = rule.GetConnectionCount();
	int i;
	for(i=0; i<connectionCount; i++){
		aeController * const controller = rule.GetControllerAt(i);
		if(!controller){
			continue;
		}
		
		writer.WriteOpeningTagStart("connection");
		writer.WriteAttributeInt("controller", animator.GetControllers().IndexOf(controller));
		writer.WriteOpeningTagEnd(false, false);
		writer.WriteTextString(rule.GetSubAnimator()->GetControllerAt(i)->GetName());
		writer.WriteClosingTag("connection", false);
	}
	
	if(!rule.GetEnablePosition()){
		writer.WriteDataTagBool("enablePosition", rule.GetEnablePosition());
	}
	if(!rule.GetEnableOrientation()){
		writer.WriteDataTagBool("enableOrientation", rule.GetEnableOrientation());
	}
	if(!rule.GetEnableSize()){
		writer.WriteDataTagBool("enableSize", rule.GetEnableSize());
	}
	if(!rule.GetEnableVertexPositionSet()){
		writer.WriteDataTagBool("enableVertexPositionSet", rule.GetEnableVertexPositionSet());
	}
	
	writer.WriteClosingTag("ruleSubAnimator");
}

void aeLSAnimator::pSaveRuleTrackTo(decXmlWriter &writer, const aeAnimator &animator,
const aeRuleTrackTo &rule){
	writer.WriteOpeningTag("ruleTrackTo");
	
	pSaveRuleCommon(writer, animator, rule);
	
	writer.WriteDataTagString("trackBone", rule.GetTrackBone().GetString());
	
	if(rule.GetTrackAxis() == deAnimatorRuleTrackTo::etaPosX){
		writer.WriteDataTagString("trackAxis", "posX");
		
	}else if(rule.GetTrackAxis() == deAnimatorRuleTrackTo::etaPosY){
		writer.WriteDataTagString("trackAxis", "posY");
		
	}else if(rule.GetTrackAxis() == deAnimatorRuleTrackTo::etaPosZ){
		writer.WriteDataTagString("trackAxis", "posZ");
		
	}else if(rule.GetTrackAxis() == deAnimatorRuleTrackTo::etaNegX){
		writer.WriteDataTagString("trackAxis", "negX");
		
	}else if(rule.GetTrackAxis() == deAnimatorRuleTrackTo::etaNegY){
		writer.WriteDataTagString("trackAxis", "negY");
		
	}else{ // deAnimatorRuleTrackTo::etaNegZ
		writer.WriteDataTagString("trackAxis", "negZ");
	}
	
	if(rule.GetUpAxis() == deAnimatorRuleTrackTo::etaPosX){
		writer.WriteDataTagString("upAxis", "posX");
		
	}else if(rule.GetUpAxis() == deAnimatorRuleTrackTo::etaPosY){
		writer.WriteDataTagString("upAxis", "posY");
		
	}else if(rule.GetUpAxis() == deAnimatorRuleTrackTo::etaPosZ){
		writer.WriteDataTagString("upAxis", "posZ");
		
	}else if(rule.GetUpAxis() == deAnimatorRuleTrackTo::etaNegX){
		writer.WriteDataTagString("upAxis", "negX");
		
	}else if(rule.GetUpAxis() == deAnimatorRuleTrackTo::etaNegY){
		writer.WriteDataTagString("upAxis", "negY");
		
	}else{ // deAnimatorRuleTrackTo::etaNegZ
		writer.WriteDataTagString("upAxis", "negZ");
	}
	
	if(rule.GetUpTarget() == deAnimatorRuleTrackTo::eutWorldX){
		writer.WriteDataTagString("upTarget", "worldX");
		
	}else if(rule.GetUpTarget() == deAnimatorRuleTrackTo::eutWorldY){
		writer.WriteDataTagString("upTarget", "worldY");
		
	}else if(rule.GetUpTarget() == deAnimatorRuleTrackTo::eutWorldZ){
		writer.WriteDataTagString("upTarget", "worldZ");
		
	}else if(rule.GetUpTarget() == deAnimatorRuleTrackTo::eutComponentX){
		writer.WriteDataTagString("upTarget", "componentX");
		
	}else if(rule.GetUpTarget() == deAnimatorRuleTrackTo::eutComponentY){
		writer.WriteDataTagString("upTarget", "componentY");
		
	}else if(rule.GetUpTarget() == deAnimatorRuleTrackTo::eutComponentZ){
		writer.WriteDataTagString("upTarget", "componentZ");
		
	}else if(rule.GetUpTarget() == deAnimatorRuleTrackTo::eutTrackBoneX){
		writer.WriteDataTagString("upTarget", "trackBoneX");
		
	}else if(rule.GetUpTarget() == deAnimatorRuleTrackTo::eutTrackBoneY){
		writer.WriteDataTagString("upTarget", "trackBoneY");
		
	}else if(rule.GetUpTarget() == deAnimatorRuleTrackTo::eutTrackBoneZ){
		writer.WriteDataTagString("upTarget", "trackBoneZ");
		
	}else{ // deAnimatorRuleTrackTo::eutController
		writer.WriteDataTagString("upTarget", "controller");
	}
	
	if(rule.GetLockedAxis() == deAnimatorRuleTrackTo::elaNone){
		writer.WriteDataTagString("lockedAxis", "none");
		
	}else if(rule.GetLockedAxis() == deAnimatorRuleTrackTo::elaX){
		writer.WriteDataTagString("lockedAxis", "x");
		
	}else if(rule.GetLockedAxis() == deAnimatorRuleTrackTo::elaY){
		writer.WriteDataTagString("lockedAxis", "z");
		
	}else{ // deAnimatorRuleTrackTo::elaZ
		writer.WriteDataTagString("lockedAxis", "z");
	}
	
	pSaveControllerTarget(writer, animator, rule.GetTargetPosition(), "position");
	pSaveControllerTarget(writer, animator, rule.GetTargetUp(), "up");
	
	writer.WriteClosingTag("ruleTrackTo");
}

void aeLSAnimator::pSaveRuleLimit(decXmlWriter &writer, const aeAnimator &animator,
const aeRuleLimit &rule){
	writer.WriteOpeningTag("ruleLimit");
	
	pSaveRuleCommon(writer, animator, rule);
	
	writer.WriteOpeningTagStart("minimumPosition");
	writer.WriteAttributeFloat("x", rule.GetMinimumPosition().x);
	writer.WriteAttributeFloat("y", rule.GetMinimumPosition().y);
	writer.WriteAttributeFloat("z", rule.GetMinimumPosition().z);
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteOpeningTagStart("maximumPosition");
	writer.WriteAttributeFloat("x", rule.GetMaximumPosition().x);
	writer.WriteAttributeFloat("y", rule.GetMaximumPosition().y);
	writer.WriteAttributeFloat("z", rule.GetMaximumPosition().z);
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteOpeningTagStart("minimumRotation");
	writer.WriteAttributeFloat("x", rule.GetMinimumRotation().x);
	writer.WriteAttributeFloat("y", rule.GetMinimumRotation().y);
	writer.WriteAttributeFloat("z", rule.GetMinimumRotation().z);
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteOpeningTagStart("maximumRotation");
	writer.WriteAttributeFloat("x", rule.GetMaximumRotation().x);
	writer.WriteAttributeFloat("y", rule.GetMaximumRotation().y);
	writer.WriteAttributeFloat("z", rule.GetMaximumRotation().z);
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteOpeningTagStart("minimumScaling");
	writer.WriteAttributeFloat("x", rule.GetMinimumScaling().x);
	writer.WriteAttributeFloat("y", rule.GetMinimumScaling().y);
	writer.WriteAttributeFloat("z", rule.GetMinimumScaling().z);
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteOpeningTagStart("maximumScaling");
	writer.WriteAttributeFloat("x", rule.GetMaximumScaling().x);
	writer.WriteAttributeFloat("y", rule.GetMaximumScaling().y);
	writer.WriteAttributeFloat("z", rule.GetMaximumScaling().z);
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteDataTagFloat("minimumVertexPositionSet", rule.GetMinimumVertexPositionSet());
	writer.WriteDataTagFloat("maximumVertexPositionSet", rule.GetMaximumVertexPositionSet());
	
	if(rule.GetCoordinateFrame() == deAnimatorRuleLimit::ecfBoneLocal){
		writer.WriteDataTagString("cframe", "local");
		
	}else if(rule.GetCoordinateFrame() == deAnimatorRuleLimit::ecfComponent){
		writer.WriteDataTagString("cframe", "component");
		
	}else{ // deAnimatorRuleBoneTransformator::ecfRotationBone
		writer.WriteDataTagString("cframe", "target");
	}
	
	writer.WriteDataTagString("targetBone", rule.GetTargetBone());
	
	if(rule.GetEnablePositionXMin() || rule.GetEnablePositionYMin() || rule.GetEnablePositionZMin()){
		writer.WriteOpeningTagStart("enablePosMin");
		if(rule.GetEnablePositionXMin()){
			writer.WriteAttributeBool("x", rule.GetEnablePositionXMin());
		}
		if(rule.GetEnablePositionYMin()){
			writer.WriteAttributeBool("y", rule.GetEnablePositionYMin());
		}
		if(rule.GetEnablePositionZMin()){
			writer.WriteAttributeBool("z", rule.GetEnablePositionZMin());
		}
		writer.WriteOpeningTagEnd(true);
	}
	
	if(rule.GetEnablePositionXMax() || rule.GetEnablePositionYMax() || rule.GetEnablePositionZMax()){
		writer.WriteOpeningTagStart("enablePosMax");
		if(rule.GetEnablePositionXMax()){
			writer.WriteAttributeBool("x", rule.GetEnablePositionXMax());
		}
		if(rule.GetEnablePositionYMax()){
			writer.WriteAttributeBool("y", rule.GetEnablePositionYMax());
		}
		if(rule.GetEnablePositionZMax()){
			writer.WriteAttributeBool("z", rule.GetEnablePositionZMax());
		}
		writer.WriteOpeningTagEnd(true);
	}
	
	if(rule.GetEnableRotationXMin() || rule.GetEnableRotationYMin() || rule.GetEnableRotationZMin()){
		writer.WriteOpeningTagStart("enableRotMin");
		if(rule.GetEnableRotationXMin()){
			writer.WriteAttributeBool("x", rule.GetEnableRotationXMin());
		}
		if(rule.GetEnableRotationYMin()){
			writer.WriteAttributeBool("y", rule.GetEnableRotationYMin());
		}
		if(rule.GetEnableRotationZMin()){
			writer.WriteAttributeBool("z", rule.GetEnableRotationZMin());
		}
		writer.WriteOpeningTagEnd(true);
	}
	
	if(rule.GetEnableRotationXMax() || rule.GetEnableRotationYMax() || rule.GetEnableRotationZMax()){
		writer.WriteOpeningTagStart("enableRotMax");
		if(rule.GetEnableRotationXMax()){
			writer.WriteAttributeBool("x", rule.GetEnableRotationXMax());
		}
		if(rule.GetEnableRotationYMax()){
			writer.WriteAttributeBool("y", rule.GetEnableRotationYMax());
		}
		if(rule.GetEnableRotationZMax()){
			writer.WriteAttributeBool("z", rule.GetEnableRotationZMax());
		}
		writer.WriteOpeningTagEnd(true);
	}
	
	if(rule.GetEnableScalingXMin() || rule.GetEnableScalingYMin() || rule.GetEnableScalingZMin()){
		writer.WriteOpeningTagStart("enableScaleMin");
		if(rule.GetEnableScalingXMin()){
			writer.WriteAttributeBool("x", rule.GetEnableScalingXMin());
		}
		if(rule.GetEnableScalingYMin()){
			writer.WriteAttributeBool("y", rule.GetEnableScalingYMin());
		}
		if(rule.GetEnableScalingZMin()){
			writer.WriteAttributeBool("z", rule.GetEnableScalingZMin());
		}
		writer.WriteOpeningTagEnd(true);
	}
	
	if(rule.GetEnableScalingXMax() || rule.GetEnableScalingYMax() || rule.GetEnableScalingZMax()){
		writer.WriteOpeningTagStart("enableScaleMax");
		if(rule.GetEnableScalingXMax()){
			writer.WriteAttributeBool("x", rule.GetEnableScalingXMax());
		}
		if(rule.GetEnableScalingYMax()){
			writer.WriteAttributeBool("y", rule.GetEnableScalingYMax());
		}
		if(rule.GetEnableScalingZMax()){
			writer.WriteAttributeBool("z", rule.GetEnableScalingZMax());
		}
		writer.WriteOpeningTagEnd(true);
	}
	
	if(rule.GetEnableVertexPositionSetMin()){
		writer.WriteDataTagFloat("enableVertexPositionSetMin", rule.GetEnableVertexPositionSetMin());
	}
	if(rule.GetEnableVertexPositionSetMax()){
		writer.WriteDataTagFloat("enableVertexPositionSetMax", rule.GetEnableVertexPositionSetMax());
	}
	
	writer.WriteClosingTag("ruleLimit");
}



bool aeLSAnimator::pHasAttribute(decXmlElementTag *tag, const char *name) const{
	return tag->FindAttribute(name) != nullptr;
}

const char *aeLSAnimator::pGetAttributeString(decXmlElementTag *tag, const char *name) const{
	decXmlAttValue *value = tag->FindAttribute(name);
	
	if(value){
		return value->GetValue();
		
	}else{
		deLogger &logger = *pLSSys->GetWindowMain()->GetEnvironment().GetLogger();
		logger.LogErrorFormat(LOGSOURCE, "Missing Attribute %s in tag %s",
			name, tag->GetName().GetString());
		DETHROW(deeInvalidParam);
	}
}

int aeLSAnimator::pGetAttributeInt(decXmlElementTag *tag, const char *name) const{
	decXmlAttValue *value = tag->FindAttribute(name);
	
	if(value){
		return (int)strtol(value->GetValue(), nullptr, 10);
		
	}else{
		deLogger &logger = *pLSSys->GetWindowMain()->GetEnvironment().GetLogger();
		logger.LogErrorFormat(LOGSOURCE, "Missing Attribute %s in tag %s",
			name, tag->GetName().GetString());
		DETHROW(deeInvalidParam);
	}
}

float aeLSAnimator::pGetAttributeFloat(decXmlElementTag *tag, const char *name) const{
	decXmlAttValue *value = tag->FindAttribute(name);
	
	if(value){
		return strtof(value->GetValue(), nullptr);
		
	}else{
		deLogger &logger = *pLSSys->GetWindowMain()->GetEnvironment().GetLogger();
		logger.LogErrorFormat(LOGSOURCE, "Missing Attribute %s in tag %s",
			name, tag->GetName().GetString());
		DETHROW(deeInvalidParam);
	}
}



void aeLSAnimator::pLoadAnimator(decXmlElementTag *root, aeAnimator &animator){
	deLogger &logger = *pLSSys->GetWindowMain()->GetEnvironment().GetLogger();
	decXmlElementTag *tag;
	int i;
	
	for(i=0; i<root->GetElementCount(); i++){
		tag = root->GetElementIfTag(i);
		if(tag){
			if(strcmp(tag->GetName(), "display") == 0){
				pLoadDisplay(tag, animator);
				
			}else if(strcmp(tag->GetName(), "rig") == 0){
				if(tag->GetFirstData()){
					animator.SetRigPath(GetCDataString(*tag));
				}
				
			}else if(strcmp(tag->GetName(), "animation") == 0){
				if(tag->GetFirstData()){
					animator.SetAnimationPath(GetCDataString(*tag));
				}
				
			}else if(strcmp(tag->GetName(), "bone") == 0){
				animator.AddBone(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "vertexPositionSet") == 0){
				animator.AddVertexPositionSet(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "controller") == 0){
				pLoadController(tag, animator);
				
			}else if(strcmp(tag->GetName(), "link") == 0){
				pLoadLink(tag, animator);
				
			}else{
				const aeRule::Ref rule(aeRule::Ref::NewDeprecated(pLoadRule(tag, animator)));
				if(rule){
					animator.AddRule(rule);
					
				}else{
					logger.LogWarnFormat(LOGSOURCE, "animator(%i:%i): Unknown Tag %s, ignoring",
						tag->GetLineNumber(), tag->GetPositionNumber(),
						tag->GetName().GetString());
				}
			}
		}
	}
	
	pLoadSubAnimators(animator);
}

void aeLSAnimator::pLoadDisplay(decXmlElementTag *root, aeAnimator &animator){
	deLogger &logger = *pLSSys->GetWindowMain()->GetEnvironment().GetLogger();
	decXmlElementTag *tag;
	int i;
	
	for(i=0; i<root->GetElementCount(); i++){
		tag = root->GetElementIfTag(i);
		if(tag){
			if(strcmp(tag->GetName(), "model") == 0){
				if(tag->GetFirstData()){
					animator.SetDisplayModelPath(GetCDataString(*tag));
				}
				
			}else if(strcmp(tag->GetName(), "skin") == 0){
				if(tag->GetFirstData()){
					animator.SetDisplaySkinPath(GetCDataString(*tag));
				}
				
			}else if(strcmp(tag->GetName(), "rig") == 0){
				if(tag->GetFirstData()){
					animator.SetDisplayRigPath(GetCDataString(*tag));
				}
				
			/*}else if( strcmp( tag->GetName(), "animation" ) == 0 ){
				if(tag->GetFirstData()){
					animator.SetAnimationPath(GetCDataString(*tag));
				}*/
				
			}else if(strcmp(tag->GetName(), "locomotion") == 0){
				pLoadLocomotion(tag, animator);
				
			}else{
				logger.LogWarnFormat(LOGSOURCE, "animator(%i:%i): Unknown Tag %s, ignoring",
					tag->GetLineNumber(), tag->GetPositionNumber(), tag->GetName().GetString());
			}
		}
	}
}

void aeLSAnimator::pLoadLocomotion(decXmlElementTag *root, aeAnimator &animator){
	deLogger &logger = *pLSSys->GetWindowMain()->GetEnvironment().GetLogger();
	aeAnimatorLocomotion &locomotion = animator.GetLocomotion();
	decXmlElementTag *tag;
	int i, leg = 0;
	
	for(i=0; i<root->GetElementCount(); i++){
		tag = root->GetElementIfTag(i);
		if(tag){
			if(strcmp(tag->GetName(), "locomotionType") == 0){
				const decString cdata(GetCDataString(*tag));
				if(cdata == "natural"){
					locomotion.SetLocomotionType(aeAnimatorLocomotion::eltNatural);
					
				}else if(cdata == "fps"){
					locomotion.SetLocomotionType(aeAnimatorLocomotion::eltNatural);
					
				}else if(cdata == "vehicle"){
					locomotion.SetLocomotionType(aeAnimatorLocomotion::eltNatural);
					
				}else{
					LogErrorUnknownValue(*tag, cdata);
				}
				
				
			}else if(strcmp(tag->GetName(), "limitLookDown") == 0){
				locomotion.SetLimitLookDown(strtof(GetCDataString(*tag), nullptr));
				
			}else if(strcmp(tag->GetName(), "limitLookUp") == 0){
				locomotion.SetLimitLookUp(strtof(GetCDataString(*tag), nullptr));
				
			}else if(strcmp(tag->GetName(), "limitLookLeft") == 0){
				locomotion.SetLimitLookLeft(strtof(GetCDataString(*tag), nullptr));
				
			}else if(strcmp(tag->GetName(), "limitLookRight") == 0){
				locomotion.SetLimitLookRight(strtof(GetCDataString(*tag), nullptr));
				
			}else if(strcmp(tag->GetName(), "limitLookUpDownSpeed") == 0){
				// deprecated
				
			}else if(strcmp(tag->GetName(), "limitLookLeftRightSpeed") == 0){
				// deprecated
				
			}else if(strcmp(tag->GetName(), "adjustTimeUpDown") == 0){
				locomotion.GetLookUpDown().SetAdjustTime(strtof(GetCDataString(*tag), nullptr));
				
			}else if(strcmp(tag->GetName(), "adjustTimeLeftRight") == 0){
				locomotion.GetLookLeftRight().SetAdjustTime(strtof(GetCDataString(*tag), nullptr));
				
			}else if(strcmp(tag->GetName(), "adjustTimeStance") == 0){
				locomotion.GetStance().SetAdjustTime(strtof(GetCDataString(*tag), nullptr));
				
			}else if(strcmp(tag->GetName(), "adjustTimeTurnIP") == 0){
				locomotion.SetAdjustTimeTurnIP(strtof(GetCDataString(*tag), nullptr));
				
			}else if(strcmp(tag->GetName(), "adjustTimeOrientation") == 0){
				locomotion.GetOrientation().SetAdjustTime(strtof(GetCDataString(*tag), nullptr));
				
			}else if(strcmp(tag->GetName(), "adjustTimeVelocity") == 0){
				locomotion.GetLinearVelocity().SetAdjustTime(strtof(GetCDataString(*tag), nullptr));
				
			}else if(strcmp(tag->GetName(), "speedWalk") == 0){
				locomotion.SetWalkSpeed(strtof(GetCDataString(*tag), nullptr));
				
			}else if(strcmp(tag->GetName(), "speedRun") == 0){
				locomotion.SetRunSpeed(strtof(GetCDataString(*tag), nullptr));
				
			}else if(strcmp(tag->GetName(), "legBlendTime") == 0){
				locomotion.SetLegBlendTime(strtof(GetCDataString(*tag), nullptr));
				
			}else if(strcmp(tag->GetName(), "useLegPairs") == 0){
				locomotion.SetUseLegPairCount((int)strtol(GetCDataString(*tag), nullptr, 10));
				
			}else if(strcmp(tag->GetName(), "leg") == 0){
				if(leg >= locomotion.GetLegCount()){
					logger.LogWarnFormat(LOGSOURCE, "animator(%i:%i): Too many leg definitions, ignoring",
						tag->GetLineNumber(), tag->GetPositionNumber());
					continue;
				}
				
				pLoadLocomotionLeg(tag, animator, leg);
				leg++;
				
			}else{
				logger.LogWarnFormat(LOGSOURCE, "animator(%i:%i): Unknown Tag %s, ignoring",
					tag->GetLineNumber(), tag->GetPositionNumber(), tag->GetName().GetString());
			}
		}
	}
}

void aeLSAnimator::pLoadLocomotionLeg(decXmlElementTag *root, aeAnimator &animator, int legnum){
	deLogger &logger = *pLSSys->GetWindowMain()->GetEnvironment().GetLogger();
	aeAnimatorLocomotionLeg &leg = *animator.GetLocomotion().GetLegAt(legnum);
	decXmlElementTag *tag;
	decVector vector;
	int i;
	
	for(i=0; i<root->GetElementCount(); i++){
		tag = root->GetElementIfTag(i);
		if(tag){
			if(strcmp(tag->GetName(), "liftOffTime") == 0){
				leg.SetLiftOffTime(strtof(GetCDataString(*tag), nullptr));
				
			}else if(strcmp(tag->GetName(), "putDownTime") == 0){
				leg.SetPutDownTime(strtof(GetCDataString(*tag), nullptr));
				
			}else if(strcmp(tag->GetName(), "putDownPosition") == 0){ // depracted
				vector.SetZero();
				pLoadVector(tag, vector);
				leg.SetPutDownPositionStand(vector);
				
			}else if(strcmp(tag->GetName(), "putDownPositionStand") == 0){
				vector.SetZero();
				pLoadVector(tag, vector);
				leg.SetPutDownPositionStand(vector);
				
			}else if(strcmp(tag->GetName(), "putDownPositionWalk") == 0){
				vector.SetZero();
				pLoadVector(tag, vector);
				leg.SetPutDownPositionWalk(vector);
				
			}else if(strcmp(tag->GetName(), "putDownPositionRun") == 0){
				vector.SetZero();
				pLoadVector(tag, vector);
				leg.SetPutDownPositionRun(vector);
				
			}else{
				logger.LogWarnFormat(LOGSOURCE, "animator(%i:%i): Unknown Tag %s, ignoring",
					tag->GetLineNumber(), tag->GetPositionNumber(), tag->GetName().GetString());
			}
		}
	}
}

void aeLSAnimator::pLoadController(decXmlElementTag *root, aeAnimator &animator){
	deLogger &logger = *pLSSys->GetWindowMain()->GetEnvironment().GetLogger();
	decXmlElementTag *tag;
	const char *cdata;
	decVector vector;
	int i, leg;
	
	const aeController::Ref controller(aeController::Ref::New());
	animator.AddController(controller);
	
	// parse tag
	for(i=0; i<root->GetElementCount(); i++){
		tag = root->GetElementIfTag(i);
		if(tag){
			if(strcmp(tag->GetName(), "name") == 0){
				controller->SetName(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "clamp") == 0){
				controller->SetClamp(GetCDataBool(*tag));
				
			}else if(strcmp(tag->GetName(), "frozen") == 0){
				controller->SetFrozen(GetCDataBool(*tag));
				
			}else if(strcmp(tag->GetName(), "limit") == 0){
				pLoadControllerLimit(tag, controller);
				
			}else if(strcmp(tag->GetName(), "limits") == 0){
				controller->SetMinimumValue(pGetAttributeFloat(tag, "min"));
				controller->SetMaximumValue(pGetAttributeFloat(tag, "max"));
				
			}else if(strcmp(tag->GetName(), "locomotionAttribute") == 0){
				cdata = GetCDataString(*tag);
				
				if(strcmp(cdata, "elapsedTime") == 0){
					controller->SetLocomotionAttribute(aeAnimatorLocomotion::eaElapsedTime);
					
				}else if(strcmp(cdata, "lookUpDown") == 0){
					controller->SetLocomotionAttribute(aeAnimatorLocomotion::eaLookUpDown);
					
				}else if(strcmp(cdata, "lookLeftRight") == 0){
					controller->SetLocomotionAttribute(aeAnimatorLocomotion::eaLookLeftRight);
					
				}else if(strcmp(cdata, "movingSpeed") == 0){
					controller->SetLocomotionAttribute(aeAnimatorLocomotion::eaMovingSpeed);
					
				}else if(strcmp(cdata, "movingDirection") == 0){
					controller->SetLocomotionAttribute(aeAnimatorLocomotion::eaMovingDirection);
					
				}else if(strcmp(cdata, "relativeMovingSpeed") == 0){
					controller->SetLocomotionAttribute(aeAnimatorLocomotion::eaRelativeMovingSpeed);
					
				}else if(strcmp(cdata, "turningSpeed") == 0){
					controller->SetLocomotionAttribute(aeAnimatorLocomotion::eaTurningSpeed);
					
				}else if(strcmp(cdata, "stance") == 0){
					controller->SetLocomotionAttribute(aeAnimatorLocomotion::eaStance);
					
				}else if(strcmp(cdata, "displacement") == 0){
					controller->SetLocomotionAttribute(aeAnimatorLocomotion::eaDisplacement);
					
				}else if(strcmp(cdata, "relativeDisplacement") == 0){
					controller->SetLocomotionAttribute(aeAnimatorLocomotion::eaRelativeDisplacement);
					
				}else if(strcmp(cdata, "bodyTiltOffset") == 0){
					controller->SetLocomotionAttribute(aeAnimatorLocomotion::eaBodyTiltOffset);
					
				}else if(strcmp(cdata, "bodyTiltUpDown") == 0){
					controller->SetLocomotionAttribute(aeAnimatorLocomotion::eaBodyTiltUpDown);
					
				}else if(strcmp(cdata, "bodyTiltLeftRight") == 0){
					controller->SetLocomotionAttribute(aeAnimatorLocomotion::eaBodyTiltRightLeft);
					
				}else if(strcmp(cdata, "timeTurnIP") == 0){
					controller->SetLocomotionAttribute(aeAnimatorLocomotion::eaTimeTurnIP);
					
				}else if(strcmp(cdata, "legGroundPosition") == 0){
					controller->SetLocomotionAttribute(aeAnimatorLocomotion::eaLegGroundPosition);
					
				}else if(strcmp(cdata, "legGroundNormal") == 0){
					controller->SetLocomotionAttribute(aeAnimatorLocomotion::eaLegGroundNormal);
					
				}else if(strcmp(cdata, "legInfluence") == 0){
					controller->SetLocomotionAttribute(aeAnimatorLocomotion::eaLegInfluence);
					
				}else if(strcmp(cdata, "legPosition") == 0){
					controller->SetLocomotionAttribute(aeAnimatorLocomotion::eaLegPosition);
					
				}else if(strcmp(cdata, "legOrientation") == 0){
					controller->SetLocomotionAttribute(aeAnimatorLocomotion::eaLegOrientation);
					
				}else{
					logger.LogWarnFormat(LOGSOURCE, "controller(%i:%i): Unknown locomotion attribute %s, ignoring",
						tag->GetLineNumber(), tag->GetPositionNumber(), cdata);
				}
				
			}else if(strcmp(tag->GetName(), "locomotionLeg") == 0){
				leg = (int)strtol(GetCDataString(*tag), nullptr, 10);
				
				if(leg >= 0 && leg < animator.GetLocomotion().GetLegCount()){
					controller->SetLocomotionLeg(leg);
				}
				
			}else if(strcmp(tag->GetName(), "vectorSimulation") == 0){
				cdata = GetCDataString(*tag);
				
				if(strcmp(cdata, "position") == 0){
					controller->SetVectorSimulation(aeController::evsPosition);
					
				}else if(strcmp(cdata, "rotation") == 0){
					controller->SetVectorSimulation(aeController::evsRotation);
					
				}else{
					logger.LogWarnFormat(LOGSOURCE, "controller(%i:%i): Unknown vector simulation %s, ignoring",
						tag->GetLineNumber(), tag->GetPositionNumber(), cdata);
				}
				
			}else if(strcmp(tag->GetName(), "value") == 0){
				controller->SetDefaultValue(GetCDataFloat(*tag));
				controller->SetCurrentValue(controller->GetDefaultValue());
				
			}else if(strcmp(tag->GetName(), "vector") == 0){
				ReadVector(*tag, vector);
				controller->SetDefaultVector(vector);
				controller->SetVector(controller->GetDefaultVector());
				
			}else{
				logger.LogWarnFormat(LOGSOURCE, "controller(%i:%i): Unknown Tag %s, ignoring",
					tag->GetLineNumber(), tag->GetPositionNumber(), tag->GetName().GetString());
			}
		}
	}
}

void aeLSAnimator::pLoadControllerLimit(decXmlElementTag *root, aeController &controller){
	deLogger &logger = *pLSSys->GetWindowMain()->GetEnvironment().GetLogger();
	decXmlElementTag *tag;
	int i;
	
	for(i=0; i<root->GetElementCount(); i++){
		tag = root->GetElementIfTag(i);
		if(tag){
			if(strcmp(tag->GetName(), "minimum") == 0){
				controller.SetMinimumValue(strtof(GetCDataString(*tag), nullptr));
				
			}else if(strcmp(tag->GetName(), "maximum") == 0){
				controller.SetMaximumValue(strtof(GetCDataString(*tag), nullptr));
				
			}else{
				logger.LogWarnFormat(LOGSOURCE, "limit(%i:%i): Unknown Tag %s, ignoring",
					tag->GetLineNumber(), tag->GetPositionNumber(), tag->GetName().GetString());
			}
		}
	}
}

void aeLSAnimator::pLoadLink(decXmlElementTag *root, aeAnimator &animator){
	deLogger &logger = *pLSSys->GetWindowMain()->GetEnvironment().GetLogger();
	decXmlElementTag *tag;
	int i, index;
	
	const aeLink::Ref link(aeLink::Ref::New());
	animator.AddLink(link);
	
	// parse tag
	for(i=0; i<root->GetElementCount(); i++){
		tag = root->GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		if(tag->GetName() == "name"){
			link->SetName(GetCDataString(*tag));
			
		}else if(tag->GetName() == "controller"){
			index = (int)strtol(GetCDataString(*tag), nullptr, 10);
			
			if(index == -1){
				link->SetController(nullptr, false);
				
			}else{
				link->SetController(animator.GetControllers().GetAt(index), false);
			}
			
		}else if(tag->GetName() == "repeat"){
			link->SetRepeat((int)strtol(GetCDataString(*tag), nullptr, 10));
			
		}else if(tag->GetName() == "curve"){
			decCurveBezier curve;
			ReadCurveBezier(*tag, curve);
			link->SetCurve(curve);
			
		}else if(tag->GetName() == "bone"){
			link->SetBone(GetCDataString(*tag));
			
		}else if(tag->GetName() == "boneParameter"){
			const char * const name = GetCDataString(*tag);
			
			if(strcmp(name, "positionX") == 0){
				link->SetBoneParameter(deAnimatorLink::ebpPositionX);
				
			}else if(strcmp(name, "positionY") == 0){
				link->SetBoneParameter(deAnimatorLink::ebpPositionY);
				
			}else if(strcmp(name, "positionZ") == 0){
				link->SetBoneParameter(deAnimatorLink::ebpPositionZ);
				
			}else if(strcmp(name, "rotationX") == 0){
				link->SetBoneParameter(deAnimatorLink::ebpRotationX);
				
			}else if(strcmp(name, "rotationY") == 0){
				link->SetBoneParameter(deAnimatorLink::ebpRotationY);
				
			}else if(strcmp(name, "rotationZ") == 0){
				link->SetBoneParameter(deAnimatorLink::ebpRotationZ);
				
			}else if(strcmp(name, "scaleX") == 0){
				link->SetBoneParameter(deAnimatorLink::ebpScaleX);
				
			}else if(strcmp(name, "scaleY") == 0){
				link->SetBoneParameter(deAnimatorLink::ebpScaleY);
				
			}else if(strcmp(name, "scaleZ") == 0){
				link->SetBoneParameter(deAnimatorLink::ebpScaleZ);
				
			}else{
				LogErrorUnknownValue(*tag, name);
			}
			
		}else if(tag->GetName() == "boneLimits"){
			link->SetBoneMinimum(GetAttributeFloat(*tag, "min"));
			link->SetBoneMaximum(GetAttributeFloat(*tag, "max"));
			
		}else if(tag->GetName() == "vertexPositionSet"){
			link->SetVertexPositionSet(GetCDataString(*tag));
			
		}else if(tag->GetName() == "vertexPositionSetLimits"){
			link->SetVertexPositionSetMinimum(GetAttributeFloat(*tag, "min"));
			link->SetVertexPositionSetMaximum(GetAttributeFloat(*tag, "max"));
			
		}else if(tag->GetName() == "wrapY"){
			link->SetWrapY(GetCDataBool(*tag));
			
		}else{
			logger.LogWarnFormat(LOGSOURCE, "%s(%i:%i): Unknown Tag %s, ignoring",
				root->GetName().GetString(), tag->GetLineNumber(), tag->GetPositionNumber(),
				tag->GetName().GetString());
		}
	}
}

aeRule::Ref aeLSAnimator::pLoadRule(decXmlElementTag *root, aeAnimator &animator){
	if(strcmp(root->GetName(), "ruleAnimation") == 0){
		return pLoadRuleAnimation(root, animator);
		
	}else if(strcmp(root->GetName(), "ruleAnimationDifference") == 0){
		return pLoadRuleAnimationDifference(root, animator);
		
	}else if(strcmp(root->GetName(), "ruleAnimationSelect") == 0){
		return pLoadRuleAnimationSelect(root, animator);
		
	}else if(strcmp(root->GetName(), "ruleBoneTransformator") == 0){
		return pLoadRuleBoneTransformator(root, animator);
		
	}else if(strcmp(root->GetName(), "ruleStateManipulator") == 0){
		return pLoadRuleStateManipulator(root, animator);
		
	}else if(strcmp(root->GetName(), "ruleStateSnapshot") == 0){
		return pLoadRuleStateSnapshot(root, animator);
		
	}else if(strcmp(root->GetName(), "ruleInverseKinematic") == 0){
		return pLoadRuleInverseKinematic(root, animator);
		
	}else if(strcmp(root->GetName(), "ruleForeignState") == 0){
		return pLoadRuleForeignState(root, animator);
		
	}else if(strcmp(root->GetName(), "ruleGroup") == 0){
		return pLoadRuleGroup(root, animator);
		
	}else if(strcmp(root->GetName(), "ruleSubAnimator") == 0){
		return pLoadRuleSubAnimator(root, animator);
		
	}else if(strcmp(root->GetName(), "ruleTrackTo") == 0){
		return pLoadRuleTrackTo(root, animator);
		
	}else if(strcmp(root->GetName(), "ruleLimit") == 0){
		return pLoadRuleLimit(root, animator);
		
	}else if(strcmp(root->GetName(), "ruleMirror") == 0){
		return pLoadRuleMirror(root, animator);
	}
	
	return nullptr;
}

aeRule::Ref aeLSAnimator::pLoadRuleAnimation(decXmlElementTag *root, aeAnimator &animator){
	deLogger &logger = *pLSSys->GetWindowMain()->GetEnvironment().GetLogger();
	const aeRuleAnimation::Ref rule(aeRuleAnimation::Ref::New());
	decXmlElementTag *tag;
	const char *name;
	int i;
	
	for(i=0; i<root->GetElementCount(); i++){
		tag = root->GetElementIfTag(i);
		if(tag){
			if(!pLoadRuleCommon(tag, animator, rule)){
				if(strcmp(tag->GetName(), "moveName") == 0){
					rule->SetMoveName(GetCDataString(*tag));
					
				}else if(strcmp(tag->GetName(), "moveTime") == 0){
					rule->SetMoveTime(strtof(GetCDataString(*tag), nullptr));
					
				}else if(strcmp(tag->GetName(), "enablePosition") == 0){
					rule->SetEnablePosition(GetCDataBool(*tag));
					
				}else if(strcmp(tag->GetName(), "enableOrientation") == 0){
					rule->SetEnableOrientation(GetCDataBool(*tag));
					
				}else if(strcmp(tag->GetName(), "enableSize") == 0){
					rule->SetEnableSize(GetCDataBool(*tag));
					
				}else if(strcmp(tag->GetName(), "enableVertexPositionSet") == 0){
					rule->SetEnableVertexPositionSet(GetCDataBool(*tag));
					
				}else if(strcmp(tag->GetName(), "target") == 0){
					name = pGetAttributeString(tag, "name");
					
					if(strcmp(name, "blendFactor") == 0){
						pLoadControllerTarget(tag, animator, rule->GetTargetBlendFactor());
						
					}else if(strcmp(name, "moveTime") == 0){
						pLoadControllerTarget(tag, animator, rule->GetTargetMoveTime());
						
					}else{
						logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown target '%s'",
							root->GetName().GetString(), tag->GetLineNumber(),
							tag->GetPositionNumber(), name);
						DETHROW(deeInvalidFileFormat);
					}
					
				}else{
					logger.LogWarnFormat(LOGSOURCE, "%s(%i:%i): Unknown Tag %s, ignoring",
						root->GetName().GetString(), tag->GetLineNumber(),
						tag->GetPositionNumber(), tag->GetName().GetString());
				}
			}
		}
	}
	return rule;
}

aeRule::Ref aeLSAnimator::pLoadRuleAnimationDifference(decXmlElementTag *root, aeAnimator &animator){
	deLogger &logger = *pLSSys->GetWindowMain()->GetEnvironment().GetLogger();
	const aeRuleAnimationDifference::Ref rule(aeRuleAnimationDifference::Ref::New());
	decXmlElementTag *tag;
	const char *name;
	int i;
	
	for(i=0; i<root->GetElementCount(); i++){
		tag = root->GetElementIfTag(i);
		if(tag){
			if(!pLoadRuleCommon(tag, animator, rule)){
				if(strcmp(tag->GetName(), "leadingMoveName") == 0){
					rule->SetLeadingMoveName(GetCDataString(*tag));
					
				}else if(strcmp(tag->GetName(), "leadingMoveTime") == 0){
					rule->SetLeadingMoveTime(strtof(GetCDataString(*tag), nullptr));
					
				}else if(strcmp(tag->GetName(), "referenceMoveName") == 0){
					rule->SetReferenceMoveName(GetCDataString(*tag));
					
				}else if(strcmp(tag->GetName(), "referenceMoveTime") == 0){
					rule->SetReferenceMoveTime(strtof(GetCDataString(*tag), nullptr));
					
				}else if(strcmp(tag->GetName(), "enablePosition") == 0){
					rule->SetEnablePosition(GetCDataBool(*tag));
					
				}else if(strcmp(tag->GetName(), "enableOrientation") == 0){
					rule->SetEnableOrientation(GetCDataBool(*tag));
					
				}else if(strcmp(tag->GetName(), "enableSize") == 0){
					rule->SetEnableSize(GetCDataBool(*tag));
					
				}else if(strcmp(tag->GetName(), "enableVertexPositionSet") == 0){
					rule->SetEnableVertexPositionSet(GetCDataBool(*tag));
					
				}else if(strcmp(tag->GetName(), "target") == 0){
					name = pGetAttributeString(tag, "name");
					
					if(strcmp(name, "blendFactor") == 0){
						pLoadControllerTarget(tag, animator, rule->GetTargetBlendFactor());
						
					}else if(strcmp(name, "leadingMoveTime") == 0){
						pLoadControllerTarget(tag, animator, rule->GetTargetLeadingMoveTime());
						
					}else if(strcmp(name, "referenceMoveTime") == 0){
						pLoadControllerTarget(tag, animator, rule->GetTargetReferenceMoveTime());
						
					}else{
						logger.LogErrorFormat("%s(%i:%i): Unknown target '%s'",
							root->GetName().GetString(), tag->GetLineNumber(),
							tag->GetPositionNumber(), name);
						DETHROW(deeInvalidFileFormat);
					}
					
				}else{
					logger.LogWarnFormat("%s(%i:%i): Unknown Tag %s, ignoring",
						root->GetName().GetString(), tag->GetLineNumber(),
						tag->GetPositionNumber(), tag->GetName().GetString());
				}
			}
		}
	}
	return rule;
}

aeRule::Ref aeLSAnimator::pLoadRuleAnimationSelect(decXmlElementTag *root, aeAnimator &animator){
	deLogger &logger = *pLSSys->GetWindowMain()->GetEnvironment().GetLogger();
	const aeRuleAnimationSelect::Ref rule(aeRuleAnimationSelect::Ref::New());
	decXmlElementTag *tag;
	decStringList moves;
	const char *name;
	int i;
	
	for(i=0; i<root->GetElementCount(); i++){
		tag = root->GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		if(pLoadRuleCommon(tag, animator, rule)){
			continue;
		}
		
		if(strcmp(tag->GetName(), "move") == 0){
			moves.Add(GetCDataString(*tag));
			
		}else if(strcmp(tag->GetName(), "enablePosition") == 0){
			rule->SetEnablePosition(GetCDataBool(*tag));
			
		}else if(strcmp(tag->GetName(), "enableOrientation") == 0){
			rule->SetEnableOrientation(GetCDataBool(*tag));
			
		}else if(strcmp(tag->GetName(), "enableSize") == 0){
			rule->SetEnableSize(GetCDataBool(*tag));
			
		}else if(strcmp(tag->GetName(), "enableVertexPositionSet") == 0){
			rule->SetEnableVertexPositionSet(GetCDataBool(*tag));
			
		}else if(strcmp(tag->GetName(), "target") == 0){
			name = pGetAttributeString(tag, "name");
			
			if(strcmp(name, "blendFactor") == 0){
				pLoadControllerTarget(tag, animator, rule->GetTargetBlendFactor());
				
			}else if(strcmp(name, "moveTime") == 0){
				pLoadControllerTarget(tag, animator, rule->GetTargetMoveTime());
				
			}else if(strcmp(name, "selection") == 0){
				pLoadControllerTarget(tag, animator, rule->GetTargetSelect());
				
			}else{
				logger.LogErrorFormat("%s(%i:%i): Unknown target '%s'",
					root->GetName().GetString(), tag->GetLineNumber(),
					tag->GetPositionNumber(), name);
				DETHROW(deeInvalidFileFormat);
			}
			
		}else{
			logger.LogWarnFormat("%s(%i:%i): Unknown Tag %s, ignoring",
				root->GetName().GetString(), tag->GetLineNumber(),
				tag->GetPositionNumber(), tag->GetName().GetString());
		}
	}
	
	rule->SetMoves(moves);
	return rule;
}

aeRule::Ref aeLSAnimator::pLoadRuleBoneTransformator(decXmlElementTag *root, aeAnimator &animator){
	deLogger &logger = *pLSSys->GetWindowMain()->GetEnvironment().GetLogger();
	const aeRuleBoneTransformator::Ref rule(aeRuleBoneTransformator::Ref::New());
	decXmlElementTag *tag;
	decVector vector;
	const char *name;
	int i;
	
	for(i=0; i<root->GetElementCount(); i++){
		tag = root->GetElementIfTag(i);
		if(tag){
			if(!pLoadRuleCommon(tag, animator, rule)){
				if(strcmp(tag->GetName(), "minimumTranslation") == 0){
					vector.SetZero();
					pLoadVector(tag, vector);
					rule->SetMinimumTranslation(vector);
					
				}else if(strcmp(tag->GetName(), "maximumTranslation") == 0){
					vector.SetZero();
					pLoadVector(tag, vector);
					rule->SetMaximumTranslation(vector);
					
				}else if(strcmp(tag->GetName(), "minimumRotation") == 0){
					vector.SetZero();
					pLoadVector(tag, vector);
					rule->SetMinimumRotation(vector);
					
				}else if(strcmp(tag->GetName(), "maximumRotation") == 0){
					vector.SetZero();
					pLoadVector(tag, vector);
					rule->SetMaximumRotation(vector);
					
				}else if(strcmp(tag->GetName(), "minimumScaling") == 0){
					vector.SetZero();
					pLoadVector(tag, vector);
					rule->SetMinimumScaling(vector);
					
				}else if(strcmp(tag->GetName(), "maximumScaling") == 0){
					vector.SetZero();
					pLoadVector(tag, vector);
					rule->SetMaximumScaling(vector);
					
				}else if(strcmp(tag->GetName(), "axis") == 0){
					vector.SetZero();
					pLoadVector(tag, vector);
					rule->SetAxis(vector);
					
				}else if(strcmp(tag->GetName(), "minimumAngle") == 0){
					rule->SetMinimumAngle(GetCDataFloat(*tag));
					
				}else if(strcmp(tag->GetName(), "maximumAngle") == 0){
					rule->SetMaximumAngle(GetCDataFloat(*tag));
					
				}else if(strcmp(tag->GetName(), "cframe") == 0){
					name = GetCDataString(*tag);
					
					if(strcmp(name, "local") == 0){
						rule->SetCoordinateFrame(deAnimatorRuleBoneTransformator::ecfBoneLocal);
						
					}else if(strcmp(name, "component") == 0){
						rule->SetCoordinateFrame(deAnimatorRuleBoneTransformator::ecfComponent);
						
					}else if(strcmp(name, "target") == 0){
						rule->SetCoordinateFrame(deAnimatorRuleBoneTransformator::ecfTargetBone);
						
					}else{
						logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown coordinate frame '%s'",
							root->GetName().GetString(), tag->GetLineNumber(),
							tag->GetPositionNumber(), name);
						DETHROW(deeInvalidFileFormat);
					}
					
				}else if(strcmp(tag->GetName(), "enablePosition") == 0){
					rule->SetEnablePosition(GetCDataBool(*tag));
					
				}else if(strcmp(tag->GetName(), "enableOrientation") == 0){
					rule->SetEnableOrientation(GetCDataBool(*tag));
					
				}else if(strcmp(tag->GetName(), "enableSize") == 0){
					rule->SetEnableSize(GetCDataBool(*tag));
					
				}else if(strcmp(tag->GetName(), "useAxis") == 0){
					rule->SetUseAxis(GetCDataBool(*tag));
					
				}else if(strcmp(tag->GetName(), "targetBone") == 0){
					rule->SetTargetBone(GetCDataString(*tag));
					
				}else if(tag->GetName() == "inputBone"){
					rule->SetInputBone(GetCDataString(*tag));
					
				}else if(tag->GetName() == "inputSource"){
					name = GetCDataString(*tag);
					
					if(strcmp(name, "targetBlend") == 0){
						rule->SetInputSource(deAnimatorRuleBoneTransformator::eisTargetBlend);
						
					}else if(strcmp(name, "targetDirect") == 0){
						rule->SetInputSource(deAnimatorRuleBoneTransformator::eisTargetDirect);
						
					}else if(strcmp(name, "boneState") == 0){
						rule->SetInputSource(deAnimatorRuleBoneTransformator::eisBoneState);
						
					}else if(strcmp(name, "boneStateInverse") == 0){
						rule->SetInputSource(deAnimatorRuleBoneTransformator::eisBoneStateInverse);
						
					}else{
						logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown coordinate frame '%s'",
							root->GetName().GetString(), tag->GetLineNumber(),
							tag->GetPositionNumber(), name);
						DETHROW(deeInvalidFileFormat);
					}
					
				}else if(strcmp(tag->GetName(), "target") == 0){
					name = pGetAttributeString(tag, "name");
					
					if(strcmp(name, "blendFactor") == 0){
						pLoadControllerTarget(tag, animator, rule->GetTargetBlendFactor());
						
					}else if(strcmp(name, "translation") == 0){
						pLoadControllerTarget(tag, animator, rule->GetTargetTranslation());
						
					}else if(strcmp(name, "rotation") == 0){
						pLoadControllerTarget(tag, animator, rule->GetTargetRotation());
						
					}else if(strcmp(name, "scaling") == 0){
						pLoadControllerTarget(tag, animator, rule->GetTargetScaling());
						
					}else{
						logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown target '%s'",
							root->GetName().GetString(), tag->GetLineNumber(),
							tag->GetPositionNumber(), name);
						DETHROW(deeInvalidFileFormat);
					}
					
				}else{
					logger.LogWarnFormat(LOGSOURCE, "%s(%i:%i): Unknown Tag %s, ignoring",
						root->GetName().GetString(), tag->GetLineNumber(),
						tag->GetPositionNumber(), tag->GetName().GetString());
				}
			}
		}
	}
	return rule;
}

aeRule::Ref aeLSAnimator::pLoadRuleStateManipulator(decXmlElementTag *root, aeAnimator &animator){
	deLogger &logger = *pLSSys->GetWindowMain()->GetEnvironment().GetLogger();
	const aeRuleStateManipulator::Ref rule(aeRuleStateManipulator::Ref::New());
	decXmlElementTag *tag;
	decVector vector;
	const char *name;
	int i;
	
	for(i=0; i<root->GetElementCount(); i++){
		tag = root->GetElementIfTag(i);
		if(tag){
			if(!pLoadRuleCommon(tag, animator, rule)){
				if(strcmp(tag->GetName(), "positionMinimum") == 0){
					vector.SetZero();
					pLoadVector(tag, vector);
					rule->SetMinimumPosition(vector);
					
				}else if(strcmp(tag->GetName(), "positionMaximum") == 0){
					vector.SetZero();
					pLoadVector(tag, vector);
					rule->SetMaximumPosition(vector);
					
				}else if(strcmp(tag->GetName(), "rotationMinimum") == 0){
					vector.SetZero();
					pLoadVector(tag, vector);
					rule->SetMinimumRotation(vector);
					
				}else if(strcmp(tag->GetName(), "rotationMaximum") == 0){
					vector.SetZero();
					pLoadVector(tag, vector);
					rule->SetMaximumRotation(vector);
					
				}else if(strcmp(tag->GetName(), "sizeMinimum") == 0){
					vector.SetZero();
					pLoadVector(tag, vector);
					rule->SetMinimumSize(vector);
					
				}else if(strcmp(tag->GetName(), "sizeMaximum") == 0){
					vector.SetZero();
					pLoadVector(tag, vector);
					rule->SetMaximumSize(vector);
					
				}else if(strcmp(tag->GetName(), "vertexPositionSetMinimum") == 0){
					rule->SetMinimumVertexPositionSet(GetCDataFloat(*tag));
					
				}else if(strcmp(tag->GetName(), "vertexPositionSetMaximum") == 0){
					rule->SetMaximumVertexPositionSet(GetCDataFloat(*tag));
					
				}else if(strcmp(tag->GetName(), "enablePosition") == 0){
					rule->SetEnablePosition(GetCDataBool(*tag));
					
				}else if(strcmp(tag->GetName(), "enableOrientation") == 0){
					rule->SetEnableRotation(GetCDataBool(*tag));
					
				}else if(strcmp(tag->GetName(), "enableSize") == 0){
					rule->SetEnableSize(GetCDataBool(*tag));
					
				}else if(strcmp(tag->GetName(), "enableVertexPositionSet") == 0){
					rule->SetEnableVertexPositionSet(GetCDataBool(*tag));
					
				}else if(strcmp(tag->GetName(), "target") == 0){
					name = pGetAttributeString(tag, "name");
					
					if(strcmp(name, "blendFactor") == 0){
						pLoadControllerTarget(tag, animator, rule->GetTargetBlendFactor());
						
					}else if(strcmp(name, "position") == 0){
						pLoadControllerTarget(tag, animator, rule->GetTargetPosition());
						
					}else if(strcmp(name, "orientation") == 0){
						pLoadControllerTarget(tag, animator, rule->GetTargetRotation());
						
					}else if(strcmp(name, "size") == 0){
						pLoadControllerTarget(tag, animator, rule->GetTargetSize());
						
					}else if(strcmp(name, "vertexPositionSet") == 0){
						pLoadControllerTarget(tag, animator, rule->GetTargetVertexPositionSet());
						
					}else{
						logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown target '%s'",
							root->GetName().GetString(), tag->GetLineNumber(),
							tag->GetPositionNumber(), name);
						DETHROW(deeInvalidFileFormat);
					}
					
				}else{
					logger.LogWarnFormat(LOGSOURCE, "%s(%i:%i): Unknown Tag %s, ignoring",
						root->GetName().GetString(), tag->GetLineNumber(),
						tag->GetPositionNumber(), tag->GetName().GetString());
				}
			}
		}
	}
	return rule;
}

aeRule::Ref aeLSAnimator::pLoadRuleStateSnapshot(decXmlElementTag *root, aeAnimator &animator){
	deLogger &logger = *pLSSys->GetWindowMain()->GetEnvironment().GetLogger();
	const aeRuleStateSnapshot::Ref rule(aeRuleStateSnapshot::Ref::New());
	decXmlElementTag *tag;
	decVector vector;
	const char *name;
	int i;
	
	for(i=0; i<root->GetElementCount(); i++){
		tag = root->GetElementIfTag(i);
		if(tag){
			if(!pLoadRuleCommon(tag, animator, rule)){
				if(strcmp(tag->GetName(), "useLastState") == 0){
					rule->SetUseLastState(GetCDataBool(*tag));
					
				}else if(strcmp(tag->GetName(), "enablePosition") == 0){
					rule->SetEnablePosition(GetCDataBool(*tag));
					
				}else if(strcmp(tag->GetName(), "enableOrientation") == 0){
					rule->SetEnableOrientation(GetCDataBool(*tag));
					
				}else if(strcmp(tag->GetName(), "enableSize") == 0){
					rule->SetEnableSize(GetCDataBool(*tag));
					
				}else if(strcmp(tag->GetName(), "enableVertexPositionSet") == 0){
					rule->SetEnableVertexPositionSet(GetCDataBool(*tag));
					
				}else if(strcmp(tag->GetName(), "target") == 0){
					name = pGetAttributeString(tag, "name");
					
					if(strcmp(name, "blendFactor") == 0){
						pLoadControllerTarget(tag, animator, rule->GetTargetBlendFactor());
						
					}else{
						logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown target '%s'",
							root->GetName().GetString(), tag->GetLineNumber(),
							tag->GetPositionNumber(), name);
						DETHROW(deeInvalidFileFormat);
					}
					
				}else{
					logger.LogWarnFormat(LOGSOURCE, "%s(%i:%i): Unknown Tag %s, ignoring",
						root->GetName().GetString(), tag->GetLineNumber(),
						tag->GetPositionNumber(), tag->GetName().GetString());
				}
			}
		}
	}
	return rule;
}

aeRule::Ref aeLSAnimator::pLoadRuleInverseKinematic(decXmlElementTag *root, aeAnimator &animator){
	deLogger &logger = *pLSSys->GetWindowMain()->GetEnvironment().GetLogger();
	const aeRuleInverseKinematic::Ref rule(aeRuleInverseKinematic::Ref::New());
	decXmlCharacterData *cdata;
	decXmlElementTag *tag;
	decVector vector;
	const char *name;
	int i;
	
	for(i=0; i<root->GetElementCount(); i++){
		tag = root->GetElementIfTag(i);
		if(tag){
			if(!pLoadRuleCommon(tag, animator, rule)){
				if(strcmp(tag->GetName(), "goalPosition") == 0){
					vector.SetZero();
					pLoadVector(tag, vector);
					rule->SetGoalPosition(vector);
					
				}else if(strcmp(tag->GetName(), "goalOrientation") == 0){
					vector.SetZero();
					pLoadVector(tag, vector);
					rule->SetGoalOrientation(vector);
					
				}else if(strcmp(tag->GetName(), "localPosition") == 0){
					vector.SetZero();
					pLoadVector(tag, vector);
					rule->SetLocalPosition(vector);
					
				}else if(strcmp(tag->GetName(), "localOrientation") == 0){
					vector.SetZero();
					pLoadVector(tag, vector);
					rule->SetLocalOrientation(vector);
					
				}else if(strcmp(tag->GetName(), "adjustOrientation") == 0){
					rule->SetAdjustOrientation(GetCDataBool(*tag));
					
				}else if(strcmp(tag->GetName(), "position") == 0){ // deprecated
					vector.SetZero();
					pLoadVector(tag, vector);
					rule->SetGoalPosition(vector);
					
				}else if(strcmp(tag->GetName(), "orientation") == 0){ // deprecated
					vector.SetZero();
					pLoadVector(tag, vector);
					rule->SetGoalOrientation(vector);
					
				}else if(strcmp(tag->GetName(), "offset") == 0){ // deprecated
					vector.SetZero();
					pLoadVector(tag, vector);
					rule->SetLocalPosition(vector);
					
				}else if(strcmp(tag->GetName(), "solverBone") == 0){
					cdata = tag->GetFirstData();
					
					if(cdata){
						rule->SetSolverBone(cdata->GetData());
						
					}else{
						rule->SetSolverBone("");
					}
					
				}else if(strcmp(tag->GetName(), "useSolverBone") == 0){
					rule->SetUseSolverBone(GetCDataBool(*tag));
					
				}else if(strcmp(tag->GetName(), "reachRange") == 0){
					rule->SetReachRange(strtof(GetCDataString(*tag), nullptr));
					
				}else if(strcmp(tag->GetName(), "reachBone") == 0){
					cdata = tag->GetFirstData();
					
					if(cdata){
						rule->SetReachBone(cdata->GetData());
						
					}else{
						rule->SetReachBone("");
					}
					
				}else if(strcmp(tag->GetName(), "reachCenter") == 0){
					vector.SetZero();
					pLoadVector(tag, vector);
					rule->SetReachCenter(vector);
					
				}else if(strcmp(tag->GetName(), "target") == 0){
					name = pGetAttributeString(tag, "name");
					
					if(strcmp(name, "blendFactor") == 0){
						pLoadControllerTarget(tag, animator, rule->GetTargetBlendFactor());
						
					}else if(strcmp(name, "goalPosition") == 0){
						pLoadControllerTarget(tag, animator, rule->GetTargetGoalPosition());
						
					}else if(strcmp(name, "goalOrientation") == 0){
						pLoadControllerTarget(tag, animator, rule->GetTargetGoalOrientation());
						
					}else if(strcmp(name, "localPosition") == 0){
						pLoadControllerTarget(tag, animator, rule->GetTargetLocalPosition());
						
					}else if(strcmp(name, "localOrientation") == 0){
						pLoadControllerTarget(tag, animator, rule->GetTargetLocalOrientation());
						
					}else if(strcmp(name, "reachRange") == 0){
						pLoadControllerTarget(tag, animator, rule->GetTargetReachRange());
						
					}else if(strcmp(name, "reachCenter") == 0){
						pLoadControllerTarget(tag, animator, rule->GetTargetReachCenter());
						
					}else if(strcmp(name, "position") == 0){ // deprecated
						pLoadControllerTarget(tag, animator, rule->GetTargetGoalPosition());
						
					}else if(strcmp(name, "orientation") == 0){ // deprecated
						pLoadControllerTarget(tag, animator, rule->GetTargetGoalOrientation());
						
					}else{
						logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown target '%s'",
							root->GetName().GetString(), tag->GetLineNumber(),
							tag->GetPositionNumber(), name);
						DETHROW(deeInvalidFileFormat);
					}
					
				}else{
					logger.LogWarnFormat(LOGSOURCE, "%s(%i:%i): Unknown Tag %s, ignoring",
						root->GetName().GetString(), tag->GetLineNumber(),
						tag->GetPositionNumber(), tag->GetName().GetString());
				}
			}
		}
	}
	return rule;
}

aeRule::Ref aeLSAnimator::pLoadRuleForeignState(decXmlElementTag *root, aeAnimator &animator){
	deLogger &logger = *pLSSys->GetWindowMain()->GetEnvironment().GetLogger();
	const aeRuleForeignState::Ref rule(aeRuleForeignState::Ref::New());
	decXmlElementTag *tag;
	const char *name;
	int i;
	
	for(i=0; i<root->GetElementCount(); i++){
		tag = root->GetElementIfTag(i);
		if(tag){
			if(!pLoadRuleCommon(tag, animator, rule)){
				if(strcmp(tag->GetName(), "scalePosition") == 0){
					rule->SetScalePosition(GetCDataFloat(*tag));
					
				}else if(strcmp(tag->GetName(), "scaleOrientation") == 0){
					rule->SetScaleOrientation(GetCDataFloat(*tag));
					
				}else if(strcmp(tag->GetName(), "scaleSize") == 0){
					rule->SetScaleSize(GetCDataFloat(*tag));
					
				}else if(strcmp(tag->GetName(), "scaleVertexPositionSet") == 0){
					rule->SetScaleVertexPositionSet(GetCDataFloat(*tag));
					
				}else if(strcmp(tag->GetName(), "enablePosition") == 0){
					rule->SetEnablePosition(GetCDataBool(*tag));
					
				}else if(strcmp(tag->GetName(), "enableOrientation") == 0){
					rule->SetEnableOrientation(GetCDataBool(*tag));
					
				}else if(strcmp(tag->GetName(), "enableSize") == 0){
					rule->SetEnableSize(GetCDataBool(*tag));
					
				}else if(strcmp(tag->GetName(), "enableVertexPositionSet") == 0){
					rule->SetEnableVertexPositionSet(GetCDataBool(*tag));
					
				}else if(strcmp(tag->GetName(), "foreignBone") == 0){
					rule->SetForeignBone(GetCDataString(*tag));
					
				}else if(strcmp(tag->GetName(), "foreignVertexPositionSet") == 0){
					rule->SetForeignVertexPositionSet(GetCDataString(*tag));
					
				}else if(strcmp(tag->GetName(), "srcCFrame") == 0){
					name = GetCDataString(*tag);
					
					if(strcmp(name, "local") == 0){
						rule->SetSourceCoordinateFrame(deAnimatorRuleForeignState::ecfBoneLocal);
						
					}else if(strcmp(name, "component") == 0){
						rule->SetSourceCoordinateFrame(deAnimatorRuleForeignState::ecfComponent);
						
					}else{
						logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown coordinate frame '%s'",
							root->GetName().GetString(), tag->GetLineNumber(),
							tag->GetPositionNumber(), name);
						DETHROW(deeInvalidFileFormat);
					}
					
				}else if(strcmp(tag->GetName(), "destCFrame") == 0){
					name = GetCDataString(*tag);
					
					if(strcmp(name, "local") == 0){
						rule->SetDestCoordinateFrame(deAnimatorRuleForeignState::ecfBoneLocal);
						
					}else if(strcmp(name, "component") == 0){
						rule->SetDestCoordinateFrame(deAnimatorRuleForeignState::ecfComponent);
						
					}else{
						logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown coordinate frame '%s'",
							root->GetName().GetString(), tag->GetLineNumber(),
							tag->GetPositionNumber(), name);
						DETHROW(deeInvalidFileFormat);
					}
					
				}else if(strcmp(tag->GetName(), "target") == 0){
					name = pGetAttributeString(tag, "name");
					
					if(strcmp(name, "blendFactor") == 0){
						pLoadControllerTarget(tag, animator, rule->GetTargetBlendFactor());
						
					}else if(strcmp(name, "position") == 0){
						pLoadControllerTarget(tag, animator, rule->GetTargetPosition());
						
					}else if(strcmp(name, "orientation") == 0){
						pLoadControllerTarget(tag, animator, rule->GetTargetOrientation());
						
					}else if(strcmp(name, "size") == 0){
						pLoadControllerTarget(tag, animator, rule->GetTargetSize());
						
					}else if(strcmp(name, "vertexPositionSet") == 0){
						pLoadControllerTarget(tag, animator, rule->GetTargetVertexPositionSet());
						
					}else{
						logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown target '%s'",
							root->GetName().GetString(), tag->GetLineNumber(),
							tag->GetPositionNumber(), name);
						DETHROW(deeInvalidFileFormat);
					}
					
				}else{
					logger.LogWarnFormat(LOGSOURCE, "%s(%i:%i): Unknown Tag %s, ignoring",
						root->GetName().GetString(), tag->GetLineNumber(),
						tag->GetPositionNumber(), tag->GetName().GetString());
				}
			}
		}
	}
	return rule;
}

aeRule::Ref aeLSAnimator::pLoadRuleMirror(decXmlElementTag *root, aeAnimator &animator){
	deLogger &logger = *pLSSys->GetWindowMain()->GetEnvironment().GetLogger();
	const aeRuleMirror::Ref rule(aeRuleMirror::Ref::New());
	int i;
	
	for(i=0; i<root->GetElementCount(); i++){
		decXmlElementTag * const tag = root->GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		if(pLoadRuleCommon(tag, animator, rule)){
			continue;
		}
		
		if(tag->GetName() == "enablePosition"){
			rule->SetEnablePosition(GetCDataBool(*tag));
			
		}else if(tag->GetName() == "enableOrientation"){
			rule->SetEnableOrientation(GetCDataBool(*tag));
			
		}else if(tag->GetName() == "enableSize"){
			rule->SetEnableSize(GetCDataBool(*tag));
			
		}else if(tag->GetName() == "enableVertexPositionSet"){
			rule->SetEnableVertexPositionSet(GetCDataBool(*tag));
			
		}else if(tag->GetName() == "mirrorAxis"){
			const decString &name = GetCDataString(*tag);
			
			if(name == "x"){
				rule->SetMirrorAxis(deAnimatorRuleMirror::emaX);
				
			}else if(name == "y"){
				rule->SetMirrorAxis(deAnimatorRuleMirror::emaY);
				
			}else if(name == "z"){
				rule->SetMirrorAxis(deAnimatorRuleMirror::emaZ);
				
			}else{
				logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown mirror axis '%s'",
					root->GetName().GetString(), tag->GetLineNumber(),
					tag->GetPositionNumber(), name.GetString());
				DETHROW(deeInvalidFileFormat);
			}
			
		}else if(tag->GetName() == "mirrorBone"){
			rule->SetMirrorBone(GetCDataString(*tag));
			
		}else if(tag->GetName() == "matchName"){
			const decString &first = GetAttributeString(*tag, "first");
			const decString &second = GetAttributeString(*tag, "second");
			const decString &strType = GetAttributeString(*tag, "type");
			deAnimatorRuleMirror::eMatchNameType type;
			
			if(strType == "first"){
				type = deAnimatorRuleMirror::emntFirst;
				
			}else if(strType == "last"){
				type = deAnimatorRuleMirror::emntLast;
				
			}else if(strType == "middle"){
				type = deAnimatorRuleMirror::emntMiddle;
				
			}else{
				logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown match name type '%s'",
					root->GetName().GetString(), tag->GetLineNumber(),
					tag->GetPositionNumber(), strType.GetString());
				DETHROW(deeInvalidFileFormat);
			}
			
			rule->AddMatchName(aeRuleMirror::cMatchName::Ref::New(first, second, type));
			
		}else if(strcmp(tag->GetName(), "target") == 0){
			const decString &name = pGetAttributeString(tag, "name");
			
			if(name == "blendFactor"){
				pLoadControllerTarget(tag, animator, rule->GetTargetBlendFactor());
				
			}else{
				logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown target '%s'",
					root->GetName().GetString(), tag->GetLineNumber(),
					tag->GetPositionNumber(), name.GetString());
				DETHROW(deeInvalidFileFormat);
			}
			
		}else{
			logger.LogWarnFormat(LOGSOURCE, "%s(%i:%i): Unknown Tag %s, ignoring",
				root->GetName().GetString(), tag->GetLineNumber(),
				tag->GetPositionNumber(), tag->GetName().GetString());
		}
	}
	return rule;
}

aeRule::Ref aeLSAnimator::pLoadRuleGroup(decXmlElementTag *root, aeAnimator &animator){
	deLogger &logger = *pLSSys->GetWindowMain()->GetEnvironment().GetLogger();
	const int elementCount = root->GetElementCount();
	const aeRuleGroup::Ref rule(aeRuleGroup::Ref::New());
	int i;
	
	for(i=0; i<elementCount; i++){
		decXmlElementTag * const tag = root->GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		if(pLoadRuleCommon(tag, animator, rule)){
			continue;
		}
		
		if(strcmp(tag->GetName(), "applicationType") == 0){
			const char * const name = GetCDataString(*tag);
			
			if(strcmp(name, "all") == 0){
				rule->SetApplicationType(deAnimatorRuleGroup::eatAll);
				
			}else if(strcmp(name, "selection") == 0){
				rule->SetApplicationType(deAnimatorRuleGroup::eatSelect);
				
			}else{
				logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown type '%s'",
					root->GetName().GetString(), tag->GetLineNumber(),
					tag->GetPositionNumber(), name);
				DETHROW(deeInvalidFileFormat);
			}
			
		}else if(strcmp(tag->GetName(), "useCurrentState") == 0){
			rule->SetUseCurrentState(GetCDataBool(*tag));
			
		}else if(strcmp(tag->GetName(), "enablePosition") == 0){
			rule->SetEnablePosition(GetCDataBool(*tag));
			
		}else if(strcmp(tag->GetName(), "enableOrientation") == 0){
			rule->SetEnableOrientation(GetCDataBool(*tag));
			
		}else if(strcmp(tag->GetName(), "enableSize") == 0){
			rule->SetEnableSize(GetCDataBool(*tag));
			
		}else if(strcmp(tag->GetName(), "enableVertexPositionSet") == 0){
			rule->SetEnableVertexPositionSet(GetCDataBool(*tag));
			
		}else if(strcmp(tag->GetName(), "target") == 0){
			const char * const name = pGetAttributeString(tag, "name");
			
			if(strcmp(name, "blendFactor") == 0){
				pLoadControllerTarget(tag, animator, rule->GetTargetBlendFactor());
				
			}else if(strcmp(name, "selection") == 0){
				pLoadControllerTarget(tag, animator, rule->GetTargetSelect());
				
			}else{
				logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown target '%s'",
					root->GetName().GetString(), tag->GetLineNumber(),
					tag->GetPositionNumber(), name);
				DETHROW(deeInvalidFileFormat);
			}
			
		}else{
			aeRule * const srule = pLoadRule(tag, animator);
			
			if(srule){
				rule->AddRule(srule);
				srule->FreeReference();
				
			}else{
				logger.LogWarnFormat(LOGSOURCE, "%s(%i:%i): Unknown Tag %s, ignoring",
					root->GetName().GetString(), tag->GetLineNumber(),
					tag->GetPositionNumber(), tag->GetName().GetString());
			}
		}
	}
	return rule;
}

aeRule::Ref aeLSAnimator::pLoadRuleSubAnimator(decXmlElementTag *root, aeAnimator &animator){
	deLogger &logger = *pLSSys->GetWindowMain()->GetEnvironment().GetLogger();
	const aeRuleSubAnimator::Ref rule(aeRuleSubAnimator::Ref::New());
	decXmlCharacterData *cdata;
	decXmlElementTag *tag;
	const char *name;
	int i;
	
	for(i=0; i<root->GetElementCount(); i++){
		tag = root->GetElementIfTag(i);
		if(tag){
			if(strcmp(tag->GetName(), "pathAnimator") == 0){
				cdata = tag->GetFirstData();
				
				if(cdata){
					rule->SetPathSubAnimator(cdata->GetData());
					
				}else{
					rule->SetPathSubAnimator("");
				}
				
			}else if(strcmp(tag->GetName(), "connection") == 0){
				const int controller = pGetAttributeInt(tag, "controller");
				const decString target(GetCDataString(*tag));
				if(target.IsEmpty() || !rule->GetSubAnimator()){
					continue;
				}
				
				const int targetIndex = rule->GetSubAnimator()->IndexOfControllerNamed(target);
				if(targetIndex == -1){
					continue;
				}
				
				rule->SetControllerAt(targetIndex, animator.GetControllers().GetAt(controller));
				
			}else if(strcmp(tag->GetName(), "enablePosition") == 0){
				rule->SetEnablePosition(GetCDataBool(*tag));
				
			}else if(strcmp(tag->GetName(), "enableOrientation") == 0){
				rule->SetEnableOrientation(GetCDataBool(*tag));
				
			}else if(strcmp(tag->GetName(), "enableSize") == 0){
				rule->SetEnableSize(GetCDataBool(*tag));
				
			}else if(strcmp(tag->GetName(), "enableVertexPositionSet") == 0){
				rule->SetEnableVertexPositionSet(GetCDataBool(*tag));
				
			}else if(strcmp(tag->GetName(), "target") == 0){
				name = pGetAttributeString(tag, "name");
				
				if(strcmp(name, "blendFactor") == 0){
					pLoadControllerTarget(tag, animator, rule->GetTargetBlendFactor());
					
				}else{
					logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown target '%s'",
						root->GetName().GetString(), tag->GetLineNumber(),
						tag->GetPositionNumber(), name);
					DETHROW(deeInvalidFileFormat);
				}
				
			}else if(!pLoadRuleCommon(tag, animator, rule)){
				logger.LogWarnFormat(LOGSOURCE, "%s(%i:%i): Unknown Tag %s, ignoring",
					root->GetName().GetString(), tag->GetLineNumber(),
					tag->GetPositionNumber(), tag->GetName().GetString());
			}
		}
	}
	return rule;
}

aeRule::Ref aeLSAnimator::pLoadRuleTrackTo(decXmlElementTag *root, aeAnimator &animator){
	deLogger &logger = *pLSSys->GetWindowMain()->GetEnvironment().GetLogger();
	const aeRuleTrackTo::Ref rule(aeRuleTrackTo::Ref::New());
	decXmlElementTag *tag;
	const char *name;
	int i;
	
	for(i=0; i<root->GetElementCount(); i++){
		tag = root->GetElementIfTag(i);
		if(tag){
			if(!pLoadRuleCommon(tag, animator, rule)){
				if(strcmp(tag->GetName(), "trackBone") == 0){
					if(tag->GetFirstData()){
						rule->SetTrackBone(GetCDataString(*tag));
					}
					
				}else if(strcmp(tag->GetName(), "trackAxis") == 0){
					name = GetCDataString(*tag);
					
					if(strcmp(name, "posX") == 0){
						rule->SetTrackAxis(deAnimatorRuleTrackTo::etaPosX);
						
					}else if(strcmp(name, "posY") == 0){
						rule->SetTrackAxis(deAnimatorRuleTrackTo::etaPosY);
						
					}else if(strcmp(name, "posZ") == 0){
						rule->SetTrackAxis(deAnimatorRuleTrackTo::etaPosZ);
						
					}else if(strcmp(name, "negX") == 0){
						rule->SetTrackAxis(deAnimatorRuleTrackTo::etaNegX);
						
					}else if(strcmp(name, "negY") == 0){
						rule->SetTrackAxis(deAnimatorRuleTrackTo::etaNegY);
						
					}else if(strcmp(name, "negZ") == 0){
						rule->SetTrackAxis(deAnimatorRuleTrackTo::etaNegZ);
						
					}else{
						logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown value '%s'",
							root->GetName().GetString(), tag->GetLineNumber(),
							tag->GetPositionNumber(), name);
						DETHROW(deeInvalidFileFormat);
					}
					
				}else if(strcmp(tag->GetName(), "upAxis") == 0){
					name = GetCDataString(*tag);
					
					if(strcmp(name, "posX") == 0){
						rule->SetUpAxis(deAnimatorRuleTrackTo::etaPosX);
						
					}else if(strcmp(name, "posY") == 0){
						rule->SetUpAxis(deAnimatorRuleTrackTo::etaPosY);
						
					}else if(strcmp(name, "posZ") == 0){
						rule->SetUpAxis(deAnimatorRuleTrackTo::etaPosZ);
						
					}else if(strcmp(name, "negX") == 0){
						rule->SetUpAxis(deAnimatorRuleTrackTo::etaNegX);
						
					}else if(strcmp(name, "negY") == 0){
						rule->SetUpAxis(deAnimatorRuleTrackTo::etaNegY);
						
					}else if(strcmp(name, "negZ") == 0){
						rule->SetUpAxis(deAnimatorRuleTrackTo::etaNegZ);
						
					}else{
						logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown value '%s'",
							root->GetName().GetString(), tag->GetLineNumber(),
							tag->GetPositionNumber(), name);
						DETHROW(deeInvalidFileFormat);
					}
					
				}else if(strcmp(tag->GetName(), "lockedAxis") == 0){
					name = GetCDataString(*tag);
					
					if(strcmp(name, "none") == 0){
						rule->SetLockedAxis(deAnimatorRuleTrackTo::elaNone);
						
					}else if(strcmp(name, "x") == 0){
						rule->SetLockedAxis(deAnimatorRuleTrackTo::elaX);
						
					}else if(strcmp(name, "y") == 0){
						rule->SetLockedAxis(deAnimatorRuleTrackTo::elaY);
						
					}else if(strcmp(name, "z") == 0){
						rule->SetLockedAxis(deAnimatorRuleTrackTo::elaZ);
						
					}else{
						logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown value '%s'",
							root->GetName().GetString(), tag->GetLineNumber(),
							tag->GetPositionNumber(), name);
						DETHROW(deeInvalidFileFormat);
					}
					
				}else if(strcmp(tag->GetName(), "upTarget") == 0){
					name = GetCDataString(*tag);
					
					if(strcmp(name, "worldX") == 0){
						rule->SetUpTarget(deAnimatorRuleTrackTo::eutWorldX);
						
					}else if(strcmp(name, "worldY") == 0){
						rule->SetUpTarget(deAnimatorRuleTrackTo::eutWorldY);
						
					}else if(strcmp(name, "worldZ") == 0){
						rule->SetUpTarget(deAnimatorRuleTrackTo::eutWorldZ);
						
					}else if(strcmp(name, "componentX") == 0){
						rule->SetUpTarget(deAnimatorRuleTrackTo::eutComponentX);
						
					}else if(strcmp(name, "componentY") == 0){
						rule->SetUpTarget(deAnimatorRuleTrackTo::eutComponentY);
						
					}else if(strcmp(name, "componentZ") == 0){
						rule->SetUpTarget(deAnimatorRuleTrackTo::eutComponentZ);
						
					}else if(strcmp(name, "trackBoneX") == 0){
						rule->SetUpTarget(deAnimatorRuleTrackTo::eutTrackBoneX);
						
					}else if(strcmp(name, "trackBoneY") == 0){
						rule->SetUpTarget(deAnimatorRuleTrackTo::eutTrackBoneY);
						
					}else if(strcmp(name, "trackBoneZ") == 0){
						rule->SetUpTarget(deAnimatorRuleTrackTo::eutTrackBoneZ);
						
					}else if(strcmp(name, "controller") == 0){
						rule->SetUpTarget(deAnimatorRuleTrackTo::eutController);
						
					}else{
						logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown value '%s'",
							root->GetName().GetString(), tag->GetLineNumber(),
							tag->GetPositionNumber(), name);
						DETHROW(deeInvalidFileFormat);
					}
					
				}else if(strcmp(tag->GetName(), "target") == 0){
					name = pGetAttributeString(tag, "name");
					
					if(strcmp(name, "blendFactor") == 0){
						pLoadControllerTarget(tag, animator, rule->GetTargetBlendFactor());
						
					}else if(strcmp(name, "position") == 0){
						pLoadControllerTarget(tag, animator, rule->GetTargetPosition());
						
					}else if(strcmp(name, "up") == 0){
						pLoadControllerTarget(tag, animator, rule->GetTargetUp());
						
					}else{
						logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown target '%s'",
							root->GetName().GetString(), tag->GetLineNumber(),
							tag->GetPositionNumber(), name);
						DETHROW(deeInvalidFileFormat);
					}
					
				}else{
					logger.LogWarnFormat(LOGSOURCE, "%s(%i:%i): Unknown Tag %s, ignoring",
						root->GetName().GetString(), tag->GetLineNumber(),
						tag->GetPositionNumber(), tag->GetName().GetString());
				}
			}
		}
	}
	return rule;
}

aeRule::Ref aeLSAnimator::pLoadRuleLimit(decXmlElementTag *root, aeAnimator &animator){
	deLogger &logger = *pLSSys->GetWindowMain()->GetEnvironment().GetLogger();
	const aeRuleLimit::Ref rule(aeRuleLimit::Ref::New());
	decXmlElementTag *tag;
	decVector vector;
	const char *name;
	int i;
	
	for(i=0; i<root->GetElementCount(); i++){
		tag = root->GetElementIfTag(i);
		if(tag){
			if(!pLoadRuleCommon(tag, animator, rule)){
				if(strcmp(tag->GetName(), "minimumPosition") == 0){
					vector.SetZero();
					pLoadVector(tag, vector);
					rule->SetMinimumPosition(vector);
					
				}else if(strcmp(tag->GetName(), "maximumPosition") == 0){
					vector.SetZero();
					pLoadVector(tag, vector);
					rule->SetMaximumPosition(vector);
					
				}else if(strcmp(tag->GetName(), "minimumRotation") == 0){
					vector.SetZero();
					pLoadVector(tag, vector);
					rule->SetMinimumRotation(vector);
					
				}else if(strcmp(tag->GetName(), "maximumRotation") == 0){
					vector.SetZero();
					pLoadVector(tag, vector);
					rule->SetMaximumRotation(vector);
					
				}else if(strcmp(tag->GetName(), "minimumScaling") == 0){
					vector.SetZero();
					pLoadVector(tag, vector);
					rule->SetMinimumScaling(vector);
					
				}else if(strcmp(tag->GetName(), "maximumScaling") == 0){
					vector.SetZero();
					pLoadVector(tag, vector);
					rule->SetMaximumScaling(vector);
					
				}else if(strcmp(tag->GetName(), "minimumVertexPositionSet") == 0){
					rule->SetMinimumVertexPositionSet(GetCDataFloat(*tag));
					
				}else if(strcmp(tag->GetName(), "maximumVertexPositionSet") == 0){
					rule->SetMaximumVertexPositionSet(GetCDataFloat(*tag));
					
				}else if(strcmp(tag->GetName(), "cframe") == 0){
					name = GetCDataString(*tag);
					
					if(strcmp(name, "local") == 0){
						rule->SetCoordinateFrame(deAnimatorRuleLimit::ecfBoneLocal);
						
					}else if(strcmp(name, "component") == 0){
						rule->SetCoordinateFrame(deAnimatorRuleLimit::ecfComponent);
						
					}else if(strcmp(name, "target") == 0){
						rule->SetCoordinateFrame(deAnimatorRuleLimit::ecfTargetBone);
						
					}else{
						logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown coordinate frame '%s'",
							root->GetName().GetString(), tag->GetLineNumber(),
							tag->GetPositionNumber(), name);
						DETHROW(deeInvalidFileFormat);
					}
					
				}else if(strcmp(tag->GetName(), "enablePosMin") == 0){
					rule->SetEnablePositionXMin(pGetAttributeInt(tag, "x") != 0);
					rule->SetEnablePositionYMin(pGetAttributeInt(tag, "y") != 0);
					rule->SetEnablePositionZMin(pGetAttributeInt(tag, "z") != 0);
					
				}else if(strcmp(tag->GetName(), "enablePosMax") == 0){
					rule->SetEnablePositionXMax(pGetAttributeInt(tag, "x") != 0);
					rule->SetEnablePositionYMax(pGetAttributeInt(tag, "y") != 0);
					rule->SetEnablePositionZMax(pGetAttributeInt(tag, "z") != 0);
					
				}else if(strcmp(tag->GetName(), "enableRotMin") == 0){
					rule->SetEnableRotationXMin(pGetAttributeInt(tag, "x") != 0);
					rule->SetEnableRotationYMin(pGetAttributeInt(tag, "y") != 0);
					rule->SetEnableRotationZMin(pGetAttributeInt(tag, "z") != 0);
					
				}else if(strcmp(tag->GetName(), "enableRotMax") == 0){
					rule->SetEnableRotationXMax(pGetAttributeInt(tag, "x") != 0);
					rule->SetEnableRotationYMax(pGetAttributeInt(tag, "y") != 0);
					rule->SetEnableRotationZMax(pGetAttributeInt(tag, "z") != 0);
					
				}else if(strcmp(tag->GetName(), "enableScaleMin") == 0){
					rule->SetEnableScalingXMin(pGetAttributeInt(tag, "x") != 0);
					rule->SetEnableScalingYMin(pGetAttributeInt(tag, "y") != 0);
					rule->SetEnableScalingZMin(pGetAttributeInt(tag, "z") != 0);
					
				}else if(strcmp(tag->GetName(), "enableScaleMax") == 0){
					rule->SetEnableScalingXMax(pGetAttributeInt(tag, "x") != 0);
					rule->SetEnableScalingYMax(pGetAttributeInt(tag, "y") != 0);
					rule->SetEnableScalingZMax(pGetAttributeInt(tag, "z") != 0);
					
				}else if(strcmp(tag->GetName(), "enableVertexPositionSetMin") == 0){
					rule->SetEnableVertexPositionSetMin(GetCDataBool(*tag));
					
				}else if(strcmp(tag->GetName(), "enableVertexPositionSetMax") == 0){
					rule->SetEnableVertexPositionSetMax(GetCDataBool(*tag));
					
				}else if(strcmp(tag->GetName(), "targetBone") == 0){
					rule->SetTargetBone(GetCDataString(*tag));
					
				}else if(strcmp(tag->GetName(), "target") == 0){
					name = pGetAttributeString(tag, "name");
					
					if(strcmp(name, "blendFactor") == 0){
						pLoadControllerTarget(tag, animator, rule->GetTargetBlendFactor());
						
					}else{
						logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown target '%s'",
							root->GetName().GetString(), tag->GetLineNumber(),
							tag->GetPositionNumber(), name);
						DETHROW(deeInvalidFileFormat);
					}
					
				}else{
					logger.LogWarnFormat(LOGSOURCE, "%s(%i:%i): Unknown Tag %s, ignoring",
						root->GetName().GetString(), tag->GetLineNumber(),
						tag->GetPositionNumber(), tag->GetName().GetString());
				}
			}
		}
	}
	return rule;
}

bool aeLSAnimator::pLoadRuleCommon(decXmlElementTag *tag, aeAnimator&, aeRule &rule){
	deLogger &logger = *pLSSys->GetWindowMain()->GetEnvironment().GetLogger();
	
	if(strcmp(tag->GetName(), "name") == 0){
		rule.SetName(GetCDataString(*tag));
		return true;
		
	}else if(strcmp(tag->GetName(), "enabled") == 0){
		rule.SetEnabled(GetCDataBool(*tag));
		return true;
		
	}else if(strcmp(tag->GetName(), "blendMode") == 0){
		decXmlCharacterData * const cdata = tag->GetFirstData();
		
		if(cdata){
			const char * const blendMode = cdata->GetData();
			
			if(strcmp(blendMode, "blend") == 0){
				rule.SetBlendMode(deAnimatorRule::ebmBlend);
				
			}else if(strcmp(blendMode, "overlay") == 0){
				rule.SetBlendMode(deAnimatorRule::ebmOverlay);
				
			}else{
				logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown value '%s'",
					tag->GetName().GetString(), tag->GetLineNumber(),
					tag->GetPositionNumber(), blendMode);
				DETHROW(deeInvalidFileFormat);
			}
			
		}else{
			logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown value ''",
				tag->GetName().GetString(), tag->GetLineNumber(), tag->GetPositionNumber());
			DETHROW(deeInvalidFileFormat);
		}
		
		return true;
		
	}else if(tag->GetName() == "blendFactor"){
		rule.SetBlendFactor(GetCDataFloat(*tag));
		return true;
		
	}else if(tag->GetName() == "invertBlendFactor"){
		rule.SetInvertBlendFactor(GetCDataBool(*tag));
		return true;
		
	}else if(strcmp(tag->GetName(), "bone") == 0){
		rule.AddBone(GetCDataString(*tag));
		return true;
		
	}else if(strcmp(tag->GetName(), "vertexPositionSet") == 0){
		rule.AddVertexPositionSet(GetCDataString(*tag));
		return true;
	}
	
	return false;
}

void aeLSAnimator::pLoadControllerTarget(decXmlElementTag *root, aeAnimator &animator, aeControllerTarget &target){
	deLogger &logger = *pLSSys->GetWindowMain()->GetEnvironment().GetLogger();
	decXmlElementTag *tag;
	int i;
	
	// parse tag
	for(i=0; i<root->GetElementCount(); i++){
		tag = root->GetElementIfTag(i);
		if(tag){
			if(strcmp(tag->GetName(), "link") == 0){
				target.AddLink(animator.GetLinks().GetAt(GetCDataInt(*tag)));
				
			}else{
				logger.LogWarnFormat(LOGSOURCE, "%s(%i:%i): Unknown Tag %s, ignoring",
					root->GetName().GetString(), tag->GetLineNumber(),
					tag->GetPositionNumber(), tag->GetName().GetString());
			}
		}
	}
}

void aeLSAnimator::pLoadVector(decXmlElementTag *root, decVector &vector){
	deLogger &logger = *pLSSys->GetWindowMain()->GetEnvironment().GetLogger();
	decXmlElementTag *tag;
	int i;
	
	// parse by attributes if one is found
	if(pHasAttribute(root, "x")){
		vector.x = pGetAttributeFloat(root, "x");
		vector.y = pGetAttributeFloat(root, "y");
		vector.z = pGetAttributeFloat(root, "z");
		
	// otherwise parse by tag ( depracted )
	}else{
		for(i=0; i<root->GetElementCount(); i++){
			tag = root->GetElementIfTag(i);
			if(tag){
				if(strcmp(tag->GetName(), "x") == 0){
					vector.x = strtof(GetCDataString(*tag), nullptr);
					
				}else if(strcmp(tag->GetName(), "y") == 0){
					vector.y = strtof(GetCDataString(*tag), nullptr);
					
				}else if(strcmp(tag->GetName(), "z") == 0){
					vector.z = strtof(GetCDataString(*tag), nullptr);
					
				}else{
					logger.LogWarnFormat(LOGSOURCE, "%s(%i:%i): Unknown Tag %s, ignoring",
						root->GetName().GetString(), tag->GetLineNumber(),
						tag->GetPositionNumber(), tag->GetName().GetString());
				}
			}
		}
	}
}

void aeLSAnimator::pLoadSubAnimators(aeAnimator &animator){
	const int count = animator.GetRules().GetCount();
	int i;
	
	for(i=0; i<count; i++){
		aeRule &rule = *animator.GetRules().GetAt(i);
		
		switch(rule.GetType()){
		case deAnimatorRuleVisitorIdentify::ertSubAnimator:
			((aeRuleSubAnimator&)rule).LoadSubAnimator();
			break;
			
		case deAnimatorRuleVisitorIdentify::ertGroup:
			pLoadSubAnimators(animator, (aeRuleGroup&)rule);
			break;
			
		default:
			break;
		}
	}
}

void aeLSAnimator::pLoadSubAnimators(aeAnimator &animator, const aeRuleGroup &group){
	const int count = group.GetRules().GetCount();
	int i;
	
	for(i=0; i<count; i++){
		aeRule &rule = *group.GetRules().GetAt(i);
		
		switch(rule.GetType()){
		case deAnimatorRuleVisitorIdentify::ertSubAnimator:
			((aeRuleSubAnimator&)rule).LoadSubAnimator();
			break;
			
		case deAnimatorRuleVisitorIdentify::ertGroup:
			pLoadSubAnimators(animator, (aeRuleGroup&)rule);
			break;
			
		default:
			break;
		}
	}
}
