#include <stdio.h>

#include "sr_protocol.h"
#include "sr_router.h"
#include "sr_rt.h"
#include "sr_if.h"


	
void Echo_Reply(uint8_t * packet, struct sr_instance* sr,char* interface, unsigned int len)
{
	sr_icmp_hdr_t *icmp_hdr = (sr_icmp_hdr_t *)(packet+ sizeof(sr_ethernet_hdr_t)+sizeof(sr_ip_hdr_t));
	sr_ethernet_hdr_t *ehdr = (sr_ethernet_hdr_t *)packet;
	sr_ip_hdr_t *iphdr = (sr_ip_hdr_t *)(packet+sizeof(sr_ethernet_hdr_t));
	struct sr_if* get_interface=sr_get_interface(sr,interface);
	uint8_t targetaddr[ETHER_ADDR_LEN];
	memcpy(targetaddr,ehdr->ether_shost,ETHER_ADDR_LEN);
	memcpy(ehdr->ether_shost,get_interface->addr,ETHER_ADDR_LEN);
	memcpy(ehdr->ether_dhost,targetaddr,ETHER_ADDR_LEN);
	


	iphdr->ip_dst=iphdr->ip_src;
	iphdr->ip_src=get_interface->ip;
 	
	icmp_hdr->icmp_type=0;
	iphdr->ip_sum=0;
	iphdr->ip_sum=cksum(iphdr,sizeof(sr_ip_hdr_t));
	icmp_hdr->icmp_sum=0;
	icmp_hdr->icmp_sum=cksum(icmp_hdr,sizeof(sr_icmp_hdr_t));

	printf("Print Packet \n");
	print_hdrs(packet,len);
	sr_send_packet(sr,packet,len,interface);
	return;
}

void Time_Exceeded(uint8_t * packet, struct sr_instance* sr,char* interface, unsigned int len){

	uint8_t* buff=malloc(sizeof(sr_ethernet_hdr_t)+sizeof(sr_ip_hdr_t)+sizeof(sr_icmp_t3_hdr_t));
	sr_icmp_t3_hdr_t *icmp_hdr_new = (sr_icmp_hdr_t *)(buff+ sizeof(sr_ethernet_hdr_t)+sizeof(sr_ip_hdr_t));
	sr_ethernet_hdr_t *ehdr_new = (sr_ethernet_hdr_t *)buff;
	sr_ip_hdr_t *iphdr_new = (sr_ip_hdr_t *)(buff+sizeof(sr_ethernet_hdr_t));

	
	sr_ethernet_hdr_t *ehdr_old = (sr_ethernet_hdr_t *)packet;
	sr_ip_hdr_t *iphdr_old = (sr_ip_hdr_t *)(packet+sizeof(sr_ethernet_hdr_t));
	struct sr_if* get_interface=sr_get_interface(sr,interface);
	uint8_t targetaddr[ETHER_ADDR_LEN];

	printf("Unkownn host error");
	memcpy(targetaddr,ehdr_old->ether_shost,ETHER_ADDR_LEN);


	/*newether_net header field update*/

	memcpy(ehdr_new->ether_shost,get_interface->addr,ETHER_ADDR_LEN);
	memcpy(ehdr_new->ether_dhost,targetaddr,ETHER_ADDR_LEN);
	ehdr_new->ether_type=ehdr_old->ether_type;

	/* ip header details*/
	iphdr_new->ip_dst=iphdr_old->ip_src;
	iphdr_new->ip_src=get_interface->ip;
 	iphdr_new->ip_v=4;
 	iphdr_new->ip_hl=5;
 	iphdr_new->ip_tos=0;

 	iphdr_new->ip_tos=iphdr_old->ip_tos;
 	iphdr_new->ip_len=htons(sizeof(sr_ip_hdr_t)+sizeof(sr_icmp_t3_hdr_t));
 	iphdr_new->ip_id=iphdr_old->ip_id;
 	iphdr_new->ip_off=iphdr_old->ip_off;
 	iphdr_new->ip_ttl=iphdr_old->ip_ttl;
 	iphdr_new->ip_p=1;

 	/*decreasing ttl*/
 	iphdr_new->ip_ttl--;
 	/* icmp header*/
	icmp_hdr_new->icmp_type=11;
	icmp_hdr_new->icmp_code=0;
	icmp_hdr_new->unused=0;
	memcpy(icmp_hdr_new->data,(uint8_t*)iphdr_old,ICMP_DATA_SIZE);
	/*check sum update*/
	iphdr_new->ip_sum=0;
	iphdr_new->ip_sum=cksum(iphdr_new,sizeof(sr_ip_hdr_t));
	icmp_hdr_new->icmp_sum=0;
	icmp_hdr_new->icmp_sum=cksum(icmp_hdr_new,sizeof(sr_icmp_t3_hdr_t));
	
	
	printf("Print Packet \n");
	print_hdrs(buff,len);
	sr_send_packet(sr,buff,len,interface);

	return;

	
}


