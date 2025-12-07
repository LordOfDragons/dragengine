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
class DE_DLL_EXPORT igdeLoadAnimator : public igdeBaseXML{
private:
	igdeEnvironment &pEnvironment;
	
	decString pName;
	decString pPattern;
	decString pDefaultExtension;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create load object. */
	igdeLoadAnimator(igdeEnvironment &environment, deLogger *logger, const char *loggerSource);
	
	/** \brief Clean up load object. */
	~igdeLoadAnimator();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name to display in a file dialog. */
	inline const decString &GetName() const{return pName;}
	
	/** \brief Set name to display in a file dialog. */
	void SetName(const char *name);
	
	/** \brief File pattern to display in a file dialog. */
	inline const decString &GetPattern() const{return pPattern;}
	
	/** \brief Set file pattern to display in a file dialog. */
	void SetPattern(const char *pattern);
	
	/** \brief Default file extension. */
	inline const decString &GetDefaultExtension() const{return pDefaultExtension;}
	
	/** \brief Set default file extension. */
	void SetDefaultExtension(const char *extension);
	
	
	
	/** \brief Load animator using a file reader. */
	void Load(const decString &pathAnimator, deAnimator &animator, decBaseFileReader &reader);
	/*@}*/
	
	
	
private:
	void pReadAnimator(const decXmlElementTag &root, const char *basePath, deAnimator &animator);
	void pReadController(const decXmlElementTag &root, deAnimator &animator);
	void pReadLink(const decXmlElementTag &root, deAnimator &animator);
	deAnimatorRule *pReadRule(const decXmlElementTag &root, const char *basePath, deAnimator &animator);
	deAnimatorRule *pReadRuleAnimation(const decXmlElementTag &root, deAnimator &animator);
	deAnimatorRule *pReadRuleAnimationDifference(const decXmlElementTag &root, deAnimator &animator);
	deAnimatorRule *pReadRuleAnimationSelect(const decXmlElementTag &root, deAnimator &animator);
	deAnimatorRule *pReadRuleBoneTransformator(const decXmlElementTag &root, deAnimator &animator);
	deAnimatorRule *pReadRuleStateManipulator(const decXmlElementTag &root, deAnimator &animator);
	deAnimatorRule *pReadRuleStateSnapshot(const decXmlElementTag &root, deAnimator &animator);
	deAnimatorRule *pReadRuleInverseKinematic(const decXmlElementTag &root, deAnimator &animator);
	deAnimatorRule *pReadRuleForeignState(const decXmlElementTag &root, deAnimator &animator);
	deAnimatorRule *pReadRuleGroup(const decXmlElementTag &root, const char *basePath, deAnimator &animator);
	deAnimatorRule *pReadRuleSubAnimator(const decXmlElementTag &root, const char *basePath, deAnimator &animator);
	deAnimatorRule *pReadRuleLimit(const decXmlElementTag &root, deAnimator &animator);
	deAnimatorRule *pReadRuleTrackTo(const decXmlElementTag &root, deAnimator &animator);
	deAnimatorRule *pReadRuleMirror(const decXmlElementTag &root, deAnimator &animator);
	bool pReadRuleCommon(const decXmlElementTag &root, deAnimator &animator, deAnimatorRule &rule);
	void pReadControllerTarget(const decXmlElementTag &root, deAnimator &animator, deAnimatorControllerTarget &target);
};

#endif
