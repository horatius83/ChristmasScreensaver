#ifndef PYRAMID_H
#define PYRAMID_H

namespace Geometry {
	class Pyramid {
	public:
		Pyramid(const Pyramid& p) {
			for(int i=0;i<GetPointsSize();++i) {
				for(int j=0;j<3;++j) {
					points[i][j] = p.points[i][j];
				}
			}
		}
		
		Pyramid(float scale) {
			float pyramid[5][3] = {
				{0,scale,0},
				{-scale,-scale,-scale},
				{scale,-scale,-scale},
				{scale,-scale,scale},
				{-scale,-scale,scale}
			};
			for(int i=0;i<GetPointsSize();++i) {
				for(int j=0;j<3;++j) {
					points[i][j] = pyramid[i][j];
				}
			}
		}

		void SubdivideCopy(std::vector<Pyramid>& container) const{
			//Get Midpoints
			float midpoints[4][3];
			for(int i=0;i<4;++i) {
				midpoints[i][0] = getMidPoint(points[0][0],points[i+1][0]);
				midpoints[i][1] = getMidPoint(points[0][1],points[i+1][1]);
				midpoints[i][2] = getMidPoint(points[0][2],points[i+1][2]);
			}

			//Get Bottom Midpoints
			float lowerMidpoints[4][3];
			for(int i=0;i<4;++i) {
				int i2 = (i+2) % 5 > 0 ? i+2 : 1;
				lowerMidpoints[i][0] = getMidPoint(points[i+1][0],points[i2][0]);
				lowerMidpoints[i][1] = getMidPoint(points[i+1][1],points[i2][1]);
				lowerMidpoints[i][2] = getMidPoint(points[i+1][2],points[i2][2]);
			}

			//Get bottom point
			float bottomPoint[3] = { points[0][0], lowerMidpoints[0][1], points[0][2] };

			//tp = top pyramid, lf = lower-left front pyramid, rb = lower right back pyramid
			float tp[5][3], lf[5][3], rf[5][3], lb[5][3], rb[5][3];
			CreatePyramidArray(tp, points[0], midpoints[1], midpoints[0], midpoints[3], midpoints[2]);
			CreatePyramidArray(lf, midpoints[0], lowerMidpoints[3], points[1], lowerMidpoints[0], bottomPoint);

			/*CreatePyramidArray(rf, midpoints[1], lowerMidpoints[0], points[2], lowerMidpoints[1], bottomPoint);
			CreatePyramidArray(lb, midpoints[2], lowerMidpoints[1], points[3], lowerMidpoints[2], bottomPoint);
			CreatePyramidArray(rb, midpoints[3], lowerMidpoints[2], points[4], lowerMidpoints[3], bottomPoint);
*/
			auto b = lowerMidpoints[0];
			auto c = points[2];
			auto d = lowerMidpoints[1];
			auto e = bottomPoint;
			
			
			//CreatePyramidArray(rf, midpoints[1], d, c, b, e);
			CreatePyramidArray(rf, midpoints[1], e, d, c, b);

			CreatePyramidArray(lb, midpoints[2], lowerMidpoints[1], points[3], lowerMidpoints[2], bottomPoint);
			CreatePyramidArray(rb, midpoints[3], lowerMidpoints[2], points[4], lowerMidpoints[3], bottomPoint);


			//Now create the pyramids and add them to the container
			container.push_back(Pyramid(tp));
			container.push_back(Pyramid(lf));
			container.push_back(Pyramid(rf));
			container.push_back(Pyramid(lb));
			container.push_back(Pyramid(rb));
		}

		int GetPointsSize() const { return 5; }
		
		
	private:
		Pyramid(float points[5][3]) {
			for(int i=0;i<GetPointsSize();++i) {
				for(int j=0;j<3;++j) {
					this->points[i][j] = points[i][j];
				}
			}
		};

		void CreatePyramidArray(float arry[5][3], const float p0[3], const float p1[3], const float p2[3], const float p3[3], const float p4[3]) const {
			for(int i=0;i<3;++i) {
				arry[0][i] = p0[i];
				arry[1][i] = p1[i];
				arry[2][i] = p2[i];
				arry[3][i] = p3[i];
				arry[4][i] = p4[i];
			}
		}

		float getMidPoint(float x0, float x1) const {
			return x0 + ((x1 - x0) / 2);
		}
	public: //Is it just me or C++ getting worse about the whole "I'm giving you an indesipherable error message that is is no way related to what actually went wrong"
		float points[5][3];
	};
}

#endif