void dest_host_unreach(uint8_t * packet, struct sr_instance* sr,char* interface, unsigned int len){
	sr_icmp_t3_hdr_t *icmp_hdr = (sr_icmp_hdr_t *)(packet+ sizeof(sr_ethernet_hdr_t)+sizeof(sr_ip_hdr_t));
	sr_ethernet_hdr_t *ehdr = (sr_ethernet_hdr_t *)packet;
	sr_ip_hdr_t *iphdr = (sr_ip_hdr_t *)(packet+sizeof(sr_ethernet_hdr_t));
	struct sr_if* get_interface=sr_get_interface(sr,interface);
	uint8_t targetaddr[ETHER_ADDR_LEN];

	printf("Unkownn host error");
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




void Time_Exceeded(uint8_t * packet, struct sr_instance* sr,char* interface, unsigned int len){

	sr_icmp_t3_hdr_t *icmp_hdr = (sr_icmp_hdr_t *)(packet+ sizeof(sr_ethernet_hdr_t)+sizeof(sr_ip_hdr_t));
	sr_ethernet_hdr_t *ehdr = (sr_ethernet_hdr_t *)packet;
	sr_ip_hdr_t *iphdr = (sr_ip_hdr_t *)(packet+sizeof(sr_ethernet_hdr_t));
	
	uint8_t targetaddr[ETHER_ADDR_LEN];

	struct sr_if* get_interface=sr_get_interface(sr,interface);

	validate_checksum(packet,len); 	
	
	memcpy(targetaddr,ehdr->ether_shost,ETHER_ADDR_LEN);
	memcpy(icmp_hdr->data,(uint8_t*)iphdr,ICMP_DATA_SIZE);
	memcpy(ehdr->ether_shost,get_interface->addr,ETHER_ADDR_LEN);
	memcpy(ehdr->ether_dhost,targetaddr,ETHER_ADDR_LEN);

	uint32_t ip_dst=iphdr->ip_dst;

	iphdr->ip_dst=iphdr->ip_src;

	iphdr->ip_src=ip_dst;
	iphdr->ip_ttl--;
	icmp_hdr->icmp_type=11;
	iphdr->ip_sum=0;
	icmp_hdr->icmp_sum=0;
	iphdr->ip_sum=cksum(iphdr,sizeof(sr_ip_hdr_t));
	icmp_hdr->icmp_sum=cksum(icmp_hdr,sizeof(sr_icmp_t3_hdr_t));
	validate_checksum(packet,len);
	printf("Print Packet \n");
	print_hdrs(packet,len);
	printf("value of interface is %s\n",interface);
	int success=sr_send_packet(sr,packet,len,interface);
	printf("value of succes: %d\n",success);
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
	icmp_hdr->icmp_code=1;
	iphdr->ip_sum=0;
	iphdr->ip_sum=cksum(iphdr,sizeof(sr_ip_hdr_t));
	icmp_hdr->icmp_sum=0;
	icmp_hdr->icmp_sum=cksum(icmp_hdr,sizeof(sr_icmp_t3_hdr_t));

	printf("Print Packet \n");
	print_hdrs(packet,len);
	sr_send_packet(sr,packet,len,interface);

	return;

}