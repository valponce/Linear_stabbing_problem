#include "Interval_tree.h"
#include "Segment_tree.h"
#include <vector>
#include <fstream>

std::vector<int> generate_random_intervals(int n,int d){
    static std::random_device rd;
    static std::mt19937 gen(rd());
    
    std::vector<int> intervals;
    intervals.reserve(2*n);
    std::uniform_int_distribution<int> dis_start(0, 1000000);
    std::uniform_int_distribution<int> dis_length(0, 2*d*1000000/n);
    for (int k=0;k<n;k++){
        int start=dis_start(gen);
        int l=dis_length(gen);
        intervals.push_back(start);
        intervals.push_back(start+l);
    }
    
    return intervals;
}

int main(int argc, char* argv[]){
    if (argc != 3) {
        std::cout<< "Invalid number of arguments"<<std::endl;
        return 1;
    }
    int d=atoi(argv[1]); // density
    int M=atoi(argv[2]); // number of repeats
    
    int nb_intervals[10]={50,100,250,500,750,1000,2500,5000,7500,10000};
    std::cout << "Number of intervals, Interval Tree, Segment Tree" <<std::endl; 
    for (auto& n:nb_intervals){
        int space_IT=0;
        int space_ST=0;
        if (n>1000){M=100;}
        for (int k=0;k<M;k++){
            std::vector<int> I =generate_random_intervals(n,d);
            Interval_tree IT(I);
            space_IT+=IT.space_IT(); 
            Segment_tree ST(I);
            space_ST+=ST.space; 
        }
        std::cout << n<< "," << float(space_IT)/float(M) << "," << float(space_ST)/float(M) <<std::endl; 
    }
    return 0;
    //std::vector<int> intervals={1,6,3,20,3,7,5,17,10,20,13,15};
    
    //std::cout << IT.query_IT(18) <<std::endl;
    //IT.ExportToDot("interval_tree.txt");
}