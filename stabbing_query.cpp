#include "Segment_tree.h"
#include "Interval_tree.h"
#include <vector>
#include <fstream>

std::vector<int> generate_center_intervals(int n,int max_endpoint){
    static std::random_device rd;
    static std::mt19937 gen(rd());
    
    std::vector<int> intervals;
    intervals.reserve(2*n);
    std::uniform_int_distribution<int> dis_start(0, max_endpoint/2);
    std::uniform_int_distribution<int> dis_end(max_endpoint/2,max_endpoint);
    for (int k=0;k<n;k++){
        intervals.push_back(dis_start(gen));
        intervals.push_back(dis_end(gen));
    }
    return intervals;
}

std::vector<int> generate_random_intervals(int n,int d,int start,int end){
    static std::random_device rd;
    static std::mt19937 gen(rd());
    
    std::vector<int> intervals;
    intervals.reserve(2*n);
    // create intervals to the left of the middle of the range
    std::uniform_int_distribution<int> dis_start(start, end);
    std::uniform_int_distribution<int> dis_length(0, 2*d*(end-start)/n);
    for (int k=0;k<n;k++){
        int s=dis_start(gen);
        int l=dis_length(gen);
        intervals.push_back(s);
        intervals.push_back(s+l);
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
    int max_endpoint=1000000;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis_query(0, max_endpoint);
    
    int nb_intervals[10]={50,100,250,500,750,1000,2500,5000,7500,10000};
    std::cout << "Number of intervals, Interval Tree with center intervals, Segment Tree with center intervals,";
    std::cout <<"Interval Tree with miss, Segment Tree with miss, Normal Interval Tree, Normal Segment Tree" <<std::endl; 
    for (auto& n:nb_intervals){
        int time_ITc=0;
        int time_STc=0;
        int time_ITmiss=0;
        int time_STmiss=0;
        int time_IT=0;
        int time_ST=0;
        if (n>1000){M=std::min(100,M);}
        for (int k=0;k<M;k++){
            //1. Center Interval
            std::vector<int> Ic =generate_center_intervals(n,max_endpoint);
            Interval_tree ITc(Ic);
            time_ITc+=ITc.query_IT(max_endpoint/2); 
            Segment_tree STc(Ic);
            time_STc+=STc.query_IT(max_endpoint/2); 

            //2. Miss queries
            std::vector<int> Imiss;
            // We generate half of the intervals to the left of the middle of the range and half to the right.
            std::vector<int> first_half = generate_random_intervals(n/2, d,0,max_endpoint/2-1);
            Imiss.insert(Imiss.end(), first_half.begin(), first_half.end());
            std::vector<int> second_half = generate_random_intervals(n-n/2,d,max_endpoint/2+1,max_endpoint);
            Imiss.insert(Imiss.end(), second_half.begin(), second_half.end());
            Interval_tree ITmiss(Imiss);
            time_ITmiss+=ITmiss.query_IT(max_endpoint/2); 
            Segment_tree STmiss(Imiss);
            time_STmiss+=STmiss.query_IT(max_endpoint/2);

            //3. Normal random queries
            std::vector<int> I =generate_random_intervals(n,d,0,max_endpoint);
            Interval_tree IT(I);
            time_IT+=IT.query_IT(dis_query(gen)); 
            Segment_tree ST(I);
            time_ST+=ST.query_IT(dis_query(gen));
        }
        std::cout << n<< "," << float(time_ITc)/float(M) << "," << float(time_STc)/float(M); 
        std::cout << "," << float(time_ITmiss)/float(M) << "," << float(time_STmiss)/float(M); 
        std::cout << "," << float(time_IT)/float(M) << "," << float(time_ST)/float(M) << std::endl; 
    }
    return 0;
}
