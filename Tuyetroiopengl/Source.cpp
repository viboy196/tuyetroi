#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include "tree.h"

#define MAX_TUYET 2000
#define WCX 640
#define WCY 480
#define size 80
float slowdown = 2.0;
float velocity = 0.0;
float gravity = 9.8;
float zoom = -40;
float xoayy = 0.0;
float xoayx = 0;
float xoayz = 0;
//float hailsize
// màu nền
float red = 127.0 / 255;
float green = 182.0 / 255;
float blue = 120.0/255;
float ground_points[size][size][3];
float ground_colors[size][size][4];
float ground_points1[size][size][3];
float ground_colors1[size][size][4];
float accum = -10;

//cây
Tree* tree[5];

// tạo struct tuyết
typedef struct {
	// trạng thái tồn tại
	bool alive; // chỉ hạt tuyết còn tồn tại ko ?
	float live; // thời gian tồn tại
	float fade; // sự hao hụt thời gian
	// màu sắc
	float red;
	float green;
	float blue;
	// tọa độ , vị trí
	float x;
	float y;
	float z;
	//vận tốc
	float velocity;
}tuyet;
typedef struct 
{
	float v_x;
	float v_z;

}gio;
gio gio1;
// mảng tuyết
tuyet arr_tuyet[MAX_TUYET];
int mau = 1;
//chọn màu bông tuyết mặc định trắng
void chonmau(int i)
{
	if (mau == -1) {
		arr_tuyet[i].red = (rand() % 100) / 100.0f;
		arr_tuyet[i].green = (rand() % 100) / 100.0f;
		arr_tuyet[i].blue = (rand() % 100) / 100.0f;
	}
	else
	{
		arr_tuyet[i].red = 1.0;
		arr_tuyet[i].green = 1.0;
		arr_tuyet[i].blue = 1.0;
		
	} 
}
// khởi tạo bông tuyết
void khoitaobongtuyet( int i){
	// trạng thái
	arr_tuyet[i].alive = true;
	arr_tuyet[i].live = 1.0;
	arr_tuyet[i].fade = float(rand() % 100) / 1000.0f + 0.002f;

	// tọa độ
	arr_tuyet[i].x = (float)(rand() % size) - size/2;
	arr_tuyet[i].y = 30.0;
	arr_tuyet[i].z = (float)(rand() % size) - size/2;
	// màu
	chonmau(i);
	/*arr_tuyet[i].red = (rand() % 100)/100.0f;
	arr_tuyet[i].green = (rand() % 100) / 100.0f;
	arr_tuyet[i].blue = (rand() % 100) / 100.0f;*/
	// vận tốc
	arr_tuyet[i].velocity = velocity;
}

