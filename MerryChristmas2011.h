#ifndef MERRYCHRISTMAS2011_H
#define MERRYCHRISTMAS2011_H

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include <gl/GL.h>
#include <gl/GLU.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "Timer.h"
#include "Pyramid.h"
#include "RenderablePyramid.h"
#include "Vector.h"

class MerryChristmas2011 {
public:
	MerryChristmas2011(int width, int height, int bpp, const std::string& logFile) : m_width(width), m_height(height), m_bpp(bpp) {
		m_log.open(logFile);
		cylinder = gluNewQuadric();
	}

	~MerryChristmas2011() {
		m_log.close();
		gluDeleteQuadric(cylinder);
		for(auto i = objs.begin(); i != objs.end(); ++i) {
			if((*i) != 0) { delete *i; };
		}
	}

	void ChangeResolution(int width, int height, int bpp) {
		//Set projection
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, m_width, m_height, 0, -1, 1);

		//Initialize modelview matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glViewport(0,0,m_width,m_height);
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluPerspective(45.0f, (GLfloat)m_width/(GLfloat)m_height, 0.1f, 100.0f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	bool Initialize(int width, int height, int bpp) {
		using std::endl;

		glShadeModel(GL_SMOOTH);
		glClearColor(0,0,0,0);
		glClearDepth(1.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		m_width = width;
		m_height = height;
		m_bpp = bpp;
		ChangeResolution(m_width, m_height, m_bpp);

		bool isSuccessful = true;

		//Create Geometry
		Geometry::Pyramid pyramid(1.0f);
		std::vector<Geometry::Pyramid> pyramids;
		pyramids.push_back(pyramid);
		//std::vector<Rendering::IRenderable*> objs;
		GeneratePyramid(objs, pyramids, 6);
		
		//Create display list
		if(objs.size() > 0) {
			sierpinskiPyramid = glGenLists(1);
			glNewList(sierpinskiPyramid,GL_COMPILE);
			objs[0]->BeginRendering();
			for(auto i = objs.begin(); i != objs.end(); ++i) {
				(*i)->Render();
			}
			objs[0]->EndRendering();
			glEndList();
		} else { isSuccessful = false; } //we need something to render

		//Create ornaments
		glEnable(GL_POINT_SMOOTH);
		srand((unsigned)time(0));
		GenerateOrnaments(this->ornaments, 6, 200, 1.2f, 1.9f);
		glPointSize(4.0f);

		//Setup lighting
		float ambient[4] = {0.5f, 0.5f, 0.5f, 1.0f};
		float diffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
		float position[4] = {0.3334,-0.3334f, 0.3334, 0.0f};
		for(int i=0;i<4;++i) {
			this->ambient[i] = ambient[i];
			this->diffuse[i] = diffuse[i];
			this->position[i] = position[i];
		}

		glLightfv(GL_LIGHT1, GL_AMBIENT, this->ambient);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, this->diffuse);
		glLightfv(GL_LIGHT1, GL_POSITION, this->position);
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHTING);

		//Setup materials
		float treeDiffuse[3] = {0.1804f, 0.5451f, 0.3412f};
		float treeSpecular[3] = {1.0f, 1.0f, 1.0f};
		float trunkDiffuse[3] = {0.545f, 0.271f, 0.075f};
		float trunkSpecular[3] = {0.645f, 0.371f, 0.175f};
		this->treeMaterial.shininess = 70.0f;
		this->trunkMaterial.shininess = 1.0f;
		
		for(int i=0;i<3;++i) {
			this->treeMaterial.diffuse[i] = treeDiffuse[i];
			this->treeMaterial.specular[i] = treeSpecular[i];
			this->trunkMaterial.diffuse[i] = trunkDiffuse[i];
			this->trunkMaterial.specular[i] = trunkSpecular[i];
		}

		GLenum errorCode = glGetError();
		
		while(errorCode != GL_NO_ERROR) {
			m_log << "ERROR: " << gluErrorString(errorCode) << endl;
			isSuccessful = false;
			errorCode = glGetError();
		}
		return isSuccessful;
	}
	
