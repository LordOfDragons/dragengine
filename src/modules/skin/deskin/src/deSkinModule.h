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

#ifndef _DESKINMODULE_H_
#define _DESKINMODULE_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/skin/deSkinMapped.h>
#include <dragengine/systems/modules/skin/deBaseSkinModule.h>

class decXmlElementTag;
class decXmlAttValue;
class decXmlWriter;
class deSkinTexture;
class deSkinProperty;
class deSkinPropertyConstructed;
class decPath;
class decCurveBezier;
class decCurveBezierPoint;
class deSkinPropertyMapped;
class deSkinPropertyNode;
class deSkinPropertyNodeGroup;
class deSkinPropertyNodeImage;
class deSkinPropertyNodeShape;
class deSkinPropertyNodeText;


// dragengine skin module
class deSkinModule : public deBaseSkinModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deSkinModule(deLoadableModule &loadableModule);
	
	/** \brief Clean up module. */
	virtual ~deSkinModule();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Load skin. */
	virtual void LoadSkin(decBaseFileReader &reader, deSkin &skin);
	
	/** \brief Save skin. */
	virtual void SaveSkin(decBaseFileWriter &writer, const deSkin &skin);
	/*@}*/
	
private:
	decXmlElementTag *pGetTagAt(const decXmlElementTag &tag, int index);
	decXmlAttValue *pFindAttribute(const decXmlElementTag &tag, const char *name);
	const char *pGetAttributeString(const decXmlElementTag &tag, const char *name);
	int pGetAttributeInt(const decXmlElementTag &tag, const char *name);
	float pGetAttributeFloat(const decXmlElementTag &tag, const char *name);
	bool pGetAttributeBool(const decXmlElementTag &tag, const char *name);
	
	void pParseSkin(const decXmlElementTag &root, deSkin &skin);
	
	deSkinMapped::Ref pParseMapped(const decXmlElementTag &root, const char *forceName = nullptr);
	void pParseMappedCurve(const decXmlElementTag &root, decCurveBezier &curve);
	void pParseMappedCurvePoint(const decXmlElementTag &root, decCurveBezier &curve);
	
	deSkinTexture *pParseTexture(const decXmlElementTag &root, decPath &basePath, deSkin &skin);
	void pParsePropertyMapped(const decXmlElementTag &root, deSkin &skin, deSkinPropertyMapped &property);
	void pParsePropertyConstructed(const decXmlElementTag &root, const deSkin &skin, deSkinPropertyConstructed &property);
	
	deSkinPropertyNode *pParsePropertyNode(const decXmlElementTag &tag, const deSkin &skin);
	bool pParsePropertyNodeCommon(const decXmlElementTag &tag, const deSkin &skin, deSkinPropertyNode &node);
	void pParsePropertyNodeGroup(const decXmlElementTag &root, const deSkin &skin, deSkinPropertyNodeGroup &group);
	void pParsePropertyNodeImage(const decXmlElementTag &root, const deSkin &skin, deSkinPropertyNodeImage &group);
	void pParsePropertyNodeShape(const decXmlElementTag &root, const deSkin &skin, deSkinPropertyNodeShape &group);
	void pParsePropertyNodeText(const decXmlElementTag &root, const deSkin &skin, deSkinPropertyNodeText &group);
	
	decColor pParseColor(const decXmlElementTag &root);
	void pReadVector2(const decXmlElementTag &tag, decVector2 &vector);
	
	void pWriteSkin(decXmlWriter &writer, const deSkin &skin);
	void pWriteMapped(decXmlWriter &writer, const deSkinMapped &mapped);
	void pWriteTexture(decXmlWriter &writer, const deSkin &skin, const deSkinTexture &texture);
	void pWriteProperty(decXmlWriter& writer, const deSkin &skin, deSkinProperty &property);
};

#endif
