#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <regex>
#include <assert.h>
using namespace std;

class Node
{
public:
    Node(string s, Node* p)
    {
        state = s;
        parent = p;
        if(!parent)/*root node*/
        {
            f = 0;
            g = 0;
            h = 0;
        }
        else/*not a root node*/
        {
            f = 0;
            g = parent->g + 1;
            h = 0;
        }
    }
    
    unsigned f, g, h; /* f = f-value, g = depth, h = heuristic*/
    string state;
    Node* parent;
};

class Problem
{
public:
    Problem(string i,string g)
    {
        initial = i;
        goal = g;
    }

    void pop_i(vector<Node*>& vec, int i)
    {
        assert(!vec.empty());
        vec.erase(vec.begin() + i);
    }
    bool isgoal(Node* node)
    {
        if(node->state == goal) return true;
        else return false;
    }
    bool iscycle(Node* node)
    {
        string s = node->state;
        Node* p = node->parent;
        while(p)
        {
            if(p->state == s) return true;
            else p = p->parent;
        }
        return false;
    }
    bool inside(vector <Node*> &reached, string state)
    {
        for(int i = 0; i < reached.size(); i++)
        {
            if(state == reached.at(i)->state) return true;
        }
        return false;
    }
    void expand(vector <Node*> &frontier, Node* parent)
    {
        vector<string> states = action(parent->state);
        max_num_nodes += states.size();
        for(int i = 0;i < states.size();i++)
        {
            Node *reg = new Node(states.at(i), parent);
            frontier.push_back(reg);
        }
    }
    vector<string> action(string s)
    {
        string reg;
        vector<string> states;
        short index = s.find("0");
        short row = index / 3;
        short col = index % 3;
        if(row != 0)/*can switch with upper puzzle*/
        {
            reg = s;
            swap(reg[index], reg[index - 3]);
            states.push_back(reg);
        }
        if(row != 2)/*can switch with lower puzzle*/
        {
            reg = s;
            swap(reg[index], reg[index + 3]);
            states.push_back(reg);
        }
        if(col != 0)/*can switch with left puzzle*/
        {
            reg = s;
            swap(reg[index], reg[index - 1]);
            states.push_back(reg);
        }
        if(col != 2)/*can switch with right puzzle*/
        {
            reg = s;
            swap(reg[index], reg[index + 1]);
            states.push_back(reg);
        }
        return states;
    }
    void make_path(Node* last_node)
    {
        string s = last_node->state;
        Node* p = last_node->parent;
        while(p)
        {
            s = p->state + s;
            p = p->parent;
        }
        movements--;/*because initial state was regarded as a step at the beginning*/
        print_path(s);
        cout << "movements = " << movements << endl;
        cout << "max number of states = " << max_num_nodes << endl;
    }
    void print_path(string result)
    {
        result = regex_replace(result, regex("0"), " ");
        for(int i = 0;i < result.size();i+=27)
        {
            if((i + 27) <= result.size())
            {
                cout << result[i] << " " << result[i+1] << " " << result[i+2];
                cout << "  ";
                cout << "  " << result[i+9] << " " << result[i+10] << " " << result[i+11];
                cout << "  ";
                cout << "  " << result[i+18] << " " << result[i+19] << " " << result[i+20];
                cout << endl;

                cout << result[i+3] << " " << result[i+4] << " " << result[i+5];
                cout << " -";
                cout << "> " << result[i+12] << " " << result[i+13] << " " << result[i+14];
                cout << " -";
                cout << "> " << result[i+21] << " " << result[i+22] << " " << result[i+23];
                cout << endl;

                cout << result[i+6] << " " << result[i+7] << " " << result[i+8];
                cout << "  ";
                cout << "  " << result[i+15] << " " << result[i+16] << " " << result[i+17];
                cout << "  ";
                cout << "  " << result[i+24] << " " << result[i+25] << " " << result[i+26];
                cout << endl;
            }
            else if((i + 18) <= result.size())
            {
                cout << result[i] << " " << result[i+1] << " " << result[i+2];
                cout << "  ";
                cout << "  " << result[i+9] << " " << result[i+10] << " " << result[i+11];
                cout << endl;

                cout << result[i+3] << " " << result[i+4] << " " << result[i+5];
                cout << " -";
                cout << "> " << result[i+12] << " " << result[i+13] << " " << result[i+14];
                cout << endl;

                cout << result[i+6] << " " << result[i+7] << " " << result[i+8];
                cout << "  ";
                cout << "  " << result[i+15] << " " << result[i+16] << " " << result[i+17];
                cout << endl;
            }
            else
            {
                cout << result[i] << " " << result[i+1] << " " << result[i+2];
                cout << endl;

                cout << result[i+3] << " " << result[i+4] << " " << result[i+5];
                cout << endl;

                cout << result[i+6] << " " << result[i+7] << " " << result[i+8];
                cout << endl;
            }
            cout << endl;
        }
    }
    
