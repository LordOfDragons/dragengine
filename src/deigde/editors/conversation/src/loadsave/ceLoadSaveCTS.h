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

#ifndef _CELOADSAVECTS_H_
#define _CELOADSAVECTS_H_

#include <deigde/utils/igdeBaseXML.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class ceProp;
class ceCoordSystem;
class ceActorController;
class ceLoadSaveSystem;
class ceConversation;
class ceConversationActor;

class decBaseFileReader;
class decBaseFileWriter;
class decXmlWriter;
class decXmlElementTag;



/**
 * \brief Load/Save Conversation Test Setup.
 */
class ceLoadSaveCTS : public igdeBaseXML{
private:
	ceLoadSaveSystem *pLSSys;
	decString pName;
	decString pPattern;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new loader. */
	ceLoadSaveCTS(ceLoadSaveSystem *lssys, deLogger *logger, const char *loggerSource);
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the name. */
	inline const decString &GetName() const{return pName;}
	/** Retrieves the pattern. */
	inline const decString &GetPattern() const{return pPattern;}
	
	/** Loads the conversation test setup from file. */
	void LoadCTS(ceConversation &conversation, decBaseFileReader &reader);
	/** Saves the conversation test setup to file. */
	void SaveCTS(const ceConversation &conversation, decBaseFileWriter &writer);
	/*@}*/
	
private:
	void pWriteCTS(decXmlWriter &writer, const ceConversation &conversation);
	void pWriteCoordSystem(decXmlWriter &writer, const ceCoordSystem &coordSystem);
	void pWriteProp(decXmlWriter &writer, const ceProp &prop);
	
	void pReadCTS(const decXmlElementTag &root, ceConversation &conversation);
	void pReadCoordSystem(const decXmlElementTag &root, ceConversation &conversation);
	void pReadProp(const decXmlElementTag &root, ceConversation &conversation);
};

#endif
