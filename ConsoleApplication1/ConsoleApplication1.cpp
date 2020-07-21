
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
sqlite3* db;
class SQLException:public exception
{
private:

public:
	SQLException(const char* msg):exception (msg){
	}
};

class SQL {
private:
	
	int rc;
	
public:
	
	SQL() {

	}
	int Open(const char* dir)
	{
		
		rc = sqlite3_open(dir, &db);
		if (rc) {
			fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
			throw SQLException("Database opening error");
		}
		else {
			fprintf(stdout, "Opened database successfully\n");
		}
		return 0;
		
	}

	~SQL()
	{
		sqlite3_close(db);
	}
};

class Operations{
private:
	int rc;
	int exit = 0;
	char* zErrMsg;
	char* messageError;
	string name;
	int16_t birthday;
	string birthdayStr;
	sqlite3_stmt *stmt;
	int err;
	userData a;
	string line;
	userData d;
	
public:
Operations(){}
int createTable(sqlite3* db) {

	string sql = "CREATE TABLE IF NOT EXISTS BIRTHDAYS("
		"NUMBER INTEGER PRIMARY KEY,"
		"NAME TEXT NOT NULL,"
		"birthday INT NOT NULL)";
	
		

		char* messageError;
		exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &messageError);
		if (exit != SQLITE_OK) {
			cerr << "Error creating table" << endl;
			throw SQLException("error creating table");
			sqlite3_free(messageError);
			return 1;
		}
		else
			cout << "Table created successfully" << endl;

	return 0;

}
int createIndex(sqlite3* db)
{
	
	string sql("CREATE INDEX IF NOT EXISTS main ON BIRTHDAYS(name)");
	rc=sqlite3_exec(db, sql.c_str(), NULL, 0, &messageError);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Index not created, error: %s\n", zErrMsg);
		throw SQLException("error creating index");
		sqlite3_free(zErrMsg);
		return 1;
	}
	else {
		fprintf(stdout, "Index created successfully\n");
	}
	return 0;
}
int readData(UserDataList& dataList) {

	
	fstream f("C:\\DataBase\\names.txt", ios::in);
	d.number = 1;
	if (!f.is_open())
	{
		cout << "couldn`t open file";
		throw SQLException ("couldn`t open file");
		return 1;
	}
	while (!f.eof())
	{
		f >> d.name >> d.birthday;
		d.number++;
		dataList.push_back(d);
	}
	f.close();
	return 0;
}
int inputData(UserDataList& dataList)
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
	return 0;
}
int insertData(sqlite3* db, UserDataList& dataList)
{
	
	const char *sql = "INSERT INTO Birthdays (name,birthday) VALUES (?,?)";
	err = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (err != SQLITE_OK)
	{
		cout << "execution failed:" << sqlite3_errmsg(db) << endl;
		throw SQLException("insert error");
		return err;
	}

	for (UserDataList::iterator it = dataList.begin(); it != dataList.end(); it++)
	{
		sqlite3_bind_text(stmt, 1, it->name.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_int(stmt, 2, it->birthday);
		err = sqlite3_step(stmt);

		sqlite3_reset(stmt);
	}
	dataList.clear();
	sqlite3_finalize(stmt);
	return 0;
}
int enterName(sqlite3* db, UserDataList& dataList)
{
	
	rc=sqlite3_prepare_v2(db, "SELECT * FROM birthdays WHERE name = ?", -1, &stmt, 0);
	if (rc != SQLITE_OK)
	{
		cout << "select failed:" << sqlite3_errmsg(db) << endl;
		throw SQLException("select error");
		return err;
	}
	cout << "Enter the name you`re looking for" << endl;
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
int selectData(sqlite3* db, UserDataList& dataList) {
	for (UserDataList::iterator it = dataList.begin(); it != dataList.end(); it++)
	{
		cout << it->number << "|" << it->name << "|" << it->birthday << endl;
	}
	return 0;
}
~Operations(){}
};




int main()
{
	try{
	std::list<userData> dataList;
	SQL data;
	data.Open(dir);
	Operations op;
	op.createTable(db);
	op.createIndex(db);
	op.readData(dataList);
	//op.inputData(dataList);
	op.insertData(db, dataList);
	op.enterName(db, dataList);
	op.selectData(db, dataList);
	
	data.~SQL();
	}
	catch (SQLException &ex) {
		std::cerr << "An exception occurred (" << ex.what()<< ")\n";
	}
	return 0;
}