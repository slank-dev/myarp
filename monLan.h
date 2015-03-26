
#include "myclass.h"
#include <pcap.h>

int pcap_init_mon(TLexOps sconfig);

char* mac_ntoa(u_char *d);

void MonitorCallback(u_char *data, const struct pcap_pkthdr *header,
										const u_char* packet);
int MonitorLan(TLexOps opt);
