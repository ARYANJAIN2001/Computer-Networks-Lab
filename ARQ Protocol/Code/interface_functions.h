#include <stdio.h>
#include <stdlib.h>

#include "data_struct.h"


frame DATA;
int data = 1;

void from_network_layer(packet *buffer)
{
   (*buffer).data = data;
   data++;
}


void to_physical_layer(frame *s)
{
   s->err = rand() % 4; // 0 is for error, otherwise no error occurred in transmission
   DATA = *s;
}

void to_network_layer(packet *buffer)
{
   printf("Receiver : Packet %d received , Ack Sent\n", (*buffer).data);
}

void from_physical_layer(frame *buffer)
{
   *buffer = DATA;
}