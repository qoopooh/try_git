import sys

from distutils.core import setup
import py2exe

sys.path.append("C:\\Program Files\\Microsoft Visual Studio 9.0\\VC\\redist\\x86\\Microsoft.VC90.CRT")

setup(
    windows=['licence.py'],
    data_files= [
        ('', ['exit.png']),
        ('', ['kpdf.png']),
        ('', ['COPYING.txt']),
    ]
)

