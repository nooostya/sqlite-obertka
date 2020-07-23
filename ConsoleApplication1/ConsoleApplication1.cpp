
#include <iostream>
#include "sqlite3.h"
#include <stdio.h>
#include <string>
#include <conio.h>
#include <iterator>
#include <list>
#include <fstream>
#include <exception>
#include "SQLException.h"
#include "SQL.h"
#include "fileOperations.h"
#include "userData.h"

using namespace std;


int main()
{
	try{
	std::list<userData> dataList;
	SQL data;
	data.Open("C:\\DataBase\\first.db");
	fileOperations op;
	data.createTable();
	data.createIndex();
	op.readData(dataList);
	//op.inputData(dataList);
	data.insertData(dataList);
	data.bindName(dataList);
	data.selectData(dataList);
	data.~SQL();
	}
	catch (SQLException &ex) {
		std::cerr << "An exception occurred: "<<ex.what()<< " number "<<ex.geterrcode();
	}
	return 0;
}