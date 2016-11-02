#include "main.h"

int qnt_pontos;
Ponto pnts[1000];
GLfloat mouse_x, mouse_y;
bool showPoli;

//Estado inicial
void init(){
	glClearColor(1.0, 1.0, 1.0, 1.0);
	srand(time(NULL));
	qnt_pontos = 0;
	showPoli = true;
}

//Calcula o fatorial de x
float fat(int x){
	float n = 1.0f;
	for (int i = x; i > 0; --i){
		n *= (float)i;
	}
	return n;
}

//Calcula combinacao (a b)
float comb(int a, int b){
	return fat(b)/(fat(a)*fat(b-a));
}

//Printa um ponto nas coordenadas (x,y)
void desenhaPonto(GLint x, GLint y){
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
		for(int i = 0; i < 8; i++) {
			float angulo = i * 2 * M_PI / 8;
			glVertex2f(x + (cos(angulo) * 4), y + (sin(angulo) * 4));
		}
	glEnd();
}

//Liga os pontos (x1, y1) e (x2, y2)
void ligaPontos(GLint x1, GLint y1, GLint x2, GLint y2){
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
		glVertex2i(x1, y1);
		glVertex2i(x2, y2);
	glEnd();
}

//Desenha curva de bezier de grau 3 entres os pontos (x1, y1), (x2, y2), (x3, y3) e (x4, y4) com suavidade n
void bezier(GLint x1, GLint y1, GLint x2, GLint y2, GLint x3, GLint y3, GLint x4, GLint y4, GLfloat n){
	glColor3f(0.0f,1.0f,0.0f);
	glBegin(GL_POINTS);
	for (GLfloat u = 0.0f; u <= 1.0f; u+=n){
		GLfloat x = 0.0f;
		GLfloat y = 0.0f;

		//Qualcular coordenadas para cada ponto da curva de bezier
		int i = 0;
		x += comb(i,3)*pow(u,i)*pow(1.0f-u,3-i)*x1;
        y += comb(i,3)*pow(u,i)*pow(1.0f-u,3-i)*y1;
		i = 1;
		x += comb(i,3)*pow(u,i)*pow(1.0f-u,3-i)*x2;
        y += comb(i,3)*pow(u,i)*pow(1.0f-u,3-i)*y2;
		i = 2;
		x += comb(i,3)*pow(u,i)*pow(1.0f-u,3-i)*x3;
        y += comb(i,3)*pow(u,i)*pow(1.0f-u,3-i)*y3;
		i = 3;
		x += comb(i,3)*pow(u,i)*pow(1.0f-u,3-i)*x4;
        y += comb(i,3)*pow(u,i)*pow(1.0f-u,3-i)*y4;

		glVertex2f(x,y);
	}      
	glEnd();
}

//Rotina magica (copiada do Template2D.cpp)
void reshape(GLsizei w, GLsizei h){
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, (GLfloat)w, (GLfloat)h, 0.0f, -5.0, 5.0);
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT);

	//Pinta todos os pontos de 'pnts'
	for (int i=0; i<qnt_pontos;++i){
		desenhaPonto(pnts[i].x, pnts[i].y);
		if (i>0 && showPoli) ligaPontos(pnts[i-1].x, pnts[i-1].y, pnts[i].x, pnts[i].y);
		if (i%3 == 0 && (i>0)) bezier(pnts[i-3].x, pnts[i-3].y, pnts[i-2].x, pnts[i-2].y, pnts[i-1].x, pnts[i-1].y, pnts[i].x, pnts[i].y, 0.001f);
	}

	glFlush();
}

void handleMouse(int btn, int state, int x, int y){
	//Printa as coordenadas clicando com o botao direito do mouse
	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		cout << "(" << x << ", " << y << ")" << endl;
	}
	//Adiciona o ponto clicando com o botao esquerdo do mouse
	if(btn == GLUT_LEFT_BUTTON){
		if (state == GLUT_DOWN){
			pnts[qnt_pontos++] = Ponto(x, y);
		}
	}
}

//Fecha aplicacao ao apertar ESC
void hadleKeyboard(unsigned char key, int x, int y){
	if(key == ESC){
		exit(0);
	}
}

//Reinicia ao apertar F5
void hadleSpecialKeyboard(int key, int x, int y){
	if(key == GLUT_KEY_F5){
		init();
	}
	if(key == GLUT_KEY_F4){
		showPoli = !showPoli;
	}
}

int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(960, 550);
	glutCreateWindow("BSplineInterpolation");

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMouseFunc(handleMouse);
	glutKeyboardUpFunc(hadleKeyboard);
	glutSpecialUpFunc(hadleSpecialKeyboard);

	init();

	glutMainLoop();
	return 0;
}