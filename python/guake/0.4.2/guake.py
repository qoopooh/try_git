# -*- coding: utf-8; -*-
"""
Copyright (C) 2007,2008,2009,2010 Lincoln de Sousa <lincoln@minaslivre.org>
Copyright (C) 2007 Gabriel Falcão <gabrielteratos@gmail.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public
License along with this program; if not, write to the
Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.
"""
import pygtk
import gobject
pygtk.require('2.0')
gobject.threads_init()

import gtk
import vte
from pango import FontDescription
import pynotify
import gconf
import dbus

import os
import sys
import signal
from thread import start_new_thread
from time import sleep
import posix

import globalhotkeys
from simplegladeapp import SimpleGladeApp, bindtextdomain
from prefs import PrefsDialog, LKEY, GKEY
from dbusiface import DbusManager, DBUS_NAME, DBUS_PATH
from common import test_gconf, pixmapfile, gladefile, ShowableError, _
from guake_globals import NAME, VERSION, LOCALE_DIR, KEY, GCONF_PATH, \
    TERMINAL_MATCH_EXPRS, TERMINAL_MATCH_TAGS, \
    ALIGN_LEFT, ALIGN_RIGHT, ALIGN_CENTER

pynotify.init('Guake!')

GNOME_FONT_PATH = '/desktop/gnome/interface/monospace_font_name'

# Loading translation
bindtextdomain(NAME, LOCALE_DIR)


class PromptQuitDialog(gtk.MessageDialog):
    """Prompts the user whether to quit or not if there are procs running.
    """
    def __init__(self, parent, running_procs):
        super(PromptQuitDialog, self).__init__(
            parent,
            gtk.DIALOG_MODAL | gtk.DIALOG_DESTROY_WITH_PARENT,
            gtk.MESSAGE_QUESTION, gtk.BUTTONS_YES_NO)

        self.set_keep_above(True)
        self.set_markup(_('Do you really want to quit Guake!?'))
        if running_procs == 1:
            self.format_secondary_markup(
                _("<b>There is one process still running.</b>")
            )
        elif running_procs > 1:
            self.format_secondary_markup(
                _("<b>There are %d processes running.</b>" % running_procs)
            )


class AboutDialog(SimpleGladeApp):
    """The About Guake dialog class
    """
    def __init__(self):
        super(AboutDialog, self).__init__(gladefile('about.glade'),
                                          root='aboutdialog')
        dialog = self.get_widget('aboutdialog')

        # images
        ipath = pixmapfile('guake-notification.png')
        img = gtk.gdk.pixbuf_new_from_file(ipath)
        dialog.set_property('logo', img)

        dialog.set_name('Guake!')
        dialog.set_version(VERSION)


