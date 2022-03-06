#include <stdio.h>

#include "sr_protocol.h"
#include "sr_router.h"
#include "sr_if.h"
#include "sr_rt.h"
#include "sr_icmp.h"

	



void arp_packet(uint8_t *buf,struct sr_instance* sr,char* interface, unsigned int len){
	sr_arp_hdr_t *arp_hdr = (sr_arp_hdr_t *)(buf+sizeof(sr_ethernet_hdr_t)); /*making general packet into ARP_Packet*/
		struct sr_if* get_interface=sr_get_interface(sr,interface);		
 	printf("Print number %d \n",ntohs(arp_hdr->ar_op) );
 	/*Checking if ARP_packet was destined for the router */
	if (arp_hdr->ar_tip==get_interface->ip){
		if (ntohs(arp_hdr->ar_op)==1){
			printf("It's ARP Request and Packet was destined for router \n");
			printf("Printing ARP request \n");
			print_hdrs(buf,len);
			/*modifying packet code  for ARP header section*/
			arp_hdr->ar_op=htons(2);   /*Making ARP opcode=2 for reply*/
			uint32_t target_ip=arp_hdr->ar_sip; /*saving Ip address of client  print_addr_ip_int(ntohl(ar_sip_client)); */
			uint32_t sender_ip=get_interface->ip;  /*saving current router ip_address*/
			unsigned char MAC_address_sender[ETHER_ADDR_LEN];
			memcpy(MAC_address_sender,get_interface->addr,ETHER_ADDR_LEN); /*coping MAC address of sender (router)*/
			/*print_addr_eth(MAC_address_sender);*/
			unsigned char MAC_address_target[ETHER_ADDR_LEN];
			memcpy(MAC_address_target,arp_hdr->ar_sha,ETHER_ADDR_LEN);/*receiver++++

			 MAC address to which packet will be sent*/
			/*print_addr_eth(MAC_address_target);*/
			/*modifying arp packet*/
			arp_hdr->ar_sip=sender_ip;
			arp_hdr->ar_tip=target_ip;
			memcpy(arp_hdr->ar_sha,MAC_address_sender,ETHER_ADDR_LEN);
			memcpy(arp_hdr->ar_tha,MAC_address_target,ETHER_ADDR_LEN);

			/*modifying ether_net part packet*/
			sr_ethernet_hdr_t *ehdr = (sr_ethernet_hdr_t *)buf;
			 /*print_addr_eth(ehdr->ether_dhost);*/
			 /*print_addr_eth(ehdr->ether_shost);*/
			memcpy(ehdr->ether_dhost,MAC_address_target,ETHER_ADDR_LEN);
			memcpy(ehdr->ether_shost,MAC_address_sender,ETHER_ADDR_LEN);
			int success=sr_send_packet(sr,buf,len,interface);

			}else if(ntohs(arp_hdr->ar_op)==2){
			printf("Printing ARP REPLY \n");
			print_hdr_arp(buf+sizeof(sr_ethernet_hdr_t));
			printf("Value inserted in cache\n");
			struct sr_arpreq *request=sr_arpcache_insert(&sr->cache,arp_hdr->ar_sha,arp_hdr->ar_sip);
			if (request!=NULL){
				/* we need to send all packets to the destination related to that ARP request*/
					struct sr_packet* packets=request->packets;
					if (packets!=NULL){
						printf("sending packets all to that request \n");
						int count=1;
						print_hdrs(packets->buf,packets->len);
						while(packets->next!=NULL){
							printf("Packet number is %d\n", count);
							sr_ethernet_hdr_t *ehdr = (sr_ethernet_hdr_t *)packets->buf;
							packet_to_other_dest(packets->buf,sr,interface,packets->len);
							packets=packets->next;
							count++;
						}	printf("Packet number is %d\n", count);
							packet_to_other_dest(packets->buf,sr,interface,packets->len);

					}
				sr_arpreq_destroy(&sr->cache, request);
				
			}


		}
		/*print_hdrs(buf,len);*/
	}


	
}

void handle_arpreq(struct sr_instance* sr,struct sr_arpreq* req,struct sr_rt* dest_rt){

	time_t now=time(0);
	if (difftime(now,req->sent)>1.0){
		if (req->times_sent>=5){
			/*send icmp host unreachable error*/
			struct sr_packet *packets=req->packets;
			if (packets!=NULL){

				 while(packets->next!=NULL){
				 struct sr_rt* dest_rt=longest_prefix_sum(sr,req->ip);
				 dest_host_unreach(packets->buf,sr,dest_rt->interface,packets->len);

				 }

			}
			 sr_arpreq_destroy(&(sr->cache), req);
		}else{
		printf("creating and sending arp_request \n");
		int success=arp_request_send(sr,req,dest_rt);
		if (success==0){
			printf("ARP packet was successfully sent \n");	
		}}

		req->sent=now;
		req->times_sent++;

	}


}











int arp_request_send(struct sr_instance* sr,struct sr_arpreq* request,struct sr_rt* dest){

	struct sr_if* get_interface=sr_get_interface(sr,dest->interface); /*check if path exists between this router and next hop*/

	/*Creating a packet intially*/
	uint8_t *packet=malloc(sizeof(sr_ethernet_hdr_t)+sizeof(sr_arp_hdr_t));	

	printf("Creating Ethernet Header \n");
	/*Creating Ethernet Header first */
 	sr_ethernet_hdr_t *ehdr = (sr_ethernet_hdr_t *)malloc(sizeof(sr_ethernet_hdr_t));
 
 	memset(ehdr->ether_dhost,255,ETHER_ADDR_LEN);

 	memcpy(ehdr->ether_shost,get_interface->addr,ETHER_ADDR_LEN);
 	printf("here \n");
 	ehdr->ether_type=htons(2054);


 	printf("Creating ARP Header \n");
 	/*Creating ARP header first */
	sr_arp_hdr_t *arp_hdr = (sr_arp_hdr_t *)malloc(sizeof(sr_arp_hdr_t));

	arp_hdr->ar_hrd=htons(arp_hrd_ethernet);
	arp_hdr->ar_pro=htons(2048);  
	memset(arp_hdr->ar_tha,255,ETHER_ADDR_LEN);
	memcpy(arp_hdr->ar_sha,get_interface->addr,ETHER_ADDR_LEN);
	arp_hdr->ar_hln=ETHER_ADDR_LEN;
	arp_hdr->ar_pln=sizeof(uint32_t);
	arp_hdr->ar_op=htons(1);
 	arp_hdr->ar_sip=get_interface->ip;
 	arp_hdr->ar_tip=request->ip;


 	/*Casting Ethernet Packet into Packet unint format*/
 	memcpy(packet,ehdr,sizeof(sr_ethernet_hdr_t));
 	/*Casting ARP Packet into Packet unint format*/
 	memcpy(packet+sizeof(sr_ethernet_hdr_t),arp_hdr,sizeof(sr_arp_hdr_t));
 	/*calculating size of total packet*/
 	unsigned int sizeofpacket=(sizeof(sr_ethernet_hdr_t))+(sizeof(sr_arp_hdr_t));
 	/*Printing the packet*/
 	/*printf("Printing Packet after creating it \n");*/
 	/*print_hdrs(packet);*/
 	/*sending the packet*/

 	int success=sr_send_packet(sr,packet,sizeofpacket,get_interface->name);

 	free(arp_hdr);
 	free(ehdr);
 	free(packet);
 	return success;	 
} 
