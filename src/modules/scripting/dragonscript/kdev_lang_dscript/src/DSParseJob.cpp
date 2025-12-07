#include <QFile>
#include <QReadWriteLock>
#include <QDirIterator>

#include <ktexteditor/document.h>

#include <language/duchain/duchainlock.h>
#include <language/duchain/duchain.h>
#include <language/duchain/topducontext.h>
#include <interfaces/icore.h>
#include <interfaces/ilanguagecontroller.h>
#include <interfaces/iprojectcontroller.h>
#include <language/backgroundparser/backgroundparser.h>
#include <language/backgroundparser/urlparselock.h>
#include <language/editor/documentrange.h>

#include <QtCore/QReadLocker>
#include <QtCore/QThread>
#include <language/duchain/duchainutils.h>

#include <KF5/KConfigCore/kconfiggroup.h>

#include <mutex>

#include "DSParseJob.h"
#include "ParseSession.h"
#include "DSLanguageSupport.h"
#include "dsp_debugvisitor.h"
#include "duchain/DeclarationBuilder.h"
#include "duchain/EditorIntegrator.h"
#include "duchain/DumpChain.h"
#include "duchain/UseBuilder.h"
#include "duchain/Helpers.h"

using KDevelop::DocumentRange;
using KDevelop::DUChain;
using KDevelop::DUChainReadLocker;
using KDevelop::DUChainWriteLocker;
using KDevelop::DUContext;
using KDevelop::ICore;
using KDevelop::ParsingEnvironmentFile;
using KDevelop::ParsingEnvironmentFilePointer;
using KDevelop::Problem;
using KDevelop::RangeInRevision;
using KDevelop::UrlParseLock;
using KDevelop::DUChainUtils::standardContextForUrl;

static const IndexedString languageString("DragonScript");

