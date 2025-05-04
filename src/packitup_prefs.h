/*
 * PackItUP! Never run out of beer again.
 * Copyright (C) 2025  edu-bm7 <edubm7@bm7.tech>
 *
 * This file is part of PackItUP!.
 *
 * PackItUP! is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PackItUP! is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PackItUP!. If not, see <https://www.gnu.org/licenses/>.
 * */

#ifndef PACKITUP_PREFS_H
#define PACKITUP_PREFS_H

// This are for compatibility with Debian/Old Distros, because of missing
// headers
#include <glibmmconfig.h>
#if __has_include(<glibmm/version.h>)
#include <glibmm/version.h>
#endif
// normal headers
#include <giomm/settings.h>
#include <gtkmm/builder.h>
#include <gtkmm/button.h>
#include <gtkmm/dropdown.h>
#include <gtkmm/fontdialogbutton.h>
#include <gtkmm/window.h>

#ifdef GLIBMM_CHECK_VERSION
#define HAS_GIO_SETTINGS_BIND_WITH_MAPPING GLIBMM_CHECK_VERSION (2, 75, 0)
#else
#define HAS_GIO_SETTINGS_BIND_WITH_MAPPING 0
#endif

class PackitupPrefs : public Gtk::Window
{
public:
  PackitupPrefs (BaseObjectType *cobject,
                 const Glib::RefPtr<Gtk::Builder> &refBuilder);

  static PackitupPrefs *create (Gtk::Window &parent);
  void on_button_close ();

protected:
#if HAS_GIO_SETTINGS_BIND_WITH_MAPPING
  // Mappings from Gio::Settings to properties
  static std::optional<unsigned int>
  map_from_ustring_to_int (const Glib::ustring &transition);
  static std::optional<Glib::ustring>
  map_from_int_to_ustring (const unsigned int &pos);
#else
  // Signal handlers
  void on_font_setting_changed (const Glib::ustring &key);
  void on_font_selection_changed ();
  void on_transition_setting_changed (const Glib::ustring &key);
  void on_transition_selection_changed ();
#endif
  Glib::RefPtr<Gtk::Builder> m_refBuilder;
  Glib::RefPtr<Gio::Settings> m_settings;
  Gtk::FontDialogButton *m_font{ nullptr };
  Gtk::DropDown *m_transition{ nullptr };
  Gtk::Button *m_prefs_close{ nullptr };
};

#endif // PACKITUP_PREFS_H
// vim: sts=2 sw=2 et
