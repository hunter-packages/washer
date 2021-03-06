/**
    @file

    Tests for menu wrappers.

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

#include "button_test_visitors.hpp" // string_button_test
#include "item_test_visitors.hpp" // id_test
#include "menu_fixtures.hpp"
#include "wchar_output.hpp" // wchar_t test output

#include <washer/gui/menu/item/command_item_description.hpp>
#include <washer/gui/menu/item/sub_menu_item_description.hpp>
#include <washer/gui/menu/menu.hpp> // test subject

#include <boost/foreach.hpp> // BOOST_FOREACH
#include <boost/test/unit_test.hpp>

using namespace washer::gui::menu;
using namespace washer::test;

namespace {

    // redefine MIIM_STRING so that tests compile even with WINVER < 0x0500
    const UINT miim_string = 0x00000040;
}

/**
 * These tests are not especially interested in the details of the items in
 * the menu (except where it may affect the menu's behaviour).  Instead they
 * just care about testing the menu and menu_bar classes.  The details of the
 * items are tested by the item* test suites.
 */
BOOST_AUTO_TEST_SUITE(menu_tests)

/**
 * Create an empty menu.
 */
BOOST_AUTO_TEST_CASE_TEMPLATE( create_empty_menu, F, menu_ownership_fixtures )
{
    menu m;

    BOOST_CHECK(m.begin() == m.end());
    BOOST_CHECK_EQUAL(m.size(), 0U);
    BOOST_CHECK_THROW(m[0], std::runtime_error);
    BOOST_FOREACH(item i, m)
    {
        (void)i;
        BOOST_FAIL("Empty menu should not iterate");
    }

    F::do_ownership_test(m);
}


/**
 * Test menu copying.
 */
BOOST_AUTO_TEST_CASE_TEMPLATE( menu_copy, F, menu_ownership_fixtures )
{
    menu m;

    // Copy constructor
    menu n = m;
    BOOST_CHECK(m == n);

    // Copy assignment
    menu p;
    n = p;
    BOOST_CHECK(n == p);
    BOOST_CHECK(n != m);
}

/**
 * Create an empty menu bar.
 */
BOOST_AUTO_TEST_CASE_TEMPLATE( empty_menu_bar, F, menu_ownership_fixtures )
{
    menu_bar m;

    BOOST_CHECK(m.begin() == m.end());
    BOOST_CHECK_EQUAL(m.size(), 0U);
    BOOST_CHECK_THROW(m[0], std::runtime_error);
    BOOST_FOREACH(item i, m)
    {
        (void)i;
        BOOST_FAIL("Empty menu should not iterate");
    }

    F::do_ownership_test(m);
}

/**
 * Wrap an empty menu(bar).
 */
BOOST_AUTO_TEST_CASE_TEMPLATE( existing_empty_menu, F, menu_fixtures )
{
    F::test_menu t = F::create_menu_to_test();
    F::menu_type m = t.menu();

    BOOST_CHECK(m.begin() == m.end());
    BOOST_CHECK_EQUAL(m.size(), 0U);
    BOOST_CHECK_THROW(m[0], std::runtime_error);
    BOOST_FOREACH(item i, m)
    {
        (void)i;
        BOOST_FAIL("Empty menu should not iterate");
    }

    F::do_ownership_test(m);
}

/**
 * Insert a non-popup (command) item in a menu(bar).
 *
 * Tests that the insert() method creates an item in the menu.
 */
BOOST_AUTO_TEST_CASE_TEMPLATE( create_command, F, menu_fixtures )
{
    F::test_menu t = F::create_menu_to_test();
    F::menu_type m = t.menu();

    m.insert(
        command_item_description(string_button_description(L"Command"), 42));

    BOOST_CHECK(m.begin() != m.end());
    BOOST_CHECK_EQUAL(m.size(), 1U);
    BOOST_CHECK_NO_THROW(m[0]);
    BOOST_CHECK_THROW(m[1], std::runtime_error);

    F::do_ownership_test(m);
}

/**
 * Wrap a menu(bar) with an existing non-popup (command) item.
 */
