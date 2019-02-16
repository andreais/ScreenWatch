#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

int main(int argc, char **argv)
{
	inquiry_info *ii = NULL;
	int max_rsp, num_rsp;
	int dev_id, sock, len, flags;
	char addr[19] = { 0 }; // defined size array -> max length for a unique device address. might as well be a define somewhere
	char name[248] = { 0 };

	// passing NULL as arg retrieves the first bluetooth adapter available
	// the argument actually corresponds to the wanted device address (so a char*)
	dev_id = hci_get_route(NULL);
	// opens a socket using the device identifier found above
	sock = hci_open_dev(dev_id);
	if (dev_id < 0 || sock < 0) {
		perror("opening socket");
		exit(1);
	}
	// max length (*1.25 approx) of the wait for the hci_inquiry
	len  = 8;
	// max devices that wants to be find
	max_rsp = 255;
	// see what this flag corresponds to online
	flags = IREQ_CACHE_FLUSH;
	// malloc of the inquiry_info dynamic array of max (given) size
	ii = malloc(max_rsp * sizeof(inquiry_info));
	// runs a bluetooth scan to dev_id, for (len * 1.25 (approx)) for maximum max_rsp, ???, returns info to &ii using flags flags
	// ??? seems to be the number of laps done by the function, whatever
	num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
	if(num_rsp < 0) 
		perror("hci_inquiry");
	for (int i = 0; i < num_rsp; i++) {
		// takes a 6 bytes baddr_t to make a string
		// see str2ba for the opposite function
		ba2str(&(ii+i)->bdaddr, addr);
		memset(name, 0, sizeof(name));
		// tries to read the user-friendly name (those that users wrote themselves) of the devices found
		// last arg is the timeout in milliseconds
		if (hci_read_remote_name(sock, &(ii+i)->bdaddr, sizeof(name), 
					name, 0) < 0)
			strcpy(name, "[unknown]");
		printf("%s\t%s\n", addr, name);
	}
	free(ii);
	close(sock);
	return 0;
}
