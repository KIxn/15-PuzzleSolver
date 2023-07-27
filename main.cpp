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
public:
    string state;
    Node* prev;

    Node(const string &state, Node *prev) : state(state), prev(prev) {}

    bool operator==(const Node &rhs) const {
        return state == rhs.state;
    }

    bool operator!=(const Node &rhs) const {
        return !(rhs.state == this->state);
    }

    bool operator<(const Node &rhs) const {
        return state < rhs.state;
    }

    bool operator>(const Node &rhs) const {
        return rhs.state < this->state;
    }

    bool operator<=(const Node &rhs) const {
        return !(rhs.state < this->state);
    }

    bool operator>=(const Node &rhs) const {
        return !(this->state < rhs.state);
    }

    friend ostream &operator<<(ostream &os, const Node &node) {
        os << "state: " << node.state << " prev: " << node.prev;
        return os;
    }
};

bool visited(vector<Node*>& set,Node* node){
    bool flag = false;
    for(const Node* n : set){
        if(node->state == n->state){
            flag = true;
            break;
        }
    }
    return(flag);
}

int main() {
    string input;
    cin >> input;
    // BFS
    vector<Node*> explored;
    Node* node = new Node(input,nullptr);
    explored.push_back(node);
    queue<Node*> frontier;
    frontier.push(node);
    while(!frontier.empty()){
        //sample new node
        node = frontier.front();
        frontier.pop();
        //check goal state
        if(node->state == "ABCDEFGHIJKLMNO#"){
            //compute score back to init
            int cost = 0;
            while(node->prev != nullptr){
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
            if(!binary_search(explored.begin(),explored.end(),tmpNode)){
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
