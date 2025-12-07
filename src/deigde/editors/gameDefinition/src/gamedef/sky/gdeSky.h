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

#ifndef _GDESKY_H_
#define _GDESKY_H_

#include "gdeSkyControllerList.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decStringSet.h>

class gdeGameDefinition;
class gdeSkyController;


/**
 * \brief Sky.
 */
class gdeSky : public deObject{
private:
	gdeGameDefinition *pGameDefinition;
	
	decString pPath;
	decString pName;
	decString pDescription;
	decString pCategory;
	decStringSet pTags;
	
	gdeSkyControllerList pControllers;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<gdeSky> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sky. */
	gdeSky();
	
	/** \brief Create sky. */
	gdeSky(const char *path, const char *name);
	
	/** \brief Create copy of sky. */
	gdeSky(const gdeSky &particleEmitter);
	
	/** \brief Clean up sky. */
	virtual ~gdeSky();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent game definition. */
	inline gdeGameDefinition *GetGameDefinition() const{return pGameDefinition;}
	
	/** \brief Set parent game definition. */
	void SetGameDefinition(gdeGameDefinition *gamedef);
	
	
	
	/** \brief Path. */
	inline const decString &GetPath() const{return pPath;}
	
	/** \brief Set path. */
	void SetPath(const char *path);
	
	/** \brief Name. */
	inline const decString &GetName() const{return pName;}
	
	/** \brief Set name. */
	void SetName(const char *name);
	
	/** \brief Description. */
	inline const decString &GetDescription() const{return pDescription;}
	
	/** \brief Set description. */
	void SetDescription(const char *description);
	
	/** \brief Cathegory. */
	inline const decString &GetCategory() const{return pCategory;}
	
	/** \brief Set cathegory. */
	void SetCategory(const char *category);
	
	/** \brief Tags. */
	inline const decStringSet &GetTags() const{return pTags;}
	
	/** \brief Set tags. */
	void SetTags(const decStringSet &tags);
	/*@}*/
	
	
	
	/** \name Controllers */
	/*@{*/
	/** \brief Controllers. */
	const gdeSkyControllerList &GetControllers() const{return pControllers;}
	
	/** \brief Add controller. */
	void AddController(gdeSkyController *controller);
	
	/** \brief Remove controller. */
	void RemoveController(gdeSkyController *controller);
	
	/** \brief Remove controllers. */
	void RemoveAllControllers();
	
	/** \brief Notify controller changed. */
	void NotifyControllerChanged(gdeSkyController *controller);
	/*@}*/
};

#endif
