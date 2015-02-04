// =====================================================================================
// 
//       Filename:  scenegraph.hpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  07/08/2014 05:02:24 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Richard Layman (), rlayman2000@yahoo.com
//        Company:  
// 
// =====================================================================================
#ifndef SCENEGRAPH_HPP
#define SCENEGRAPH_HPP

#include "deps.hpp"
#include "types.hpp"
#include "node.hpp"
#include "field.hpp"
#include "singleton.hpp"
#include "selection.hpp"
#include "data.hpp"
#include "object.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include "null.hpp"
#include "pluginmanager.hpp"
#include "state.hpp"

namespace feather
{

    /*
     * The SceneGraph holds the node tree. There is no node data stored in the
     * SceneGraph; only the node type and id. The id is mapped to the data so 
     * that the scenegraph can lookup it's values and then view or modify them
     * depending on the command. The scenegraph will manage when and in what order
     * the nodes will get called in. The scenegraph will also call the functions
     * for various stages of the update.
     *
     * When working with nodes 3 things are used - the scenegraph, data, and selection.
     * The scenegraph manages the order the nodes are updated.
     * The data holds each nodes value.
     * The selection tells the components and indics of what is to be worked on.
     * 
     * The scenegraph will call the node's doit() which will take the node's data
     * and work on it based on the selection.
     * A selection is not tied to any node. When a user clicks on a node's component,
     * a new SelectionState is added to the SelectionManager and is setup based on that node's
     * data. As the more vertex's are selected, the SelectionState is modified to match it.
     * This SelectionState is also used to draw the selections during the viewport's update.
     *
     * When a new node is added to the scenegraph a scenegraph vertex and datablock is created.
     * The datablock location is kept in sync with the vertex number by the datamanager.
     */

    static state::FState cstate;
    static FSceneGraph sg;

    static PluginManager plugins;

    static std::vector<FNodeDescriptor> node_selection;


    namespace scenegraph
    {
        status load_plugins() {
            return plugins.load_plugins();
        };
        
        /* Add Node
         * This function is called during specialization of nodes when
         * a new node is added to the scenegraph. It's called by add_node_to_sg
         * and is specialized by each node.
         */
        /* 
        template <int _Type, int _Node>
        static status add_node(int id) { return status(FAILED,"no matching node for add_node"); };
        */

        int add_node(int t, int n) {
            std::cout << "add node: " << n << ", type: " << t << std::endl;

            // TODO
            // Here I need to ask the plugin manager if the node exists
 
            FNodeDescriptor node = boost::add_vertex(sg);
            sg[node].type = static_cast<feather::node::Type>(t);
            sg[node].id = n;
            plugins.create_fields(n,sg[node].fields);
            node_selection.push_back(n); 

            // Return the node number
            return static_cast<int>(node);
        };

        field::FieldBase* get_fieldBase(int uid, int node, int field) {
            return plugins.get_fieldBase(uid,node,field,sg[uid].fields); 
        };


        /* Add Node to SceneGraph
         * This is the recursive function that will keep going till
         * finds a match for the node or fail.
         * If a match is found the add_node function is called which
         * is specialized by each node.
         */
        /*
        template <int _Type, int _Node>
            struct add_node_to_sg {
                static status exec(int node)
                {
                    if(node==_Node)
                        return add_node<_Type,_Node>(node);
                    else
                        return add_node_to_sg<_Type,_Node-1>::exec(node);
                };
            };

        template <int _Type>
            struct add_node_to_sg<_Type,0> { static status exec(int node) { return status(FAILED, "could not add unknown node to scenegraph"); }; };
        */


        /* DoIt
         * This gets called during the scenegraph update and
         * currently is used to display the data but this may
         * change.
         */
        /* 
        template <int _Type, int _Node>
            struct do_it {
                static status exec(FNodeDescriptor node)
                {
                    return plugins.do_it(node);
                    // used this if you need to call a specific node
                    //return plugins.do_it(325);
                    // old test
                    //return do_it<_Type,_Node-1>::exec(node);
                };
            };

        template <int _Type> struct do_it<_Type,-1> { static status exec(FNodeDescriptor node) { return status(FAILED, "no node do_it found"); }; };
        */

    } // namespace scenegraph

    class node_visitor : public boost::default_bfs_visitor
    {
        public:
            //typedef boost::on_finish_vertex event_filter;

