/*

This handles and generates ICMP packets



*/


#ifndef SR_ICMP_H
#define SR_ICMP_H

void Echo_Reply(uint8_t * packet, struct sr_instance* sr,char* interface, unsigned int len);
void Dest_net_unreach(uint8_t * packet, struct sr_instance* sr,char* interface, unsigned int len);
void Time_Exceeded(uint8_t * packet, struct sr_instance* sr,char* interface, unsigned int len);
void dest_host_unreach(uint8_t * packet, struct sr_instance* sr,char* interface, unsigned int len);
void Port_unreach(uint8_t * packet, struct sr_instance* sr,char* interface, unsigned int len);




#endif