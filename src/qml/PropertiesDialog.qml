/***********************************************************************
 *
 * Filename: PropertiesDialog.qml 
 *
 * Description: Application settings window.
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
import QtQuick.Controls 1.2
import QtQuick.Window 2.2
import feather.plugin 1.0

Window {
    id: propEditor
    visible: false
    width: 500
    height: 400
    property Properties properties: Null

    color: properties.getColor("windowBg")

    Plugins { id: pluginsModule }

    TabView {
        anchors.fill: parent
        anchors.margins: 2
 
        Tab {
            title: "UI Settings"
        }

        Tab {
            title: "Colors"

            ColorProperties {
                id: colorProperties
                anchors.fill: parent
                properties: propEditor.properties 
            }
        }

        Tab {
            title: "Plugins"

            TableView {
                id: tableView

                TableViewColumn {
                    id: nameColumn
                    role: "name"
                    title: "Name"
                    width: 100
                }

                TableViewColumn {
                    role: "description"
                    title: "Description"
                    width: 100
                }

                TableViewColumn {
                    role: "author"
                    title: "Author"
                    width: 100
                }

                model: pluginsModule

            }

        }


    
    }

}
