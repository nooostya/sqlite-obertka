
#include <iostream>
#include "sqlite3.h"
#include <stdio.h>
#include <string>
#include <conio.h>
#include <iterator>
#include <list>
#include <fstream>
using namespace std;

struct userData
{
	string name;
	int birthday;
	int number;
};

typedef std::list<userData> UserDataList;

class SQL {
private:
	sqlite3* db;
	const char* dir = "C:\\DataBase\\first.db";
	
public:
	SQL() {

	}
	int Open(const char* dir)
	{
		sqlite3_open(dir, &db);
	}

	~SQL()
	{
		sqlite3_close(db);
	}
};

class Operations{
public:
Operations(){}
int createTable(sqlite3* db) {

	string sql = "CREATE TABLE IF NOT EXISTS BIRTHDAYS("
		"NUMBER INTEGER PRIMARY KEY,"
		"NAME TEXT NOT NULL,"
		"birthday INT NOT NULL)";
	try
	{
		int exit = 0;

		char* messageError;
		exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &messageError);
		if (exit != SQLITE_OK) {
			cerr << "Error creating table" << endl;
			sqlite3_free(messageError);

		}
		else
			cout << "Table created successfully" << endl;
	}
	catch (const exception & e)
	{
		cerr << e.what();
	}

	return 0;

}
int createIndex(sqlite3* db)
{
	char* messageError;

	string sql("CREATE INDEX IF NOT EXISTS main ON BIRTHDAYS(name)");
	sqlite3_exec(db, sql.c_str(), NULL, 0, &messageError);
	return 0;
}
int readData(UserDataList& dataList) {
	string line;
	fstream f("C:\\DataBase\\names.txt", ios::in);
	userData d;
	d.number = 1;
	if (!f.is_open())
	{
		cout << "couldn`t open file";
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
	string name;
	int16_t birthday;
	string birthdayStr;
	while (true)
	{
		std::cout << "name: " << endl;

		getline(cin, name);
		if (name.empty())
		{
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
	sqlite3_stmt *stmt;
	int err;
	string name;
	const char *sql = "INSERT INTO Birthdays (name,birthday) VALUES (?,?)";
	err = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (err != SQLITE_OK)
	{
		cout << "execution failed:" << sqlite3_errmsg(db) << endl;
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
	sqlite3_stmt *stmt;
	string name;
	userData a;
	sqlite3_prepare_v2(db, "SELECT * FROM birthdays WHERE name = ?", -1, &stmt, 0);
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
	SQL db;
	db.Open;
	Operations op;
	
	db.~SQL();
	
	

	return 0;
}