            /*
             *  The order of update for a graph like this:
             *      A
             *     / \
             *  ->B-->C
             *  |  \ /
             *  ----D
             *
             *  would be this: 
             *      init vertex A // do any setup that is needed for the node
             *      init vertex B
             *      init vertex C
             *      init vertex D
             *      discover vertex A // discover will call the parent and it's children nodes
             *      examine edge A->B // send the field value to the target node's fields - this will probably not be needed send the target will just hold a pointer to the source field  
             *      tree edge A->B
             *      discover vertex B
             *      examine edge A->C
             *      tree edge A->C
             *      discover vertex C
             *      finish vertex A // call the nodes do_it() 
             *      examine edge A->B
             *      examine edge B->C
             *      examine edge B->D
             *      tree edge B->D
             *      discover vertex D
             *      examine edge D->B
             *      finish vertex B
             *      examine edge A->C
             *      examine edge B->C
             *      examine edge C->D
             *      finish vertex C
             *      examine edge B->D
             *      examine edge C->D
             *      examine edge D->B
             *      finish vertex D
             */

            /*
             * The way node fields are handled are like this:
             *  ________                ________
             * | NODE A |  bgl edge    | NODE B |
             * |        |------------->|        |
             * |________|              |________|
             *
             * When a node is adjusted it will call Boost::BGL and tell
             * it that it has changed and to start the update at that node.
             * BGL will first call all the node::init() functions for
             * every node connected to the start node.
             * Then the graph walking begins.
             *      1: the node::discover() gets called for parent node followed by:
             *              node::examine() [parent to child]
             *              node::tree() [parent to child]
             *              node::discover() [child node]
             *              {THE ABOVE THREE STEPS ARE REPEATED FOR EACH CHILD}
             *      2: the node::do_it() for the parent is called since everything is safe for it to update
             *      3: move on to the next child node and repeat steps 1 and 2
             *
             * The opengl update is done the same except it always starts at
             * the root node and node::finish() is where the specialized draw()
             * function will be found.
             *
             * To handle these different states the scenegraph holds a State value
             * that will tell wheither the graph is in UPDATE, DRAW, or RENDER.
             *
             */

            /*
             * Update Process:
             */

            // VERTEX


            // Init Vertex
            /*
             * This gets called by all vertex at the beginning of an update.
             */
            template < typename Vertex, typename Graph >
                void initialize_vertex(Vertex u, const Graph & g) const
                {
                    std::cout << "init vertex " << sg[u].id << std::endl;
                    if(cstate.sgmode==state::DrawGL)
                            plugins.draw_gl(sg[u].id);
                }

            // Start Vertex
            /*
             * This should be called by the source vertex at the start but I haven't seen it yet.
             */
            template < typename Vertex, typename Graph >
                void start_vertex(Vertex u, const Graph & g) const
                {
                    std::cout << "start vertex " << sg[u].id << std::endl;
                }

            // Discover Vertex
            /*
             * This gets called each time a vertex is encountered
             */
            template < typename Vertex, typename Graph >
                void discover_vertex(Vertex u, const Graph & g) const
                {
                    std::cout << "discover vertex:" << u << " id:" << sg[u].id << std::endl;
                    //scenegraph::do_it<node::N>::exec(u);

                    if(cstate.sgmode==state::DoIt)
                    {
                        status p = plugins.do_it(sg[u].id,sg[u].fields);
                        if(!p.state)
                            std::cout << "NODE FAILED! : \"" << p.msg << "\"\n";
                    }
                   
                    /* 
                    switch(cstate.sgmode) {
                        case state::DoIt:
                            plugins.do_it(sg[u].id);
                            //status p = plugins.do_it(sg[u].id);
                            //if(!p.state)
                            //    std::cout << "NODE FAILED! : \"" << p.msg << "\"\n";
                            break;
                        case state::DrawGL:
                            std::cout << "draw_gl " << sg[u].id << std::endl;
                            plugins.draw_gl(sg[u].id);
                            break;
                        default:
                            break;
                    }
                    */

                    // This might still come in handy later on
                    /*
                    switch(sg[u].type)
                    {
                        case node::Null:
                            scenegraph::do_it<node::Null,null::N>::exec(u);
                            break;
                        case node::Camera:
                            scenegraph::do_it<node::Camera,camera::N>::exec(u);
                            break;
                        case node::Light:
                            scenegraph::do_it<node::Light,light::N>::exec(u);
                            break;
                        case node::Texture:
                            scenegraph::do_it<node::Texture,texture::N>::exec(u);
                            break;
                        case node::Shader:
                            scenegraph::do_it<node::Shader,shader::N>::exec(u);
                            break;
                        case node::Object:
                            scenegraph::do_it<node::Object,object::N>::exec(u);
                            break;
                        default:
                            break;
                    }
                    */
                }

