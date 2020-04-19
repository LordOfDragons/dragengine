/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MELOADXMLWORLDTASK_H_
#define _MELOADXMLWORLDTASK_H_

#include <deigde/gui/igdeStepableTask.h>
#include <deigde/utils/igdeBaseXML.h>

#include <dragengine/common/xmlparser/decXmlDocumentReference.h>

class meDecal;
class meLoadSaveSystem;
class meNavigationSpace;
class meObject;
class meObjectTexture;
class meWorld;

class decBaseFileReader;
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
	decBaseFileReader *pReader;
	decXmlDocumentReference pXMLDocument;
	decXmlElementTag *pXMLRoot;
	int pNextTag;
	bool pProcessPFCache;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create task. */
	meLoadXMLWorldTask( meLoadSaveSystem *lssys, meWorld *world, decBaseFileReader *file );
	
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
	
	void pLoadObject( const decXmlElementTag &root, meObject &object );
	void pLoadObjectTexture( const decXmlElementTag &root, meObjectTexture &texture );
	void pLoadObjectTextureTransform( const decXmlElementTag &root, meObjectTexture &texture );
	void pLoadDecal( const decXmlElementTag &root, meDecal &decal );
	void pLoadDecalTransform( const decXmlElementTag &root, meDecal &decal );
	void pLoadNavigationSpace( const decXmlElementTag &root, meNavigationSpace &navspace );
};

#endif
