// =====================================================================================
// 
//       Filename:  main.cpp
// 
//    Description:  This plugin holds the POLYGON_PLANE, POLYGON_CUBE scenegraph nodes.
// 
//        Version:  1.0
//        Created:  11/03/2014 04:44:11 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Richard Layman (), rlayman2000@yahoo.com
//        Company:  
// 
// =====================================================================================
#include "deps.hpp"
#include "pluginmanager.hpp"
#include "field.hpp"
#include "node.hpp"
#include "parameter.hpp"
#include "command.hpp"

/*
 ***************************************
 *              PLUGIN INIT            *
 ***************************************
 *
 * PLUGIN_INIT()
 * 
 * This macro is needed to make it callable
 * by the core.
 * Every plugin needs it and it's always
 * the same so you should never have to
 * edit the below code.
 *
 */

#ifdef __cplusplus
extern "C" {
#endif
    C_PLUGIN_WRAPPER()
#ifdef __cplusplus
}
#endif

using namespace feather;

#define POLYGON_PLANE 325
#define POLYGON_CUBE 326

// THIS NEEDS TO BE FIXED
SET_NODE_ID(POLYGON_PLANE,325)
//SET_NODE_ID(POLYGON_CUBE,326)

/*
 ***************************************
 *              PLUGIN INIT            *
 ***************************************
 *
 * This macro handles some misc bindings
 * between the plugin and the pluginmanager.
 */ 
PLUGIN_INIT()

// get the field
feather::field::FieldBase* get_field(int nid, int fid, PluginNodeFields* fields) {
    switch(nid) {
        case POLYGON_PLANE:
            return find_field<POLYGON_PLANE,5>::exec(fid,fields);
        case POLYGON_CUBE:
            return find_field<POLYGON_CUBE,5>::exec(fid,fields);
        default:
            return NULL;
    }
};



/*
 ***************************************
 *            POLYGON PLANE            *
 ***************************************
*/

namespace feather {

    /*
     ***************************************
     *              NODE FIELDS            *
     ***************************************
     *
     * You need to make a struct that uses
     * PluginNodeFields as it's base class.
     * This struct will hold all input and
     * outout fields. These fields will be
     * used to connect nodes together 
     */
    struct PolygonPlaneFields : public PluginNodeFields
    {
        field::Field<int> *subX;
        field::Field<int> *subY;
    };

} // namespace feather


/*
 ***************************************
 *          ADD_FIELD_TO_NODE          *
 ***************************************
 * ADD_FIELD_TO_NODE(node,struct,attr,key)
 * node = Enum of the node that the field
 *      will be put into.
 * struct = Name of the struct that holds
 *      the fields.
 * attr = Name of the field attr in the
 *      struct.
 * key = Starting from 1, the number of
 *      the field in the attribute.
 *
 * The macro will tie the field to the node
 * so that it's value can be used by the
 * node's do_it command.
 */
ADD_FIELD_TO_NODE(POLYGON_PLANE,PolygonPlaneFields,subX,1)
ADD_FIELD_TO_NODE(POLYGON_PLANE,PolygonPlaneFields,subY,2)


namespace feather
{
    // POLYGON PLANE NODE SETUP

    // do_it
    template <> status node_do_it<POLYGON_PLANE>(PluginNodeFields* fields) {
        PolygonPlaneFields* plane = static_cast<PolygonPlaneFields*>(fields);
        std::cout << "plane: subX:" << plane->subX << std::endl;

        return status();
    };

} // namespace feather

/*
 ***************************************
 *              NODE_INIT              *
 ***************************************
 * NODE_INIT(node)
 * node = Enum of the node that the field
 *      will be put into.
 *
 * This macro will setup the node so that
 * it can be called by the scenegraph.
 */
NODE_INIT(POLYGON_PLANE)



/*
 ***************************************
 *            POLYGON CUBE             *
 ***************************************
*/

namespace feather
{
    struct PolygonCubeFields : public PluginNodeFields
    {
        field::Field<int> *subX;
        field::Field<int> *subY;
        field::Field<int> *subZ;
    };

} // namespace feather

ADD_FIELD_TO_NODE(POLYGON_CUBE,PolygonCubeFields,subX,1)
ADD_FIELD_TO_NODE(POLYGON_CUBE,PolygonCubeFields,subY,2)
ADD_FIELD_TO_NODE(POLYGON_CUBE,PolygonCubeFields,subZ,3)

namespace feather
{

    // do_it
    template <> status node_do_it<POLYGON_CUBE>(PluginNodeFields* fields) {
        PolygonCubeFields* cube = static_cast<PolygonCubeFields*>(fields);
        std::cout << "cube: subX:" << cube->subX << std::endl;
        return status();
    };

} // namespace feather

NODE_INIT(POLYGON_CUBE)


/*
 ***************************************
 *              COMMANDS               *
 ***************************************
 * Put all your commands here.
 * Commands are functions that can
 * be called from the interface and
 * are only called once. An example
 * of this would be something like
 * render() or import_obj().
 * You will still need to edit the
 * qml interface to call an commands
 * that you've created.
 */

namespace feather
{
    namespace command
    {
        /*
         * Here you will add enums for every
         * command that you need to call.
         * The first enum must always be N.
         * example:
         * enum Command { N=0, IMPORT_OBJ, EXPORT_OBJ };
         */
        enum Command { N=0, IMPORT_OBJ, EXPORT_OBJ };

        /*
         * Here you will add all the commands that
         * will be linked the the above enums.
         * You need to have a function for each
         * enum above and they need to be formated
         * this way:
         *      status [name](parameter::ParameterList params)
         *      {
         *              ...
         *      };
         * example:
         *      status import_obj(parameter::ParameterList params)
         *      {
         *              // code to import obj file
         *      };
         */
        status import_obj(parameter::ParameterList params) {
            std::string filename = params.getParameterValue<std::string>("filename");
            bool selection = params.getParameterValue<bool>("selection");

            std::cout << "running import_obj command\nfilename: " << filename << "\n\tselection: " << selection << std::endl;
            return status();
        };

        status export_obj(parameter::ParameterList params) {
            std::cout << "running export_obj command" << std::endl;
            return status();
        };

    } // namespace command

} // namespace feather

/*
 * ADD_COMMAND([name],[enum],[function])
 * Add all the commands that can be called from
 * the qml interface.
 *
 * example:
 * ADD_COMMAND("import_obj",IMPORT_OBJ,import_obj)
 */
ADD_COMMAND("import_obj",IMPORT_OBJ,import_obj)
ADD_COMMAND("export_obj",EXPORT_OBJ,export_obj)


/*
 * INIT_COMMAND_CALLS([enum])
 * Make the command callable by the core.
 * This make takes the last enum of the
 * feather::command::Command enum.
 *
 * example:
 * INIT_COMMAND_CALLS(EXPORT_OBJ)
 */
INIT_COMMAND_CALLS(EXPORT_OBJ)
