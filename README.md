# `splatdotink`

`splatdotink` is a fairly simple library, allowing a developer to access data
from `splatoon2.ink` in a relatively painless manner.

The library depends upon [`restclient-cpp`](https://github.com/mrtazz/restclient-cpp) and [`nlohmann::json`](https://github.com/nlohmann/json). Install them either through your preferred package manager or their repositories. Just make sure that they're visible to `pkg-config`.

## Building the library

### From the repository:

It's the age-old autotools dance: `autoreconf -vsi && ./configure && make`.

### From a release tarball:

Similar to building from the repository itself; just drop the call to `autoreconf`.

## LICENSE

Copyright (C) 2020 Alexander Bluefall.

splatdotink is free software: you can redistribute it and/or modify it under the
terms of the GNU Lesser General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

splatdotink is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along
with splatdotink. If not, see <https://www.gnu.org/licenses/>.
