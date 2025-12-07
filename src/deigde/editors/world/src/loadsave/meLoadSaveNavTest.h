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

#ifndef _MELOADSAVENAVTEST_H_
#define _MELOADSAVENAVTEST_H_

#include <deigde/utils/igdeBaseXML.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class mePathFindTestType;
class meWorld;
class meLoadSaveSystem;

class decBaseFileReader;
class decBaseFileWriter;
class decXmlWriter;
class decXmlElementTag;



/**
 * \brief Load/Save Navigation Test.
 */
class meLoadSaveNavTest : public igdeBaseXML{
private:
	meLoadSaveSystem *pLSSys;
	decString pName;
	decString pPattern;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new loader. */
	meLoadSaveNavTest(meLoadSaveSystem *lssys, deLogger *logger, const char *loggerSource);
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	/** \brief Retrieves the pattern. */
	inline const decString &GetPattern() const{ return pPattern; }
	
	/** \brief Loads the navigation test from file. */
	void LoadNavTest(meWorld &world, decBaseFileReader &reader);
	/** \brief Saves the navigation test to file. */
	void SaveNavTest(const meWorld &world, decBaseFileWriter &writer);
	/*@}*/
	
private:
	void pWriteNavTest(decXmlWriter &writer, const meWorld &world);
	void pWriteNavTestType(decXmlWriter &writer, const mePathFindTestType &type);
	
	void pReadNavTest(const decXmlElementTag &root, meWorld &world);
	void pReadNavTestType(const decXmlElementTag &root, meWorld &world);
};

#endif