class GConfHandler(object):
    """Handles gconf changes, if any gconf variable is changed, a
    different method is called to handle this change.
    """
    def __init__(self, guake):
        """Constructor of GConfHandler, just add the guake dir to the
        gconf client and bind the keys to its handler methods.
        """
        self.guake = guake

        client = gconf.client_get_default()
        client.add_dir(GCONF_PATH, gconf.CLIENT_PRELOAD_RECURSIVE)

        notify_add = client.notify_add

        # these keys does not need to be watched.
        #notify_add(KEY('/general/default_shell'), self.shell_changed)
        #notify_add(KEY('/general/use_login_shell'), self.login_shell_toggled)
        #notify_add(KEY('/general/use_popup'), self.popup_toggled)
        #notify_add(KEY('/general/window_losefocus'), self.losefocus_toggled)

        notify_add(KEY('/general/show_resizer'), self.show_resizer_toggled)

        notify_add(KEY('/general/use_trayicon'), self.trayicon_toggled)
        notify_add(KEY('/general/window_ontop'), self.ontop_toggled)
        notify_add(KEY('/general/window_tabbar'), self.tabbar_toggled)
        notify_add(KEY('/general/window_height'), self.size_changed)

        notify_add(KEY('/general/use_scrollbar'), self.scrollbar_toggled)
        notify_add(KEY('/general/history_size'), self.history_size_changed)
        notify_add(KEY('/general/scroll_output'), self.keystroke_output)
        notify_add(KEY('/general/scroll_keystroke'), self.keystroke_toggled)

        notify_add(KEY('/general/use_default_font'), self.default_font_toggled)
        notify_add(KEY('/style/font/style'), self.fstyle_changed)
        notify_add(KEY('/style/font/color'), self.fcolor_changed)
        notify_add(KEY('/style/font/palette'), self.fpalette_changed)
        notify_add(KEY('/style/background/color'), self.bgcolor_changed)
        notify_add(KEY('/style/background/image'), self.bgimage_changed)
        notify_add(KEY('/style/background/transparency'),
                   self.bgtransparency_changed)

        notify_add(KEY('/general/compat_backspace'), self.backspace_changed)
        notify_add(KEY('/general/compat_delete'), self.delete_changed)

    def show_resizer_toggled(self, client, connection_id, entry, data):
        """If the gconf var show_resizer be changed, this method will
        be called and will show/hide the resizer.
        """
        if entry.value.get_bool():
            self.guake.resizer.show()
        else:
            self.guake.resizer.hide()

    def trayicon_toggled(self, client, connection_id, entry, data):
        """If the gconf var use_trayicon be changed, this method will
        be called and will show/hide the trayicon.
        """
        self.guake.tray_icon.set_visible(entry.value.get_bool())

    def ontop_toggled(self, client, connection_id, entry, data):
        """If the gconf var window_ontop be changed, this method will
        be called and will set the keep_above attribute in guake's
        main window.
        """
        self.guake.window.set_keep_above(entry.value.get_bool())

    def tabbar_toggled(self, client, connection_id, entry, data):
        """If the gconf var use_tabbar be changed, this method will be
        called and will show/hide the tabbar.
        """
        if entry.value.get_bool():
            self.guake.toolbar.show()
        else:
            self.guake.toolbar.hide()

    def alignment_changed(self, client, connection_id, entry, data):
        """If the gconf var window_halignment be changed, this method will
        be called and will call the move function in guake.
        """
        window_rect = self.guake.get_final_window_rect()
        self.guake.window.move(window_rect.x, window_rect.y)

    def size_changed(self, client, connection_id, entry, data):
        """If the gconf var window_height or window_width are changed,
        this method will be called and will call the resize function
        in guake.
        """
        window_rect = self.guake.get_final_window_rect()
        self.guake.window.move(window_rect.x, window_rect.y)
        self.guake.window.resize(window_rect.width, window_rect.height)

    def scrollbar_toggled(self, client, connection_id, entry, data):
        """If the gconf var use_scrollbar be changed, this method will
        be called and will show/hide scrollbars of all terminals open.
        """
        for i in self.guake.term_list:
            # There is an hbox in each tab of the main notebook and it
            # contains a Terminal and a Scrollbar. Since only have the
            # Terminal here, we're going to use this to get the
            # scrollbar and hide/show it.
            hbox = i.get_parent()
            terminal, scrollbar = hbox.get_children()
            if entry.value.get_bool():
                scrollbar.show()
            else:
                scrollbar.hide()

    def history_size_changed(self, client, connection_id, entry, data):
        """If the gconf var history_size be changed, this method will
        be called and will set the scrollback_lines property of all
        terminals open.
        """
        for i in self.guake.term_list:
            i.set_scrollback_lines(entry.value.get_int())

    def keystroke_output(self, client, connection_id, entry, data):
        """If the gconf var scroll_output be changed, this method will
        be called and will set the scroll_on_output in all terminals
        open.
        """
        for i in self.guake.term_list:
            i.set_scroll_on_output(entry.value.get_bool())

    def keystroke_toggled(self, client, connection_id, entry, data):
        """If the gconf var scroll_keystroke be changed, this method
        will be called and will set the scroll_on_keystroke in all
        terminals open.
        """
        for i in self.guake.term_list:
            i.set_scroll_on_keystroke(entry.value.get_bool())

    def default_font_toggled(self, client, connection_id, entry, data):
        """If the gconf var use_default_font be changed, this method
        will be called and will change the font style to the gnome
        default or to the choosen font in style/font/style in all
        terminals open.
        """
        if entry.value.get_bool():
            key = GNOME_FONT_PATH
        else:
            key = KEY('/style/font/style')

        font = FontDescription(client.get_string(key))
        for i in self.guake.term_list:
            i.set_font(font)

    def fstyle_changed(self, client, connection_id, entry, data):
        """If the gconf var style/font/style be changed, this method
        will be called and will change the font style in all terminals
        open.
        """
        font = FontDescription(entry.value.get_string())
        for i in self.guake.term_list:
            i.set_font(font)

    def fcolor_changed(self, client, connection_id, entry, data):
        """If the gconf var style/font/color be changed, this method
        will be called and will change the font color in all terminals
        open.
        """
        fgcolor = gtk.gdk.color_parse(entry.value.get_string())
        for i in self.guake.term_list:
            i.set_color_dim(fgcolor)
            i.set_color_foreground(fgcolor)
            i.set_color_bold(fgcolor)

    def fpalette_changed(self, client, connection_id, entry, data):
        """If the gconf var style/font/palette be changed, this method
        will be called and will change the color scheme in all terminals
        open.
        """
        fgcolor = gtk.gdk.color_parse(
            client.get_string(KEY('/style/font/color')))
        bgcolor = gtk.gdk.color_parse(
            client.get_string(KEY('/style/background/color')))
        palette = [gtk.gdk.color_parse(color) for color in 
            entry.value.get_string().split(':')]
        for i in self.guake.term_list:
            i.set_colors(fgcolor, bgcolor, palette)

    def bgcolor_changed(self, client, connection_id, entry, data):
        """If the gconf var style/background/color be changed, this
        method will be called and will change the background color in
        all terminals open.
        """
        bgcolor = gtk.gdk.color_parse(entry.value.get_string())
        for i in self.guake.term_list:
            i.set_color_background(bgcolor)
            i.set_background_tint_color(bgcolor)

    def bgimage_changed(self, client, connection_id, entry, data):
        """If the gconf var style/background/image be changed, this
        method will be called and will change the background image and
        will set the transparent flag to false if an image is set in
        all terminals open.
        """
        image = entry.value.get_string()
        for i in self.guake.term_list:
            if image and os.path.exists(image):
                i.set_background_image_file(image)
                i.set_background_transparent(False)
            else:
                """We need to clear the image if it's not set but there is
                a bug in vte python bidnings which doesn't allow None to be
                passed to set_background_image (C GTK function expects NULL).
                The user will need to restart Guake after clearing the image.
                i.set_background_image(None)
                """
                if self.guake.has_argb:
                    i.set_background_transparent(False)
                else:
                    i.set_background_transparent(True)

    def bgtransparency_changed(self, client, connection_id, entry, data):
        """If the gconf var style/background/transparency be changed, this
        method will be called and will set the saturation and transparency
        properties in all terminals open.
        """
        transparency = entry.value.get_int()
        for i in self.guake.term_list:
            i.set_background_saturation(transparency / 100.0)
            if self.guake.has_argb:
                i.set_opacity(int((100 - transparency) / 100.0 * 65535))

    def backspace_changed(self, client, connection_id, entry, data):
        """If the gconf var compat_backspace be changed, this method
        will be called and will change the binding configuration in
        all terminals open.
        """
        for i in self.guake.term_list:
            i.set_backspace_binding(entry.value.get_string())

    def delete_changed(self, client, connection_id, entry, data):
        """If the gconf var compat_delete be changed, this method
        will be called and will change the binding configuration in
        all terminals open.
        """
        for i in self.guake.term_list:
            i.set_delete_binding(entry.value.get_string())


