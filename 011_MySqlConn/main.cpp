#include <iostream>
#include "mysql.h"

//CREATE DATABASE testdb;
//USE testdb;
//CREATE TABLE test(id int, name varchar(32), surname verchar(32));
//INSERT INTO test(id, name, surame) VALUES(1, "Marianne", "Loghart");
//INSERT INTO test(id, name, surame) VALUES(2, "Jimmy", "Page");
//INSERT INTO test(id, name, surame) VALUES(3, "Ling", "Montage");
//SELECT * FROM test;

using namespace std;

int main(void)
{	
	MYSQL * conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	conn = mysql_init(0);
	conn = mysql_real_connect(conn, "localhost", "root", "", "test", 3306, NULL, 0);
	if (conn)
	{
		const char * query = "SELECT * FROM test";
		mysql_query(conn, "CREATE DATABASE testdb1");
		mysql_query(conn, "USE testdb1");
		mysql_query(conn, "CREATE TABLE test(id int, name varchar(32), surname varchar(32))");
		mysql_query(conn, "INSERT INTO test(id, name, surname) VALUES(1, 'Marianne', 'Loghart')");
		mysql_query(conn, "INSERT INTO test(id, name, surname) VALUES(2, 'Jimmy', 'Page')");
		mysql_query(conn, "INSERT INTO test(id, name, surname) VALUES(4, 'Ling', 'Montage')");
		mysql_query(conn, query);
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
		{
			if (atoi(row[0]) == 1)
				std::cout << "ID: " << row[0] << " Name: " << row[1] << " SurName: " << row[2] << std::endl;
		}
	}
	
	mysql_close(conn);
	return 0;
}