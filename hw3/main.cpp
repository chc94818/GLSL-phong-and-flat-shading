#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> /* offsetof */
#include "../GL/glew.h"
#include "../GL/glut.h"
#include "../shader_lib/shader.h"
#include <iostream>
#include "glm/glm.h"
extern "C"
{
	#include "glm_helper.h"
}
using namespace std;
void init(void);
void display(void);
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void idle(void);

GLMmodel *XiaoPingGuo;
GLfloat light_pos[] = { 10.0, 10.0, 0.0 };
float eyex = 0.0;
float eyey = 0.0;

GLuint program_apple,program_stem;
GLuint vboApppleVertex,vboAppleTexVertex, vboApppleNorVertex, vboStemVertex, vboStemTexVertex, vboStemNorVertex;

GLfloat mtx[16], mtx2[16], mtx3[16], mtx4[16];
GLuint tex, tex1;
GLint loc, loc1, loc2, loc3, loc4, loc5;

bool shadingType = false;

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("OpenGL HW3 - student_id");
	glutReshapeWindow(512, 512);

	glewInit();

	init();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	glmDelete(XiaoPingGuo);
	return 0;
}

//phong shading
void phongShading() {

	//載入shader-------------------------------------------------------------------------------------
	GLuint appleShader_v = createShader("Shaders/phong_apple_vert.vert", "vertex");
	GLuint appleShader_f = createShader("Shaders/phong_apple_frag.frag", "fragment");
	program_apple = createProgram(appleShader_v, appleShader_f);

	GLuint stemShader_v = createShader("Shaders/phong_stem_vert.vert", "vertex");
	GLuint stemShader_f = createShader("Shaders/phong_stem_frag.frag", "fragment");
	program_stem = createProgram(stemShader_v, stemShader_f);

	//----------------------------------------------------------------------------------------------

	//取apple點-------------------------------------------------------------------------------------
	int vertAppleTri;//三角形序號
	int vertAppleSize = XiaoPingGuo->groups->next->numtriangles * 9;//三個三維點 3*3 = 9
	float *vertApple = new float[vertAppleSize];//儲存三角形點位置
	for (int i = 0, step = 0; i < XiaoPingGuo->groups->next->numtriangles; i++)
	{
		vertAppleTri = XiaoPingGuo->groups->next->triangles[i];//取出三角形序號
		//依序取得三個點位置
		for (int j = 0; j < 3; j++)
		{
			vertApple[step++] = XiaoPingGuo->vertices[XiaoPingGuo->triangles[vertAppleTri].vindices[j] * 3];
			vertApple[step++] = XiaoPingGuo->vertices[XiaoPingGuo->triangles[vertAppleTri].vindices[j] * 3 + 1];
			vertApple[step++] = XiaoPingGuo->vertices[XiaoPingGuo->triangles[vertAppleTri].vindices[j] * 3 + 2];
		}
	}
	//VBO 存入 ATTRIBUTE
	glGenBuffers(1, &vboApppleVertex);
	glBindBuffer(GL_ARRAY_BUFFER, vboApppleVertex);
	glBufferData(GL_ARRAY_BUFFER, vertAppleSize * sizeof(float), vertApple, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);//存到第0個位置
	glVertexAttribPointer(0/*存到第0個位置*/, 3, GL_FLOAT, GL_FALSE, 0, 0); // stride 0 for tightly packed
	glBindBuffer(GL_ARRAY_BUFFER, 1);
	//-------------------------------------------------------------------------------------
	//取apple貼圖點-------------------------------------------------------------------------------------
	int texAppleTri;//三角形序號
	int texAppleSize = XiaoPingGuo->groups->next->numtriangles * 6;//兩個三維點 2*3 = 6
	float *texApple = new float[texAppleSize];//儲存三角形點位置
	for (int i = 0, step = 0; i < XiaoPingGuo->groups->next->numtriangles; i++)
	{
		texAppleTri = XiaoPingGuo->groups->next->triangles[i];//取出三角形序號
		//依序取得兩個點位置
		for (int j = 0; j < 3; j++)
		{
			texApple[step++] = XiaoPingGuo->texcoords[XiaoPingGuo->triangles[texAppleTri].tindices[j] * 2];
			texApple[step++] = XiaoPingGuo->texcoords[XiaoPingGuo->triangles[texAppleTri].tindices[j] * 2 + 1];
		}
	}
	//VBO 存入 ATTRIBUTE
	glGenBuffers(1, &vboAppleTexVertex);
	glBindBuffer(GL_ARRAY_BUFFER, vboAppleTexVertex);
	glBufferData(GL_ARRAY_BUFFER, texAppleSize * sizeof(float), texApple, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);//存到第1個位置
	glVertexAttribPointer(1/*存到第1個位置*/, 2, GL_FLOAT, GL_FALSE, 0, 0); // stride 0 for tightly packed
	glBindBuffer(GL_ARRAY_BUFFER, 1);

	//-------------------------------------------------------------------------------------
	//取apple stem點-------------------------------------------------------------------------------------
	int vertStemTri;//三角形序號
	int vertStemSize = XiaoPingGuo->groups->numtriangles * 9;//三個三維點 3*3 = 9
	float *vertStem = new float[vertStemSize];//儲存三角形點位置
	for (int i = 0, step = 0; i < XiaoPingGuo->groups->numtriangles; i++)
	{
		vertStemTri = XiaoPingGuo->groups->triangles[i];//取出三角形序號
		//依序取得三個點位置
		for (int j = 0; j < 3; j++)
		{
			vertStem[step++] = XiaoPingGuo->vertices[XiaoPingGuo->triangles[vertStemTri].vindices[j] * 3];
			vertStem[step++] = XiaoPingGuo->vertices[XiaoPingGuo->triangles[vertStemTri].vindices[j] * 3 + 1];
			vertStem[step++] = XiaoPingGuo->vertices[XiaoPingGuo->triangles[vertStemTri].vindices[j] * 3 + 2];
		}
	}
	//VBO 存入 ATTRIBUTE
	glGenBuffers(1, &vboStemVertex);
	glBindBuffer(GL_ARRAY_BUFFER, vboStemVertex);
	glBufferData(GL_ARRAY_BUFFER, vertStemSize * sizeof(float), vertStem, GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);//存到第3個位置
	glVertexAttribPointer(3/*存到第3個位置*/, 3, GL_FLOAT, GL_FALSE, 0, 0); // stride 0 for tightly packed
	glBindBuffer(GL_ARRAY_BUFFER, 1);
	//-------------------------------------------------------------------------------------
	//取apple stem貼圖點-------------------------------------------------------------------------------------
	int texStemTri;//三角形序號
	int texStemSize = XiaoPingGuo->groups->numtriangles * 6;//兩個三維點 2*3 = 6
	float *texStem = new float[texStemSize];//儲存三角形點位置
	for (int i = 0, step = 0; i < XiaoPingGuo->groups->numtriangles; i++)
	{
		texStemTri = XiaoPingGuo->groups->triangles[i];//取出三角形序號
		//依序取得兩個點位置
		for (int j = 0; j < 3; j++)
		{
			texStem[step++] = XiaoPingGuo->texcoords[XiaoPingGuo->triangles[texStemTri].tindices[j] * 2];
			texStem[step++] = XiaoPingGuo->texcoords[XiaoPingGuo->triangles[texStemTri].tindices[j] * 2 + 1];
		}
	}
	//VBO 存入 ATTRIBUTE
	glGenBuffers(1, &vboStemTexVertex);
	glBindBuffer(GL_ARRAY_BUFFER, vboStemTexVertex);
	glBufferData(GL_ARRAY_BUFFER, texStemSize * sizeof(float), texStem, GL_STATIC_DRAW);
	glEnableVertexAttribArray(4);//存到第4個位置
	glVertexAttribPointer(4/*存到第4個位置*/, 2, GL_FLOAT, GL_FALSE, 0, 0); // stride 0 for tightly packed
	glBindBuffer(GL_ARRAY_BUFFER, 1);
	//-------------------------------------------------------------------------------------

	//取apple點法向量
	int norAppleTri;//三角形序號
	int norAppleSize = XiaoPingGuo->groups->next->numtriangles * 9;//三個三維點 3*3 = 9
	float *norApple = new float[norAppleSize];//儲存三角形點位置
	for (int i = 0,step = 0; i < XiaoPingGuo->groups->next->numtriangles; i++)
	{
		norAppleTri = XiaoPingGuo->groups->next->triangles[i];//取出三角形序號
		//依序取得三個點位置
		for (int j = 0; j < 3; j++)
		{
			norApple[step++] = XiaoPingGuo->normals[XiaoPingGuo->triangles[norAppleTri].nindices[j] * 3];
			norApple[step++] = XiaoPingGuo->normals[XiaoPingGuo->triangles[norAppleTri].nindices[j] * 3 + 1];
			norApple[step++] = XiaoPingGuo->normals[XiaoPingGuo->triangles[norAppleTri].nindices[j] * 3 + 2];
			
		}
	}
	//VBO 存入 ATTRIBUTE
	glGenBuffers(1, &vboApppleNorVertex);
	glBindBuffer(GL_ARRAY_BUFFER, vboApppleNorVertex);
	glBufferData(GL_ARRAY_BUFFER, norAppleSize * sizeof(float), norApple, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);//存到第2個位置
	glVertexAttribPointer(2/*存到第2個位置*/, 3, GL_FLOAT, GL_FALSE, 0, 0); // stride 0 for tightly packed
	glBindBuffer(GL_ARRAY_BUFFER, 1);

	//取apple stem點法向量
	int norStemTri;//三角形序號
	int norStemSize = XiaoPingGuo->groups->numtriangles * 9;//三個三維點 3*3 = 9
	float *norStem = new float[norStemSize];//儲存三角形點位置
	for (int i = 0, step = 0; i < XiaoPingGuo->groups->numtriangles; i++)
	{
		norStemTri = XiaoPingGuo->groups->triangles[i];//取出三角形序號
		//依序取得三個點位置
		for (int j = 0; j < 3; j++)
		{
			norStem[step++] = XiaoPingGuo->vertices[XiaoPingGuo->triangles[norStemTri].vindices[j] * 3];
			norStem[step++] = XiaoPingGuo->vertices[XiaoPingGuo->triangles[norStemTri].vindices[j] * 3 + 1];
			norStem[step++] = XiaoPingGuo->vertices[XiaoPingGuo->triangles[norStemTri].vindices[j] * 3 + 2];
			
		}
	}
	//VBO 存入 ATTRIBUTE
	glGenBuffers(1, &vboStemNorVertex);
	glBindBuffer(GL_ARRAY_BUFFER, vboStemNorVertex);
	glBufferData(GL_ARRAY_BUFFER, norStemSize * sizeof(float), norStem, GL_STATIC_DRAW);
	glEnableVertexAttribArray(5);//存到第5個位置
	glVertexAttribPointer(5/*存到第5個位置*/, 3, GL_FLOAT, GL_FALSE, 0, 0); // stride 0 for tightly packed
	glBindBuffer(GL_ARRAY_BUFFER, 1);


}


