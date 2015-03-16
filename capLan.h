
#include "myclass.h"


void CaptureCallback(u_char *data, const struct pcap_pkthdr *header,
										const u_char* packet);
int CaptureLan(TLexOps opt);
