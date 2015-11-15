#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  int count = 0;
  char *qs = getenv("QUERY_STRING");
  char username[30];
  sscanf(qs, "username=%s", username);
  printf("Content-type: text/html;charset=gbk\r\n\r\n");
  printf("<html><head></head><body>");
  if (strcmp(username, "admin") == 0) {
    printf("<h1 style='color:red;'>login successfully...</h1>");
  } else {
    printf("<h2 style='color:green;'>login failly...</h1>");
  }
  printf("</body></html>");
  return 0;
}