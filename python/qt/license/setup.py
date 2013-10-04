import sys, subprocess

from distutils.core import setup
import py2exe

sys.path.append("C:\\Program Files\\Microsoft Visual Studio 9.0\\VC\\redist\\x86\\Microsoft.VC90.CRT")
subprocess.call(['pyside-uic', 'licence.ui', '-o', 'ui_licence.py'])

setup(
    windows=[{
        'script':'licence.py',
        'icon_resources':[(1,'lic.ico')]
        }],
    data_files= [
        ('', ['exit.png']),
        ('', ['kpdf.png']),
        ('', ['COPYING.txt']),
    ]
)

