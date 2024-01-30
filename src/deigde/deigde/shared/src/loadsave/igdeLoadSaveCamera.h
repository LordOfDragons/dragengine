/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2024, Roland Plüss (roland@rptd.ch)
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

#ifndef _IGDELOADSAVECAMERA_H_
#define _IGDELOADSAVECAMERA_H_

#include "../utils/igdeBaseXML.h"

#include <dragengine/common/string/decString.h>

class igdeEnvironment;
class igdeCamera;

class decBaseFileReader;
class decBaseFileWriter;
class decXmlWriter;


/**
 * \brief Load/save camera parameters.
 * \version 1.21
 */
class DE_DLL_EXPORT igdeLoadSaveCamera : public igdeBaseXML{
private:
	igdeEnvironment &pEnvironment;
	
	decString pName;
	decString pPattern;
	decString pDefaultExtension;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create load object. */
	igdeLoadSaveCamera( igdeEnvironment &environment, deLogger *logger, const char *loggerSource );
	
	/** \brief Clean up load object. */
	~igdeLoadSaveCamera() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name to display in a file dialog. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name to display in a file dialog. */
	void SetName( const char *name );
	
	/** \brief File pattern to display in a file dialog. */
	inline const decString &GetPattern() const{ return pPattern; }
	
	/** \brief Set file pattern to display in a file dialog. */
	void SetPattern( const char *pattern );
	
	/** \brief Default file extension. */
	inline const decString &GetDefaultExtension() const{ return pDefaultExtension; }
	
	/** \brief Set default file extension. */
	void SetDefaultExtension( const char *extension );
	
	
	
	/** \brief Load camera. */
	virtual void Load( const decString &path, igdeCamera &camera, decBaseFileReader &reader );
	
	/** \brief Save camera. */
	virtual void Save( const igdeCamera &camera, decBaseFileWriter &writer );
	/*@}*/
	
	
	
private:
	void pReadCamera( const decXmlElementTag &root, igdeCamera &camera );
	
	void pWriteCamera( decXmlWriter &writer, const igdeCamera &camera );
};

#endif
