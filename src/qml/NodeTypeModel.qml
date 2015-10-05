/***********************************************************************
 *
 * Filename: NodeTypeModel.qml 
 *
 * Description: Holds all the type of all the different nodes that can be stored in the scenegraph. This is not to be the specific node - this model will hold a type like Light but many different light nodes can be under this type. 
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

import QtQuick 2.3
import feather.node 1.0

ListModel {
    id: typeModel
    ListElement { text: "Empty"; type: Node.Empty }
    ListElement { text: "Camera"; type: Node.Camera}
    ListElement { text: "Light"; type: Node.Light}

}