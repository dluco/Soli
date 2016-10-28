from gi.repository import GObject, GLib, Gtk, Gio, Soli

SETTINGS_SCHEMA = "ca.dluco.soli.plugins.restoresession"

class RestoreSessionAppActivatable(GObject.Object, Soli.AppActivatable):
	__gtype_name__ = "RestoreSessionAppActivatable"

	app = GObject.property(type=Soli.App)

	def __init__(self):
		GObject.Object.__init__(self)

		self.settings = Gio.Settings.new(SETTINGS_SCHEMA)
		self.activated = False

	def do_activate(self):
		self.app.connect_after('activate', self.on_app_activate_after)

		self.app.connect('window-added', self.on_window_added)

		self.app.connect('quit', self.on_app_quit)

	def do_deactivate(self):
		pass

	def on_app_activate_after(self, app, data=None):
		if not self.activated and len(self.app.get_main_windows()) <= 1:
			window = self.app.get_active_main_window()
			self.restore_session(window)

		# App has been activated at least once,
		# so stop trying to restore session
		self.activated = True

	def on_app_quit(self, app, data=None):
		window = self.app.get_active_main_window()
		if window:
			self.save_session(window)

	def on_window_added(self, app, window, data=None):
		if type(window) is Soli.Window:
			window.connect('delete-event', self.on_window_delete_event)

	def on_window_delete_event(self, window, event, data=None):
		# Save sesion if this window is the last main window
		if len(self.app.get_main_windows()) <= 1:
			self.save_session(window)

	def restore_session(self, window):
		uris = []
		active_uri = ''

		uris = self.settings.get_value('uris')
		active_uri = self.settings.get_string('active-uri')

		set_active_uri = False

		if uris:
			# Close initial blank document
			if len(window.get_documents()) <= 1:
				tab = window.get_active_tab()
				if tab:
					doc = tab.get_document()
					if doc and doc.is_untouched():
						window.close_tab(tab)
						# No other documents loaded, so safe to
						# restore the active URI from previous session
						set_active_uri = True

			# Create tabs for saved uris as necessary
			for uri in uris:
				location = Gio.file_new_for_uri(uri)
				tab = window.get_tab_from_location(location)
				if not tab:
					window.create_tab_from_location(location, \
													None, 0, 0, False, False)

		if active_uri and set_active_uri:
			# Switch to the saved active URI
			location = Gio.file_new_for_uri(active_uri)
			tab = window.get_tab_from_location(location)
			if tab:
				window.set_active_tab(tab)

	def save_session(self, window):
		uris = []
		active_uri = ''

		for document in window.get_documents():
			gfile = document.get_location()
			if gfile:
				uris.append(gfile.get_uri())

		active_doc = window.get_active_document()
		if active_doc:
			gfile = active_doc.get_location()
			if gfile:
				active_uri = gfile.get_uri()

		self.settings.set_value('uris', GLib.Variant("as", uris))
		self.settings.set_string('active-uri', active_uri)
