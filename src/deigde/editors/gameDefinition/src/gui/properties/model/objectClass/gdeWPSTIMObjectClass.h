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

#ifndef _GDEWPSTIMOBJECTCLASS_H_
#define _GDEWPSTIMOBJECTCLASS_H_

#include "../gdeWPSTreeItemModel.h"

class gdeObjectClass;
class gdeOCBillboard;
class gdeOCCamera;
class gdeOCComponent;
class gdeOCEnvMapProbe;
class gdeOCLight;
class gdeOCNavigationBlocker;
class gdeOCNavigationSpace;
class gdeOCParticleEmitter;
class gdeOCForceField;
class gdeOCSnapPoint;
class gdeOCSpeaker;
class gdeOCWorld;
class gdeWPSTIMOCBillboard;
class gdeWPSTIMOCCamera;
class gdeWPSTIMOCComponent;
class gdeWPSTIMOCEnvMapProbe;
class gdeWPSTIMOCLight;
class gdeWPSTIMOCNavBlocker;
class gdeWPSTIMOCNavSpace;
class gdeWPSTIMOCParticleEmitter;
class gdeWPSTIMOCForceField;
class gdeWPSTIMOCSnapPoint;
class gdeWPSTIMOCSpeaker;
class gdeWPSTIMOCWorld;


/**
 * \brief Object class.
 */
class gdeWPSTIMObjectClass : public gdeWPSTreeItemModel{
private:
	gdeObjectClass::Ref pObjectClass;
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	gdeWPSTIMObjectClass(gdeWPSTreeModel &tree, gdeObjectClass *objectClass);
	
protected:
	/** \brief Clean up tree item model. */
	~gdeWPSTIMObjectClass() override;
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Object class. */
	inline const gdeObjectClass::Ref &GetObjectClass() const{ return pObjectClass; }
	
	
	
	/** \brief Model with billboard or \em NULL. */
	gdeWPSTIMOCBillboard *GetChildWith(gdeOCBillboard *billboard) const;
	
	/** \brief Model with camera or \em NULL. */
	gdeWPSTIMOCCamera *GetChildWith(gdeOCCamera *camera) const;
	
	/** \brief Model with component or \em NULL. */
	gdeWPSTIMOCComponent *GetChildWith(gdeOCComponent *component) const;
	
	/** \brief Model with envmap probe or \em NULL. */
	gdeWPSTIMOCEnvMapProbe *GetChildWith(gdeOCEnvMapProbe *envMapProbe) const;
	
	/** \brief Model with light or \em NULL. */
	gdeWPSTIMOCLight *GetChildWith(gdeOCLight *light) const;
	
	/** \brief Model with navblocker or \em NULL. */
	gdeWPSTIMOCNavBlocker *GetChildWith(gdeOCNavigationBlocker *navblocker) const;
	
	/** \brief Model with navspace or \em NULL. */
	gdeWPSTIMOCNavSpace *GetChildWith(gdeOCNavigationSpace *navspace) const;
	
	/** \brief Model with particle emitter or \em NULL. */
	gdeWPSTIMOCParticleEmitter *GetChildWith(gdeOCParticleEmitter *particleEmitter) const;
	
	/** \brief Model with force field or \em NULL. */
	gdeWPSTIMOCForceField *GetChildWith(gdeOCForceField *forceField) const;
	
	/** \brief Model with snap point or \em NULL. */
	gdeWPSTIMOCSnapPoint *GetChildWith(gdeOCSnapPoint *snapPoint) const;
	
	/** \brief Model with speaker or \em NULL. */
	gdeWPSTIMOCSpeaker *GetChildWith(gdeOCSpeaker *speaker) const;
	
	/** \brief Model with world or nullptr. */
	gdeWPSTIMOCWorld *GetChildWith(gdeOCWorld *world) const;
	
	
	
	/** \brief Name changed. */
	void NameChanged();
	
	/** \brief Sub objects changed. */
	void SubObjectsChanged();
	
	/** \brief Verify if object class is valid. */
	bool IsValid() const;
	
	/** \brief Validate component and update state accordingly. */
	void Validate();
	
	/** \brief Validate due to change in object class names or existance. */
	void ValidateObjectClassName();
	
	/** \brief Validate due to change in category names or existance. */
	void ValidateCategoryName();
	
	
	
	/** \brief Added to tree. */
	void OnAddedToTree() override;
	
	/** \brief Compare this item with another for sorting. */
	int Compare(const gdeWPSTreeItemModel &item) const override;
	
	/** \brief User selected item. */
	void OnSelected() override;
	
	/** \brief User requests context menu for selected item. */
	void OnContextMenu(igdeMenuCascade &contextMenu) override;
	
	/** \brief Select object mest matching name. */
	void SelectBestMatching(const char *string) override;
	/*@}*/
	
	
	
private:
	void pAddSubObjects();
};

#endif

