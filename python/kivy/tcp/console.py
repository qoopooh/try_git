from kivy.uix.boxlayout import BoxLayout
from kivy.lang import Builder

Builder.load_file('console.kv')

class Console(BoxLayout):

    def __init__(self, app, **kwargs):
        super(Console, self).__init__(**kwargs)
        self._app = app

    def text(self, val):
        print('text input text is: {txt}'.format(txt=val))


