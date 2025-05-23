#include <iostream>
#include <vector>
using namespace std;

const int M=5;

template<typename TK>
struct Node{
    TK *keys;
    Node** children;
    int count;//contador de llaves
    bool leaf;

    Node(initializer_list<TK> elements){
        this->leaf = true;
        this->keys = new TK[M-1];
        this->children = new Node<TK>*[M];    
        this->count = 0;                
        for(auto it = elements.begin(); it != elements.end(); ++it){
            this->keys[this->count] = *it;
            this->children[this->count] = nullptr;
            this->count++;
        }
        this->children[this->count] = nullptr; 
    }

    void link_child(int index_child, initializer_list<TK> elements){
        this->leaf = false;
        this->children[index_child] = new Node(elements);
    }

    ~Node(){
        for(int i = 0; i<count; i++){
            if(!leaf) delete children[i];
        }
        if(!leaf) delete children[count];
        delete[] keys;
        delete[] children;
    }
};

template<typename T>
class BTree{
private:
    Node<T>* root;
public:
    BTree(Node<T>* root=nullptr){
        this->root = root;
    }

    ~BTree(){
        delete root;
    }

    // Mostrar el árbol en orden
    void toString(Node<T>* nodo, string &result){
        int i= 0;
        for (; i < nodo->count; i++)
        {
            if(!nodo->leaf) toString(nodo->children[i], result);
            result = std::to_string(nodo->keys[i])+",";
        }
        if(!nodo->leaf) toString(nodo->keys[i], result);
    }
    string toString(){
        string result = "";
        toString(root, result);
        return result;
    }

    // Implementar el método search con complejidad O(log n)
    bool search(T key){
        Node<T> *actual = root;
        while(actual!= nullptr){
            int i = 0;
            while(i<actual->count and key > actual->keys[i]){
                i++;
            }
            if(i<actual->count and key == actual->keys[i]) return true;
            if(actual->isLeaf) return false;
            actual = actual->children[i];

        }
        return false;
        
    }

    // Implementar el método range_search con complejidad O(k * log n )
    vector<int> range_search(T begin, T end){
        //TODO
    }

    // Verifique las propiedades de un árbol B
    bool check_properties(){
        //TODO
        //1- cada nodo debe tener al menos M/2
        //2- garantizar que las hojas esten al mismo nivel
        //   cada nodo debe tener count+1 hijos
        //3- los elementos en el nodo deben estar ordenados
        //4- dado un elemento en un nodo interno:
        //    - los elemenos del subarbol izquierdo son menores 
        //    - los elemenos del subarbol derecho son mayores
    }

    // Construya un árbol B a partir de un vector de elementos ordenados
    static BTree* build_from_ordered_vector(vector<T> elements){
        //TODO
    }

private:   
    
    void toString(Node<T>* nodo, string &result){
        int i=0;
        for (; i < nodo->count; i++)  {
            if (!nodo->leaf) toString(nodo->children[i], result);
            result += std::to_string(nodo->keys[i]) + ",";
        }
        if (!nodo->leaf) toString(nodo->children[i], result);
    }
   
};

void test1(){
    Node<int>* root = new Node<int>({11,16,20,31});
    root->link_child(0, {3,5});
    root->link_child(1, {12,15});
    root->link_child(2, {17,19});
    root->link_child(3, {22,25,27,28});
    root->link_child(4, {32,36,37,38});
    BTree<int>* btree = new BTree<int>(root);

    cout<<"25:"<<btree->search(25)<<endl; //25:1
    cout<<"16:"<<btree->search(16)<<endl; //16:1
    cout<<"35:"<<btree->search(35)<<endl; //35:0

    cout<<btree->toString()<<endl;//3,5,11,12,15,16,17,19,20,22,25,27,28,31,32,36,37,38 
    
    vector<int> result = btree->range_search(14, 20);
    for(auto elem : result) cout<<elem<<", "; //15,16,17,19,20
    cout<<endl; 
    delete btree;
}

void test2(){
    Node<int>* root = new Node<int>({20});
    root->link_child(0, {11,16});
    root->link_child(1, {31,35});
    root->children[0]->link_child(0, {3,5});
    root->children[0]->link_child(1, {12,15});
    root->children[0]->link_child(2, {17,19});
    root->children[1]->link_child(0, {22,25,27,28});
    root->children[1]->link_child(1, {32,33});
    root->children[1]->link_child(2, {36,38});
    BTree<int>* btree = new BTree<int>(root);

    cout<<"25:"<<btree->search(25)<<endl; //25:1
    cout<<"16:"<<btree->search(16)<<endl; //16:1
    cout<<"37:"<<btree->search(37)<<endl; //37:0

    cout<<btree->toString()<<endl;//3,5,11,12,15,16,17,19,20,22,25,27,28,31,32,33,35,36,38  
    vector<int> result = btree->range_search(14, 21);
    for(auto elem : result) cout<<elem<<", "; //15,16,17,19,20
    cout<<endl;     
    delete btree;
}

void test3(){
    std::vector<int> elements = {1,2,3,4,5,6,7,8,9,10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    BTree<int>* btree = BTree<int>::build_from_ordered_vector(elements);
    if(btree->check_properties()){
        cout<<"El árbol cumple con las propiedades de un árbol B."<<endl;
        cout<<btree->toString()<<endl; //1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20
    }else{
        cout<<"El árbol no cumple con las propiedades de un árbol B."<<endl;
    }    
    delete btree;
}

int main()
{
    cout<<"Test 1"<<endl;
    test1();
    cout<<"Test 2"<<endl;
    test2();
    cout<<"Test 3"<<endl;
    test3();
    return 0;
}