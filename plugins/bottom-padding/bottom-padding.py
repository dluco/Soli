import gi
gi.require_version('Gtk', '3.0')
gi.require_version('Soli', '1.0')
from gi.repository import GObject, Gtk, Pango, Soli

CSS_CLASS = b'.soli-view'

class BottomPaddingPlugin(GObject.Object, Soli.ViewActivatable):
	view = GObject.property(type=Soli.View)

	def __init__(self):
		GObject.Object.__init__(self)

		self.page_size = -1
		self.font_height = -1
		self.description = None

		self.style_provider = Gtk.CssProvider()

	def do_activate(self):
		# Add custom css style provider for view
		style_context = self.view.get_style_context()
		style_context.add_provider(self.style_provider,
				Gtk.STYLE_PROVIDER_PRIORITY_APPLICATION)

		self.adjustment = self.view.get_vadjustment()

		# Get the font height and viewport page-size
		self.description = self.get_font_desc()
		self.font_height = self.get_font_height()

		self.page_size = self.get_page_size()

		# Set bottom padding for the first time
		# NOTE: depending on how fast the ::style-updated signal handler
		# is connected, setting the padding may trigger on_style_updated
		self.update_padding()

		# Listen for changes in the viewport page-size
		self.adjustment_handler = self.adjustment.connect('changed', \
				self.on_adjustment_changed)

		# Listen for changes to the widget's style (font)
		self.style_handler = self.view.connect('style-updated', \
				self.on_style_updated)

	def do_deactivate(self):
		# Remove custom css style provider
		style_context = self.view.get_style_context()
		style_context.remove_provider(self.style_provider)

		# Disconnect signal handlers
		self.adjustment.disconnect(self.adjustment_handler)
		self.view.disconnect(self.style_handler)

	def on_adjustment_changed(self, adjustment):
		page_size = self.get_page_size()

		if not page_size == self.page_size:
			# Page size changed, so padding must be updated as well
			self.page_size = page_size
			self.update_padding()

	def on_style_updated(self, widget):
		description = self.get_font_desc()

		if not self.description or not description.equal(self.description):
			self.description = description
			font_height = self.get_font_height()

			if not font_height == self.font_height:
				# Font height has changed, so padding must be updated as well
				self.font_height = font_height
				self.update_padding()

	def get_page_size(self):
		return int(self.adjustment.get_property('page-size'))

	def get_font_desc(self):
		style_context = self.view.get_style_context()

		# FIXME: using get_property() instead of get_font() results
		# in a corrupted UTF-8 description name, and likely also leads
		# to the font ascent and descent being wrong

#        return style_context.get_property('font', \
		#                                          style_context.get_state()).copy()

		return style_context.get_font(style_context.get_state()).copy()

	def get_font_height(self):
		pango_context = self.view.get_pango_context()
		metrics = pango_context.get_metrics(self.description, \
				pango_context.get_language())

		ascent = round(metrics.get_ascent() / Pango.SCALE)
		descent = round(metrics.get_descent() / Pango.SCALE)

		return ascent + descent

	def update_padding(self):
		if self.page_size <= 0 or self.font_height <= 0:
			# Page size or font height information is not available
			return

		css = CSS_CLASS + b' { padding-bottom: ' \
			+ str(self.page_size - self.font_height).encode() \
			+ b'px }'
		self.style_provider.load_from_data(css)
