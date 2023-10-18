/* 
 * Drag[en]gine IGDE Conversation Editor
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

#ifndef _CELOADSAVECTC_H_
#define _CELOADSAVECTC_H_

#include <deigde/utils/igdeBaseXML.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class ceLoadSaveSystem;
class ceConversationActor;
class ceActorPose;
class ceActorController;

class decBaseFileReader;
class decBaseFileWriter;
class decXmlWriter;
class decXmlElementTag;



/**
 * Load/Save conversation test actor.
 */
class ceLoadSaveCTA : public igdeBaseXML{
private:
	ceLoadSaveSystem &pLoadSaveSystem;
	decString pName;
	decString pPattern;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create load/save conversation test actor. */
	ceLoadSaveCTA( ceLoadSaveSystem &loadSaveSystem, deLogger *logger, const char *loggerSource );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Pattern. */
	inline const decString &GetPattern() const{ return pPattern; }
	
	/** Load conversation test actor. */
	void LoadCTA( ceConversationActor &actor, decBaseFileReader &reader );
	
	/** Save conversation test actor. */
	void SaveCTA( const ceConversationActor &actor, decBaseFileWriter &writer );
	
	
	
	/** Write actor tag. */
	void WriteActor( decXmlWriter &writer, const ceConversationActor &actor, const char *tagName );
	
	/** Read actor tag. */
	void ReadActor( const decXmlElementTag &root, ceConversationActor &actor );
	/*@}*/
	
	
	
private:
	void pWritePose( decXmlWriter &writer, const ceActorPose &pose );
	void pWriteController( decXmlWriter &writer, const ceActorController &controller );
	
	void pReadPose( const decXmlElementTag &root, ceConversationActor &actor );
	void pReadController( const decXmlElementTag &root, ceActorController &controller );
};

#endif
