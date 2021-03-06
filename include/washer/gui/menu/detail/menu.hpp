/**
    @file

    Menu wrapper implementation details.

    @if license

    Copyright (C) 2012  Alexander Lamaison <awl03@doc.ic.ac.uk>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    If you modify this Program, or any covered work, by linking or
    combining it with the OpenSSL project's OpenSSL library (or a
    modified version of that library), containing parts covered by the
    terms of the OpenSSL or SSLeay licenses, the licensors of this
    Program grant you additional permission to convey the resulting work.

    @endif
*/

#ifndef WASHER_GUI_MENU_DETAIL_MENU_HPP
#define WASHER_GUI_MENU_DETAIL_MENU_HPP
#pragma once

#include <washer/gui/menu/detail/menu_win32.hpp>
                                 // create_popup_menu, create_menu, destroy_menu
#include <washer/trace.hpp>

namespace washer {
namespace gui {
namespace menu {
namespace detail {

inline void safe_destroy_menu(HMENU hmenu)
{
    try
    {
        win32::destroy_menu(hmenu);
    }
    catch (const std::exception& e)
    {
        washer::trace("Exception while destroying menu: %s") % e.what();
    }
}

inline void no_destroy_menu(HMENU)
{
}

struct menu_handle_creator
{
    HMENU operator()()
    {
        return win32::create_popup_menu();
    }
};

struct menu_bar_handle_creator
{
    HMENU operator()()
    {
        return win32::create_menu();
    }
};

}}}} // namespace washer::gui::menu::detail

#endif