//flat shading
void flatShading() {

	//載入shader-------------------------------------------------------------------------------------
	GLuint appleShader_v = createShader("Shaders/flat_apple_vert.vert", "vertex");
	GLuint appleShader_f = createShader("Shaders/flat_apple_frag.frag", "fragment");
	program_apple = createProgram(appleShader_v, appleShader_f);

	GLuint stemShader_v = createShader("Shaders/flat_stem_vert.vert", "vertex");
	GLuint stemShader_f = createShader("Shaders/flat_stem_frag.frag", "fragment");
	program_stem = createProgram(stemShader_v, stemShader_f);

	//----------------------------------------------------------------------------------------------

	//取apple點-------------------------------------------------------------------------------------
	int vertAppleTri;//三角形序號
	int vertAppleSize = XiaoPingGuo->groups->next->numtriangles * 9;//三個三維點 3*3 = 9
	float *vertApple = new float[vertAppleSize];//儲存三角形點位置
	for (int i = 0, step = 0; i < XiaoPingGuo->groups->next->numtriangles; i++)
	{
		vertAppleTri = XiaoPingGuo->groups->next->triangles[i];//取出三角形序號
															   //依序取得三個點位置
		for (int j = 0; j < 3; j++)
		{
			vertApple[step++] = XiaoPingGuo->vertices[XiaoPingGuo->triangles[vertAppleTri].vindices[j] * 3];
			vertApple[step++] = XiaoPingGuo->vertices[XiaoPingGuo->triangles[vertAppleTri].vindices[j] * 3 + 1];
			vertApple[step++] = XiaoPingGuo->vertices[XiaoPingGuo->triangles[vertAppleTri].vindices[j] * 3 + 2];
		}
	}
	//VBO 存入 ATTRIBUTE
	glGenBuffers(1, &vboApppleVertex);
	glBindBuffer(GL_ARRAY_BUFFER, vboApppleVertex);
	glBufferData(GL_ARRAY_BUFFER, vertAppleSize * sizeof(float), vertApple, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);//存到第0個位置
	glVertexAttribPointer(0/*存到第0個位置*/, 3, GL_FLOAT, GL_FALSE, 0, 0); // stride 0 for tightly packed
	glBindBuffer(GL_ARRAY_BUFFER, 1);
	//-------------------------------------------------------------------------------------
	//取apple貼圖點-------------------------------------------------------------------------------------
	int texAppleTri;//三角形序號
	int texAppleSize = XiaoPingGuo->groups->next->numtriangles * 6;//兩個三維點 2*3 = 6
	float *texApple = new float[texAppleSize];//儲存三角形點位置
	for (int i = 0, step = 0; i < XiaoPingGuo->groups->next->numtriangles; i++)
	{
		texAppleTri = XiaoPingGuo->groups->next->triangles[i];//取出三角形序號
															  //依序取得兩個點位置
		for (int j = 0; j < 3; j++)
		{
			texApple[step++] = XiaoPingGuo->texcoords[XiaoPingGuo->triangles[texAppleTri].tindices[j] * 2];
			texApple[step++] = XiaoPingGuo->texcoords[XiaoPingGuo->triangles[texAppleTri].tindices[j] * 2 + 1];
		}
	}
	//VBO 存入 ATTRIBUTE
	glGenBuffers(1, &vboAppleTexVertex);
	glBindBuffer(GL_ARRAY_BUFFER, vboAppleTexVertex);
	glBufferData(GL_ARRAY_BUFFER, texAppleSize * sizeof(float), texApple, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);//存到第1個位置
	glVertexAttribPointer(1/*存到第1個位置*/, 2, GL_FLOAT, GL_FALSE, 0, 0); // stride 0 for tightly packed
	glBindBuffer(GL_ARRAY_BUFFER, 1);

	//-------------------------------------------------------------------------------------
	//取apple stem點-------------------------------------------------------------------------------------
	int vertStemTri;//三角形序號
	int vertStemSize = XiaoPingGuo->groups->numtriangles * 9;//三個三維點 3*3 = 9
	float *vertStem = new float[vertStemSize];//儲存三角形點位置
	for (int i = 0, step = 0; i < XiaoPingGuo->groups->numtriangles; i++)
	{
		vertStemTri = XiaoPingGuo->groups->triangles[i];//取出三角形序號
														//依序取得三個點位置
		for (int j = 0; j < 3; j++)
		{
			vertStem[step++] = XiaoPingGuo->vertices[XiaoPingGuo->triangles[vertStemTri].vindices[j] * 3];
			vertStem[step++] = XiaoPingGuo->vertices[XiaoPingGuo->triangles[vertStemTri].vindices[j] * 3 + 1];
			vertStem[step++] = XiaoPingGuo->vertices[XiaoPingGuo->triangles[vertStemTri].vindices[j] * 3 + 2];
		}
	}
	//VBO 存入 ATTRIBUTE
	glGenBuffers(1, &vboStemVertex);
	glBindBuffer(GL_ARRAY_BUFFER, vboStemVertex);
	glBufferData(GL_ARRAY_BUFFER, vertStemSize * sizeof(float), vertStem, GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);//存到第3個位置
	glVertexAttribPointer(3/*存到第3個位置*/, 3, GL_FLOAT, GL_FALSE, 0, 0); // stride 0 for tightly packed
	glBindBuffer(GL_ARRAY_BUFFER, 1);
	//-------------------------------------------------------------------------------------
	//取apple stem貼圖點-------------------------------------------------------------------------------------
	int texStemTri;//三角形序號
	int texStemSize = XiaoPingGuo->groups->numtriangles * 6;//兩個三維點 2*3 = 6
	float *texStem = new float[texStemSize];//儲存三角形點位置
	for (int i = 0, step = 0; i < XiaoPingGuo->groups->numtriangles; i++)
	{
		texStemTri = XiaoPingGuo->groups->triangles[i];//取出三角形序號
													   //依序取得兩個點位置
		for (int j = 0; j < 3; j++)
		{
			texStem[step++] = XiaoPingGuo->texcoords[XiaoPingGuo->triangles[texStemTri].tindices[j] * 2];
			texStem[step++] = XiaoPingGuo->texcoords[XiaoPingGuo->triangles[texStemTri].tindices[j] * 2 + 1];
		}
	}
	//VBO 存入 ATTRIBUTE
	glGenBuffers(1, &vboStemTexVertex);
	glBindBuffer(GL_ARRAY_BUFFER, vboStemTexVertex);
	glBufferData(GL_ARRAY_BUFFER, texStemSize * sizeof(float), texStem, GL_STATIC_DRAW);
	glEnableVertexAttribArray(4);//存到第4個位置
	glVertexAttribPointer(4/*存到第4個位置*/, 2, GL_FLOAT, GL_FALSE, 0, 0); // stride 0 for tightly packed
	glBindBuffer(GL_ARRAY_BUFFER, 1);
	//-------------------------------------------------------------------------------------

	//取apple點法向量
	int norAppleTri;//三角形序號
	int norAppleSize = XiaoPingGuo->groups->next->numtriangles * 9;//三個三維點 3*3 = 9
	float *norApple = new float[norAppleSize];//儲存三角形點位置
	for (int i = 0, step = 0; i < XiaoPingGuo->groups->next->numtriangles; i++)
	{
		norAppleTri = XiaoPingGuo->groups->next->triangles[i];//取出三角形序號
															  //依序取得三個點位置
		for (int j = 0; j < 3; j++)
		{
			norApple[step++] = XiaoPingGuo->normals[XiaoPingGuo->triangles[norAppleTri].nindices[j] * 3];
			norApple[step++] = XiaoPingGuo->normals[XiaoPingGuo->triangles[norAppleTri].nindices[j] * 3 + 1];
			norApple[step++] = XiaoPingGuo->normals[XiaoPingGuo->triangles[norAppleTri].nindices[j] * 3 + 2];

		}
	}
	//VBO 存入 ATTRIBUTE
	glGenBuffers(1, &vboApppleNorVertex);
	glBindBuffer(GL_ARRAY_BUFFER, vboApppleNorVertex);
	glBufferData(GL_ARRAY_BUFFER, norAppleSize * sizeof(float), norApple, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);//存到第2個位置
	glVertexAttribPointer(2/*存到第2個位置*/, 3, GL_FLOAT, GL_FALSE, 0, 0); // stride 0 for tightly packed
	glBindBuffer(GL_ARRAY_BUFFER, 1);

	//取apple stem點法向量
	int norStemTri;//三角形序號
	int norStemSize = XiaoPingGuo->groups->numtriangles * 9;//三個三維點 3*3 = 9
	float *norStem = new float[norStemSize];//儲存三角形點位置
	for (int i = 0, step = 0; i < XiaoPingGuo->groups->numtriangles; i++)
	{
		norStemTri = XiaoPingGuo->groups->triangles[i];//取出三角形序號
													   //依序取得三個點位置
		for (int j = 0; j < 3; j++)
		{
			norStem[step++] = XiaoPingGuo->vertices[XiaoPingGuo->triangles[norStemTri].vindices[j] * 3];
			norStem[step++] = XiaoPingGuo->vertices[XiaoPingGuo->triangles[norStemTri].vindices[j] * 3 + 1];
			norStem[step++] = XiaoPingGuo->vertices[XiaoPingGuo->triangles[norStemTri].vindices[j] * 3 + 2];

		}
	}
	//VBO 存入 ATTRIBUTE
	glGenBuffers(1, &vboStemNorVertex);
	glBindBuffer(GL_ARRAY_BUFFER, vboStemNorVertex);
	glBufferData(GL_ARRAY_BUFFER, norStemSize * sizeof(float), norStem, GL_STATIC_DRAW);
	glEnableVertexAttribArray(5);//存到第5個位置
	glVertexAttribPointer(5/*存到第5個位置*/, 3, GL_FLOAT, GL_FALSE, 0, 0); // stride 0 for tightly packed
	glBindBuffer(GL_ARRAY_BUFFER, 1);


}

