#ifndef METASHELL_DATA_SHELL_CONFIG_HPP
#define METASHELL_DATA_SHELL_CONFIG_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2017, Abel Sinkovics (abel@sinkovics.hu)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <metashell/data/shell_config_name.hpp>

#include <metashell/iface/json_writer.hpp>

#include <string>
#include <vector>

namespace metashell
{
  namespace data
  {
    class shell_config
    {
    public:
      shell_config_name name;

      std::vector<std::string> extra_clang_args;
      bool use_precompiled_headers = false;
      unsigned templight_trace_capacity;
      std::string engine = "internal";
      bool preprocessor_mode = false;
    };

    void display(iface::json_writer& out_, const shell_config& cfg_);
  }
}

#endif
