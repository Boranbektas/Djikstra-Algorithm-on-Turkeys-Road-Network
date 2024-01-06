#ifndef graph_H
#define graph_H

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iterator>
#include <queue>
#include <algorithm>
#include <limits>
#include <stack> 
#include <deque>


using namespace std;

struct Edge{
    string Destination;
    int cost;
};


class Graph{

private:
    unordered_map<string,vector<Edge>> AdjacencyList;
    unordered_map <string,vector<int>> distances;
    int V;
    int E;
    string LabelOfNodes[81]={ "adana", "adiyaman", "afyon", "agri", "amasya", "ankara", "antalya", "artvin", "aydin", "balikesir",
        "bilecik", "bingol", "bitlis", "bolu", "burdur", "bursa", "canakkale", "cankiri", "corum", "denizli",
        "diyarbakir", "edirne", "elazig", "erzincan", "erzurum", "eskisehir", "gaziantep", "giresun", "gumushane",
        "hakkari", "hatay", "isparta", "icel", "istanbul", "izmir", "kars", "kastamonu", "kayseri", "kirklareli",
        "kirsehir", "kocaeli", "konya", "kutahya", "malatya", "manisa", "kahramanmaras", "mardin", "mugla",
        "mus", "nevsehir", "nigde", "ordu", "rize", "sakarya", "samsun", "siirt", "sinop", "sivas",
        "tekirdag", "tokat", "trabzon", "tunceli", "sanliurfa", "usak", "van", "yozgat", "zonguldak", "aksaray",
        "bayburt", "karaman", "kirikkale", "batman", "sirnak", "bartin", "ardahan", "igdir", "yalova", "karabuk",
        "kilis", "osmaniye", "duzce"};
   
public:
    Graph(){V=0;E=0;}
    void CreateGraph(ifstream &adjacent_cities);
    void GenerateDistanceMap(ifstream &city_distances);
    void AddEdgeWeights();
    int GetV(){return V;}
    int GetE(){return E;}

    void AddVertexWithNeighbours(vector<string> token){
        vector <Edge> v;
        for(size_t i=1;i<token.size();i++){
            Edge e;
            e.Destination=token[i];
            e.cost=FindEdgeWeight(token[0],token[i]);   //finds Edge Weight from the Distance map.
            v.push_back(e);
        }
        AdjacencyList[token[0]]=v;
    }
    
    int GetCityindex(string s){
        for(size_t i=0;i<V;i++){
            if(s==LabelOfNodes[i]){
                return i;
            }
        }
        return -1;
    }

    int FindEdgeWeight(string v,string u){
        int index=GetCityindex(u);
        int Weight=distances[v][index];
        return Weight;
    }

    bool isAdjacent(string s1,string s2){
        for(size_t i=0;i<AdjacencyList[s1].size();i++){
            if(AdjacencyList[s1][i].Destination==s2){
                return true;
            }
        }
        return false;
    }
    
    void AddVertex(string s){
        vector <Edge> v;
        AdjacencyList[s]=v;
    }
    
    void RemoveVertex(string s){
        AdjacencyList.erase(s);
    }
    
    void AddEdge(string s,Edge e){
        AdjacencyList[s].push_back(e);
    }

    deque<string> Djikstra(string start ,string goal,int &Costofpath);
    vector <pair<int,string>> KclosestCities(string start,int k);

};


void Graph::CreateGraph(ifstream &adjacent_cities){

    string line;
    int NumberOfLines=80;   //There are 81 cities in turkey. /??/ //
    V=NumberOfLines;
    E=0;
    int platenumber=1;
    while(adjacent_cities){
        adjacent_cities>>line;
        vector <string> tokens; //vector to save tokens.
        stringstream ss(line);  //stringstream for tokenizing line
        string intermediate; 
        while(getline(ss,intermediate,',')){
            //transforming token to lowercaps.
            transform(intermediate.begin(),intermediate.end(),intermediate.begin(),::tolower);
            tokens.push_back(intermediate); //push tokenized strings to vector.
            }
            AddVertexWithNeighbours(tokens);
        }

}


