#include <iostream>
#include <inttypes.h>
#include "controller.hh"
#include "timestamp.hh"

using namespace std;

/* Default constructor */
Controller::Controller( const bool debug, long double window, long double thresh)
  : debug_( debug ),this_window_(window),ssthresh_(thresh)
{

}

/* Get current window size, in datagrams */
unsigned int Controller::window_size()
{
  /* Default: fixed window size of 100 outstanding datagrams */
  

  if ( debug_ ) {
    cerr << "At time " << timestamp_ms()
	 << " window size is " << this_window_ << endl;
  }

  return this_window_;
}

/* A datagram was sent */
void Controller::datagram_was_sent( const uint64_t sequence_number,
				    /* of the sent datagram */
				    const uint64_t send_timestamp,
                                    /* in milliseconds */
				    const bool after_timeout
				    /* datagram was sent because of a timeout */ )
{
  /* Default: take no action */

  if ( debug_ ) {
    cerr << "At time " << send_timestamp
	 << " sent datagram " << sequence_number << " (timeout = " << after_timeout << ")\n";
  }
}



void Controller::set_window_size(long double size)
{
  this_window_=size;
}

void Controller::slow_start()
{
 this_window_=this_window_+1;

}


        

/* An ack was received */



long double next_time_out=105;
long double previous_RTT=0;
long double difference=0;


void Controller::ack_received( const uint64_t sequence_number_acked,
			       /* what sequence number was acknowledged */
			       const uint64_t send_timestamp_acked,
			       /* when the acknowledged datagram was sent (sender's clock) */
			       const uint64_t recv_timestamp_acked,
			       /* when the acknowledged datagram was received (receiver's clock)*/
			       const uint64_t timestamp_ack_received )
                               /* when the ack was received (by sender) */
{
  /* Default: take no action */
  //slow start

  unsigned int current_RTT=timestamp_ack_received-send_timestamp_acked;
      cout<<"value of ssthresh_: "<<ssthresh_<<endl;  
      cout<<"RTT value is "<<current_RTT<<endl;

 
  
  long double difference=0;
  difference=current_RTT-previous_RTT;
  cout<<"Difference is "<<difference<<endl;
  previous_RTT=current_RTT;

  if (current_RTT<=timeout_ms())
  {    
    if (this_window_<=ssthresh_){
      this_window_++;
      cout<<"here"<<endl;
    }else{
      this_window_=this_window_+(2/this_window_);
    }

  }else{
    cout<<"Setting thresh value"<<endl;
    ssthresh_=this_window_/2+1 ;
    this_window_=3;
  }

  if (this_window_<1){
    this_window_=1;
  }
    
  printf("Printing sequence ACK %" PRIu64 "\n", sequence_number_acked);
   cerr << "At time " << timestamp_ms()
   << " window size is " << window_size()
   << ", received @ time " << timestamp_ack_received << " by sender's clock)"<<endl;

  if ( debug_ ) {
    cerr << "At time " << timestamp_ack_received
	 << " received ack for datagram " << sequence_number_acked
	 << " (send @ time " << send_timestamp_acked
	 << ", received @ time " << recv_timestamp_acked << " by receiver's clock)"
	 << endl;
  }
}

/* How long to wait (in milliseconds) if there are no acks
   before sending one more datagram */
unsigned int Controller::timeout_ms()
{
  return next_time_out; /* timeout of one second */
}



