import urllib.request
import shutil
from SCons.Builder import Builder
from SCons.Action import Action

# Provides support to download artifact.
#
# Adds builder DownloadArtifact()

# SCons Tools required this method to be present. It is called if exists() returns true
def generate(env):
	def builderDownloadArtifact(env, target, source):
		url = "{}/{}".format(env['url_extern_artifacts'], env['url'])
		filename = target[0].abspath
		
		with urllib.request.urlopen(url) as r, open(filename, 'wb') as f:
			shutil.copyfileobj(r, f)

	def downloadArtifactHelper(filenameArtifact, urlPath, alias, fakeSource='SConscript'):
		if type(filenameArtifact) not in [list, tuple]:
			filenameArtifact = [filenameArtifact]

		nodes = []
		buildNodes = []

		for x in filenameArtifact:
			node = env.File(x).srcnode()
			nodes.append(node)
			buildNodes.append(env.DownloadArtifact(node, fakeSource, url="{}/{}".format(urlPath, x)))
		
		env.Alias(alias, buildNodes)
		return nodes

	env.Append(BUILDERS={'DownloadArtifact': Builder(action=Action(
		builderDownloadArtifact, '$DOWNLOADARTIFACTCOMSTR'))})
	env['DOWNLOADARTIFACTCOM'] = 'Download Artifact "$TARGET"'
	env['DOWNLOADARTIFACTCOMSTR'] = env['DOWNLOADARTIFACTCOM']
	
	env.DownloadArtifactHelper = downloadArtifactHelper

# SCons Tools required this method to be present. Checks if tool is applicable
def exists(env):
	return True
