<h1 align="center"> <img src="https://raw.githubusercontent.com/BM7Tech/packitup/master/src/icons/hicolor/64x64/apps/tech.bm7.packitup.png" alt="PackItUP"> Pack It UP! <p> Never run out of beer again.</p> 
</h1>

[![License](https://img.shields.io/badge/License-GPLv3-blue)](./LICENSE)
[![English](https://img.shields.io/badge/README-en--US-blue)](./README.md)
[![Português](https://img.shields.io/badge/README-pt--BR-green)](./README.pt-BR.md)
- This is our application for you and your friends never run out of beer again in your parties/barbecue, etc :)
## Tested on:
- [x] Ubuntu/Kubuntu
- [x] Fedora Workstation/KDE Plasma
- [x] OpenSUSE Tumbleweed
- [x] Arch Linux
- [x] Alpine Linux
- [ ] Void Linux
## KDE Plasma/Kubuntu
Although the app work in KDE environment, theming it might be tricky and it might contain minor glitches. It doesn't contain LibAdwaita though, making KDE lovers life easier.
## GNOME/LibAdwaita
If you want a more adept version of the app for the GNOME Desktop Environment(dark theme support, etc), check it out [PackItUP! - GNOME](https://github.com/BM7Tech/packitup-gnome).
## Install
You have two ways of installing our application:

## 1 - Download The AppImage
- You can just [Download the AppImage](https://github.com/BM7Tech/packitup/releases/download/v1.0.2/PackItUP-v1.0.2.AppImage)
- Then `chmod +x PackItUP-v1.0.2.AppImage` and either double click the app image to launch it,
or in your terminal `./Packitup-v1.0.2.AppImage`

## 2 - Build from source
1. Install [Build prerequisites](#build-prerequisites)
2. Then [Download Release Source Code](https://github.com/BM7Tech/packitup/releases/download/v1.0.2/packitup-v1.0.2.tar.gz)
3. `tar xzf packitup-v1.0.2.tar.gz && cd packitup-v1.0.2`
4. `meson setup build --prefix=/usr`
    - At the moment you could install it with a custom location, but localization packages won't work(Translation packages). 
    - If you wish to do it anyway, run `meson setup --prefix=YOUR_CUSTOM_INSTALL_PATH build`
5. `cd build` 
    - Change to the build directory
6. `ninja`
7. `sudo ninja install`

## Uninstall
If you downloaded the `AppImage`, just delete the file and you are done,
else follow the instructions bellow:

If you downloaded the source code, to uninstall our program we have a small 
`uninstall.sh` script that take cares of everything. 
Note that our script takes 2 arguments `--build` or `-b` and `--prefix` or `-p`.
The `--prefix` argument is optional, only use it if your PackItUP! installation
isn't in your PATH(you used --prefix in meson setup and this prefix ins't in your PATH).
The `--build` argument takes your build folder path(`build` in our example),
relative to where you are launching `uninstall.sh`.
To uninstall just run:
```
sudo ./uninstall.sh --build build
```
Where `build` is the build directory you specified with `meson setup`.
You can also just `cd build`, then `sudo ninja uninstall` but don't forget to
update your Gtk icons cache for the correct folder where our application icons were
installed.

## Build prerequisites

- GCC 8+ or Clang 5+ (C++17 support)

### Ubuntu
```sh
sudo apt update
sudo apt install -y \
  fonts-noto \
  libgtk-4-1 libgtk-4-dev \
  libgtkmm-4.0-0 libgtkmm-4.0-dev \
  build-essential meson ninja-build curl \
  gettext libxml2-utils \
  pkg-config python3 \
  xdg-desktop-portal xdg-desktop-portal-gtk xdg-utils
```

### Fedora/RHEL
```
sudo dnf install -y \
  google-noto-fonts-common \
  gtk4 gtk4-devel gtkmm4.0 gtkmm4.0-devel \
  @c-development @development-tools \
  libxml2 gettext \
  meson ninja-build curl pkgconf-pkg-config python3 \
  xdg-desktop-portal xdg-desktop-portal-gtk xdg-utils
```

### OpenSUSE Tumbleweed
```
sudo zypper refresh
sudo zypper install -y \
  google-noto-fonts \
  gtk4 gtkmm4-devel \
  meson ninja curl gcc-c++ pkg-config python3 \
  xdg-desktop-portal xdg-desktop-portal-gtk xdg-utils
```

### Arch Linux
```
sudo pacman -Syu --needed \
  noto-fonts \
  base-devel meson ninja curl pkgconf python \
  gtk4 gtkmm-4.0 \
  libxml2 gettext \
  xdg-desktop-portal xdg-desktop-portal-gtk xdg-utils
```

### Alpine Linux
```
apk add \
  font-noto-all
  build-base pkgconf meson ninja-build python3 curl \
  gtk4.0 gtk4.0-dev \
  xdg-desktop-portal xdg-desktop-portal-gtk xdg-utils
```

### Void Linux (packages names might be wrong, didn't test it)
```
sudo xbps-install -Sy \
  noto-fonts-ttf \
  gtk4 gtk4-devel gtkmm4 gtkmm4-devel \
  base-devel meson ninja curl gcc pkgconf python3 \
  xdg-desktop-portal xdg-desktop-portal-gtk xdg-utils
```

### Window Manager in X11
- You'll need a compositor to handle shadow, transparency, stacking and other effects.
E.g. `picom`

### Fonts
- To run the app in a language that is not english(right now only pt-BR is available)
you'll probally will need a font that supports it. E.g `noto-fonts`