namespace DragonScript{

DSParseJob::DSParseJob(const IndexedString &url, ILanguageSupport *languageSupport) :
ParseJob(url, languageSupport),
pParentJob(nullptr),
pDUContext(nullptr){
}

DSParseJob::~DSParseJob(){
}

DSLanguageSupport* DSParseJob::ds() const{
	return dynamic_cast<DSLanguageSupport*>(languageSupport());
}

void DSParseJob::run(ThreadWeaver::JobPointer self, ThreadWeaver::Thread *thread){
	Q_UNUSED(self)
	Q_UNUSED(thread)
	
	if(abortRequested() || ICore::self()->shuttingDown()){
		abortJob();
		return;
	}
	
	QReadLocker parselock(languageSupport()->parseLock());
	UrlParseLock urlLock(document());
	
	ProblemPointer p = readContents();
	
	if(!(minimumFeatures() & (TopDUContext::ForceUpdate | Resheduled))
	&& !isUpdateRequired(languageString)){
		DUChainReadLocker lock;
		foreach(const ParsingEnvironmentFilePointer &file,
		DUChain::self()->allEnvironmentFiles(document())){
			if(file->language() != languageString){
				continue;
			}
			
			if(!file->needsUpdate() && file->featuresSatisfied(minimumFeatures()) && file->topContext()){
				qDebug() << "KDevDScript: DSParseJob::run: Already up to date" << document().str();
				setDuChain(file->topContext());
				if(ICore::self()->languageController()->backgroundParser()->trackerForUrl(document())){
					lock.unlock();
					highlightDUChain();
				}
				return;
			}
			break;
		}
	}
	qDebug() << "KDevDScript: DSParseJob::run: parsing" << document().str();
	
	ReferencedTopDUContext toUpdate = nullptr;
	{
		DUChainReadLocker lock;
		toUpdate = standardContextForUrl(document().toUrl());
	}
	if(toUpdate){
		translateDUChainToRevision(toUpdate);
		toUpdate->setRange(RangeInRevision(0, 0, INT_MAX, INT_MAX));
	}
	
	/*if( p ){  // ???
		//TODO: associate problem with topducontext
		abortJob();
		return;
	}*/
	
	ParseSession session(document(), contents().contents);
// 	session.setDebug( true );
	
	StartAst *ast = nullptr;
	bool matched = session.parse(&ast);
	
	if(abortRequested() || ICore::self()->shuttingDown()){
		abortJob();
		return;
	}
	
	EditorIntegrator editor(session);
	
	if(matched){
		// set up the declaration builder, it gets the parsePriority so it can re-schedule imported files with a better priority
		DeclarationBuilder builder(editor, parsePriority(), session);
		
		pDUContext = builder.build(document(), ast, toUpdate.data());
		
		if(builder.requiresReparsing()){
			qDebug() << "KDevDScript: DSParseJob::run: Reschedule file" << document().str() << "for parsing";
			abortJob();
			const TopDUContext::Features feat = static_cast<TopDUContext::Features>(
					minimumFeatures() | TopDUContext::VisibleDeclarationsAndContexts | Resheduled);
			ICore::self()->languageController()->backgroundParser()->addDocument(document(),
				feat, parsePriority(), nullptr, DSParseJob::FullSequentialProcessing, 500);
			return;
		}
		
// 		DumpChain().dump( pDUContext );
		if(abortRequested() || ICore::self()->shuttingDown()) {
			abortJob();
			return;
		}
		
		setDuChain(pDUContext);
		
		// gather uses of variables and functions on the document
		UseBuilder usebuilder(editor);
		usebuilder.buildUses(ast);
		
		// some internal housekeeping work
		{
		DUChainWriteLocker lock(DUChain::lock());
		pDUContext->setFeatures(minimumFeatures());
		ParsingEnvironmentFilePointer parsingEnvironmentFile = pDUContext->parsingEnvironmentFile();
		if(parsingEnvironmentFile){
			parsingEnvironmentFile->setModificationRevision(contents().modification);
			DUChain::self()->updateContextEnvironment(pDUContext, parsingEnvironmentFile.data());
		}
		}
		
		qDebug() << "KDevDScript: DSParseJob::run: Parsing succeeded" << document().str();
		
		if(abortRequested() || ICore::self()->shuttingDown()){
			abortJob();
			return;
		}
		
		// start the code highlighter if parsing was successful.
		highlightDUChain();
		
	}else{
		qDebug() << "KDevDScript: DSParseJob::run: Parsing failed" << document().str();
		
		DUChainWriteLocker lock;
		pDUContext = toUpdate.data();
		// if there's already a chain for the document, do some cleanup.
		if(pDUContext){
			ParsingEnvironmentFilePointer parsingEnvironmentFile = pDUContext->parsingEnvironmentFile();
			if(parsingEnvironmentFile){
				//parsingEnvironmentFile->clearModificationRevisions(); // TODO why?
				parsingEnvironmentFile->setModificationRevision(contents().modification);
			}
			pDUContext->clearProblems();
			
		// otherwise, create a new, empty top context for the file. This serves as a placeholder until
		// the syntax is fixed; for example, it prevents the document from being reparsed again until it is modified.
		}else{
			ParsingEnvironmentFile * const file =
				new ParsingEnvironmentFile(document());
			file->setLanguage(languageString);
			pDUContext = new TopDUContext(document(),
				RangeInRevision(0, 0, INT_MAX, INT_MAX), file);
			pDUContext->setType(DUContext::Global);
			DUChain::self()->addDocumentChain(pDUContext);
			Q_ASSERT(pDUContext->type() == DUContext::Global);
		}
		
		setDuChain(pDUContext);
	}
	
	if(abortRequested() || ICore::self()->shuttingDown()){
		abortJob();
		return;
	}
	
	// The parser might have given us some syntax errors, which are now added to the document.
	{
	DUChainWriteLocker lock;
	foreach(const ProblemPointer &p, session.problems()){
		pDUContext->addProblem(p);
	}
	}
	
	// If enabled, and if the document is open, do PEP8 checking.
	//eventuallyDoPEP8Checking( pDUContext );
	
	if(minimumFeatures() & TopDUContext::AST){
		DUChainWriteLocker lock;
		//m_currentSession->ast = m_ast; // ??
		//pDUContext->setAst( QExplicitlySharedDataPointer<IAstContainer>( session.data() ) ); ??
	}
	
	setDuChain(pDUContext);
	
	DUChain::self()->emitUpdateReady(document(), duChain());
	qDebug() << "KDevDScript: DSParseJob::run: emitUpdateReady" << document().str();
}

void DSParseJob::setParentJob(DSParseJob *job){
	pParentJob = job;
}

bool DSParseJob::hasParentDocument(const IndexedString &doc){
	if(document() == doc){
		return true;
	}
	if(!pParentJob){
		return false;
	}
	if(pParentJob->document() == doc){
		return true;
	}
	
	return pParentJob->hasParentDocument(doc);
}

ProblemPointer DSParseJob::createProblem(const QString &description, AstNode *node,
EditorIntegrator *editor, IProblem::Source source, IProblem::Severity severity){
	Q_UNUSED(editor)
	Q_UNUSED(node)
	
	ProblemPointer p(new Problem());
	p->setSource(source);
	p->setSeverity(severity);
	p->setDescription(description);
	p->setFinalLocation(DocumentRange(document(), editor->findRange(*node).castToSimpleRange()));
	qDebug() << "KDevDScript: DSParseJob::run:" << p->description();
	return p;
}

}
