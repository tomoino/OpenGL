#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <string.h>

#define DIVISION 100 // 分割数。多いほど曲面に近くなる。

GLfloat GREEN[] = {0.6, 0.9, 0.4, 1.0};		//緑色
GLfloat WHITE[] = {1.0, 1.0, 1.0, 1.0};		//白色
GLfloat BLUE[] = {0.2, 0.4, 0.8, 1.0};		//青色
GLfloat GLASS[] = {0.2, 0.4, 0.8, 0.4};		//透明青色

// TODO: glVertex3dv
void Cylinder(double height, double radius, double x, double y, double z) {
  glPushMatrix(); // 座標系の保存
  glTranslatef(x, y, z); // 座標変換
  double step = 2.0 * M_PI / (double)DIVISION; // 側面1辺あたりの中心角
  int i;
  double theta = 0;
  /* 上面  */
  glNormal3d(0.0, 1.0, 0.0);
  glBegin(GL_POLYGON);
  for (i = 0; i < DIVISION; i++) {
    glVertex3d(radius * sin(theta), height, radius * cos(theta));
    theta += step;
  }
  glEnd();

  /* 下面 */
  glNormal3d(0.0, -1.0, 0.0);
  glBegin(GL_POLYGON);
  theta = step * DIVISION;
  for (i = DIVISION; --i >= 0; ) {
    glVertex3d(radius * sin(theta), 0.0, radius * cos(theta));
    theta -= step;
  }
  glEnd();

  /* 側面 */
  glBegin(GL_QUAD_STRIP);
  theta = 0;
  for (i = 0; i <= DIVISION; i++) {
    double x = sin(theta);
    double z = cos(theta);

    glNormal3d(x, 0.0, z);
    glVertex3f(radius * x, height, radius * z);
    glVertex3f(radius * x, 0.0, radius * z);
    theta += step;
  }
  glEnd();
  glPopMatrix();
}

void Sphere (double radius, double x, double y, double z) {
  glPushMatrix(); // 座標系の保存
  glTranslatef(x, y, z); // 座標変換
  glutSolidSphere(radius, DIVISION, DIVISION);//半径, Z軸まわりの分割数, Z軸に沿った分割数
  glPopMatrix(); // 座標系の保存
}

void Capsule (double height, double radius, double x, double y, double z) {
  glPushMatrix(); // 座標系の保存
  glTranslatef(x, y, z); // 座標変換
  Sphere(radius, 0, height, 0);//半径, Z軸まわりの分割数, Z軸に沿った分割数
  Cylinder(height, radius, 0, 0, 0);
  Sphere(radius, 0, 0, 0);//半径, Z軸まわりの分割数, Z軸に沿った分割数
 	glPopMatrix(); // 座標系の保存
}

void CurveSurface (double height, double radius, double theta, double slope, double x, double y, double z) {
  glPushMatrix();
  glTranslatef(x, y, z);
  double deltaTheta = (theta * M_PI / 180 ) / (double)DIVISION;
  glBegin(GL_TRIANGLE_STRIP);
  double initTheta = (90 - theta/2) * M_PI / 180;
  double rx, rz, t;
  
  for (int i = 0; i <= DIVISION; i++) {
    t = initTheta + i * deltaTheta;
    rx = -radius * cos(t);
    rz = radius * sin(t);
    glNormal3d(rx, 0, rz);
    glVertex3d(rx, height, rz);
    glVertex3d(rx*(1+slope), 0, rz*(1+slope));
  }
  glEnd();
  glPopMatrix();
}

void FaceShield (double x, double y, double z) {
  glPushMatrix(); // 座標系の保存
  glTranslatef(x, y, z); // 座標変換
  double theta = 80;
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, BLUE);
  CurveSurface(0.1, 2.5, 80, 0.1, 0, 0.9, 0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, GLASS);
  CurveSurface(0.9, 2.5, 80, 0, 0, 0, 0);
  glPopMatrix();
}

void Body (double height, double radius) {
  // 頭部
  glPushMatrix(); // 座標系の保存
  glTranslatef(0, height, 0); // 座標変換

  Sphere(radius, 0, 0, 0);//半径, Z軸まわりの分割数, Z軸に沿った分割数

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, WHITE);
  // 頭と体の区切り
  Cylinder(height*0.1, radius, 0, -height/10, 0); 
  // 右目
  Sphere(radius*0.1, -radius*0.4, radius*0.5, radius*0.7);//半径, Z軸まわりの分割数, Z軸に沿った分割数
  // 左目
  Sphere(radius*0.1, radius*0.4, radius*0.5, radius*0.7);//半径, Z軸まわりの分割数, Z軸に沿った分割数

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, GREEN);
  // 右触覚
  glPushMatrix();
  glRotatef(30,0,0,1);
  Capsule(radius*0.4, radius*0.05, 0, radius, 0);
  glPopMatrix();
  // 左触覚
  glPushMatrix();
  glRotatef(-30,0,0,1);
  Capsule(radius*0.4, radius*0.05, 0, radius, 0);
  glPopMatrix();
 	glPopMatrix(); // 座標系の保存

  // 体
  Cylinder(height*0.9, radius, 0, 0, 0);
}
// 描画処理を行う
void display (void) {
	int i;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, GREEN);
	Body(3.0,2.0);
  // 右腕
	Capsule(1.4, 0.5, 2.8, 1.0, 0);
  // 左腕
	Capsule(1.4, 0.5, -2.8, 1, 0);
  // 右足
	Capsule(1.4, 0.5, 0.9, -1.1, 0);
  // 左足
	Capsule(1.4, 0.5, -0.9, -1.1, 0);

  // フェイスシールド
  FaceShield(0, 4, 2.3);

	glutSwapBuffers();
}

void init (void) {
	glClearColor(1.0, 1.0, 1.0, 0.0); // 背景色の指定
	glEnable(GL_DEPTH_TEST); // 陰面除去
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	//glCullFace(GL_BACK);
  // 透明化を有効にする
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// 光源の有効化
	glEnable(GL_LIGHTING); // 陰影
	glEnable(GL_LIGHT0); // 光源
	// glEnable(GL_LIGHT1);
	// TODO: 光源未設定
	// TODO: 材質未設定
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 1, 1.0, 100.0); // カメラの視野を設定
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 6.0, 12, 0.0, -2.0, -12.0, 0.0, 1.0, 0.0); // カメラの向き
}

int main (int argc, char *argv[]) {
  glutInitWindowSize(600, 600); // ウィンドウのサイズ指定
	glutInit(&argc, argv); // GLUTおよび OpenGL環境を初期化　
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH); // ディスプレイの表示モードの設定
	glutCreateWindow("1Y18F014 井上智裕 Report"); // ウィンドウを開く
	glutDisplayFunc(display); // 図形表示を行う関数を指定
	init(); // 初期化処理
	glutMainLoop(); // プログラムをイベントの待ち受け状態にするためのループ
	return 0;
}
