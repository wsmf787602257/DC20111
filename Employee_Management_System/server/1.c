#include "./include/func.h"
int main(int argc, const char *argv[])
{
	int ret;
	
	ret = serverInitSqlite3();
	if(ret < 0)
	{
		printf("initsqlite error\n");
		goto END;
	}
	memset(sql, 0, sizeof(256));
	sprintf(sql, "select max(idnumber) as maxvalue from admin");

	ret = sqlite3_exec(employeedb, sql, userMaxIdCallback, NULL, &errmsg);
	if(ret < 0)
	{
		fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
		return -1;
	}

	int row;
	int column;
	memset(sql, 0, sizeof(256));
	sprintf(sql, "select * from user where name='asdqw'");
	ret = sqlite3_get_table(employeedb, sql, &psresult, &row,&column, &errmsg);
	printf("ret = %d\n",ret);
	if(ret < 0)
	{
		printf("errmsg %s\n",errmsg);
	}
	printf("row =%d column = %d\n",row, column);
END:
	sqlite3_close(employeedb);
	return 0;
}
