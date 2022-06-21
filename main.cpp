#include <iostream>
#include <vector>

// node can be of type fork or leaf
enum Type {F, L};

// tree structure
struct Node{
    int value; // represents the data held by the node (could be mass, position, etc.)
    Type type;
    struct Node *nw;
    struct Node *ne;
    struct Node *se;
    struct Node *sw;
};

// flat node
struct NodeSerialized{
    int value; // represents the data held by the node (could be mass, position, etc.)
    Type type;
    int children[4]; // link to the array positions of the 4 possible children of the node
};

// creates a new node with no children
Node* newNode(int key, Type t){
    Node* temp = new Node;
    temp->value = key;
    temp->type = t;
    temp->nw = temp->sw = temp->ne = temp->se = NULL;
    return (temp);
}

// creates a serialized node with no children (the array of children is set to -1)
NodeSerialized newNodeSerialized(int key, Type t){
    NodeSerialized ns = {key, t, {-1,-1,-1,-1}};
    return ns;
}

// function creating a tree example
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

// pre-order DFS of the tree to print in console
void printTraverseTree(Node* root){
    std::cout << "(" << root->value << ", " << (root->type == L ? "L" : "F") << ")"<<std::endl;

    // if the node is a leaf then I can stop because it means there are no more children
    if(root->type == L){
        return;

    //otherwise it's a fork so I need to visit the 4 children
    }else{
        printTraverseTree(root->nw);
        printTraverseTree(root->ne);
        printTraverseTree(root->se);
        printTraverseTree(root->sw);
    }

}

// serial traversal of the vector to print in console
void printVect(std::vector<NodeSerialized> values){
    for(int i = 0; i < values.size(); i++){
        std::cout << "id: " << i << " {" << values[i].value << ", " << (values[i].type == L ? "L" : "F") << "} [";
        for(int j = 0; j < 4; j++){
            std::cout << " " << values[i].children[j];
        }
        std::cout << "]" << std::endl;
    }
}


/**
 * When called from main set id as 0 to read the tree from the start of the vector.
 *
 * @param root a new empy node that will act as the root of the sub-tree analyzed
 * @param serializedTree the vector containing the serialized representation of the tree
 * @param id current index of the node in the array
 * @return tree(root)
 */
Node* createTreeFromVector(Node* root, std::vector<NodeSerialized> serializedTree, int id){
    //create a new node based on the serial one in serializedTree[id]
    root->value = serializedTree[id].value;
    root->type = serializedTree[id].type;

    // for now set children to null
    root->nw = root->sw = root->ne = root->se = NULL;

    // if the node is a fork I make a recursive call on the children (represented in the array of children)
    if(root->type == F){
        // notice how I pass as the new vector id of the function the id of the children
        root->nw = createTreeFromVector(new Node, serializedTree, serializedTree[id].children[0]);
        root->ne = createTreeFromVector(new Node, serializedTree, serializedTree[id].children[1]);
        root->se = createTreeFromVector(new Node, serializedTree, serializedTree[id].children[2]);
        root->sw = createTreeFromVector(new Node, serializedTree, serializedTree[id].children[3]);
    }
    return root;
}

/**
 * When called, it will build the representation of the tree inside of a vector
 *
 * @param root current root of the sub-tree being traversed
 * @param serializedTree vector that is going to contain the serialized nodes
 * @return currentId ,it's the id in which the node is going to be saved inside the array (the main output is 0 because it starts at the root)
 */
int createvect(Node* root, std::vector<NodeSerialized>& serializedTree){
    // I create the serialized version of the node
    NodeSerialized newNode = newNodeSerialized(root->value, root->type);

    // I save the current vector id in which the serialized node is going to be saved
    int currentId = serializedTree.size();
    // I push the data inside the vector
    serializedTree.push_back(newNode);

    // this variable is going to temporarily store the vector ids in which the children will end up
    int sonId = -1;
    // if the node is a fork I need to save the children in the vector and obtain their position id inside the vector
    if(root->type == F){
        // note that the recursive call is going to return the id position in which the children node is going to be saved
        sonId = createvect(root->nw, serializedTree);
        // I save the children id
        serializedTree[currentId].children[0] = sonId;
        sonId = createvect(root->ne, serializedTree);
        serializedTree[currentId].children[1] = sonId;
        sonId = createvect(root->se, serializedTree);
        serializedTree[currentId].children[2] = sonId;
        sonId = createvect(root->sw, serializedTree);
        serializedTree[currentId].children[3] = sonId;
    }

    // I return the node id in the vector
    return currentId;
}

int main() {
    std::cout << "FIRST ITERATION" << std::endl;
    // I create an example tree
    Node* root = createTree();
    // I print it in console
    printTraverseTree(root);

    // I create a vector and save inside of it the structure of the tree
    std::vector<NodeSerialized> values;
    createvect(root, values);
    // I print the vector in console
    printVect(values);

    // reverse process:
    // I construct the tree from the vector and then print them
    std::cout << "SECOND ITERATION" << std::endl;
    Node* root2 = createTreeFromVector(new Node, values, 0);
    printTraverseTree(root2);
    std::vector<NodeSerialized> values2;
    createvect(root2, values2);
    printVect(values2);
    return 0;
}
