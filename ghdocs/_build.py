import os
import re
import shutil
import datetime
from github import Github
from github.GitRelease import GitRelease


def run_command(cmdline: str) -> None:
    retval = os.system(cmdline)
    if retval != 0:
        raise Exception("Run command failed: rc={} cmdline='{}'".format(retval, cmdline))

class Release:
    """GitHub release wrapper."""
    reVersion = re.compile(r"^v(?P<major>[0-9]+)\.(?P<minor>[0-9]+)(\.(?P<patch>[0-9]+))?$")
    
    def __init__(self: 'Release', release: GitRelease) -> None:
        self._tag_name = release.tag_name
        self._publish_time = release.published_at
        match = Release.reVersion.match(release.tag_name).groupdict()
        self._major = match['major']
        self._minor = match['minor']
        self._patch = match['patch']
        self._version = '{}.{}.{}'.format(self._major, self._minor, self._patch)\
            if self._patch else '{}.{}'.format(self._major, self._minor)

    @property
    def tag_name(self: 'Release') -> str:
        return self._tag_name

    @property
    def major(self: 'Release') -> str:
        return self._major

    @property
    def minor(self: 'Release') -> str:
        return self._minor

    @property
    def patch(self: 'Release') -> str:
        return self._patch

    @property
    def publish_time(self: 'Release') -> datetime.datetime:
        return self._publish_time

    @property
    def version(self: 'Release') -> str:
        return self._version

    def same_minor(self: 'Release', other: 'Release') -> bool:
        return other._major == self._major and other._minor == self._minor


class DocAppBuilder:
    """Build Doc API."""
    reProjNum = re.compile(r"PROJECT_NUMBER += .+")

    def __init__(self: 'DocAppBuilder',
                 path_base: str,
                 path_artifact: str,
                 path_datafile: str,
                 path_doxyfile: str = "Doxyfile",
                 path_src: str = "src",
                 path_doc: str = "doc/html") -> None:
        self.path_doxyfile = os.path.join(path_base, path_doxyfile)
        self.path_doxyfile_in = self.path_doxyfile + '.in'
        self.path_src = os.path.join(path_base, path_src)
        self.path_doc = os.path.join(path_base, path_doc)
        self.path_artifact = path_artifact
        self.path_datafile = path_datafile

    def build(self: 'DocAppBuilder', version: str, is_latest: bool) -> None:
        shutil.rmtree(self.path_doc, True)
        self._update_doxyfile(version)
        self._build_doxyfile()
        self._copy_apidoc(version)

    def _update_doxyfile(self: 'DocAppBuilder', version: str) -> None:
        if os.path.exists(self.path_doxyfile_in):
            with open(self.path_doxyfile_in, 'r') as f:
                content = f.read()
            content = content.replace('%VERSION%', version)
        else:
            with open(self.path_doxyfile, 'r') as f:
                content = f.read()
            content = DocAppBuilder.reProjNum.sub(
                "PROJECT_NUMBER = {}".format(version), content)
        with open(self.path_doxyfile, 'w') as f:
            f.write(content)

    def _build_doxyfile(self: 'DocAppBuilder') -> None:
        os.chdir(self.path_src)
        run_command("doxygen {}".format(os.path.relpath(
            self.path_doxyfile, self.path_src)))

    def _copy_apidoc(self: 'DocAppBuilder', version: str) -> None:
        dest: str = os.path.join(self.path_artifact, version)
        shutil.rmtree(dest, True)

        directory = os.path.dirname(dest)
        if not os.path.exists(directory):
            os.makedirs(directory)

        shutil.copytree(self.path_doc, dest)

    def write_data_file(self: 'DocAppBuilder', versions: list[Release]) -> None:
        directory = os.path.dirname(self.path_datafile)
        if not os.path.exists(directory):
            os.makedirs(directory)

        with open(self.path_datafile, 'w') as f:
            for each in versions:
                f.write("- version: '{}'\n".format(each.version))