class GConfKeyHandler(object):
    """Handles changes in keyboard shortcuts.
    """
    def __init__(self, guake):
        """Constructor of Keyboard, only receives the guake instance
        to be used in internal methods.
        """
        self.guake = guake
        self.accel_group = None # see reload_accelerators
        self.client = gconf.client_get_default()

        notify_add = self.client.notify_add
        notify_add(GKEY('show_hide'), self.reload_globals)

        keys = ['toggle_fullscreen', 'new_tab', 'close_tab', 'rename_tab',
                'previous_tab', 'next_tab', 'clipboard_copy', 'clipboard_paste',
                'quit', 'sdadm',
                ]
        for key in keys:
            notify_add(LKEY(key), self.reload_accelerators)
            self.client.notify(LKEY(key))

    def reload_globals(self, client, connection_id, entry, data):
        """Unbind all global hotkeys and rebind the show_hide
        method. If more global hotkeys should be added, just connect
        the gconf key to the watch system and add.
        """
        self.guake.hotkeys.unbind_all()
        key = entry.get_value().get_string()
        if not self.guake.hotkeys.bind(key, self.guake.show_hide):
            raise ShowableError(_('key binding error'),
                                _('Unable to bind global <b>%s</b> key') % key,
                                -1)

    def reload_accelerators(self, *args):
        """Reassign an accel_group to guake main window and guake
        context menu and calls the load_accelerators method.
        """
        if self.accel_group:
            self.guake.window.remove_accel_group(self.accel_group)
        self.accel_group = gtk.AccelGroup()
        self.guake.window.add_accel_group(self.accel_group)
        self.guake.context_menu.set_accel_group(self.accel_group)
        self.load_accelerators()

    def load_accelerators(self):
        """Reads all gconf paths under /apps/guake/keybindings/local
        and adds to the main accel_group.
        """
        gets = lambda x:self.client.get_string(LKEY(x))
        key, mask = gtk.accelerator_parse(gets('quit'))
        if key > 0:
            self.accel_group.connect_group(key, mask, gtk.ACCEL_VISIBLE,
                                           self.guake.accel_quit)

        key, mask = gtk.accelerator_parse(gets('new_tab'))
        if key > 0:
            self.accel_group.connect_group(key, mask, gtk.ACCEL_VISIBLE,
                                           self.guake.accel_add)

        key, mask = gtk.accelerator_parse(gets('close_tab'))
        if key > 0:
            self.accel_group.connect_group(
                key, mask, gtk.ACCEL_VISIBLE,
                self.guake.close_tab)

        key, mask = gtk.accelerator_parse(gets('previous_tab'))
        if key > 0:
            self.accel_group.connect_group(key, mask, gtk.ACCEL_VISIBLE,
                                           self.guake.accel_prev)

        key, mask = gtk.accelerator_parse(gets('next_tab'))
        if key > 0:
            self.accel_group.connect_group(key, mask, gtk.ACCEL_VISIBLE,
                                           self.guake.accel_next)

        key, mask = gtk.accelerator_parse(gets('rename_tab'))
        if key > 0:
            self.accel_group.connect_group(key, mask, gtk.ACCEL_VISIBLE,
                                           self.guake.accel_rename)

        key, mask = gtk.accelerator_parse(gets('clipboard_copy'))
        if key > 0:
            self.accel_group.connect_group(key, mask, gtk.ACCEL_VISIBLE,
                                           self.guake.accel_copy_clipboard)

        key, mask = gtk.accelerator_parse(gets('clipboard_paste'))
        if key > 0:
            self.accel_group.connect_group(key, mask, gtk.ACCEL_VISIBLE,
                                           self.guake.accel_paste_clipboard)

        key, mask = gtk.accelerator_parse(gets('toggle_fullscreen'))
        if key > 0:
            self.accel_group.connect_group(key, mask, gtk.ACCEL_VISIBLE,
                                           self.guake.accel_toggle_fullscreen)

        key, mask = gtk.accelerator_parse(gets('sdadm'))
        if key > 0:
            self.accel_group.connect_group(key, mask, gtk.ACCEL_VISIBLE,
                                           self.guake.accel_sdadm)

class GuakeTerminal(vte.Terminal):
    """Just a vte.Terminal with some properties already set.
    """
    def __init__(self):
        super(GuakeTerminal, self).__init__()
        self.configure_terminal()
        self.add_matches()
        self.connect('button-press-event', self.button_press)
        self.matched_value = ''

    def configure_terminal(self):
        """Sets all customized properties on the terminal
        """
        client = gconf.client_get_default()
        word_chars = client.get_string(KEY('/general/word_chars'))
        self.set_word_chars(word_chars)
        self.set_audible_bell(False)
        self.set_visible_bell(False)
        self.set_sensitive(True)
        self.set_flags(gtk.CAN_DEFAULT)
        self.set_flags(gtk.CAN_FOCUS)

    def add_matches(self):
        """Adds all regular expressions declared in
        guake_globals.TERMINAL_MATCH_EXPRS to the terminal to make vte
        highlight text that matches them.
        """
        for expr in TERMINAL_MATCH_EXPRS:
            tag = self.match_add(expr)
            self.match_set_cursor_type(tag, gtk.gdk.HAND2)

    def button_press(self, terminal, event):
        """Handles the button press event in the terminal widget. If
        any match string is caught, another aplication is open to
        handle the matched resource uri.
        """
        self.matched_value = ''
        matched_string = self.match_check(
            int(event.x / self.get_char_width()),
            int(event.y / self.get_char_height()))

        if event.button == 1 \
                and event.get_state() & gtk.gdk.CONTROL_MASK \
                and matched_string:
            value, tag = matched_string

            if TERMINAL_MATCH_TAGS[tag] == 'schema':
                # value here should not be changed, it is right and
                # ready to be used.
                pass
            elif TERMINAL_MATCH_TAGS[tag] == 'http':
                value = 'http://%s' % value
            elif TERMINAL_MATCH_TAGS[tag] == 'email':
                value = 'mailto:%s' % value

            gtk.show_uri(self.window.get_screen(), value,
                         gtk.gdk.x11_get_server_time(self.window))
        elif event.button == 3 and matched_string:
            self.matched_value = matched_string[0]

