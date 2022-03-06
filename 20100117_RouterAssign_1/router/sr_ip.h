/*

This handles and generates IP packets

*/

#ifndef SR_IP_H
#define SR_IP_H


void IP_packet(uint8_t * packet, struct sr_instance* sr,char* interface, unsigned int len);
int ip_packet_router_check(struct sr_instance* sr,uint8_t * packet);
int packet_to_other_dest(uint8_t * packet, struct sr_instance* sr,char* interface, unsigned int len);
int validate_IP(uint8_t * buf, unsigned int length);
struct sr_rt* longest_prefix_sum(struct sr_instance* sr, sr_ip_hdr_t *iphdr);

#endif


