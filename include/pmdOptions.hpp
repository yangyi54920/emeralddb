/******************************************
*
*******************************************/
#ifndef PMDOPTIONS_HPP__
#define PMDOPTIONS_HPP__

#include "core.hpp"
#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>

using namespace std;
namespace po = boost::program_options;

#define PMD_OPTION_HELP                  "help"
#define PMD_OPTION_DBPATH                "dbpath"
#define PMD_OPTION_SVCNAME               "svcname"
#define PMD_OPTION_MAXPOOL               "maxpool"
#define PMD_OPTION_LOGPATH               "logpath"
#define PMD_OPTION_CONFPATH              "confpath"//配置文件的路径
#define PMD_OPTION_HASHENABLE            "hashenable"
#define PMD_OPTOIN_ADDRS                 "addrs"



#define PMD_ADD_PARAM_OPTIONS_BEGIN( desc )\
        desc.add_options()

#define PMD_ADD_PARAM_OPTIONS_END ;

#define PMD_COMMANDS_STRING(a,b) (string(a) + string(b)).c_str()//把两个字符串加起来构成一个新的字符串

#define PMD_COMMANDS_OPTIONS \
        ( PMD_COMMANDS_STRING ( PMD_OPTION_HELP, ",h"), "help" ) \
        ( PMD_COMMANDS_STRING ( PMD_OPTION_DBPATH, ",d"), boost::program_options::value<string>(), "database file full path" ) \
        ( PMD_COMMANDS_STRING ( PMD_OPTION_SVCNAME, ",s"), boost::program_options::value<string>(), "local service name" ) \
        ( PMD_COMMANDS_STRING ( PMD_OPTION_MAXPOOL, ",m"), boost::program_options::value<unsigned int>(), "max pooled agent" ) \
        ( PMD_COMMANDS_STRING ( PMD_OPTION_HASHENABLE, ",H"), boost::program_options::value<unsigned int>(), "enable conhash" ) \
        ( PMD_COMMANDS_STRING ( PMD_OPTION_LOGPATH, ",l"), boost::program_options::value<string>(), "diagnostic log file full path" ) \
        ( PMD_COMMANDS_STRING ( PMD_OPTION_CONFPATH, ",c"), boost::program_options::value<string>(), "configuration file full path" ) \
        ( PMD_COMMANDS_STRING ( PMD_OPTOIN_ADDRS, ",a"), boost::program_options::value<std::vector<std::string> >(), "all addrs of servers when enable conhash" ) \

#define CONFFILENAME                   "edb.conf"
#define LOGFILENAME                    "diag.log"
#define DBFILENAME                     "edb.data"
#define SVCNAME                        "48127"
#define NUMPOOL 					   20

class pmdOptions
{
	public:
		pmdOptions();
		~pmdOptions();

	public:
	    int	readCmd(int argc, char **argv,
				po::options_description &desc,
				po::variables_map &vm);
		int importVM(const po::variables_map &vm, bool isDefault = true);
		int readConfigureFile(const char *path,
							  po::options_description &desc,
							  po::variables_map &vm);
		int init(int argc, char **argv);

	public:
		//inline functions
		inline char* getDBPath()
		{
			return _dbPath;
		}

		inline char* getLogPath()
		{
			return _logPath;
		}

		inline char*getConfPath()
		{
			return _confPath;
		}

		inline char* getServiceName()
		{
			return _svcName;
		}

		inline int getMaxPool()
		{
			return _maxPool;
		}

	private:
		char _dbPath[OSS_MAX_PATHSIZE + 1];
		char _logPath[OSS_MAX_PATHSIZE + 1];
		char _confPath[OSS_MAX_PATHSIZE + 1];
		char _svcName[NI_MAXSERV + 1];
		int _maxPool;
};

#endif