class GuakeTerminalBox(gtk.HBox):
    """A box to group the terminal and a scrollbar.
    """
    def __init__(self):
        super(GuakeTerminalBox, self).__init__()
        self.terminal = GuakeTerminal()
        self.add_terminal()
        self.add_scroll_bar()

    def add_terminal(self):
        """Packs the terminal widget.
        """
        self.pack_start(self.terminal, True, True)
        self.terminal.show()

    def add_scroll_bar(self):
        """Packs the scrollbar.
        """
        adj = self.terminal.get_adjustment()
        scroll = gtk.VScrollbar(adj)
        scroll.set_no_show_all(True)
        self.pack_start(scroll, False, False)


class Guake(SimpleGladeApp):
    """Guake main class. Handles specialy the main window.
    """
    def __init__(self):
        super(Guake, self).__init__(gladefile('guake.glade'))
        self.client = gconf.client_get_default()

        # setting global hotkey and showing a pretty notification =)
        globalhotkeys.init()

        # trayicon!
        img = pixmapfile('guake-tray.png')
        self.tray_icon = gtk.status_icon_new_from_file(img)
        self.tray_icon.set_tooltip(_('Guake Terminal'))
        self.tray_icon.connect('popup-menu', self.show_menu)
        self.tray_icon.connect('activate', self.show_hide)

        # adding images from a different path.
        ipath = pixmapfile('guake.png')
        self.get_widget('image1').set_from_file(ipath)
        ipath = pixmapfile('add_tab.png')
        self.get_widget('image2').set_from_file(ipath)

        # important widgets
        self.window = self.get_widget('window-root')
        self.notebook = self.get_widget('notebook-teminals')
        self.tabs = self.get_widget('hbox-tabs')
        self.toolbar = self.get_widget('toolbar')
        self.mainframe = self.get_widget('mainframe')
        self.resizer = self.get_widget('resizer')

        # check and set ARGB for real transparency
        screen = self.window.get_screen()
        colormap = screen.get_rgba_colormap()
        if colormap != None and screen.is_composited():
            self.window.set_colormap(colormap)
            self.has_argb = True
        else:
            self.has_argb = False

        # List of vte.Terminal widgets, it will be useful when needed
        # to get a widget by the current page in self.notebook
        self.term_list = []

        # This is the pid of shells forked by each terminal. Will be
        # used to kill the process when closing a tab
        self.pid_list = []

        # It's intended to know which tab was selected to
        # close/rename. This attribute will be set in
        # self.show_tab_menu
        self.selected_tab = None

        # holds the number of created tabs. This counter will not be
        # reset to avoid problems of repeated tab names.
        self.tab_counter = 0

        # holds fullscreen status
        self.fullscreen = False

        # holds the timestamp of the losefocus event
        self.losefocus_time = 0

        # double click stuff
        def double_click(hbox, event):
            """Handles double clicks on tabs area and when receive
            one, calls add_tab.
            """
            if event.button == 1 and event.type == gtk.gdk._2BUTTON_PRESS:
                self.add_tab()
        evtbox = self.get_widget('event-tabs')
        evtbox.connect('button-press-event', double_click)

        # Flag to prevent guake hide when window_losefocus is true and
        # user tries to use the context menu.
        self.showing_context_menu = False
        def hide_context_menu(menu):
            """Turn context menu flag off to make sure it is not being
            shown.
            """
            self.showing_context_menu = False
        self.get_widget('context-menu').connect('hide', hide_context_menu)
        self.get_widget('tab-menu').connect('hide', hide_context_menu)
        self.window.connect('focus-out-event', self.on_window_losefocus)

        # Handling the delete-event of the main window to avoid
        # problems when closing it.
        def destroy(*args):
            self.hide()
            return True
        self.window.connect('delete-event', destroy)

        # Flag to completly disable losefocus hiding
        self.disable_losefocus_hiding = False

        # this line is important to resize the main window and make it
        # smaller.
        self.window.set_geometry_hints(min_width=1, min_height=1)

        # resizer stuff
        self.resizer.connect('motion-notify-event', self.on_resizer_drag)

        # adding the first tab on guake
        self.add_tab()

        # loading and setting up configuration stuff
        GConfHandler(self)
        GConfKeyHandler(self)
        self.hotkeys = globalhotkeys.GlobalHotkey()
        self.load_config()

        key = self.client.get_string(GKEY('show_hide'))
        keyval, mask = gtk.accelerator_parse(key)
        label = gtk.accelerator_get_label(keyval, mask)
        filename = pixmapfile('guake-notification.png')

        if not self.hotkeys.bind(key, self.show_hide):
            notification = pynotify.Notification(
                _('Guake!'),
                _('A problem happened when binding <b>%s</b> key.\n'
                  'Please use Guake Preferences dialog to choose another '
                  'key (The trayicon was enabled)') % label, filename)
            self.client.set_bool(KEY('/general/use_trayicon'), True)
            notification.show()

        elif self.client.get_bool(KEY('/general/use_popup')):
            # Pop-up that shows that guake is working properly (if not
            # unset in the preferences windows)
            notification = pynotify.Notification(
                _('Guake!'),
                _('Guake is now running,\n'
                  'press <b>%s</b> to use it.') % label, filename)
            notification.show()

    def execute_command(self, command, tab=None):
        """Execute the `command' in the `tab'. If tab is None, the
        command will be executed in the currently selected
        tab. Command should end with '\n', otherwise it will be
        appended to the string.
        """
        if not self.term_list:
            self.add_tab()

        if command[-1] != '\n':
            command += '\n'

        index = self.notebook.get_current_page()
        self.term_list[tab or index].feed_child(command)

    def on_resizer_drag(self, widget, event):
        """Method that handles the resize drag. It does not actuall
        moves the main window. It just set the new window size in
        gconf.
        """
        (x, y), mod = event.device.get_state(widget.window)
        if not 'GDK_BUTTON1_MASK' in mod.value_names:
            return

        max_height = self.window.get_screen().get_height()
        percent = y / (max_height / 100)

        if percent < 1:
            percent = 1

        self.client.set_int(KEY('/general/window_height'), int(percent))

    def on_window_losefocus(self, window, event):
        """Hides terminal main window when it loses the focus and if
        the window_losefocus gconf variable is True.
        """
        if self.disable_losefocus_hiding or self.showing_context_menu:
            return

        value = self.client.get_bool(KEY('/general/window_losefocus'))
        visible = window.get_property('visible')
        if value and visible:
            self.losefocus_time = \
                gtk.gdk.x11_get_server_time(self.window.window)
            self.hide()

    def show_menu(self, status_icon, button, activate_time):
        """Show the tray icon menu.
        """
        menu = self.get_widget('tray-menu')
        menu.popup(None, None, gtk.status_icon_position_menu,
                   button, activate_time, status_icon)

    def show_context_menu(self, terminal, event):
        """Show the context menu, only with a right click on a vte
        Terminal.
        """
        if event.button != 3:
            return False

        self.showing_context_menu = True

        guake_clipboard = gtk.clipboard_get()
        if not guake_clipboard.wait_is_text_available():
            self.get_widget('context_paste').set_sensitive(False)
        else:
            self.get_widget('context_paste').set_sensitive(True)
        context_menu = self.get_widget('context-menu')
        context_menu.popup(None, None, None, 3, gtk.get_current_event_time())
        return True

    def show_rename_dialog(self, target, event):
        """On double-click over a tab, show the rename dialog.
        """
        if event.button == 1:
            if event.type == gtk.gdk._2BUTTON_PRESS:
                self.accel_rename()
                self.set_terminal_focus()
                self.selected_tab.pressed()
                return

    def show_tab_menu(self, target, event):
        """Shows the tab menu with a right click. After that, the
        focus come back to the terminal.
        """
        if event.button == 3:
            self.showing_context_menu = True
            self.selected_tab = target
            menu = self.get_widget('tab-menu')
            menu.popup(None, None, None, 3, event.get_time())
        self.set_terminal_focus()

    def show_about(self, *args):
        """Hides the main window and creates an instance of the About
        Dialog.
        """
        self.hide()
        AboutDialog()

    def show_prefs(self, *args):
        """Hides the main window and creates an instance of the
        Preferences window.
        """
        self.hide()
        PrefsDialog().show()

    def show_hide(self, *args):
        """Toggles the main window visibility
        """
        event_time = self.hotkeys.get_current_event_time()

        if self.losefocus_time and \
                self.losefocus_time >= event_time and \
                (self.losefocus_time - event_time) < 10:
            self.losefocus_time = 0
            return

        if not self.window.get_property('visible'):
            self.show()
            self.set_terminal_focus()
        else:
            self.hide()
 
    def show(self):
        """Shows the main window and grabs the focus on it.
        """
        # setting window in all desktops
        self.get_widget('window-root').stick()

        # add tab must be called before window.show to avoid a
        # blank screen before adding the tab.
        if not self.term_list:
            self.add_tab()

        window_rect = self.get_final_window_rect()
        self.window.resize(window_rect.width, window_rect.height)
        self.window.show_all()
        self.window.move(window_rect.x, window_rect.y)

        try:
            # does it work in other gtk backends
            time = gtk.gdk.x11_get_server_time(self.window.window)
        except AttributeError:
            time = 0

        self.window.window.show()
        self.window.window.focus(time)

        # This is here because vte color configuration works only
        # after the widget is shown.
        self.client.notify(KEY('/style/font/color'))
        self.client.notify(KEY('/style/background/color'))

    def hide(self):
        """Hides the main window of the terminal and sets the visible
        flag to False.
        """
        self.window.hide() # Don't use hide_all here!

    def get_final_window_rect(self):
        """Gets the final size of the main window of guake. The height
        is the window_height property, width is window_width and the
        horizontal alignment is given by window_alignment.
        """
        screen = self.window.get_screen()
        height = self.client.get_int(KEY('/general/window_height'))
        width = 100
        halignment = self.client.get_int(KEY('/general/window_halignment'))

        # get the rectangle just from the first/default monitor in the
        # future we might create a field to select which monitor you
        # wanna use
        monitor = 1 # Berm: use the right monitor 
        window_rect = screen.get_monitor_geometry(monitor)
        if window_rect.width == 0: # Berm
            monitor = 0
            window_rect = screen.get_monitor_geometry(monitor)

        total_width = window_rect.width
        window_rect.height = window_rect.height * height / 100
        window_rect.width = window_rect.width * width / 100

        if width < total_width:
            if halignment == ALIGN_CENTER:
                window_rect.x = (total_width - window_rect.width) / 2
                if monitor == 1: # Berm
                    right_window_rect = screen.get_monitor_geometry(0)
                    window_rect.x += right_window_rect.width 
            elif halignment == ALIGN_LEFT:
                window_rect.x = 0
            elif halignment == ALIGN_RIGHT:
                window_rect.x = total_width - window_rect.width
        window_rect.y = 0
        return window_rect

    def get_running_fg_processes(self):
        """Get the number processes for each terminal/tab. The code is taken
        from gnome-terminal.
        """
        total_procs = 0
        term_idx = 0
        for terminal in self.term_list:
            fdpty = terminal.get_pty()
            term_pid = self.pid_list[term_idx]
            fgpid = posix.tcgetpgrp(fdpty)
            if not (fgpid == -1 or fgpid == term_pid):
            	total_procs += 1
            term_idx += 1
        return total_procs

    # -- configuration --

    def load_config(self):
        """"Just a proxy for all the configuration stuff.
        """
        self.client.notify(KEY('/general/use_trayicon'))
        self.client.notify(KEY('/general/prompt_on_quit'))
        self.client.notify(KEY('/general/window_tabbar'))
        self.client.notify(KEY('/general/window_ontop'))
        self.client.notify(KEY('/general/window_height'))
        self.client.notify(KEY('/general/use_scrollbar'))
        self.client.notify(KEY('/general/history_size'))
        self.client.notify(KEY('/general/show_resizer'))
        self.client.notify(KEY('/style/font/style'))
        self.client.notify(KEY('/style/font/color'))
        self.client.notify(KEY('/style/font/palette'))
        self.client.notify(KEY('/style/background/color'))
        self.client.notify(KEY('/style/background/image'))
        self.client.notify(KEY('/style/background/transparency'))
        self.client.notify(KEY('/general/use_default_font'))
        self.client.notify(KEY('/general/compat_backspace'))
        self.client.notify(KEY('/general/compat_delete'))

    def accel_quit(self, *args):
        """Callback to prompt the user whether to quit Guake or not.
        """
        if self.client.get_bool(KEY('/general/prompt_on_quit')):
            procs = self.get_running_fg_processes()
            if procs >= 1:
                dialog = PromptQuitDialog(self.window, procs)
                response = dialog.run() == gtk.RESPONSE_YES
                dialog.destroy()
                if response:
                    gtk.main_quit()
            else:
                gtk.main_quit()
        else:
            gtk.main_quit()

    def accel_add(self, *args):
        """Callback to add a new tab. Called by the accel key.
        """
        self.add_tab()
        return True

    def accel_prev(self, *args):
        """Callback to go to the previous tab. Called by the accel key.
        """
        if self.notebook.get_current_page() == 0:
            self.notebook.set_current_page(self.notebook.get_n_pages()-1)
        else:
            self.notebook.prev_page()
        return True

    def accel_next(self, *args):
        """Callback to go to the next tab. Called by the accel key.
        """
        if self.notebook.get_current_page()+1 == self.notebook.get_n_pages():
            self.notebook.set_current_page(0)
        else:
            self.notebook.next_page()
        return True

    def accel_rename(self, *args):
        """Callback to show the rename tab dialog. Called by the accel
        key.
        """
        pagepos = self.notebook.get_current_page()
        self.selected_tab = self.tabs.get_children()[pagepos]
        self.on_rename_activate()
        return True

    def accel_copy_clipboard(self, *args):
        """Callback to copy text in the shown terminal. Called by the
        accel key.
        """
        current_term = self.term_list[self.notebook.get_current_page()]

        if current_term.get_has_selection():
            current_term.copy_clipboard()
        elif current_term.matched_value:
            guake_clipboard = gtk.clipboard_get()
            guake_clipboard.set_text(current_term.matched_value)

        return True

    def accel_paste_clipboard(self, *args):
        """Callback to paste text in the shown terminal. Called by the
        accel key.
        """
        pos = self.notebook.get_current_page()
        self.term_list[pos].paste_clipboard()
        return True

    def accel_toggle_fullscreen(self, *args):
        """Callback toggle the fullscreen status of the main
        window. It uses the toolbar_visible_in_fullscreen variable
        from gconf to decide if the tabbar will or not be
        shown. Called by the accel key.
        """
        val = self.client.get_bool(KEY('general/toolbar_visible_in_fullscreen'))

        if not self.fullscreen:
            self.window.fullscreen()
            self.fullscreen = True

            # The resizer widget really don't need to be shown in
            # fullscreen mode, but tabbar will only be shown if a
            # hidden gconf key is false.
            self.resizer.hide()
            if not val:
                self.toolbar.hide()
        else:
            window_rect = self.get_final_window_rect()
            self.window.unfullscreen()
            self.window.resize(window_rect.width, window_rect.height)
            self.window.move(window_rect.x, window_rect.y)
            self.fullscreen = False

            # making sure that tabbar and resizer will come back to
            # their default state.
            self.client.notify(KEY('/general/window_tabbar'))
            self.client.notify(KEY('/general/show_resizer'))

        return True

    def accel_sdadm(self, *args):
        """Callback to add a new tab. Called by the accel key.
        """
        self.add_tab()
        self.execute_command('ssh sdadm')
        return True

    # -- callbacks --

    def on_terminal_exited(self, term, widget):
        """When a terminal is closed, shell process should be killed,
        this is the method that does that, or, at least calls
        `delete_tab' method to do the work.
        """
        self.delete_tab(self.notebook.page_num(widget), kill=False)

    def on_rename_activate(self, *args):
        """Shows a dialog to rename the current tab.
        """
        entry = gtk.Entry()
        entry.set_text(self.selected_tab.get_label())
        entry.set_property('can-default', True)
        entry.show()

        vbox = gtk.VBox()
        vbox.set_border_width(6)
        vbox.show()

        dialog = gtk.Dialog(_("Rename tab"),
                            self.window,
                            gtk.DIALOG_MODAL | gtk.DIALOG_DESTROY_WITH_PARENT,
                            (gtk.STOCK_CANCEL, gtk.RESPONSE_REJECT,
                             gtk.STOCK_OK, gtk.RESPONSE_ACCEPT))

        dialog.set_size_request(300, -1)
        dialog.vbox.pack_start(vbox)
        dialog.set_border_width(4)
        dialog.set_has_separator(False)
        dialog.set_default_response(gtk.RESPONSE_ACCEPT)
        dialog.add_action_widget(entry, gtk.RESPONSE_ACCEPT)
        entry.reparent(vbox)

        self.disable_losefocus_hiding = True
        response = dialog.run()
        self.disable_losefocus_hiding = False

        if response == gtk.RESPONSE_ACCEPT:
            self.selected_tab.set_label(entry.get_text())

        dialog.destroy()
        self.set_terminal_focus()

    def on_close_activate(self, *args):
        """Tab context menu close handler
        """
        tabs = self.tabs.get_children()
        pagepos = tabs.index(self.selected_tab)
        self.delete_tab(pagepos)

    # -- tab related functions --

    def close_tab(self, *args):
        """Closes the current tab.
        """
        pagepos = self.notebook.get_current_page()
        self.delete_tab(pagepos)

    def rename_current_tab(self, new_text):
        """Sets the `self.selected_tab' var with the selected radio
        button and change its label to `new_text'.
        """
        pagepos = self.notebook.get_current_page()
        self.selected_tab = self.tabs.get_children()[pagepos]
        self.selected_tab.set_label(new_text)

    def get_current_dir(self):
        """Gets the working directory of the current tab to create a
        new one in the same dir.
        """
        active_pagepos = self.notebook.get_current_page()
        directory = os.path.expanduser('~')
        if active_pagepos >= 0:
            cwd = os.readlink("/proc/%d/cwd" % self.pid_list[active_pagepos])
            if os.path.exists(cwd):
                directory = cwd
        return directory

    def get_fork_params(self, default_params=None):
        """Return all parameters to be passed to the fork_command
        method of a vte terminal. Params returned can be expanded by
        the `params' parameter that receive a dictionary.
        """
        # use dictionary to pass named params to work around command
        # parameter in fork_command not accepting None as argument.
        # When we pass None as command, vte starts the default user
        # shell.
        params = {}

        shell = self.client.get_string(KEY('/general/default_shell'))
        if shell and os.path.exists(shell):
            params['command'] = shell

        login_shell = self.client.get_bool(KEY('/general/use_login_shell'))
        if login_shell:
            params['argv'] = ['-']

        if self.client.get_bool(KEY('/general/open_tab_cwd')):
            params['directory'] = self.get_current_dir()
        params['loglastlog'] = login_shell

        # Leting caller change/add values to fork params.
        if default_params:
            params.update(default_params)

        # Environment variables are not actually parameters but they
        # need to be set before calling terminal.fork_command()
        # method. So I found this place good to do it.
        self.update_proxy_vars()
        return params

    def update_proxy_vars(self):
        """This method updates http{s,}_proxy environment variables
        with values found in gconf.
        """
        proxy = '/system/http_proxy/'
        if self.client.get_bool(proxy + 'use_http_proxy'):
            host = self.client.get_string(proxy + 'host')
            port = self.client.get_int(proxy + 'port')
            if self.client.get_bool(proxy + 'use_same_proxy'):
                ssl_host = host
                ssl_port = port
            else:
                ssl_host = self.client.get_string('/system/proxy/secure_host')
                ssl_port = self.client.get_int('/system/proxy/secure_port')

            if self.client.get_bool(proxy + 'use_authentication'):
                auth_user = self.client.get_string(
                    proxy + 'authentication_user')
                auth_pass = self.client.get_string(
                    proxy + 'authentication_password')
                os.environ['http_proxy'] = 'http://%s:%s@%s:%d' % (
                    auth_user, auth_pass, host, port)
                os.environ['https_proxy'] = 'http://%s:%s@%s:%d' % (
                    auth_user, auth_pass, ssl_host, ssl_port)
            else:
                os.environ['http_proxy'] = 'http://%s:%d' % (host, port)
                os.environ['https_proxy'] = 'http://%s:%d' % (
                    ssl_host, ssl_port)

    def add_tab(self, directory=None):
        """Adds a new tab to the terminal notebook.
        """
        box = GuakeTerminalBox()
        box.terminal.grab_focus()
        box.terminal.connect('button-press-event', self.show_context_menu)
        box.terminal.connect('child-exited', self.on_terminal_exited, box)
        box.show()

        self.term_list.append(box.terminal)

        # We can choose the directory to vte launch. It is important
        # to be used by dbus interface. I'm testing if directory is a
        # string because when binded to a signal, the first param can
        # be a button not a directory.
        default_params = {}
        if isinstance(directory, basestring):
            default_params['directory'] = directory

        final_params = self.get_fork_params(default_params)
        pid = box.terminal.fork_command(**final_params)
        self.pid_list.append(pid)

        # Adding a new radio button to the tabbar
        label = _('Terminal %s') % self.tab_counter
        tabs = self.tabs.get_children()
        parent = tabs and tabs[0] or None

        bnt = gtk.RadioButton(group=parent, label=label)
        bnt.set_property('can-focus', False)
        bnt.set_property('draw-indicator', False)
        bnt.connect('button-press-event', self.show_tab_menu)
        bnt.connect('button-press-event', self.show_rename_dialog)
        bnt.connect('clicked',
                    lambda *x: self.notebook.set_current_page(
                        self.notebook.page_num(box)
                    ))
        bnt.show()

        self.tabs.pack_start(bnt, expand=False, padding=1)
        self.tab_counter += 1

        self.notebook.append_page(box, None)
        self.notebook.set_current_page(self.notebook.page_num(box))
        self.load_config()

    def delete_tab(self, pagepos, kill=True):
        """This function will destroy the notebook page, terminal and
        tab widgets and will call the function to kill interpreter
        forked by vte.
        """
        self.tabs.get_children()[pagepos].destroy()
        self.notebook.remove_page(pagepos)
        self.term_list.pop(pagepos).destroy()
        pid = self.pid_list.pop(pagepos)

        if kill:
            start_new_thread(self.delete_shell, (pid,))

        if not self.term_list:
            self.hide()
            # avoiding the delay on next Guake show request
            self.add_tab()

    def delete_shell(self, pid):
        """This function will kill the shell on a tab, trying to send
        a sigterm and if it doesn't work, a sigkill. Between these two
        signals, we have a timeout of 3 seconds, so is recommended to
        call this in another thread. This doesn't change any thing in
        UI, so you can use python's start_new_thread.
        """
        os.kill(pid, signal.SIGTERM)
        num_tries = 30

        while num_tries > 0:
            try:
                # Try to wait for the pid to be closed. If it does not
                # exist anymore, an OSError is raised and we can
                # safely ignore it.
                if os.waitpid(pid, os.WNOHANG)[0] != 0:
                    break
            except OSError:
                break
            sleep(0.1)
            num_tries -= 1

        if num_tries == 0:
            try:
                os.kill(pid, signal.SIGKILL)
                os.waitpid(pid, 0)
            except OSError:
                # if this part of code was reached, means that SIGTERM
                # did the work and SIGKILL wasnt needed.
                pass

    def set_terminal_focus(self):
        """Grabs the focus on the current tab.
        """
        page = self.notebook.get_current_page()
        self.term_list[page].grab_focus()

    def select_current_tab(self, notebook, user_data, page):
        """When current self.notebook page is changed, the tab bar
        made with radio buttons must be updated and this method does
        this work.
        """
        self.tabs.get_children()[page].set_active(True)

    def select_tab(self, tab_index):
        """Select an already added tab by its index.
        """
        try:
            self.tabs.get_children()[tab_index].set_active(True)
        except IndexError:
            pass

    def get_selected_tab(self):
        """return the selected tab index, it also set the
        self.selected_tab var.
        """
        pagepos = self.notebook.get_current_page()
        self.selected_tab = self.tabs.get_children()[pagepos]
        return pagepos

