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

#ifndef _IGDETEMPLATE_H_
#define _IGDETEMPLATE_H_

#include "igdeTemplateFileList.h"

#include <dragengine/deObject.h>
#include <dragengine/common/string/decStringList.h>



/**
 * \brief Project template.
 */
class igdeTemplate : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeTemplate> Ref;



private:
	decString pBasePath;
	decString pName;
	decString pDescription;
	decString pScriptModule;
	decStringList pBaseGameDefinitions;
	igdeTemplateFileList pFiles;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create template. */
	igdeTemplate();
	
	/** \brief Create template. */
	igdeTemplate(const igdeTemplate &atemplate);
	
protected:
	/** \brief Clean up template. */
	virtual ~igdeTemplate();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Base path to directory containing template. */
	inline const decString &GetBasePath() const{return pBasePath;}
	
	/** \brief Set base path to directory containing template. */
	void SetBasePath(const char *path);
	
	/** \brief Name. */
	inline const decString &GetName() const{return pName;}
	
	/** \brief Set name. */
	void SetName(const char *name);
	
	/** \brief Description. */
	inline const decString &GetDescription() const{return pDescription;}
	
	/** \brief Set description. */
	void SetDescription(const char *description);
	
	/** \brief Script module required for this template. */
	inline const decString &GetScriptModule() const{return pScriptModule;}
	
	/** \brief Set script module required for this template. */
	void SetScriptModule(const char *scriptModule);
	
	/** \brief Base game definition list. */
	inline decStringList &GetBaseGameDefinitions(){return pBaseGameDefinitions;}
	inline const decStringList &GetBaseGameDefinitions() const{return pBaseGameDefinitions;}
	
	/** \brief Files. */
	inline igdeTemplateFileList &GetFiles(){return pFiles;}
	inline const igdeTemplateFileList &GetFiles() const{return pFiles;}
	/*@}*/
};

#endif
