// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/forward_trace_iterator.hpp>

#include <algorithm>

using namespace metashell;

forward_trace_iterator::forward_trace_iterator() : _finished(true) {}

forward_trace_iterator::forward_trace_iterator(
    const data::metaprogram& mp_, const boost::optional<int>& max_depth_)
  : _finished(false),
    _max_depth(max_depth_),
    _mp(&mp_),
    _discovered(mp_.get_state().discovered)
{
  visit(_mp->get_current_edge(), 0);
}

void forward_trace_iterator::visit(
    const data::metaprogram::optional_edge_descriptor& edge_, int depth_)
{
  data::metaprogram::vertex_descriptor vertex =
      edge_ ? _mp->get_target(*edge_) : _mp->get_root_vertex();

  _current = data::call_graph_node(
      edge_ ? _mp->to_frame(*edge_) : _mp->get_root_frame(), depth_,
      (_discovered[vertex] || (_max_depth && *_max_depth <= depth_)) ?
          0 :
          _mp->get_enabled_out_degree(vertex));

  if (!_discovered[vertex])
  {
    if (_mp->get_mode() != data::metaprogram::mode_t::full)
    {
      _discovered[vertex] = true;
    }

    if (!_max_depth || *_max_depth > depth_)
    {
      auto edges = _mp->get_filtered_out_edges(vertex);
      for (const data::metaprogram::edge_descriptor& out_edge : edges)
      {
        _to_visit.push(std::make_tuple(out_edge, depth_ + 1));
      }
    }
  }
}

forward_trace_iterator& forward_trace_iterator::operator++()
{
  if (_to_visit.empty())
  {
    _finished = true;
  }
  else
  {
    data::metaprogram::optional_edge_descriptor edge;
    int depth;
    std::tie(edge, depth) = _to_visit.top();
    _to_visit.pop();
    visit(edge, depth);
  }
  return *this;
}

const data::call_graph_node& forward_trace_iterator::operator*() const
{
  return _current;
}

bool forward_trace_iterator::operator==(const forward_trace_iterator& i_) const
{
  return _finished == i_._finished;
}
