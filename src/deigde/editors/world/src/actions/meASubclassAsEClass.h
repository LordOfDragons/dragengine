/* 
 * Drag[en]gine IGDE World Editor
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
