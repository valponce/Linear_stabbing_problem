
#include <random>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
template <typename Value>

class Interval_tree {

private:
    struct node {
        Value median;
        std::vector<std::pair<Value,Value>> _left_endpoints;
        std::vector<std::pair<Value,Value>> _right_endpoints;
        node* _left;
        node* _right;
        
        node() : median(0),_left_endpoints({}), _right_endpoints({}), _left(nullptr), _right(nullptr) {}
        node(const Value& m,std::vector<Value> l_end, std::vector<Value> r_end ,node *left=nullptr,node *right=nullptr) :
            median(m),_left_endpoints(l_end), _right_endpoints(r_end), _left(left), _right(right) {}
    
        ~node() {
            delete _left; delete _right;
        }

        void print_node(std::ofstream& out){
            out << "(";
            for (int i=0;i<_left_endpoints.size();i++){
                out << "[" << _left_endpoints[i].first<< "," << _left_endpoints[i].second << "],";
            }
            out << ") ||";
            out << "(";
            for (int i=0;i<_right_endpoints.size();i++){
                out << "[" << _right_endpoints[i].first<< "," << _right_endpoints[i].second << "],";
            }
            out << ")";
        }

        int query(const Value& q){
            int op=0;
            if(q<median){
                do{
                 op++;
                }
                while (_left_endpoints[op-1].first<=q);
                if (_left!=nullptr){
                    op+=_left->query(q);
                } 
            }
            else if(q>median){
                do{
                 op++;
                }
                while (_right_endpoints[op-1].second>q);
                if (_right!=nullptr){
                    op+=_right->query(q);
                }
            }
            return op;
        }

    };

    Value median(std::vector<Value> intervals){
        int n=intervals.size(); // forcément pair 
        std::vector<Value> list;
        list.reserve(n);
        std::copy(intervals.begin(), intervals.end(),std::back_inserter(list));
        std::sort(list.begin(),list.end());

        return list[n/2]; //median
    }

    node* create_tree(std::vector<Value> intervals){
        if (intervals.empty()){
            return nullptr;
        }
        else{
            Value m=median(intervals);
            int n=intervals.size();
            node* current=new node();
            current->median=m;
            std::vector<Value> left;
            std::vector<Value> right;
            for (int i=0;i<n;i+=2){
                if(intervals[i]<=m && m<=intervals[i+1]){
                    current->_left_endpoints.push_back({intervals[i],intervals[i+1]});
                    current->_right_endpoints.push_back({intervals[i],intervals[i+1]});
                }
                else if (intervals[i+1]<m){
                    left.push_back(intervals[i]);
                    left.push_back(intervals[i+1]);
                }
                else{
                    right.push_back(intervals[i]);
                    right.push_back(intervals[i+1]);
                }
            }
            std::sort(current->_left_endpoints.begin(),current->_left_endpoints.end());
            std::sort(current->_right_endpoints.begin(),current->_right_endpoints.end(),
                [](const auto& a, const auto& b) {return a.second > b.second; });
            current->_left=create_tree(left);
            current->_right=create_tree(right);
            return current;
        }
        
    }
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

public:
    Interval_tree(std::vector<Value> intervals){
        if (intervals.empty()){
            root=nullptr;
        }
        else{
            root=create_tree(intervals);
        }
    }

    ~Interval_tree() {
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