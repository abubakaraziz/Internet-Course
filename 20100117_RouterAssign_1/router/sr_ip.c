#include <stdio.h>

#include "sr_protocol.h"
#include "sr_router.h"
#include "sr_rt.h"
#include "sr_if.h"
#include "sr_icmp.h"

int validate_IP(uint8_t * buf, unsigned int length){
	int minlength = sizeof(sr_ethernet_hdr_t);
	minlength += sizeof(sr_ip_hdr_t);
    if (length < minlength) {
      fprintf(stderr, "Failed to print IP header, insufficient length\n");
      return -1;
}
	return 0; /* 0 means packet is right */

}

struct sr_if* ip_packet_router_check(struct sr_instance* sr,uint8_t * packet)
{
	sr_ip_hdr_t *iphdr = (sr_ip_hdr_t *)(packet+ sizeof(sr_ethernet_hdr_t));
	struct sr_if* if_walker = 0;
    if_walker = sr->if_list;

    while(if_walker)
    {
       if(if_walker->ip==iphdr->ip_dst)
        { return if_walker; }
        if_walker = if_walker->next;
    }

    return 0;

}



struct sr_rt* print_routing_dest_address(struct sr_instance* sr, sr_ip_hdr_t *iphdr){
	/* Function  prints routing table destination address */
	struct sr_rt* current_record_table=(struct sr_rt*) malloc(sizeof(struct sr_rt));
	current_record_table=sr->routing_table;
	if (current_record_table!=NULL){
		printf("Printing Destination IP addresses in Routing \n");
		while(current_record_table->next!=NULL)
		{	
			print_addr_ip(current_record_table->dest);
			current_record_table=current_record_table->next;
		}
		print_addr_ip(current_record_table->dest);
		current_record_table=current_record_table->next;
	}
	
	return NULL; /*NULL means ipacket nterface not found error*/

}



struct sr_rt* longest_prefix_sum(struct sr_instance* sr, sr_ip_hdr_t *iphdr){
	
	struct in_addr addr;
	addr.s_addr=iphdr->ip_dst;

	struct sr_rt* current_record_table=(struct sr_rt*) malloc(sizeof(struct sr_rt));
	current_record_table=sr->routing_table;
	/*print_routing_dest_address(sr,iphdr);*/


	if (current_record_table!=NULL){
		while(current_record_table->next!=NULL){
			if (addr.s_addr==current_record_table->dest.s_addr){
				printf("Target IP matched in Routing Table \n");
				print_addr_ip_int(ntohl(iphdr->ip_dst));

				return current_record_table;
			}
			current_record_table=current_record_table->next;
		}

		if (addr.s_addr==current_record_table->dest.s_addr){
			return current_record_table;
		}
	}
	return NULL; /*NULL means ipacket nterface not found error*/

}




