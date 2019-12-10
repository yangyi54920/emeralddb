/********************************
*
*********************************/
#ifndef PMD_HPP__
#define PDM_HPP__

#include "core.hpp"

enum EDB_DB_STATUS
{
	EDB_DB_NORMAL = 0,
	EDB_DB_SHUTDOWN,
	EDB_DB_PANIC
};

#define EDB_IS_DB_NORMAL (EDB_DB_NORMAL == pmdGetKRCB()->getDBStatus())
#define EDB_IS_DB_DOWN (EDB_DB_SHUTDOWN == pmdGetKRCB()->getDBStatus() || \
						EDB_DB_PANIC == pmdGetKRCB()->getDBStatus())
#define EDB_IS_DB_UP (!EDB_IS_DB_DOWN)
#define EDB_SHUTDOWN_DB (pmdGetKRCB()->setDBStatus(EDB_DB_SHUTDOWN);)

class pmdOptions;
class EDB_KRCB
{
	private:
		//configured options
		char _dataFilePath[OSS_MAX_PATHSIZE + 1];
		char _logFilePath[OSS_MAX_PATHSIZE + 1];
		int _maxPool;
		char _svcName[NI_MAXSERV + 1];
		EDB_DB_STATUS _dbStatus;

	public:
		//constructor
		EDB_KRCB()
		{
			_dbStatus = EDB_DB_NORMAL;
			memset(_dataFilePath, 0, sizeof(_dataFilePath));
			memset(_logFilePath, 0, sizeof(_logFilePath));
			_maxPool = 0;
			memset(_svcName, 0, sizeof(_svcName));
		}

		//destructor
		~EDB_KRCB(){}

		//get database status
		inline EDB_DB_STATUS getDBStatus()
		{
			return _dbStatus;
		}

		inline const char* getDataFilePath()
		{
			return _dataFilePath;
		}

		inline const char* getLogFilePath()
		{
			return _logFilePath;
		}

		inline const char* getSvcName()
		{
			return _svcName;
		}

		inline int getMaxPool()
		{
			return _maxPool;
		}

		//setup database status
		inline void setDBStatus(EDB_DB_STATUS status)
		{
			_dbStatus = status;
		}

		//set data file path
		void setDataFilePath(const char *pPath)
		{
			strncpy(_dataFilePath, pPath, sizeof(_dataFilePath));
		}

		void setLogFilePath(const char *pPath)
		{
			strncpy(_logFilePath, pPath, sizeof(_logFilePath));
		}

		void setSvcName(const char *pName)
		{
			strncpy(_svcName, pName, sizeof(_svcName));
		}

		void set MaxPool(int maxPool)
		{
			_maxPool = maxPool;
		}

		//setup from pmdOptions
		int init(pmdOptions *options);
};

extern EDB_KRCB pmd_krcb;

inline EDB_KRCB* pmdGetKRCB()
{
	return &pmd_krcb;
}

#endif