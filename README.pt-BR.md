
# ![PackItUP! Logo](./src/icons/hicolor/48x48/apps/tech.bm7.packitup.png) Pack It UP! - Nunca mais fique sem cerveja.
[![License](https://img.shields.io/badge/License-GPLv3-blue)](./LICENSE)
[![English](https://img.shields.io/badge/README-en--US-blue)](./README.md)
[![Português](https://img.shields.io/badge/README-pt--BR-green)](./README.pt-BR.md)
- Essa é o nosso aplicativo para calcular quanto de cerveja você e seus amigos
precisam comprar para não faltar cerveja na sua festa/churrasco/encontro :)

## Testado em:
- [x] Ubuntu/Kubuntu
- [x] Fedora Workstation/KDE Plasma
- [x] OpenSUSE Tumbleweed
- [x] Arch Linux
- [x] Alpine Linux
- [ ] Void Linux

## KDE Plasma/Kubuntu
Apesar do aplicativo funcionar no ambiente Desktop KDE, mudar a aparência da aplicação para combinar com o seu Desktop pode ser um tanto quanto complicado e conter alguns pequenos bugs. Entretando o aplicativo não contém a livraria LibAdwaita, facilitando a vida daqueles que amam KDE.

## GNOME/LibAdwaita
Caso você queira uma versão mais adequada ao Ambiente Desktop GNOME (suporte ao tema escuro, etc), veja [PackItUP! - GNOME](https://github.com/BM7Tech/packitup-gnome).

## Instalar
No momento a única maneira de instalar o nosso programa é clonando o repositório:

1. Instale as [Dependências de compilação](#build-prerequisites)
2. `git clone https://github.com/edu-bm7/packitup`
3. `cd packitup`
4. `meson setup builddir --prefix=/usr`
    - Caso queira, você pode escolher um diretório customizado para instalação, porém, os pacotes de tradução não irão funcionar. 
    - Se quiser fazê-lo mesmo assim, `meson setup --prefix=SEU_DIRETORIO builddir`
5. `cd builddir`
    - Mude para o diretório onde será feito a compilação
6. `ninja`
7. `sudo ninja install`

## Desinstalar
Para desinstalar nosso programa temos um pequeno script `uninstall.sh` que cuida
de todo o processo. Nosso script recebe 2 argumentos `--build` ou `-b`, e `--prefix` ou
`-p`. O argumento `--prefix` é opicional, só o utilize caso sua instalação do PackItUP!
não estaja em seu PATH(você utilizou --prefix quando configurou o meson e esse prefixo não
existe no seu PATH). O argumento `--build` recebe o caminho para a sua pasta de compilação
(`builddir` no nosso exemplo), o caminho é relativo ao local em que você está 
executando `uninstall.sh`.
Para desinstalar simplesmente execute:
```
sudo ./uninstall.sh --build builddir
```
Onde `builddir` é o diretório em que você especificou em `meson setup`. 
Você também pode simplesmente `cd builddir`, e `sudo ninja uninstall`, porém
não se esqueça de atualizar seu cache dos ícones Gtk para a pasta correta em que
os ícones do nosso aplicativo foi instalado.

## Requisítos para compilação

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
  gtk4.0 gtk4.0-dev gtkmm4 gtkmm4-dev \
  libxml2 gettext \
  xdg-desktop-portal xdg-desktop-portal-gtk xdg-utils
```

### Void Linux (Nomes de pacto4e4s podem estar errado)
```
sudo xbps-install -Sy \
  noto-fonts-ttf \
  gtk4 gtk4-devel gtkmm4 gtkmm4-devel \
  base-devel meson ninja curl gcc pkgconf python3 \
  xdg-desktop-portal xdg-desktop-portal-gtk xdg-utils

```

### Gerenciadores de Janela no X11
- Você irá precisar de um compositor para gerenciar sombras, transparência, stacking e outros efeitos.
Ex: `picom`

### Fontes
- Para executar nosso aplicativo em uma lingaguem que não seja o inglês (no momento apenas português brasileiro é suportado) você provalvelmente precisará de um pacote de fontes que suportam os caracteres dessa lingua. Ex: `noto-fonts`

