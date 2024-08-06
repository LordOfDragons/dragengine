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

#ifndef _DEMSGDKGAMECONFIG_H_
#define _DEMSGDKGAMECONFIG_H_

#include <dragengine/common/string/decString.h>

class decXmlElementTag;
class decXmlAttValue;
class deMicrosoftGdk;


/**
 * Game configuration (MicrosoftGame.config).
 */
class deMsgdkGameConfig{
private:
	deMicrosoftGdk &pModule;


public:
	decString scid;
	bool advancedUserModel;
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** Load game config. */
	deMsgdkGameConfig(deMicrosoftGdk &module, const decString &config);
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	
	/*@}*/


private:
	decXmlElementTag *pGetTagAt(decXmlElementTag &tag, int index);
	const decXmlAttValue *pFindAttribute(decXmlElementTag &tag, const char *name);
	const char *pGetAttributeString(decXmlElementTag &tag, const char *name);
	int pGetAttributeInt(decXmlElementTag &tag, const char *name);
	float pGetAttributeFloat(decXmlElementTag &tag, const char *name);

	void pLoadFromXml(const decString &config);
	void pReadTagGame(decXmlElementTag &root);
	void pReadTagExtendedAttributeList(decXmlElementTag &root);
};

#endif
