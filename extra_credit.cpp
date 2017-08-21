// Jin Yan Ruan: modified code from Lisa Hellerstein.
// This is a bare-bones implementation of the single-source unweighted
// shortest paths algorithm
// It does not have nice classes.

// Assumption: The vertices of the graph are labeled from 0 to n - 1,
// where n is the number of vertices.

//THIS IS FOR THE EXTRA CREDIT ASSIGNMENT: NEW FUNCTIONS INCLUDE A DISTANCE METHOD AND A TRANSFER TIME METHOD
#include <iostream>
#include <list>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <cmath>

#define pi 3.14159
#define earthRadius 6371.0

using namespace std;

const int DEFAULT_VAL =  -1;      // must be less than 0
const string default_string = "-1";

typedef unordered_map < string, list<string>> Graph;        // The graph is given in an adjacency list.
// Vertices are indexed from 0 to V-1
// The indices of the vertices adjacent to vertex i
// are in the list Graph[i].
// Graph can be directed or undirected.

struct vertexInf                    // Stores information for a vertex
{
    int dist;  // distance to vertex from the source
    string prev;  // previous node in BFS tree
    int min_transfer_time;
};


struct transferstop{
    string fromstop;
    string tostop;
    int transfertime;
};

struct stopData{
    //transferstop transferTime;
    vector<transferstop> alltransfertimes;
    double lat;
    double longitude;
};


class theMTA{
public:
    
    void add_map(unordered_map<string, list<string>>& adjList, unordered_map<string, stopData>& trainstopmap){
        //used code from previous days
        
        //insert code here
        ifstream ifs("stops.txt");
        if(!ifs){
            cout << "failed to open";
            exit(1);
        }
        
        string line;
        
        string prev = "";
        string curr;
        
        while ( getline(ifs, line) ) {
            //cout << line << endl;
            //full_line contains all words and ints in a line
            vector<string> full_line;
            //split line and save each word to word string
            string word;
            stringstream ss(line);
            string token;
            //while (ss >> word){
            while (getline(ss, token, ',')){
                //save all strings in line to array
                full_line.push_back(token);
            }
            //cout << "stop id: " << full_line[0] << endl;
            
            curr = full_line[0];
            
            list<string> lst;
            
            if(full_line[0].length() != 4 && full_line[0] != "stop_id"){
                if (!prev.empty()){
                    lst.push_back(prev);
                }
                
                adjList[prev].push_back(curr);
                prev = curr;
                adjList[curr] = lst;
                
                stopData thestop;
                thestop.lat = atof(full_line[4].c_str());
                thestop.longitude = atof(full_line[5].c_str());
                trainstopmap[curr] = thestop;
            }
        }
        
        ifstream ifs2("transfers.txt");
        if(!ifs2){
            cout << "failed";
            exit(1);
        }
        vector<string> testDup;
        string line2;
        
        string previousstopid;
        
        
        while(getline(ifs2, line2)){
            vector<string> full_line;
            string word;
            stringstream ss(line2);
            string token;
            
            
            while(getline(ss, token, ',')){
                full_line.push_back(token);
            }
            
            if(full_line[0]!="from_stop_id"){
                string fromid = full_line[0];
                string toid = full_line[1];
                
                transferstop transferpair;
                transferpair.fromstop = fromid;
                transferpair.tostop = toid;
                transferpair.transfertime = atoi(full_line[3].c_str());
                trainstopmap[fromid].alltransfertimes.push_back(transferpair);
                //trainstopmap[fromid].transferTime = transferpair;
                
                if(fromid!=toid){
                    adjList[fromid].push_back(toid);
                    //trainstopmap[fromid].transferTime = atoi(full_line[3].c_str());
                }
                
            }
            
        }
        
    }
    
    vector<string> printpath(string j,  unordered_map<string, vertexInf> & vinfo)
    {
        stack<string> t;
        vector<string> allstops;
        
        string current = j;
        while (current != default_string)
        {
            t.push(current);
            current = vinfo[current].prev;
        }
        while (!t.empty())
        {
            cout << t.top() << " ";
            allstops.push_back(t.top());
            t.pop();
        }
        return allstops;
    }
    
    
    
    // Breadth First Search
    // The unweighted shortest path algorithm on the graph g, with vertex i as the source
    // Prints the length (number of edges) of the shortest path from the source to every vertex
    // in the graph
    
