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

#ifndef _DELSHAREDCONFIGXML_H_
#define _DELSHAREDCONFIGXML_H_

#include "delBaseXML.h"

class delGameProfile;
class delGPModule;


/**
 * \brief Shared Load/Save Config XML.
 */
class DE_DLL_EXPORT delSharedConfigXML : public delBaseXML{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create shared config xml read/save. */
	delSharedConfigXML(deLogger *logger, const char *loggingSource);
	
	/** \brief Clean up shared config xml read/save. */
	virtual ~delSharedConfigXML();
	/*@}*/
	
	
	
protected:
	void WriteProfile(decXmlWriter &writer, const delGameProfile &profile, const char *tagName = "profile");
	void WriteProfileSystems(decXmlWriter &writer, const delGameProfile &profile);
	void WriteProfileDisableModuleVersions(decXmlWriter &writer, const delGameProfile &profile);
	void WriteProfileModules(decXmlWriter &writer, const delGameProfile &profile);
	void WriteProfileModule(decXmlWriter &writer, const delGPModule &module);
	void WriteProfileModuleParameters(decXmlWriter &writer, const delGPModule &module);
	void WriteProfileWindow(decXmlWriter &writer, const delGameProfile &profile);
	
	void ReadProfile(const decXmlElementTag &root, delGameProfile &profile);
	void ReadProfileSystems(const decXmlElementTag &root, delGameProfile &profile);
	void ReadProfileDisableModuleVersions(const decXmlElementTag &root, delGameProfile &profile);
	void ReadProfileModules(const decXmlElementTag &root, delGameProfile &profile);
	void ReadProfileModule(const decXmlElementTag &root, delGameProfile &profile);
	void ReadProfileModuleParameters(const decXmlElementTag &root, delGPModule &module);
	void ReadProfileWindow(const decXmlElementTag &root, delGameProfile &profile);
};

#endif
