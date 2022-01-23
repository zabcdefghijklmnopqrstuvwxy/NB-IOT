#!/usr/bin/python3

import cgi, cgitb 
from pymysql import *

 
# ..FieldStorage....
form = cgi.FieldStorage() 
#..html..........
str_data_1  =  form.getvalue('data_1')
str_data_2  =  form.getvalue('data_2')

conn = connect(host='localhost',port=3306,user='root',password='',database='NBIOT',charset='utf8')
# ..Cursor..
cursor = conn.cursor()
# ..sql..
sql = "select * from NBDEV order by id desc limit 100"
cursor.execute(sql)
# ......
# ..Cursor..
#cs1.close()
#....
print("Content-type:text/html")
print("");
print("<html>")
print("<head>")
print('<meta charset="utf-8">')
print('<meta http-equiv="refresh" content="60">')
print("<title>GET</title>")
print("</head>")
print('<body style="position:relative;">')
print('<div style="position:relative;width=400px;height:100px;border:1px solid #ddd;background-color:cyan">')
print('<h2 style="margin:0 auto;width:200px;height:80px;">NBDATA LIST</h2>')
print("</div>")
print('<div style="width=800px;height:800px;margin:0 auto;border:1px solid #555;background-color:white;">')

print('<div style="margin:100px auto;width:400px;height:600px;">')
print("id&nbsp;&nbsp;&nbsp;&nbsp;devno&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;timestamp&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;gps&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;tempeature")
print("<br></br>")
for temp in cursor.fetchall():
	print(temp[0])
	print("&nbsp;&nbsp;&nbsp;")
	print(temp[1])
	print("&nbsp;&nbsp;&nbsp;")
	print(temp[2])
	print("&nbsp;&nbsp;&nbsp;")
	print(temp[3])
	print("&nbsp;&nbsp;&nbsp;")
	print(temp[4])
	print("<br></br>")
	
conn.close()
print("</div>")

print("</div>")
#print("<h2>data_1:%s,data_2:%s</h2>"%(str_data_1,str_data_2))
print("</body>")
print("</html>")

