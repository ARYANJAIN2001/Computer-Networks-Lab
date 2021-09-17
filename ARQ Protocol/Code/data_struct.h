typedef struct
{
    int data;
} packet;

typedef struct
{
    int kind;
    int seq_no;
    int ack_no;
    packet packet_data;
    int err;
} frame;
