from flask import Flask

app = Flask(__name__)

__version__ = '0.2'

@app.route('/')
def index():
    return 'App ' + __version__

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8000)

