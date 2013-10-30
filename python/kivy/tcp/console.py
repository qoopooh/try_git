from kivy.uix.boxlayout import BoxLayout
from kivy.lang import Builder

Builder.load_file('console.kv')

class Console(BoxLayout):

    def text(self, val):
        print('text input text is: {txt}'.format(txt=val))


