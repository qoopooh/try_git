#!/usr/bin/env python

from flask import Flask, render_template

app = Flask(__name__)

__version__ = '0.1'

@app.route('/')
def index():
    templateData = {
        'title': 'Flask ' + __version__,
    }

    return render_template('index.html', **templateData)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8000, debug=True)

