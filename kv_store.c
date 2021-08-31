#define KBUILD_MODNAME "mf_db"
#define MAX_CMD_LENGTH 30
#define MAX_KEY_LENGTH 30
#define MAX_VAL_LENGTH 30
#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/in.h>
#include <linux/udp.h>
#include <linux/tcp.h>

BPF_PROG_ARRAY(prog_array, 10);

int tail_call(struct xdp_md *ctx) {
	void *data = (void *)(long)ctx->data;
	void *data_end = (void *)(long)ctx->data_end;

	struct ethhdr *eth = data;
	struct iphdr *ip = data + sizeof(*eth);
	struct tcphdr *tcp = (void *)ip + sizeof(*ip);

	
	char *payload_iterator = (char *) (tcp + 1);
	if((void *)payload_iterator >= data_end) {
		return XDP_PASS;
	}
	
	//Could start right away with offset = 4*tcp->doff+1 (potential verifier issues)
	unsigned int offset = 0;

	char key_string[256];
	int key_size = 0;
	
	#pragma clang loop unroll(disable)
	for(; offset < MAX_CMD_LENGTH && (void *) (payload_iterator+offset+1) <= data_end; offset++) {
		if(payload_iterator[offset] == 's' && (void *) (payload_iterator+offset+3) <= data_end && payload_iterator[offset+1] == 'e' && payload_iterator[offset+2] == 't') {
			offset += 4;
			break;
		} 
	}
	
	//Figure out if loop can be bounded to soley MAX_KEY_LENGTH
	for(; offset < (MAX_CMD_LENGTH + MAX_KEY_LENGTH) && (void *) (payload_iterator+offset+10) <= data_end; offset++) {
			key_string[key_size] = payload_iterator[offset];
			key_size++;	
	}
	
	bpf_trace_printk("%s\n", key_string);

	return XDP_PASS;
	
}

int mf_db(struct xdp_md *ctx) {
	void *data = (void *)(long)ctx->data;
	void *data_end = (void *)(long)ctx->data_end;

	struct ethhdr *eth = data;
	struct iphdr *ip = data + sizeof(*eth);
	struct tcphdr *tcp;


	if((void *)ip + 1 > data_end) {
		return XDP_PASS;
	}
	
	switch(ip->protocol) {
		case IPPROTO_TCP:
			//Check is now redudant, rather check for destination matching memcached only
			tcp = (void *)ip + sizeof(*ip);
			if((void *)tcp + 1 > data_end) {
				return XDP_PASS;
			}
			prog_array.call(ctx, 2);
			break;
		case IPPROTO_UDP:
			return XDP_PASS;
			break;
		default:
			return XDP_PASS;
	}

		
	return XDP_PASS;
}
