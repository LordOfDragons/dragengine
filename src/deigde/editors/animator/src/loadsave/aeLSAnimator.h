/* 
 * Drag[en]gine IGDE Animator Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _AELSANIMATOR_H_
#define _AELSANIMATOR_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

#include <deigde/utils/igdeBaseXML.h>

class aeRuleGroup;
class aeRuleLimit;
class aeAnimator;
class aeController;
class aeLink;
class aeRule;
class aeRuleAnimation;
class aeRuleAnimationDifference;
class aeRuleAnimationSelect;
class aeRuleBoneTransformator;
class aeRuleStateManipulator;
class aeRuleStateSnapshot;
class aeRuleInverseKinematic;
class aeRuleForeignState;
class aeRuleSubAnimator;
class aeRuleTrackTo;
class aeControllerTarget;
class aeLoadSaveSystem;

class aeController;
class decXmlWriter;
class decXmlElementTag;
class decBaseFileReader;
class decBaseFileWriter;
class deAnimatorLink;



/**
 * Loads and saves actor animators in the Drag[en]gine Actor Animator XML format.
 */
class aeLSAnimator : public igdeBaseXML{
private:
	aeLoadSaveSystem *pLSSys;
	
	decString pName;
	decString pPattern;
	
public:
	// constructor, destructor
	aeLSAnimator( aeLoadSaveSystem *lssys );
	~aeLSAnimator();
	
	// management
	inline const decString &GetName() const{ return pName; }
	void SetName( const char *name );
	inline const decString &GetPattern() const{ return pPattern; }
	void SetPattern( const char *pattern );
	
	// loading and saving
	void LoadAnimator( aeAnimator *animator, decBaseFileReader *file );
	void SaveAnimator( aeAnimator *animator, decBaseFileWriter *file );
	
private:
	void pSaveDisplay( decXmlWriter &writer, aeAnimator *animator );
	void pSaveLocomotion( decXmlWriter &writer, aeAnimator *animator );
	void pSaveController( decXmlWriter &writer, aeController *controller );
	void pSaveLink( decXmlWriter &writer, aeAnimator *animator, aeLink *link );
	void pSaveRuleCommon( decXmlWriter &writer, aeAnimator *animator, aeRule *rule );
	void pSaveControllerTarget( decXmlWriter &writer, aeAnimator *animator, const aeControllerTarget &target, const char *name );
	void pSaveRule( decXmlWriter &writer, aeAnimator *animator, aeRule *rule );
	void pSaveRuleAnimation( decXmlWriter &writer, aeAnimator *animator, aeRuleAnimation *rule );
	void pSaveRuleAnimationDifference( decXmlWriter &writer, aeAnimator *animator, aeRuleAnimationDifference *rule );
	void pSaveRuleAnimationSelect( decXmlWriter &writer, aeAnimator *animator, aeRuleAnimationSelect *rule );
	void pSaveRuleBoneTransformator( decXmlWriter &writer, aeAnimator *animator, aeRuleBoneTransformator *rule );
	void pSaveRuleStateManipulator( decXmlWriter &writer, aeAnimator *animator, aeRuleStateManipulator *rule );
	void pSaveRuleStateSnapshot( decXmlWriter &writer, aeAnimator *animator, aeRuleStateSnapshot *rule );
	void pSaveRuleInverseKinematic( decXmlWriter &writer, aeAnimator *animator, aeRuleInverseKinematic *rule );
	void pSaveRuleForeignState( decXmlWriter &writer, aeAnimator *animator, aeRuleForeignState *rule );
	void pSaveRuleGroup( decXmlWriter &writer, aeAnimator *animator, aeRuleGroup *rule );
	void pSaveRuleSubAnimator( decXmlWriter &writer, aeAnimator *animator, aeRuleSubAnimator *rule );
	void pSaveRuleTrackTo( decXmlWriter &writer, aeAnimator *animator, aeRuleTrackTo *rule );
	void pSaveRuleLimit( decXmlWriter &writer, aeAnimator *animator, aeRuleLimit *rule );
	
	bool pHasAttribute( decXmlElementTag *tag, const char *name ) const;
	const char *pGetAttributeString( decXmlElementTag *tag, const char *name ) const;
	int pGetAttributeInt( decXmlElementTag *tag, const char *name ) const;
	float pGetAttributeFloat( decXmlElementTag *tag, const char *name ) const;
	
	void pLoadAnimator( decXmlElementTag *root, aeAnimator *animator );
	void pLoadDisplay( decXmlElementTag *root, aeAnimator *animator );
	void pLoadLocomotion( decXmlElementTag *root, aeAnimator *animator );
	void pLoadLocomotionLeg( decXmlElementTag *root, aeAnimator *animator, int leg );
	void pLoadController( decXmlElementTag *root, aeAnimator *animator );
	void pLoadControllerLimit( decXmlElementTag *root, aeController *controller );
	void pLoadLink( decXmlElementTag *root, aeAnimator *animator );
	aeRule *pLoadRule( decXmlElementTag *root, aeAnimator *animator );
	aeRule *pLoadRuleAnimation( decXmlElementTag *root, aeAnimator *animator );
	aeRule *pLoadRuleAnimationDifference( decXmlElementTag *root, aeAnimator *animator );
	aeRule *pLoadRuleAnimationSelect( decXmlElementTag *root, aeAnimator *animator );
	aeRule *pLoadRuleLimit( decXmlElementTag *root, aeAnimator *animator );
	aeRule *pLoadRuleStateManipulator( decXmlElementTag *root, aeAnimator *animator );
	aeRule *pLoadRuleStateSnapshot( decXmlElementTag *root, aeAnimator *animator );
	aeRule *pLoadRuleInverseKinematic( decXmlElementTag *root, aeAnimator *animator );
	aeRule *pLoadRuleForeignState( decXmlElementTag *root, aeAnimator *animator );
	aeRule *pLoadRuleGroup( decXmlElementTag *root, aeAnimator *animator );
	aeRule *pLoadRuleSubAnimator( decXmlElementTag *root, aeAnimator *animator );
	aeRule *pLoadRuleTrackTo( decXmlElementTag *root, aeAnimator *animator );
	aeRule *pLoadRuleBoneTransformator( decXmlElementTag *root, aeAnimator *animator );
	bool pLoadRuleCommon( decXmlElementTag *root, aeAnimator *animator, aeRule *rule );
	void pLoadControllerTarget( decXmlElementTag *root, aeAnimator *animator, aeControllerTarget &target );
	void pLoadVector( decXmlElementTag *root, decVector &vector );
};

// end of include only once
#endif