BOOST_AUTO_TEST_CASE_TEMPLATE( existing_command, F, menu_fixtures )
{
    F::test_menu t = F::create_menu_to_test();
    F::menu_type m = t.menu();

    washer::test::do_insertion(
        t.handle().get(), L"C", 42, NULL, MIIM_ID | miim_string, MFT_STRING);

    BOOST_CHECK(m.begin() != m.end());
    BOOST_CHECK_EQUAL(m.size(), 1U);
    BOOST_CHECK_NO_THROW(m[0]);
    BOOST_CHECK_THROW(m[1], std::runtime_error);

    F::do_ownership_test(m);
}

/**
 * Create a popup item in a menu(bar).
 *
 * Tests that the insert() method creates an item in the menu and that the
 * submenu doesn't affect iteration in the parent menu.
 */
BOOST_AUTO_TEST_CASE_TEMPLATE( create_submenu, F, menu_fixtures )
{
    F::test_menu t = F::create_menu_to_test();
    F::menu_type m = t.menu();

    menu sub;
    sub.insert(command_item_description(string_button_description(L"Boo"), 1));

    m.insert(sub_menu_item_description(string_button_description(L"Pop"), sub));

    BOOST_CHECK(m.begin() != m.end());
    BOOST_CHECK_EQUAL(m.size(), 1U);
    BOOST_CHECK_NO_THROW(m[0]);
    BOOST_CHECK_THROW(m[1], std::runtime_error);

    F::do_ownership_test(m);
}

/**
 * Test iterator copying.
 */
BOOST_AUTO_TEST_CASE_TEMPLATE(
    iterator_copy, F, menu_with_handle_creator_fixtures )
{
    F::test_menu t = F::create_menu_to_test();
    F::menu_type m = t.menu();

    // Copy constructor
    F::menu_type::iterator start = m.begin();

    // Copy conversion constructor
    F::menu_type::const_iterator const_start = m.begin();

    BOOST_CHECK(start == const_start);

    // Copy assignment
    start = m.begin();

    // Copy conversion assignment
    const_start = m.begin();

    BOOST_CHECK(start == const_start);
}

/**
 * Test iterator forward-traversal.
 */
BOOST_AUTO_TEST_CASE_TEMPLATE(
    iterator_increment, F, menu_with_handle_creator_fixtures )
{
    F::test_menu t = F::create_menu_to_test();
    F::menu_type m = t.menu();

    do_insertion(
        t.handle().get(), L"C", 42, NULL, MIIM_ID | miim_string, MFT_STRING);
    do_insertion(
        t.handle().get(), L"D", 43, NULL, MIIM_ID | miim_string, MFT_STRING);

    F::menu_type::iterator it = m.begin();

    BOOST_CHECK(it != m.end());

    it++;

    BOOST_CHECK(it != m.begin());
    BOOST_CHECK(it != m.end());

    it++;

    BOOST_CHECK(it == m.end());

    BOOST_CHECK_THROW(++it, std::runtime_error);
}

/**
 * Test iterator backward-traversal.
 */
BOOST_AUTO_TEST_CASE_TEMPLATE(
    iterator_decrement, F, menu_with_handle_creator_fixtures )
{
    F::test_menu t = F::create_menu_to_test();
    F::menu_type m = t.menu();

    do_insertion(
        t.handle().get(), L"C", 42, NULL, MIIM_ID | miim_string, MFT_STRING);
    do_insertion(
        t.handle().get(), L"D", 43, NULL, MIIM_ID | miim_string, MFT_STRING);

    F::menu_type::iterator it = m.end();

    BOOST_CHECK(it != m.begin());

    it--;

    BOOST_CHECK(it != m.begin());
    BOOST_CHECK(it != m.end());

    it--;

    BOOST_CHECK(it == m.begin());

    BOOST_CHECK_THROW(--it, std::runtime_error);
}

/**
 * Test iterator forward skip.
 */
