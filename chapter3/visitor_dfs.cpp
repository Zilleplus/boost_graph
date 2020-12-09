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

// Couldn't find the actual example used in the text,
// but the one from chapter 1 will do just fine it seems :-).
// Mostly code serrouding the dfs visitor, which is a neat idea itself,
// and a large part of chapter 3 of the boost::graph book.

using graph_type = boost::adjacency_list<boost::listS, boost::vecS, boost::directedS>;

typedef boost::graph_traits<graph_type>::vertex_descriptor vertex_t;
typedef boost::graph_traits<graph_type>::edge_descriptor edge_t;
// white means undiscovered, gray means discovered but still searching descendants, and black
// means the vertex and all of its descendants have been discovered.
enum class color_type { white,
    gray,
    black };

bool has_cycle_dfs(
    const graph_type& g,
    vertex_t u,
    color_type* color)
{
    color[u] = color_type::gray;
    boost::graph_traits<graph_type>::adjacency_iterator vi, vi_end;
    for (boost::tie(vi, vi_end) = boost::adjacent_vertices(u, g); vi != vi_end; ++vi) {
        if (color[*vi] == color_type::white) {
            if (has_cycle_dfs(g, *vi, color)) {
                return true;
            } else if (color[*vi] == color_type::gray) {
                return true;
            }
        }
    }
    color[u] = color_type::black;
    return false;
}

bool has_cycle(const graph_type& g)
{
    std::vector<color_type> color(
        boost::num_vertices(g),
        color_type::white);

    boost::graph_traits<graph_type>::vertex_iterator vi, vi_end;
    for (boost::tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
        if (color[*vi] == color_type::white) {
            if (has_cycle_dfs(g, *vi, &color[0])) {
                return true;
            }
        }
    }

    return false;
}

template <typename Visitor>
void dfs_v1(const graph_type& g, vertex_t u, color_type* color, Visitor vis)
{
    color[u] = color_type::gray;
    vis.discover_vertex(u, g);

    boost::graph_traits<graph_type>::out_edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = out_edges(u, g); ei != ei_end; ++ei) {
        // boost::target/ boost::source -> get start/end edge
        if (color[boost::target(*ei, g)] == color_type::white) {
            vis.tree_edge(*ei, g);
            dfs_v1(g, target(*ei, g), color, vis);
        } else if (color[target(*ei, g)] == color_type::gray) {
            vis.back_edge(*ei, g);
        } else {
            vis.forward_or_cross_edge(*ei, g);
        }
    }

    color[u] = color_type::black;
    vis.finish_vertex(u, g);
}

template <typename Visitor>
void generic_dfs_v1(const graph_type& g, Visitor vis)
{
    std::vector<color_type> color(num_vertices(g), color_type::white);
    boost::graph_traits<graph_type>::vertex_iterator vi, vi_end;
    for (boost::tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
        if (color[*vi] == color_type::white) {
            dfs_v1(g, *vi, &color[0], vis);
        }
    }
}

struct default_dfs_visitor {
    template <typename V, typename G>
    void discover_vertex(V, const G&) { }

    template <typename E, typename G>
    void tree_edge(E, const G&) { }

    template <typename E, typename G>
    void back_edge(E, const G&) { }

    template <typename E, typename G>
    void forward_or_cross_edge(E, const G&) { }

    template <typename V, typename G>
    void finish_vertex(V, const G&) { }
};

struct topo_visitor : public default_dfs_visitor {
    topo_visitor(vertex_t*& order)
        : topo_order(order)
    {
    }

    void finish_vertex(vertex_t u, const graph_type&)
    {
        *--topo_order = u;
    }

    vertex_t*& topo_order;
};

void topo_sort(const graph_type& g, vertex_t* topo_order)
{
    topo_visitor vis(topo_order);
    generic_dfs_v1(g, vis);
}

struct cycle_detector : public default_dfs_visitor{
    cycle_detector(bool& cycle) : has_cycle(cycle) {}
    void back_edge (edge_t, const graph_type&)
    {
        has_cycle = true;
    }

    bool& has_cycle;
};

bool has_cycle_vis(const graph_type& g)
{
    bool has_cycle = true;
    cycle_detector vis(has_cycle);
    generic_dfs_v1(g, vis);
    return has_cycle;
}

int main()
{
    const char* tasks[] = {
        "pick up kids from school",
        "buy groceries (and snack)",
        "get cash at ATM",
        "drop off kids at soccer practice",
        "cook dinner",
        "pick up kids from soccer",
        "eat dinner"
    };

    const int n_tasks = sizeof(tasks) / sizeof(char*);

    graph_type g(n_tasks);
    add_edge(0, 3, g);
    add_edge(1, 3, g);
    add_edge(1, 4, g);
    add_edge(2, 1, g);
    add_edge(3, 5, g);
    add_edge(4, 6, g);
    add_edge(5, 6, g);

    std::cout << "cycle detection: " << (has_cycle(g) ? "true" : "false") << std::endl;
    //std::deque<int> topo_order;

    //topological_sort(g, std::front_inserter(topo_order),
    //        vertex_index_map(identity_property_map()));

    //for(const auto& [index, i] : topo_order
    //        | boost::adaptors::indexed())
    //{
    //    std::cout << tasks[i.head] << std::endl;
    //}

    return 0;
}
