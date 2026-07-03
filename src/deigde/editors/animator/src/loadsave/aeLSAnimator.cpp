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
	
	decXmlWriter writer(file);
	
	// write header
	writer.WriteXMLDeclaration();
	writer.WriteOpeningTag("animator", false, true);
	
	// display section
	pSaveDisplay(writer, *animator);
	
	// animator properties
	writer.WriteDataTagString("rig", animator->mpRigPath.GetValue());
	writer.WriteDataTagString("animation", animator->mpAnimationPath.GetValue());
	
	// write bones
	animator->mpAffectedBones->Visit([&](const decString &bone){
		writer.WriteDataTagString("bone", bone);
	});
	
	// write vertex position sets
	animator->mpAffectedVps->Visit([&](const decString &vps){
		writer.WriteDataTagString("vertexPositionSet", vps);
	});
	
	// write controllers
	animator->mpControllers->Visit([&](const aeController &controller){
		writer.WriteNewline();
		pSaveController(writer, controller);
	});
	
	// write links
	animator->mpLinks->Visit([&](const aeLink &link){
		writer.WriteNewline();
		pSaveLink(writer, *animator, link);
	});
	
	// write rules
	animator->GetRules().Visit([&](const aeRule &rule){
		writer.WriteNewline();
		pSaveRule(writer, *animator, rule);
	});
	
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
	writer.WriteDataTagString("model", animator.mpDisplayModelPath.GetValue());
	
	if(saveExplainingComments) writer.WriteComment("Path to skin to use.");
	writer.WriteDataTagString("skin", animator.mpDisplaySkinPath.GetValue());
	
	if(saveExplainingComments) writer.WriteComment("Path to rig to use.");
	writer.WriteDataTagString("rig", animator.mpDisplayRigPath.GetValue());
	
//	if( saveExplainingComments ) writer.WriteComment( "Path to animation to use." );
//	writer.WriteDataTagString( "animation", animator.GetAnimationPath() );
	
	pSaveLocomotion(writer, animator);
	
	writer.WriteClosingTag("display");
}

