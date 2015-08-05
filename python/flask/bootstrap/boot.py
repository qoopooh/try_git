#!/usr/bin/env python

__version__ = "1.0"

from flask import Flask, render_template
import datetime

app = Flask(__name__)

def get_git_version():
    import subprocess
    label = subprocess.check_output(['git', 'rev-parse', '--short', 'HEAD'])
    return label
__version__ += "_" + get_git_version()

@app.route("/")
def hello():
    time_string = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    templateData = {
        'title': 'Flask ' + __version__,
        'page': u'บ้าน',
        'time' : time_string,
        'my_string': 'Berm Win',
        'my_list': [0,1,2,3,4,5],
    }

    return render_template('template.html', **templateData)

