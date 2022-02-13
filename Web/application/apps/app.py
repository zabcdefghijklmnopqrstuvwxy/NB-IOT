from flask import Flask,render_template,url_for
from flask import request
from pymysql import *

app = Flask(__name__,static_folder='.',static_url_path='',template_folder='./templates')

@app.route('/login',methods=['GET', 'POST'])
def login():
    account=request.form['account']
    passwd=request.form['passwd']

    print("account is " + account + "\tpasswd is " + passwd)
    if account == 'admin' and passwd == '123456':
        conn = connect(host='localhost',port=3306,user='root',password='123456',database='NBIOT',charset='utf8')
        cursor = conn.cursor()
        sql = "select * from NBDEV order by id desc limit 30"
        cursor.execute(sql)
        nbdev = cursor.fetchall()
        return render_template('login.html',nbdev=nbdev)
    else:
        return render_template('index.html')


@app.route('/',methods=['GET'])
def index():
    return render_template('index.html')


if __name__ == '__main__':
    app.run(host='0.0.0.0',port=5000,debug=True)

