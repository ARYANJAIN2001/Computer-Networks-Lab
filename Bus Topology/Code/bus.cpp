#include <bits/stdc++.h>
#include "network_components.hpp"

using namespace std;

int Node :: assign_id = -1;
int Link :: assign_link_id = -1;

set<int> all_mac_addresses;
map<int,int> mac_address;

int assign_mac_address(int id)
{
    int new_addr = 0;
    while (new_addr == 0 || all_mac_addresses.count(new_addr))
    {
        new_addr = rand();
    }

    all_mac_addresses.insert(new_addr);
    mac_address[id] = new_addr;

    return new_addr;
}

void make_network(vector<Link *> &links, vector<Node *> &nodes);

int main()
{

    //  MAKING THE NETWORK  //
    vector<Node *> nodes;
    vector<Link *> links;

    make_network(links, nodes);

    cout<<"Network Ready !!"<<endl;
    nodes[2]->send_msg("Hello. How are you?", 1, mac_address[7]);

    return 0;
}

void make_network(vector<Link *> &links, vector<Node *> &nodes)
{
    for (int i = 0; i < 10; i++)
    {
        nodes.push_back(new Node(assign_mac_address(i)));
    }
    
    links.push_back(new Link(0, {nodes[0],nodes[1],nodes[2]}));
    links.push_back(new Link(1, {nodes[3],nodes[4],nodes[2]}));
    links.push_back(new Link(2, {nodes[2],nodes[4],nodes[5], nodes[6], nodes[7]}));
    links.push_back(new Link(0, {nodes[2],nodes[8],nodes[9]}));

    nodes[0]->insert_links({links[0]});
    nodes[1]->insert_links({links[0]});
    nodes[2]->insert_links({links[0], links[1], links[2], links[3]});
    nodes[3]->insert_links({links[1]});
    nodes[4]->insert_links({links[1], links[2]});
    nodes[5]->insert_links({links[2]});
    nodes[6]->insert_links({links[2]});
    nodes[7]->insert_links({links[2]});
    nodes[8]->insert_links({links[3]});
    nodes[9]->insert_links({links[3]});
}