#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <stdbool.h>
#define PI (3.14159)

GLfloat pos0[] = { 5.0, 0.0, 0.0, 1.0 };
GLfloat pos1[] = { 0.0, 0.0, 5.0, 1.0 };
enum COLOR { WHITE, RED, GREEN, BLUE, YELLOW, MAGENTA, CYAN, GRAY, BLACK };
GLfloat color[][4] = {
		{ 1.0, 1.0, 1.0, 1.0 },
		{ 1.0, 0.0, 0.0, 1.0 },
		{ 0.0, 1.0, 0.0, 1.0 },
		{ 0.0, 0.0, 1.0, 1.0 },
		{ 1.0, 1.0, 0.0, 1.0 },
		{ 1.0, 0.0, 1.0, 1.0 },
		{ 0.0, 1.0, 1.0, 1.0 },
		{ 0.7, 0.7, 0.7, 1.0 },
		{ 0.0, 0.0, 0.0, 1.0 } };//色を増やす場合はここに追加
double x = 0;
double y = 0;
double z = 0;
int mySpecialValue = 0;
double tekiList[][3] = {
		{ 0.0, 2.0, 0.0 },
		{ 4.0, 4.0, 0.0 },
		{ 2.0, 6.0, 0.0 },
		{ 0.0, 10.0, 0.0 },
		{ 4.0, 13.0, 0.0 },
		{ 3.0, 14.0, 0.0 },
		{ 2.0, 15.0, 0.0 },
		{ 1.0, 16.0, 0.0 },
		{ 3.0, 19.0, 0.0 },
		{ 2.0, 20.0, 0.0 },
		{ 1.0, 21.0, 0.0 },
		{ 0.0, 22.0, 0.0 } };//障害物のリスト
int tekiIndex = 12;

int XG = 5;//地面
int YG = 25;
double L = 1;

bool jumpFlag = false; // jumpしているかどうか。ジャンプ中はtrue。

void calcNormal(GLdouble v0[3], GLdouble v1[3], GLdouble v2[3], GLdouble n[3])
{
	GLdouble v2v[2][3];
	GLdouble vt[3];
	double abs;
	int i;
	for (i = 0; i < 3; i++)
	{
		v2v[0][i] = v1[i] - v0[i];
		v2v[1][i] = v2[i] - v0[i];
	}
	vt[0] = v2v[0][1] * v2v[1][2] - v2v[0][2] * v2v[1][1];
	vt[1] = v2v[0][2] * v2v[1][0] - v2v[0][0] * v2v[1][2];
	vt[2] = v2v[0][0] * v2v[1][1] - v2v[0][1] * v2v[1][0];
	abs = sqrt(vt[0] * vt[0] + vt[1] * vt[1] + vt[2] * vt[2]);
	for (i = 0; i < 3; i++)
		n[i] = vt[i] / abs;
}

