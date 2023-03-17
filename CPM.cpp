#include<SFML/Graphics.hpp>
#include<string>
#include<cstdlib>
#include<iostream>
#include<unordered_map>
#include<queue>
using namespace std;
int MinCompletionTime = 0;
class Vertex
{
public:
    std::string data;
    int time;
    int  id;
    bool cflag = false;
    int earliest_start_time;
    int early_finish_time;
    int late_start_time;
    int latest_finish_time;
    int slack;
    bool visited;

    //x and y are used to postion vertex during graph rendering
    float x = 0;
    float y = 0;



    Vertex(std::string data, int time, bool flag = false) : data(data), time(time), cflag(flag),
        earliest_start_time(0), early_finish_time(0),
        late_start_time(0), latest_finish_time(100), slack(-1),
        visited(false)
    {
    }
};


class Graph
{
public:
    std::vector<Vertex*> vertices;
    std::unordered_map<Vertex*, std::vector<Vertex*>> edges;

    void add_vertex(Vertex* vertex)
    {
        vertices.push_back(vertex);
    }



    string FinalVertexName = "FinalVertex";
    int FinalVertexTime = 0;
    int ID = 1123;


    Vertex* finish_vertex = new Vertex(FinalVertexName, FinalVertexTime, true);

    void add_edge(Vertex* v1, Vertex* v2, char last_node)
    {
        edges[v2].push_back(v1);
        if (last_node == 'y' || last_node == 'Y')
        {
            if (v1->early_finish_time > finish_vertex->late_start_time)
            {
                finish_vertex->late_start_time = v1->early_finish_time;
            }
            edges[v1].push_back(finish_vertex);
        }
    }
    void add_new_vertex()
    { // FORWARD PASS ADDED HERE
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
        std::cout << "Enter the name of a dependant vertex (if any else press X to exit): ";
        std::cin >> dependancy_name;
        cout << "Last node? y/n: ";
        cin >> last_node; // FIND A BETTER PLACEMENT AS IT IS CALLED MANY TIMES
        while (dependancy_name != "X")
        {
            Vertex* dependancy = nullptr;
            for (auto vertex : vertices)
            {
                if (vertex->data == dependancy_name)
                {
                    dependancy = vertex;
                    break;
                }
            }

            if (dependancy->early_finish_time > new_vertex->earliest_start_time)
            {
                new_vertex->earliest_start_time = dependancy->early_finish_time;
                new_vertex->early_finish_time = new_vertex->earliest_start_time + time;
            }



            if (dependancy)
            {
                add_edge(new_vertex, dependancy, last_node);
            }
            else
            {
                std::cout << "Error: specified dependant vertex not found." << std::endl;
            }

            std::cout << "Enter the name of a dependant vertex (if any else press X to exit):  ";
            std::cin >> dependancy_name;
        }
    }
    void backward_pass()
    {
        vector<Vertex*> reverse_vertices = vertices;
        reverse(reverse_vertices.begin(), reverse_vertices.end());
        cout << endl;
        std::cout << "Reverse Vertices:" << std::endl;
        for (auto vertex : reverse_vertices)
        {
            std::cout << vertex->data << " (" << vertex->time << ")"
                << " (" << vertex->earliest_start_time << ")"
                << " (" << vertex->early_finish_time << ")"
                << " (" << vertex->late_start_time << ")"
                << " (" << vertex->latest_finish_time << ")" << std::endl;
        }

        for (auto vertex : reverse_vertices)
        {
            auto it = edges.find(vertex);
            if (it != edges.end())
            {
                for (auto neighbor : it->second)
                {
                    if (vertex->latest_finish_time > neighbor->late_start_time)
                    {
                        vertex->latest_finish_time = neighbor->late_start_time;
                        vertex->late_start_time = vertex->latest_finish_time - vertex->time;
                    }
                }
            }
        }
    }

    void calculate_CP()
    {
        cout << "critical path" << endl;
        for (auto vertex : vertices)
        {
            vertex->slack = vertex->earliest_start_time - vertex->late_start_time;
            if (vertex->slack == 0)
            {


                cout << vertex->data << "\t";
                vertex->cflag = true;
                MinCompletionTime += vertex->time;
            }
        }
    }