void dest_host_unreach(uint8_t * packet, struct sr_instance* sr,char* interface, unsigned int len){
	uint8_t* buff=malloc(sizeof(sr_ethernet_hdr_t)+sizeof(sr_ip_hdr_t)+sizeof(sr_icmp_t3_hdr_t));
	sr_icmp_t3_hdr_t *icmp_hdr_new = (sr_icmp_hdr_t *)(buff+ sizeof(sr_ethernet_hdr_t)+sizeof(sr_ip_hdr_t));
	sr_ethernet_hdr_t *ehdr_new = (sr_ethernet_hdr_t *)buff;
	sr_ip_hdr_t *iphdr_new = (sr_ip_hdr_t *)(buff+sizeof(sr_ethernet_hdr_t));

	
	sr_ethernet_hdr_t *ehdr_old = (sr_ethernet_hdr_t *)packet;
	sr_ip_hdr_t *iphdr_old = (sr_ip_hdr_t *)(packet+sizeof(sr_ethernet_hdr_t));
	struct sr_if* get_interface=sr_get_interface(sr,interface);
	uint8_t targetaddr[ETHER_ADDR_LEN];

	printf("desthostnnreachable");
	memcpy(targetaddr,ehdr_old->ether_shost,ETHER_ADDR_LEN);


	/*newether_net header field update*/

	memcpy(ehdr_new->ether_shost,get_interface->addr,ETHER_ADDR_LEN);
	memcpy(ehdr_new->ether_dhost,targetaddr,ETHER_ADDR_LEN);
	ehdr_new->ether_type=ehdr_old->ether_type;

	/* ip header details*/
	iphdr_new->ip_dst=iphdr_old->ip_src;
	iphdr_new->ip_src=get_interface->ip;
 	iphdr_new->ip_v=4;
 	iphdr_new->ip_hl=5;
 	iphdr_new->ip_tos=0;

 	iphdr_new->ip_tos=iphdr_old->ip_tos;
 	iphdr_new->ip_len=htons(sizeof(sr_ip_hdr_t)+sizeof(sr_icmp_t3_hdr_t));
 	iphdr_new->ip_id=iphdr_old->ip_id;
 	iphdr_new->ip_off=iphdr_old->ip_off;
 	iphdr_new->ip_ttl=iphdr_old->ip_ttl;
 	iphdr_new->ip_p=1;

 	/* icmp header*/
	icmp_hdr_new->icmp_type=3;
	icmp_hdr_new->icmp_code=1;
	icmp_hdr_new->unused=0;
	memcpy(icmp_hdr_new->data,(uint8_t*)iphdr_old,ICMP_DATA_SIZE);
	/*check sum update*/
	iphdr_new->ip_sum=0;
	iphdr_new->ip_sum=cksum(iphdr_new,sizeof(sr_ip_hdr_t));
	icmp_hdr_new->icmp_sum=0;
	icmp_hdr_new->icmp_sum=cksum(icmp_hdr_new,sizeof(sr_icmp_t3_hdr_t));
	/*decreasing ttl*/
	
	printf("Print Packet \n");
	print_hdrs(buff,len);
	sr_send_packet(sr,buff,len,interface);

	return;


}



