
#include <iostream>
#include "sqlite3.h"
#include <stdio.h>
#include <string>
#include <conio.h>
#include <iterator>
#include <list>
#include <fstream>
#include <exception>
using namespace std;

struct userData
{
	string name;
	int birthday;
	int number;
};

typedef std::list<userData> UserDataList;
const char* dir = "C:\\DataBase\\first.db";

class SQLException:public exception
{
private:

public:
	SQLException(const char* msg):exception (msg){
	}
};

class SQL {
private:
	sqlite3* db;
	sqlite3_stmt *stmt;
	char* messageError;
	userData a;
	string line;
	

public:
	
	SQL() : db(nullptr){

	}

	void Open(const char* dir)
	{
		
		int rc = sqlite3_open(dir, &db);
		if (rc) 
		{
			throw SQLException("Database opening error");
		}
		else {
			fprintf(stdout, "Opened database successfully\n");
		}
	}
	void createTable() {

		string sql = "CREATE TABLE IF NOT EXISTS BIRTHDAYS("
			"NUMBER INTEGER PRIMARY KEY,"
			"NAME TEXT NOT NULL,"
			"birthday INT NOT NULL)";
		
		int rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &messageError);
		if (rc != SQLITE_OK) {
			throw SQLException("error creating table");
			
		}
		else
			cout << "Table created successfully" << endl;

	}
	int createIndex()
	{

		string sql("CREATE INDEX IF NOT EXISTS main ON BIRTHDAYS(name)");
		int rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &messageError);
		if (rc != SQLITE_OK) {
			throw SQLException("error creating index");
		}
		else {
			cout<< "Index created successfully"<<endl;
		}
		return 0;
	}
	int insertData(UserDataList& dataList)
	{

		const char *sql = "INSERT INTO Birthdays (name,birthday) VALUES (?,?)";
		int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
		if (rc != SQLITE_OK)
		{
			throw SQLException("insert error");
		}

		for (UserDataList::iterator it = dataList.begin(); it != dataList.end(); it++)
		{
			sqlite3_bind_text(stmt, 1, it->name.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(stmt, 2, it->birthday);
			rc = sqlite3_step(stmt);

			sqlite3_reset(stmt);
		}
		dataList.clear();
		sqlite3_finalize(stmt);
		return 0;
	}
	int enterName(UserDataList& dataList)
	{

		int rc = sqlite3_prepare_v2(db, "SELECT * FROM birthdays WHERE name = ?", -1, &stmt, 0);
		if (rc != SQLITE_OK)
		{
			throw SQLException("select error");
		}
		cout << "Enter the name you`re looking for" << endl;
		string name;
		cin >> name;
		sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			a.number = sqlite3_column_int(stmt, 0);
			a.name = (const char*)(sqlite3_column_text(stmt, 1));
			a.birthday = sqlite3_column_int(stmt, 2);
			dataList.push_back(a);
		}
		if (sqlite3_step(stmt) != SQLITE_ROW) {
			throw SQLException("name not found");
		}

		sqlite3_finalize(stmt);
		return 0;

	}
	int selectData(UserDataList& dataList) {
		for (UserDataList::iterator it = dataList.begin(); it != dataList.end(); it++)
		{
			cout << it->number << "|" << it->name << "|" << it->birthday << endl;
		}
		return 0;
	}
	~SQL()
	{
		if (db != nullptr)
			sqlite3_close(db);
	}
};

class fileOperations{
private:
	userData d;
	string name, birthdayStr;
	int16_t birthday;
public:
fileOperations(){}


void readData(UserDataList& dataList) {

	
	fstream f("C:\\DataBase\\names.txt", ios::in);
	d.number = 1;
	if (!f.is_open())
	{
		throw SQLException ("couldn`t open file");
	}
	while (!f.eof())
	{
		f >> d.name >> d.birthday;
		d.number++;
		dataList.push_back(d);
	}
	f.close();
}
void inputData(UserDataList& dataList)
{
	
	while (true)
	{
		std::cout << "name: " << endl;

		getline(cin, name);
		if (name.empty())
		{
			std::cout << "Name error" << endl;
			throw SQLException("name error");
			break;
		}
	again:
		std::cout << "birthday : ";
		getline(cin, birthdayStr);

		if (birthdayStr.empty())
		{
			break;
		}

		birthday = atoi(birthdayStr.c_str());
		if (birthday == 0) {
			cout << "Not valid, try again" << endl;
			goto again;
		}

		userData d;
		d.name = name;
		d.birthday = birthday;
		dataList.push_back(d);

	}
	
}

~fileOperations(){}
};




int main()
{
	try{
	std::list<userData> dataList;
	SQL data;
	data.Open(dir);
	fileOperations op;
	data.createTable();
	data.createIndex();
	op.readData(dataList);
	//op.inputData(dataList);
	data.insertData(dataList);
	data.enterName(dataList);
	data.selectData(dataList);
	data.~SQL();
	}
	catch (SQLException &ex) {
		std::cerr << "An exception occurred (" << ex.what()<< ")\n";
	}
	return 0;
}