    void print_vertices_and_edges()
    {
        std::cout << "Vertices:" << std::endl;
        for (auto vertex : vertices)
        {
            std::cout << vertex->data << " (" << vertex->time << ")"
                << " (" << vertex->earliest_start_time << ")"
                << " (" << vertex->early_finish_time << ")"
                << " (" << vertex->late_start_time << ")"
                << " (" << vertex->latest_finish_time << ")" << std::endl;
        }


        std::cout << "Edges:" << std::endl;
        for (auto vertex : vertices) {
            std::cout << vertex->data;
            auto it = edges.find(vertex);
            if (it != edges.end()) {
                std::cout << " -> ";
                for (auto neighbor : it->second) {
                    std::cout << neighbor->data << " ";
                }
            }
            std::cout << std::endl;
        }

        cout << endl;
        /*
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

    void sort_graph_topologically()
    {
        std::unordered_map<Vertex*, int> in_degree;
        for (auto vertex : vertices)
        {
            in_degree[vertex] = 0;
        }

        for (auto vertex : vertices)
        {
            for (auto neighbor : edges[vertex])
            {
                ++in_degree[neighbor];
            }
        }

        std::queue<Vertex*> q;
        for (auto vertex : vertices)
        {
            if (in_degree[vertex] == 0)
            {
                q.push(vertex);
            }
        }

        std::vector<Vertex*> sorted;
        while (!q.empty())
        {
            auto current = q.front();
            q.pop();
            sorted.push_back(current);
            for (auto neighbor : edges[current])
            {
                --in_degree[neighbor];
                if (in_degree[neighbor] == 0)
                {
                    q.push(neighbor);
                }
            }
        }

        std::cout << "Topological sort of the graph: " << std::endl;
        for (auto vertex : sorted)
        {
            std::cout << vertex->data << " (" << vertex->time << ")" << std::endl;
        }
    }









    void printEdge()
    {
        for (auto vertex : vertices) {
            std::cout << vertex->id;
            auto it = edges.find(vertex);
            if (it != edges.end()) {
                std::cout << " -> ";
                for (auto neighbor : it->second) {
                    std::cout << neighbor->id;
                }
            }
            std::cout << std::endl;
        }
    }


    void updateID()
    {
        int i = 0;
        for (auto it : vertices)
        {
            it->id = i;
            i++;
        }
    }

};


class Output
{
public:


    sf::Font displayfont;
    sf::Vector2f coordinates;

public:
    sf::Text name;
    sf::Text LST;
    sf::Text CompletionTime;
    sf::Text msg;

    Output() {};
    Output(sf::Vector2f coord) : coordinates(coord) {};

    void printOutput(sf::RenderWindow& window)
    {
        displayfont.loadFromFile("Fonts/BRITANIC.TTF");
        name.setFont(displayfont);
        LST.setFont(displayfont);
        LST.setPosition(coordinates.x, coordinates.y + 30);
        name.setPosition(coordinates.x, coordinates.y + 10);





        name.setOutlineColor(sf::Color::Black);

        name.setCharacterSize(18);
        LST.setCharacterSize(12);
        LST.setFillColor(sf::Color::Black);

        name.setFillColor(sf::Color::Black);

        CompletionTime.setFont(displayfont);
        CompletionTime.setPosition(200, 535);
        CompletionTime.setCharacterSize(18);
        CompletionTime.setFillColor(sf::Color::Black);
        CompletionTime.setString("Minimum Completion time: " + to_string(MinCompletionTime));

        msg.setFont(displayfont);
        msg.setPosition(765, 535);
        msg.setCharacterSize(18);
        msg.setFillColor(sf::Color::Black);
        //CompletionTime.setString("Minmuum Completion time: "+ to_string(MinCompletionTime)); 
        window.draw(name);
        window.draw(LST);
        window.draw(CompletionTime);
        window.draw(msg);

    }
    string convert(int EST, int EFT, int LST, int LFT)
    {
        string est = to_string(EST);
        string eft = to_string(EFT);
        string lst = to_string(LST);
        string lft = to_string(LFT);
        string total = est + " " + eft + "\n" + lst + " " + lft;
        return total;
    }
};
int main()
{// float col = 1080/2.0;

    Graph graph;
    Output output;

    std::cout << "Enter the number of vertices: ";
    int num_vertices;
    std::cin >> num_vertices;
    for (int i = 0; i < num_vertices; ++i)
    {
        graph.add_new_vertex();
    }
    graph.add_vertex(graph.finish_vertex);
    graph.updateID();

    graph.backward_pass();
    graph.sort_graph_topologically();

    graph.print_vertices_and_edges();

    graph.calculate_CP();

    cout << "cp time " << MinCompletionTime;

    // for(int i=0;i<4;i++)
    // {
    //    Vertex *newV =new Vertex(i,0,0);
    //    Vertex *newV1 = new Vertex(3*i,0,0);
    //     g.add_vertex(newV1);
    //     g.add_vertex(newV);

    //     g.add_edge(newV,newV1);  //we need to generate new unordered map to store the edges



    // }




    int i = 1;
    int j = 0;
    //use this to setup the distancse so that not clustered form
    //the vertex can be set up for the final listed vertices vector
    for (auto it : graph.vertices)
    {
        if ((it->id) % 2 == 0) {
            it->x = 100 + i * 50;
            it->y = 300 + j * 20;
        }
        else
        {
            it->x = 100 + i * 50;
            it->y = 300 - j * 20;
        }
        i += 2;
        j++;
    }
    //we need to iterate the unordered map in order to draw the edge

    for (auto vertex : graph.vertices) {

        auto it = graph.edges.find(vertex);
        if (it != graph.edges.end()) {
            std::cout << " -> ";
            for (auto neighbor : it->second) {
                std::cout << neighbor->id;
            }
        }
        std::cout << std::endl;
    }





    sf::RenderWindow window(sf::VideoMode(1080, 720), "SFML works!");
    sf::CircleShape source(20.f);
    sf::CircleShape dest(20.f);
    sf::Font font;
    if (!font.loadFromFile("Fonts/BRITANIC.TTF"))
    {
        cout << "error loading file" << endl;
    }
    window.clear(sf::Color(255, 255, 255));
    dest.setFillColor(sf::Color::Red);
    source.setFillColor(sf::Color::Green);
    source.setPosition(sf::Vector2f(200 - 20, 200 - 20));
    dest.setPosition(sf::Vector2f(200 - 20, 80 - 20));
    sf::VertexArray line(sf::Lines, 2);
    line[0].position = sf::Vector2f(200, 200);
    line[0].color = sf::Color::Black; line[1].color = sf::Color::Black;
    line[1].position = sf::Vector2f(200, 80);

    sf::VertexArray line2(sf::Lines, 2);
    line2[0].color = sf::Color::Black; line2[1].color = sf::Color::Black;
    line[0].position = sf::Vector2f(200, 200);
    line2[1].position = sf::Vector2f(200, 80);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color(255, 255, 255, 180));


        for (auto vertex : graph.vertices) {

            auto it = graph.edges.find(vertex);
            if (it != graph.edges.end()) {

                for (auto neighbor : it->second) {
                    line[0].position = sf::Vector2f(vertex->x, vertex->y);
                    line[1].position = sf::Vector2f(neighbor->x, neighbor->y);
                    line2[0].position = sf::Vector2f(vertex->x + 0.5, vertex->y + 0.5);
                    line2[1].position = sf::Vector2f(neighbor->x + 0.5, neighbor->y + 0.5);
                    window.draw(line);
                    window.draw(line2);
                }
            }

        }
        for (auto it : graph.vertices)
        {
            source.setPosition(sf::Vector2f(it->x - 20, it->y - 20));
            dest.setPosition(sf::Vector2f(it->x - 20, it->y - 20));
            if (it->cflag == false)
                window.draw(source);
            else
                window.draw(dest);
            output.name.setString(it->data);
            output.LST.setString(output.convert(it->earliest_start_time, it->early_finish_time, it->late_start_time, it->latest_finish_time));

            output.coordinates.x = it->x;
            output.coordinates.y = it->y;
            output.printOutput(window);



        }
        dest.setPosition(sf::Vector2f(750 - 20, 550 - 20));
        window.draw(dest);
        output.msg.setString(":Critical Nodes");
        output.printOutput(window);


        window.display();


    }

    return 0;
}