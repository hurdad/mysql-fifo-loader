
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "MySQLFIFOLoader.hpp"
using namespace std;
using namespace MySQLFIFOLoaderNS;

int main() {

	// MySQL config
	MySQLConfig myconf;
	myconf.hostname = "localhost";
	myconf.port = 3306;
	myconf.username = "user";
	myconf.password = "pass";
	myconf.db = "db";

	MySQLFIFOLoader loader(myconf);

	FILE *fifo = loader.startFIFO("mytable", ",", "\\n");

	//write data to fifo
	if (fifo != NULL) {
		fputs("val1,val2,val3", fifo);
	}
}
