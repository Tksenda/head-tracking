#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<GL/glut.h>

FILE *fp;
char *py="./ir.py";
char *get="GET\n";
int number,x[10],y[10],i;

void display( void ) {
        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_POLYGON);
	
        glEnd();
	printf("display\n");
        glFlush();
}

void timer(int value){
	printf("timer\n");
	fputs(get,fp);
	printf("timer2\n");
	scanf("%d",&number);
	printf("timer3\n");
	if(number == 0){
		printf("%d\n",number);
		printf("No hoge found\n");
	}
	printf("timer4\n");
	for(i=0;i<number;i++){                  
		fscanf(fp,"%d",&x[i]);
		fscanf(fp,"%d",&y[i]);
	}
	printf("timer5\n");
	glutPostRedisplay();
	glutTimerFunc(50 , timer , 0);
}

int main(int argc , char *argv[]) {
	fp = popen(py,"w");
	sleep(10);
        glutInit(&argc , argv);
        glutInitWindowPosition(100 , 50);
        glutInitWindowSize(400 , 300);
        glutInitDisplayMode(GLUT_RGBA);
        glutCreateWindow("head-tracking");
        glutDisplayFunc(display);
        glutTimerFunc(100 , timer , 0);

        glutMainLoop();
	pclose(fp);
        return 0;
}                    
