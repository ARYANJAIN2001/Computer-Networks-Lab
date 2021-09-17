typedef enum
{
    frame_arrival,
    err,time_out,
    no_event
} event_type;


void wait_for_event_sender(event_type *);
void wait_for_event_receiver(event_type *);