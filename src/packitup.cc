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

#include "packitup.h"
#include "packitup_prefs.h"
#include "packitup_window.h"
#include <exception>
#include <gdkmm/paintable.h>
#include <gdkmm/pixbuf.h>
#include <gdkmm/snapshot.h>
#include <glibmm/i18n.h>
#include <gtkmm/aboutdialog.h>
#include <iostream>

Packitup::Packitup ()
    : Gtk::Application ("tech.bm7.packitup",
                        Gio::Application::Flags::DEFAULT_FLAGS)
{
}

Glib::RefPtr<Packitup>
Packitup::create ()
{
  return Glib::make_refptr_for_instance<Packitup> (new Packitup ());
}

PackitupWindow *
Packitup::create_appwindow ()
{
  auto packitup_window = PackitupWindow::create ();

  // same as set_application, manages lifecycle of windows
  add_window (*packitup_window);

  // Closes app when all windows are gone
  packitup_window->signal_hide ().connect (
      [packitup_window] () { delete packitup_window; });

  return packitup_window;
}

void
Packitup::on_activate ()
{
  try
    {
      auto appwindow = create_appwindow ();
      appwindow->present ();
    }
  catch (const Glib::Error &ex)
    {
      std::cerr << "Glib::Error Packitup::on_activate(): " << ex.what ()
                << std::endl;
    }
  catch (const std::exception &ex)
    {
      std::cerr << "std::exception Packitup::on_activate(): " << ex.what ()
                << std::endl;
    }
}

void
Packitup::on_startup ()
{
  // base class implementation
  Gtk::Application::on_startup ();

  // Actions and keyboard accelerators for the menu
  add_action ("preferences",
              sigc::mem_fun (*this, &Packitup::on_action_preferences));
  add_action ("about", sigc::mem_fun (*this, &Packitup::on_action_about));
  add_action ("quit", sigc::mem_fun (*this, &Packitup::on_action_quit));
  set_accel_for_action ("app.quit", "<Ctrl>Q");
}

void
Packitup::on_action_preferences ()
{
  try
    {
      auto prefs_dialog = PackitupPrefs::create (*get_active_window ());
      prefs_dialog->present ();

      // Delete when its hidden
      prefs_dialog->signal_hide ().connect (
          [prefs_dialog] () { delete prefs_dialog; });
    }
  catch (const Glib::Error &ex)
    {
      std::cerr << "Glib::Error: Packitup::on_action_preferences(): "
                << ex.what () << std::endl;
    }
  catch (const std::exception &ex)
    {
      std::cerr << "std::exception: Packitup::on_action_preferences(): "
                << ex.what () << std::endl;
    }
}

void
Packitup::on_action_about ()
{
  try
    {
      auto refBuilder = Gtk::Builder::create_from_resource (
          "/tech/bm7/packitup/src/about.ui");
      auto dialog = refBuilder->get_widget<Gtk::AboutDialog> ("about_window");

      dialog->set_transient_for (*get_active_window ());
      dialog->set_program_name ("PackItUP!");
      dialog->set_copyright ("Copyright (C) 2025  edu-bm7 <edubm7@bm7.tech>");
      dialog->set_logo (Gdk::Texture::create_from_resource (
          "/tech/bm7/packitup/src/tech.bm7.packitup.svg"));
      dialog->set_license_type (Gtk::License::GPL_3_0);
      dialog->set_wrap_license (true);
      dialog->set_comments (_ ("Never run out of beer again."));
      dialog->set_website ("https://github.com/BM7Tech/packitup.git");
      dialog->set_website_label ("PackItUP! Website");
      std::vector<Glib::ustring> list_authors;
      list_authors.push_back ("edu-bm7 https://github.com/edu-bm7");
      list_authors.push_back ("BM7Tech https://github.com/BM7Tech");
      dialog->set_authors (list_authors);
      dialog->set_visible (true);
      dialog->present ();
      dialog->signal_hide ().connect ([dialog] () { delete dialog; });
    }
  catch (const Glib::Error &ex)
    {
      std::cerr << "Glib::Error: Packitup::on_action_about(): " << ex.what ()
                << std::endl;
    }
  catch (const std::exception &ex)
    {
      std::cerr << "std::exception: Packitup::on_action_about(): "
                << ex.what () << std::endl;
    }
}

void
Packitup::on_action_quit ()
{
  auto windows = get_windows ();
  for (auto window : windows)
    window->set_visible (false);

  quit ();
}

// vim: sts=2 sw=2 et
