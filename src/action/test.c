#include <stdio.h>
#include <string.h>
#include "../lib/cgic/cgic.h"
#include "../lib/ctemplate/ctemplate.h"
int cgiMain() {
	cgiHeaderContentType("text/html;charset=gbk");
	int n, f;
	char txt1[32], txt2[32];
	TMPL_varlist *mainlist, *vl;
	TMPL_loop *loop;
	TMPL_loop *persons;
	loop = 0;
	f = 1;
	for (n = 1; n < 11; n++) {
		sprintf(txt1, "%d", n);
		sprintf(txt2, "%d", f *= n);
		vl = TMPL_add_var(0, "n", txt1, "nfact", txt2, 0);
		loop = TMPL_add_varlist(loop, vl);
	}

	for (n = 10; n < 21; n++) {
		sprintf(txt1, "%d", n);
		sprintf(txt2, "%d", f *= n);
		vl = TMPL_add_var(0, "n1", txt1, "nfact1", txt2, "aa", "mmmm", 0);
		persons = TMPL_add_varlist(persons, vl);
	}

	mainlist = TMPL_add_var(0, "title", "10 factorials", 0);
	mainlist = TMPL_add_loop(mainlist, "fact", loop);
	mainlist = TMPL_add_loop(mainlist, "person", persons);
	TMPL_write("../resource/template/p1.html", 0, 0, mainlist, stdout, stderr);
	TMPL_free_varlist(mainlist);
	return 0;
}
