#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "events.h"
#include "interface_functions.h"

// for simulating different testcases include different testcase file headers. (testcase_1.h, testcase_2.h , etc)
// for running custom testcases, you can change the values in the testcase_1.h file
#include "testcase_3.h"

// Utility function to increment the frame number
#define increment_frame_no(k) k = (1 - k);

void receiver();
void sender();

/* 
  For simulation purpose
    turn = true  : receiver's turn
    turn = false : sender's turn
*/
bool turn;
bool DISCONNECT = false;

int main()
{
    srand(seed);
    while (DISCONNECT == false)
    {
        sender();
        usleep(800000);
        receiver();
    }
    return 0;
}

void sender()
{
    static int next_frame_to_send = 0;
    static frame s;
    static int flag = 0;

    packet buffer;
    event_type event;

    if (!flag)
    {
        from_network_layer(&buffer);
        s.packet_data = buffer;
        s.seq_no = next_frame_to_send;
        
        printf("Sender   : Info = %d    Seq No = %d\n", s.packet_data.data, s.seq_no);
        turn = true;
        to_physical_layer(&s);
        flag = true;
    }

    wait_for_event_sender(&event);

    if (!turn)
    {
        switch (event)
        {
        case frame_arrival:
            from_physical_layer(&s);
            if (s.ack_no == next_frame_to_send)
            {
                printf("Sender   : Ack No = %d   Acknowledgement received Successfully\n", s.ack_no);
                if (data > TOT_PACKETS)
                {
                    DISCONNECT = true;
                    printf("\nProgram executed successfully!!\n"); // when all cases are completed.
                    return;
                }
                from_network_layer(&buffer);
                increment_frame_no(next_frame_to_send);

                s.seq_no = next_frame_to_send;
                s.packet_data = buffer;
                printf("-------------------------------------------------------------\n");
                printf("Sender   : Info = %d    Seq No = %d\n", s.packet_data.data, s.seq_no);

                turn = true;
            }
            to_physical_layer(&s);

            break;

        case time_out:
            printf("Sender   : Resending Frame\n");

            turn = true;
            to_physical_layer(&s);

            break;

        default:
            break;
        }
    }
}

void receiver()
{
    static int frame_expected = 0;
    frame r, s;
    event_type event;

    wait_for_event_receiver(&event);
    if (turn == true)
    {
        switch (event)
        {
        case frame_arrival:
            from_physical_layer(&r);
            if (r.seq_no == frame_expected)
            {
                to_network_layer(&r.packet_data);
                increment_frame_no(frame_expected);
            }
            else
                printf("Receiver : Acknowledgement Resent\n");

            s.ack_no = 1 - frame_expected;
            turn = 0;

            to_physical_layer(&s);

            break;

        case err:
            turn = false; //  if frame was damaged during transmission, sender should send it again
            printf("Receiver : Damaged Frame Received\n");
            break;

        default:
            break;
        }
    }
}

void wait_for_event_sender(event_type *e)
{
    static int timer = 0;

    if (turn == false)
    {
        timer++;
        if (timer == TIMEOUT)
        {
            printf("Sender   : Ack not received => TIMEOUT event occurred\n");
            timer = 0;
            *e = time_out;
            return;
        }
        if (DATA.err == 0)
            *e = err;
        else
        {
            *e = frame_arrival;
            timer = 0;
        }
    }
}

void wait_for_event_receiver(event_type *e)
{
    if (turn == true)
    {
        if (DATA.err == 0)
            *e = err;
        else
            *e = frame_arrival;
    }
}
