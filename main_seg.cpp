#include "Segment_tree.h"
#include <vector>
#include <fstream>

int main(int argc, char* argv[]){
    if (argc != 1) {
        std::cout<< "Invalid number of arguments"<<std::endl;
        return 1;
    }
    std::vector<int> intervals={1,6,3,20,3,7,5,17,10,20,13,15};
    Segment_tree ST(intervals);
    //std::cout << ST.query_IT(18) <<std::endl;
    ST.ExportToDot("segment_tree.txt");
}