#include "DumpChain.h"

#include <language/duchain/types/identifiedtype.h>
#include <language/duchain/ducontext.h>
#include <language/duchain/topducontext.h>
#include <language/duchain/declaration.h>
#include <language/duchain/duchainpointer.h>
#include <language/duchain/use.h>
#include <serialization/indexedstring.h>

#include <QDebug>

using namespace KDevelop;

namespace DragonScript {

DumpChain::DumpChain() : indent( 0 ){
}

DumpChain::~DumpChain(){
}

void DumpChain::dump( DUContext *context, bool imported ){
	if( ! context ){
		return;
	}
	
	qDebug() << "KDevDScript: DumpChain" << QString( indent*2, ' ' )
		<< ( imported ? "==import==> Context " : "New Context " )
		<< context->scopeIdentifier( true )
		<< context->transformFromLocalRevision( context->range() ) << " " << context
		<< " " << ( dynamic_cast<TopDUContext*>( context ) ? "top-context" : "" );
	
	if( ! imported )	{
		foreach( Declaration *dec, context->localDeclarations() ){
			const auto uses = dec->uses();
			qDebug() << "KDevDScript: ContextBuilder" << QString( ( indent + 1 ) * 2, ' ' )
				<< "Declaration: " << dec->toString() << " [" << dec->qualifiedIdentifier() << "]  "
				<< dec << "(internal ctx" << dec->internalContext() << ")"
				<< context->transformFromLocalRevision( dec->range() ) << ", "
				<< ( dec->isDefinition() ? "definition, " : "declaration, " )
				<< uses.count() << "use(s)";
			
			for( auto it = uses.constBegin(); it != uses.constEnd(); ++it ){
				qDebug() << "KDevDScript: ContextBuilder" << QString( ( indent + 1 ) * 2, ' ' )
					<< "File:" << it.key().str();
				
				foreach( const RangeInRevision &r, it.value() ){
					qDebug() << "KDevDScript: ContextBuilder" << QString( ( indent + 2 ) * 2, ' ')
						<< "Use:" << context->transformFromLocalRevision( r );
				}
			}
		}
	}
	
	++indent;
	if( ! imported ){
		foreach( const DUContext::Import &parent, context->importedParentContexts() ){
			dump( parent.context( dynamic_cast<TopDUContext*>( context ) ), true );
		}
		foreach( DUContext *child, context->childContexts() ){
			dump( child );
		}
	}
	--indent;
}

}
