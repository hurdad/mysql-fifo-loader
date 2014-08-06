//#include <iostream>
#include <stdio.h>
//#include <sys/types.h>
//#include <sys/stat.h>

#include "MySQLFIFOLoader.hpp"
using namespace std;
using namespace MySQLFIFOLoaderNS;

int main() {

	// MySQL config
	MySQLConfig myconf;
	myconf.hostname = "localhost";
	myconf.port = 3306;
	myconf.username = "root";
	myconf.password = "dev";
	myconf.db = "fifo";
	myconf.charset = "latin1";

	//init FIFO Loader
	MySQLFIFOLoader loader(myconf);

	//start FIFO - returns fifo FILE* handler
	FILE *fifo = loader.startFIFO("test", ",", "\\n");

	//write csv data to fifo
	if (fifo != NULL) {
		fputs("1,2\n", fifo);
		fputs("3,4\n", fifo);
		fputs("5,6\n", fifo);
		fputs("7,8\n", fifo);
	}

}
