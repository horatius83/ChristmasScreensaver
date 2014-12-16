#ifndef RENDERABLEPYRAMID_H
#define RENDERABLEPYRAMID_H

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include <gl/GL.h>
#include <gl/GLU.h>
#include "Pyramid.h"
#include "IRenderable.h"
#include "Vector.h"

namespace Rendering {
	class RenderablePyramid : public IRenderable {
	public:
		RenderablePyramid(const Geometry::Pyramid& p) : _pyramid(p) {
			float colors[3][3] = { // Why is this hard-coded?
				{0, 0.25f, 0},
				{0.1804f, 0.5451f, 0.3412f},
				{0.1804f, 0.5451f, 0.3412f}
			};
			for(int i=0;i<3;++i) {
				for(int j=0;j<3;++j) {
					_colors[i][j] = colors[i][j];
				}
			}
			short indices[12] = {
				0, 1, 2, 
				0, 2, 3, 
				0, 3, 4, 
				0, 4, 1
			};
			for(int i=0;i<12;++i) {
				_indices[i] = indices[i];
			}

			/*
			//Calculate the normals
			Math::Vector apex(_pyramid.points[0]);
			Math::Vector v0(_pyramid.points[1]);
			Math::Vector v1(_pyramid.points[2]);
			Math::Vector normal = Math::Vector::Cross(v0,v1);
			normal.Normalize();
			for(int i=0;i<4;++i) {
				_normals[i][0] = normal.x;
				_normals[i][1] = normal.y;
				_normals[i][2] = normal.z;
			}
			*/
			
			//calculate the normals
			//start in 2d
			Math::Vector apex(_pyramid.points[0]);
			for(int i=0;i<4;++i) {
				Math::Vector lp0(_pyramid.points[(i + 1 % 5 == 0) ? 1 : i + 1]);
				Math::Vector lp1(_pyramid.points[(i + 2 % 5 == 0) ? 1 : i + 2]);
				Math::Vector v0(apex, lp0);
				Math::Vector v1(apex, lp1);
				Math::Vector normal = Math::Vector::Cross(v0,v1);
				normal.Normalize();
				//normal.Mul(-1.0f);
				_normals[i][0] = normal.x;
				_normals[i][1] = normal.y;
				_normals[i][2] = normal.z;
			}
		}
		
		void BeginRendering() {
			glBegin(GL_TRIANGLES);
		}

		void Render() {
			for(int i=0;i<12;++i) {
				if(i % 3 == 0) {
					glNormal3fv(_normals[i / 3]);
				}
				//glColor3fv(colors[i % 3]);
				glVertex3fv(_pyramid.points[_indices[i]]);
			}
		}

		void EndRendering() {
			glEnd();
		}
	private:
		float _colors[3][3];
		short _indices[12];
		float _normals[4][3];
		Geometry::Pyramid _pyramid;
	};
}

#endif