void Dest_net_unreach(uint8_t * packet, struct sr_instance* sr,char* interface, unsigned int len){
	
	printf("Inside dest net unreach \n");
	sr_icmp_t3_hdr_t *icmp_hdr = (sr_icmp_hdr_t *)(packet+ sizeof(sr_ethernet_hdr_t)+sizeof(sr_ip_hdr_t));
	sr_ethernet_hdr_t *ehdr = (sr_ethernet_hdr_t *)packet;
	sr_ip_hdr_t *iphdr = (sr_ip_hdr_t *)(packet+sizeof(sr_ethernet_hdr_t));
	struct sr_if* get_interface=sr_get_interface(sr,interface);
	uint8_t targetaddr[ETHER_ADDR_LEN];

	memcpy(targetaddr,ehdr->ether_shost,ETHER_ADDR_LEN);
	memcpy(icmp_hdr->data,(uint8_t*)iphdr,ICMP_DATA_SIZE);
	memcpy(ehdr->ether_shost,get_interface->addr,ETHER_ADDR_LEN);
	memcpy(ehdr->ether_dhost,targetaddr,ETHER_ADDR_LEN);

	iphdr->ip_dst=iphdr->ip_src;
	iphdr->ip_src=get_interface->ip;
 	
	icmp_hdr->icmp_type=3;
	icmp_hdr->icmp_code=0;
	iphdr->ip_sum=0;
	iphdr->ip_sum=cksum(iphdr,sizeof(sr_ip_hdr_t));
	icmp_hdr->icmp_sum=0;
	icmp_hdr->icmp_sum=cksum(icmp_hdr,sizeof(sr_icmp_t3_hdr_t));

	printf("Print Packet \n");
	print_hdrs(packet,len);
	sr_send_packet(sr,packet,len,interface);

	return;

}



void Port_unreach(uint8_t * packet, struct sr_instance* sr,char* interface, unsigned int len){
	uint8_t* buff=malloc(sizeof(sr_ethernet_hdr_t)+sizeof(sr_ip_hdr_t)+sizeof(sr_icmp_t3_hdr_t));
	sr_icmp_t3_hdr_t *icmp_hdr_new = (sr_icmp_hdr_t *)(buff+ sizeof(sr_ethernet_hdr_t)+sizeof(sr_ip_hdr_t));
	sr_ethernet_hdr_t *ehdr_new = (sr_ethernet_hdr_t *)buff;
	sr_ip_hdr_t *iphdr_new = (sr_ip_hdr_t *)(buff+sizeof(sr_ethernet_hdr_t));

	
	sr_ethernet_hdr_t *ehdr_old = (sr_ethernet_hdr_t *)packet;
	sr_ip_hdr_t *iphdr_old = (sr_ip_hdr_t *)(packet+sizeof(sr_ethernet_hdr_t));
	struct sr_if* get_interface=sr_get_interface(sr,interface);
	uint8_t targetaddr[ETHER_ADDR_LEN];

	printf("Unkownn host error");
	memcpy(targetaddr,ehdr_old->ether_shost,ETHER_ADDR_LEN);


	/*newether_net header field update*/

	memcpy(ehdr_new->ether_shost,get_interface->addr,ETHER_ADDR_LEN);
	memcpy(ehdr_new->ether_dhost,targetaddr,ETHER_ADDR_LEN);
	ehdr_new->ether_type=ehdr_old->ether_type;

	/* ip header details*/
	iphdr_new->ip_dst=iphdr_old->ip_src;
	iphdr_new->ip_src=get_interface->ip;
 	iphdr_new->ip_v=4;
 	iphdr_new->ip_hl=5;
 	iphdr_new->ip_tos=0;

 	iphdr_new->ip_tos=iphdr_old->ip_tos;
 	iphdr_new->ip_len=htons(sizeof(sr_ip_hdr_t)+sizeof(sr_icmp_t3_hdr_t));
 	iphdr_new->ip_id=iphdr_old->ip_id;
 	iphdr_new->ip_off=iphdr_old->ip_off;
 	iphdr_new->ip_ttl=iphdr_old->ip_ttl;
 	iphdr_new->ip_p=1;

 	/* icmp header*/
	icmp_hdr_new->icmp_type=3;
	icmp_hdr_new->icmp_code=3;
	icmp_hdr_new->unused=0;
	memcpy(icmp_hdr_new->data,(uint8_t*)iphdr_old,ICMP_DATA_SIZE);
	/*check sum update*/
	iphdr_new->ip_sum=0;
	iphdr_new->ip_sum=cksum(iphdr_new,sizeof(sr_ip_hdr_t));
	icmp_hdr_new->icmp_sum=0;
	icmp_hdr_new->icmp_sum=cksum(icmp_hdr_new,sizeof(sr_icmp_t3_hdr_t));
	/*decreasing ttl*/
	
	printf("Print Packet \n");
	print_hdrs(buff,len);
	sr_send_packet(sr,buff,len,interface);

	return;


}