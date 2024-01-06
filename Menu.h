#ifndef Menu_H
#define Menu_H

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iterator>
#include <queue>
#include "graph.h"
using namespace std;



class Menu{

    private:
        Graph Turkey;
        string SelectedCity;
        int K; 
    public:
        Menu(ifstream &f1,ifstream &f2){
            Turkey.GenerateDistanceMap(f2); //Generating Distances from city distances file
            Turkey.CreateGraph(f1);     //Creating graph from adjacent cities file
            K=-1;
        }
        void SetK(int k){K=k;}

        void Start(){
            short choice=-1;
            do{
                cout<<"1-Enter City Name."<<endl;
                cout<<"2-Print Selected City."<<endl;
                cout<<"3-Find Shortest Path to."<<endl;
                cout<<"4-List K closest Cities."<<endl;
                cout<<"5-Exit."<<endl;
                cout<<"Select action number you want to perform."<<endl;
                cin>>choice;

                switch (choice)
                {
                case 1:
                    EnterCity();
                    break;
                case 2:
                    PrintSelectedCity();
                    break;
                case 3:
                    FindShortestPathto();
                    break;
                case 4:
                    ListKClosest();
                    break;
                case 5:
                    cout<<"You exited the program."<<endl;
                    cout<<"BYE!"<<endl;
                    break;
            
                default:
                    cout<<"Enter a valid choice."<<endl;
                    break;
                }

            }while(choice!=5);
        }

        void EnterCity(){
            cout<<endl;
            string s;
            cout<<"Enter a city.(please write with english letters):";
            cin>>s;
            transform(s.begin(),s.end(),s.begin(),::tolower);
            cout<<endl;
            SelectedCity=s;
        }
        void PrintSelectedCity(){
            cout<<endl;
            cout<<"Selected city is: ";
            cout<<SelectedCity<<endl;
            cout<<endl;
        }
        
        void FindShortestPathto(){
            cout<<endl;
            string s;
            cout<<"Enter a destination city to find the shortest path to.(please write with english letters):";
            cin>>s;
            int Costofpath;
            transform(s.begin(),s.end(),s.begin(),::tolower);
            deque <string> path=Turkey.Djikstra(SelectedCity,s,Costofpath);
            PrintPath(path,Costofpath);
        }

        void ListKClosest(){
            int k;
            cout<<"Enter k to find out k closest cities:";
            cin>>k;
            SetK(k);
            cout<<endl;
            vector<pair<int,string>> kclosest =Turkey.KclosestCities(SelectedCity,K);
            PrintKclosest(kclosest);

        }

        void PrintKclosest(vector<pair<int,string>> v){
            cout<<endl;
            cout<<K<<" Closest cities are:";
            for(size_t i=0;i<v.size()-1;i++){
                cout<<i+1<<"."<<v[i+1].second<<"("<<v[i+1].first<<")";
            }
            cout<<endl;
        }

        void PrintPath(deque<string> q,int cost){
            cout<<endl;
            int counter=0;
            size_t size=q.size();
            cout<<"Path from "<<q.back()<<" to "<<q.front()<<" is:"<<endl;
            while(!q.empty()){
                if(counter==0){
                    cout<<q.back();
                }
                else if(counter==size){
                    cout<<q.back();
                }
                else{
                    cout<<"->"<<q.back();
                }
                q.pop_back();
                counter++;
            }
            cout<<endl;
            cout<<"Cost of the path is:"<<cost<<endl;
        }

};



#endif