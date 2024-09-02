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

#ifndef _MEASUBCLASSASECLASS_H_
#define _MEASUBCLASSASECLASS_H_

#include <deigde/codec/igdeCodecPropertyString.h>
#include <deigde/gamedefinition/property/igdeGDProperty.h>
#include <deigde/gui/event/igdeAction.h>

class meWindowMain;
class meObject;
class meObjectTexture;

class igdeGDClass;

class decXmlWriter;


/**
 * \brief Action subclassing active object as EClass XML.
 */
class meASubclassAsEClass : public igdeAction{
private:
	meWindowMain &pWindow;
	igdeCodecPropertyString pCodec;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create action. */
	meASubclassAsEClass( meWindowMain &window );
	/*@}*/
	
	
	
	/** \name Run action. */
	/*@{*/
	/** \brief Run action. */
	virtual void OnAction();
	
	/** \brief Update action parameters. */
	virtual void Update();
	/*@}*/
	
	
	
protected:
	void WriteEClass( const meObject &object, const igdeGDClass &gdclass,
		const decString &classname, decXmlWriter &writer, const decString &basePath );
	
	void WriteEClassProperties( const meObject &object, const igdeGDClass &gdclass,
		decXmlWriter &writer, const decString &basePath );
	
	void WriteEClassProperty( const decString &property, const decString &value,
		const igdeGDProperty *gdProperty, decXmlWriter &writer, const decString &basePath );
	
	void WriteEClassTextureReplacements( const meObject &object,
		decXmlWriter &writer, const decString &basePath );
	
	void WriteEClassTextureReplacements( const meObjectTexture &texture,
		decXmlWriter &writer, const decString &basePath );
};

#endif
