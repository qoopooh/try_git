import os
from kivy.uix.boxlayout import BoxLayout
from kivy.lang import Builder

path = os.path.dirname(__file__)
path = os.path.join(path, 'console.kv')
Builder.load_file(path)

class Console(BoxLayout):

    def __init__(self, app, **kwargs):
        super(Console, self).__init__(**kwargs)
        self._app = app


