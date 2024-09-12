# Available at setup time due to pyproject.toml
from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup, find_packages
from setuptools.command.install import install
from setuptools.command.install_lib import install_lib
from glob import glob
import os
import sys
import shutil
import subprocess
import atexit
from time import sleep

__version__ = "0.1.2"

from setuptools import setup, find_packages
from setuptools.command.install import install
import subprocess



# https://stackoverflow.com/questions/20288711/post-install-script-with-python-setuptools/38422349#38422349
def _post_install():
    print('POST INSTALL')

#class FinishInstallCommand(install):
#class FinishInstallLib(install_lib):

    #def run(self):
    #    install.run(self)
    #def finalize_options(self):
    #    install.finalize_options(self)
#    def run(self):
#        install_lib.run(self)

    main_file = subprocess.check_output([sys.executable, "-c", "import ceto; print(ceto.__file__)"], text=True)
    main_dir = os.path.dirname(main_file)
    print("main_dir", main_dir)

    for f in os.listdir(main_dir):
        if ".macro_impl" in f:
            os.remove(os.path.join(main_dir, f))

    for f in os.listdir(os.path.join(rootdir, "include")):
        if f.endswith(".cth"):
            print(f)
            subprocess.run([sys.executable, "-m", "ceto", "--_nostandardlibmacros", os.path.join(main_dir, "ceto_private_" + f)])

    for f in [os.path.join(rootdir, "tests", "regression", "bounds_check.ctp"), os.path.join(rootdir, "tests", "macros_list_comprehension.ctp"), os.path.join(rootdir, "tests", "regression", "template_func_builtin_macro_convenience.ctp")]:
        print(f)
        subprocess.run([sys.executable, "-m", "ceto", f])
        


class new_install(install):
    def __init__(self, *args, **kwargs):
        super(new_install, self).__init__(*args, **kwargs)
        atexit.register(_post_install)

rootdir = os.path.dirname(__file__)
manifest = os.path.join(rootdir, "MANIFEST.in")

packaged_include_ceto = os.path.join(rootdir, "ceto", "ceto.h")
packaged_ast_header = os.path.join(rootdir, "ceto", "ast.cth")
packaged_utility_header = os.path.join(rootdir, "ceto", "utility.cth")
packaged_range_utility_header = os.path.join(rootdir, "ceto", "range_utility.cth")
packaged_visitor_header = os.path.join(rootdir, "ceto", "visitor.cth")

extra_packaged = [manifest, packaged_ast_header, packaged_utility_header, packaged_range_utility_header, packaged_visitor_header]

with open(manifest, "w") as f:
    f.write("""
include ceto/*.cth
include ceto/*.h
""")

for f in os.listdir(os.path.join(rootdir, "include")):
    if f.endswith(".cth"):
        dest = os.path.join(rootdir, "ceto", "ceto_private_" + f)
        shutil.copyfile(os.path.join(rootdir, "include", f), dest)
        extra_packaged.append(dest)

shutil.copyfile(os.path.join(rootdir, "include", "ceto.h"), packaged_include_ceto)
shutil.copyfile(os.path.join(rootdir, "selfhost", "ast.cth"), packaged_ast_header)
shutil.copyfile(os.path.join(rootdir, "selfhost", "utility.cth"), packaged_utility_header)
shutil.copyfile(os.path.join(rootdir, "selfhost", "range_utility.cth"), packaged_range_utility_header)
shutil.copyfile(os.path.join(rootdir, "selfhost", "visitor.cth"), packaged_visitor_header)

if sys.platform == "win32":
    _extra_compile_args = ["/Zc:__cplusplus", "/Wall", "/permissive-"]
    _extra_link_args = []
else:
    _extra_link_args = ["-Wl,-ldl"]
    _extra_compile_args = ["-O0", "-g3"]

ext_modules = [
    Pybind11Extension("ceto._abstractsyntaxtree",
        ["selfhost/ast.donotedit.autogenerated.cpp"],
        #define_macros = [('VERSION_INFO', __version__)],
        cxx_std=20,
        include_dirs=["include", "selfhost/thirdparty"],
        extra_compile_args=_extra_compile_args,
        extra_link_args=_extra_link_args
    ),
]

setup(
    name="ceto",
    packages = ["ceto"],
    entry_points = {
        'console_scripts': ['ceto=ceto.compiler:main'],
    },
    cmdclass={
        'build_ext': build_ext,
        'install': new_install,
        #'install_lib': FinishInstallLib
    },
    version=__version__,
    author="Ehren Metcalfe",
    author_email="ehren.m@gmail.com",
    url="https://github.com/ehren/ceto",
    description="General purpose programming language transpiled to C++",
    long_description="Parens/call expression language transpiled to c++20. \"Python\" with 2 parentheses moved or inserted (with extra C++ syntax). Codegen based on https://github.com/lukasmartinelli/py14 with additions e.g. implicit make_shared/unique, checked autoderef via '.', swiftish lambda capture, implicit move from last use of unique, const by default, extra CTAD!",
    ext_modules=ext_modules,
    include_package_data=True,
    extras_require={"test": "pytest"},
    install_requires=[
        'cpyparsing',  # pyparsing also supported
    ],
    zip_safe=False,
    python_requires=">=3.8",
)
