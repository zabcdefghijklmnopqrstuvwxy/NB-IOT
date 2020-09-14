#!/bin/sh

EchoUsage()
{
	echo "usage: ./make.sh --make whole project
	./make.sh clean --clean whole project
	./make.sh install --install all builded libs and exceable files" 
}

MakeExce()
{
	echo "######################### Make Sql ################################"
	cd mysql
	make $1
	cd ..

	echo "######################### Make TcpServer ##########################"
	cd tcpserver
	make $1
	cd ..
	
	echo "######################### Make Main ###############################"
	cd main
	make $1
	cd ..

	echo "######################### Make Finish ##############################"
}

if [ $# -eq 0 ] || [ $1 = "clean" ]
then
MakeExce $1
fi

if [ $# -gt 1 ] && [ $1 != "clean" ]
then
EchoUsage $1
fi

