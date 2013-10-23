
__all__ = ('Heartbeat')

from kivy.factory import Factory
from kivy.uix.label import Label
from kivy.uix.image import Image
from kivy.properties import StringProperty, OptionProperty, \
         ObjectProperty, BooleanProperty

class Heartbeat(Label):

    state = OptionProperty('down', options=('down', 'beat'))
    allow_stretch = BooleanProperty(True)
    keep_ratio = BooleanProperty(False)
    border = ObjectProperty(None)
    beat_delay = ObjectProperty(None)
    beat = BooleanProperty(False)
    run = BooleanProperty(False)
    run_interval = ObjectProperty(None)

    def __init__(self, **kwargs):
        super(Heartbeat, self).__init__(**kwargs)

        self.register_event_type('on_beat')
        self.register_event_type('on_down')

    def on_down(self):
        pass

    def on_beat(self):
        pass


Factory.register('Heartbeat', cls=Heartbeat)

if __name__ == '__main__':
    from kivy.uix.gridlayout import GridLayout
    from kivy.base import runTouchApp

    class MainView(GridLayout):
        def __init__(self, **kwargs):
            kwargs['cols'] = 2
            super(MainView, self).__init__(**kwargs)

            hb = Heartbeat()
            hb.run = True
            self.add_widget(hb)

    runTouchApp(MainView())

