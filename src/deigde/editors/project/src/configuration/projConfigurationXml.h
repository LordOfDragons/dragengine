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

#ifndef _PROJCONFIGURATIONXML_H_
#define _PROJCONFIGURATIONXML_H_

#include <deigde/utils/igdeBaseXML.h>

#include <dragengine/common/string/decString.h>

class decBaseFileReader;
class decBaseFileWriter;
class projConfiguration;
class decXmlWriter;
class decXmlElementTag;


/**
 * \brief Load/Save Configuration XML.
 */
class projConfigurationXml : public igdeBaseXML{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create configuration xml read/save. */
	projConfigurationXml(deLogger *logger, const char *loggerSource);
	
	/** Clean up configuration xml read/save. */
	~projConfigurationXml() override;
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** Read from XML file. */
	void ReadFromFile(decBaseFileReader &reader, projConfiguration &config);
	
	/** Write to XML file. */
	void WriteToFile(decBaseFileWriter &writer, const projConfiguration &config);
	/*@}*/
	
	
private:
	void pWriteConfig(decXmlWriter &writer, const projConfiguration &config);
	void pWriteRemoteLauncher(decXmlWriter &writer, const projConfiguration &config);
	
	void pReadConfig(const decXmlElementTag &root, projConfiguration &config);
	void pReadRemoteLauncher(const decXmlElementTag &root, projConfiguration &config);
};

#endif
