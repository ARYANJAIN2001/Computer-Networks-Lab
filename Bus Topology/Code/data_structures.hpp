#include <iostream>
using namespace std;

class DataFrame
{
    public:
        int mac_addr_source;
        int mac_addr_dest;

        string message;

        int seq_no;

        DataFrame(int mac_addr_source, int mac_addr_dest, string msg, int seq_no)
        {
            this->mac_addr_dest = mac_addr_dest;
            this->mac_addr_source = mac_addr_source;
            this->message = msg;
            this->seq_no = seq_no; 
        }
};