void aeLSAnimator::pSaveLocomotion(decXmlWriter &writer, const aeAnimator &animator){
	const aeAnimatorLocomotion &locomotion = animator.GetLocomotion();
	int l, legCount = locomotion.GetLegs().GetCount();
	
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
	
	if(fabsf(locomotion.mpLimitLookDown - -90.0f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("limitLookDown", locomotion.mpLimitLookDown);
	}
	if(fabsf(locomotion.mpLimitLookUp - 90.0f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("limitLookUp", locomotion.mpLimitLookUp);
	}
	if(fabsf(locomotion.mpLimitLookLeft - -90.0f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("limitLookLeft", locomotion.mpLimitLookLeft);
	}
	if(fabsf(locomotion.mpLimitLookRight - 90.0f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("limitLookRight", locomotion.mpLimitLookRight);
	}
	
	if(fabsf(locomotion.mpAdjustTimeUpDown - 0.0f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("adjustTimeUpDown", locomotion.mpAdjustTimeUpDown);
	}
	if(fabsf(locomotion.mpAdjustTimeLeftRight - 0.0f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("adjustTimeLeftRight", locomotion.mpAdjustTimeLeftRight);
	}
	if(fabsf(locomotion.mpAdjustTimeStance - 0.5f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("adjustTimeStance", locomotion.mpAdjustTimeStance);
	}
	if(fabsf(locomotion.mpAdjustTimeTurnIP - 1.0f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("adjustTimeTurnIP", locomotion.mpAdjustTimeTurnIP);
	}
	if(fabsf(locomotion.mpAdjustTimeOrientation - 1.0f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("adjustTimeOrientation", locomotion.mpAdjustTimeOrientation);
	}
	if(fabsf(locomotion.mpAdjustTimeVelocity - 0.5f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("adjustTimeVelocity", locomotion.mpAdjustTimeVelocity);
	}
	
	if(fabsf(locomotion.mpSpeedWalk - 1.25f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("speedWalk", locomotion.mpSpeedWalk);
	}
	if(fabsf(locomotion.mpSpeedRun - 4.0f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("speedRun", locomotion.mpSpeedRun);
	}
	
	if(fabsf(locomotion.mpLegBlendTime) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("legBlendTime", locomotion.mpLegBlendTime);
	}
	if(locomotion.mpUseLegPairs != 1){
		writer.WriteDataTagInt("useLegPairs", locomotion.mpUseLegPairs);
	}
	
	for(l=0; l<legCount; l++){
		const aeAnimatorLocomotionLeg &leg = *locomotion.GetLegs().GetAt(l);
		const decVector &pdposStand = leg.mpPutDownPositionStand;
		const decVector &pdposWalk = leg.mpPutDownPositionWalk;
		const decVector &pdposRun = leg.mpPutDownPositionRun;
		
		const bool hasLiftOffTime = fabsf(leg.mpLiftOffTime) > FLOAT_SAFE_EPSILON;
		const bool hasPutDownTime = fabsf(leg.mpPutDownTime - 0.5f) > FLOAT_SAFE_EPSILON;
		const bool hasPdposStand = !pdposStand.IsZero();
		const bool hasPdposWalk = !pdposWalk.IsZero();
		const bool hasPdposRun = !pdposRun.IsZero();
		
		if(!hasLiftOffTime && !hasPutDownTime && !hasPdposStand && !hasPdposWalk && !hasPdposRun){
			continue;
		}
		
		writer.WriteOpeningTag("leg");
		
		//writer.WriteDataTagString( "name", leg.GetName() );
		if(hasLiftOffTime){
			writer.WriteDataTagFloat("liftOffTime", leg.mpLiftOffTime);
		}
		if(hasPutDownTime){
			writer.WriteDataTagFloat("putDownTime", leg.mpPutDownTime);
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
	
	writer.WriteDataTagString("name", controller.mpName.GetValue());
	
	writer.WriteOpeningTagStart("limits");
	writer.WriteAttributeFloat("min", controller.mpMinimumValue);
	writer.WriteAttributeFloat("max", controller.mpMaximumValue);
	writer.WriteOpeningTagEnd(true);
	
	if(controller.mpFrozen){
		writer.WriteDataTagBool("frozen", controller.mpFrozen);
	}
	if(!controller.mpClamp){
		writer.WriteDataTagBool("clamp", controller.mpClamp);
	}
	
	switch(controller.mpLocomotionAttribute){
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
		
	case aeAnimatorLocomotion::eaNone:
		break;
	}
	
	if(controller.mpLocomotionLeg > 0){
		writer.WriteDataTagInt("locomotionLeg", controller.mpLocomotionLeg);
	}
	
	switch(controller.mpVectorSimulation){
	case aeController::evsPosition:
		writer.WriteDataTagString("vectorSimulation", "position");
		break;
		
	case aeController::evsRotation:
		writer.WriteDataTagString("vectorSimulation", "rotation");
		break;
		
	case aeController::evsNone:
		break;
	}
	
	if(fabsf(controller.mpDefaultValue) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("value", controller.mpDefaultValue);
	}
	if(!controller.mpDefaultVector->IsZero()){
		WriteVector(writer, "vector", controller.mpDefaultVector);
	}
	
	writer.WriteClosingTag("controller");
}

void aeLSAnimator::pSaveLink(decXmlWriter &writer, const aeAnimator &animator, const aeLink &link){
	int controllerIndex = -1;
	
	if(link.mpController){
		controllerIndex = animator.GetControllers().IndexOf(link.mpController);
	}
	
	writer.WriteOpeningTag("link");
	
	writer.WriteDataTagString("name", link.mpName.GetValue());
	writer.WriteDataTagInt("controller", controllerIndex);
	if(link.mpRepeat != 1){
		writer.WriteDataTagInt("repeat", link.mpRepeat);
	}
	if(!link.mpBone->IsEmpty()){
		writer.WriteDataTagString("bone", link.mpBone.GetValue());
	}
	
	switch(link.mpBoneParameter){
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
	
	if(fabsf(link.mpBoneMinimum) > FLOAT_SAFE_EPSILON
	|| fabsf(link.mpBoneMaximum - 1.0f) > FLOAT_SAFE_EPSILON){
		writer.WriteOpeningTagStart("boneLimits");
		writer.WriteAttributeFloat("min", link.mpBoneMinimum);
		writer.WriteAttributeFloat("max", link.mpBoneMaximum);
		writer.WriteOpeningTagEnd(true);
	}
	
	if(!link.mpVertexPositionSet->IsEmpty()){
		writer.WriteDataTagString("vertexPositionSet", link.mpVertexPositionSet.GetValue());
	}
	
	if(fabsf(link.mpVpsMinimum) > FLOAT_SAFE_EPSILON
	|| fabsf(link.mpVpsMaximum - 1.0f) > FLOAT_SAFE_EPSILON){
		writer.WriteOpeningTagStart("vertexPositionSetLimits");
		writer.WriteAttributeFloat("min", link.mpVpsMinimum);
		writer.WriteAttributeFloat("max", link.mpVpsMaximum);
		writer.WriteOpeningTagEnd(true);
	}
	
	if(link.mpWrapY){
		writer.WriteDataTagBool("wrapY", link.mpWrapY.GetValue());
	}
	
	const decCurveBezier &curve = link.mpCurve;
	if(curve.GetInterpolationMode() != decCurveBezier::eimLinear || curve.GetPointCount() != 2
	|| !curve.GetPointAt(0).GetPoint().IsEqualTo(decVector2(0.0f, 0.0f))
	|| !curve.GetPointAt(1).GetPoint().IsEqualTo(decVector2(1.0f, 1.0f))){
		WriteCurveBezier(writer, "curve", link.mpCurve);
	}
	
	writer.WriteClosingTag("link");
}

void aeLSAnimator::pSaveRuleCommon(decXmlWriter &writer, const aeAnimator &animator, const aeRule &rule){
	writer.WriteDataTagString("name", rule.mpName.GetValue());
	if(!rule.GetEnabled()){
		writer.WriteDataTagBool("enabled", rule.mpEnabled);
	}
	
	if(rule.mpBlendMode == deAnimatorRule::ebmBlend){
		//writer.WriteDataTagString( "blendMode", "blend" ); // default value
		
	}else if(rule.mpBlendMode == deAnimatorRule::ebmOverlay){
		writer.WriteDataTagString("blendMode", "overlay");
		
	}else{
		DETHROW(deeInvalidParam);
	}
	
	if(fabsf(rule.mpBlendFactor - 1.0f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("blendFactor", rule.mpBlendFactor);
	}
	if(rule.mpInvertBlendFactor){
		writer.WriteDataTagBool("invertBlendFactor", rule.mpInvertBlendFactor);
	}
	
	pSaveControllerTarget(writer, animator, rule.mpTargetBlendFactor, "blendFactor");
	
	rule.mpAffectedBones->Visit([&](const decString &bone){
		writer.WriteDataTagString("bone", bone);
	});
	
	rule.mpAffectedVps->Visit([&](const decString &vps){
		writer.WriteDataTagString("vertexPositionSet", vps);
	});
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
const igdeMetaPropertyObjectSetStorage<aeLink>::Storage &target, const char *name){
	if(target->IsEmpty()){
		return;
	}
	
	writer.WriteOpeningTagStart("target");
	writer.WriteAttributeString("name", name);
	writer.WriteOpeningTagEnd();
	
	const auto &links = animator.mpLinks.GetValue();
	target->Visit([&](aeLink *link){
		writer.WriteDataTagInt("link", links.IndexOf(link));
	});
	
	writer.WriteClosingTag("target");
}

void aeLSAnimator::pSaveRuleAnimation(decXmlWriter &writer, const aeAnimator &animator,
const aeRuleAnimation &rule){
	writer.WriteOpeningTag("ruleAnimation");
	
	pSaveRuleCommon(writer, animator, rule);
	
	writer.WriteDataTagString("moveName", rule.mpMoveName.GetValue());
	writer.WriteDataTagFloat("moveTime", rule.mpMoveTime);
	
	if(!rule.GetEnablePosition()){
		writer.WriteDataTagBool("enablePosition", rule.mpEnablePosition);
	}
	if(!rule.GetEnableOrientation()){
		writer.WriteDataTagBool("enableOrientation", rule.mpEnableOrientation);
	}
	if(rule.GetEnableSize()){
		writer.WriteDataTagBool("enableSize", rule.mpEnableSize);
	}
	if(!rule.GetEnableVertexPositionSet()){
		writer.WriteDataTagBool("enableVertexPositionSet", rule.mpEnableVertexPositionSet);
	}
	
	pSaveControllerTarget(writer, animator, rule.mpTargetMoveTime, "moveTime");
	
	writer.WriteClosingTag("ruleAnimation");
}

void aeLSAnimator::pSaveRuleAnimationDifference(decXmlWriter &writer, const aeAnimator &animator,
const aeRuleAnimationDifference &rule){
	writer.WriteOpeningTag("ruleAnimationDifference");
	
	pSaveRuleCommon(writer, animator, rule);
	
	writer.WriteDataTagString("leadingMoveName", rule.mpLeadingMoveName.GetValue());
	writer.WriteDataTagFloat("leadingMoveTime", rule.mpLeadingMoveTime);
	writer.WriteDataTagString("referenceMoveName", rule.mpReferenceMoveName.GetValue());
	writer.WriteDataTagFloat("referenceMoveTime", rule.mpReferenceMoveTime);
	if(rule.mpUseComponentSpace){
		writer.WriteDataTagBool("useComponentSpace", true);
	}
	
	if(!rule.mpEnablePosition){
		writer.WriteDataTagBool("enablePosition", rule.mpEnablePosition);
	}
	if(!rule.mpEnableOrientation){
		writer.WriteDataTagBool("enableOrientation", rule.mpEnableOrientation);
	}
	if(rule.mpEnableSize){
		writer.WriteDataTagBool("enableSize", rule.mpEnableSize);
	}
	if(!rule.mpEnableVertexPositionSet){
		writer.WriteDataTagBool("enableVertexPositionSet", rule.mpEnableVertexPositionSet);
	}
	
	pSaveControllerTarget(writer, animator, rule.mpTargetLeadingMoveTime, "leadingMoveTime");
	pSaveControllerTarget(writer, animator, rule.mpTargetReferenceMoveTime, "referenceMoveTime");
	
	writer.WriteClosingTag("ruleAnimationDifference");
}

void aeLSAnimator::pSaveRuleAnimationSelect(decXmlWriter &writer, const aeAnimator &animator,
const aeRuleAnimationSelect &rule){
	writer.WriteOpeningTag("ruleAnimationSelect");
	
	pSaveRuleCommon(writer, animator, rule);
	
	rule.mpMoves->Visit([&](const decString &move){
		writer.WriteDataTagString("move", move);
	});
	
	if(!rule.mpEnablePosition){
		writer.WriteDataTagBool("enablePosition", rule.mpEnablePosition);
	}
	if(!rule.mpEnableOrientation){
		writer.WriteDataTagBool("enableOrientation", rule.mpEnableOrientation);
	}
	if(rule.mpEnableSize){
		writer.WriteDataTagBool("enableSize", rule.mpEnableSize);
	}
	if(!rule.mpEnableVertexPositionSet){
		writer.WriteDataTagBool("enableVertexPositionSet", rule.mpEnableVertexPositionSet);
	}
	
	pSaveControllerTarget(writer, animator, rule.mpTargetMoveTime, "moveTime");
	pSaveControllerTarget(writer, animator, rule.mpTargetSelect, "selection");
	
	writer.WriteClosingTag("ruleAnimationSelect");
}

void aeLSAnimator::pSaveRuleBoneTransformator(decXmlWriter &writer, const aeAnimator &animator,
const aeRuleBoneTransformator &rule){
	writer.WriteOpeningTag("ruleBoneTransformator");
	
	pSaveRuleCommon(writer, animator, rule);
	
	if(!rule.mpMinTranslation->IsZero()){
		writer.WriteOpeningTagStart("minimumTranslation");
		writer.WriteAttributeFloat("x", rule.mpMinTranslation->x);
		writer.WriteAttributeFloat("y", rule.mpMinTranslation->y);
		writer.WriteAttributeFloat("z", rule.mpMinTranslation->z);
		writer.WriteOpeningTagEnd(true);
	}
	
	if(!rule.mpMaxTranslation->IsZero()){
		writer.WriteOpeningTagStart("maximumTranslation");
		writer.WriteAttributeFloat("x", rule.mpMaxTranslation->x);
		writer.WriteAttributeFloat("y", rule.mpMaxTranslation->y);
		writer.WriteAttributeFloat("z", rule.mpMaxTranslation->z);
		writer.WriteOpeningTagEnd(true);
	}
	
	if(!rule.mpMinRotation.GetEulerAngles().IsZero()){
		writer.WriteOpeningTagStart("minimumRotation");
		writer.WriteAttributeFloat("x", rule.mpMinRotation.GetEulerAngles().x);
		writer.WriteAttributeFloat("y", rule.mpMinRotation.GetEulerAngles().y);
		writer.WriteAttributeFloat("z", rule.mpMinRotation.GetEulerAngles().z);
		writer.WriteOpeningTagEnd(true);
	}
	
	if(!rule.mpMaxRotation.GetEulerAngles().IsZero()){
		writer.WriteOpeningTagStart("maximumRotation");
		writer.WriteAttributeFloat("x", rule.mpMaxRotation.GetEulerAngles().x);
		writer.WriteAttributeFloat("y", rule.mpMaxRotation.GetEulerAngles().y);
		writer.WriteAttributeFloat("z", rule.mpMaxRotation.GetEulerAngles().z);
		writer.WriteOpeningTagEnd(true);
	}
	
	if(!rule.mpMinScaling->IsEqualTo(decVector(1.0f, 1.0f, 1.0f))){
		writer.WriteOpeningTagStart("minimumScaling");
		writer.WriteAttributeFloat("x", rule.mpMinScaling->x);
		writer.WriteAttributeFloat("y", rule.mpMinScaling->y);
		writer.WriteAttributeFloat("z", rule.mpMinScaling->z);
		writer.WriteOpeningTagEnd(true);
	}
	
	if(!rule.mpMaxScaling->IsEqualTo(decVector(1.0f, 1.0f, 1.0f))){
		writer.WriteOpeningTagStart("maximumScaling");
		writer.WriteAttributeFloat("x", rule.mpMaxScaling->x);
		writer.WriteAttributeFloat("y", rule.mpMaxScaling->y);
		writer.WriteAttributeFloat("z", rule.mpMaxScaling->z);
		writer.WriteOpeningTagEnd(true);
	}
	
	if(!rule.mpAxis->IsEqualTo(decVector(0.0f, 0.0f, 1.0f))){
		writer.WriteOpeningTagStart("axis");
		writer.WriteAttributeFloat("x", rule.mpAxis->x);
		writer.WriteAttributeFloat("y", rule.mpAxis->y);
		writer.WriteAttributeFloat("z", rule.mpAxis->z);
		writer.WriteOpeningTagEnd(true);
	}
	
	if(fabsf(rule.mpMinAngle) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("minimumAngle", rule.mpMinAngle);
	}
	if(fabsf(rule.mpMaxAngle) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("maximumAngle", rule.mpMaxAngle);
	}
	
	switch(rule.mpCoordinateFrame){
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
	
	if(rule.mpEnablePosition){
		writer.WriteDataTagBool("enablePosition", rule.mpEnablePosition);
	}
	if(!rule.mpEnableOrientation){
		writer.WriteDataTagBool("enableOrientation", rule.mpEnableOrientation);
	}
	if(rule.mpEnableSize){
		writer.WriteDataTagBool("enableSize", rule.mpEnableSize);
	}
	if(rule.mpUseAxis){
		writer.WriteDataTagBool("useAxis", rule.mpUseAxis);
	}
	if(!rule.mpTargetBone->IsEmpty()){
		writer.WriteDataTagString("targetBone", rule.mpTargetBone.GetValue());
	}
	
	if(!rule.mpInputBone->IsEmpty()){
		writer.WriteDataTagString("inputBone", rule.mpInputBone.GetValue());
	}
	
	switch(rule.mpInputSource){
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
	
	pSaveControllerTarget(writer, animator, rule.mpTargetTranslation, "translation");
	pSaveControllerTarget(writer, animator, rule.mpTargetRotation, "rotation");
	pSaveControllerTarget(writer, animator, rule.mpTargetScaling, "scaling");
	
	writer.WriteClosingTag("ruleBoneTransformator");
}

void aeLSAnimator::pSaveRuleStateManipulator(decXmlWriter &writer, const aeAnimator &animator,
const aeRuleStateManipulator &rule){
	writer.WriteOpeningTag("ruleStateManipulator");
	
	pSaveRuleCommon(writer, animator, rule);
	
	writer.WriteOpeningTagStart("positionMinimum");
	writer.WriteAttributeFloat("x", rule.mpMinPosition->x);
	writer.WriteAttributeFloat("y", rule.mpMinPosition->y);
	writer.WriteAttributeFloat("z", rule.mpMinPosition->z);
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteOpeningTagStart("positionMaximum");
	writer.WriteAttributeFloat("x", rule.mpMaxPosition->x);
	writer.WriteAttributeFloat("y", rule.mpMaxPosition->y);
	writer.WriteAttributeFloat("z", rule.mpMaxPosition->z);
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteOpeningTagStart("rotationMinimum");
	writer.WriteAttributeFloat("x", rule.mpMinRotation.GetEulerAngles().x);
	writer.WriteAttributeFloat("y", rule.mpMinRotation.GetEulerAngles().y);
	writer.WriteAttributeFloat("z", rule.mpMinRotation.GetEulerAngles().z);
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteOpeningTagStart("rotationMaximum");
	writer.WriteAttributeFloat("x", rule.mpMaxRotation.GetEulerAngles().x);
	writer.WriteAttributeFloat("y", rule.mpMaxRotation.GetEulerAngles().y);
	writer.WriteAttributeFloat("z", rule.mpMaxRotation.GetEulerAngles().z);
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteOpeningTagStart("sizeMinimum");
	writer.WriteAttributeFloat("x", rule.mpMinSize->x);
	writer.WriteAttributeFloat("y", rule.mpMinSize->y);
	writer.WriteAttributeFloat("z", rule.mpMinSize->z);
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteOpeningTagStart("sizeMaximum");
	writer.WriteAttributeFloat("x", rule.mpMaxSize->x);
	writer.WriteAttributeFloat("y", rule.mpMaxSize->y);
	writer.WriteAttributeFloat("z", rule.mpMaxSize->z);
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteDataTagFloat("vertexPositionSetMinimum", rule.mpMinVertexPositionSet);
	writer.WriteDataTagFloat("vertexPositionSetMaximum", rule.mpMaxVertexPositionSet);
	
	if(rule.mpEnablePosition){
		writer.WriteDataTagBool("enablePosition", rule.mpEnablePosition);
	}
	if(!rule.mpEnableRotation){
		writer.WriteDataTagBool("enableOrientation", rule.mpEnableRotation);
	}
	if(rule.mpEnableSize){
		writer.WriteDataTagBool("enableSize", rule.mpEnableSize);
	}
	if(!rule.mpEnableVertexPositionSet){
		writer.WriteDataTagBool("enableVertexPositionSet", rule.mpEnableVertexPositionSet);
	}
	
	pSaveControllerTarget(writer, animator, rule.mpTargetPosition, "position");
	pSaveControllerTarget(writer, animator, rule.mpTargetRotation, "orientation");
	pSaveControllerTarget(writer, animator, rule.mpTargetSize, "size");
	pSaveControllerTarget(writer, animator, rule.mpTargetVertexPositionSet, "vertexPositionSet");
	
	writer.WriteClosingTag("ruleStateManipulator");
}

void aeLSAnimator::pSaveRuleStateSnapshot(decXmlWriter &writer, const aeAnimator &animator,
const aeRuleStateSnapshot &rule){
	writer.WriteOpeningTag("ruleStateSnapshot");
	
	pSaveRuleCommon(writer, animator, rule);
	
	if(!rule.mpUseLastState){
		writer.WriteDataTagBool("useLastState", rule.mpUseLastState);
	}
	
	if(!rule.mpEnablePosition){
		writer.WriteDataTagBool("enablePosition", rule.mpEnablePosition);
	}
	if(!rule.mpEnableOrientation){
		writer.WriteDataTagBool("enableOrientation", rule.mpEnableOrientation);
	}
	if(rule.mpEnableSize){
		writer.WriteDataTagBool("enableSize", rule.mpEnableSize);
	}
	if(!rule.mpEnableVertexPositionSet){
		writer.WriteDataTagBool("enableVertexPositionSet", rule.mpEnableVertexPositionSet);
	}
	
	writer.WriteClosingTag("ruleStateSnapshot");
}

void aeLSAnimator::pSaveRuleInverseKinematic(decXmlWriter &writer, const aeAnimator &animator,
const aeRuleInverseKinematic &rule){
	writer.WriteOpeningTag("ruleInverseKinematic");
	
	pSaveRuleCommon(writer, animator, rule);
	
	const decVector &goalPosition = rule.mpGoalPosition;
	writer.WriteOpeningTagStart("goalPosition");
	writer.WriteAttributeFloat("x", goalPosition.x);
	writer.WriteAttributeFloat("y", goalPosition.y);
	writer.WriteAttributeFloat("z", goalPosition.z);
	writer.WriteOpeningTagEnd(true);
	
	const decVector &goalOrientation = rule.mpGoalOrientation;
	writer.WriteOpeningTagStart("goalOrientation");
	writer.WriteAttributeFloat("x", goalOrientation.x);
	writer.WriteAttributeFloat("y", goalOrientation.y);
	writer.WriteAttributeFloat("z", goalOrientation.z);
	writer.WriteOpeningTagEnd(true);
	
	const decVector &localPosition = rule.mpLocalPosition;
	writer.WriteOpeningTagStart("localPosition");
	writer.WriteAttributeFloat("x", localPosition.x);
	writer.WriteAttributeFloat("y", localPosition.y);
	writer.WriteAttributeFloat("z", localPosition.z);
	writer.WriteOpeningTagEnd(true);
	
	const decVector &localOrientation = rule.mpLocalOrientation;
	writer.WriteOpeningTagStart("localOrientation");
	writer.WriteAttributeFloat("x", localOrientation.x);
	writer.WriteAttributeFloat("y", localOrientation.y);
	writer.WriteAttributeFloat("z", localOrientation.z);
	writer.WriteOpeningTagEnd(true);
	
	if(rule.mpAdjustOrientation){
		writer.WriteDataTagBool("adjustOrientation", rule.mpAdjustOrientation);
	}
	
	if(!rule.mpSolverBone->IsEmpty()){
		writer.WriteDataTagString("solverBone", rule.mpSolverBone.GetValue());
	}
	if(rule.mpUseSolverBone){
		writer.WriteDataTagBool("useSolverBone", rule.mpUseSolverBone);
	}
	
	writer.WriteDataTagFloat("reachRange", rule.mpReachRange);
	if(!rule.mpReachBone->IsEmpty()){
		writer.WriteDataTagString("reachBone", rule.mpReachBone.GetValue());
	}
	
	const decVector &reachCenter = rule.mpReachCenter;
	if(!reachCenter.IsZero()){
		writer.WriteOpeningTagStart("reachCenter");
		writer.WriteAttributeFloat("x", reachCenter.x);
		writer.WriteAttributeFloat("y", reachCenter.y);
		writer.WriteAttributeFloat("z", reachCenter.z);
		writer.WriteOpeningTagEnd(true);
	}
	
	pSaveControllerTarget(writer, animator, rule.mpTargetGoalPosition, "goalPosition");
	pSaveControllerTarget(writer, animator, rule.mpTargetGoalOrientation, "goalOrientation");
	pSaveControllerTarget(writer, animator, rule.mpTargetLocalPosition, "localPosition");
	pSaveControllerTarget(writer, animator, rule.mpTargetLocalOrientation, "localOrientation");
	pSaveControllerTarget(writer, animator, rule.mpTargetReachRange, "reachRange");
	pSaveControllerTarget(writer, animator, rule.mpTargetReachCenter, "reachCenter");
	
	writer.WriteClosingTag("ruleInverseKinematic");
}

void aeLSAnimator::pSaveRuleForeignState(decXmlWriter &writer, const aeAnimator &animator,
const aeRuleForeignState &rule){
	writer.WriteOpeningTag("ruleForeignState");
	
	pSaveRuleCommon(writer, animator, rule);
	
	writer.WriteDataTagString("foreignBone", rule.mpForeignBone.GetValue());
	writer.WriteDataTagString("foreignVertexPositionSet", rule.mpForeignVertexPositionSet.GetValue());
	writer.WriteDataTagFloat("scalePosition", rule.mpScalePosition);
	writer.WriteDataTagFloat("scaleOrientation", rule.mpScaleOrientation);
	writer.WriteDataTagFloat("scaleSize", rule.mpScaleSize);
	writer.WriteDataTagFloat("scaleVertexPositionSet", rule.mpScaleVertexPositionSet);
	
	if(!rule.mpEnablePosition){
		writer.WriteDataTagBool("enablePosition", rule.mpEnablePosition);
	}
	if(!rule.mpEnableOrientation){
		writer.WriteDataTagBool("enableOrientation", rule.mpEnableOrientation);
	}
	if(rule.mpEnableSize){
		writer.WriteDataTagBool("enableSize", rule.mpEnableSize);
	}
	if(!rule.mpEnableVertexPositionSet){
		writer.WriteDataTagBool("enableVertexPositionSet", rule.mpEnableVertexPositionSet);
	}
	
	if(rule.mpSourceCoordinateFrame == deAnimatorRuleForeignState::ecfBoneLocal){
		writer.WriteDataTagString("srcCFrame", "local");
		
	}else{ // deAnimatorRuleForeignState::ecfComponent
		writer.WriteDataTagString("srcCFrame", "component");
	}
	
	if(rule.mpDestCoordinateFrame == deAnimatorRuleForeignState::ecfBoneLocal){
		writer.WriteDataTagString("destCFrame", "local");
		
	}else{ // deAnimatorRuleForeignState::ecfComponent
		writer.WriteDataTagString("destCFrame", "component");
	}
	
	pSaveControllerTarget(writer, animator, rule.mpTargetPosition, "position");
	pSaveControllerTarget(writer, animator, rule.mpTargetOrientation, "orientation");
	pSaveControllerTarget(writer, animator, rule.mpTargetSize, "size");
	pSaveControllerTarget(writer, animator, rule.mpTargetVertexPositionSet, "vertexPositionSet");
	
	writer.WriteClosingTag("ruleForeignState");
}


void aeLSAnimator::pSaveRuleMirror(decXmlWriter &writer, const aeAnimator &animator,
const aeRuleMirror &rule){
	writer.WriteOpeningTag("ruleMirror");
	
	pSaveRuleCommon(writer, animator, rule);
	
	switch(rule.mpMirrorAxis){
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
	
	if(!rule.mpMirrorBone->IsEmpty()){
		writer.WriteDataTagString("mirrorBone", rule.mpMirrorBone.GetValue());
	}
	
	rule.mpMatchNames->Visit([&](const aeRuleMirror::MatchName &matchName){
		writer.WriteOpeningTagStart("matchName");
		writer.WriteAttributeString("first", matchName.mpFirst.GetValue());
		writer.WriteAttributeString("second", matchName.mpSecond.GetValue());
		
		switch(matchName.mpType){
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
	});
	
	if(!rule.mpEnablePosition){
		writer.WriteDataTagBool("enablePosition", rule.mpEnablePosition);
	}
	if(!rule.mpEnableOrientation){
		writer.WriteDataTagBool("enableOrientation", rule.mpEnableOrientation);
	}
	if(rule.mpEnableSize){
		writer.WriteDataTagBool("enableSize", rule.mpEnableSize);
	}
	if(!rule.mpEnableVertexPositionSet){
		writer.WriteDataTagBool("enableVertexPositionSet", rule.mpEnableVertexPositionSet);
	}
	
	writer.WriteClosingTag("ruleMirror");
}

void aeLSAnimator::pSaveRuleGroup(decXmlWriter &writer, const aeAnimator &animator,
const aeRuleGroup &rule){
	writer.WriteOpeningTag("ruleGroup");
	
	pSaveRuleCommon(writer, animator, rule);
	
	if(!rule.mpEnablePosition){
		writer.WriteDataTagBool("enablePosition", rule.mpEnablePosition);
	}
	if(!rule.mpEnableOrientation){
		writer.WriteDataTagBool("enableOrientation", rule.mpEnableOrientation);
	}
	if(rule.mpEnableSize){
		writer.WriteDataTagBool("enableSize", rule.mpEnableSize);
	}
	if(!rule.mpEnableVertexPositionSet){
		writer.WriteDataTagBool("enableVertexPositionSet", rule.mpEnableVertexPositionSet);
	}
	
	rule.mpRules->Visit([&](const aeRule &each){
		writer.WriteNewline();
		pSaveRule(writer, animator, each);
	});
	
	if(rule.mpUseCurrentState){
		writer.WriteDataTagBool("useCurrentState", rule.mpUseCurrentState);
	}
	
	switch(rule.mpApplicationType){
	case deAnimatorRuleGroup::eatAll:
		writer.WriteDataTagString("applicationType", "all");
		break;
		
	case deAnimatorRuleGroup::eatSelect:
		writer.WriteDataTagString("applicationType", "selection");
		break;
	}
	
	pSaveControllerTarget(writer, animator, rule.mpTargetSelect, "selection");
	
	writer.WriteClosingTag("ruleGroup");
}

void aeLSAnimator::pSaveRuleSubAnimator(decXmlWriter &writer, const aeAnimator &animator,
const aeRuleSubAnimator &rule){
	writer.WriteOpeningTag("ruleSubAnimator");
	
	pSaveRuleCommon(writer, animator, rule);
	
	writer.WriteDataTagString("pathAnimator", rule.mpPathSubAnimator.GetValue());
	
	rule.GetConnections().VisitIndexed([&](int i, aeController *controller){
		if(!controller){
			return;
		}
		
		writer.WriteOpeningTagStart("connection");
		writer.WriteAttributeInt("controller", animator.GetControllers().IndexOf(controller));
		writer.WriteOpeningTagEnd(false, false);
		writer.WriteTextString(rule.GetSubAnimator()->GetControllers().GetAt(i)->GetName());
		writer.WriteClosingTag("connection", false);
	});
	
	if(!rule.mpEnablePosition){
		writer.WriteDataTagBool("enablePosition", rule.mpEnablePosition);
	}
	if(!rule.mpEnableOrientation){
		writer.WriteDataTagBool("enableOrientation", rule.mpEnableOrientation);
	}
	if(!rule.mpEnableSize){
		writer.WriteDataTagBool("enableSize", rule.mpEnableSize);
	}
	if(!rule.mpEnableVertexPositionSet){
		writer.WriteDataTagBool("enableVertexPositionSet", rule.mpEnableVertexPositionSet);
	}
	
	writer.WriteClosingTag("ruleSubAnimator");
}

void aeLSAnimator::pSaveRuleTrackTo(decXmlWriter &writer, const aeAnimator &animator,
const aeRuleTrackTo &rule){
	writer.WriteOpeningTag("ruleTrackTo");
	
	pSaveRuleCommon(writer, animator, rule);
	
	writer.WriteDataTagString("trackBone", rule.mpTrackBone.GetValue());
	
	switch(rule.mpTrackAxis){
	case deAnimatorRuleTrackTo::etaPosX:
		writer.WriteDataTagString("trackAxis", "posX");
		break;
		
	case deAnimatorRuleTrackTo::etaPosY:
		writer.WriteDataTagString("trackAxis", "posY");
		break;
		
	case deAnimatorRuleTrackTo::etaPosZ:
		writer.WriteDataTagString("trackAxis", "posZ");
		break;
		
	case deAnimatorRuleTrackTo::etaNegX:
		writer.WriteDataTagString("trackAxis", "negX");
		break;
		
	case deAnimatorRuleTrackTo::etaNegY:
		writer.WriteDataTagString("trackAxis", "negY");
		break;
		
	case deAnimatorRuleTrackTo::etaNegZ:
		writer.WriteDataTagString("trackAxis", "negZ");
		break;
	}
	
	switch(rule.mpUpAxis){
	case deAnimatorRuleTrackTo::etaPosX:
		writer.WriteDataTagString("upAxis", "posX");
		break;
		
	case deAnimatorRuleTrackTo::etaPosY:
		writer.WriteDataTagString("upAxis", "posY");
		break;
		
	case deAnimatorRuleTrackTo::etaPosZ:
		writer.WriteDataTagString("upAxis", "posZ");
		break;
		
	case deAnimatorRuleTrackTo::etaNegX:
		writer.WriteDataTagString("upAxis", "negX");
		break;
		
	case deAnimatorRuleTrackTo::etaNegY:
		writer.WriteDataTagString("upAxis", "negY");
		break;
		
	case deAnimatorRuleTrackTo::etaNegZ:
		writer.WriteDataTagString("upAxis", "negZ");
		break;
	}
	
	switch(rule.mpUpTarget){
	case deAnimatorRuleTrackTo::eutWorldX:
		writer.WriteDataTagString("upTarget", "worldX");
		break;
		
	case deAnimatorRuleTrackTo::eutWorldY:
		writer.WriteDataTagString("upTarget", "worldY");
		break;
		
	case deAnimatorRuleTrackTo::eutWorldZ:
		writer.WriteDataTagString("upTarget", "worldZ");
		break;
		
	case deAnimatorRuleTrackTo::eutComponentX:
		writer.WriteDataTagString("upTarget", "componentX");
		break;
		
	case deAnimatorRuleTrackTo::eutComponentY:
		writer.WriteDataTagString("upTarget", "componentY");
		break;
		
	case deAnimatorRuleTrackTo::eutComponentZ:
		writer.WriteDataTagString("upTarget", "componentZ");
		break;
		
	case deAnimatorRuleTrackTo::eutTrackBoneX:
		writer.WriteDataTagString("upTarget", "trackBoneX");
		break;
		
	case deAnimatorRuleTrackTo::eutTrackBoneY:
		writer.WriteDataTagString("upTarget", "trackBoneY");
		break;
		
	case deAnimatorRuleTrackTo::eutTrackBoneZ:
		writer.WriteDataTagString("upTarget", "trackBoneZ");
		break;
		
	case deAnimatorRuleTrackTo::eutController:
		writer.WriteDataTagString("upTarget", "controller");
		break;
	}
	
	switch(rule.mpLockedAxis){
	case deAnimatorRuleTrackTo::elaNone:
		writer.WriteDataTagString("lockedAxis", "none");
		break;
		
	case deAnimatorRuleTrackTo::elaX:
		writer.WriteDataTagString("lockedAxis", "x");
		break;
		
	case deAnimatorRuleTrackTo::elaY:
		writer.WriteDataTagString("lockedAxis", "y");
		break;
		
	case deAnimatorRuleTrackTo::elaZ:
		writer.WriteDataTagString("lockedAxis", "z");
		break;
	}
	
	pSaveControllerTarget(writer, animator, rule.mpTargetPosition, "position");
	pSaveControllerTarget(writer, animator, rule.mpTargetUp, "up");
	
	writer.WriteClosingTag("ruleTrackTo");
}

void aeLSAnimator::pSaveRuleLimit(decXmlWriter &writer, const aeAnimator &animator,
const aeRuleLimit &rule){
	writer.WriteOpeningTag("ruleLimit");
	
	pSaveRuleCommon(writer, animator, rule);
	
	writer.WriteOpeningTagStart("minimumPosition");
	writer.WriteAttributeFloat("x", rule.mpMinPosition->x);
	writer.WriteAttributeFloat("y", rule.mpMinPosition->y);
	writer.WriteAttributeFloat("z", rule.mpMinPosition->z);
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteOpeningTagStart("maximumPosition");
	writer.WriteAttributeFloat("x", rule.mpMaxPosition->x);
	writer.WriteAttributeFloat("y", rule.mpMaxPosition->y);
	writer.WriteAttributeFloat("z", rule.mpMaxPosition->z);
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteOpeningTagStart("minimumRotation");
	writer.WriteAttributeFloat("x", rule.mpMinRotation.GetEulerAngles().x);
	writer.WriteAttributeFloat("y", rule.mpMinRotation.GetEulerAngles().y);
	writer.WriteAttributeFloat("z", rule.mpMinRotation.GetEulerAngles().z);
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteOpeningTagStart("maximumRotation");
	writer.WriteAttributeFloat("x", rule.mpMaxRotation.GetEulerAngles().x);
	writer.WriteAttributeFloat("y", rule.mpMaxRotation.GetEulerAngles().y);
	writer.WriteAttributeFloat("z", rule.mpMaxRotation.GetEulerAngles().z);
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteOpeningTagStart("minimumScaling");
	writer.WriteAttributeFloat("x", rule.mpMinScaling->x);
	writer.WriteAttributeFloat("y", rule.mpMinScaling->y);
	writer.WriteAttributeFloat("z", rule.mpMinScaling->z);
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteOpeningTagStart("maximumScaling");
	writer.WriteAttributeFloat("x", rule.mpMaxScaling->x);
	writer.WriteAttributeFloat("y", rule.mpMaxScaling->y);
	writer.WriteAttributeFloat("z", rule.mpMaxScaling->z);
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteDataTagFloat("minimumVertexPositionSet", rule.GetMinimumVertexPositionSet());
	writer.WriteDataTagFloat("maximumVertexPositionSet", rule.GetMaximumVertexPositionSet());
	
	switch(rule.mpCoordinateFrame){
	case deAnimatorRuleLimit::ecfBoneLocal:
		writer.WriteDataTagString("cframe", "local");
		break;
		
	case deAnimatorRuleLimit::ecfComponent:
		writer.WriteDataTagString("cframe", "component");
		break;
		
	case deAnimatorRuleLimit::ecfTargetBone:
		writer.WriteDataTagString("cframe", "target");
		break;
	}
	
	writer.WriteDataTagString("targetBone", rule.mpTargetBone.GetValue());
	
	if(rule.mpEnablePositionXMin || rule.mpEnablePositionYMin || rule.mpEnablePositionZMin){
		writer.WriteOpeningTagStart("enablePosMin");
		if(rule.mpEnablePositionXMin){
			writer.WriteAttributeBool("x", rule.mpEnablePositionXMin);
		}
		if(rule.mpEnablePositionYMin){
			writer.WriteAttributeBool("y", rule.mpEnablePositionYMin);
		}
		if(rule.mpEnablePositionZMin){
			writer.WriteAttributeBool("z", rule.mpEnablePositionZMin);
		}
		writer.WriteOpeningTagEnd(true);
	}
	
	if(rule.mpEnablePositionXMax || rule.mpEnablePositionYMax || rule.mpEnablePositionZMax){
		writer.WriteOpeningTagStart("enablePosMax");
		if(rule.mpEnablePositionXMax){
			writer.WriteAttributeBool("x", rule.mpEnablePositionXMax);
		}
		if(rule.mpEnablePositionYMax){
			writer.WriteAttributeBool("y", rule.mpEnablePositionYMax);
		}
		if(rule.mpEnablePositionZMax){
			writer.WriteAttributeBool("z", rule.mpEnablePositionZMax);
		}
		writer.WriteOpeningTagEnd(true);
	}
	
	if(rule.mpEnableRotationXMin || rule.mpEnableRotationYMin || rule.mpEnableRotationZMin){
		writer.WriteOpeningTagStart("enableRotMin");
		if(rule.mpEnableRotationXMin){
			writer.WriteAttributeBool("x", rule.mpEnableRotationXMin);
		}
		if(rule.mpEnableRotationYMin){
			writer.WriteAttributeBool("y", rule.mpEnableRotationYMin);
		}
		if(rule.mpEnableRotationZMin){
			writer.WriteAttributeBool("z", rule.mpEnableRotationZMin);
		}
		writer.WriteOpeningTagEnd(true);
	}
	
	if(rule.mpEnableRotationXMax || rule.mpEnableRotationYMax || rule.mpEnableRotationZMax){
		writer.WriteOpeningTagStart("enableRotMax");
		if(rule.mpEnableRotationXMax){
			writer.WriteAttributeBool("x", rule.mpEnableRotationXMax);
		}
		if(rule.mpEnableRotationYMax){
			writer.WriteAttributeBool("y", rule.mpEnableRotationYMax);
		}
		if(rule.mpEnableRotationZMax){
			writer.WriteAttributeBool("z", rule.mpEnableRotationZMax);
		}
		writer.WriteOpeningTagEnd(true);
	}
	
	if(rule.mpEnableScalingXMin || rule.mpEnableScalingYMin || rule.mpEnableScalingZMin){
		writer.WriteOpeningTagStart("enableScaleMin");
		if(rule.mpEnableScalingXMin){
			writer.WriteAttributeBool("x", rule.mpEnableScalingXMin);
		}
		if(rule.mpEnableScalingYMin){
			writer.WriteAttributeBool("y", rule.mpEnableScalingYMin);
		}
		if(rule.mpEnableScalingZMin){
			writer.WriteAttributeBool("z", rule.mpEnableScalingZMin);
		}
		writer.WriteOpeningTagEnd(true);
	}
	
	if(rule.mpEnableScalingXMax || rule.mpEnableScalingYMax || rule.mpEnableScalingZMax){
		writer.WriteOpeningTagStart("enableScaleMax");
		if(rule.mpEnableScalingXMax){
			writer.WriteAttributeBool("x", rule.mpEnableScalingXMax);
		}
		if(rule.mpEnableScalingYMax){
			writer.WriteAttributeBool("y", rule.mpEnableScalingYMax);
		}
		if(rule.mpEnableScalingZMax){
			writer.WriteAttributeBool("z", rule.mpEnableScalingZMax);
		}
		writer.WriteOpeningTagEnd(true);
	}
	
	if(rule.mpEnableVertexPositionSetMin){
		writer.WriteDataTagFloat("enableVertexPositionSetMin", rule.mpEnableVertexPositionSetMin);
	}
	if(rule.mpEnableVertexPositionSetMax){
		writer.WriteDataTagFloat("enableVertexPositionSetMax", rule.mpEnableVertexPositionSetMax);
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
	decStringSet affectedBones, affectedVps;
	decXmlElementTag *tag;
	int i;
	
	for(i=0; i<root->GetElementCount(); i++){
		tag = root->GetElementIfTag(i);
		if(tag){
			if(strcmp(tag->GetName(), "display") == 0){
				pLoadDisplay(tag, animator);
				
			}else if(strcmp(tag->GetName(), "rig") == 0){
				if(tag->GetFirstData()){
					animator.mpRigPath.SetValue(GetCDataString(*tag), false);
				}
				
			}else if(strcmp(tag->GetName(), "animation") == 0){
				if(tag->GetFirstData()){
					animator.mpAnimationPath.SetValue(GetCDataString(*tag), false);
				}
				
			}else if(strcmp(tag->GetName(), "bone") == 0){
				affectedBones.Add(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "vertexPositionSet") == 0){
				affectedVps.Add(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "controller") == 0){
				pLoadController(tag, animator);
				
			}else if(strcmp(tag->GetName(), "link") == 0){
				pLoadLink(tag, animator);
				
			}else{
				const aeRule::Ref rule(pLoadRule(tag, animator));
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
	
	animator.mpAffectedBones.SetValue(affectedBones, false);
	animator.mpAffectedVps.SetValue(affectedVps, false);
	
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
					animator.mpDisplayModelPath.SetValue(GetCDataString(*tag), false);
				}
				
			}else if(strcmp(tag->GetName(), "skin") == 0){
				if(tag->GetFirstData()){
					animator.mpDisplaySkinPath.SetValue(GetCDataString(*tag), false);
				}
				
			}else if(strcmp(tag->GetName(), "rig") == 0){
				if(tag->GetFirstData()){
					animator.mpDisplayRigPath.SetValue(GetCDataString(*tag), false);
				}
				
			/*}else if( strcmp( tag->GetName(), "animation" ) == 0 ){
				if(tag->GetFirstData()){
					animator.mpAnimationPath.SetValue(GetCDataString(*tag), false);
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
					locomotion.mpLocomotionType.SetValue(aeAnimatorLocomotion::eltNatural, false);
					
				}else if(cdata == "fps"){
					locomotion.mpLocomotionType.SetValue(aeAnimatorLocomotion::eltFPS, false);
					
				}else if(cdata == "vehicle"){
					locomotion.mpLocomotionType.SetValue(aeAnimatorLocomotion::eltVehicle, false);
					
				}else{
					LogErrorUnknownValue(*tag, cdata);
				}
				
				
			}else if(strcmp(tag->GetName(), "limitLookDown") == 0){
				locomotion.mpLimitLookDown.SetValue(strtof(GetCDataString(*tag), nullptr), false);
				
			}else if(strcmp(tag->GetName(), "limitLookUp") == 0){
				locomotion.mpLimitLookUp.SetValue(strtof(GetCDataString(*tag), nullptr), false);
				
			}else if(strcmp(tag->GetName(), "limitLookLeft") == 0){
				locomotion.mpLimitLookLeft.SetValue(strtof(GetCDataString(*tag), nullptr), false);
				
			}else if(strcmp(tag->GetName(), "limitLookRight") == 0){
				locomotion.mpLimitLookRight.SetValue(strtof(GetCDataString(*tag), nullptr), false);
				
			}else if(strcmp(tag->GetName(), "limitLookUpDownSpeed") == 0){
				// deprecated
				
			}else if(strcmp(tag->GetName(), "limitLookLeftRightSpeed") == 0){
				// deprecated
				
			}else if(strcmp(tag->GetName(), "adjustTimeUpDown") == 0){
				locomotion.mpAdjustTimeUpDown.SetValue(strtof(GetCDataString(*tag), nullptr), false);
				
			}else if(strcmp(tag->GetName(), "adjustTimeLeftRight") == 0){
				locomotion.mpAdjustTimeLeftRight.SetValue(strtof(GetCDataString(*tag), nullptr), false);
				
			}else if(strcmp(tag->GetName(), "adjustTimeStance") == 0){
				locomotion.mpAdjustTimeStance.SetValue(strtof(GetCDataString(*tag), nullptr), false);
				
			}else if(strcmp(tag->GetName(), "adjustTimeTurnIP") == 0){
				locomotion.mpAdjustTimeTurnIP.SetValue(strtof(GetCDataString(*tag), nullptr), false);
				
			}else if(strcmp(tag->GetName(), "adjustTimeOrientation") == 0){
				locomotion.mpAdjustTimeOrientation.SetValue(strtof(GetCDataString(*tag), nullptr), false);
				
			}else if(strcmp(tag->GetName(), "adjustTimeVelocity") == 0){
				locomotion.mpAdjustTimeVelocity.SetValue(strtof(GetCDataString(*tag), nullptr), false);
				
			}else if(strcmp(tag->GetName(), "speedWalk") == 0){
				locomotion.mpSpeedWalk.SetValue(strtof(GetCDataString(*tag), nullptr), false);
				
			}else if(strcmp(tag->GetName(), "speedRun") == 0){
				locomotion.mpSpeedRun.SetValue(strtof(GetCDataString(*tag), nullptr), false);
				
			}else if(strcmp(tag->GetName(), "legBlendTime") == 0){
				locomotion.mpLegBlendTime.SetValue(strtof(GetCDataString(*tag), nullptr), false);
				
			}else if(strcmp(tag->GetName(), "useLegPairs") == 0){
				locomotion.mpUseLegPairs.SetValue((int)strtol(GetCDataString(*tag), nullptr, 10), false);
				
			}else if(strcmp(tag->GetName(), "leg") == 0){
				if(leg >= locomotion.GetLegs().GetCount()){
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
	aeAnimatorLocomotionLeg &leg = *animator.GetLocomotion().GetLegs().GetAt(legnum);
	decXmlElementTag *tag;
	decVector vector;
	int i;
	
	for(i=0; i<root->GetElementCount(); i++){
		tag = root->GetElementIfTag(i);
		if(tag){
			if(strcmp(tag->GetName(), "liftOffTime") == 0){
				leg.mpLiftOffTime.SetValue(strtof(GetCDataString(*tag), nullptr), false);
				
			}else if(strcmp(tag->GetName(), "putDownTime") == 0){
				leg.mpPutDownTime.SetValue(strtof(GetCDataString(*tag), nullptr), false);
				
			}else if(strcmp(tag->GetName(), "putDownPosition") == 0){ // depracted
				vector.SetZero();
				ReadVector(*tag, vector);
				leg.mpPutDownPositionStand.SetValue(vector, false);
				
			}else if(strcmp(tag->GetName(), "putDownPositionStand") == 0){
				vector.SetZero();
				ReadVector(*tag, vector);
				leg.mpPutDownPositionStand.SetValue(vector, false);
				
			}else if(strcmp(tag->GetName(), "putDownPositionWalk") == 0){
				vector.SetZero();
				ReadVector(*tag, vector);
				leg.mpPutDownPositionWalk.SetValue(vector, false);
				
			}else if(strcmp(tag->GetName(), "putDownPositionRun") == 0){
				vector.SetZero();
				ReadVector(*tag, vector);
				leg.mpPutDownPositionRun.SetValue(vector, false);
				
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
	
	const auto controller = aeController::Ref::New(*pLSSys->GetWindowMain());
	animator.AddController(controller);
	
	// parse tag
	for(i=0; i<root->GetElementCount(); i++){
		tag = root->GetElementIfTag(i);
		if(tag){
			if(strcmp(tag->GetName(), "name") == 0){
				controller->mpName.SetValue(GetCDataString(*tag), false);
				
			}else if(strcmp(tag->GetName(), "clamp") == 0){
				controller->mpClamp.SetValue(GetCDataBool(*tag), false);
				
			}else if(strcmp(tag->GetName(), "frozen") == 0){
				controller->mpFrozen.SetValue(GetCDataBool(*tag), false);
				
			}else if(strcmp(tag->GetName(), "limit") == 0){
				pLoadControllerLimit(tag, controller);
				
			}else if(strcmp(tag->GetName(), "limits") == 0){
				controller->mpMinimumValue.SetValue(pGetAttributeFloat(tag, "min"), false);
				controller->mpMaximumValue.SetValue(pGetAttributeFloat(tag, "max"), false);
				
			}else if(strcmp(tag->GetName(), "locomotionAttribute") == 0){
				cdata = GetCDataString(*tag);
				
				if(strcmp(cdata, "elapsedTime") == 0){
					controller->mpLocomotionAttribute.SetValue(aeAnimatorLocomotion::eaElapsedTime, false);
					
				}else if(strcmp(cdata, "lookUpDown") == 0){
					controller->mpLocomotionAttribute.SetValue(aeAnimatorLocomotion::eaLookUpDown, false);
					
				}else if(strcmp(cdata, "lookLeftRight") == 0){
					controller->mpLocomotionAttribute.SetValue(aeAnimatorLocomotion::eaLookLeftRight, false);
					
				}else if(strcmp(cdata, "movingSpeed") == 0){
					controller->mpLocomotionAttribute.SetValue(aeAnimatorLocomotion::eaMovingSpeed, false);
					
				}else if(strcmp(cdata, "movingDirection") == 0){
					controller->mpLocomotionAttribute.SetValue(aeAnimatorLocomotion::eaMovingDirection, false);
					
				}else if(strcmp(cdata, "relativeMovingSpeed") == 0){
					controller->mpLocomotionAttribute.SetValue(aeAnimatorLocomotion::eaRelativeMovingSpeed, false);
					
				}else if(strcmp(cdata, "turningSpeed") == 0){
					controller->mpLocomotionAttribute.SetValue(aeAnimatorLocomotion::eaTurningSpeed, false);
					
				}else if(strcmp(cdata, "stance") == 0){
					controller->mpLocomotionAttribute.SetValue(aeAnimatorLocomotion::eaStance, false);
					
				}else if(strcmp(cdata, "displacement") == 0){
					controller->mpLocomotionAttribute.SetValue(aeAnimatorLocomotion::eaDisplacement, false);
					
				}else if(strcmp(cdata, "relativeDisplacement") == 0){
					controller->mpLocomotionAttribute.SetValue(aeAnimatorLocomotion::eaRelativeDisplacement, false);
					
				}else if(strcmp(cdata, "bodyTiltOffset") == 0){
					controller->mpLocomotionAttribute.SetValue(aeAnimatorLocomotion::eaBodyTiltOffset, false);
					
				}else if(strcmp(cdata, "bodyTiltUpDown") == 0){
					controller->mpLocomotionAttribute.SetValue(aeAnimatorLocomotion::eaBodyTiltUpDown, false);
					
				}else if(strcmp(cdata, "bodyTiltLeftRight") == 0){
					controller->mpLocomotionAttribute.SetValue(aeAnimatorLocomotion::eaBodyTiltRightLeft, false);
					
				}else if(strcmp(cdata, "timeTurnIP") == 0){
					controller->mpLocomotionAttribute.SetValue(aeAnimatorLocomotion::eaTimeTurnIP, false);
					
				}else if(strcmp(cdata, "legGroundPosition") == 0){
					controller->mpLocomotionAttribute.SetValue(aeAnimatorLocomotion::eaLegGroundPosition, false);
					
				}else if(strcmp(cdata, "legGroundNormal") == 0){
					controller->mpLocomotionAttribute.SetValue(aeAnimatorLocomotion::eaLegGroundNormal, false);
					
				}else if(strcmp(cdata, "legInfluence") == 0){
					controller->mpLocomotionAttribute.SetValue(aeAnimatorLocomotion::eaLegInfluence, false);
					
				}else if(strcmp(cdata, "legPosition") == 0){
					controller->mpLocomotionAttribute.SetValue(aeAnimatorLocomotion::eaLegPosition, false);
					
				}else if(strcmp(cdata, "legOrientation") == 0){
					controller->mpLocomotionAttribute.SetValue(aeAnimatorLocomotion::eaLegOrientation, false);
					
				}else{
					logger.LogWarnFormat(LOGSOURCE, "controller(%i:%i): Unknown locomotion attribute %s, ignoring",
						tag->GetLineNumber(), tag->GetPositionNumber(), cdata);
				}
				
			}else if(strcmp(tag->GetName(), "locomotionLeg") == 0){
				leg = (int)strtol(GetCDataString(*tag), nullptr, 10);
				
				if(leg >= 0 && leg < animator.GetLocomotion().GetLegs().GetCount()){
					controller->mpLocomotionLeg.SetValue(leg, false);
				}
				
			}else if(strcmp(tag->GetName(), "vectorSimulation") == 0){
				cdata = GetCDataString(*tag);
				
				if(strcmp(cdata, "position") == 0){
					controller->mpVectorSimulation.SetValue(aeController::evsPosition, false);
					
				}else if(strcmp(cdata, "rotation") == 0){
					controller->mpVectorSimulation.SetValue(aeController::evsRotation, false);
					
				}else{
					logger.LogWarnFormat(LOGSOURCE, "controller(%i:%i): Unknown vector simulation %s, ignoring",
						tag->GetLineNumber(), tag->GetPositionNumber(), cdata);
				}
				
			}else if(strcmp(tag->GetName(), "value") == 0){
				controller->mpDefaultValue.SetValue(GetCDataFloat(*tag), false);
				controller->mpCurrentValue.SetValue(controller->GetDefaultValue(), false);
				
			}else if(strcmp(tag->GetName(), "vector") == 0){
				ReadVector(*tag, vector);
				controller->mpDefaultVector.SetValue(vector, false);
				controller->mpVector.SetValue(controller->GetDefaultVector(), false);
				
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
				controller.mpMinimumValue.SetValue(strtof(GetCDataString(*tag), nullptr), false);
				
			}else if(strcmp(tag->GetName(), "maximum") == 0){
				controller.mpMaximumValue.SetValue(strtof(GetCDataString(*tag), nullptr), false);
				
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
	
	const aeLink::Ref link(aeLink::Ref::New(*pLSSys->GetWindowMain()));
	animator.AddLink(link);
	
	// parse tag
	for(i=0; i<root->GetElementCount(); i++){
		tag = root->GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		if(tag->GetName() == "name"){
			link->mpName.SetValue(GetCDataString(*tag), false);
			
		}else if(tag->GetName() == "controller"){
			index = (int)strtol(GetCDataString(*tag), nullptr, 10);
			
			if(index == -1){
				link->mpController.SetValue(nullptr, false);
				
			}else{
				link->mpController.SetValue(animator.GetControllers()[index], false);
			}
			
		}else if(tag->GetName() == "repeat"){
			link->mpRepeat.SetValue((int)strtol(GetCDataString(*tag), nullptr, 10), false);
			
		}else if(tag->GetName() == "curve"){
			decCurveBezier curve;
			ReadCurveBezier(*tag, curve);
			link->mpCurve.SetValue(curve, false);
			
		}else if(tag->GetName() == "bone"){
			link->mpBone.SetValue(GetCDataString(*tag), false);
			
		}else if(tag->GetName() == "boneParameter"){
			const char * const name = GetCDataString(*tag);
			
			if(strcmp(name, "positionX") == 0){
				link->mpBoneParameter.SetValue(deAnimatorLink::ebpPositionX, false);
				
			}else if(strcmp(name, "positionY") == 0){
				link->mpBoneParameter.SetValue(deAnimatorLink::ebpPositionY, false);
				
			}else if(strcmp(name, "positionZ") == 0){
				link->mpBoneParameter.SetValue(deAnimatorLink::ebpPositionZ, false);
				
			}else if(strcmp(name, "rotationX") == 0){
				link->mpBoneParameter.SetValue(deAnimatorLink::ebpRotationX, false);
				
			}else if(strcmp(name, "rotationY") == 0){
				link->mpBoneParameter.SetValue(deAnimatorLink::ebpRotationY, false);
				
			}else if(strcmp(name, "rotationZ") == 0){
				link->mpBoneParameter.SetValue(deAnimatorLink::ebpRotationZ, false);
				
			}else if(strcmp(name, "scaleX") == 0){
				link->mpBoneParameter.SetValue(deAnimatorLink::ebpScaleX, false);
				
			}else if(strcmp(name, "scaleY") == 0){
				link->mpBoneParameter.SetValue(deAnimatorLink::ebpScaleY, false);
				
			}else if(strcmp(name, "scaleZ") == 0){
				link->mpBoneParameter.SetValue(deAnimatorLink::ebpScaleZ, false);
				
			}else{
				LogErrorUnknownValue(*tag, name);
			}
			
		}else if(tag->GetName() == "boneLimits"){
			link->mpBoneMinimum.SetValue(GetAttributeFloat(*tag, "min"), false);
			link->mpBoneMaximum.SetValue(GetAttributeFloat(*tag, "max"), false);
			
		}else if(tag->GetName() == "vertexPositionSet"){
			link->mpVertexPositionSet.SetValue(GetCDataString(*tag), false);
			
		}else if(tag->GetName() == "vertexPositionSetLimits"){
			link->mpVpsMinimum.SetValue(GetAttributeFloat(*tag, "min"), false);
			link->mpVpsMaximum.SetValue(GetAttributeFloat(*tag, "max"), false);
			
		}else if(tag->GetName() == "wrapY"){
			link->mpWrapY.SetValue(GetCDataBool(*tag), false);
			
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
	
	return {};
}

aeRule::Ref aeLSAnimator::pLoadRuleAnimation(decXmlElementTag *root, aeAnimator &animator){
	deLogger &logger = *pLSSys->GetWindowMain()->GetEnvironment().GetLogger();
	const aeRuleAnimation::Ref rule(aeRuleAnimation::Ref::New(*pLSSys->GetWindowMain(), ""));
	decXmlElementTag *tag;
	const char *name;
	int i;
	
	for(i=0; i<root->GetElementCount(); i++){
		tag = root->GetElementIfTag(i);
		if(tag){
			if(!pLoadRuleCommon(tag, animator, rule)){
				if(strcmp(tag->GetName(), "moveName") == 0){
					rule->mpMoveName.SetValue(GetCDataString(*tag), false);
					
				}else if(strcmp(tag->GetName(), "moveTime") == 0){
					rule->mpMoveTime.SetValue(strtof(GetCDataString(*tag), nullptr), false);
					
				}else if(strcmp(tag->GetName(), "enablePosition") == 0){
					rule->mpEnablePosition.SetValue(GetCDataBool(*tag), false);
					
				}else if(strcmp(tag->GetName(), "enableOrientation") == 0){
					rule->mpEnableOrientation.SetValue(GetCDataBool(*tag), false);
					
				}else if(strcmp(tag->GetName(), "enableSize") == 0){
					rule->mpEnableSize.SetValue(GetCDataBool(*tag), false);
					
				}else if(strcmp(tag->GetName(), "enableVertexPositionSet") == 0){
					rule->mpEnableVertexPositionSet.SetValue(GetCDataBool(*tag), false);
					
				}else if(strcmp(tag->GetName(), "target") == 0){
					name = pGetAttributeString(tag, "name");
					
					if(strcmp(name, "blendFactor") == 0){
						pLoadControllerTarget(tag, animator, rule->mpTargetBlendFactor);
						
					}else if(strcmp(name, "moveTime") == 0){
						pLoadControllerTarget(tag, animator, rule->mpTargetMoveTime);
						
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
	const aeRuleAnimationDifference::Ref rule(aeRuleAnimationDifference::Ref::New(*pLSSys->GetWindowMain(), ""));
	decXmlElementTag *tag;
	int i;
	
	for(i=0; i<root->GetElementCount(); i++){
		tag = root->GetElementIfTag(i);
		if(tag){
			if(!pLoadRuleCommon(tag, animator, rule)){
				if(strcmp(tag->GetName(), "leadingMoveName") == 0){
					rule->mpLeadingMoveName.SetValue(GetCDataString(*tag), false);
					
				}else if(strcmp(tag->GetName(), "leadingMoveTime") == 0){
					rule->mpLeadingMoveTime.SetValue(strtof(GetCDataString(*tag), nullptr), false);
					
				}else if(strcmp(tag->GetName(), "referenceMoveName") == 0){
					rule->mpReferenceMoveName.SetValue(GetCDataString(*tag), false);
					
				}else if(strcmp(tag->GetName(), "referenceMoveTime") == 0){
					rule->mpReferenceMoveTime.SetValue(strtof(GetCDataString(*tag), nullptr), false);
					
				}else if(strcmp(tag->GetName(), "enablePosition") == 0){
					rule->mpEnablePosition.SetValue(GetCDataBool(*tag), false);
					
				}else if(strcmp(tag->GetName(), "enableOrientation") == 0){
					rule->mpEnableOrientation.SetValue(GetCDataBool(*tag), false);
					
				}else if(strcmp(tag->GetName(), "enableSize") == 0){
					rule->mpEnableSize.SetValue(GetCDataBool(*tag), false);
					
				}else if(strcmp(tag->GetName(), "enableVertexPositionSet") == 0){
					rule->mpEnableVertexPositionSet.SetValue(GetCDataBool(*tag), false);
					
				}else if(strcmp(tag->GetName(), "useComponentSpace") == 0){
					rule->mpUseComponentSpace.SetValue(GetCDataBool(*tag), false);
					
				}else if(strcmp(tag->GetName(), "target") == 0){
					const decString &name = pGetAttributeString(tag, "name");
					
					if(name == "blendFactor"){
						pLoadControllerTarget(tag, animator, rule->mpTargetBlendFactor);
						
					}else if(name == "leadingMoveTime"){
						pLoadControllerTarget(tag, animator, rule->mpTargetLeadingMoveTime);
						
					}else if(name == "referenceMoveTime"){
						pLoadControllerTarget(tag, animator, rule->mpTargetReferenceMoveTime);
						
					}else{
						logger.LogErrorFormat("%s(%i:%i): Unknown target '%s'",
							root->GetName().GetString(), tag->GetLineNumber(),
							tag->GetPositionNumber(), name.GetString());
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
	const aeRuleAnimationSelect::Ref rule(aeRuleAnimationSelect::Ref::New(*pLSSys->GetWindowMain(), ""));
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
			rule->mpEnablePosition.SetValue(GetCDataBool(*tag), false);
			
		}else if(strcmp(tag->GetName(), "enableOrientation") == 0){
			rule->mpEnableOrientation.SetValue(GetCDataBool(*tag), false);
			
		}else if(strcmp(tag->GetName(), "enableSize") == 0){
			rule->mpEnableSize.SetValue(GetCDataBool(*tag), false);
			
		}else if(strcmp(tag->GetName(), "enableVertexPositionSet") == 0){
			rule->mpEnableVertexPositionSet.SetValue(GetCDataBool(*tag), false);
			
		}else if(strcmp(tag->GetName(), "target") == 0){
			name = pGetAttributeString(tag, "name");
			
			if(strcmp(name, "blendFactor") == 0){
				pLoadControllerTarget(tag, animator, rule->mpTargetBlendFactor);
				
			}else if(strcmp(name, "moveTime") == 0){
				pLoadControllerTarget(tag, animator, rule->mpTargetMoveTime);
				
			}else if(strcmp(name, "selection") == 0){
				pLoadControllerTarget(tag, animator, rule->mpTargetSelect);
				
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
	const aeRuleBoneTransformator::Ref rule(aeRuleBoneTransformator::Ref::New(*pLSSys->GetWindowMain(), ""));
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
					ReadVector(*tag, vector);
					rule->mpMinTranslation.SetValue(vector, false);
					
				}else if(strcmp(tag->GetName(), "maximumTranslation") == 0){
					vector.SetZero();
					ReadVector(*tag, vector);
					rule->mpMaxTranslation.SetValue(vector, false);
					
				}else if(strcmp(tag->GetName(), "minimumRotation") == 0){
					vector.SetZero();
					ReadVector(*tag, vector);
					rule->mpMinRotation.SetEulerAngles(vector, false);
					
				}else if(strcmp(tag->GetName(), "maximumRotation") == 0){
					vector.SetZero();
					ReadVector(*tag, vector);
					rule->mpMaxRotation.SetEulerAngles(vector, false);
					
				}else if(strcmp(tag->GetName(), "minimumScaling") == 0){
					vector.SetZero();
					ReadVector(*tag, vector);
					rule->mpMinScaling.SetValue(vector, false);
					
				}else if(strcmp(tag->GetName(), "maximumScaling") == 0){
					vector.SetZero();
					ReadVector(*tag, vector);
					rule->mpMaxScaling.SetValue(vector, false);
					
				}else if(strcmp(tag->GetName(), "axis") == 0){
					vector.SetZero();
					ReadVector(*tag, vector);
					rule->mpAxis.SetValue(vector, false);
					
				}else if(strcmp(tag->GetName(), "minimumAngle") == 0){
					rule->mpMinAngle.SetValue(GetCDataFloat(*tag), false);
					
				}else if(strcmp(tag->GetName(), "maximumAngle") == 0){
					rule->mpMaxAngle.SetValue(GetCDataFloat(*tag), false);
					
				}else if(strcmp(tag->GetName(), "cframe") == 0){
					name = GetCDataString(*tag);
					
					if(strcmp(name, "local") == 0){
						rule->mpCoordinateFrame.SetValue(deAnimatorRuleBoneTransformator::ecfBoneLocal, false);
						
					}else if(strcmp(name, "component") == 0){
						rule->mpCoordinateFrame.SetValue(deAnimatorRuleBoneTransformator::ecfComponent, false);
						
					}else if(strcmp(name, "target") == 0){
						rule->mpCoordinateFrame.SetValue(deAnimatorRuleBoneTransformator::ecfTargetBone, false);
						
					}else{
						logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown coordinate frame '%s'",
							root->GetName().GetString(), tag->GetLineNumber(),
							tag->GetPositionNumber(), name);
						DETHROW(deeInvalidFileFormat);
					}
					
				}else if(strcmp(tag->GetName(), "enablePosition") == 0){
					rule->mpEnablePosition.SetValue(GetCDataBool(*tag), false);
					
				}else if(strcmp(tag->GetName(), "enableOrientation") == 0){
					rule->mpEnableOrientation.SetValue(GetCDataBool(*tag), false);
					
				}else if(strcmp(tag->GetName(), "enableSize") == 0){
					rule->mpEnableSize.SetValue(GetCDataBool(*tag), false);
					
				}else if(strcmp(tag->GetName(), "useAxis") == 0){
					rule->mpUseAxis.SetValue(GetCDataBool(*tag), false);
					
				}else if(strcmp(tag->GetName(), "targetBone") == 0){
					rule->mpTargetBone.SetValue(GetCDataString(*tag), false);
					
				}else if(tag->GetName() == "inputBone"){
					rule->mpInputBone.SetValue(GetCDataString(*tag), false);
					
				}else if(tag->GetName() == "inputSource"){
					name = GetCDataString(*tag);
					
					if(strcmp(name, "targetBlend") == 0){
						rule->mpInputSource.SetValue(deAnimatorRuleBoneTransformator::eisTargetBlend, false);
						
					}else if(strcmp(name, "targetDirect") == 0){
						rule->mpInputSource.SetValue(deAnimatorRuleBoneTransformator::eisTargetDirect, false);
						
					}else if(strcmp(name, "boneState") == 0){
						rule->mpInputSource.SetValue(deAnimatorRuleBoneTransformator::eisBoneState, false);
						
					}else if(strcmp(name, "boneStateInverse") == 0){
						rule->mpInputSource.SetValue(deAnimatorRuleBoneTransformator::eisBoneStateInverse, false);
						
					}else{
						logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown coordinate frame '%s'",
							root->GetName().GetString(), tag->GetLineNumber(),
							tag->GetPositionNumber(), name);
						DETHROW(deeInvalidFileFormat);
					}
					
				}else if(strcmp(tag->GetName(), "target") == 0){
					name = pGetAttributeString(tag, "name");
					
					if(strcmp(name, "blendFactor") == 0){
						pLoadControllerTarget(tag, animator, rule->mpTargetBlendFactor);
						
					}else if(strcmp(name, "translation") == 0){
						pLoadControllerTarget(tag, animator, rule->mpTargetTranslation);
						
					}else if(strcmp(name, "rotation") == 0){
						pLoadControllerTarget(tag, animator, rule->mpTargetRotation);
						
					}else if(strcmp(name, "scaling") == 0){
						pLoadControllerTarget(tag, animator, rule->mpTargetScaling);
						
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
	const aeRuleStateManipulator::Ref rule(aeRuleStateManipulator::Ref::New(*pLSSys->GetWindowMain(), ""));
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
					ReadVector(*tag, vector);
					rule->mpMinPosition.SetValue(vector, false);
					
				}else if(strcmp(tag->GetName(), "positionMaximum") == 0){
					vector.SetZero();
					ReadVector(*tag, vector);
					rule->mpMaxPosition.SetValue(vector, false);
					
				}else if(strcmp(tag->GetName(), "rotationMinimum") == 0){
					vector.SetZero();
					ReadVector(*tag, vector);
					rule->mpMinRotation.SetEulerAngles(vector, false);
					
				}else if(strcmp(tag->GetName(), "rotationMaximum") == 0){
					vector.SetZero();
					ReadVector(*tag, vector);
					rule->mpMaxRotation.SetEulerAngles(vector, false);
					
				}else if(strcmp(tag->GetName(), "sizeMinimum") == 0){
					vector.SetZero();
					ReadVector(*tag, vector);
					rule->mpMinSize.SetValue(vector, false);
					
				}else if(strcmp(tag->GetName(), "sizeMaximum") == 0){
					vector.SetZero();
					ReadVector(*tag, vector);
					rule->mpMaxSize.SetValue(vector, false);
					
				}else if(strcmp(tag->GetName(), "vertexPositionSetMinimum") == 0){
					rule->mpMinVertexPositionSet.SetValue(GetCDataFloat(*tag), false);
					
				}else if(strcmp(tag->GetName(), "vertexPositionSetMaximum") == 0){
					rule->mpMaxVertexPositionSet.SetValue(GetCDataFloat(*tag), false);
					
				}else if(strcmp(tag->GetName(), "enablePosition") == 0){
					rule->mpEnablePosition.SetValue(GetCDataBool(*tag), false);
					
				}else if(strcmp(tag->GetName(), "enableOrientation") == 0){
					rule->mpEnableRotation.SetValue(GetCDataBool(*tag), false);
					
				}else if(strcmp(tag->GetName(), "enableSize") == 0){
					rule->mpEnableSize.SetValue(GetCDataBool(*tag), false);
					
				}else if(strcmp(tag->GetName(), "enableVertexPositionSet") == 0){
					rule->mpEnableVertexPositionSet.SetValue(GetCDataBool(*tag), false);
					
				}else if(strcmp(tag->GetName(), "target") == 0){
					name = pGetAttributeString(tag, "name");
					
					if(strcmp(name, "blendFactor") == 0){
						pLoadControllerTarget(tag, animator, rule->mpTargetBlendFactor);
						
					}else if(strcmp(name, "position") == 0){
						pLoadControllerTarget(tag, animator, rule->mpTargetPosition);
						
					}else if(strcmp(name, "orientation") == 0){
						pLoadControllerTarget(tag, animator, rule->mpTargetRotation);
						
					}else if(strcmp(name, "size") == 0){
						pLoadControllerTarget(tag, animator, rule->mpTargetSize);
						
					}else if(strcmp(name, "vertexPositionSet") == 0){
						pLoadControllerTarget(tag, animator, rule->mpTargetVertexPositionSet);
						
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
	const aeRuleStateSnapshot::Ref rule(aeRuleStateSnapshot::Ref::New(*pLSSys->GetWindowMain(), ""));
	decXmlElementTag *tag;
	decVector vector;
	const char *name;
	int i;
	
	for(i=0; i<root->GetElementCount(); i++){
		tag = root->GetElementIfTag(i);
		if(tag){
			if(!pLoadRuleCommon(tag, animator, rule)){
				if(strcmp(tag->GetName(), "useLastState") == 0){
					rule->mpUseLastState.SetValue(GetCDataBool(*tag), false);
					
				}else if(strcmp(tag->GetName(), "enablePosition") == 0){
					rule->mpEnablePosition.SetValue(GetCDataBool(*tag), false);
					
				}else if(strcmp(tag->GetName(), "enableOrientation") == 0){
					rule->mpEnableOrientation.SetValue(GetCDataBool(*tag), false);
					
				}else if(strcmp(tag->GetName(), "enableSize") == 0){
					rule->mpEnableSize.SetValue(GetCDataBool(*tag), false);
					
				}else if(strcmp(tag->GetName(), "enableVertexPositionSet") == 0){
					rule->mpEnableVertexPositionSet.SetValue(GetCDataBool(*tag), false);
					
				}else if(strcmp(tag->GetName(), "target") == 0){
					name = pGetAttributeString(tag, "name");
					
					if(strcmp(name, "blendFactor") == 0){
						pLoadControllerTarget(tag, animator, rule->mpTargetBlendFactor);
						
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
	const aeRuleInverseKinematic::Ref rule(aeRuleInverseKinematic::Ref::New(*pLSSys->GetWindowMain(), ""));
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
					ReadVector(*tag, vector);
					rule->mpGoalPosition.SetValue(vector, false);
					
				}else if(strcmp(tag->GetName(), "goalOrientation") == 0){
					vector.SetZero();
					ReadVector(*tag, vector);
					rule->mpGoalOrientation.SetEulerAngles(vector, false);
					
				}else if(strcmp(tag->GetName(), "localPosition") == 0){
					vector.SetZero();
					ReadVector(*tag, vector);
					rule->mpLocalPosition.SetValue(vector, false);
					
				}else if(strcmp(tag->GetName(), "localOrientation") == 0){
					vector.SetZero();
					ReadVector(*tag, vector);
					rule->mpLocalOrientation.SetEulerAngles(vector, false);
					
				}else if(strcmp(tag->GetName(), "adjustOrientation") == 0){
					rule->mpAdjustOrientation.SetValue(GetCDataBool(*tag), false);
					
				}else if(strcmp(tag->GetName(), "position") == 0){ // deprecated
					vector.SetZero();
					ReadVector(*tag, vector);
					rule->mpGoalPosition.SetValue(vector, false);
					
				}else if(strcmp(tag->GetName(), "orientation") == 0){ // deprecated
					vector.SetZero();
					ReadVector(*tag, vector);
					rule->mpGoalOrientation.SetEulerAngles(vector, false);
					
				}else if(strcmp(tag->GetName(), "offset") == 0){ // deprecated
					vector.SetZero();
					ReadVector(*tag, vector);
					rule->mpLocalPosition.SetValue(vector, false);
					
				}else if(strcmp(tag->GetName(), "solverBone") == 0){
					cdata = tag->GetFirstData();
					
					if(cdata){
						rule->mpSolverBone.SetValue(cdata->GetData(), false);
						
					}else{
						rule->mpSolverBone.SetValue("", false);
					}
					
				}else if(strcmp(tag->GetName(), "useSolverBone") == 0){
					rule->mpUseSolverBone.SetValue(GetCDataBool(*tag), false);
					
				}else if(strcmp(tag->GetName(), "reachRange") == 0){
					rule->mpReachRange.SetValue(strtof(GetCDataString(*tag), nullptr), false);
					
				}else if(strcmp(tag->GetName(), "reachBone") == 0){
					cdata = tag->GetFirstData();
					
					if(cdata){
						rule->mpReachBone.SetValue(cdata->GetData(), false);
						
					}else{
						rule->mpReachBone.SetValue("", false);
					}
					
				}else if(strcmp(tag->GetName(), "reachCenter") == 0){
					vector.SetZero();
					ReadVector(*tag, vector);
					rule->mpReachCenter.SetValue(vector, false);
					
				}else if(strcmp(tag->GetName(), "target") == 0){
					name = pGetAttributeString(tag, "name");
					
					if(strcmp(name, "blendFactor") == 0){
						pLoadControllerTarget(tag, animator, rule->mpTargetBlendFactor);
						
					}else if(strcmp(name, "goalPosition") == 0){
						pLoadControllerTarget(tag, animator, rule->mpTargetGoalPosition);
						
					}else if(strcmp(name, "goalOrientation") == 0){
						pLoadControllerTarget(tag, animator, rule->mpTargetGoalOrientation);
						
					}else if(strcmp(name, "localPosition") == 0){
						pLoadControllerTarget(tag, animator, rule->mpTargetLocalPosition);
						
					}else if(strcmp(name, "localOrientation") == 0){
						pLoadControllerTarget(tag, animator, rule->mpTargetLocalOrientation);
						
					}else if(strcmp(name, "reachRange") == 0){
						pLoadControllerTarget(tag, animator, rule->mpTargetReachRange);
						
					}else if(strcmp(name, "reachCenter") == 0){
						pLoadControllerTarget(tag, animator, rule->mpTargetReachCenter);
						
					}else if(strcmp(name, "position") == 0){ // deprecated
						pLoadControllerTarget(tag, animator, rule->mpTargetGoalPosition);
						
					}else if(strcmp(name, "orientation") == 0){ // deprecated
						pLoadControllerTarget(tag, animator, rule->mpTargetGoalOrientation);
						
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
	const aeRuleForeignState::Ref rule(aeRuleForeignState::Ref::New(*pLSSys->GetWindowMain(), ""));
	decXmlElementTag *tag;
	const char *name;
	int i;
	
	for(i=0; i<root->GetElementCount(); i++){
		tag = root->GetElementIfTag(i);
		if(tag){
			if(!pLoadRuleCommon(tag, animator, rule)){
				if(strcmp(tag->GetName(), "scalePosition") == 0){
					rule->mpScalePosition.SetValue(GetCDataFloat(*tag), false);
					
				}else if(strcmp(tag->GetName(), "scaleOrientation") == 0){
					rule->mpScaleOrientation.SetValue(GetCDataFloat(*tag), false);
					
				}else if(strcmp(tag->GetName(), "scaleSize") == 0){
					rule->mpScaleSize.SetValue(GetCDataFloat(*tag), false);
					
				}else if(strcmp(tag->GetName(), "scaleVertexPositionSet") == 0){
					rule->mpScaleVertexPositionSet.SetValue(GetCDataFloat(*tag), false);
					
				}else if(strcmp(tag->GetName(), "enablePosition") == 0){
					rule->mpEnablePosition.SetValue(GetCDataBool(*tag), false);
					
				}else if(strcmp(tag->GetName(), "enableOrientation") == 0){
					rule->mpEnableOrientation.SetValue(GetCDataBool(*tag), false);
					
				}else if(strcmp(tag->GetName(), "enableSize") == 0){
					rule->mpEnableSize.SetValue(GetCDataBool(*tag), false);
					
				}else if(strcmp(tag->GetName(), "enableVertexPositionSet") == 0){
					rule->mpEnableVertexPositionSet.SetValue(GetCDataBool(*tag), false);
					
				}else if(strcmp(tag->GetName(), "foreignBone") == 0){
					rule->mpForeignBone.SetValue(GetCDataString(*tag), false);
					
				}else if(strcmp(tag->GetName(), "foreignVertexPositionSet") == 0){
					rule->mpForeignVertexPositionSet.SetValue(GetCDataString(*tag), false);
					
				}else if(strcmp(tag->GetName(), "srcCFrame") == 0){
					name = GetCDataString(*tag);
					
					if(strcmp(name, "local") == 0){
						rule->mpSourceCoordinateFrame.SetValue(deAnimatorRuleForeignState::ecfBoneLocal, false);
						
					}else if(strcmp(name, "component") == 0){
						rule->mpSourceCoordinateFrame.SetValue(deAnimatorRuleForeignState::ecfComponent, false);
						
					}else{
						logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown coordinate frame '%s'",
							root->GetName().GetString(), tag->GetLineNumber(),
							tag->GetPositionNumber(), name);
						DETHROW(deeInvalidFileFormat);
					}
					
				}else if(strcmp(tag->GetName(), "destCFrame") == 0){
					name = GetCDataString(*tag);
					
					if(strcmp(name, "local") == 0){
						rule->mpDestCoordinateFrame.SetValue(deAnimatorRuleForeignState::ecfBoneLocal, false);
						
					}else if(strcmp(name, "component") == 0){
						rule->mpDestCoordinateFrame.SetValue(deAnimatorRuleForeignState::ecfComponent, false);
						
					}else{
						logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown coordinate frame '%s'",
							root->GetName().GetString(), tag->GetLineNumber(),
							tag->GetPositionNumber(), name);
						DETHROW(deeInvalidFileFormat);
					}
					
				}else if(strcmp(tag->GetName(), "target") == 0){
					name = pGetAttributeString(tag, "name");
					
					if(strcmp(name, "blendFactor") == 0){
						pLoadControllerTarget(tag, animator, rule->mpTargetBlendFactor);
						
					}else if(strcmp(name, "position") == 0){
						pLoadControllerTarget(tag, animator, rule->mpTargetPosition);
						
					}else if(strcmp(name, "orientation") == 0){
						pLoadControllerTarget(tag, animator, rule->mpTargetOrientation);
						
					}else if(strcmp(name, "size") == 0){
						pLoadControllerTarget(tag, animator, rule->mpTargetSize);
						
					}else if(strcmp(name, "vertexPositionSet") == 0){
						pLoadControllerTarget(tag, animator, rule->mpTargetVertexPositionSet);
						
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
	const aeRuleMirror::Ref rule(aeRuleMirror::Ref::New(*pLSSys->GetWindowMain(), ""));
	aeMCPRuleMirrorMatchNames::ListType matchNames;
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
			rule->mpEnablePosition.SetValue(GetCDataBool(*tag), false);
			
		}else if(tag->GetName() == "enableOrientation"){
			rule->mpEnableOrientation.SetValue(GetCDataBool(*tag), false);
			
		}else if(tag->GetName() == "enableSize"){
			rule->mpEnableSize.SetValue(GetCDataBool(*tag), false);
			
		}else if(tag->GetName() == "enableVertexPositionSet"){
			rule->mpEnableVertexPositionSet.SetValue(GetCDataBool(*tag), false);
			
		}else if(tag->GetName() == "mirrorAxis"){
			const decString &name = GetCDataString(*tag);
			
			if(name == "x"){
				rule->mpMirrorAxis.SetValue(deAnimatorRuleMirror::emaX, false);
				
			}else if(name == "y"){
				rule->mpMirrorAxis.SetValue(deAnimatorRuleMirror::emaY, false);
				
			}else if(name == "z"){
				rule->mpMirrorAxis.SetValue(deAnimatorRuleMirror::emaZ, false);
				
			}else{
				logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown mirror axis '%s'",
					root->GetName().GetString(), tag->GetLineNumber(),
					tag->GetPositionNumber(), name.GetString());
				DETHROW(deeInvalidFileFormat);
			}
			
		}else if(tag->GetName() == "mirrorBone"){
			rule->mpMirrorBone.SetValue(GetCDataString(*tag), false);
			
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
			
			matchNames.Add(aeRuleMirror::MatchName::Ref::New(animator.GetWindowMain(), first, second, type));
			
		}else if(strcmp(tag->GetName(), "target") == 0){
			const decString &name = pGetAttributeString(tag, "name");
			
			if(name == "blendFactor"){
				pLoadControllerTarget(tag, animator, rule->mpTargetBlendFactor);
				
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
	rule->mpMatchNames.SetValue(matchNames, false);
	return rule;
}

aeRule::Ref aeLSAnimator::pLoadRuleGroup(decXmlElementTag *root, aeAnimator &animator){
	deLogger &logger = *pLSSys->GetWindowMain()->GetEnvironment().GetLogger();
	const int elementCount = root->GetElementCount();
	const aeRuleGroup::Ref rule(aeRuleGroup::Ref::New(*pLSSys->GetWindowMain(), ""));
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
				rule->mpApplicationType.SetValue(deAnimatorRuleGroup::eatAll, false);
				
			}else if(strcmp(name, "selection") == 0){
				rule->mpApplicationType.SetValue(deAnimatorRuleGroup::eatSelect, false);
				
			}else{
				logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown type '%s'",
					root->GetName().GetString(), tag->GetLineNumber(),
					tag->GetPositionNumber(), name);
				DETHROW(deeInvalidFileFormat);
			}
			
		}else if(strcmp(tag->GetName(), "useCurrentState") == 0){
			rule->mpUseCurrentState.SetValue(GetCDataBool(*tag), false);
			
		}else if(strcmp(tag->GetName(), "enablePosition") == 0){
			rule->mpEnablePosition.SetValue(GetCDataBool(*tag), false);
			
		}else if(strcmp(tag->GetName(), "enableOrientation") == 0){
			rule->mpEnableOrientation.SetValue(GetCDataBool(*tag), false);
			
		}else if(strcmp(tag->GetName(), "enableSize") == 0){
			rule->mpEnableSize.SetValue(GetCDataBool(*tag), false);
			
		}else if(strcmp(tag->GetName(), "enableVertexPositionSet") == 0){
			rule->mpEnableVertexPositionSet.SetValue(GetCDataBool(*tag), false);
			
		}else if(strcmp(tag->GetName(), "target") == 0){
			const char * const name = pGetAttributeString(tag, "name");
			
			if(strcmp(name, "blendFactor") == 0){
				pLoadControllerTarget(tag, animator, rule->mpTargetBlendFactor);
				
			}else if(strcmp(name, "selection") == 0){
				pLoadControllerTarget(tag, animator, rule->mpTargetSelect);
				
			}else{
				logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown target '%s'",
					root->GetName().GetString(), tag->GetLineNumber(),
					tag->GetPositionNumber(), name);
				DETHROW(deeInvalidFileFormat);
			}
			
		}else{
			const aeRule::Ref srule(pLoadRule(tag, animator));
			
			if(srule){
				rule->AddRule(srule);
				
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
	const aeRuleSubAnimator::Ref rule(aeRuleSubAnimator::Ref::New(*pLSSys->GetWindowMain(), ""));
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
					rule->mpPathSubAnimator.SetValue(cdata->GetData(), false);
					
				}else{
					rule->mpPathSubAnimator.SetValue("", false);
				}
				
			}else if(strcmp(tag->GetName(), "connection") == 0){
				const int controller = pGetAttributeInt(tag, "controller");
				const decString target(GetCDataString(*tag));
				if(target.IsEmpty() || !rule->GetSubAnimator()){
					continue;
				}
				
				const int targetIndex = rule->GetSubAnimator()->GetControllers().IndexOfNamed(target);
				if(targetIndex == -1){
					continue;
				}
				
				rule->SetControllerAt(targetIndex, animator.GetControllers().GetAt(controller));
				
			}else if(strcmp(tag->GetName(), "enablePosition") == 0){
				rule->mpEnablePosition.SetValue(GetCDataBool(*tag), false);
				
			}else if(strcmp(tag->GetName(), "enableOrientation") == 0){
				rule->mpEnableOrientation.SetValue(GetCDataBool(*tag), false);
				
			}else if(strcmp(tag->GetName(), "enableSize") == 0){
				rule->mpEnableSize.SetValue(GetCDataBool(*tag), false);
				
			}else if(strcmp(tag->GetName(), "enableVertexPositionSet") == 0){
				rule->mpEnableVertexPositionSet.SetValue(GetCDataBool(*tag), false);
				
			}else if(strcmp(tag->GetName(), "target") == 0){
				name = pGetAttributeString(tag, "name");
				
				if(strcmp(name, "blendFactor") == 0){
					pLoadControllerTarget(tag, animator, rule->mpTargetBlendFactor);
					
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
	const aeRuleTrackTo::Ref rule(aeRuleTrackTo::Ref::New(*pLSSys->GetWindowMain(), ""));
	decXmlElementTag *tag;
	const char *name;
	int i;
	
	for(i=0; i<root->GetElementCount(); i++){
		tag = root->GetElementIfTag(i);
		if(tag){
			if(!pLoadRuleCommon(tag, animator, rule)){
				if(strcmp(tag->GetName(), "trackBone") == 0){
					if(tag->GetFirstData()){
						rule->mpTrackBone.SetValue(GetCDataString(*tag), false);
					}
					
				}else if(strcmp(tag->GetName(), "trackAxis") == 0){
					name = GetCDataString(*tag);
					
					if(strcmp(name, "posX") == 0){
						rule->mpTrackAxis.SetValue(deAnimatorRuleTrackTo::etaPosX, false);
						
					}else if(strcmp(name, "posY") == 0){
						rule->mpTrackAxis.SetValue(deAnimatorRuleTrackTo::etaPosY, false);
						
					}else if(strcmp(name, "posZ") == 0){
						rule->mpTrackAxis.SetValue(deAnimatorRuleTrackTo::etaPosZ, false);
						
					}else if(strcmp(name, "negX") == 0){
						rule->mpTrackAxis.SetValue(deAnimatorRuleTrackTo::etaNegX, false);
						
					}else if(strcmp(name, "negY") == 0){
						rule->mpTrackAxis.SetValue(deAnimatorRuleTrackTo::etaNegY, false);
						
					}else if(strcmp(name, "negZ") == 0){
						rule->mpTrackAxis.SetValue(deAnimatorRuleTrackTo::etaNegZ, false);
						
					}else{
						logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown value '%s'",
							root->GetName().GetString(), tag->GetLineNumber(),
							tag->GetPositionNumber(), name);
						DETHROW(deeInvalidFileFormat);
					}
					
				}else if(strcmp(tag->GetName(), "upAxis") == 0){
					name = GetCDataString(*tag);
					
					if(strcmp(name, "posX") == 0){
						rule->mpUpAxis.SetValue(deAnimatorRuleTrackTo::etaPosX, false);
						
					}else if(strcmp(name, "posY") == 0){
						rule->mpUpAxis.SetValue(deAnimatorRuleTrackTo::etaPosY, false);
						
					}else if(strcmp(name, "posZ") == 0){
						rule->mpUpAxis.SetValue(deAnimatorRuleTrackTo::etaPosZ, false);
						
					}else if(strcmp(name, "negX") == 0){
						rule->mpUpAxis.SetValue(deAnimatorRuleTrackTo::etaNegX, false);
						
					}else if(strcmp(name, "negY") == 0){
						rule->mpUpAxis.SetValue(deAnimatorRuleTrackTo::etaNegY, false);
						
					}else if(strcmp(name, "negZ") == 0){
						rule->mpUpAxis.SetValue(deAnimatorRuleTrackTo::etaNegZ, false);
						
					}else{
						logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown value '%s'",
							root->GetName().GetString(), tag->GetLineNumber(),
							tag->GetPositionNumber(), name);
						DETHROW(deeInvalidFileFormat);
					}
					
				}else if(strcmp(tag->GetName(), "lockedAxis") == 0){
					name = GetCDataString(*tag);
					
					if(strcmp(name, "none") == 0){
						rule->mpLockedAxis.SetValue(deAnimatorRuleTrackTo::elaNone, false);
						
					}else if(strcmp(name, "x") == 0){
						rule->mpLockedAxis.SetValue(deAnimatorRuleTrackTo::elaX, false);
						
					}else if(strcmp(name, "y") == 0){
						rule->mpLockedAxis.SetValue(deAnimatorRuleTrackTo::elaY, false);
						
					}else if(strcmp(name, "z") == 0){
						rule->mpLockedAxis.SetValue(deAnimatorRuleTrackTo::elaZ, false);
						
					}else{
						logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown value '%s'",
							root->GetName().GetString(), tag->GetLineNumber(),
							tag->GetPositionNumber(), name);
						DETHROW(deeInvalidFileFormat);
					}
					
				}else if(strcmp(tag->GetName(), "upTarget") == 0){
					name = GetCDataString(*tag);
					
					if(strcmp(name, "worldX") == 0){
						rule->mpUpTarget.SetValue(deAnimatorRuleTrackTo::eutWorldX, false);
						
					}else if(strcmp(name, "worldY") == 0){
						rule->mpUpTarget.SetValue(deAnimatorRuleTrackTo::eutWorldY, false);
						
					}else if(strcmp(name, "worldZ") == 0){
						rule->mpUpTarget.SetValue(deAnimatorRuleTrackTo::eutWorldZ, false);
						
					}else if(strcmp(name, "componentX") == 0){
						rule->mpUpTarget.SetValue(deAnimatorRuleTrackTo::eutComponentX, false);
						
					}else if(strcmp(name, "componentY") == 0){
						rule->mpUpTarget.SetValue(deAnimatorRuleTrackTo::eutComponentY, false);
						
					}else if(strcmp(name, "componentZ") == 0){
						rule->mpUpTarget.SetValue(deAnimatorRuleTrackTo::eutComponentZ, false);
						
					}else if(strcmp(name, "trackBoneX") == 0){
						rule->mpUpTarget.SetValue(deAnimatorRuleTrackTo::eutTrackBoneX, false);
						
					}else if(strcmp(name, "trackBoneY") == 0){
						rule->mpUpTarget.SetValue(deAnimatorRuleTrackTo::eutTrackBoneY, false);
						
					}else if(strcmp(name, "trackBoneZ") == 0){
						rule->mpUpTarget.SetValue(deAnimatorRuleTrackTo::eutTrackBoneZ, false);
						
					}else if(strcmp(name, "controller") == 0){
						rule->mpUpTarget.SetValue(deAnimatorRuleTrackTo::eutController, false);
						
					}else{
						logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown value '%s'",
							root->GetName().GetString(), tag->GetLineNumber(),
							tag->GetPositionNumber(), name);
						DETHROW(deeInvalidFileFormat);
					}
					
				}else if(strcmp(tag->GetName(), "target") == 0){
					name = pGetAttributeString(tag, "name");
					
					if(strcmp(name, "blendFactor") == 0){
						pLoadControllerTarget(tag, animator, rule->mpTargetBlendFactor);
						
					}else if(strcmp(name, "position") == 0){
						pLoadControllerTarget(tag, animator, rule->mpTargetPosition);
						
					}else if(strcmp(name, "up") == 0){
						pLoadControllerTarget(tag, animator, rule->mpTargetUp);
						
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
	const aeRuleLimit::Ref rule(aeRuleLimit::Ref::New(*pLSSys->GetWindowMain(), ""));
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
					ReadVector(*tag, vector);
					rule->mpMinPosition.SetValue(vector, false);
					
				}else if(strcmp(tag->GetName(), "maximumPosition") == 0){
					vector.SetZero();
					ReadVector(*tag, vector);
					rule->mpMaxPosition.SetValue(vector, false);
					
				}else if(strcmp(tag->GetName(), "minimumRotation") == 0){
					vector.SetZero();
					ReadVector(*tag, vector);
					rule->mpMinRotation.SetEulerAngles(vector, false);
					
				}else if(strcmp(tag->GetName(), "maximumRotation") == 0){
					vector.SetZero();
					ReadVector(*tag, vector);
					rule->mpMaxRotation.SetEulerAngles(vector, false);
					
				}else if(strcmp(tag->GetName(), "minimumScaling") == 0){
					vector.SetZero();
					ReadVector(*tag, vector);
					rule->mpMinScaling.SetValue(vector, false);
					
				}else if(strcmp(tag->GetName(), "maximumScaling") == 0){
					vector.SetZero();
					ReadVector(*tag, vector);
					rule->mpMaxScaling.SetValue(vector, false);
					
				}else if(strcmp(tag->GetName(), "minimumVertexPositionSet") == 0){
					rule->mpMinVertexPositionSet.SetValue(GetCDataFloat(*tag), false);
					
				}else if(strcmp(tag->GetName(), "maximumVertexPositionSet") == 0){
					rule->mpMaxVertexPositionSet.SetValue(GetCDataFloat(*tag), false);
					
				}else if(strcmp(tag->GetName(), "cframe") == 0){
					name = GetCDataString(*tag);
					
					if(strcmp(name, "local") == 0){
						rule->mpCoordinateFrame.SetValue(deAnimatorRuleLimit::ecfBoneLocal, false);
						
					}else if(strcmp(name, "component") == 0){
						rule->mpCoordinateFrame.SetValue(deAnimatorRuleLimit::ecfComponent, false);
						
					}else if(strcmp(name, "target") == 0){
						rule->mpCoordinateFrame.SetValue(deAnimatorRuleLimit::ecfTargetBone, false);
						
					}else{
						logger.LogErrorFormat(LOGSOURCE, "%s(%i:%i): Unknown coordinate frame '%s'",
							root->GetName().GetString(), tag->GetLineNumber(),
							tag->GetPositionNumber(), name);
						DETHROW(deeInvalidFileFormat);
					}
					
				}else if(strcmp(tag->GetName(), "enablePosMin") == 0){
					rule->mpEnablePositionXMin.SetValue(pGetAttributeInt(tag, "x") != 0, false);
					rule->mpEnablePositionYMin.SetValue(pGetAttributeInt(tag, "y") != 0, false);
					rule->mpEnablePositionZMin.SetValue(pGetAttributeInt(tag, "z") != 0, false);
					
				}else if(strcmp(tag->GetName(), "enablePosMax") == 0){
					rule->mpEnablePositionXMax.SetValue(pGetAttributeInt(tag, "x") != 0, false);
					rule->mpEnablePositionYMax.SetValue(pGetAttributeInt(tag, "y") != 0, false);
					rule->mpEnablePositionZMax.SetValue(pGetAttributeInt(tag, "z") != 0, false);
					
				}else if(strcmp(tag->GetName(), "enableRotMin") == 0){
					rule->mpEnableRotationXMin.SetValue(pGetAttributeInt(tag, "x") != 0, false);
					rule->mpEnableRotationYMin.SetValue(pGetAttributeInt(tag, "y") != 0, false);
					rule->mpEnableRotationZMin.SetValue(pGetAttributeInt(tag, "z") != 0, false);
					
				}else if(strcmp(tag->GetName(), "enableRotMax") == 0){
					rule->mpEnableRotationXMax.SetValue(pGetAttributeInt(tag, "x") != 0, false);
					rule->mpEnableRotationYMax.SetValue(pGetAttributeInt(tag, "y") != 0, false);
					rule->mpEnableRotationZMax.SetValue(pGetAttributeInt(tag, "z") != 0, false);
					
				}else if(strcmp(tag->GetName(), "enableScaleMin") == 0){
					rule->mpEnableScalingXMin.SetValue(pGetAttributeInt(tag, "x") != 0, false);
					rule->mpEnableScalingYMin.SetValue(pGetAttributeInt(tag, "y") != 0, false);
					rule->mpEnableScalingZMin.SetValue(pGetAttributeInt(tag, "z") != 0, false);
					
				}else if(strcmp(tag->GetName(), "enableScaleMax") == 0){
					rule->mpEnableScalingXMax.SetValue(pGetAttributeInt(tag, "x") != 0, false);
					rule->mpEnableScalingYMax.SetValue(pGetAttributeInt(tag, "y") != 0, false);
					rule->mpEnableScalingZMax.SetValue(pGetAttributeInt(tag, "z") != 0, false);
					
				}else if(strcmp(tag->GetName(), "enableVertexPositionSetMin") == 0){
					rule->mpEnableVertexPositionSetMin.SetValue(GetCDataBool(*tag), false);
					
				}else if(strcmp(tag->GetName(), "enableVertexPositionSetMax") == 0){
					rule->mpEnableVertexPositionSetMax.SetValue(GetCDataBool(*tag), false);
					
				}else if(strcmp(tag->GetName(), "targetBone") == 0){
					rule->mpTargetBone.SetValue(GetCDataString(*tag), false);
					
				}else if(strcmp(tag->GetName(), "target") == 0){
					name = pGetAttributeString(tag, "name");
					
					if(strcmp(name, "blendFactor") == 0){
						pLoadControllerTarget(tag, animator, rule->mpTargetBlendFactor);
						
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
				rule.mpBlendMode.SetValue(deAnimatorRule::ebmBlend, false);
				
			}else if(strcmp(blendMode, "overlay") == 0){
				rule.mpBlendMode.SetValue(deAnimatorRule::ebmOverlay, false);
				
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
		rule.mpBlendFactor.SetValue(GetCDataFloat(*tag), false);
		return true;
		
	}else if(tag->GetName() == "invertBlendFactor"){
		rule.mpInvertBlendFactor.SetValue(GetCDataBool(*tag), false);
		return true;
		
	}else if(strcmp(tag->GetName(), "bone") == 0){
		rule.mpAffectedBones.SetValue(rule.mpAffectedBones.GetValue() + decStringSet(devctag, GetCDataString(*tag)), false);
		return true;
		
	}else if(strcmp(tag->GetName(), "vertexPositionSet") == 0){
		rule.mpAffectedVps.SetValue(rule.mpAffectedVps.GetValue() + decStringSet(devctag, GetCDataString(*tag)), false);
		return true;
	}
	
	return false;
}

void aeLSAnimator::pLoadControllerTarget(decXmlElementTag *root, aeAnimator &animator, igdeMetaPropertyObjectSetStorage<aeLink>::Storage &target){
	deLogger &logger = *pLSSys->GetWindowMain()->GetEnvironment().GetLogger();
	decTObjectSet<aeLink> links;
	decXmlElementTag *tag;
	int i;
	
	// parse tag
	for(i=0; i<root->GetElementCount(); i++){
		tag = root->GetElementIfTag(i);
		if(tag){
			if(strcmp(tag->GetName(), "link") == 0){
				links.Add(animator.GetLinks()[GetCDataInt(*tag)]);
				
			}else{
				logger.LogWarnFormat(LOGSOURCE, "%s(%i:%i): Unknown Tag %s, ignoring",
					root->GetName().GetString(), tag->GetLineNumber(),
					tag->GetPositionNumber(), tag->GetName().GetString());
			}
		}
	}
	
	target.SetValue(links, false);
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
