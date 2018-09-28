#!/bin/python
#pylint: disable=too-few-public-methods, missing-docstring, C0413
#-----------------------------------------------------------------------------
# Image Processing Library
#
# MIT License
# Copyright (c) 2018 Martin A Slater (mslater@hellinc.net)
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#-----------------------------------------------------------------------------
"""
Command line tool to generate the function documentation images.
"""

#-----------------------------------------------------------------------------
# Imports
#-----------------------------------------------------------------------------
from __future__ import absolute_import, print_function
import os
import platform
import argparse
import subprocess
import tempfile
import stat
import shutil
import time

#-----------------------------------------------------------------------------
# Constants
#-----------------------------------------------------------------------------

BASE_DIR = os.path.realpath(os.path.dirname(__file__))
OUTPUT_DIR = os.path.join(BASE_DIR, 'pages', 'functions', 'images')
IMAGE_DIR = os.path.join(BASE_DIR, 'images')
BUILD_DIR = os.path.join(BASE_DIR, '..', 'build', 'bin')

if platform.system() == 'Windows':
    BUILD_DIR = os.path.join(BUILD_DIR, 'Debug')

DRIVER_PATH = os.path.join(BUILD_DIR, 'ty_ipl_driver')

# unary functions
UNARY_FUNCTIONS = [
    'auto_level_opencv',
    'auto_level_rgb_stretch',
    'greyscale',
    'min',
    'max',
    'sepia_rgb',
    'visualize_palette'
]

# binary functions
BINARY_FUNCTIONS = [
    'bitwise_and',
    'bitwise_or',
    'bitwise_xor',
    'blend_add',
    'blend_average',
    'blend_color_burn',
    'blend_color_dodge',
    'blend_darken',
    'blend_difference',
    'blend_exclusion',
    'blend_glow',
    'blend_hard_light',
    'blend_hard_max',
    'blend_lighten',
    'blend_linear_burn',
    'blend_linear_dodge',
    'blend_linear_light',
    'blend_multiply',
    'blend_negation',
    'blend_normal',
    'blend_overlay',
    'blend_phoenix',
    'blend_pin_light',
    'blend_reflect',
    'blend_screen',
    'blend_soft_light',
    'blend_sub',
    'blend_vivid_lights',
    'add',
    'subtract',
    'multiply',
    'divide'
]

# parameterized functions

#------------------------------------------------------------------------------
# A whole lot of messing around for deleting a directory and all its contents
# on windows. There can be a race condition between deleting files in a
# directory and deleting the actual directory that causes intermitent failures
# with 'WindowsError: [Error 145] The directory is not empty'. This is caused
# by some other process (antivirus, indexing etc) having an outstanding handle
# open. The code below is taken from https://hg.python.org/cpython/rev/c863dadc65eb
# which addresses this issue. For more details see the thread at
# https://mail.python.org/pipermail/python-dev/2013-September/128353.html
#------------------------------------------------------------------------------

def _waitfor(func, pathname, waitall=False):
    func(pathname)

    # setup the wait loop
    if waitall:
        dirname = pathname
    else:
        dirname, name = os.path.split(pathname)
        dirname = dirname or '.'
    # Check for `pathname` to be removed from the filesystem.
    timeout = 0.001
    while timeout < 1.0:
        # Note we are only testing for the existance of the file(s) in
        # the contents of the directory regardless of any security or
        # access rights.
        L = os.listdir(dirname)
        if not (L if waitall else name in L):
            return
        # Increase the timeout and try again
        time.sleep(timeout)
        timeout *= 2

def _unlink(filename):
    _waitfor(os.unlink, filename)

def _rmdir(dirname):
    _waitfor(os.rmdir, dirname)

def rmtree(path):
    def _rmtree_inner(path):
        for name in os.listdir(path):
            fullname = os.path.join(path, name)
            if os.path.isdir(fullname):
                os.chmod(fullname, stat.S_IWUSR)
                _waitfor(_rmtree_inner, fullname, waitall=True)
                os.rmdir(fullname)
            else:
                os.unlink(fullname)
    _waitfor(_rmtree_inner, path, waitall=True)
    _waitfor(os.rmdir, path)

#-----------------------------------------------------------------------------
# Class
#-----------------------------------------------------------------------------

class MakeDocImages(object):
    """ MakeDocImages """

    def __init__(self, args):
        """ Constructor """

    def run(self):
        self._unary_funcs()
        self._binary_funcs()

    def _unary_funcs(self):
        for func in UNARY_FUNCTIONS:
            script = '''
                    call experiment_add_image(name="source", src=__src__);
                    call %s(src=__src__, dst=__dst__);
                    ''' % (func)
            self._execute_script(func, script, 'nature.png')

    def _binary_funcs(self):
        for func in BINARY_FUNCTIONS:
            script = '''
                    input image src2;
                    call load_image(path="%s", dst=src2);
                    call experiment_add_image(name="source", src=__src__);
                    call experiment_add_image(name="mask", src=src2);
                    call %s(src1=__src__, src2=src2, dst=__dst__);
                    ''' % (os.path.join(IMAGE_DIR, 'mask1.png'), func)
            self._execute_script(func, script, 'nature.png')

    def _execute_script(self, func, script, source_image):
            temp_dir = tempfile.mkdtemp(prefix='fx_')
            fname = tempfile.mkstemp(suffix='.fx', prefix=func + '_')[1]
            with open(fname, 'w+') as fx:
                fx.write(script)
            self._execute(fname, os.path.join(IMAGE_DIR, source_image), temp_dir)

            # get the output and copy to the correct directory
            output = self._find_contact_sheet(temp_dir)
            dst = os.path.join(OUTPUT_DIR, func + '.png')
            shutil.copyfile(output, dst)
            rmtree(temp_dir)

    def _execute(self, script_path, image_path, temp_dir):
        args = [DRIVER_PATH, '--experiment', '--output_dir=' + temp_dir, script_path, image_path]
        if subprocess.call(args):
            raise Exception(' '.join(args))

    def _find_contact_sheet(self, temp_dir):
        # find path to the contact sheet
        dirs = os.listdir(temp_dir)
        assert len(dirs) == 1
        odir = os.path.join(temp_dir, dirs[0])
        files = [f for f in os.listdir(odir) if os.path.isfile(os.path.join(odir, f))]
        assert len(files) == 1
        return os.path.join(odir, files[0])




#-----------------------------------------------------------------------------
# Main
#-----------------------------------------------------------------------------

def main():
    """ Main script entry point """
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('-a', '--arg',
                        help='A boolean arg',
                        dest='barg', action='store_true')
    args = parser.parse_args()
    MakeDocImages(args).run()

if __name__ == "__main__":
    main()
