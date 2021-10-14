#include <bits/stdc++.h>

#include <thread>
#include <unistd.h>

#include "data_structures.hpp"

using namespace std;

class Link;

class Node{
    static int assign_id; 
    
    public:
        int mac_addr;
        int device_id;
        vector<Link *> links;

        Node(int mac_address){
            mac_addr = mac_address;
            device_id = ++assign_id;
        }
        
        void insert_links(vector< Link*> links);
        void send_msg(string msg, int seq_no, int mac_addr_dest);
        void receive_frame(DataFrame data);
};

class Link
{
    static int assign_link_id;

    private:
        vector<Node *> nodes;
        int direction_of_transfer;
        int link_id;

    public:
        Link(int direction, vector< Node *> nodes){
            direction_of_transfer = direction;
            for(int i = 0 ; i < nodes.size() ; ++i){
                insert_node(nodes[i]);
            }

            link_id = ++assign_link_id;
        }

        void insert_node(Node* n)
        {
            nodes.push_back(n);
        }


        void send_frame_in_link(DataFrame data)
        {

            int node_id;
            for(int i = 0 ; i < nodes.size() ; ++i)
            {
                if(data.mac_addr_source == nodes[i]->mac_addr){
                    node_id = i;
                    break;
                }
            }

            // cout<<"Sending in Link : "<<link_id<<endl;

            switch (direction_of_transfer)
            {

                case 0: // bidirectional transfer can happen
                {
                    vector<thread *> transfer_thread;

                    transfer_thread.push_back(new thread(&Link::send_to_left, this, data, node_id));
                    transfer_thread.push_back(new thread(&Link::send_to_right, this, data, node_id));

                    for (int i = 0; i < transfer_thread.size(); ++i)
                        transfer_thread[i]->join();

                    break;
                }

                case 1: // tranfer can happen to left only
                {
                    send_to_left(data, node_id);
                    break;
                }
                case 2: // transfer can happen to right only
                {
                    send_to_right(data, node_id);
                    break;
                }
                default:
                    break;
                }
        }

        void send_to_left(DataFrame data, int node_id)
        {
            for (int i = node_id - 1; i >= 0; --i)
            {
                cout<<"Link from which message was received: "<<link_id<<endl;
                nodes[i]->receive_frame(data);
                usleep(500000);
            }
        }

        void send_to_right(DataFrame data, int node_id)
        {
            for (int i = node_id + 1; i < nodes.size(); ++i)
            {
                cout<<"Link from which message was received: "<<link_id<<endl;
                nodes[i]->receive_frame(data);
                usleep(500000);
            }
        }
};

void Node :: send_msg(string msg, int seq_no, int mac_addr_dest)
{
    cout<< "Sending message now."<<endl;

    DataFrame *data = new DataFrame(mac_addr, mac_addr_dest, msg, seq_no);
    vector<thread *> send_to_links;

    for (int i = 0; i < links.size(); i++)
    {
        send_to_links.push_back(new thread(&Link::send_frame_in_link, links[i], *data));
        usleep(50);
    }

    for (int i = 0; i < links.size(); i++)
    {
        send_to_links[i]->join();
    }
}

void Node :: receive_frame(DataFrame data)
{
    cout << device_id << " : Data Frame with seq_no: " << data.seq_no << " Received." << endl;

    if (data.mac_addr_dest == mac_addr || data.mac_addr_dest == 0 )
    {
        cout << device_id << " : Data Frame with seq_no: " << data.seq_no << " Accepted. Message Received : "<< data.message << endl;
    }
    else
    {
        cout << device_id << " : Data Frame with seq_no: " << data.seq_no << " Rejected." << endl;
    }
}

void Node :: insert_links(vector<Link*> network_link)
{
    for(int i = 0 ; i < network_link.size() ; ++i){
        links.push_back(network_link[i]);
    }
}