void Graph::GenerateDistanceMap(ifstream &city_distances){  //maps out distances between Vertices
    
    if(city_distances.is_open()){
        string line;
        getline(city_distances,line);
        while(getline(city_distances,line)){
            string cityname;
            string distance;
            vector<int> tempdistances;
            stringstream ss(line);
            getline(ss,cityname,';');    //passing redundant platenumber.
            getline(ss,cityname,';');   //Extracting city name

            while(getline(ss,distance,';')){
                tempdistances.push_back(stoi(distance));
            }
            transform(cityname.begin(),cityname.end(),cityname.begin(),::tolower);
            distances.emplace(cityname,tempdistances);

        }
        city_distances.close();
        
    }
    else{
        cout<<"City Distances file cannot be opened."<<endl;
    }

}

deque<string> Graph::Djikstra(string start,string goal,int &Costofpath){
    deque<string> path;       
    Costofpath=0;
    unordered_map<string,int> dis;
    unordered_map<string, string> parent; 

    for (const auto& entry : AdjacencyList) {       //asigning infinity to all cities
        dis[entry.first]=INT_MAX;
    }
    
    dis[start]=0;
    //Asigning priority queue (can be assigned with edge struct,for simplicity of implementation left with primal types)

    priority_queue <pair<int,string>,vector<pair<int,string>>,greater<pair<int,string>>> perimeter; 

    perimeter.push({0,start});

    while(!perimeter.empty()){

        string currentcity = perimeter.top().second;        //Get the minimum distanced node away from our path
        int currentdistance = perimeter.top().first;

        if(currentcity == goal){    
            Costofpath=dis[currentcity];        
            break;
        }
        perimeter.pop();

        for(const auto &neighbour:AdjacencyList[currentcity]) {     //for each neighbour compare it with distances that already has been found.
            string nextcity = neighbour.Destination;
            int edgeweight = neighbour.cost;

            int newDistance = currentdistance + edgeweight;     //if it has a better edge to go update it, and add to the path.

            if(newDistance <dis[nextcity]){
                dis[nextcity]=newDistance;
                parent[nextcity]=currentcity;
                Costofpath=newDistance;
                perimeter.push({newDistance,nextcity});
            }
        }
    }
    
   //reconstucting the path from parent map that holds our path relationship.
    string current = goal;
    while (!current.empty()) {
        path.push_back(current);
        current = parent[current];
    }
    return path;
    
}


vector<pair<int,string>> Graph::KclosestCities(string start,int k){

    priority_queue <pair<int,string>,vector<pair<int,string>>,greater<pair<int,string>>> perimeter;
    perimeter.push({0,start});

    unordered_map<string,int> dis;

    for (const auto& entry : AdjacencyList) {       //asigning infinity to all cities
        dis[entry.first]=INT_MAX;
    }

    dis[start]=0;

    //Djikstras algorithm for all the map.

    while(!perimeter.empty()){

        string Currentcity=perimeter.top().second;
        int currentdistance = perimeter.top().first;

        perimeter.pop();

        for(const auto &neighbour:AdjacencyList[Currentcity]){
            
            string nextcity=neighbour.Destination;
            int nextedgeweight =neighbour.cost;

            int newDistance =currentdistance + nextedgeweight;
            
            //relaxation//
            if(newDistance<dis[nextcity]){
                dis[nextcity]=newDistance;
                perimeter.push({newDistance,nextcity});
            }

        }

    }

    vector <pair<int,string>> kclosest ;
    for(const auto &entry:dis){
        pair<int,string> p={entry.second,entry.first};
        kclosest.push_back(p);
    }

    sort(kclosest.begin(),kclosest.end());

    if(k<kclosest.size()){      //Truncating the vector to size k.
        kclosest.resize(k+1);
    }

    return kclosest;



}


#endif