int packet_to_other_dest(uint8_t * packet, struct sr_instance* sr,char* interface, unsigned int len){
	sr_ethernet_hdr_t *ehdr = (sr_ethernet_hdr_t *)packet;
	sr_ip_hdr_t *iphdr = (sr_ip_hdr_t *)(packet+sizeof(sr_ethernet_hdr_t));
	
		
	
	if (iphdr->ip_ttl<=1){
		printf("TTL ERROR \n");
		Time_Exceeded(packet,sr,interface,len);
		return ;
	}
	
	struct sr_rt* dest_rt=longest_prefix_sum(sr,iphdr); /*dest interface is the source interface f router through packet will be sent*/
		if (dest_rt==NULL){
			printf("LPM not found \n");
			dest_host_unreach(packet,sr,interface,len);
			return;
		}

	struct sr_arpentry* arp_request_field=sr_arpcache_lookup(&(sr->cache),iphdr->ip_dst); /* IF NULL then not found in ARP_Cache*/

	if (arp_request_field){
		/*printf("Mac address found in cache \n");
		
		printf("Printing dest Interface below \n");
		printf("%s\n",dest_rt->interface );*/
		struct sr_if* get_interface=sr_get_interface(sr,dest_rt->interface); /*getting the source interface mac from router to dest ip*/
		/*printf("Printing get_Interface below \n");*/
		printf("%s\n",get_interface->name);

		memcpy(ehdr->ether_shost,get_interface->addr,ETHER_ADDR_LEN); 
		printf("Print Ethernet Source Address \n");
		print_addr_eth(ehdr->ether_shost);	
		memcpy(ehdr->ether_dhost,arp_request_field->mac,ETHER_ADDR_LEN); 
		printf("Print Ethernet Dest  Address \n");
		print_addr_eth(ehdr->ether_dhost);
		printf("Printing packet that is going to be sent\n");
		/*recalculating checksum*/
		
		iphdr->ip_ttl=iphdr->ip_ttl-1;	
		iphdr->ip_sum=0;
		iphdr->ip_sum=cksum(iphdr,sizeof(sr_ip_hdr_t));
			

		print_hdrs(packet,len);
		int success=sr_send_packet(sr,packet,len,get_interface->name);
		if (success==0){
			printf("Packet was successfully transmitted \n");
		}
		printf("Printing this ARP request \n");
		unsigned char *mac = arp_request_field->mac;
        fprintf(stderr, "%.1x%.1x%.1x%.1x%.1x%.1x   %.8x   %.24s   %d\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], ntohl(arp_request_field->ip), ctime(&(arp_request_field->added)), arp_request_field->valid);

		sr_arpcache_dump(&(sr->cache));
		
		/*sr_arpreq_destroy(&(sr->cache),arp_request_field);*/
		
		return success;
	}/*If arp_request_field is found means IP is already in cache then just send the packet*/
	else{
		/*If address not found cache not found create a new ARP request and send it and add packet to the queue*/
		struct sr_arpreq* req=sr_arpcache_queuereq(&(sr->cache),iphdr->ip_dst,packet,len,interface);
		handle_arpreq(sr,req,dest_rt);		

		/*printf("creating and sending arp_request \n");*/
		/*arp_request_send(sr,req,dest_rt);*//*This function creates ARP request and sends the packet*/
		return 0;
	}

}

int validate_checksum(uint8_t * packet,unsigned int len){
	uint16_t ethtype = ethertype(packet); /*checking if packet is IP or ARP*/
	if (ethtype == ethertype_arp) { /* ARP */
		}
	else if (ethtype==ethertype_ip){
			sr_ip_hdr_t *ip_hdr = (sr_ip_hdr_t *)(packet+sizeof(sr_ethernet_hdr_t));
			uint16_t ip_sum=ip_hdr->ip_sum;
			ip_hdr->ip_sum=0;
			/*printf("Checking check sum of IP packet\n");*/
			/*printf("Value of checksum is %d\n", cksum(ip_hdr,sizeof(sr_ip_hdr_t)));*/	

			if (cksum(ip_hdr,sizeof(sr_ip_hdr_t))!=ip_sum){
				printf("CHeckSUM is wrong\n");
				return 1;
			}else{
			printf("Packet is fine \n");
			}
			ip_hdr->ip_sum=ip_sum;
			return 0;
	}
}


void IP_packet(uint8_t * packet, struct sr_instance* sr,char* interface, unsigned int len){

	if ((validate_IP(packet,len)!=0 || validate_checksum(packet,len)!=0)){ /*checking for error*/
		printf("Wrong packet \n");
		return;
	}
	struct sr_if* rt=ip_packet_router_check(sr,packet);

	if (rt){
		printf("This is router packet\n");
		printf("Printing IP packet \n");
		print_hdrs(packet,len);
		sr_ip_hdr_t *iphdr = (sr_ip_hdr_t *)(packet+sizeof(sr_ethernet_hdr_t));
		if (iphdr->ip_p==1)
		{Echo_Reply(packet,sr,interface,len);
			return;
		}else if(iphdr->ip_p==17 || iphdr->ip_p==6){
		printf("here \n");
		Port_unreach(packet,sr,interface,len);
		return;
		}else{
			return;
		}
	}else{
		printf("Receives IP packet with following details \n" );
		print_hdrs(packet,len);
		validate_checksum(packet,len);
		packet_to_other_dest(packet,sr,interface,len);
	
	}

}