    string initial;
    string goal;
    string result;

    /*number of states change*/
    unsigned movements = 0;
    /*max number of states ever saved in the memory during the process.*/
    unsigned max_num_nodes = 0;
};

class BFS: public Problem
{
public:
    BFS(string i, string g): Problem(i, g) {}
    void bfs(string mode)
    {
        vector<Node*> frontier;
        vector<Node*> reached;
        Node* root = new Node(initial, nullptr);
        heuristics(root);
        reached.push_back(root);
        frontier.push_back(root);
        max_num_nodes++;

        while(!frontier.empty())
        {
            int best = select_best(frontier, mode);
            Node* current = frontier.at(best);
            reached.push_back(current);
            pop_i(frontier, best);
            movements++;

            if(isgoal(current))
            {
                make_path(current);
                return ;
            }
            vector<string> child_states = action(current->state);
            for(int i = 0; i < child_states.size();i++)
            {
                if(!inside(reached, child_states.at(i)))
                {
                    Node* reg = new Node(child_states.at(i), current);
                    heuristics(reg);
                    reached.push_back(reg);
                    frontier.push_back(reg);
                    max_num_nodes++;
                }
            }
        }
        cout << "failure" << endl;
    }
    int select_best(vector <Node*> &frontier, string mode)
    {
        unsigned min_cost = UINT_MAX;
        int idx;
        if(mode == "greedy")
        {
            for(int i = 0;i < frontier.size();i++)
            {
                if(frontier.at(i)->h < min_cost)
                {
                    idx = i;
                    min_cost = frontier.at(i)->h;
                }
            }
        }
        if((mode == "astar") || (mode == "best_f"))
        {
            for(int i = 0;i < frontier.size();i++)
            {
                if(frontier.at(i)->f <= min_cost)
                {
                    idx = i;
                    min_cost = frontier.at(i)->f;
                }                    
            }
        }
        if(mode == "second_best_f")
        {
            unsigned min2_cost = UINT_MAX;
            int idx2;
            for(int i = 0;i < frontier.size();i++)
            {
                if(frontier.at(i)->f <= min_cost)
                {
                    idx = i;
                    min_cost = frontier.at(i)->f;
                }
            }
            idx2 = idx;
            for(int i = 0;i < frontier.size();i++)
            {
                if((frontier.at(i)->f <= min2_cost) && (frontier.at(i)->f > min_cost))
                {
                    idx2 = i;
                    min2_cost = frontier.at(i)->f;
                }
            }
            return idx2;
        }
        return idx;
    }
    void heuristics(Node* node)/*sum of Manhattan distances*/
    {
        for(int i = 0; i < 9;i++)
        {
            int tmp = node->state[i] - '0';
            short i_row = i / 3;
            short i_col = i % 3;
            if(tmp != 0)
            {
                int j = goal.find(node->state[i]);
                short j_row = j / 3;
                short j_col = j % 3;
                node->h += abs(i_row - j_row) + abs(i_col - j_col);
            }
        }
        node->h /= 2;/*divided since Manhattan(a, b) == Manhattan(b, a)*/
        if(node->parent) node->f = node->g + node->h;
        else node->f = node->h;
    }
    
};

