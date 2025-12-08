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

#ifndef _GDEWPSOCENVMAPPROBE_H_
#define _GDEWPSOCENVMAPPROBE_H_

#include "../../../gamedef/objectClass/envmapprobe/gdeOCEnvMapProbe.h"

#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class gdeGameDefinition;
class gdeObjectClass;
class gdeOCEnvMapProbe;
class gdeWindowProperties;
class gdeWPSOCEnvMapProbeListener;



/**
 * \brief Object class environment map probe property panel.
 */
class gdeWPSOCEnvMapProbe : public igdeContainerScroll{
private:
	gdeWindowProperties &pWindowProperties;
	gdeWPSOCEnvMapProbeListener::Ref pListener;
	
	gdeGameDefinition::Ref pGameDefinition;
	
	igdeEditVector::Ref pEditPosition;
	igdeEditVector::Ref pEditRotation;
	igdeEditVector::Ref pEditScaling;
	
	igdeTextField::Ref pEditShapeInfluence;
	igdeTextField::Ref pEditShapeReflection;
	igdeTextField::Ref pEditShapeReflectionMask;
	igdeTextField::Ref pEditInfluenceBorderSize;
	igdeTextField::Ref pEditInfluencePriority;
	
	igdeComboBox::Ref pCBPropertyNames;
	igdeComboBoxFilter::Ref pCBPropertyNameTarget;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	gdeWPSOCEnvMapProbe(gdeWindowProperties &windowMain);
	
protected:
	/** \brief Clean up panel. */
	virtual ~gdeWPSOCEnvMapProbe();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Game definition or \em NULL if not set. */
	inline const gdeGameDefinition::Ref &GetGameDefinition() const{ return pGameDefinition; }
	
	/** \brief Set game definition or \em NULL if not set. */
	void SetGameDefinition(gdeGameDefinition *gameDefinition);
	
	
	
	/** \brief Active object class or \em NULL if absent. */
	gdeObjectClass *GetObjectClass() const;
	
	/** \brief Active object class environment map probe or \em NULL if not set. */
	gdeOCEnvMapProbe *GetEnvMapProbe() const;
	
	/** \brief Selected property name. */
	const gdeOCEnvMapProbe::eProperties GetPropertyName() const;
	
	
	
	/** \brief Update object class property list. */
	void UpdatePropertyList();
	
	
	
	/** \brief Update environment map probe. */
	void UpdateEnvMapProbe();
	
	/** \brief Update environment map probe property name. */
	void UpdatePropertyName();
	/*@}*/
};

#endif