            // Finish Vertex
            /*
             * This gets called each time a vertex's child vertex are finished
             */
            template < typename Vertex, typename Graph >
                void finish_vertex(Vertex u, const Graph & g) const
                {
                    std::cout << "finish vertex " << sg[u].id << std::endl;
                }

            // EDGES

            // Examine Edge
            /*
             * This gets called after a vertex has been discovered and after a node has finished
             */
            template < typename Edge, typename Graph >
                void examine_edge(Edge u, const Graph & g) const
                {
                    std::cout << "examine edge - n1:" << sg[u].n1 
                        << " f1:" << sg[u].f1
                        << " n2:" << sg[u].n2
                        << " f2:" << sg[u].f2
                        << std::endl;
                }



            // Tree Edge
            /*
             * This gets called each after the examine edge and only for edges that will make a tree
             * This would be used to draw the Outliner graph
             */
            template < typename Edge, typename Graph >
                void tree_edge(Edge u, const Graph & g) const
                {
                    std::cout << "tree edge - n1:" << sg[u].n1 
                        << " f1:" << sg[u].f1
                        << " n2:" << sg[u].n2
                        << " f2:" << sg[u].f2
                        << std::endl;
                }

            // Forward or Cross  Edge
            /*
             * This should be called in forward or cross edges but I never saw it. 
             */
            template < typename Edge, typename Graph >
                void forward_or_cross_edge(Edge u, const Graph & g) const
                {
                    std::cout << "forward or cross edge " << sg[u].id << std::endl;
                }


    };


    namespace scenegraph
    {

        status draw_gl()
        {
            // Temporarely turn off gl drawing for testing
            /*
            // set the state node drawing the gl 
            cstate.sgmode = state::DrawGL;

            node_visitor vis;
            //node_d_visitor vis;
            std::cout << "\n*****DRAW GL START*****\n";
            breadth_first_search(sg, vertex(0, sg), visitor(vis));
            std::cout << "*****DRAW GL COMPLETE*****\n";
            */
            return status();
        }

        status update()
        {
            // Temporary turn off do_it updating for testing
            // set the state node update 
            cstate.sgmode = state::DoIt;

            node_visitor vis;
            //node_d_visitor vis;
            std::cout << "\n*****GRAPH UPDATE*****\n";
            breadth_first_search(sg, vertex(0, sg), visitor(vis));
            //FNodeDescriptor s = vertex(0, scenegraph);
               
               //dijkstra_shortest_paths(scenegraph, s,
               //predecessor_map(boost::make_iterator_property_map(p.begin(),
               //get(boost::vertex_index, scenegraph))).distance_map(boost::make_iterator_property_map(d.begin(),
               //get(boost::vertex_index, scenegraph))));
            std::cout << "*****UPDATE COMPLETE*****\n";

            draw_gl();

            return status();
        };


        status connect(FNodeDescriptor n1, int f1, FNodeDescriptor n2, int f2)
        {
            // verify that the two fields have types that can be connected.
            
            
            FFieldConnection connection = boost::add_edge(n1, n2, sg);
            sg[connection.first].n1 = n1;
            sg[connection.first].f1 = f1;
            sg[connection.first].n2 = n2;
            sg[connection.first].f2 = f2;

            return status();
        };

        
        //template <int _Type, int _Node>
        //status add_node(int id) { return status(FAILED,"no matching node for add_node"); };

        /* 
           status connect(FNodeDescriptor n1, FNodeDescriptor n2)
           {

        //FFieldConnection connection = boost::add_edge(n1, n2, scenegraph);
        //scenegraph[connection.first].name = "test1";
        return status(true,"");
        };
        */

        // template <> status do_it() { std::cout << "missing node" << std::endl; return status(false,"failed"); };

    } // namespace scenegraph

    #define GET_NODE_DATA(nodedata)\
    template <> nodedata* DataObject::get_data<nodedata>(FNodeDescriptor node) { return static_cast<nodedata*>(sg[node].data); };

 
} // namespace feather

#endif
