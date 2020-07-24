
#include <iostream>
#include "sqlite3.h"
#include <exception>
#include "SQLException.h"
#include "SQL.h"
#include "fileOperations.h"
#include "userData.h"
#include <string>
#include "windows.h"

int main()
{
	try{
	std::list<userData> dataList;
std::string name;
	SQL data;
	data.Open("C:\\DataBase\\first.db");
	fileOperations op;
	op.readData(dataList);
	//op.inputData(dataList);
	data.insertData(dataList);
	std::cout << "Enter the name you`re looking for" << std::endl;
	std::cin >> name;
	data.bindName(dataList, name);
	
	data.selectData(dataList);
	data.~SQL();
	}
	catch (SQLException &ex) {
		std::cerr << "An exception occurred: "<<ex.what()<< ", error number "<<ex.geterrcode();
	}
	return 0;
}