#include <bits/stdc++.h>
#include "network_components.hpp"

using namespace std;

int node :: assign_id = 0; 

int main(){

    node* x = new node(123); 
    node* y = new node(12676);

    cout<<"x: "<<x->device_id<<" , y : "<<y->device_id<<endl;

    return 0;
}