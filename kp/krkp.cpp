#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <algorithm>
#include <string>

using namespace std;

struct Node {
    vector<int> heirs;
    bool busy = false;
    string work = "not job";
};
typedef int (*FnPtr)();
map<int, Node> graph;
map<string, FnPtr> myF;  

bool isCyclicUtil (int v, bool visited[], bool in_rec_stack[]) {
    if (!visited[v]) {
        visited[v] = true;
        in_rec_stack[v] = true;

        for (int i = 0; i < graph[v].heirs.size(); i++) {
            int n = graph[v].heirs[i];
            if (!visited[n] && isCyclicUtil(n, visited, in_rec_stack)) {
                return true;
            } else if (in_rec_stack[n]) {
                return true;
            }
        }

    }
    in_rec_stack[v] = false;
    return false;
}

bool isCyclic() {
    bool *visited = new bool[graph.size()];
    bool *in_rec_stack = new bool[graph.size()];
    for (int i = 0; i < graph.size(); i++) {
        visited[i] = false;
        in_rec_stack[i] = false;
    }

    for (int i = 0; i < graph.size(); i++) {
        if (isCyclicUtil(i, visited, in_rec_stack)) {
            return true;
        }
    }
    return false;
}

bool isConnected(std::vector<int>& endnods) {
    int count_out;
    int id_node;
    bool found;
    for (auto & pair : graph) {
        id_node = pair.first; 
        count_out = pair.second.heirs.size();
        found = std::find(endnods.begin(), endnods.end(), id_node) != endnods.end();
        if (count_out > 1 && found == false)
            return false;
    }
    return true;
}


int F1() { return 1; }
int F2() { return 22; }
int F3() { 
    int i; 
    cout << "Введите число: "; 
    cin >> i; 
    if(i == -1) 
        return -1; 
    else 
        return i;
    }
int F4() { return 444; }
typedef int (*FnPtr)();



void all_work(std::vector<int>& startNodes){
    queue<int> queue;
    int id_work_node, id_work_node_next, size;
    Node *work_node;
    string work;
    Node *next_work_node;
    int res;
    for (int i = 0; i < startNodes.size(); i++){
        queue.push(startNodes[i]);
    }
        for (int i = 0; i < graph.size() - 1 ; i++){
            id_work_node = queue.front();
            cout << "JOB: " << id_work_node << endl;
            queue.pop();
            work_node = &(graph.find(id_work_node) -> second);
            work = work_node->work;
            if(work != "not job"){
                res = myF[work]();
                if (res == -1){
                    cout << "error" << endl;
                    break;
                }
                cout << res << endl;
            }
            else{
                cout << work << endl;
            }
            work_node->busy = true;
            size = work_node->heirs.size();
            if(size != 0){
                id_work_node_next = work_node->heirs.front();
                next_work_node = &(graph.find(id_work_node_next) -> second);
                if(next_work_node->busy == false){
                    next_work_node->busy = true;
                    queue.push(id_work_node_next);
                }
            }
        }
        
}

int main() {
    ifstream configFile("config1.xml");
    string line;
    vector<int> startNodes;
    vector<int> endNodes;
    int id;
    Node *work_node_now;
    myF["F1"] = F1;
    myF["F2"] = F2;
    myF["F3"] = F3;
    myF["F4"] = F4;
    while (getline(configFile, line)) {
        if (line.find('=') != string::npos) {
            istringstream is(line);
            string key, value;
            if (getline(is, key, '=') && getline(is, value)) {
                if (key == "NODE") {
                    Node node;
                    id = stoi(value);
                    graph[stoi(value)] = node;
                } else if (key == "CONNECTION") {
                    int from, to;
                    istringstream(value) >> from >> to;
                    graph[from].heirs.push_back(to);
                } else if (key == "START_NODE") {
                    startNodes.push_back(stoi(value));
                } else if (key == "END_NODE") {
                    endNodes.push_back(stoi(value));
                } else if (key == "JOB") {
                    work_node_now = &(graph.find(id) -> second);
                    work_node_now->work = value;
                }
            }
        }
    }
    configFile.close();
    if (!isConnected(endNodes)) {
        cout << "Граф не связан или пересвязан" << endl;return 1;}
    if (isCyclic()) {
        cout << "Граф содержит циклы" << endl;return 1;}
    if (startNodes.empty() || endNodes.empty()) {
        cout << "Отсутствуют стартовые или завершающие узлы" << endl;   return 1;}
    all_work(startNodes);
    return 0;
}