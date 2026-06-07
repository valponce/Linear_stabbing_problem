
#include <random>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <span>

template <typename Value>

bool is_include(std::pair<Value,Value> I,std::pair<Value,Value> s){
    // I include in s ?
    if (s.first<=I.first && I.second <=s.second){
        return true;
    }
    else {return false;}

}

template <typename Value>
bool intersect(std::pair<Value,Value> I,std::pair<Value,Value> s){
    // attention au interval ouvert à droite
    if ((s.first<I.second && I.first <=s.first) || (s.second<I.second && I.first <=s.second)){
        return true;
    }
    else {return false;}
}

template <typename Value>
class Segment_tree {

private:
    struct node {
        std::vector<std::pair<Value,Value>> _segments;
        std::pair<Value,Value> _interval;
        node* _left;
        node* _right;
        
        
        node(std::pair<Value,Value> I) :_segments({}),_interval(I),_left(nullptr), _right(nullptr){};

        node(std::pair<Value,Value> I,node* left,node* right) :_segments({}),_interval(I),
            _left(left), _right(right){};
    
        ~node() {
            delete _left; delete _right;
        }

        void insert(std::pair<Value,Value> segment){
            if (is_include(_interval,segment)){
                _segments.push_back(segment);
            }
            else{
                if(intersect(_left->_interval,segment)){
                    _left->insert(segment);
                }
                if(intersect(_right->_interval,segment)){
                    _right->insert(segment);
                }
            }
        }

        int query(const Value& q){
            if(_interval.first<=q && q<_interval.second){
                return _segments.size()+_left.query(q)+_right.query(q);
            }
            else {
                return 0;
            }
        }

        void print_node(std::ofstream& out){
            out << "[" << _interval.first<< "," << _interval.second << "]";
            out << " || ";
            out << "(";
            for (int i=0;i<_segments.size();i++){
                out << "[" << _segments[i].first<< "," << _segments[i].second << "],";
            }
            out << ")";
        }

    };
        
    node* root;
    
    void ToDotLinkChildren(node* current, std::ofstream& out) {
        if (current->_left!=nullptr){
            // 2. Lier le nœud à son enfant s'il en a un
            out << "  \"" << current->_left << "\" [label=\""; 
            current->_left->print_node(out);
            out << "\"];\n";
            out << "  \"" << current << "\" -> \"" << current->_left << "\";\n";
                
            // Appel récursif pour la liste des enfants
            ToDotLinkChildren(current->_left, out);
        }
        else{
            // 2. Lier le nœud à son enfant s'il en a un
            out << "  \"" << current->_left << "+"<< current<< "\" [label=\"Empty\", color=\"black\", shape=square];\n";
            out << "  \"" << current << "\" -> \"" << current->_left << "+"<< current << "\";\n";
        }
        if (current->_right!=nullptr){
            // 2. Lier le nœud à son enfant s'il en a un
            out << "  \"" << current->_right << "\" [label=\"";
            current->_right->print_node(out);
            out << "\"];\n";
            out << "  \"" << current << "\" -> \"" << current->_right << "\";\n";
                
            // Appel récursif pour la liste des enfants
            ToDotLinkChildren(current->_right, out);
        }
        else{
            // 2. Lier le nœud à son enfant s'il en a un
            out << "  \"" << current->_right << "-" << current<<"\" [label=\"Empty\", color=\"black\", shape=square];\n";
            out << "  \"" << current << "\" -> \"" << current->_right << "-" << current<< "\";\n";
        }
        
    }

    node* create_tree(std::span<node*> nodes){
        if(nodes.size()==1){
            return new node({nodes[0]->_interval.first,nodes[0]->_interval.second});
        }
        else{
            int n=nodes.size();
            node* current=new node({nodes[0]->_interval.first,nodes[n-1]->_interval.second});
            if ((n/2)%2==0){
                std::span<node*> subleft(nodes.begin(), n/2);
                std::span<node*> subright(nodes.begin()+n/2, n/2);
                current->_left=create_tree(subleft);
                current->_right=create_tree(subright);
            }
            else{
                std::span<node*> subleft(nodes.begin(), n/2+1);
                std::span<node*> subright(nodes.begin()+n/2, n/2-1);
                current->_left=create_tree(subleft);
                current->_right=create_tree(subright);
            }
            return current; 
        }         
    }

    void insert_ST(std::vector<Value> segments,int n){
        for (int i=0; i<n;i+=2){
            root->insert({segments[i],segments[i+1]});
        }
    }

public:
    Segment_tree(std::vector<Value> intervals){
        if (intervals.empty()){
            root=nullptr;
        }
        else{
            int n=intervals.size(); // forcément pair 
            std::vector<Value> list;
            list.reserve(n);
            std::copy(intervals.begin(), intervals.end(),std::back_inserter(list));
            std::sort(list.begin(),list.end());
            auto last=std::unique(list.begin(),list.end());
            list.erase(last, list.end());

            std::vector<node*> nodes;
            int m=list.size();
            for (int k=0; k<m-1;k++){
                nodes.push_back(new node({list[k],list[k]}));
                nodes.push_back(new node({list[k],list[k+1]}));
            }
            nodes.push_back(new node({list[m-1],list[m-1]}));
            nodes.push_back(new node({list[m-1],INFINITY}));
            root=create_tree(nodes);
            insert_ST(intervals,n);
        }
    }

    ~Segment_tree() {
        delete root;
    }

    int query_IT(const Value& q){
        if (root==nullptr){
            return 0;
        }
        else{
            return root->query(q);
        }
    }
        
    void ExportToDot(const std::string& filename) {
        std::ofstream out(filename);
        if (!out.is_open()){
            std::cerr<< "File not open"<<std::endl;
            return;
        }
        
        out << "digraph Intervaltree {\n";
        out << "  rankdir=TB;\n"; // Du haut vers le bas
        out << "  node [shape=circle];\n";

        if (root==nullptr) {
            out << "  Empty [label=\"Heap Vide\", shape=none];\n";
        } else {            
            // On lance le parcours à la racine
            out << "  \"" << root << "\" [label=\""; 
            root->print_node(out);
            out << "\"];\n";
            ToDotLinkChildren(root,out);
        }

        out << "}\n";
        out.close();

    }
};