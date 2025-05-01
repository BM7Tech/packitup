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

project('packitup', 'c', 'cpp',
  version: '0.0.0',
  license: 'GPL-3.0-or-later',
  default_options: [
    'cpp_std=c++17'
  ]
)
install_data(
  'LICENSE',
  install_dir: get_option('prefix') / get_option('datadir') / 'licenses' / meson.project_name()
)

subdir('po')
subdir('src')