BOOST_AUTO_TEST_CASE_TEMPLATE(
    iterator_advance, F, menu_with_handle_creator_fixtures )
{
    F::test_menu t = F::create_menu_to_test();
    F::menu_type m = t.menu();

    do_insertion(
        t.handle().get(), L"C", 42, NULL, MIIM_ID | miim_string, MFT_STRING);
    do_insertion(
        t.handle().get(), L"D", 43, NULL, MIIM_ID | miim_string, MFT_STRING);

    F::menu_type::iterator it = m.begin();

    BOOST_CHECK(it != m.end());

    it = it + 2;

    BOOST_CHECK(it == m.end());

    BOOST_CHECK_THROW(it + 1, std::runtime_error);
    BOOST_CHECK_THROW(it - 3, std::runtime_error);
}

/**
 * Test iterator reverse skip.
 */
BOOST_AUTO_TEST_CASE_TEMPLATE(
    iterator_reverse, F, menu_with_handle_creator_fixtures )
{
    F::test_menu t = F::create_menu_to_test();
    F::menu_type m = t.menu();

    do_insertion(
        t.handle().get(), L"C", 42, NULL, MIIM_ID | miim_string, MFT_STRING);
    do_insertion(
        t.handle().get(), L"D", 43, NULL, MIIM_ID | miim_string, MFT_STRING);

    F::menu_type::iterator it = m.end();

    BOOST_CHECK(it != m.begin());

    it = it - 2;

    BOOST_CHECK(it == m.begin());

    BOOST_CHECK_THROW(it - 1, std::runtime_error);
    BOOST_CHECK_THROW(it + 3, std::runtime_error);
}

/**
 * Test iterator forward skip.
 */
BOOST_AUTO_TEST_CASE_TEMPLATE(
    iterator_distance, F, menu_with_handle_creator_fixtures )
{
    F::test_menu t = F::create_menu_to_test();
    F::menu_type m = t.menu();

    do_insertion(
        t.handle().get(), L"C", 42, NULL, MIIM_ID | miim_string, MFT_STRING);
    do_insertion(
        t.handle().get(), L"D", 43, NULL, MIIM_ID | miim_string, MFT_STRING);

    F::menu_type::iterator it = m.begin();

    BOOST_CHECK_EQUAL(m.begin() - m.begin(), 0);
    BOOST_CHECK_EQUAL(m.begin() - m.end(), -2);
    BOOST_CHECK_EQUAL(m.end() - m.begin(), 2);
}

/**
 * Test menu's default item setting and finding.
 */
BOOST_AUTO_TEST_CASE_TEMPLATE( default_item, F, menu_fixtures )
{
    F::test_menu t = F::create_menu_to_test();
    F::menu_type m = t.menu();

    m.insert(command_item_description(string_button_description(L"First"), 1));
    menu sub;
    sub.insert(command_item_description(string_button_description(L"Boo"), 1));

    m.insert(
        sub_menu_item_description(string_button_description(L"Second"), sub));
    m.insert(command_item_description(string_button_description(L"Third"), 1));

    BOOST_CHECK(m.default_item() == m.end());

    m.default_item(m.begin());
    BOOST_CHECK(m.default_item() == m.begin());

    m.default_item(m.begin() + 1);
    BOOST_CHECK(m.default_item() == m.begin() + 1);

    m.default_item(m.begin() + 2);
    BOOST_CHECK(m.default_item() == m.begin() + 2);

    F::do_ownership_test(m);
}

namespace {

    template<typename M>
    void set_up_test_menu(M& m)
    {
        m.insert(
            command_item_description(
                string_button_description(L"Command1"), 1));
        m.insert(
            command_item_description(
                string_button_description(L"Command2"), 2));
        m.insert(
            command_item_description(
                string_button_description(L"Command3"), 3));
    }

}

/**
 * Insert an item at the beginning of a menu.
 *
 * Should shuffle other items along.
 */
BOOST_AUTO_TEST_CASE_TEMPLATE(
    insert_beginning, F, menu_with_handle_creator_fixtures )
{
    F::test_menu t = F::create_menu_to_test();
    F::menu_type m = t.menu();

    set_up_test_menu(m);

    m.insert(
        command_item_description(string_button_description(L"Interloper"), 42),
        m.begin());

    BOOST_CHECK_EQUAL(m.size(), 4U);
    m[0].accept(id_test(42));
    m[1].accept(id_test(1));
    m[2].accept(id_test(2));
    m[3].accept(id_test(3));
}

