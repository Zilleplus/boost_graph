#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/range.hpp>
#include <boost/range/adaptor/indexed.hpp>
#include <deque>
#include <iostream>
#include <list>
#include <string>
#include <type_traits>
#include <vector>

// From the docs, An adjacency_list has the following generic parameters:
// * OutEdgeList: 	The selector for the container used to 
//                  represent the edge-list for each of the vertices. 	vecS
// * VertexList:	The selector for the container used to represent
//                  the vertex-list of the graph. 	vecS
// * Directed:	A selector to choose whether the graph is directed,
//              undirected, or directed with bidirectional edge access 
//              (access to both out-edges and in-edges). The options are directedS,
//              undirectedS, and bidirectionalS. 	directedS
// * VertexProperties:	for specifying internal property storage. 	no_property
// * EdgeProperties:	for specifying internal property storage. 	no_property
// * GraphProperties:	for specifying property storage for the graph object. 	no_property
// * EdgeList:	The selector for the container used to represent the edge-list for the graph.

namespace boost{
enum vertex_compile_cost_t {vertex_compile_cost = 111}; // a unique #
    BOOST_INSTALL_PROPERTY(vertex, compile_cost);
}

using graph_type = boost::adjacency_list<
    boost::listS,  // by using a list, 
                   // we get linear removal time!, 
                   // but vertices are not int's any more.
    boost::listS,
    boost::directedS,
    // Vertex properties:
    boost::property<boost::vertex_name_t, std::string,
        boost::property<boost::vertex_compile_cost_t, float,
            boost::property<boost::vertex_distance_t, float,
                boost::property<boost::vertex_color_t, boost::default_color_type> > > >,
    boost::property<boost::edge_weight_t, float>
    >;

using name_map_t = boost::property_map<graph_type, boost::vertex_name_t>::type;
using compile_cost_map_t = boost::property_map<graph_type, boost::vertex_compile_cost_t>::type;
using distance_map_t = boost::property_map<graph_type, boost::vertex_distance_t>::type;
using color_map_t = boost::property_map<graph_type, boost::vertex_color_t>::type;


int main()
{
    graph_type g;

    name_map_t name_map = get(boost::vertex_name, g);
    compile_cost_map_t compile_cost_map = get(boost::vertex_compile_cost, g);
    // -> get the property through compile_cost_map[vertex_element]
    // -> there is a set method to set the vertex/edge...

    return 0;
}

