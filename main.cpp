#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <queue>
#include <unordered_set>

using namespace std;

void printBoard(string& input){
    for (int i = 0; i < input.length(); ++i) {
        if((i % 4) == 0){
            cout << input[i];
        }
        else if(((i+1) % 4) == 0){
            cout << ' ' << input[i] << '\n';
        }
        else{
            cout << ' ' << input[i];
        }
    }
}

vector<string> getMoves(const string& board){
    int space = board.find('#');
    vector<string> moves;
    if((space-4) >= 0){
        moves.push_back("UP");
    }
    if((space+4) < board.length()){
        moves.push_back("DOWN");
    }
    if(((space-1) >= 0) && ((space % 4) != 0)){
        moves.push_back("LEFT");
    }
    if(((space+1) < board.length()) && ((space+1) % 4) != 0){
        moves.push_back("RIGHT");
    }
    return(moves);
}

class Node{
private:
    int h1Cost(){
        string goalState = "ABCDEFGHIJKLMNO#";
        int icost = 0;
        for (int i = 0; i < goalState.size(); i++) {
            if(goalState[i] != this->state[i]){
                icost++;
            }
        }
        return(icost);
    }

    int h2Cost(){
        string goalState = "ABCDEFGHIJKLMNO#";
        int totalDistance = 0;
        int size = 4;

        for (int i = 0; i < this->state.size(); i++) {
            if(this->state[i] != '#'){
                // Calculate the position (x, y) of the current tile in the state
                int x1 = i / size;
                int y1 = i % size;

                // Find the position (x, y) of the same tile in the goal state
                int x2 = goalState.find(this->state[i]) / size;
                int y2 = goalState.find(this->state[i]) % size;

                // Calculate the Manhattan distance for the current tile and add to totalDistance
                totalDistance += (abs(x1 - x2) + abs(y1 - y2));
            }
        }

        return totalDistance;
    }
public:
    string state;
    Node* prev;
    int g; // Cost from the starting node to this node
    int cost; // The total cost (f = g + h)

    Node(const string& state, Node* prev, int g) : state(state), prev(prev), g(g), cost(g + h2Cost()) {}

    Node(const string &state, Node *prev) : state(state), prev(prev),cost(h2Cost()) {}

    void makeMove(const string& move) {
        size_t space = state.find('#');
        if (move == "UP" && (space - 4) >= 0) {
            swap(state[space], state[space - 4]);
        } else if (move == "DOWN" && (space + 4) < state.length()) {
            swap(state[space], state[space + 4]);
        } else if (move == "LEFT" && (space - 1) >= 0) {
            swap(state[space], state[space - 1]);
        } else if (move == "RIGHT" && (space + 1) < state.length()) {
            swap(state[space], state[space + 1]);
        }
    }

    friend ostream &operator<<(ostream &os, const Node &node) {
        os << "state: " << node.state << " cost: " << node.cost << " prev: " << node.prev;
        return os;
    }
};

bool visited(unordered_set<Node*>& set,Node* node){
    bool flag = false;
    for(const Node* n : set){
        if((node->state == n->state) && (node->cost == n->cost)){
            flag = true;
            break;
        }
    }
    return(flag);
}

// Custom comparison function for the priority queue
struct NodeCompare {
    bool operator()(const Node* lhs, const Node* rhs) const {
        // We want to prioritize nodes with lower cost
        return lhs->cost > rhs->cost;
    }
};

int main() {
    string input;
    cin >> input;
    // BFS
    unordered_set<string> explored;
    Node* node = new Node(input, nullptr, 0);
    explored.insert(input);
    priority_queue<Node*, vector<Node*>, NodeCompare> frontier;
    frontier.push(node);
    while (!frontier.empty()) {
        // Sample new node
        node = frontier.top();
        frontier.pop();
        // Check goal state
        if (node->state == "ABCDEFGHIJKLMNO#") {
            // Compute score back to init
            int cost = 0;
            while (node->prev != nullptr) {
                node = node->prev;
                cost++;
            }
            cout << cost << endl;
            return 0;
        }
        // Generate available moves
        vector<string> moves = getMoves(node->state);
        for (const string& move : moves) {
            // Optimize memory allocation by reusing nodes
            Node* tmpNode = new Node(node->state, node, node->g + 1);
            tmpNode->makeMove(move);
            if (explored.find(tmpNode->state) == explored.end()) {
                frontier.push(tmpNode);
                explored.insert(tmpNode->state);
            }
            else {
                delete (tmpNode);
            }
        }
    }

    return 0;
}
