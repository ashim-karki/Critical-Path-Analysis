#include <vector>
#include <string>
#include<queue>
#include <iostream>
#include <unordered_map>
using namespace std;
class Vertex {
public:
    std::string data;
    int time;
    int earliest_start_time;
    int early_finish_time;
    int late_start_time;
    int latest_finish_time;
    int slack;
    bool visited;

    Vertex(std::string data, int time) :
        data(data), time(time),
        earliest_start_time(0), early_finish_time(0),
        late_start_time(0), latest_finish_time(100), slack(-1),
        visited(false)
    {}
};

class Graph {
public:
    std::vector<Vertex*> vertices;
    std::unordered_map<Vertex*, std::vector<Vertex*> > edges;

    void add_vertex(Vertex* vertex) {
        vertices.push_back(vertex);
    }
    string FinalVertexName = "FinalVertex";
    int FinalVertexTime = 0;
    Vertex* finish_vertex = new Vertex(FinalVertexName, FinalVertexTime);

    void add_edge(Vertex* v1, Vertex* v2, char last_node) {
        edges[v2].push_back(v1);
        if (last_node == 'y' || last_node == 'Y') {
            if (v1->early_finish_time > finish_vertex->late_start_time) {
                finish_vertex->late_start_time = v1->early_finish_time;
            }
            edges[v1].push_back(finish_vertex);
        }
    }
    void add_new_vertex() { //FORWARD PASS ADDED HERE
        std::string name;
        int time;
        char last_node;

        std::cout << "Enter the name of the new vertex: ";
        std::cin >> name;
        std::cout << "Enter the time associated with the new vertex: ";
        std::cin >> time;

        Vertex* new_vertex = new Vertex(name, time);
        add_vertex(new_vertex);

        new_vertex->earliest_start_time = 0;
        new_vertex->early_finish_time = new_vertex->earliest_start_time + time;

        std::string dependancy_name;
        std::cout << "Enter the name of a dependant vertex (if any else press A to exit): ";
        std::cin >> dependancy_name;
        while (dependancy_name != "X") {
            Vertex* dependancy = nullptr;
            for (auto vertex : vertices) {
                if (vertex->data == dependancy_name) {
                    dependancy = vertex;
                    break;
                }
            }

            if (dependancy->early_finish_time > new_vertex->earliest_start_time) {
                new_vertex->earliest_start_time = dependancy->early_finish_time;
                new_vertex->early_finish_time = new_vertex->earliest_start_time + time;
            }

            cout << "Last node? y/n: "; cin >> last_node; //FIND A BETTER PLACEMENT AS IT IS CALLED MANY TIMES

            if (dependancy) {
                add_edge(new_vertex, dependancy, last_node);
            }
            else {
                std::cout << "Error: specified dependant vertex not found." << std::endl;
            }

            std::cout << "Enter the name of a dependant vertex (if any): ";
            std::cin >> dependancy_name;
        }
    }

    void backward_pass() {
        vector<Vertex*> reverse_vertices = vertices;
        reverse(reverse_vertices.begin(), reverse_vertices.end());
        cout << endl;
        std::cout << "Reverse Vertices:" << std::endl;
        for (auto vertex : reverse_vertices) {
            std::cout << vertex->data << " (" << vertex->time << ")" << " (" << vertex->earliest_start_time << ")" << " (" << vertex->early_finish_time << ")" << " (" << vertex->late_start_time << ")" << " (" << vertex->latest_finish_time << ")" << std::endl;
        }

        for (auto vertex : reverse_vertices) {
            auto it = edges.find(vertex);
            if (it != edges.end()) {
                for (auto neighbor : it->second) {
                    if (vertex->latest_finish_time > neighbor->late_start_time) {
                        vertex->latest_finish_time = neighbor->late_start_time;
                        vertex->late_start_time = vertex->latest_finish_time - vertex->time;
                    }
                }
            }
        }
    }

    void calculate_CP() {
        cout << endl;
        for (auto vertex : vertices) {
            vertex->slack = vertex->earliest_start_time - vertex->late_start_time;
            if (vertex->slack == 0) cout << vertex->data << "\t";
        }
    }

    void print_vertices_and_edges() {
        std::cout << "Vertices:" << std::endl;
        for (auto vertex : vertices) {
            std::cout << vertex->data << " (" << vertex->time << ")" << " (" << vertex->earliest_start_time << ")" << " (" << vertex->early_finish_time << ")" << " (" << vertex->late_start_time << ")" << " (" << vertex->latest_finish_time << ")" << std::endl;
        }

        /*cout<<"Forward Pass:"<<endl;
        std::cout << "Edges:" << std::endl;
        for (auto vertex : vertices) {
            std::cout << vertex->data;
            auto it = edges.find(vertex);
            if (it != edges.end()) {
                std::cout << " -> ";
                for (auto neighbor : it->second) {
                    std::cout << neighbor->data;
                }
            }
            std::cout << std::endl;
        }

        cout<<endl;
        cout<<"Backward Pass:"<<endl;
        std::cout << "Edges:" << std::endl;
        for (auto vertex : vertices) {
            std::cout << vertex->data;
            auto it = edges_backward.find(vertex);
            if (it != edges_backward.end()) {
                std::cout << " -> ";
                for (auto neighbor : it->second) {
                    std::cout << neighbor->data;
                }
            }
            std::cout << std::endl;
        }*/
    }

    void sort_graph_topologically() {
        std::unordered_map<Vertex*, int> in_degree;
        for (auto vertex : vertices) {
            in_degree[vertex] = 0;
        }

        for (auto vertex : vertices) {
            for (auto neighbor : edges[vertex]) {
                ++in_degree[neighbor];
            }
        }

        std::queue<Vertex*> q;
        for (auto vertex : vertices) {
            if (in_degree[vertex] == 0) {
                q.push(vertex);
            }
        }

        std::vector<Vertex*> sorted;
        while (!q.empty()) {
            auto current = q.front();
            q.pop();
            sorted.push_back(current);
            for (auto neighbor : edges[current]) {
                --in_degree[neighbor];
                if (in_degree[neighbor] == 0) {
                    q.push(neighbor);
                }
            }
        }

        std::cout << "Topological sort of the graph: " << std::endl;
        for (auto vertex : sorted) {
            std::cout << vertex->data << " (" << vertex->time << ")" << std::endl;
        }
    }
};

int main() {
    Graph graph;
    std::cout << "Enter the number of vertices: ";
    int num_vertices;
    std::cin >> num_vertices;
    for (int i = 0; i < num_vertices; ++i) {
        graph.add_new_vertex();
    }
    graph.add_vertex(graph.finish_vertex);

    graph.backward_pass();

    graph.print_vertices_and_edges();

    graph.calculate_CP();

    return 0;
}