class IDS: public Problem/*Iterative-Deepening Search*/
{
public:
    IDS(string i,string g): Problem(i, g) {ids();}    
    void ids()/*Iterative-Deepening Search*/
    {
        for(unsigned depth = 0; depth < UINT_MAX; depth++)
        {
            result = dls(depth);
            if(result != "cutoff")
            {
                if(result == "failure") cout << "failure";
                break;
            }
        }
    }
    string dls(unsigned l)/*Depth-Limited Search*/
    {
        vector<Node*> frontier;
        Node* root = new Node(initial, nullptr);
        frontier.push_back(root);
        max_num_nodes++;
        
        Node* current;
        string iter_result = "failure";

        while(!frontier.empty())
        {
            current = frontier.back();
            frontier.pop_back();/*LIFO queue*/
            movements++;

            if(isgoal(current))
            {
                make_path(current);
                return "solved";
            }
            if(current->g > l) iter_result = "cutoff";
            else if(!iscycle(current)) expand(frontier, current);
        }
        return iter_result;
    }
};

class UCS: public Problem/*Uniform Cost Search, should be same as breadth first search*/
{
public:
    UCS(string i,string g): Problem(i, g) {ucs();}
    void ucs()
    {
        vector<Node*> frontier;
        vector<Node*> reached;
        Node* root = new Node(initial, nullptr);
        reached.push_back(root);
        frontier.push_back(root);
        max_num_nodes++;

        while(!frontier.empty())
        {
            Node* current = frontier.front();
            reached.push_back(current);
            pop_i(frontier, 0);/*FIFO queue*/
            movements++;

            if(isgoal(current))
            {
                make_path(current);
                return ;
            }
            vector<string> child_states = action(current->state);
            for(int i = 0; i < child_states.size();i++)
            {
                if(!inside(reached, child_states.at(i)))
                {
                    Node* reg = new Node(child_states.at(i), current);
                    reached.push_back(reg);
                    frontier.push_back(reg);
                    max_num_nodes++;
                }
            }
        }
        cout << "failure" << endl;
    }
};

class GBFS: public BFS/*Greedy Best First Search*/
{
public:
    GBFS(string i,string g): BFS(i, g) {gbfs();}
    void gbfs()
    {
        bfs("greedy");
    }
};

class Astar: public BFS/*A* search*/
{
public:
    Astar(string i,string g): BFS(i, g) {astar();}
    void astar()
    {
        bfs("astar");
    }
};

class RBFS: public BFS /*Recursive Best First Search*/
{
public:
    RBFS(string i,string g): BFS(i, g)
    {
        root = new Node(initial, nullptr);
        heuristics(root);
        rbfs(root, UINT_MAX);
    }    
    pair<string, unsigned> rbfs(Node* node,unsigned f_limit)
    {
        if(isgoal(node))
        {
            make_path(node);
            return make_pair("solved", node->f);
        }
        vector<Node*> successors;
        expand(successors, node);
        if(successors.empty()) return make_pair("failure", UINT_MAX);
        for(int i = 0; i < successors.size(); i++)
        {
            heuristics(successors.at(i));
            successors.at(i)->f = max(successors.at(i)->f, node->f);
        }
        while(true)
        {
            int idx = select_best(successors, "best_f");
            Node* best = successors.at(idx);
            movements++;
            if(best->f > f_limit) return make_pair("failure", best->f);
            
            int idx2 = select_best(successors, "second_best_f");
            unsigned alternative = successors.at(idx2)->f;
            pair<string, unsigned> p = rbfs(best, min(f_limit, alternative));            
            string recursive_result = p.first;
            best->f = p.second;
            if(recursive_result != "failure") return make_pair(recursive_result, best->f);
        }
    }

    Node* root;
};

