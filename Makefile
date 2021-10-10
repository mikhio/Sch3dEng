run:
	clear
	make compile
	make start
compile:
	g++ app.cpp -o ./build/app -L/opt/local/lib -lglfw -lglew -framework OpenGL -g
start:
	./build/app
