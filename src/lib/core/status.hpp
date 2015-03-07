/***********************************************************************
 *
 * Filename: status.hpp
 *
 * Description: Used to tell if and how a call passed or failed.
 *
 * Copyright (C) 2015 Richard Layman, rlayman2000@yahoo.com 
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

#ifndef STATUS_HPP
#define STATUS_HPP

#include "deps.hpp"

namespace feather 
{

    enum StatusState { FAILED, PASSED, WARNING };

    struct status
    {
        status(StatusState _state=PASSED, std::string _msg="") : state(_state),msg(_msg) {};
        StatusState state;
        std::string msg;
    };

}; // namespace feather

#endif
