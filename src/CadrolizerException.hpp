/* CadrolizerException.hpp -- Cadrolizer Exception class.
 *
 * Copyright (C) 2015 Artyom V. Poptsov <poptsov.artyom@gmail.com>
 *
 * This file is part of Cadrolizer.
 *
 * Cadrolizer is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Cadrolizer is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Cadrolizer.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __CADROLIZER_EXCEPTION_HPP__
#define __CADROLIZER_EXCEPTION_HPP__

#include <exception>

using namespace std;

class CadrolizerException: public exception
{
 private:
        const char* m_message;

 public:
        CadrolizerException(const char* message)
                : m_message(message) {}

        const char* what() const throw() {
                return m_message;
        }
};

#endif  /* ifndef __CADROLIZER_EXCEPTION_HPP__ */

/* CadrolizerException.hpp ends here. */
