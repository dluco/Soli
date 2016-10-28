from gi.repository import GObject, Gtk, GtkSource, PeasGtk, Soli

class HighlightSelectedPlugin(GObject.Object, Soli.ViewActivatable):
    __gtype_name__ = 'HighlightSelectedPlugin'

    view = GObject.property(type=Soli.View)

    def __init__(self):
        GObject.Object.__init__(self)

    def do_activate(self):
        doc = self.view.get_buffer()
        self.settings = GtkSource.SearchSettings()
        self.search = GtkSource.SearchContext(buffer=doc)

        if doc is not None and self.search is not None:
            doc.connect("notify::style-scheme", self.on_style_changed)
            doc.connect("mark-set", self.on_mark_set)

            self.get_a_match_style(doc.get_style_scheme())
            self.search.set_settings(self.settings)

    def do_deactivate(self):
        pass

    def on_style_changed(self, doc, spec):
        if doc is not None:
            self.get_a_match_style(doc.get_style_scheme())

    def on_mark_set(self, doc, iter, mark):
        if doc is None or mark is None:
            return

        insert = doc.get_insert()
        if mark != insert:
            return

        if doc.get_selection_bounds():
            start, end = doc.get_selection_bounds()
            text = doc.get_text(start, end, False)

            if text is not None and len(text.strip(" \t")) > 0:
                self.settings.set_search_text(text)
                self.search.set_highlight(True)
            else:
                self.settings.set_search_text(None)
                self.search.set_highlight(False)
        else:
            self.settings.set_search_text(None)
            self.search.set_highlight(False)

    def get_a_match_style(self, scheme):
        if scheme is None or self.search is None:
            return

        style = scheme.get_style("current-line")

        if style is not None:
            self.search.set_match_style(style)
            self.settings.set_search_text(None)
            self.search.set_highlight(False)
