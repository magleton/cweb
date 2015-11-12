# cweb 是一个使用C语言开发WEB的应用。使用mysql作为数据库，基于Ubuntu开发

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

```
gcc -o cgi-bin/test.cgi src/test.c lib/cgic/cgic.c lib/ctemplate/ctemplate.c
```
