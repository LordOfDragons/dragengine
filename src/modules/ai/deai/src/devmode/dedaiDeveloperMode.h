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
	dedaiDeveloperMode(deDEAIModule &dedai);
	
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
	bool ExecuteCommand(const decUnicodeArgumentList &command, decUnicodeString &answer);
	
	
	
	/**
	 * \brief Touch update tracker increasing the update tracker number.
	 * \details Call after a state changed that potentially requires updates somewhere.
	 */
	void TouchUpdateTracker();
	
	/** \brief Update tracker number. */
	inline unsigned int GetUpdateTracker() const{ return pUpdateTracker; }
	/*@}*/
	
	
	
private:
	void pCmdHelp(const decUnicodeArgumentList &command, decUnicodeString &answer);
	void pCmdEnable(const decUnicodeArgumentList &command, decUnicodeString &answer);
	
	void pCmdShowSpaces(const decUnicodeArgumentList &command, decUnicodeString &answer);
	void pCmdShowSpaceLinks(const decUnicodeArgumentList &command, decUnicodeString &answer);
	void pCmdSpaceHighlightCostType(const decUnicodeArgumentList &command, decUnicodeString &answer);
	void pCmdShowBlockers(const decUnicodeArgumentList &command, decUnicodeString &answer);
	void pCmdShowPath(const decUnicodeArgumentList &command, decUnicodeString &answer);
	void pCmdShowPathFaces(const decUnicodeArgumentList &command, decUnicodeString &answer);
	
	void pCmdQuickDebug(const decUnicodeArgumentList &command, decUnicodeString &answer);
};

#endif