void init(void) {

	XiaoPingGuo = glmReadOBJ("Model/apple.obj");
	glmUnitize(XiaoPingGuo);
	glmFacetNormals(XiaoPingGuo);
	glmVertexNormals(XiaoPingGuo, 90.0, GL_FALSE);
	print_model_info(XiaoPingGuo);
	
	if (shadingType == true)
	{
		phongShading();
	}
	else
	{
		flatShading();
	}
}

void display(void)
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 1.0, 1e-2, 1e2);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyex, eyey, 3.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);
	//glmDraw(XiaoPingGuo, GLM_TEXTURE); /* Make sure glmLoadOBJ work. If it works, remove this line. */

	//載入apple shader-------------------------------------------------------------------------------------
	//註冊投影陣列
	glGetFloatv(GL_PROJECTION_MATRIX, mtx);
	loc = glGetUniformLocation(program_apple, "Projection");

	//註冊modelview陣列
	glGetFloatv(GL_MODELVIEW_MATRIX, mtx2);
	loc1 = glGetUniformLocation(program_apple, "MV");

	//取得texture id，註冊texture
	tex = XiaoPingGuo->textures[0].id;
	glEnable(GL_TEXTURE_2D);
	loc2 = glGetUniformLocation(program_apple, "MyTexture1");

	//註冊光線，眼睛，ambient，specular，shininess
	GLint l, n, v, k_a, k_s, a;
	l = glGetUniformLocation(program_apple, "light");
	v = glGetUniformLocation(program_apple, "vector");
	k_a = glGetUniformLocation(program_apple, "ka");
	k_s = glGetUniformLocation(program_apple, "ks");
	a = glGetUniformLocation(program_apple, "alpha");

	glUseProgram(program_apple);

	//對應投影矩陣與modelview矩陣
	glUniformMatrix4fv(loc, 1, GL_FALSE, mtx);
	glUniformMatrix4fv(loc1, 1, GL_FALSE, mtx2);

	if (shadingType == true)
	{	
		// texture index為2
		// 對應光線，眼睛，ambient，specular，shininess
		// phong shading 模式		
		glUniform3f(l, light_pos[0], light_pos[1], light_pos[2]);
		glUniform3f(v, eyex, eyey, 3.0);
		glUniform4f(k_a, XiaoPingGuo->materials[2].ambient[0], XiaoPingGuo->materials[2].ambient[1], XiaoPingGuo->materials[2].ambient[2], XiaoPingGuo->materials[2].ambient[3]);
		glUniform4f(k_s, XiaoPingGuo->materials[2].specular[0], XiaoPingGuo->materials[2].specular[1], XiaoPingGuo->materials[2].specular[2], XiaoPingGuo->materials[2].specular[3]);
		glUniform1f(a, XiaoPingGuo->materials[2].shininess);
	}
	else
	{
		// 對應光線，眼睛，ambient，specular，shininess
		// flat shading 模式
		glUniform3f(l, light_pos[0], light_pos[1], light_pos[2]);
		glUniform3f(v, eyex, eyey, 3.0);
		glUniform4f(k_a, XiaoPingGuo->materials[2].ambient[0], XiaoPingGuo->materials[2].ambient[1], XiaoPingGuo->materials[2].ambient[2], XiaoPingGuo->materials[2].ambient[3]);
		glUniform4f(k_s, XiaoPingGuo->materials[2].specular[0], XiaoPingGuo->materials[2].specular[1], XiaoPingGuo->materials[2].specular[2], XiaoPingGuo->materials[2].specular[3]);
		glUniform1f(a, XiaoPingGuo->materials[2].shininess);
	}

	// 對應texture
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(loc2, 0);

	// 用多個三角形繪製apple
	glDrawArrays(GL_TRIANGLES, 0, XiaoPingGuo->groups->next->numtriangles * 3);
	glBindTexture(GL_TEXTURE_2D, NULL);

	glUseProgram(0);
	//---------------------------------------------------------------------------------------------------
	//載入stem shader-------------------------------------------------------------------------------------
	// 註冊投影陣列
	glGetFloatv(GL_PROJECTION_MATRIX, mtx3);
	loc3 = glGetUniformLocation(program_stem, "Projection");

	// 註冊modelview陣列
	glGetFloatv(GL_MODELVIEW_MATRIX, mtx4);
	loc4 = glGetUniformLocation(program_stem, "MV");

	// 取得texture id，註冊texture
	tex1 = XiaoPingGuo->textures[1].id;
	glEnable(GL_TEXTURE_2D);
	loc5 = glGetUniformLocation(program_stem, "MyTexture1");

	// 註冊光線，眼睛，ambient，specular，shininess
	GLint l1, n1, v1, k_a1, k_s1, a1;
	l1 = glGetUniformLocation(program_stem, "light");
	v1 = glGetUniformLocation(program_stem, "vector");
	k_a1 = glGetUniformLocation(program_stem, "ka");
	k_s1 = glGetUniformLocation(program_stem, "ks");
	a1 = glGetUniformLocation(program_stem, "alpha");

	glUseProgram(program_stem);

	// 對應投影矩陣與modelview矩陣
	glUniformMatrix4fv(loc3, 1, GL_FALSE, mtx3);
	glUniformMatrix4fv(loc4, 1, GL_FALSE, mtx4);

	if (shadingType == true)
	{
		// texture index為3
		// 對應光線，眼睛，ambient，specular，shininess
		// phong shading 模式
		glUniform3f(l1, light_pos[0], light_pos[1], light_pos[2]);
		glUniform3f(v1, eyex, eyey, 3.0);
		glUniform4f(k_a1, XiaoPingGuo->materials[3].ambient[0], XiaoPingGuo->materials[3].ambient[1], XiaoPingGuo->materials[3].ambient[2], XiaoPingGuo->materials[3].ambient[3]);
		glUniform4f(k_s1, XiaoPingGuo->materials[3].specular[0], XiaoPingGuo->materials[3].specular[1], XiaoPingGuo->materials[3].specular[2], XiaoPingGuo->materials[3].specular[3]);
		glUniform1f(a1, XiaoPingGuo->materials[3].shininess);
	}
	else
	{
		// 對應光線，眼睛，ambient，specular，shininess
		// flat shading 模式
		glUniform3f(l1, light_pos[0], light_pos[1], light_pos[2]);
		glUniform3f(v1, eyex, eyey, 3.0);
		glUniform4f(k_a1, XiaoPingGuo->materials[3].ambient[0], XiaoPingGuo->materials[3].ambient[1], XiaoPingGuo->materials[3].ambient[2], XiaoPingGuo->materials[3].ambient[3]);
		glUniform4f(k_s1, XiaoPingGuo->materials[3].specular[0], XiaoPingGuo->materials[3].specular[1], XiaoPingGuo->materials[3].specular[2], XiaoPingGuo->materials[3].specular[3]);
		glUniform1f(a1, XiaoPingGuo->materials[3].shininess);
	}

	// 對應texture
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, tex1);
	glUniform1i(loc5, 1);

	// 用多個三角形繪製stem
	glDrawArrays(GL_TRIANGLES, 0, XiaoPingGuo->groups->numtriangles * 3);
	glBindTexture(GL_TEXTURE_2D, NULL);

	glUseProgram(0);
	//---------------------------------------------------------------------------------------------------


	glutSwapBuffers();
	glDisable(GL_DEPTH_TEST);
}


void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
	{	//ESC
		exit(0);
		break;
	}
	case 'd':
	{
		eyex += 0.1;
		break;
	}
	case 'a':
	{
		eyex -= 0.1;
		break;
	}
	case 'w':
	{
		eyey += 0.1;
		break;
	}
	case 's':
	{
		eyey -= 0.1;
		break;
	}
	case 'c':
	{
		if (shadingType) {
			flatShading();
			shadingType = false;
		}
		else {
			shadingType = true;
			phongShading();
		}
		cout << shadingType << endl;
		break;
	}
	default:
	{
		break;
	}
	}
}

void idle(void)
{
	glutPostRedisplay();
}