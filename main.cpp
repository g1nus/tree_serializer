#include <iostream>
#include <vector>

enum Type {F, L};

struct NodeSerialized{
    int value;
    Type type;
    int sons[4];
};

struct Node{
    int value;
    Type type;
    struct Node *nw;
    struct Node *ne;
    struct Node *se;
    struct Node *sw;
};

NodeSerialized newNodeSerialized(int key, Type t){
    NodeSerialized ns = {key, t, {-1,-1,-1,-1}};
    return ns;
}

Node* newNode(int key, Type t)
{
    Node* temp = new Node;
    temp->value = key;
    temp->type = t;
    temp->nw = temp->sw = temp->ne = temp->se = NULL;
    return (temp);
}

Node* createTree(){
    Node *root           = newNode(0, F);
    root->nw             = newNode(1, F);
    root->nw->nw         = newNode(2, L);
    root->nw->ne         = newNode(3, L);
    root->nw->se         = newNode(4, F);
    root->nw->se->nw     = newNode(5, L);
    root->nw->se->ne     = newNode(6, L);
    root->nw->se->se     = newNode(7, L);
    root->nw->se->sw     = newNode(8, L);
    root->nw->sw         = newNode(9, L);
    root->ne             = newNode(10, L);
    root->se             = newNode(11, L);
    root->sw             = newNode(12, F);
    root->sw->nw         = newNode(13, L);
    root->sw->ne         = newNode(14, L);
    root->sw->se         = newNode(15, L);
    root->sw->sw         = newNode(16, F);
    root->sw->sw->nw     = newNode(17, L);
    root->sw->sw->ne     = newNode(18, L);
    root->sw->sw->se     = newNode(19, L);
    root->sw->sw->sw     = newNode(20, L);

    return root;
}

void traverse_tree(Node* root){
    std::cout << "(" << root->value << ", " << (root->type == L ? "L" : "F") << ")"<<std::endl;
    if(root->type == L){
        return;
    }else{
        traverse_tree(root->nw); traverse_tree(root->ne);
        traverse_tree(root->se); traverse_tree(root->sw);
    }

}

int createvect(Node* root, std::vector<NodeSerialized>& values){
    NodeSerialized newNode = newNodeSerialized(root->value, root->type);
    int my_id = values.size();
    values.push_back(newNode);
    int son_id = -1;
    if(root->type != L){
        son_id = createvect(root->nw, values);
        values[my_id].sons[0] = son_id;
        son_id = createvect(root->ne, values);
        values[my_id].sons[1] = son_id;
        son_id = createvect(root->se, values);
        values[my_id].sons[2] = son_id;
        son_id = createvect(root->sw, values);
        values[my_id].sons[3] = son_id;
    }
    return my_id;
}

/*void createvect(Node* root, std::vector<NodeSerialized>& values){
    values.push_back(newNodeSerialized(root->value, root->type));
    if(root->type == L){
        return;
    }else{
        createvect(root->nw, values); createvect(root->ne, values);
        createvect(root->se, values); createvect(root->sw, values);
    }
}*/

void printvect(std::vector<NodeSerialized> values){
    for(int i = 0; i < values.size(); i++){
        std::cout << "id: " << i << " {" << values[i].value << ", " << (values[i].type == L ? "L" : "F") << "} [";
        for(int j = 0; j < 4; j++){
            std::cout << " " << values[i].sons[j];
        }
        std::cout << "]" << std::endl;
    }
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    Node* root = createTree();
    traverse_tree(root);

    std::vector<NodeSerialized> values;
    createvect(root, values);
    printvect(values);
    return 0;
}
