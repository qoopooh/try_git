
__all__ = ('Heartbeat')

from kivy.factory import Factory
from kivy.uix.label import Label
from kivy.uix.image import Image
#from kivy.graphics import *
from kivy.properties import StringProperty, OptionProperty, \
         ObjectProperty, BooleanProperty

class Heartbeat(Label):

    state = OptionProperty('down', options=('dis', 'down', 'beat'))
    allow_stretch = BooleanProperty(True)
    keep_ratio = BooleanProperty(False)
    border = ObjectProperty(None)
    texture_background = ObjectProperty(None)
    background_beat = StringProperty('atlas://hb/hb_on')
    background_down = StringProperty('atlas://hb/hb_off')
    background_dis = StringProperty('atlas://hb/hb_dis')

    beat_delay = ObjectProperty(None)
    run_interval = ObjectProperty(None)
    beat = BooleanProperty(False)
    run = BooleanProperty(False)


    def __init__(self, **kwargs):
        super(Heartbeat, self).__init__(**kwargs)

        self.register_event_type('on_beat')
        self.register_event_type('on_down')
        self.border = (16, 16, 16, 16)
        self.img = Image(source = self.background_dis,
                allow_stretch = self.allow_stretch,
                keep_ratio = self.keep_ratio, mipmap =True)
        
        self.img.bind(texture = self.on_tex_chagned)
        self.on_tex_chagned()

        self.beat_delay = .1
        self.run_interval = self.beat_delay * 4

    def on_tex_chagned(self, *largs):
        self.texture_background = self.img.texture

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

