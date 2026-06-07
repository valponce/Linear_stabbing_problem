#include "Interval_tree.h"
#include <vector>

int main(int argc, char* argv[]){
    if (argc != 1) {
        std::cout<< "Invalid number of arguments"<<std::endl;
        return 1;
    }
    std::vector<int> intervals={1,6,3,20,3,7,5,17,10,20,13,15};
    Interval_tree kdT(intervals);
    kdT.ExportToDot("interval_tree.txt");
}