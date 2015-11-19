# cweb 是一个使用C语言开发的CGI WEB应用。使用mysql作为数据库，基于Ubuntu开发。

#环境配置
* nginx:
```
location ~ .*\.cgi$ {
    root /home/amcro/www/cgi-bin;
    fastcgi_pass 127.0.0.1:8080;
    fastcgi_index index.cgi;
    include fastcgi.conf;
}
```
* 安装mysql客户端库
```
sudo apt-get install libmysqlclient-dev
```
* 安装fcgiwrap
```
sudo apt-get install fcgiwrap
```
* 启动fcgiwrap
```
spawn-fcgi -f /usr/sbin/fcgiwrap -p 8080
```

#编译命令
* 没有使用MYSQL的编译命令
```
gcc -o cgi-bin/test.cgi src/test.c lib/cgic/cgic.c lib/ctemplate/ctemplate.c
```
* 使用MYSQL的编译命令
```
gcc test.c -o test  `mysql_config --cflags --libs`
```
* 最后生成CGI文件的编译命令,开启调试模式
```
gcc -o cgi-bin/login.cgi -DDEBUG src/model/impl/base.c lib/ctemplate/ctemplate.c lib/cgic/cgic.c lib/oop/lw_new.c src/action/login.c src/model/impl/user.c `mysql_config --libs --cflags`
```

* 最后生成CGI文件的编译命令
```
gcc -o cgi-bin/login.cgi src/model/impl/base.c lib/ctemplate/ctemplate.c lib/cgic/cgic.c lib/oop/lw_new.c src/action/login.c src/model/impl/user.c `mysql_config --libs --cflags`
```
