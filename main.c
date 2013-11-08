#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include<GL/glut.h>
#include<GL/gl.h>

#define BUFLEN 256
#define PART 360

int sock;
const char *py = "./ir.py";
const char get[256] = "GET\n", quit[256] = "QUT\n";
int number, x[10], y[10];
double point[2][2], x_point, y_point; //point[0:l,1:r][0:x,1:y]

void display(void) {
	int i, n = PART;
	float x, y, r = 0.7, x_1, y_1, r_1 = 0.6;
	double rate;

        glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_TRIANGLE_STRIP);

	for(i = 0; i < n + 1; i++) {
		rate = (double)i / n;
		x = r * cos(2.0 * M_PI * rate);
		y = r * sin(2.0 * M_PI * rate);
		x_1 = r_1 * cos(2.0 * M_PI * rate);
		y_1 = r_1 * sin(2.0 * M_PI * rate);
		glVertex3f(x, y, 0.0);
		glVertex3f(x_1, y_1, 0.0);
	}
        glEnd();
	glBegin(GL_QUADS);
		glVertex3f(x_point + 0.05, y_point + 0.05, 0.0);
		glVertex3f(x_point + 0.05, y_point - 0.05, 0.0);
		glVertex3f(x_point - 0.05, y_point - 0.05, 0.0);
		glVertex3f(x_point - 0.05, y_point + 0.05, 0.0);
	glEnd();
        glFlush();
}

void timer(int value) {
	int i, count;
	char buf[BUFLEN];

	write(sock, get, strlen(get));
        usleep(100);
	read(sock, buf, BUFLEN);

	count = sscanf(buf, "%d", &number);
	printf("%d\n", number);

	for(i = 0; i < number; i++) {                  
		count += sscanf(buf + count, "%d", &x[i]);
		count += sscanf(buf + count, "%d", &y[i]);
//		printf("%d %d\n", x[i], y[i]);
	}
	if(x[0] > 0 && y[0] > 0 && x[1] > 0 && y[1] > 0){
		if(x[0]>x[1]){
			point[0][0] = (double)x[1] / 1000;
			point[0][1] = (double)y[1] / 1000;
			point[1][0] = (double)x[0] / 1000;
			point[1][1] = (double)y[0] / 1000;
		}else{
			point[0][0] = (double)x[0] / 1000;
                        point[0][1] = (double)y[0] / 1000;
                        point[1][0] = (double)x[1] / 1000;
                        point[1][1] = (double)y[1] / 1000;
		}
	}else{
		point[0][0] = 0;//(double)x[1] / 1100;
                point[0][1] = 0;//(double)y[1] / 600;
                point[1][0] = 0;//(double)x[0] / 1100;
                point[1][1] = 0;//(double)y[0] / 600;		
	}
	x_point = (point[0][0] + point[1][0]) / 2;
	y_point = (point[0][1] + point[1][1]) / 2;

	for(i = 0; i< 2; i++){
		printf("%f %f\n",point[i][0],point[i][1]);
	}	

	glutPostRedisplay();
	glutTimerFunc(100, timer, 0);
}

int main(int argc , char *argv[]) {
	struct sockaddr_in server;

	glutInit(&argc, argv);
	glutInitWindowSize(500, 500);
	glutInitDisplayMode(GLUT_RGBA);
        glutCreateWindow("head-tracking");

	sock = socket(AF_INET, SOCK_STREAM, 0);
	server.sin_family = AF_INET;
	server.sin_port = htons(12357);
	inet_aton("127.0.0.1", &(server.sin_addr));
	connect(sock, (struct sockaddr *)&server, sizeof(server));

        glutDisplayFunc(display);
        glutTimerFunc(100, timer, 0);
        glutMainLoop();

	close(sock);
        return 0;
}
