#include <bits/stdc++.h>
#include "network_components.hpp"

using namespace std;

int Node ::assign_id = -1;
int Link ::assign_link_id = -1;

set<int> all_mac_addresses;
map<int, int> mac_address;

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

void make_network_1(vector<Link *> &links, vector<Node *> &nodes);
void make_network_2(vector<Link *> &links, vector<Node *> &nodes);
int main()
{

    //  MAKING THE NETWORK  //
    vector<Node *> nodes;
    vector<Link *> links;

    make_network_1(links, nodes);
    
    //  Unicast message sent by node 2 to node 5
    nodes[2]->send_msg("Message from node 2 to 5", 1, mac_address[5]);
    cout<<"............................................................................................................"<<endl;
    usleep(2000000);
    //  Broadcast message sent from node 6. It can't reach every other node in the network as all links are not bi-directional.
    nodes[6]->send_msg("This is a broadcast message from node 6.", 1, 0);
    cout<<"............................................................................................................"<<endl;
    usleep(2000000);


    //  if link 2 has failed , we simulate sending of a broadcast message from node 2 after link 2 has failed 
    links[2]->enable_failure(); 
        nodes[2]->send_msg("This is a broadcast message sent after link 2 has failed.", 1 , 0);
        cout<<"............................................................................................................"<<endl;
        usleep(2000000);


    links[2]->disable_failure(); // restoring the link after simulating failure

    //  if node 5 has failed 
    nodes[5]->enable_failure();
        
        // we simulate sending a message from node 2 to node 5 first
        nodes[2]->send_msg("Message from node 2 to node 5 after node failure.", 1 , mac_address[5]);
        cout<<"............................................................................................................"<<endl;
        usleep(2000000);

        // we simulate sending a broadcast message from node 2 then (to show it only affects node 5)
        nodes[2]->send_msg("Message from node 2 to node 5 after node failure.", 1 , 0);
        cout<<"............................................................................................................"<<endl;
        usleep(2000000);

    nodes[5]->disable_failure();

    cout<<"............................................................................................................"<<endl;
    
    make_network_2(links,nodes);
    cout<<"New network ready for simulations."<<endl;

    cout<<"............................................................................................................"<<endl;

    //  sending a broadcast message from node 7 to all 
    nodes[7]->send_msg("Broadcast message on network 2.", 1, 0);
    cout<<"............................................................................................................"<<endl;
    usleep(5000000);
   
    //  
        links[0]->enable_failure();
        links[2]->enable_failure();

            //  sending a boadcast message from node 7 to all after failure of links 0 and 2.
            nodes[7]->send_msg("Broadcast message on network 2.", 1, 0);
            cout<<"............................................................................................................"<<endl;
            usleep(2000000);

        links[0]->disable_failure();
        links[2]->disable_failure();
    // 
    



    return 0;
}

void make_network_1(vector<Link *> &links, vector<Node *> &nodes)
{
    nodes.clear();
    links.clear();
    mac_address.clear();

    for (int i = 0; i < 10; i++)
    {
        nodes.push_back(new Node(assign_mac_address(i)));
    }

    links.push_back(new Link(0, {nodes[0], nodes[1], nodes[2]}));
    links.push_back(new Link(1, {nodes[3], nodes[4], nodes[2]}));
    links.push_back(new Link(2, {nodes[2], nodes[4], nodes[5], nodes[6], nodes[7]}));
    links.push_back(new Link(0, {nodes[2], nodes[8], nodes[9]}));

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

void make_network_2(vector<Link *> &links, vector<Node *> &nodes)
{
    nodes.clear();
    links.clear();
    mac_address.clear();
    for (int i = 0; i < 8; i++)
    {
        nodes.push_back(new Node(assign_mac_address(i)));
    }

    links.push_back(new Link(2, {nodes[7], nodes[0], nodes[1], nodes[2], nodes[5]}));
    links.push_back(new Link(0, {nodes[3], nodes[1], nodes[2], nodes[4], nodes[7], nodes[5], nodes[6]}));
    links.push_back(new Link(1, {nodes[3], nodes[2], nodes[4], nodes[6], nodes[7]}));

    nodes[0]->insert_links({links[0]});
    nodes[1]->insert_links({links[0], links[1]});
    nodes[2]->insert_links({links[0], links[1], links[2]});
    nodes[3]->insert_links({links[1], links[2]});
    nodes[4]->insert_links({links[1], links[2]});
    nodes[5]->insert_links({links[0], links[1]});
    nodes[6]->insert_links({links[1], links[2]});
    nodes[7]->insert_links({links[0], links[1], links[2]});
    
}