main: main.cpp Point/Point.cpp Camera/Camera.cpp Object/Object.cpp Color/Color.cpp \
	Object/CheckerBoard/CheckerBoard.cpp Object/Sphere/Sphere.cpp Object/Triangle/Triangle.cpp \
	PointLight/PointLight.cpp SpotLight/SpotLight.cpp \
	Ray/Ray.cpp 
	g++ main.cpp \
		Point/Point.cpp Camera/Camera.cpp Object/Object.cpp Color/Color.cpp \
		Object/CheckerBoard/CheckerBoard.cpp Object/Sphere/Sphere.cpp Object/Triangle/Triangle.cpp \
		PointLight/PointLight.cpp SpotLight/SpotLight.cpp \
		Ray/Ray.cpp \
	-o main.out -lglut -lGLU -lGL

run: 
	make main && ./main.out spec/description.txt

clean:
	rm -f *.out