void drawGround()
{
	int i, j;
	GLdouble normal[3] = { 0.0, 0.0, 1.0 };

	glPushMatrix();

	glNormal3dv(normal);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, color[YELLOW]);
	glMaterialfv(GL_FRONT, GL_AMBIENT, color[BLACK]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, color[WHITE]);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
	glBegin(GL_QUADS);
	glVertex3d(-0.5*L, -0.5*L, 0.0);//スタート
	glVertex3d((XG - 0.5)*L, -0.5*L, 0.0);
	glVertex3d((XG - 0.5)*L, 0.5*L, 0.0);
	glVertex3d(-0.5*L, 0.5*L, 0.0);

	glVertex3d(-0.5*L, (YG - 0.5)*L, 0.0);//ゴール
	glVertex3d((XG - 0.5)*L, (YG - 0.5)*L, 0.0);
	glVertex3d((XG - 0.5)*L, (YG + 0.5)*L, 0.0);
	glVertex3d(-0.5*L, (YG + 0.5)*L, 0.0);
	glEnd();

	glMaterialfv(GL_FRONT, GL_DIFFUSE, color[GRAY]);//灰色
	glMaterialfv(GL_FRONT, GL_AMBIENT, color[BLACK]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, color[WHITE]);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
	glBegin(GL_QUADS);
	for (i = 1; i < YG; i++)
		for (j = 0; j < XG; j++)
		{
		GLdouble v[4][3];
		if (!((i^j) & 1))continue;
		v[0][0] = (j + 0 - 0.5)*L;
		v[0][1] = (i + 0 - 0.5)*L;
		v[0][2] = 0;
		v[1][0] = (j + 1 - 0.5)*L;
		v[1][1] = (i + 0 - 0.5)*L;
		v[1][2] = 0;
		v[2][0] = (j + 1 - 0.5)*L;
		v[2][1] = (i + 1 - 0.5)*L;
		v[2][2] = 0;
		v[3][0] = (j + 0 - 0.5)*L;
		v[3][1] = (i + 1 - 0.5)*L;
		v[3][2] = 0;
		glVertex3dv(v[0]);
		glVertex3dv(v[1]);
		glVertex3dv(v[2]);
		glVertex3dv(v[3]);
		}
	glEnd();

	glMaterialfv(GL_FRONT, GL_DIFFUSE, color[BLACK]);//黒
	glMaterialfv(GL_FRONT, GL_AMBIENT, color[BLACK]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, color[WHITE]);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
	glBegin(GL_QUADS);
	for (i = 0; i < YG; i++)
		for (j = 0; j < XG; j++)
		{
		GLdouble v[4][3];
		if ((i^j) & 1)continue;
		v[0][0] = (j + 0 - 0.5)*L;
		v[0][1] = (i + 0 - 0.5)*L;
		v[0][2] = 0;
		v[1][0] = (j + 1 - 0.5)*L;
		v[1][1] = (i + 0 - 0.5)*L;
		v[1][2] = 0;
		v[2][0] = (j + 1 - 0.5)*L;
		v[2][1] = (i + 1 - 0.5)*L;
		v[2][2] = 0;
		v[3][0] = (j + 0 - 0.5)*L;
		v[3][1] = (i + 1 - 0.5)*L;
		v[3][2] = 0;
		glVertex3dv(v[0]);
		glVertex3dv(v[1]);
		glVertex3dv(v[2]);
		glVertex3dv(v[3]);
		}
	glEnd();

	glPopMatrix();
}

double theta;
void drawJiki(void)
{
	glPushMatrix();

	glTranslatef(x, y, z);
	glTranslatef(0.0, 0.0, 0.5);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, color[GREEN]);
	glMaterialfv(GL_FRONT, GL_AMBIENT, color[BLACK]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, color[WHITE]);
	glMaterialf(GL_FRONT, GL_SHININESS, 50.0);

	glutSolidSphere(0.5, 100, 100);
	glPopMatrix();
}
void drawTeki(void)
{
	int i;
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, color[RED]);
	glMaterialfv(GL_FRONT, GL_AMBIENT, color[BLACK]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, color[WHITE]);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0);

	for (i = 0; i < tekiIndex; i++)
	{
		glPushMatrix();
		glTranslatef(tekiList[i][0], tekiList[i][1], tekiList[i][2]);
		glutSolidCone(0.5, 1, 30, 30);
		glPopMatrix();
	}
	glPopMatrix();
}
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();

	drawGround();
	drawJiki();
	drawTeki();

	glPopMatrix();
	glutSwapBuffers();
}
int collision()
{
	//衝突判定
	int i;
	double MARGIN = 0.05;
	for (i = 0; i < tekiIndex; i++)
	{
		//簡単な衝突判定：立方体同士の衝突判定
		if ((tekiList[i][0] - x <1 - MARGIN) && (tekiList[i][0] - x >-1 + MARGIN)
			&& (tekiList[i][1] - y <1 - MARGIN) && (tekiList[i][1] - y >-1 + MARGIN)
			&& (tekiList[i][2] - z <1 - MARGIN) && (tekiList[i][2] - z >-1 + MARGIN))
		{
			printf("(%.02f,%.02f,%.02f):(%.02f,%.02f,%.02f)\n", x, y, z, tekiList[i][0], tekiList[i][1], tekiList[i][2]);
			return 1;
		}
	}
	return 0;
}

