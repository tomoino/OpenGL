#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <string.h>

GLfloat GREEN[] = {0.6, 0.9, 0.4, 1.0};		//緑色
GLfloat WHITE[] = {1.0, 1.0, 1.0, 1.0};		//緑色

// TODO: glVertex3dv
void Cylinder(double height, double radius) {
  int division = 100; // 分割数。多いほど曲面に近くなる。
  double step = 2.0 * M_PI / (double)division; // 側面1辺あたりの中心角
  int i;
  double theta = 0;
  /* 上面  */
  glNormal3d(0.0, 1.0, 0.0);
  glBegin(GL_POLYGON);
  for (i = 0; i < division; i++) {
    glVertex3d(radius * sin(theta), height, radius * cos(theta));
    theta += step;
  }
  glEnd();

  /* 下面 */
  glNormal3d(0.0, -1.0, 0.0);
  glBegin(GL_POLYGON);
  theta = step * division;
  for (i = division; --i >= 0; ) {
    glVertex3d(radius * sin(theta), 0.0, radius * cos(theta));
    theta -= step;
  }
  glEnd();

  /* 側面 */
  glBegin(GL_QUAD_STRIP);
  theta = 0;
  for (i = 0; i <= division; i++) {
    double x = sin(theta);
    double z = cos(theta);

    glNormal3d(x, 0.0, z);
    glVertex3f(radius * x, height, radius * z);
    glVertex3f(radius * x, 0.0, radius * z);
    theta += step;
  }
  glEnd();
}

void Capsule (double height, double radius) {
  int division = 100; // 分割数。多いほど曲面に近くなる。
  glPushMatrix(); // 座標系の保存
  glTranslatef(0, height, 0); // 座標変換
  glutSolidSphere(radius, division, division);//半径, Z軸まわりの分割数, Z軸に沿った分割数
 	glPopMatrix(); // 座標系の保存
  Cylinder(height, radius);
  glutSolidSphere(radius, division, division);//半径, Z軸まわりの分割数, Z軸に沿った分割数
}

void Body (double height, double radius) {
  int division = 100; // 分割数。多いほど曲面に近くなる。
  // 頭部
  glPushMatrix(); // 座標系の保存
  glTranslatef(0, height, 0); // 座標変換
  glutSolidSphere(radius, division, division);//半径, Z軸まわりの分割数, Z軸に沿った分割数
  glPushMatrix();
  glTranslatef(0, -height/10, 0); // 座標変換
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, WHITE);
  Cylinder(height*0.1, radius); // 頭と体の区切り
  glPopMatrix();
  // 右目
  glPushMatrix();
  glTranslatef(-radius*0.4, radius*0.5, radius*0.7); // 座標変換
  glutSolidSphere(radius*0.1, division, division);//半径, Z軸まわりの分割数, Z軸に沿った分割数
  glPopMatrix();
  // 左目
  glPushMatrix();
  glTranslatef(radius*0.4, radius*0.5, radius*0.7); // 座標変換
  glutSolidSphere(radius*0.1, division, division);//半径, Z軸まわりの分割数, Z軸に沿った分割数
  glPopMatrix();
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, GREEN);
  // 右触覚
  glPushMatrix();
  glRotatef(30,0,0,1);
  glTranslatef(0, radius, 0); // 座標変換
  Capsule(radius*0.4, radius*0.05);
  glPopMatrix();
  // 左触覚
  glPushMatrix();
  glRotatef(-30,0,0,1);
  glTranslatef(0, radius, 0); // 座標変換
  Capsule(radius*0.4, radius*0.05);
  glPopMatrix();
 	glPopMatrix(); // 座標系の保存


  
  // 体
  Cylinder(height*0.9, radius);
}
// 描画処理を行う
void display (void) {
	int i;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
	glPushMatrix(); // 座標系の保存


  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, GREEN);
	Body(3.0,2.0);
  // 右腕
  glPushMatrix(); // 座標系の保存
	glTranslatef(2.8, 1, 0); // 座標変換
	Capsule(1.4,0.5);
	glPopMatrix(); // 座標系の復元
  // 左腕
  glPushMatrix(); // 座標系の保存
	glTranslatef(-2.8, 1, 0); // 座標変換
	// Capsule(1.4,0.5);
	glPopMatrix(); // 座標系の復元
  // 右足
  glPushMatrix(); // 座標系の保存
	glTranslatef(0.9, -1.1, 0); // 座標変換
	Capsule(1.4,0.5);
	glPopMatrix(); // 座標系の復元
  // 左足
  glPushMatrix(); // 座標系の保存
	glTranslatef(-0.9, -1.1, 0); // 座標変換
	Capsule(1.4,0.5);
	glPopMatrix(); // 座標系の復元

	glPopMatrix(); // 座標系の復元

	glutSwapBuffers();
}

void init (void) {
	glClearColor(1.0, 1.0, 1.0, 0.0); // 背景色の指定
	glEnable(GL_DEPTH_TEST); // 陰面除去
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	//glCullFace(GL_BACK);

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
