#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <queue>

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


string makeMove(const string& board,const string& move){
    string board2;
    board2.assign(board);

    size_t space = board2.find('#');
    if(move == "UP"){
        if((space-4) >= 0){
            swap(board2[space], board2[space - 4]);
            return(board2);
        }
    }
    else if(move == "DOWN"){
        if((space+4) < board2.length()){
            swap(board2[space], board2[space + 4]);
            return(board2);
        }
    }
    else if(move == "LEFT"){
        if((space-1) >= 0){
            swap(board2[space], board2[space - 1]);
            return(board2);
        }
    }
    else if(move == "RIGHT"){
        if((space+1) < board2.length()){
            swap(board2[space], board2[space + 1]);
            return(board2);
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

    // Utility function to calculate Manhattan distance between two positions
    int manhattanDistance(int x1, int y1, int x2, int y2) {
        return (abs(x1 - x2) + abs(y1 - y2));
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
                totalDistance += manhattanDistance(x1, y1, x2, y2);
            }
        }

        return totalDistance;
    }
public:
    string state;
    Node* prev;
    int cost;

    Node(const string &state, Node *prev) : state(state), prev(prev),cost(h2Cost()) {}

//    bool operator==(const Node &rhs) const {
//        return ((state == rhs.state) && (cost == rhs.cost));
//    }
//
//    bool operator!=(const Node &rhs) const {
//        return ((rhs.state != this->state) && (rhs.cost != this->cost));
//    }
//
//    bool operator<(const Node &rhs) const {
//        return ((state < rhs.state) && (cost < rhs.cost));
//    }
//
//    bool operator>(const Node &rhs) const {
//        return((rhs.state < this->state) && (rhs.cost < this->cost));
//    }
//
//    bool operator<=(const Node &rhs) const {
//        return (!(rhs.state < this->state) && (rhs.cost < this->cost));
//    }
//
//    bool operator>=(const Node &rhs) const {
//        return (!(this->state < rhs.state) && (this->cost < rhs.cost));
//    }

    friend ostream &operator<<(ostream &os, const Node &node) {
        os << "state: " << node.state << " cost: " << node.cost << " prev: " << node.prev;
        return os;
    }
};

bool visited(vector<Node*>& set,Node* node){
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
    vector<Node*> explored;
    Node* node = new Node(input,nullptr);
    explored.push_back(node);
    priority_queue<Node*,vector<Node*>,NodeCompare> frontier;
    frontier.push(node);
    while(!frontier.empty()){
        //sample new node
        node = frontier.top();
        frontier.pop();
        //check goal state
        if(node->state == "ABCDEFGHIJKLMNO#"){
//            printf("GOAL FOUND!\n");
            //compute score back to init
            int cost = 0;
            while(node->prev != nullptr){
//                printBoard(node->state);
//                cout << endl;
                node = node->prev;
                cost++;
            }
            cout << cost << endl;
            return 0;
        }
        //generate available moves
        vector<string> moves = getMoves(node->state);
        for (const string& move : moves){
            string newState = makeMove(node->state,move);
            Node* tmpNode = new Node(newState,node);
            if(!visited(explored,tmpNode)){
                frontier.push(tmpNode);
                explored.push_back(tmpNode);
            }
            else{
                delete(tmpNode);
            }
        }
    }

    return 0;
}
