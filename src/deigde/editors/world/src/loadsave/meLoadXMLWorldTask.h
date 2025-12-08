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

#ifndef _MELOADXMLWORLDTASK_H_
#define _MELOADXMLWORLDTASK_H_

#include <deigde/gui/igdeStepableTask.h>
#include <deigde/utils/igdeBaseXML.h>

#include <dragengine/common/xmlparser/decXmlDocument.h>
#include "decBaseFileReader.h"

class meDecal;
class meLoadSaveSystem;
class meNavigationSpace;
class meObject;
class meObjectTexture;
class meWorld;

class decXmlElementTag;



/**
 * \brief Load XML World Task.
 * 
 * Load world from file in the Drag[en]gine World World XML format using stepable task.
 */
class meLoadXMLWorldTask : public igdeStepableTask, igdeBaseXML{
private:
	meLoadSaveSystem *pLSSys;
	meWorld *pWorld;
	decBaseFileReader::Ref pReader;
	decXmlDocument::Ref pXMLDocument;
	decXmlElementTag *pXMLRoot;
	int pNextTag;
	bool pProcessPFCache;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create task. */
	meLoadXMLWorldTask(meLoadSaveSystem *lssys, meWorld *world, decBaseFileReader *file);
	
	/** \brief Clean up task. */
	virtual ~meLoadXMLWorldTask();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Run next step of task.
	 * \returns \em true if task requires more steps or \em fals if finished after this step.
	 */
	virtual bool Step();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pLoadWorldEditor(const decXmlElementTag &root);
	void pLoadWorldEditorBackgroundObject(const decXmlElementTag &root);
	void pLoadWorldEditorLimitBox(const decXmlElementTag &root);
	void pLoadObject(const decXmlElementTag &root, meObject &object);
	void pLoadObjectTexture(const decXmlElementTag &root, meObjectTexture &texture);
	void pLoadObjectTextureTransform(const decXmlElementTag &root, meObjectTexture &texture);
	void pLoadDecal(const decXmlElementTag &root, meDecal &decal);
	void pLoadDecalTransform(const decXmlElementTag &root, meDecal &decal);
	void pLoadNavigationSpace(const decXmlElementTag &root, meNavigationSpace &navspace);
};

#endif
