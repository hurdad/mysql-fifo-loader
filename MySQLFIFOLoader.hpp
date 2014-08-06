#ifndef MYSQLFIFOLOADER_HPP_
#define MYSQLFIFOLOADER_HPP_
#include <sstream> //string stream
#include <sys/types.h> //mkfifo
#include <sys/stat.h> //mkfifo

namespace MySQLFIFOLoaderNS {

using namespace std;

struct MySQLConfig {
	string hostname;
	string username;
	string password;
	int port;
	string db;
	string charset;
};

class MySQLFIFOLoader {
public:
	MySQLFIFOLoader(MySQLConfig myconf) :
			_myconf(myconf) {

	}
	virtual ~MySQLFIFOLoader() {

		//this will cause the transaction to commit on mysql]
		if (_fh)
			fclose(_fh);

		//delete tmp  fifo file
		remove(_fifo_path);

		//close mysql client process to avoid zombies
		if (_ph)
			pclose(_ph);

	}

	//returns fifo FILE* handler
	FILE* startFIFO(string table, string delimiter, string terminator) {

		//get tmp file name
		_fifo_path = tmpnam(NULL);

		//init fifo
		if (mkfifo(_fifo_path, 0666) == -1)
			return NULL;

		//build sql
		stringstream ss_sql_load;
		ss_sql_load << "LOAD DATA LOCAL INFILE \"" << string(_fifo_path)
				<< "\"  INTO TABLE `" << _myconf.db << "`.`" << table
				<< "` CHARACTER SET " << _myconf.charset << "\n";
		ss_sql_load << " FIELDS TERMINATED BY \"" << delimiter << "\"\n";
		ss_sql_load << " LINES TERMINATED BY \"" << terminator << "\"";

		//build mysql client cmd
		stringstream ss_mysql_cmd;
		ss_mysql_cmd << "mysql -u " << _myconf.username << " -p"
				<< _myconf.password << " -h " << _myconf.hostname << " -P "
				<< _myconf.port << " -D " << _myconf.db << " -e '"
				<< ss_sql_load.str() << "'";

		//submit LOAD sql via mysql client
		_ph = popen(ss_mysql_cmd.str().c_str(), "r");
		if (_ph == NULL)
			return NULL;

		//open fifo file for writing
		_fh = fopen(_fifo_path, "wb");

		//return fifo file handler
		return _fh;

	}
private:
	char * _fifo_path;
	MySQLConfig _myconf;
	FILE *_fh;
	FILE *_ph;
};

}
#endif /* MYSQLFIFOLOADER_HPP_ */