/**
 * Insert an item in the middle of a menu.
 *
 * Should shuffle later items along.
 */
BOOST_AUTO_TEST_CASE_TEMPLATE(
    insert_middle, F, menu_with_handle_creator_fixtures )
{
    F::test_menu t = F::create_menu_to_test();
    F::menu_type m = t.menu();

    set_up_test_menu(m);

    m.insert(
        command_item_description(string_button_description(L"Interloper"), 42),
        m.begin() + 2);

    BOOST_CHECK_EQUAL(m.size(), 4U);
    m[0].accept(id_test(1));
    m[1].accept(id_test(2));
    m[2].accept(id_test(42));
    m[3].accept(id_test(3));
}

/**
 * Insert an item explicitly at the end of a menu.
 *
 * Should shuffle other items along.
 */
BOOST_AUTO_TEST_CASE_TEMPLATE(
    insert_end, F, menu_with_handle_creator_fixtures )
{
    F::test_menu t = F::create_menu_to_test();
    F::menu_type m = t.menu();

    set_up_test_menu(m);

    m.insert(
        command_item_description(string_button_description(L"Interloper"), 42),
        m.end());

    BOOST_CHECK_EQUAL(m.size(), 4U);
    m[0].accept(id_test(1));
    m[1].accept(id_test(2));
    m[2].accept(id_test(3));
    m[3].accept(id_test(42));
}


/**
 * Fail to find items in empty menu.
 */
BOOST_AUTO_TEST_CASE_TEMPLATE(
    find_empty, F, menu_with_handle_creator_fixtures )
{
    F::test_menu t = F::create_menu_to_test();
    F::menu_type m = t.menu();

    BOOST_CHECK(find_first_item_with_id(m.begin(), m.end(), 1) == m.end());
}

/**
 * Find items in menu.
 */
BOOST_AUTO_TEST_CASE_TEMPLATE( find, F, menu_with_handle_creator_fixtures )
{
    F::test_menu t = F::create_menu_to_test();
    F::menu_type m = t.menu();

    set_up_test_menu(m);

    find_first_item_with_id(m.begin(), m.end(), 1)->accept(id_test(1));
    find_first_item_with_id(m.begin(), m.end(), 2)->accept(id_test(2));
    find_first_item_with_id(m.begin(), m.end(), 3)->accept(id_test(3));

    // No-existent item should not be found
    BOOST_CHECK(find_first_item_with_id(m.begin(), m.end(), 42) == m.end());

    // Items outside the range should not be found
    BOOST_CHECK(find_first_item_with_id(m.begin() + 1, m.end(), 1) == m.end());
    BOOST_CHECK(find_first_item_with_id(m.begin() + 2, m.end(), 2) == m.end());
    BOOST_CHECK(find_first_item_with_id(m.begin() + 3, m.end(), 3) == m.end());
    BOOST_CHECK(
        find_first_item_with_id(m.begin(), m.end() - 1, 3) == m.end() - 1);
    BOOST_CHECK(
        find_first_item_with_id(m.begin(), m.end() - 2, 2) == m.end() - 2);
    BOOST_CHECK(
        find_first_item_with_id(m.begin(), m.end() - 3, 3) == m.end() - 3);
}

/**
 * Find items in menu.
 */
BOOST_AUTO_TEST_CASE_TEMPLATE(
    find_despite_duplicates, F, menu_with_handle_creator_fixtures )
{
    F::test_menu t = F::create_menu_to_test();
    F::menu_type m = t.menu();

    set_up_test_menu(m);

    m.insert(
        command_item_description(string_button_description(L"Interloper"), 2));

    typename F::menu_type::iterator first = find_first_item_with_id(
        m.begin(), m.end(), 2);

    first->accept(make_button_test(string_button_test(L"Command2")));

    typename F::menu_type::iterator second = find_first_item_with_id(
        first + 1, m.end(), 2);

    second->accept(make_button_test(string_button_test(L"Interloper")));

    BOOST_CHECK(first != second);
}

BOOST_AUTO_TEST_SUITE_END()
