#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iterator>
#include <queue>
#include "graph.h"
#include "Menu.h"

using namespace std;



int main(void){
    ifstream adjacent_cities_file  ("adjacent_cities.txt");
    ifstream city_distances_file  ("city_distances.txt");

    Menu m(adjacent_cities_file,city_distances_file);

    m.Start();
    
    cout<<endl;
    
    return 0;
}