	void DrawScene() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		double seconds = m_timer.GetElapsedTime();
		float fseconds = (float)seconds;
		static float totalSeconds = 0;
		totalSeconds += fseconds;

		m_timer.Restart();
		static double rotation = 0;
		rotation += seconds * 10.0;

		glLoadIdentity();
		glTranslatef(0,0.5f,-4.0f);
		glRotated(rotation,0,1.0,0);
		
		this->treeMaterial.Bind();
		glCallList(sierpinskiPyramid);

		glPushMatrix();
		glTranslatef(0,-0.9f,0);
		
		glDisable(GL_LIGHTING);
		for(unsigned int i=0;i<ornaments.size();++i) {
			float fi = (float)i;
			float r = 1.0f + 0.5f * sin(2.0f * totalSeconds - fi);
			float g = 1.0f + 0.5f * sin(4.0f * totalSeconds - 2.0f * fi);
			float b = 1.0f + 0.5f * sin(6.0f * totalSeconds - 3.0f * fi);
			glColor3f(r,g,b);
			glCallList(ornaments[i]);
		}
		glEnable(GL_LIGHTING);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0,-1.0f,0);
		glRotatef(90.0f,1.0f,0,0);

		this->trunkMaterial.Bind();
		//glColor3f(139.0f/255.0f, 69.0f/255.0f, 19.0f/255.0f);
		gluCylinder(cylinder, 0.2f, 0.25f, 0.5f, 10, 10);
		glPopMatrix();
	}

	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }
	int GetBpp() { return m_bpp; }
private:
	//Generate the ornaments on the outside of this pyramid
	void GenerateOrnaments(std::vector<GLuint>& ornamentLists, int typesOfOrnaments, int numberOfOrnaments, float width, float height) {
		//x = (-w/h)y + w
		float m = -width / height;
		float b = width; 

		for(int i=0;i<typesOfOrnaments;++i) {
			GLuint displayList = glGenLists(1);
			ornamentLists.push_back(displayList);
			glNewList(displayList,GL_COMPILE);
			glBegin(GL_POINTS);

			for(int j=0;j<numberOfOrnaments;++j) {
				//y = [0,height], phi = [0,2pi]
				float y = GenRand() * height;
				float phi = GenRand() * 2.0f * 3.141592654f;
				float w = m * y + b;
				float x = w * (float)cos(phi);
				float z = w * (float)sin(phi);
				glVertex3f(x,y,z);
			}
			glEnd();
			glEndList();
		}
	}

	void GeneratePyramid(std::vector<Rendering::IRenderable*>& objs, const std::vector<Geometry::Pyramid>& pyramids, int levels) {
		std::vector<Geometry::Pyramid> a;
		a = pyramids; //copy the data into a
		std::vector<Geometry::Pyramid> b;
		std::vector<Geometry::Pyramid>* from = &a; //we're going to switch back and forth between these
		std::vector<Geometry::Pyramid>* to = &b;

		for(int i=0;i<levels;++i) {
			to->clear();
			for(auto iter = from->begin(); iter != from->end(); ++iter) { 
				iter->SubdivideCopy(*to);
			}
			std::vector<Geometry::Pyramid>* temp = to;
			to = from;
			from = temp;
		}

		for(auto i = from->begin(); i != from->end(); ++i) {
			objs.push_back(new Rendering::RenderablePyramid(*i));
		}
	}
	float GenRand() { return (float)rand() / (float)RAND_MAX; }

	//lighting
	float ambient[4];
	float diffuse[4];
	float position[4];

	//materials
	struct Material {
		GLfloat diffuse[4];
		GLfloat specular[4];
		GLfloat shininess;

		void Bind() {
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);
		}
	};

	Material treeMaterial;
	Material trunkMaterial;

	std::vector<Rendering::IRenderable*> objs;
	GLUquadric* cylinder;
	GLuint sierpinskiPyramid;
	std::vector<GLuint> ornaments;
	Rendering::Timer m_timer;
	std::ofstream m_log;
	int m_width;
	int m_height;
	int m_bpp;
};

#endif