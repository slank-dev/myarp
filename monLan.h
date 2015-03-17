
#include "myclass.h"
#include <pcap.h>



void MonitorCallback(u_char *data, const struct pcap_pkthdr *header,
										const u_char* packet);
int MonitorLan(TLexOps opt);
