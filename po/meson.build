# PackItUP! Never run out of beer again.
# Copyright (C) 2025  edu-bm7 <edubm7@bm7.tech>
# 
# This file is part of PackItUP!.
# 
# PackItUP! is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# PackItUP! is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with PackItUP!. If not, see <https://www.gnu.org/licenses/>.

i18n = import('i18n')
locale_dir = get_option('prefix') / get_option('localedir')
# define GETTEXT_PACKAGE
add_project_arguments('-DGETTEXT_PACKAGE="packitup"', '-DLOCALEDIR="@0@"'.format(locale_dir), language:'cpp')
i18n.gettext(meson.project_name(),
  args: [
    '--from-code=UTF-8',
    '--keyword=_',  # Ensure it catches gettext() calls
  ],
  install: true,
  install_dir: locale_dir
)
