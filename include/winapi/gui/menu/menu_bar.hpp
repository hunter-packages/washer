/**
    @file

    Windows menu bar HMENU wrapper.

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

#ifndef WINAPI_GUI_MENU_MENU_BAR_HPP
#define WINAPI_GUI_MENU_MENU_BAR_HPP
#pragma once

#include <winapi/gui/menu/detail/menu.hpp> // menu_handle, safe_destroy_menu
#include <winapi/gui/menu/detail/menu_common_core.hpp>
#include <winapi/gui/menu/detail/menu_item_iterator.hpp>
#include <winapi/gui/menu/detail/menu_win32.hpp> // create_menu
#include <winapi/gui/menu/menu_bar_item.hpp>

#include <Windows.h> // MENUITEMINFO

namespace winapi {
namespace gui {

template<typename>
class window;

namespace menu {

/**
 * Main application window menu bar.
 */
class menu_bar : private detail::menu_common_core<menu_bar_item>
{
#if defined (_MSC_VER) && (_MSC_VER > 1400)
    template<typename>
    friend class window;
#else
    // HACK to workaround VC8 (2005) and presumably earlier that can't
    // befriend a template class in a parent namespace.
    // See: http://stackoverflow.com/q/10694416/67013
    friend class window<char>;
    friend class window<wchar_t>;
#endif

    typedef detail::menu_common_core<menu_bar_item> core;
    typedef int iterator_type;

public:

    menu_bar()
        :
    core(
        detail::menu_handle(
            detail::win32::create_menu(), detail::safe_destroy_menu))
    {}

    /**
     * Returns the number of items in the menu.
     */
    using core::size;

    /**
     * Appends an item onto the end of the menu.
     */
    using core::append;

    /**
     * Insert an item into the menu at the given iterator position.
     *
     * Shuffles existing items along.
     */
    using core::insert;

    using core::begin;
    using core::end;

    /**
     * Test if objects wrap the same Win32 menu.
     */
    bool operator==(const menu_bar& other) const
    {
        const core& other_core = other;
        const core& this_core = *this;
        return this_core == other_core;
    }

    /**
     * Tests if the underlying Win32 menu still exists.
     *
     * Windows take over the lifetime of a menu and destroy the menu when they
     * themselves are destroyed.  Therefore it is possible for this menu to
     * become invalid outside the control of this wrapper.
     */
    using core::valid;

private:

    /** To allow window<T>::menu to access raw HWND. */
    using core::handle;
};

}}} // namespace winapi::gui::menu

#endif