def main():
    """Parses the command line parameters and decide if dbus methods
    should be called or not. If there is already a guake instance
    running it will be used and a True value will be returned,
    otherwise, false will be returned.
    """
    from optparse import OptionParser
    parser = OptionParser()
    parser.add_option('-t', '--toggle-visibility', dest='show_hide',
            action='store_true', default=False,
            help=_('Toggles the visibility of the terminal window'))

    parser.add_option('-p', '--preferences', dest='show_preferences',
            action='store_true', default=False,
            help=_('Shows Guake preference window'))

    parser.add_option('-a', '--about', dest='show_about',
            action='store_true', default=False,
            help=_('Shows Guake\'s about info'))

    parser.add_option('-n', '--new-tab', dest='new_tab',
            action='store', default='',
            help=_('Add a new tab'))

    parser.add_option('-s', '--select-tab', dest='select_tab',
            action='store', default='',
            help=_('Select a tab'))

    parser.add_option('-g', '--selected-tab', dest='selected_tab',
            action='store_true', default=False,
            help=_('Return the selectd tab index.'))

    parser.add_option('-e', '--execute-command', dest='command',
            action='store', default='',
            help=_('Execute an arbitrary command in the selected tab.'))

    parser.add_option('-r', '--rename-tab', dest='rename_tab',
            action='store', default='',
            help=_('Rename the selected tab.'))

    parser.add_option('-q', '--quit', dest='quit',
            action='store_true', default=False,
            help=_('Says to Guake go away =('))

    parser.add_option('-d', '--sd', dest='sdadm',
            action='store_true', default=False,
            help=_('Add new ssh sdadm'))

    options = parser.parse_args()[0]

    # Trying to get an already running instance of guake. If it is not
    # possible, lets create a new instance. This function will return
    # a boolean value depending on this decision.
    try:
        bus = dbus.SessionBus()
        remote_object = bus.get_object(DBUS_NAME, DBUS_PATH)
        already_running = True
    except dbus.DBusException:
        instance = Guake()
        remote_object = DbusManager(instance)
        already_running = False

    called_with_param = False

    if options.show_hide:
        remote_object.show_hide()
        called_with_param = True

    if options.show_preferences:
        remote_object.show_prefs()
        called_with_param = True

    if options.new_tab:
        remote_object.add_tab(options.new_tab)
        called_with_param = True

    if options.select_tab:
        selected = int(options.select_tab)
        remote_object.select_tab(selected)
        called_with_param = True

    if options.selected_tab:
        selected = remote_object.get_selected_tab()
        sys.stdout.write('%d\n' % selected)
        called_with_param = True

    if options.command:
        remote_object.execute_command(options.command)
        called_with_param = True

    if options.rename_tab:
        remote_object.rename_current_tab(options.rename_tab)
        called_with_param = True

    if options.show_about:
        remote_object.show_about()
        called_with_param = True

    if options.quit:
        remote_object.quit()
        called_with_param = True

    if options.sdadm:
        remote_object.add_tab(options.new_tab)
        remote_object.execute_command('ssh sdadm')
        called_with_param = True

    if not called_with_param and already_running:
        # here we know that guake was called without any parameter and
        # it is already running, so, lets toggle its visibility.
        remote_object.show_hide()

    return already_running

if __name__ == '__main__':
    if not test_gconf():
        raise ShowableError(_('Guake can not init!'),
            _('Gconf Error.\n'
              'Have you installed <b>guake.schemas</b> properlly?'))

    if not main():
        gtk.main()
