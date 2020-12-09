#include <deque>
#include <vector>
#include <list>
#include <iostream>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/range.hpp>
#include <boost/range/adaptor/indexed.hpp>

int main()
{
    using namespace boost;

    const char * tasks[] = {
        "pick up kids from school",
        "buy groceries (and snack)",
        "get cash at ATM",
        "drop off kids at soccer practice",
        "cook dinner",
        "pick up kids from soccer",
        "eat dinner"
    };

    const int n_tasks = sizeof(tasks) /sizeof(char*);

    adjacency_list<listS, vecS, directedS> g(n_tasks);
    add_edge(0, 3, g);
    add_edge(1, 3, g);
    add_edge(1, 4, g);
    add_edge(2, 1, g);
    add_edge(3, 5, g);
    add_edge(4, 6, g);
    add_edge(5, 6, g);

    std::deque<int> topo_order;

    topological_sort(g, std::front_inserter(topo_order),
            vertex_index_map(identity_property_map()));

    for(const auto& [index, i] : topo_order 
            | boost::adaptors::indexed())
    {
        std::cout << tasks[i.head] << std::endl;
    }


    return 0;
}
