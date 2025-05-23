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
        for(int i = 0; i < count; i++){
            if(!leaf) delete children[i];
        }
        if(!leaf) delete children[count];
        //liberar los arrays de datos
        delete [] keys;
        delete [] children;
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
        // TODO: liberar memoria de root
    }

    // Mostrar el árbol en orden
    string toString(){
        string result = "";
        toString(root, result);
        return result;
    }

    // Implementar el método search con complejidad O(log n)
    bool search(T key){
        //TODO
    }    

    // Implementar el método range_search con complejidad O(k * log n )
    vector<int> range_search(T begin, T end){
        //TODO
    }

    bool check_node(Node<T>* node,
                    int depth,
                    int &leaf_level,
                    bool has_min, T min_val,
                    bool has_max, T max_val,
                    bool is_root)
    {
        int t = (M+1)/2;

        // 1) número de claves
        int min_keys = 0;

        if(is_root) min_keys = 1;
        else min_keys = t - 1;

        if (node->count < min_keys || node->count > M - 1)
            return false;

        // 3) claves ordenadas
        for (int i = 0; i + 1 < node->count; ++i) {
            if (!(node->keys[i] < node->keys[i+1]))
                return false;
        }

        if (node->leaf) {
            // 2) hojas al mismo nivel
            if (leaf_level == -1) {
                leaf_level = depth;
            } else if (leaf_level != depth) {
                return false;
            }
            return true;
        } else {
            // 2b) cada nodo interno tiene count+1 hijos
            int ccount = node->count + 1;

            // verificamos que los punteros existan
            for (int i = 0; i < ccount; ++i) {
                if (!node->children[i]) return false;
            }

            // 4) Los elementos del subarbol izquierdo son menores / Los elementos del subarbol derecho son mayores

            for (int i = 0; i < ccount; ++i) {
                bool child_has_min = has_min || (i > 0);
                T child_min = (i > 0 ? node->keys[i-1] : min_val);
                bool child_has_max = has_max || (i < node->count);
                T child_max = (i < node->count ? node->keys[i] : max_val);
                if (!check_node(node->children[i],
                                depth + 1,
                                leaf_level,
                                child_has_min, child_min,
                                child_has_max, child_max,
                        /*is_root=*/false))
                {
                    return false;
                }
            }
            return true;
        }
    }

    // Verifique las propiedades de un árbol B
    bool check_properties(){
        if (!root) return true;
        int leaf_level = -1;
        return check_node(root, 0, leaf_level, false, T(), false, T(), true);
    }

    // Construya un árbol B a partir de un vector de elementos ordenados
    static BTree* build_from_ordered_vector(vector<T> elements){
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
    cout<<btree->toString()<<endl;

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