class XmlSchemaBuilder:
    """Build XML Schema."""

    def __init__(self: 'XmlSchemaBuilder',
                 path_src: str,
                 path_artifact: str,
                 path_datafile: str) -> None:
        self.path_src = path_src
        self.path_artifact = path_artifact
        self.path_datafile = path_datafile

    def build(self: 'XmlSchemaBuilder', version: str, is_latest: bool) -> None:
        self._copy_xmlschema(version)
        if is_latest:
            self._copy_xmlschema('latest')

    def _copy_xmlschema(self: 'XmlSchemaBuilder', version: str) -> None:
        dest: str = os.path.join(self.path_artifact, version)
        shutil.rmtree(dest, True)

        directory = os.path.dirname(dest)
        if not os.path.exists(directory):
            os.makedirs(directory)

        if os.path.exists(self.path_src):
            shutil.copytree(self.path_src, dest)

    def write_data_file(self: 'XmlSchemaBuilder', versions: list[Release]) -> None:
        directory = os.path.dirname(self.path_datafile)
        if not os.path.exists(directory):
            os.makedirs(directory)

        with open(self.path_datafile, 'w') as f:
            for each in versions:
                f.write("- version: '{}'\n".format(each.version))


def find_repo_releases(repo_name: str) -> list[Release]:
    """Find all released of repository.

    Sorts releases by time from newest to oldest.
    Skips nightly release. Keeps only the latest
    minor release of releases with same major.
    """
    with Github() as gh:
        repo = gh.get_repo(repo_name)

        releases: list[Release] = []

        for each in repo.get_releases():
            if each.tag_name == "nightly":
                continue
            try:
                releases.append(Release(each))
            except Exception:
                continue

        releases.sort(key=lambda x: x.publish_time)
        releases.reverse()
        temp_list = releases
        releases = []

        for each in temp_list:
            if not any(x for x in releases if each.same_minor(x)):
                releases.append(each)
    return releases

def build_dragengine(releases: list[Release]) -> None:
    curdir = os.getcwd()

    pathApiDoc = os.path.join(curdir, "artifacts", "apidoc")
    pathDataDir = os.path.join(curdir, "_data", "apidoc")
    pathBaseSrc = os.path.join(curdir, "_repository", "dragengine", "src")

    builders: list[DocAppBuilder|XmlSchemaBuilder] = []

    builders.append(DocAppBuilder(
        path_base=os.path.join(pathBaseSrc, "dragengine"),
        path_artifact=os.path.join(pathApiDoc, "dragengine"),
        path_datafile=os.path.join(pathDataDir, "dragengine.yml"),
        path_src="src",
        path_doc="src/doc/html"))

    builders.append(DocAppBuilder(
        path_base=os.path.join(pathBaseSrc, "launcher", "shared"),
        path_artifact=os.path.join(pathApiDoc, "delauncher"),
        path_datafile=os.path.join(pathDataDir, "delauncher.yml"),
        path_src="."))

    builders.append(DocAppBuilder(
        path_base=os.path.join(pathBaseSrc, "deigde", "deigde"),
        path_artifact=os.path.join(pathApiDoc, "deigde"),
        path_datafile=os.path.join(pathDataDir, "deigde.yml"),
        path_src="shared/src"))

    builders.append(DocAppBuilder(
        path_base=os.path.join(pathBaseSrc, "modules", "scripting", "dragonscript"),
        path_artifact=os.path.join(pathApiDoc, "dragonscript"),
        path_datafile=os.path.join(pathDataDir, "dragonscript.yml"),
        path_doxyfile="doxyfile_script",
        path_src=".",
        path_doc="doc/script/html"))

    builderXmlSchema = XmlSchemaBuilder(
        path_src=os.path.join(pathBaseSrc, "dragengine", "doc", "xmlschema"),
        path_artifact=os.path.join(curdir, "artifacts", "xmlschema", "dragengine"),
        path_datafile=os.path.join(curdir, "_data", "xmlschema", "xmlschema.yml"))
    builders.append(builderXmlSchema)

    for each in builders:
        shutil.rmtree(each.path_artifact, True)

    if not os.path.exists("_repository/dragengine"):
        if not os.path.exists("_repository"):
            os.makedirs("_repository")
        run_command(' '.join([
            "git",
            "clone",
            "https://github.com/LordOfDragons/dragengine.git",
            "_repository/dragengine"]))

    os.chdir("_repository/dragengine")

    for release in releases:
        run_command("git checkout -f '{}'".format(release.tag_name))
        run_command("git reset --hard")
        run_command("git clean -x -f")

        is_latest = release == releases[0]
        for builder in builders:
            builder.build(release.version, is_latest)

    # temporary
    run_command("git checkout master")
    run_command("git reset --hard")
    run_command("git clean -x -f")
    builderXmlSchema.build('latest', False)
    # end temporary

    os.chdir(curdir)
    for each in builders:
        each.write_data_file(releases)

releases = find_repo_releases("LordOfDragons/dragengine")
limit = 3 # 5
build_dragengine(releases[:limit])