void myTimerFunc(int value)
{
	int groundXMin = 0, groundXMax = (XG-1)*L, groundYMin = 0, groundYMax = (YG-1)*L;
	if (mySpecialValue & (1 << 0))
	{
		y += 0.1;
		if (collision())y -= 0.1;
		if (y > groundYMax)y = groundYMax;
	}
	else if (mySpecialValue & (1 << 1))
	{
		x -= 0.1;
		if (collision())x += 0.1;
		if (x < groundXMin)x = groundXMin;
	}
	else if (mySpecialValue & (1 << 2))
	{
		x += 0.1;
		if (collision())x -= 0.1;
		if (x > groundXMax)x = groundXMax;
	}
	else if (mySpecialValue & (1 << 3))
	{
		y -= 0.1;
		if (collision())y += 0.1;
		if (y < groundYMin)y = groundYMin;
	}

	//視点を移動
	glLoadIdentity();
	gluLookAt(0.0 + x, -10.0 + y, 2.0, 0.0 + x, 0.0 + y, 1.5, 0.0, 0.0, 1.0);

	glutTimerFunc(10, myTimerFunc, 0);
}

void jump(int value) {
	double v = 0.2;//ジャンプの速度
	double g = -0.01;//落ちる加速度
	z = v*value + g*value*value / 2;
	if (collision()) {
		printf("z衝突");
		z = v*(value-1) + g*(value-1)*(value-1) / 2; // 衝突前のz座標に戻す
		glutTimerFunc(10, jump, value);//次の処理を登録
		return;
	}
	if (z < 0)
	{
		z = 0;
		jumpFlag = false;
	}
	else
	{
		glutTimerFunc(50, jump, value + 1);//次の処理を登録
	}
}

void myKeyboardFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
	case ' ': // スペースキーが押されたとき
		if (!jumpFlag) {
			jump(0);
			jumpFlag = true;
		}
		break;
	}
}
void mySpcialFunc(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		mySpecialValue |= 1 << 0;//mySpecialValueの1bit目を1にする
		break;
	case GLUT_KEY_LEFT:
		mySpecialValue |= 1 << 1;//mySpecialValueの2bit目を1にする
		break;
	case GLUT_KEY_RIGHT:
		mySpecialValue |= 1 << 2;//mySpecialValueの3bit目を1にする
		break;
	case GLUT_KEY_DOWN:
		mySpecialValue |= 1 << 3;//mySpecialValueの4bit目を1にする
		break;
	default:
		break;
	}
}
void mySpcialUpFunc(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		mySpecialValue &= ~(1 << 0);//mySpecialValueの1bit目を0にする
		break;
	case GLUT_KEY_LEFT:
		mySpecialValue &= ~(1 << 1);//mySpecialValueの2bit目を0にする
		break;
	case GLUT_KEY_RIGHT:
		mySpecialValue &= ~(1 << 2);//mySpecialValueの3bit目を0にする
		break;
	case GLUT_KEY_DOWN:
		mySpecialValue &= ~(1 << 3);//mySpecialValueの4bit目を0にする
		break;
	default:
		break;
	}

}

void idle(void)
{
	glutPostRedisplay();
}
void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glutKeyboardFunc(myKeyboardFunc);
	glutSpecialFunc(mySpcialFunc);
	glutSpecialUpFunc(mySpcialUpFunc);

	glutIdleFunc(idle);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, 1, .1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, -10.0, 2.0, 0.0, 0.0, 1.5, 0.0, 0.0, 1.0);
	glLightfv(GL_LIGHT1, GL_POSITION, pos1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, color[WHITE]);
	myTimerFunc(0);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("1Y18F014 井上智裕: 8-B発展課題");
	glutDisplayFunc(display);
	init();
	glutMainLoop();
	return 0;
}
