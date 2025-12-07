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

#ifndef _CELOADSAVESYSTEM_H_
#define _CELOADSAVESYSTEM_H_

#include "ceLoadSaveConversation.h"
#include "../langpack/ceLangPack.h"

#include <deigde/gui/filedialog/igdeFilePatternList.h>

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceConversation;
class ceConversationActor;
class ceLoadSaveConversation;
class ceLoadSaveCTA;
class ceLoadSaveCTGS;
class ceLoadSaveCTS;
class ceWindowMain;
class igdeGameDefinition;
class ceLoadSaveLangPack;
class ceLangPack;


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
	
	decObjectOrderedSet pLSLangPacks;
	igdeFilePatternList pFPListLangPack;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create load/save system. */
	ceLoadSaveSystem(ceWindowMain &windowMain);
	
	/** \brief Clean up load/save system. */
	~ceLoadSaveSystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Load save conversation. */
	inline ceLoadSaveConversation *GetLSConversation(){ return pLSConversation; }
	
	/** \brief Load conversation from file. */
	ceConversation *LoadConversation(const char *filename);
	
	/** \brief Save conversation to file. */
	void SaveConversation(ceConversation *conversation, const char *filename);
	
	/** \brief File pattern list. */
	inline const igdeFilePatternList *GetConversationFilePatterns() const{ return &pFPConversation; }
	
	
	
	/** \brief Load save conversation test setup. */
	inline ceLoadSaveCTS *GetLSCTS(){ return pLSCTS; }
	
	/** \brief Load conversation test setup from file. */
	void LoadCTS(const char *filename, ceConversation &conversation);
	
	/** \brief Save conversation test setup to file. */
	void SaveCTS(const char *filename, ceConversation &conversation);
	
	/** \brief Conversation test setup file pattern list. */
	inline const igdeFilePatternList *GetCTSFilePatterns() const{ return &pFPCTS; }
	
	
	
	/** \brief Load save conversation test actor. */
	inline ceLoadSaveCTA *GetLSCTA(){ return pLSCTA; }
	
	/** \brief Load conversation test actor from file. */
	void LoadCTA(const char *filename, ceConversationActor &actor);
	
	/** \brief Save conversation test actor to file. */
	void SaveCTA(const char *filename, ceConversationActor &actor);
	
	/** \brief Conversation test actor file pattern list. */
	inline const igdeFilePatternList *GetCTAFilePatterns() const{ return &pFPCTA; }
	
	
	
	/** \brief Load save conversation test game state. */
	inline ceLoadSaveCTGS *GetLSCTGS(){ return pLSCTGS; }
	
	/** \brief Load conversation test game state from file. */
	void LoadCTGS(const char *filename, ceConversation &conversation);
	
	/** \brief Save conversation test game state to file. */
	void SaveCTGS(const char *filename, const ceConversation &conversation);
	
	/** \brief Conversation test game state file pattern list. */
	inline const igdeFilePatternList *GetCTGSFilePatterns() const{ return &pFPCTGS; }
	
	
	
	
	/** Count of load save langpacks. */
	int GetLSLangPackCount() const;
	
	/** Load save langpack at the given index. */
	ceLoadSaveLangPack *GetLSLangPackAt(int index) const;
	
	/** Index of the load save langpack. */
	int IndexOfLSLangPack(ceLoadSaveLangPack *lsLangPack) const;
	
	/** Save langpack exists. */
	bool HasLSLangPack(ceLoadSaveLangPack *lsLangPack) const;
	
	/** Index of the load save langpack matching the given filename. */
	int IndexOfLSLangPackMatching(const char *filename);
	
	/** Add load save langpack. */
	void AddLSLangPack(ceLoadSaveLangPack *lsLangPack);
	
	/** Remove load save langpack. */
	void RemoveLSLangPack(ceLoadSaveLangPack *lsLangPack);
	
	/** Remove all load save langpacks. */
	void RemoveAllLSLangPacks();
	
	/** Update load save langpack list from the engine. */
	void UpdateLSLangPacks();
	
	/** Load langpack from file if possible. */
	ceLangPack::Ref LoadLangPack(const char *filename);
	
	/** Save langpack to file if possible. */
	void SaveLangPack(ceLangPack &langpack);
	
	/** File pattern list. */
	inline const igdeFilePatternList &GetLangPackFPList() const{ return pFPListLangPack; }
	/*@}*/
	
	
	
private:
	void pBuildFilePattern();
};

#endif