// khởi tạo nền và các bông tuyết
void initCay();
void Init(){
	glShadeModel(GL_SMOOTH);
	glClearColor((float)(0.5), (float)(0.5), (float)(0.5),0.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	gio1.v_x = 0;
	gio1.v_z = 0;
	// khởi tạo vị trí và màu nền của nền
	for (int  i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{	// tọa độ
			ground_points[i][j][0] = i - size / 2;
			ground_points[i][j][1] = accum;
			ground_points[i][j][2] = j - size / 2;

			// màu
			ground_colors[i][j][0] = red; // value đỏ
			ground_colors[i][j][1] = green; // value xanh lá cây
			ground_colors[i][j][2] = blue; // value xanh nước biển
			ground_colors[i][j][3] = 0.0; // acummulation factor
			// tọa độ
			ground_points1[i][j][0] = i - size / 2;
			ground_points1[i][j][1] = accum;
			ground_points1[i][j][2] = j - size / 2;

		}
	}

	for (int i = 0; i < MAX_TUYET; i++)
	{
		khoitaobongtuyet(i);
	}
	initCay();
}
int tt = 1;
//vẽ bông tuyết
void vebongtuyet(int i)
{
	
		
			// vẽ hát tuyết hình tròn k
			glColor3f(arr_tuyet[i].red, arr_tuyet[i].green, arr_tuyet[i].blue);
			glPushMatrix();
			glTranslated(arr_tuyet[i].x, arr_tuyet[i].y, arr_tuyet[i].z);
			glutSolidSphere(0.1, 10, 10);
			glPopMatrix();
		
	
}
// cập nhật tọa độ bông tuyết thứ i 
void locxoay(int i); 
void capnhatvitri(int i){
		
	int x = (int)(arr_tuyet[i].x);
	int y = (int)(arr_tuyet[i].z);
		arr_tuyet[i].z += gio1.v_z / 10;
		arr_tuyet[i].x += gio1.v_x /10;
		arr_tuyet[i].y += arr_tuyet[i].velocity / (slowdown * 1000);

		arr_tuyet[i].velocity -= gravity / 10;
		//tg sống
		arr_tuyet[i].live -= arr_tuyet[i].fade;
		if (	arr_tuyet[i].y <= accum &&
				(int)(arr_tuyet[i].x) + size / 2 >= 0 && 
				(int)(arr_tuyet[i].x) + size / 2 <= size  && 
				(int)(arr_tuyet[i].z) + size / 2 >= 0 && 
				(int)(arr_tuyet[i].z) + size / 2 < size) 
		{
			ground_colors[(int)(arr_tuyet[i].x)+ size / 2][(int)(arr_tuyet[i].z) + size / 2][0] = arr_tuyet[i].red;
			ground_colors[(int)(arr_tuyet[i].x) + size / 2][(int)(arr_tuyet[i].z) + size / 2][1] = arr_tuyet[i].green;
			ground_colors[(int)(arr_tuyet[i].x) + size / 2][(int)(arr_tuyet[i].z) + size / 2][2] = arr_tuyet[i].blue;
			ground_colors[(int)(arr_tuyet[i].x) + size / 2][(int)(arr_tuyet[i].z) + size / 2][3] += 1.0;

			if(ground_colors[(int)(arr_tuyet[i].x) + size/2][(int)(arr_tuyet[i].z) + size/2][3] >1.0)
				ground_points[(int)(arr_tuyet[i].x)+ size / 2][(int)(arr_tuyet[i].z) + size / 2][1] += 0.1;
			arr_tuyet[i].live = -1.0;
		}
		//
		if (arr_tuyet[i].live < 0.0)
			khoitaobongtuyet(i);
		//
		if (arr_tuyet[i].y < accum)
			khoitaobongtuyet;
		//
		if (tt == -1) {
			locxoay(i);
		}

}
void locxoay(int i){
	float r = sqrtf((arr_tuyet[i].x - ground_points[size / 2][size / 3][0]) *
					(arr_tuyet[i].x - ground_points[size / 2][size / 2][0]) +
					(arr_tuyet[i].z - ground_points[size / 2][size / 2][2]) *
					(arr_tuyet[i].z - ground_points[size / 2][size / 2][2]));
	
	float x = ground_points[size / 2][size / 2][0] + (arr_tuyet[i].x - ground_points[size / 2][size / 2][0]) * cosf(1.0f / (5*r)) - (arr_tuyet[i].z - ground_points[size / 2][size / 2][2]) * sinf(1.0f / (10*r));
	float z = ground_points[size / 2][size / 2][2] + (arr_tuyet[i].x - ground_points[size / 2][size / 2][0]) * sinf(1.0f / (5*r)) + (arr_tuyet[i].z - ground_points[size / 2][size / 2][2]) * cosf(1.0f / (10*r));
	x += (ground_points[size / 2][size / 2][0] - x) / 2500 * r;
	z += (ground_points[size / 2][size / 2][2] - z) / 2500 * r;
	arr_tuyet[i].x = x;
	arr_tuyet[i].z = z;
}
//vẽ tuyết rơi + cập nhật vị trí 
void vemantuyet(){
	for (int i = 0; i < MAX_TUYET; i++) {
		vebongtuyet(i);
		capnhatvitri(i);
	}
}

void renderCay(void) {
	for (int i = 0; i < 5; i++)
		tree[i]->render();
}

void initCay() {
	tree[0] = new Tree(0.0f, accum, 0.0f, 2.5f, 0.25f, 1.0f);
	tree[0]->buildTree();

	tree[1] = new Tree(-30.0f, accum, 15.0f, 1.8f, 0.2f, 0.8f);
	tree[1]->setTreeBranch(3);
	tree[1]->setLeafStyle(1);
	tree[1]->setRange(30.0f);
	tree[1]->setTrunkColor(0.27f, 0.01f, 0.6f);
	tree[1]->setLeafColor(1.0f, 0.17f, 0.9f);
	tree[1]->enableRandomColor(false);
	tree[1]->enableRandomAngle(false);
	tree[1]->buildTree();

	tree[2] = new Tree(30.0f, accum, 15.0f, 2.0f, 0.2f, 0.8f);
	tree[2]->setTreeBranch(3);
	tree[2]->setLeafStyle(2);
	tree[2]->setRange(30.0f);
	tree[2]->enableRandomColor(true);
	tree[2]->buildTree();

	tree[3] = new Tree(-30.0f, accum, -15.0f, 2.0f, 0.2f, 0.8f);
	tree[3]->setTreeBranch(3);
	tree[3]->setLeafStyle(2);
	tree[3]->setRange(25.0f);
	tree[3]->enableRandomColor(false);
	tree[3]->buildTree();

	tree[4] = new Tree(30.0f, accum, -15.0f, 2.2f, 0.3f, 0.8f);
	tree[4]->setTreeBranch(3);
	tree[4]->setLeafStyle(1);
	tree[4]->setRange(30.0f);
	tree[4]->setTrunkColor(0.0f, 0.6f, 0.7f);
	tree[4]->setLeafColor(1.0f, 0.25f, 0.25f);
	tree[4]->enableRandomColor(false);
	tree[4]->buildTree();
}

// vẽ nền
void venen(){
	glColor3f(red, green, blue);
	glBegin(GL_QUADS);
	for (int i = 0; i < size -1; i++)
	{
		for (int j = 0; j < size -1; j++)
		{
			glColor3fv(ground_colors[i][j]);
			glVertex3f(	ground_points[i][j][0],
						ground_points[i][j][1], 
						ground_points[i][j][2]);
			glColor3fv(ground_colors[i][j+1]);
			glVertex3f(	ground_points[i][j+1][0], 
						ground_points[i][j+1][1], 
						ground_points[i][j+1][2]);
			glColor3fv(ground_colors[i+1][j+1]);
			glVertex3f(	ground_points[i+1][j + 1][0], 
						ground_points[i+1][j + 1][1], 
						ground_points[i+1][j + 1][2]);
			glColor3fv(ground_colors[i+1][j]);
			glVertex3f(	ground_points[i+1][j][0], 
						ground_points[i+1][j][1], 
						ground_points[i+1][j][2]);

		}
	}
	glEnd();
	

}
//
void PhimDieuKhien(int key, int x, int y) {
	if (key == GLUT_KEY_UP) {
		zoom += 1.0;
	}
	if (key == GLUT_KEY_DOWN) {
		zoom -= 1.0;
	}
	if (key == GLUT_KEY_RIGHT) {
		xoayy += 1.0;
	}
	if (key == GLUT_KEY_LEFT) {
		xoayy -= 1.0;
	}
	
	
}

void PhimCoBan(unsigned char key, int x, int y) {
	if (key == ',') { // really '<' slow down
		 slowdown -= 0.01;
	}
	if (key == '.') { // really '>' speed up
		 slowdown += 0.01;
	}
	if (key == 'd' || key == 'D') { // really '<' slow down
		gio1.v_x += 0.01;
	}
	if (key == 'a' || key == 'A') { // really '>' speed up
		gio1.v_x -= 0.01;
	}
	if (key == 'w' || key == 'W') { // really '<' slow down
		gio1.v_z -= 0.01;
	}
	if (key == 'z' || key == 'Z') { // really '>' speed up
		gio1.v_z += 0.01;
	}
	if (key == 'q') { // QUIT
		exit(0);
	}
	if (key == 'o' || key == 'O') {
		xoayx += 1.0;
	}
	if (key == 'l' || key == 'L') {
		xoayx -= 1.0;
	}
	if (key == 'i' || key == 'I') {
		xoayz += 1.0;
	}
	if (key == 'k' || key == 'K') {
		xoayz -= 1.0;
	}
	if (key == 'j' || key == 'J') {
		tt *= -1;
	}
	if (key == 'c' || key == 'C')
	{
		mau *= -1;
	}
}

// vẽ toàn cảnh
void vetoancanh(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	
	glLoadIdentity();
	glTranslatef(0.0, 0.0, zoom);
	
	glRotated(xoayy, 0.0, 1, 0.0);
	glRotated(xoayx, 1.0, 0, 0);
	glRotated(xoayz, 0, 0, 1.0);
	//glRotated(70, 0.0, -1, 0.0);
	venen();
	renderCay();
	vemantuyet();
	glPopMatrix();
	glutSwapBuffers();
}
// góc nhìn
void reshape( int w , int h){
	if (h == 0) h = 1;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(90, (float)w / (float)h, 0.3, 200);
	//glViewport(0, 0, w, h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void idle() {
	glutPostRedisplay();
}
int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WCX, WCY);
	glutCreateWindow("tuyet roi day troi");
	Init();
	glutDisplayFunc(vetoancanh);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutKeyboardFunc(PhimCoBan);
	glutSpecialFunc(PhimDieuKhien);
	glutMainLoop();

	return 0;
}