#!/usr/bin/env python

from flask import Flask, render_template
import datetime

app = Flask(__name__)

@app.route("/")
def hello():
    time_string = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    templateDate = {
        'title': 'Flask App',
        'time' : time_string
    }

    return render_template('time.html', **templateDate)

if __name__ == "__main__":
    #app.run(host='0.0.0.0', port=8081, debug=True)
    app.run(host='0.0.0.0', port=8081)

