#include <QFileDialog>

#include "ProjectConfigPage.h"

#include "ui_projectConfigPage.h"
#include "../duchain/Helpers.h"

namespace DragonScript {

ProjectConfigPage::ProjectConfigPage(IPlugin* self,
const KDevelop::ProjectConfigOptions &options, QWidget *parent) :
ConfigPage(self, nullptr, parent),
pUI(new Ui_ProjectConfig)
{
	pConfigGroup = options.project->projectConfiguration()->group("dragonscriptsupport");
	pUI->setupUi(this);
	pProject = options.project;
	
	pModelPathInclude = new QStringListModel(this);
	pUI->listPathInclude->setModel(pModelPathInclude);
	
	connect(pUI->editPathDSI, SIGNAL(textChanged(const QString &)), this, SLOT(editPathDSIChanged()));
	
	connect(pUI->btnPathIncludeSelect, SIGNAL(clicked()), this, SLOT(btnPathIncludeSelect()));
	connect(pUI->btnPathIncludeAdd, SIGNAL(clicked()), this, SLOT(btnPathIncludeAdd()));
	connect(pUI->btnPathIncludeRemove, SIGNAL(clicked()), this, SLOT(btnPathIncludeRemove()));
}

ProjectConfigPage::~ProjectConfigPage(){
}

QString ProjectConfigPage::name() const{
	return i18n("DragonScript Settings");
}



void ProjectConfigPage::apply(){
	pConfigGroup.writeEntry("pathDSI", pUI->editPathDSI->text());
	pConfigGroup.writeEntry("pathInclude", pModelPathInclude->stringList());
	// force reparsing?
}

void ProjectConfigPage::defaults(){
	pUI->editPathDSI->setText(QString());
	pUI->editPathInclude->setText(QString());
	pModelPathInclude->removeRows(0, pModelPathInclude->rowCount());
	
	emit changed();
}

void ProjectConfigPage::reset(){
	pUI->editPathDSI->setText(pConfigGroup.readEntry("pathDSI", ""));
	pModelPathInclude->setStringList(pConfigGroup.readEntry("pathInclude", QStringList()));
	
	emit changed();
}



void ProjectConfigPage::editPathDSIChanged(){
	emit changed();
}

void ProjectConfigPage::btnPathIncludeSelect(){
	QString path(pUI->editPathInclude->text());
	if(path.isEmpty()){
		path = pProject->path().toLocalFile();
	}
	
	path = QFileDialog::getExistingDirectory(this, i18n("Select Include Directory"),
		path, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	
	if(! path.isEmpty()){
		pUI->editPathInclude->setText(path);
	}
}

void ProjectConfigPage::btnPathIncludeAdd(){
	const QString path(pUI->editPathInclude->text());
	if(path.isEmpty()){
		return;
	}
	
	if(pModelPathInclude->stringList().contains(path)){
		return;
	}
	
	const int row = pModelPathInclude->rowCount();
	pModelPathInclude->insertRows(row, 1);
	const QModelIndex index(pModelPathInclude->index(row));
	pModelPathInclude->setData(index, path);
	pUI->listPathInclude->setCurrentIndex(index);
	
	emit changed();
}

void ProjectConfigPage::btnPathIncludeRemove(){
	const QModelIndex index(pUI->listPathInclude->currentIndex());
	if(! index.isValid()){
		return;
	}
	
	pModelPathInclude->removeRows(index.row(), 1);
	
	emit changed();
}

}
