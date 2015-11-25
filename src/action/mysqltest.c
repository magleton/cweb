#include "../../lib/cgic/cgic.h"
#include "../../lib/ctemplate/ctemplate.h"
#include "../../lib/oop/lw_new.h"
#include "../../lib/base/cmysql.h"

int cgiMain() {
	cgiHeaderContentType("text/html;charset=utf-8");
	tableField **table_field = NULL;
	int i, field_cnt = 0;

		field_cnt = getTableFields("user", &table_field); //获取表的信息
		for (i = 0; i < field_cnt; ++i) {
			if (strcmp(table_field[i]->field_name, "id")) { //排除掉ID字段
				fprintf(cgiOut,
						"field_cnt= %d Field =%s Type=%d Length=%d<br/>",
						field_cnt, table_field[i]->field_name,
						table_field[i]->field_type,
						table_field[i]->field_length);
			}
		}
	return 0;
}