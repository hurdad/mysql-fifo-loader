#ifndef MYSQLFIFOLOADER_HPP_
#define MYSQLFIFOLOADER_HPP_

namespace MySQLFIFOLoaderNS {

using namespace std;

struct MySQLConfig {
	string hostname;
	string username;
	string password;
	int port;
	string db;
};

class MySQLFIFOLoader {
public:
	MySQLFIFOLoader(MySQLConfig myconf) :
			_myconf(myconf) {

	}
	virtual ~MySQLFIFOLoader() {
		fclose(_fh);
		pclose(_ph);
	}

	//returns
	FILE* startFIFO(string table, string delimiter, string terminator) {

		//get tmp file name
		char *fifo_path = tmpnam(NULL);

		//init fifo
		if (!mkfifo(fifo_path, 0444))
			return NULL;

		//build sql
		string load;
		load = "LOAD DATA LOCAL INFILE \"" + string(fifo_path)
				+ "\"  INTO TABLE `" + _myconf.db + "`.`" + table
				+ "` CHARACTER SET " + _charset + " ";

		//    mysql_cmd = "mysql -u{$shard['user']} -h{$shard['host']} -P{$shard['port']} {$shard['db']}";
		//    if (!empty($shard['password']) && trim($shard['password'])) {
		//      	mysql_cmd += " -p{$shard['password']}";
		// }
		//  mysql_cmd += " -e '{$load}'";

		string mysql_cmd = "mysql ";

		//submit LOAD sql via mysql client
		if ((_ph = popen(mysql_cmd.c_str(), "r")) == NULL)
			return NULL;

		//open fifo file for writing
		_fh = fopen(fifo_path, "wb");

		//return fifo file handler
		return _fh;

	}
private:
	string _charset;
	MySQLConfig _myconf;
	FILE *_fh;
	FILE *_ph;
};

}
#endif /* MYSQLFIFOLOADER_HPP_ */
