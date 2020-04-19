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

#ifndef _CELOADSAVESYSTEM_H_
#define _CELOADSAVESYSTEM_H_

#include <deigde/gui/filedialog/igdeFilePatternList.h>

#include "ceLoadSaveConversation.h"

class ceConversation;
class ceConversationActor;
class ceLoadSaveConversation;
class ceLoadSaveCTA;
class ceLoadSaveCTGS;
class ceLoadSaveCTS;
class ceWindowMain;
class igdeGameDefinition;



/**
 * \brief Load/Save System.
 */
class ceLoadSaveSystem{
private:
	ceWindowMain &pWindowMain;
	
	ceLoadSaveConversation *pLSConversation;
	igdeFilePatternList pFPConversation;
	
	ceLoadSaveCTS *pLSCTS;
	igdeFilePatternList pFPCTS;
	
	ceLoadSaveCTA *pLSCTA;
	igdeFilePatternList pFPCTA;
	
	ceLoadSaveCTGS *pLSCTGS;
	igdeFilePatternList pFPCTGS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create load/save system. */
	ceLoadSaveSystem( ceWindowMain &windowMain );
	
	/** \brief Clean up load/save system. */
	~ceLoadSaveSystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Load save conversation. */
	inline ceLoadSaveConversation *GetLSConversation(){ return pLSConversation; }
	
	/** \brief Load conversation from file. */
	ceConversation *LoadConversation( const char *filename );
	
	/** \brief Save conversation to file. */
	void SaveConversation( ceConversation *conversation, const char *filename );
	
	/** \brief File pattern list. */
	inline const igdeFilePatternList *GetConversationFilePatterns() const{ return &pFPConversation; }
	
	
	
	/** \brief Load save conversation test setup. */
	inline ceLoadSaveCTS *GetLSCTS(){ return pLSCTS; }
	
	/** \brief Load conversation test setup from file. */
	void LoadCTS( const char *filename, ceConversation &conversation );
	
	/** \brief Save conversation test setup to file. */
	void SaveCTS( const char *filename, ceConversation &conversation );
	
	/** \brief Conversation test setup file pattern list. */
	inline const igdeFilePatternList *GetCTSFilePatterns() const{ return &pFPCTS; }
	
	
	
	/** \brief Load save conversation test actor. */
	inline ceLoadSaveCTA *GetLSCTA(){ return pLSCTA; }
	
	/** \brief Load conversation test actor from file. */
	void LoadCTA( const char *filename, ceConversationActor &actor );
	
	/** \brief Save conversation test actor to file. */
	void SaveCTA( const char *filename, ceConversationActor &actor );
	
	/** \brief Conversation test actor file pattern list. */
	inline const igdeFilePatternList *GetCTAFilePatterns() const{ return &pFPCTA; }
	
	
	
	/** \brief Load save conversation test game state. */
	inline ceLoadSaveCTGS *GetLSCTGS(){ return pLSCTGS; }
	
	/** \brief Load conversation test game state from file. */
	void LoadCTGS( const char *filename, ceConversation &conversation );
	
	/** \brief Save conversation test game state to file. */
	void SaveCTGS( const char *filename, const ceConversation &conversation );
	
	/** \brief Conversation test game state file pattern list. */
	inline const igdeFilePatternList *GetCTGSFilePatterns() const{ return &pFPCTGS; }
	/*@}*/
	
	
	
private:
	void pBuildFilePattern();
};

#endif
