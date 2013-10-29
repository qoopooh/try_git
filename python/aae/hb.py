
__all__ = ('Heartbeat',)

from kivy.uix.label import Label
from kivy.uix.image import Image
#from kivy.graphics import *
from kivy.properties import StringProperty, OptionProperty, \
         ObjectProperty, BooleanProperty, ListProperty

class Heartbeat(Label):

    state = OptionProperty('off', options=('dis', 'off', 'on'))
    allow_stretch = BooleanProperty(True)
    keep_ratio = BooleanProperty(False)
    border = ObjectProperty(None)
    texture_background = ObjectProperty(None)
    background_beat = StringProperty('atlas://images/hb/hb_on')
    background_down = StringProperty('atlas://images/hb/hb_off')
    background_image = StringProperty('atlas://images/hb/hb_dis')

    beat_delay = ObjectProperty(None)
    run_interval = ObjectProperty(None)
    beat = BooleanProperty(False)
    run = BooleanProperty(False)

    background_color = ListProperty([1, 1, 1, 1])
    border = ListProperty([16, 16, 16, 16])

    __event__ = ('on_on', 'on_off')

    def __init__(self, **kwargs):

        self.register_event_type('on_on')
        self.register_event_type('on_off')
        self.border = (16, 16, 16, 16)
        self.img = Image(source = self.background_image)
        
#self.img.bind(texture = self.on_tex_chagned)
#self.on_tex_chagned()

        self.beat_delay = .1
        self.run_interval = self.beat_delay * 4
        self.state = 'dis'
        self.background_texture = self.img.texture
        super(Heartbeat, self).__init__(**kwargs)

    #def on_tex_chagned(self, *largs):
        #self.texture_background = self.img.texture

    def on_background_image(self, *l):
        self.img.source = self.background_image

    def on_on(self):
        pass

    def on_off(self):
        pass


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

