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

#ifndef _IGDEEDITABLEENTITY_H_
#define _IGDEEDITABLEENTITY_H_

#include <dragengine/deObject.h>

#include <dragengine/common/string/decString.h>

class igdeGameDefinition;
class igdeEnvironment;
class igdeUndoSystem;

class deEngine;
class deLogger;



/**
 * \brief Editable Entity.
 * 
 * Base class for editable entities in editors. Usually there exists in each
 * editor one base class which stores the entity the editor is working on.
 * this class provides a bunch of typical parameters as well as listening
 * support and an undo system. Furthermore this class stores also a the game
 * engine, the game definition and the environment. Most editors require
 * access to these objects for proper operation so they are provided here out
 * of the box. For handling loading and saving the file path and the changed
 * and saved state is stored. The file path is stored as two string values.
 * The file path string stores the file path as selected by the user and is
 * an absolute path in the virtual file system of the IGDE. Besides this the
 * directory path is stored too representing the directory the file path is
 * located in. This is useful for editable entities which require to load
 * dependency files relative to the file location. This string value can be
 * used directly for engine resource managers as the base path. The changed
 * and saved states keep track if the file has been already saved and if the
 * entity has changed since the last time it has been saved. The SetChanged
 * and SetSaved methods send notifications if the respective state changed.
 */
class DE_DLL_EXPORT igdeEditableEntity : public deObject{
private:
	igdeEnvironment *pEnvironment;
	
	decString pPathDirectory;
	decString pPathFile;
	bool pChanged;
	bool pSaved;
	
	igdeUndoSystem *pUndoSystem;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeEditableEntity> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \bried Creates a new editable entity.
	 * 
	 * The path are both set to the empty string so set a reasonable default
	 * file name in the constructor of the subclass. The saved and changed
	 * state are both set to false.
	 */
	igdeEditableEntity(igdeEnvironment *environment);
	
	/** \brief Clean up editable entity. */
	virtual ~igdeEditableEntity();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the environment. */
	inline igdeEnvironment *GetEnvironment() const{ return pEnvironment; }
	/** \brief Retrieves the engine object. */
	deEngine *GetEngine() const;
	
	/** \brief Retrieves the game definition. */
	igdeGameDefinition *GetGameDefinition() const;
	
	/** \brief Retrieves the logger. */
	deLogger *GetLogger() const;
	
	/** \brief Retrieves the directory path. */
	inline const decString &GetDirectoryPath() const{ return pPathDirectory; }
	/** \brief Retrieves the file path. */
	inline const decString &GetFilePath() const{ return pPathFile; }
	/** \brief Sets the file path. */
	void SetFilePath(const char *path);
	
	/** \brief Determines if the entity has changed. */
	inline bool GetChanged() const{ return pChanged; }
	/**
	 * \brief Sets if the entity has changed.
	 * 
	 * If this is different than the current changed state a notification is send to all listeners.
	 */
	void SetChanged(bool changed);
	
	/** \brief Determines if the entity has been saved. */
	inline bool GetSaved() const{ return pSaved; }
	/**
	 * \brief Sets if the entity has been saved.
	 * 
	 * If this is different than the current saved state a notification is send to all listeners.
	 */
	void SetSaved(bool saved);
	
	/** \brief Retrieves the undo system. */
	inline igdeUndoSystem *GetUndoSystem() const{ return pUndoSystem; }
	
	/** \brief Notifies all listeners that the changed or saved state changed. */
	virtual void NotifyStateChanged();
	
	/** \brief Notifies all listeners that the undo system changed. */
	virtual void NotifyUndoChanged();
	/*@}*/
};

#endif