class SixteenPuzzle: public BFS/*same as A* search, with some important functions re-defined.*/
{
public:    
    SixteenPuzzle(string i, string g): BFS(i, g) {bfs("astar");}
    void bfs(string mode)
    {
        vector<Node*> frontier;
        vector<Node*> reached;
        Node* root = new Node(initial, nullptr);
        heuristics(root);
        reached.push_back(root);
        frontier.push_back(root);
        max_num_nodes++;

        while(!frontier.empty())
        {
            int best = select_best(frontier, mode);
            Node* current = frontier.at(best);
            reached.push_back(current);
            pop_i(frontier, best);
            movements++;

            if(isgoal(current))
            {
                make_path(current);
                return ;
            }
            vector<string> child_states = action(current->state);
            for(int i = 0; i < child_states.size();i++)
            {
                if(!inside(reached, child_states.at(i)))
                {
                    Node* reg = new Node(child_states.at(i), current);
                    heuristics(reg);
                    reached.push_back(reg);
                    frontier.push_back(reg);
                    max_num_nodes++;
                }
            }
        }
        cout << "failure" << endl;
    }

    void heuristics(Node* node)/*sum of Manhattan distances*/
    {
        for(int i = 0; i < 16;i++)
        {
            int tmp = node->state[i] - '0';
            short i_row = i / 4;
            short i_col = i % 4;
            if(tmp != 0)
            {
                int j = goal.find(node->state[i]);
                short j_row = j / 4;
                short j_col = j % 4;
                node->h += abs(i_row - j_row) + abs(i_col - j_col);
            }
        }
        node->h /= 2;/*divided since Manhattan(a, b) == Manhattan(b, a)*/
        if(node->parent) node->f = node->g + node->h;
        else node->f = node->h;
    }
    void make_path(Node* last_node)
    {
        string s = last_node->state;
        Node* p = last_node->parent;
        while(p)
        {
            s = p->state + s;
            p = p->parent;
        }
        movements--;/*because initial state was regarded as a step at the beginning*/
        print_path(s);
        cout << "movements = " << movements << endl;
        cout << "max number of states = " << max_num_nodes << endl;
    }
    void print_path(string result)
    {
        result = regex_replace(result, regex("0"), " ");
        for(int i = 0;i < result.size();i+=48)
        {
            if((i + 48) <= result.size())
            {
                cout << result[i] << " " << result[i+1] << " " << result[i+2] << " " << result[i+3];
                cout << "  ";
                cout << "  " << result[i+16] << " " << result[i+17] << " " << result[i+18]<< " " << result[i+19];
                cout << "  ";
                cout << "  " << result[i+32] << " " << result[i+33] << " " << result[i+34]<< " " << result[i+35];
                cout << endl;

                cout << result[i+4] << " " << result[i+5] << " " << result[i+6]<< " " << result[i+7];
                cout << " -";
                cout << "> " << result[i+20] << " " << result[i+21] << " " << result[i+22]<< " " << result[i+23];
                cout << " -";
                cout << "> " << result[i+36] << " " << result[i+37] << " " << result[i+38]<< " " << result[i+39];
                cout << endl;

                cout << result[i+8] << " " << result[i+9] << " " << result[i+10]<< " " << result[i+11];
                cout << "  ";
                cout << "  " << result[i+24] << " " << result[i+25] << " " << result[i+26]<< " " << result[i+27];
                cout << "  ";
                cout << "  " << result[i+40] << " " << result[i+41] << " " << result[i+42]<< " " << result[i+43];
                cout << endl;

                cout << result[i+12] << " " << result[i+13] << " " << result[i+14]<< " " << result[i+15];
                cout << "  ";
                cout << "  " << result[i+28] << " " << result[i+29] << " " << result[i+30]<< " " << result[i+31];
                cout << "  ";
                cout << "  " << result[i+44] << " " << result[i+45] << " " << result[i+46]<< " " << result[i+47];
                cout << endl;
            }
            else if((i + 32) <= result.size())
            {
                cout << result[i] << " " << result[i+1] << " " << result[i+2] << " " << result[i+3];
                cout << "  ";
                cout << "  " << result[i+16] << " " << result[i+17] << " " << result[i+18]<< " " << result[i+19];
                cout << endl;

                cout << result[i+4] << " " << result[i+5] << " " << result[i+6]<< " " << result[i+7];
                cout << " -";
                cout << "> " << result[i+20] << " " << result[i+21] << " " << result[i+22]<< " " << result[i+23];
                cout << endl;

                cout << result[i+8] << " " << result[i+9] << " " << result[i+10]<< " " << result[i+11];
                cout << "  ";
                cout << "  " << result[i+24] << " " << result[i+25] << " " << result[i+26]<< " " << result[i+27];
                cout << endl;

                cout << result[i+12] << " " << result[i+13] << " " << result[i+14]<< " " << result[i+15];
                cout << "  ";
                cout << "  " << result[i+28] << " " << result[i+29] << " " << result[i+30]<< " " << result[i+31];
                cout << endl;
            }
            else
            {
                cout << result[i] << " " << result[i+1] << " " << result[i+2] << " " << result[i+3];
                cout << endl;

                cout << result[i+4] << " " << result[i+5] << " " << result[i+6]<< " " << result[i+7];
                cout << endl;

                cout << result[i+8] << " " << result[i+9] << " " << result[i+10]<< " " << result[i+11];
                cout << endl;

                cout << result[i+12] << " " << result[i+13] << " " << result[i+14]<< " " << result[i+15];
                cout << endl;
            }
            cout << endl;
        }
    }
    void expand(vector <Node*> &frontier, Node* parent)
    {
        vector<string> states = action(parent->state);
        max_num_nodes += states.size();
        for(int i = 0;i < states.size();i++)
        {
            Node *reg = new Node(states.at(i), parent);
            frontier.push_back(reg);
        }
    }
    vector<string> action(string s)
    {
        string reg;
        vector<string> states;
        short index = s.find("0");
        short row = index / 4;
        short col = index % 4;
        if(row != 0)/*can switch with upper puzzle*/
        {
            reg = s;
            swap(reg[index], reg[index - 4]);
            states.push_back(reg);
        }
        if(row != 3)/*can switch with lower puzzle*/
        {
            reg = s;
            swap(reg[index], reg[index + 4]);
            states.push_back(reg);
        }
        if(col != 0)/*can switch with left puzzle*/
        {
            reg = s;
            swap(reg[index], reg[index - 1]);
            states.push_back(reg);
        }
        if(col != 4)/*can switch with right puzzle*/
        {
            reg = s;
            swap(reg[index], reg[index + 1]);
            states.push_back(reg);
        }
        return states;
    }
};

int main()
{
    string initial = "864570231";
    string goal = "586074231";
    string bonus_initial = "EB5A7CD0824F6391";
    string bonus_question = "7EB58CDA204F6391";
    short method;
    
    cout << "1: Iterative-Deepening Search"  << endl;
    cout << "2: Uniform-Cost Search"  << endl;
    cout << "3: Greedy Best-First Search" << endl;
    cout << "4: A* search"  << endl;
    cout << "5: Recursive Best-First Search"  << endl;
    cout << "6: Bonus Question" << endl;
    cout << "0: exit" << endl;
    cout << "--------------------------" << endl;
    cout << "method = ";
    while(cin >> method)
    {
        if(method == 0) break;
        if(method == 1) IDS solve_ids(initial, goal);
        if(method == 2) UCS solve_ucs(initial, goal);
        if(method == 3) GBFS solve_gbfs(initial, goal);
        if(method == 4) Astar solve_astar(initial, goal);
        if(method == 5) RBFS solve_rbfs(initial, goal);
        if(method == 6)
        {
            cout << "Using A* Search..." << endl;
            SixteenPuzzle solve_bonus(bonus_initial, bonus_question);
        }
        cout << "--------------------------" << endl << "method = ";
    }    
    return 0;
}