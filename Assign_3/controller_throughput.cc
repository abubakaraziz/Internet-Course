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

  return (unsigned int) this_window_;
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

        

/* An ack was received */


double alpha=0.15;
double next_time_out=100;
double slot=30;
double estimate_throughput=0;
double observed_throughput=0;
double delay=50;
double successive_time_out=0;
uint64_t end_time=0;
bool first_time=true;
double packet_count=0;


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



  cout<<"timestamp_ack_received"<<timestamp_ack_received<<endl;
  cerr << "RTT time is " << (timestamp_ack_received-send_timestamp_acked)<<endl;
  if ((timestamp_ack_received-send_timestamp_acked)>timeout_ms()){
    successive_time_out++;
  }else{
    successive_time_out=0;
  }
  
  if (successive_time_out>=2){
    delay=44;
  }else{
    delay=50;
  }
  
  if (timestamp_ack_received<=end_time){
 // cout<<"endtime"<<end_time<<endl;
    packet_count=packet_count+1;
    //cout<<packet_count<<endl;
    //cout<<"here"<<endl;

  }else{

  cout<<"packet count"<<packet_count<<endl;
  observed_throughput=(packet_count/(slot/2));
  //cout<<observed_throughput<<endl;
  //cout<<"observed_throughput "<<observed_throughput<<endl;
  estimate_throughput=(estimate_throughput*(1-alpha)) +(alpha*observed_throughput);
 //cout<<estimate_throughput<<endl;
  this_window_=estimate_throughput*delay;
  
 // cout<<"timestamp_ack_received"<<timestamp_ack_received<<endl;
 // cout<<"Setting end time endtime"<<end_time<<endl;   
  end_time=slot+end_time;
  //cout<<"New endtime"<<end_time<<endl; 

  packet_count=1;
 

  }


  
  if (this_window_<1){
    this_window_=3;
  }
  cout<<"window size is "<<this_window_<<endl;


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



