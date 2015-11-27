# cweb 是一个使用C语言开发的CGI WEB应用。使用mysql作为数据库，基于Ubuntu开发。

#Linux环境配置
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

#Windows环境配置
* 安装Cgywin,镜像可以选择 mirrors.neusoft.edu.cn(http)或者mirrors.ustc.edu.cn(http).或者<a href="https://cygwin.com/mirrors.html" target="_blank">参考</a>

* 配置Apache,打开httpd.conf. __注意：Apache不能被安装为服务，只能在命令行启动__.<a href="http://www.apachelounge.com/download/"  target="_blank">下载绿色版Apache</a>

* 找到 LoadModule cgi_module modules/mod_cgi.so ， 并去掉前面的#号.

* 找到下面相应的章节，修改为下面的格式
```
<IfModule alias_module>
    ScriptAlias /cgi-bin/  "这里配置为DocumentRoot的值"
</IfModule>
<Directory "这里配置为DocumentRoot的值">
    AllowOverride None
    Options +ExecCGI
    Require all granted
</Directory>
<IfModule mime_module>
    AddHandler cgi-script .cgi
</IfModule>
```
* 在Cygwin里面安装crypt和libmysqlclient-devel


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
gcc -o cgi-bin/login.cgi src/action/login.c src/model/impl/md5.c lib/base/cmysql.c src/model/impl/session.c lib/ctemplate/ctemplate.c lib/cgic/cgic.c lib/oop/lw_new.c src/model/impl/upload.c src/model/impl/user.c src/model/impl/base.c  `mysql_config --libs --cflags` -DDEBUG
```

* 最后生成CGI文件的编译命令
```
gcc -o cgi-bin/login.cgi src/action/login.c src/model/impl/md5.c lib/base/cmysql.c src/model/impl/session.c lib/ctemplate/ctemplate.c lib/cgic/cgic.c lib/oop/lw_new.c src/model/impl/upload.c src/model/impl/user.c src/model/impl/base.c  `mysql_config --libs --cflags`
```

* 开启session功能，并且没有配置独立**域名**的编译命令
```
gcc -o cgi-bin/login.cgi src/action/login.c src/model/impl/md5.c lib/base/cmysql.c src/model/impl/session.c lib/ctemplate/ctemplate.c lib/cgic/cgic.c lib/oop/lw_new.c src/model/impl/upload.c src/model/impl/user.c src/model/impl/base.c  `mysql_config --libs --cflags` -DDEVELOPMENT
```
