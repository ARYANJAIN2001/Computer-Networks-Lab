#include <bits/stdc++.h>
#include <thread>
#include "data_structures.hpp"

using namespace std;

class link;

class node{
    static int assign_id; 
    
    public:
        int mac_addr;
        int device_id;
        vector<link *> links;

        node(int mac_address){
            mac_addr = mac_address;
            device_id = ++assign_id;
        }
         
        void send_msg_to_all_links(string msg, int seq_no, int mac_addr_dest);
        void receive_frame(DataFrame data);
};

class link
{
    private:
        vector<node *> nodes;
        int direction_of_transfer;

    public:
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

            switch (direction_of_transfer)
            {
                case 0: // bidirectional transfer can happen
                {
                    vector<thread *> transfer_thread;

                transfer_thread.push_back(new thread(&link::send_to_left, this, data, node_id));
                transfer_thread.push_back(new thread(&link::send_to_right, this, data, node_id));

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
                nodes[i]->receive_frame(data);
            }
        }

        void send_to_right(DataFrame data, int node_id)
        {
            for (int i = node_id + 1; i < nodes.size(); ++i)
            {
                nodes[i]->receive_frame(data);
            }
        }
};

void node :: send_msg_to_all_links(string msg, int seq_no, int mac_addr_dest)
{
    DataFrame *data = new DataFrame(mac_addr, mac_addr_dest, msg, seq_no);
    vector<thread *> send_to_links;
    
    for (int i = 0; i < links.size(); i++)
    {
        send_to_links.push_back(new thread(&link::send_frame_in_link, links[i], *data));
    }

    for (int i = 0; i < links.size(); i++)
    {
        send_to_links[i]->join();
    }
}

void node :: receive_frame(DataFrame data)
{
    cout << device_id << " : Data Frame " << data.seq_no << "Received." << endl;

    if (data.mac_addr_dest == mac_addr || data.mac_addr_dest == 0 )
    {
        cout << device_id << " : Data Frame " << data.seq_no << "Accepted." << endl;
    }
    else
    {
        cout << device_id << " : Data Frame " << data.seq_no << "Rejected." << endl;
    }
}



