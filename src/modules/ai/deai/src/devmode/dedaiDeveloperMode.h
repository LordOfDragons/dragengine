/* 
 * Drag[en]gine AI Module
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

#ifndef _DEDAIDEVELOPERMODE_H_
#define _DEDAIDEVELOPERMODE_H_

class deDEAIModule;
class decUnicodeArgumentList;
class decUnicodeString;



/**
 * \brief Gives access to the developer mode.
 */
class dedaiDeveloperMode{
private:
	deDEAIModule &pDEAI;
	bool pEnabled;
	
	bool pShowSpaces;
	bool pShowSpaceLinks;
	int pSpaceHighlightCostType;
	bool pShowBlockers;
	bool pShowPath;
	bool pShowPathFaces;
	
	int pQuickDebug;
	
	unsigned int pUpdateTracker;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create developer mode. */
	dedaiDeveloperMode( deDEAIModule &dedai );
	
	/** \brief Clean up developer mode. */
	~dedaiDeveloperMode();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deDEAIModule &GetModule() const{ return pDEAI; }
	
	/** \brief Developer mode is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	
	
	/** \brief Show navigation spaces. */
	inline bool GetShowSpaces() const{ return pShowSpaces; }
	
	/** \brief Show navigation space links. */
	inline bool GetShowSpaceLinks() const{ return pShowSpaceLinks; }
	
	/** \brief Set space type to highlight on navigation spaces or -1 to disable. */
	inline int GetSpaceHighlightCostType() const{ return pSpaceHighlightCostType; }
	
	/** \brief Show navigation blockers. */
	inline bool GetShowBlockers() const{ return pShowBlockers; }
	
	/** \brief Show navigator path. */
	inline bool GetShowPath() const{ return pShowPath; }
	
	/** \brief Show navigator path faces. */
	inline bool GetShowPathFaces() const{ return pShowPathFaces; }
	
	
	
	/** \brief Quick debug. */
	inline int GetQuickDebug() const{ return pQuickDebug; }
	
	
	
	/**
	 * \brief Execute command.
	 * \returns \em true if command is recognized.
	 */
	bool ExecuteCommand( const decUnicodeArgumentList &command, decUnicodeString &answer );
	
	
	
	/**
	 * \brief Touch update tracker increasing the update tracker number.
	 * \details Call after a state changed that potentially requires updates somewhere.
	 */
	void TouchUpdateTracker();
	
	/** \brief Update tracker number. */
	inline unsigned int GetUpdateTracker() const{ return pUpdateTracker; }
	/*@}*/
	
	
	
private:
	void pCmdHelp( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdEnable( const decUnicodeArgumentList &command, decUnicodeString &answer );
	
	void pCmdShowSpaces( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdShowSpaceLinks( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdSpaceHighlightCostType( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdShowBlockers( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdShowPath( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdShowPathFaces( const decUnicodeArgumentList &command, decUnicodeString &answer );
	
	void pCmdQuickDebug( const decUnicodeArgumentList &command, decUnicodeString &answer );
};

#endif
