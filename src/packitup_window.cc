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

#include "packitup_window.h"
#include "gtkmm/requisition.h"
#include "packitup.h"
#include <glib/gi18n.h>
#include <glibmm/i18n.h>
#include <glibmm/miscutils.h>
#include <gtkmm/enums.h>
#include <gtkmm/frame.h>
#include <gtkmm/icontheme.h>
#include <gtkmm/object.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/textview.h>
#include <gtkmm/version.h>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

#define HAS_STYLE_PROVIDER_ADD_PROVIDER_FOR_DISPLAY                           \
  GTKMM_CHECK_VERSION (4, 9, 1)

#define HAS_LOAD_FROM_STRING GTKMM_CHECK_VERSION (4, 12, 0)

PackitupWindow::PackitupWindow (BaseObjectType *cobject,
                                const Glib::RefPtr<Gtk::Builder> &refBuilder)
    : Gtk::ApplicationWindow (cobject), m_refBuilder (refBuilder)
{

  //  The app buttons to calculate the result
  m_spinButtonMore
      = m_refBuilder->get_widget<Gtk::SpinButton> ("spin_button_more");
  if (!m_spinButtonMore)
    throw std::runtime_error ("no \"spin_button_more\" object in window.ui");

  m_header = m_refBuilder->get_widget<Gtk::HeaderBar> ("header");
  if (!m_header)
    throw std::runtime_error ("no \"header\" object in window.ui");

  m_spinButtonAlright
      = m_refBuilder->get_widget<Gtk::SpinButton> ("spin_button_alright");
  if (!m_spinButtonAlright)
    throw std::runtime_error (
        "no \"spin_button_alright\" object in window.ui");
  m_spinButtonAlright->set_numeric ();
  m_spinButtonMore->set_numeric ();

  // Add scrollable window to the whole application for WM support
  auto applicationBoxLayout
      = m_refBuilder->get_widget<Gtk::Box> ("application_box_layout");
  if (!applicationBoxLayout)
    throw std::runtime_error (
        "no \"application_box_layout\" object in window.ui");
  auto app_scrolledWindow = Gtk::make_managed<Gtk::ScrolledWindow> ();
  app_scrolledWindow->set_policy (Gtk::PolicyType::AUTOMATIC,
                                  Gtk::PolicyType::AUTOMATIC);
  app_scrolledWindow->set_expand ();

  set_child (*app_scrolledWindow);
  app_scrolledWindow->set_child (*applicationBoxLayout);
  app_scrolledWindow->set_propagate_natural_width (true);
  app_scrolledWindow->set_propagate_natural_height (true);

  app_scrolledWindow->set_min_content_height (700);

  m_unitDropDown = m_refBuilder->get_widget<Gtk::DropDown> ("unit_dropdown");
  if (!m_unitDropDown)
    throw std::runtime_error ("no \"unit_dropdown\" object in window.ui");

  m_unitDropDown->property_selected ().signal_changed ().connect (
      sigc::mem_fun (*this, &PackitupWindow::on_unit_dropdown_changed));
  m_resultButton = m_refBuilder->get_widget<Gtk::Button> ("result_button");
  if (!m_resultButton)
    throw std::runtime_error ("no \"result_button\" object in window.ui");

  // Bind Button
  m_resultButton->signal_clicked ().connect (
      sigc::mem_fun (*this, &PackitupWindow::on_result_clicked));

  // App results scrolled window
  m_resultVBox = m_refBuilder->get_widget<Gtk::Box> ("result_VBox");
  if (!m_resultVBox)
    throw std::runtime_error ("no \"result_VBox\" object in window.ui");

  // Bottle size dropdown menu
  m_bottleSizeDropDown
      = m_refBuilder->get_widget<Gtk::DropDown> ("bottle_size_dropdown");
  if (!m_bottleSizeDropDown)
    throw std::runtime_error (
        "no \"bottle_size_dropdown\" object in window.ui");

  m_bottleSizeDropDown->set_selected (0);

  // Bottle size dropdown menu
  m_refBottleSizeList
      = m_refBuilder->get_object<Gtk::StringList> ("bottle_size_list");
  if (!m_refBottleSizeList)
    throw std::runtime_error ("no \"bottle_size_list\" object in window.ui");

  // Bottle size dropdown menu
  m_refPackSizeList
      = m_refBuilder->get_object<Gtk::StringList> ("pack_size_list");
  if (!m_refPackSizeList)
    throw std::runtime_error ("no \"pack_size_list\" object in window.ui");

  // Pack size dropdown menu
  m_packSizeDropDown
      = m_refBuilder->get_widget<Gtk::DropDown> ("pack_size_dropdown");
  if (!m_packSizeDropDown)
    throw std::runtime_error ("no \"pack_size_dropdown\" object in window.ui");

  m_packSizeDropDown->set_selected (0);

  // App info box
  auto m_info_VBox = m_refBuilder->get_widget<Gtk::Box> ("info_box");
  if (!m_info_VBox)
    throw std::runtime_error ("no \"info_box\" object in window.ui");

  // TextView of the 'Info' Gtk::Box
  auto m_info_view = Gtk::make_managed<Gtk::TextView> ();
  m_info_view->set_editable (false);
  m_info_view->set_cursor_visible (false);
  m_info_view->set_wrap_mode (Gtk::WrapMode::WORD);

  m_info_VBox->append (*m_info_view);
  // Set the View Buffer
  auto m_refInfoBuffer = m_info_view->get_buffer ();

  // Bind font settings to infoTextBuffer
  m_refSettings = Gio::Settings::create ("tech.bm7.packitup");
  auto m_refTagInfoFont = m_refInfoBuffer->create_tag ();
  m_refSettings->bind ("font", m_refTagInfoFont->property_font ());
  m_refInfoBuffer->set_text (_ (
      "This is our application for how many packs of beer you and your "
      "friends need to buy so you won't be out of beer :)\n\n"
      "The calculation takes how many people drinks more than moderated, and "
      "how many drinks alright\n\n"
      "More than moderated would be more than or equal to 2 packs that "
      "day(12 bottles "
      "of 269ml or 8oz, ~3.0L or ~96oz).\n\n"
      "Keep in mind that in order to not let you go out of beer this "
      "calculation will round up the amount of packs needed.\n\n"
      "The values in Liters and Ounces aren't direct convertable. It takes "
      "into account localization."));
  m_refInfoBuffer->apply_tag (m_refTagInfoFont, m_refInfoBuffer->begin (),
                              m_refInfoBuffer->end ());

  m_refGtkSettings = Gtk::Settings::get_default ();
  m_refGtkSettings->property_gtk_enable_animations ()
      .signal_changed ()
      .connect (sigc::mem_fun ((*this),
                               &PackitupWindow::update_revealer_transition));

  update_revealer_transition ();

  // App TextBuffer and TextView of the 'Calculate' button
  auto m_text_view = Gtk::make_managed<Gtk::TextView> ();
  m_text_view->set_editable (false);
  m_text_view->set_cursor_visible (false);
  m_text_view->set_wrap_mode (Gtk::WrapMode::WORD);

  // Add result's frame
  auto m_frame = Gtk::make_managed<Gtk::Frame> ();
  m_frame->set_label (_ ("Result"));
  m_frame->set_label_align (Gtk::Align::CENTER);
  m_frame->set_expand ();
  m_resultVBox->append (*m_frame);

  // Append the Revealer to the layout
  m_frame->set_child (m_revealer);
  m_revealer.set_margin_start (12);
  m_revealer.set_margin_end (12);
  m_revealer.set_transition_duration (500);
  auto m_scrolled_window = Gtk::make_managed<Gtk::ScrolledWindow> ();
  m_scrolled_window->set_policy (Gtk::PolicyType::AUTOMATIC,
                                 Gtk::PolicyType::ALWAYS);
  m_scrolled_window->set_expand ();
  m_revealer.set_child (*m_scrolled_window);
  m_scrolled_window->set_child (*m_text_view);
  m_revealer.set_reveal_child (true);

  // Set the View Buffer
  m_refBuffer = m_text_view->get_buffer ();

  // Bind font settings to TextBuffer results
  m_refTagFont = m_refBuffer->create_tag ();
  m_refSettings->bind ("font", m_refTagFont->property_font ());

  m_refBuffer->set_text (_ ("Select the values above and click \"Calculate\" "
                            "to begin..."));
  m_refBuffer->apply_tag (m_refTagFont, m_refBuffer->begin (),
                          m_refBuffer->end ());
  m_revealer.property_child_revealed ().signal_changed ().connect (
      sigc::mem_fun (*this, &PackitupWindow::on_result_changed));

  // App Gears menu, with preferences, about and quit button
  m_gears = m_refBuilder->get_widget<Gtk::MenuButton> ("gears");
  if (!m_gears)
    throw std::runtime_error ("no \"gears\" object in window.ui");

  new_decoration_layout ();

  // Ensure that the HeaderBar has at least icon:close decoration layout
  m_refGtkSettings->property_gtk_decoration_layout ()
      .signal_changed ()
      .connect (sigc::mem_fun (*this, &PackitupWindow::new_decoration_layout));

  // Set the window icon from gresources
  auto icon_theme = Gtk::IconTheme::get_for_display (get_display ());
  icon_theme->add_resource_path ("/tech/bm7/packitup/src");
  set_icon_name ("tech.bm7.packitup"); // Uses the theme icon

  m_refAppCustomCssProvider = Gtk::CssProvider::create ();
  m_refThemeCssProvider = Gtk::CssProvider::create ();
  m_refAppCustomCssProvider->signal_parsing_error ().connect (
      [] (const auto &section, const auto &error) {
        on_parsing_error (section, error);
      });

  m_providerAdded = false;
  reload_theme_css ();
#if HAS_STYLE_PROVIDER_ADD_PROVIDER_FOR_DISPLAY
  Gtk::StyleProvider::add_provider_for_display (
      get_display (), m_refThemeCssProvider, GTK_STYLE_PROVIDER_PRIORITY_USER);
  Gtk::StyleProvider::add_provider_for_display (
      get_display (), m_refAppCustomCssProvider,
      GTK_STYLE_PROVIDER_PRIORITY_USER);
#else
  Gtk::StyleContext::add_provider_for_display (
      get_display (), m_refThemeCssProvider,
      GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  Gtk::StyleContext::add_provider_for_display (
      get_display (), m_refAppCustomCssProvider,
      GTK_STYLE_PROVIDER_PRIORITY_USER);
#endif
  // GNOME Dark/Light theme switch
  auto gnome = Gio::Settings::create ("org.gnome.desktop.interface");
  gnome->signal_changed ().connect ([this] (const Glib::ustring &key) {
    if (key == "gtk-theme" || key == "color-scheme")
      reload_theme_css ();
  });
  m_refGtkSettings->property_gtk_theme_name ().signal_changed ().connect (
      sigc::mem_fun (*this, &PackitupWindow::reload_theme_css));
  m_refGtkSettings->property_gtk_application_prefer_dark_theme ()
      .signal_changed ()
      .connect (sigc::mem_fun (*this, &PackitupWindow::reload_theme_css));
}

void
PackitupWindow::new_decoration_layout ()
{
  Glib::ustring default_layout
      = m_refGtkSettings->property_gtk_decoration_layout ().get_value ();

  Glib::ustring new_layout = default_layout;
  // Split decoration layout into left/right around ':'
  size_t pos = new_layout.find (":");
  Glib::ustring left_side
      = (pos == Glib::ustring::npos ? new_layout : new_layout.substr (0, pos));

  Glib::ustring right_side
      = (pos == Glib::ustring::npos ? "" : new_layout.substr (pos + 1));

  size_t pos_r_close = right_side.find ("close");
  size_t pos_l_close = left_side.find ("close");
  bool close_on_left = false;
  if (pos_r_close != Glib::ustring::npos)
    new_layout = "icon:" + right_side;
  else if (pos_l_close != Glib::ustring::npos)
    {
      new_layout = left_side + ":icon";
      close_on_left = true;
    }
  else
    new_layout = "icon:";

  m_header->set_decoration_layout (new_layout);

  // Connect the menu(gears_menu.ui) to the MenuButton m_gears
  // The connection between action and menu item is specified in gears_menu.ui)
  auto menu_builder = Gtk::Builder::create_from_resource (
      "/tech/bm7/packitup/src/gears_menu.ui");
  auto menu = menu_builder->get_object<Gio::MenuModel> ("menu");
  if (!menu)
    throw std::runtime_error ("No \"menu\" object in gears_menu.ui");
  m_gears->set_menu_model (menu);

  // Pack the menu on the opposite side of the close menu
  m_header->remove (*m_gears);
  if (close_on_left)
    m_header->pack_end (*m_gears);
  else
    m_header->pack_start (*m_gears);
}

void
PackitupWindow::reload_theme_css ()
{
  // Look up the current GTK theme name frok GtkSettings
  auto theme = Gtk::Settings::get_default ()->property_gtk_theme_name ();
  auto css_path = find_theme_css_path (theme);
  if (!css_path.empty ())
    {
      if (!m_providerAdded)
        {
#if HAS_STYLE_PROVIDER_ADD_PROVIDER_FOR_DISPLAY
          Gtk::StyleProvider::add_provider_for_display (
              get_display (), m_refThemeCssProvider,
              GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
          Gtk::StyleProvider::add_provider_for_display (
              get_display (), m_refAppCustomCssProvider,
              GTK_STYLE_PROVIDER_PRIORITY_USER);
#else
          Gtk::StyleContext::add_provider_for_display (
              get_display (), m_refThemeCssProvider,
              GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
          Gtk::StyleContext::add_provider_for_display (
              get_display (), m_refAppCustomCssProvider,
              GTK_STYLE_PROVIDER_PRIORITY_USER);
#endif
          m_providerAdded = true;
        }
      m_refThemeCssProvider->load_from_path (css_path);

#if HAS_LOAD_FROM_STRING
      Glib::ustring css = "windowcontrols>image{ min-height:24px; "
                          "min-width:24px; margin: 0px; }";
      m_refAppCustomCssProvider->load_from_string (css);
#else
      std::string css = "windowcontrols>image{ min-height:24px; "
                        "min-width:24px; margin: 0px; }";
      m_refAppCustomCssProvider->load_from_data (css);
#endif
    }
  else
    {
      if (m_providerAdded)
        {
#if HAS_STYLE_PROVIDER_ADD_PROVIDER_FOR_DISPLAY
          Gtk::StyleProvider::remove_provider_for_display (
              get_display (), m_refThemeCssProvider);
#else
          Gtk::StyleContext::remove_provider_for_display (
              get_display (), m_refThemeCssProvider);
#endif
          m_providerAdded = false;
        }
#if HAS_LOAD_FROM_STRING
      Glib::ustring css = "windowcontrols>image{ min-height:24px; "
                          "min-width:24px; margin: 0px 6px; }";
      m_refAppCustomCssProvider->load_from_string (css);
#else
      std::string css = "windowcontrols>image{ min-height:24px; "
                        "min-width:24px; margin: 0px 6px; }";
      m_refAppCustomCssProvider->load_from_data (css);
#endif
    }
}

void
PackitupWindow::update_revealer_transition ()
{
  bool animations_on = m_refGtkSettings->property_gtk_enable_animations ();
  if (animations_on)
    m_refSettings->bind ("transition", m_revealer.property_transition_type ());
  else
    m_revealer.set_transition_type (Gtk::RevealerTransitionType::NONE);
}

Glib::ustring
PackitupWindow::find_theme_css_path (const Glib::ustring &theme)
{
  // Handle Dark Theme Switch
  auto dark = Gtk::Settings::get_default ()
                  ->property_gtk_application_prefer_dark_theme ();
  // Split "Theme:dark" -> base="Theme" variant="dark"
  auto sep = theme.find (":");
  auto base = (sep == Glib::ustring::npos ? theme : theme.substr (0, sep));
  auto variant = (sep == Glib::ustring::npos ? Glib::ustring{}
                                             : theme.substr (sep + 1));
  auto css_file = ((variant == "dark" || dark) ? "gtk-dark.css" : "gtk.css");

  {
    // User themes in ~/.config/gtk-4.0/gtk.css will always be applied
    auto cfg = Glib::build_filename (Glib::get_user_config_dir (), "gtk-4.0",
                                     css_file);
    if (Gio::File::create_for_path (cfg)->query_exists ())
      return cfg;
  }

  // helper to see if theme exist
  auto exists = [&] (const Glib::ustring &p) {
    return Gio::File::create_for_path (p)->query_exists ();
  };

  // User .themes dir
  {
    auto user_themes_dir = Glib::build_filename (
        Glib::get_home_dir (), ".themes", base, "gtk-4.0", css_file);
    if (exists (user_themes_dir))
      return user_themes_dir;
  }

  // User data dirs
  {
    auto user_data = Glib::build_filename (
        Glib::get_user_data_dir (), "themes", base, "gtk-4.0", css_file);
    if (exists (user_data))
      return user_data;
  }

  // user system data dirs
  for (auto &dir : Glib::get_system_data_dirs ())
    {
      auto path
          = Glib::build_filename (dir, "themes", base, "gtk-4.0", css_file);
      if (exists (path))
        return path;
    }

  return {};
}

void
PackitupWindow::on_parsing_error (
    const Glib::RefPtr<const Gtk::CssSection> &section,
    const Glib::Error &error)
{
  std::cerr << "on_parsing_error(): " << error.what () << std::endl;
  if (section)
    {
      const auto file = section->get_file ();
      if (file)
        {
          std::cerr << "  URI = " << file->get_uri () << std::endl;
        }

      auto start_location = section->get_start_location ();
      auto end_location = section->get_end_location ();
      std::cerr << "  start_line = " << start_location.get_lines () + 1
                << ", end_line = " << end_location.get_lines () + 1
                << std::endl;
      std::cerr << "  start_position = " << start_location.get_line_chars ()
                << ", end_position = " << end_location.get_line_chars ()
                << std::endl;
    }
}

PackitupWindow *
PackitupWindow::create ()
{
  auto refBuilder = Gtk::Builder::create_from_resource (
      "/tech/bm7/packitup/src/window.ui");

  auto window = Gtk::Builder::get_widget_derived<PackitupWindow> (
      refBuilder, "app_window");

  if (!window)
    throw std::runtime_error ("No \"app_window\" object in window.ui");

  return window;
}

void
PackitupWindow::on_unit_dropdown_changed ()
{
  while (m_refBottleSizeList->get_n_items ())
    m_refBottleSizeList->remove (0);
  auto unit_idx = m_unitDropDown->get_selected ();
  if (unit_idx > 0)
    {
      m_refBottleSizeList->append (_ ("8oz"));
      m_refBottleSizeList->append (_ ("11.2oz"));
      m_refBottleSizeList->append (_ ("11.5oz"));
      m_refBottleSizeList->append (_ ("12oz"));
    }
  else
    {
      m_refBottleSizeList->append (_ ("250ml"));
      m_refBottleSizeList->append (_ ("269ml"));
      m_refBottleSizeList->append (_ ("330ml"));
      m_refBottleSizeList->append (_ ("355ml"));
    }
}

void
PackitupWindow::on_result_clicked ()
{

  if (m_transitionInProgress)
    return;
  m_transitionInProgress = true;

  // Start the hide animation
  m_revealer.set_reveal_child (false);
}

void
PackitupWindow::on_result_changed ()
{
  if (!m_revealer.get_reveal_child ())
    {
      //  Get app values
      auto more_Value = m_spinButtonMore->get_value ();
      auto alright_Value = m_spinButtonAlright->get_value ();
      auto unit_idx = m_unitDropDown->get_selected ();
      auto single_bottle_idx = m_bottleSizeDropDown->get_selected ();
      auto pack_size_idx = m_packSizeDropDown->get_selected ();
      auto single_bottle
          = std::stof (m_refBottleSizeList->get_string (single_bottle_idx));
      auto pack_size
          = std::stoi (m_refPackSizeList->get_string (pack_size_idx));

      // Set the Unit to use
      if (unit_idx > 0)
        m_unit = "oz";
      else
        {
          m_unit = "L";
          single_bottle /= 1000;
        }

      // Calculate the number of packs for that day they will have to buy
      auto pack = single_bottle * pack_size;
      float pack_size_max_value;
      if (pack > 1.5)
        pack_size_max_value = 1.5;
      else
        pack_size_max_value = pack;
      m_amountOfBeer = ((more_Value * 2 * pack_size_max_value)
                        + more_Value * single_bottle)
                       + (pack_size_max_value * alright_Value
                          + (alright_Value * 2 * single_bottle));
      m_numberOfPacks = std::ceil (m_amountOfBeer / pack);
      m_amountOfBeerPacks = pack * m_numberOfPacks;
      // The total number of people that will consume that amount of beer
      m_totalPeopleNumber = more_Value + alright_Value;

      // Insert text with aproriate tags one at a time
      Glib::ustring initial_str
          = _ ("The total number of packs you might need: ");
      auto refTagBold = m_refBuffer->create_tag ();
      refTagBold->property_weight () = 800;
      m_refBuffer->set_text (initial_str);
      auto offset_field = initial_str.size ();
      auto iterBuffer = m_refBuffer->get_iter_at_offset (offset_field);

      // We need to convert int/float values to ustring, and to calculate
      // their strlen
      Glib::ustring str_number_of_packs = Glib::ustring::compose (
          _ ("%1"), Glib::ustring::format (std::fixed, std::setprecision (0),
                                           m_numberOfPacks));
      m_refBuffer->insert_with_tag (iterBuffer, str_number_of_packs,
                                    refTagBold);

      offset_field += str_number_of_packs.size ();
      iterBuffer = m_refBuffer->get_iter_at_offset (offset_field);

      Glib::ustring second_str = _ ("\nYou might need ");
      m_refBuffer->insert (iterBuffer, second_str);

      offset_field += second_str.size ();
      iterBuffer = m_refBuffer->get_iter_at_offset (offset_field);

      Glib::ustring str_amount_of_beer = Glib::ustring::compose (
          _ ("%1%2"),
          Glib::ustring::format (std::fixed, ::std::setprecision (1),
                                 m_amountOfBeer),
          m_unit);
      m_refBuffer->insert_with_tag (iterBuffer, str_amount_of_beer,
                                    refTagBold);

      Glib::ustring third_str = _ (" of beer for ");
      offset_field += str_amount_of_beer.size ();
      iterBuffer = m_refBuffer->get_iter_at_offset (offset_field);

      m_refBuffer->insert (iterBuffer, third_str);

      Glib::ustring str_total_ppl_number
          = Glib::ustring::compose (_ ("%1"), m_totalPeopleNumber);
      offset_field += third_str.size ();
      iterBuffer = m_refBuffer->get_iter_at_offset (offset_field);
      m_refBuffer->insert_with_tag (iterBuffer, str_total_ppl_number,
                                    refTagBold);
      Glib::ustring fourth_str;
      if (m_totalPeopleNumber > 1)
        fourth_str = _ (" people.\n");
      else
        fourth_str = _ (" person.\n");

      offset_field += str_total_ppl_number.size ();
      iterBuffer = m_refBuffer->get_iter_at_offset (offset_field);
      m_refBuffer->insert (iterBuffer, fourth_str);

      Glib::ustring str_packs;
      if (m_numberOfPacks > 1)
        str_packs = _ ("packs");
      else
        str_packs = _ ("pack");
      Glib::ustring str_total = Glib::ustring::compose (
          _ ("%1 %2 of beer have %3%4."), m_numberOfPacks, str_packs,
          m_amountOfBeerPacks, m_unit);
      offset_field += fourth_str.size ();
      iterBuffer = m_refBuffer->get_iter_at_offset (offset_field);
      m_refBuffer->insert_with_tag (iterBuffer, str_total, refTagBold);
      m_refBuffer->apply_tag (m_refTagFont, m_refBuffer->begin (),
                              m_refBuffer->end ());

      m_revealer.set_reveal_child (true);
    }
  else
    m_transitionInProgress = false;
}

// vim: sts=2 sw=2 et
