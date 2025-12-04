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

#ifndef _PEEWPTYPE_H_
#define _PEEWPTYPE_H_

#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/igdeSpinTextFieldReference.h>
#include <deigde/gui/composed/igdeEditPathReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>
#include <deigde/gui/event/igdeAction::Ref.h>
#include <deigde/gui/resources/igdeIcon::Ref.h>

class peeEmitter;
class peeType;
class peeParameter;
class peeWindowProperties;
class peeWPTypeListener;



/**
 * \brief Type Panel.
 */
class peeWPType : public igdeContainerScroll{
private:
	peeWindowProperties &pWindowProperties;
	peeWPTypeListener *pListener;
	
	peeEmitter *pEmitter;
	
	igdeIcon::Ref pIconUnused;
	igdeIcon::Ref pIconUsed;
	
	igdeAction::Ref pActionType;
	igdeAction::Ref pActionTypeAdd;
	igdeAction::Ref pActionTypeRemove;
	igdeAction::Ref pActionTypeRename;
	
	igdeComboBoxReference pCBType;
	igdeButtonReference pBtnType;
	
	igdeTextFieldReference pEditBurstLifetime;
	igdeCheckBoxReference pChkEmitBurst;
	
	igdeEditPathReference pEditSkin;
	igdeEditPathReference pEditModel;
	igdeEditPathReference pEditModelSkin;
	igdeComboBoxReference pCBCastFrom;
	igdeComboBoxReference pCBSimType;
	igdeCheckBoxReference pChkIntervalAsDistance;
	igdeEditPathReference pEditPathTrailEmitter;
	igdeComboBoxReference pCBTrailController;
	igdeTextFieldReference pEditTrailTargetController;
	
	igdeTextFieldReference pEditPhysicsSize;
	igdeComboBoxReference pCBCollisionResponse;
	igdeEditPathReference pEditPathCollisionEmitter;
	igdeTextFieldReference pEditEmitMinImpulse;
	igdeComboBoxReference pCBEmitController;
	igdeTextFieldReference pEditEmitTargetController;
	
	igdeListBoxReference pListParams;
	
	igdeTextFieldReference pEditParamValue;
	igdeTextFieldReference pEditParamSpread;
	igdeComboBoxReference pCBParamCtrlValue;
	igdeComboBoxReference pCBParamCtrlSpread;
	
	bool pPreventUpdate;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	peeWPType( peeWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up panel. */
	virtual ~peeWPType();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Window properties. */
	inline peeWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** \brief Emitter. */
	inline peeEmitter *GetEmitter() const{ return pEmitter; }
	
	/** \brief Set emitter. */
	void SetEmitter( peeEmitter *emitter );
	
	/** \brief Emitter path changed. */
	void OnEmitterPathChanged();
	
	/** \brief Active type. */
	peeType *GetType() const;
	
	/** \brief Active parameter. */
	peeParameter *GetParameter() const;
	
	/** \brief Update type list. */
	void UpdateTypeList();
	
	/** \brief Change selection to active type. */
	void SelectActiveType();
	
	/** \brief Update emitter. */
	void UpdateEmitter();
	
	/** \brief Update type. */
	void UpdateType();
	
	/** \brief Update controller list. */
	void UpdateControllerList();
	
	/** \brief Change selection to active trail type. */
	void SelectActiveTrailController();
	
	/** \brief Change selection to active emit type. */
	void SelectActiveEmitController();
	
	/** \brief Change selection to active parameter. */
	void SelectActiveParameter();
	
	/** \brief Update active parameter. */
	void UpdateParameter();
	
	/** \brief Update parameter usage icon. */
	void UpdateParameterUsage( const peeParameter &parameter );
	
	
	
	/** \brief Actions. */
	inline igdeAction* GetActionTypeAdd() const{ return pActionTypeAdd; }
	inline igdeAction* GetActionTypeRemove() const{ return pActionTypeRemove; }
	inline igdeAction* GetActionTypeRename() const{ return pActionTypeRename; }
	/*@}*/
};

#endif