    vector<string> shortestpaths( Graph & g, string s, string d)
    {
        queue<string> q;             // q is the queue of vertex numbers
        
        //    if (s < 0 || s > g.size()-1)
        //	{ cout << "Bad index" << endl;
        //		return;
        //	}
        //
        unordered_map<string, vertexInf> vertices(g.size());               // stores BFS info for the vertices
        // info for vertex j is in position j
        
        //    for (int j=0; j < vertices.size(); ++j)                 // Initialize distances and prev values
        //	{ vertices[j].dist = DEFAULT_VAL; vertices[j].prev = DEFAULT_VAL; }
        
        for(pair<string, list<string>> element: g){
            vertices[element.first];
        }
        
        for (pair<string, vertexInf> element : vertices)
        {
            vertices[element.first].dist = DEFAULT_VAL;
            vertices[element.first].prev = default_string;
            
        }
        
        
        vertices[s].dist = 0;
        
        q.push(s);
        while  (!q.empty() )
        {
            string v = q.front();
            q.pop();
            for (list<string>::iterator w = g[v].begin(); w != g[v].end(); w++)
            {
                
                if (vertices[*w].dist == DEFAULT_VAL)          // distance of *w from source not determined yet
                {
                    vertices[*w].dist = vertices[v].dist+1;
                    vertices[*w].prev = v;
                    q.push(*w);
                }
            }
        }
        
        vector<string> allstops;
        for (pair<string, vertexInf> element : vertices)
        {
            if(element.first == d){
                cout << "vertex " << element.first << endl;
                cout << "distance: " << vertices[element.first].dist << endl;
                cout << "shortest path: ";
                allstops = printpath(element.first,vertices);
                cout << endl;
                
            }
            
        }
        return allstops;
    }
    
    void print_min_transfer_time(const vector<string>& stopList, unordered_map<string, stopData> allStops){
        int mintime = 0;
        bool discovered = false;

        for(int i = 1; i < stopList.size(); ++i){
            string fromstop = stopList[i-1];
            string tostop = stopList[i];
            vector<transferstop> alltransfertimes = allStops[fromstop].alltransfertimes;
            
            for(int j = 0; j < alltransfertimes.size(); ++j){
                if(alltransfertimes[j].fromstop == fromstop && alltransfertimes[j].tostop == tostop){
                    mintime += alltransfertimes[j].transfertime;
                    discovered = true;
                }
            }
            //mintime += allStops[stopList[]].transferTime.transfertime;
        }
        if(discovered == true){
            cout << "The minimum time to go through each stop once is: " << mintime << endl;
        }
        else{
            cout << "No transfer data available" << endl;
        }
    }
    
    
    // This function converts decimal degrees to radians
    double degtorad(double deg) {
        return (deg * pi / 180);
    }
    
    //  This function converts radians to decimal degrees
    double radtodeg(double rad) {
        return (rad * 180 / pi);
    }
    
    
    double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d) {
        double lat1r, lon1r, lat2r, lon2r, u, v;
        lat1r = degtorad(lat1d);
        lon1r = degtorad(lon1d);
        lat2r = degtorad(lat2d);
        lon2r = degtorad(lon2d);
        u = sin((lat2r - lat1r)/2);
        v = sin((lon2r - lon1r)/2);
        return 2.0 * earthRadius * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
    }
    
    
    void print_distance_total(const vector<string>& stopList, unordered_map<string, stopData> allstops){
        double totaldistance = 0;
        for(int i = 1; i < stopList.size(); ++i){
            double lat1d = allstops[stopList[i-1]].lat;
            double lon1d = allstops[stopList[i-1]].longitude;
            
            double lat2d = allstops[stopList[i]].lat;
            double lon2d = allstops[stopList[i]].longitude;
            
            totaldistance += distanceEarth(lat1d, lon1d, lat2d, lon2d);
            
        }
        cout << "The total distance to traverse each stop once from start stop to end stop is: "  << totaldistance << endl;
    }
    
};


int main()
{
    theMTA mta;
    unordered_map<string, list<string>> adjList;
    unordered_map<string, stopData> stopList;
    vector<string> allstops;
    mta.add_map(adjList, stopList);
    Graph g = adjList;
    
    bool stopmenu = false;
    while(stopmenu == false){
        string input = "";
        cout << "Please enter either: 'stop' or 'path' : ";
        getline(cin, input);
        if(input == "path"){
            string fromstop = "";
            cout << "Please enter stop to start from: ";
            getline(cin, fromstop);
            
            string tostop = "";
            cout << "Please enter stop to start from: ";
            getline(cin, tostop);
            
            allstops = mta.shortestpaths(g, fromstop, tostop);
            mta.print_min_transfer_time(allstops, stopList);
            mta.print_distance_total(allstops, stopList);

        }
        else if(input =="stop"){
            stopmenu = true;
            cout << "you selected stop. terminating now. " << endl;
        }
        else{
            cout << "wrong input." << endl;
        }
    }
    
//    allstops = mta.shortestpaths(g, "127", "R16");
//    mta.print_min_transfer_time(allstops, stopList);
//    mta.print_distance_total(allstops, stopList);
    
}
