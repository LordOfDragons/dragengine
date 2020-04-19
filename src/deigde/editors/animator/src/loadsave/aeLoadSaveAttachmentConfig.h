/* 
 * Drag[en]gine IGDE Animator Editor
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

#ifndef _AELOADSAVEATTACHMENTCONFIG_H_
#define _AELOADSAVEATTACHMENTCONFIG_H_

#include <deigde/utils/igdeBaseXML.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class aeAttachment;
class aeAnimator;

class decBaseFileReader;
class decBaseFileWriter;
class decXmlWriter;
class decXmlElementTag;



/**
 * \brief Load/Save Attachment Configuration.
 * 
 */
class aeLoadSaveAttachmentConfig : public igdeBaseXML{
private:
	decString pName;
	decString pPattern;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create load/save. */
	aeLoadSaveAttachmentConfig( deLogger *logger, const char *loggerSource );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Pattern. */
	inline const decString &GetPattern() const{ return pPattern; }
	
	/** \brief Load attachment configuration from file. */
	void LoadAttachmentConfig( aeAnimator &animator, decBaseFileReader &reader );
	
	/** \brief Save attachment configuration to file. */
	void SaveAttachmentConfig( const aeAnimator &animator, decBaseFileWriter &writer );
	/*@}*/
	
private:
	void pWriteConfiguration( decXmlWriter &writer, const aeAnimator &animator );
	void pWriteAttachment( decXmlWriter &writer, const aeAttachment &attachment );
	
	void pReadConfiguration( const decXmlElementTag &root, aeAnimator &animator );
	void pReadAttachment( const decXmlElementTag &root, aeAnimator &animator );
};

#endif
