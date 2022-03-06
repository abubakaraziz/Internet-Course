/*

This handles and generates ARP packets


*/
#ifndef SR_ARP_H
#define SR_ARP_H



void arp_packet(uint8_t * packet, struct sr_instance* sr,char* interface, unsigned int);
int arp_request_send(struct sr_instance* sr,struct sr_arpreq* request ,struct sr_rt* dest); /*This function checks and generates arp request if ip packet from router if packet is for other servers*/
void send_packets_on_this_request(struct sr_arpreq* request,struct sr_instance* sr,char* interface,uint8_t *addr);
void handle_arpreq(struct sr_instance* sr,struct sr_arpreq* req,struct sr_rt* dest_rt);


#endif