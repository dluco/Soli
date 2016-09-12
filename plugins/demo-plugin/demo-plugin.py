from gi.repository import GObject
from gi.repository import Peas
from gi.repository import Gtk
from gi.repository import Soli

class DemoPlugin(GObject.Object, Soli.WindowActivatable):
    object = GObject.property(type=GObject.Object)

    def do_activate(self):
        print("activate")

    def do_deactivate(self):
        print("deactivate")

    def do_update_state(self):
        print("updated state!")
