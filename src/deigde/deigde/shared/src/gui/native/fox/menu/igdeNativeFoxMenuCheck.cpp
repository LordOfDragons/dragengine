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

#ifdef IGDE_TOOLKIT_FOX

#include "igdeNativeFoxMenuCheck.h"
#include "../../../igdeContainer.h"
#include "../../../igdeCommonDialogs.h"
#include "../../../event/igdeAction.h"
#include "../../../menu/igdeMenuCheck.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Events
///////////

FXDEFMAP( igdeNativeFoxMenuCheck ) igdeNativeFoxMenuCheckMap[] = {
	FXMAPFUNC( SEL_COMMAND, igdeNativeFoxMenuCheck::ID_SELF, igdeNativeFoxMenuCheck::onMenuAction ),
	FXMAPFUNC( SEL_UPDATE, igdeNativeFoxMenuCheck::ID_SELF, igdeNativeFoxMenuCheck::updateMenuAction )
};



// Class igdeNativeFoxMenuCheck
/////////////////////////////////

FXIMPLEMENT( igdeNativeFoxMenuCheck, FXMenuCheck,
	igdeNativeFoxMenuCheckMap, ARRAYNUMBER( igdeNativeFoxMenuCheckMap ) )

// Constructor, destructor
////////////////////////////

igdeNativeFoxMenuCheck::igdeNativeFoxMenuCheck(){ }

igdeNativeFoxMenuCheck::igdeNativeFoxMenuCheck( igdeMenuCheck &powner, FXComposite *pparent ) :
FXMenuCheck( pparent, BuildConstrText( powner ), this, ID_SELF ),
pOwner( &powner )
{
	if( ! powner.GetEnabled() ){
		disable();
	}
	
	setCheck( powner.GetChecked() );
}

igdeNativeFoxMenuCheck::~igdeNativeFoxMenuCheck(){
}

igdeNativeFoxMenuCheck *igdeNativeFoxMenuCheck::CreateNativeWidget( igdeMenuCheck &powner ){
	if( ! powner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const pparent = ( FXComposite* ) powner.GetParent()->GetNativeContainer();
	if( ! pparent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxMenuCheck( powner, pparent );
}

void igdeNativeFoxMenuCheck::PostCreateNativeWidget(){
	FXComposite &pparent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( pparent.id() ){
		create();
	}
}

void igdeNativeFoxMenuCheck::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeFoxMenuCheck::UpdateChecked(){
	setCheck( pOwner->GetChecked() );
}


FXString igdeNativeFoxMenuCheck::BuildConstrText( igdeMenuCheck &powner ){
	return igdeUIFoxHelper::MnemonizeString( powner.GetText(), powner.GetMnemonic() )
		+ "\t" + igdeUIFoxHelper::AccelString( powner.GetHotKey() )
		+ "\t" + powner.GetDescription().GetString();
}



// Events
///////////

long igdeNativeFoxMenuCheck::onMenuAction( FXObject*, FXSelector, void* ){
	if( ! pOwner->GetEnabled() ){
		return 0;
	}
	
	pOwner->SetChecked( getCheck() );
	
	try{
		pOwner->OnAction();
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( pOwner, e );
		return 0;
	}
	
	return 1;
}

long igdeNativeFoxMenuCheck::updateMenuAction( FXObject*, FXSelector, void* ){
	igdeAction * const action = pOwner->GetAction();
	if( ! action ){
		return 0;
	}
	
	try{
		action->Update();
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
	}
	
	return 0;
}

#endif
