#pragma once
#include <iostream>
#include <list>
#include "sqlite3.h"
#include <string>
#include"userData.h"
class SQL {
private:
	
	sqlite3* db;
	char name;
	

public:

	SQL() : db(nullptr) {

	}

	void Open(const char* dir);
	void createTable();
	int createIndex();
	void insertData(UserDataList& dataList);
	int bindName(UserDataList& dataList, std::string name);
	int selectData(UserDataList& dataList);
	~SQL()
	{
		if (db != nullptr)
			sqlite3_close(db);
	}
};