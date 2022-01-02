/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDELOADANIMATOR_H_
#define _IGDELOADANIMATOR_H_

#include "../utils/igdeBaseXML.h"

#include <dragengine/common/string/decString.h>

class deAnimatorControllerTarget;
class igdeEnvironment;
class igdeGameProject;

class deAnimator;
class deAnimatorRule;
class decBaseFileReader;


/**
 * \brief Load animator file in the Drag[en]gine Animator format.
 */
class igdeLoadAnimator : public igdeBaseXML{
private:
	igdeEnvironment &pEnvironment;
	
	decString pName;
	decString pPattern;
	decString pDefaultExtension;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create load object. */
	igdeLoadAnimator( igdeEnvironment &environment, deLogger *logger, const char *loggerSource );
	
	/** \brief Clean up load object. */
	~igdeLoadAnimator();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name to display in a file dialog. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name to display in a file dialog. */
	void SetName( const char *name );
	
	/** \brief File pattern to display in a file dialog. */
	inline const decString &GetPattern() const{ return pPattern; }
	
	/** \brief Set file pattern to display in a file dialog. */
	void SetPattern( const char *pattern );
	
	/** \brief Default file extension. */
	inline const decString &GetDefaultExtension() const{ return pDefaultExtension; }
	
	/** \brief Set default file extension. */
	void SetDefaultExtension( const char *extension );
	
	
	
	/** \brief Load animator using a file reader. */
	void Load( const decString &pathAnimator, deAnimator &animator, decBaseFileReader &reader );
	/*@}*/
	
	
	
private:
	void pReadAnimator( const decXmlElementTag &root, const char *basePath, deAnimator &animator );
	void pReadController( const decXmlElementTag &root, deAnimator &animator );
	void pReadLink( const decXmlElementTag &root, deAnimator &animator );
	deAnimatorRule *pReadRule( const decXmlElementTag &root, const char *basePath, deAnimator &animator );
	deAnimatorRule *pReadRuleAnimation( const decXmlElementTag &root, deAnimator &animator );
	deAnimatorRule *pReadRuleAnimationDifference( const decXmlElementTag &root, deAnimator &animator );
	deAnimatorRule *pReadRuleAnimationSelect( const decXmlElementTag &root, deAnimator &animator );
	deAnimatorRule *pReadRuleBoneTransformator( const decXmlElementTag &root, deAnimator &animator );
	deAnimatorRule *pReadRuleStateManipulator( const decXmlElementTag &root, deAnimator &animator );
	deAnimatorRule *pReadRuleStateSnapshot( const decXmlElementTag &root, deAnimator &animator );
	deAnimatorRule *pReadRuleInverseKinematic( const decXmlElementTag &root, deAnimator &animator );
	deAnimatorRule *pReadRuleForeignState( const decXmlElementTag &root, deAnimator &animator );
	deAnimatorRule *pReadRuleGroup( const decXmlElementTag &root, const char *basePath, deAnimator &animator );
	deAnimatorRule *pReadRuleSubAnimator( const decXmlElementTag &root, const char *basePath, deAnimator &animator );
	deAnimatorRule *pReadRuleLimit( const decXmlElementTag &root, deAnimator &animator );
	deAnimatorRule *pReadRuleTrackTo( const decXmlElementTag &root, deAnimator &animator );
	deAnimatorRule *pReadRuleMirror( const decXmlElementTag &root, deAnimator &animator );
	bool pReadRuleCommon( const decXmlElementTag &root, deAnimator &animator, deAnimatorRule &rule );
	void pReadControllerTarget( const decXmlElementTag &root, deAnimator &animator, deAnimatorControllerTarget &target );
};

#endif
