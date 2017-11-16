#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
using namespace std;

class Shape {
private:
    double x, y, z;
public:
    Shape(double x=0, double y=0, double z=0) : x(x), y(y), z(z) {}
    double getX(){ return x;}
    double getY(){ return y;}
    double getZ() {return z;}
    virtual double getR() {};
    virtual double getH() {};
    virtual int getFacets(){} ;
    virtual double getAll() {};
};

class Cylinder : public Shape{
private:
    double r, h;
    int facets;

public:
    Cylinder(double x, double y, double z, double r, double h, int facets) : Shape(x,y,z),r(r),h(h),facets(facets){
        cout<< getX() << ' ' << getY() << ' ' << getZ() << ' ' << r << ' ' << h << ' ' << facets << endl;
    }
    int getFacets(){ return facets;}
    double getR(){ return r;}
    double getH() {return h;}
};
static double  pi = 3.14159265358979323846;
class Cube: public Shape {
private:
    double size;
public:
    Cube(double x=0, double y=0, double z=0, double size=100) : Shape(x,y,z), size(size) {
        cout<< getX() << ' ' << getY() << ' ' << getZ() << ' ' << size << endl;
    }
    double getAll(){ return size;}
};

/*
 * class Vect3d, use to
 */
class Vect3d{
private:
    double x, y, z;
public:
    Vect3d(){}
    Vect3d(double x, double y,double z):x(x), y(y), z(z) {}
    Vect3d getNormals(const Vect3d& p1, const Vect3d &p2, const Vect3d&p3) const{
        double a = ( (p2.y-p1.y)*(p3.z-p1.z)-(p2.z-p1.z)*(p3.y-p1.y) );
        double b = ( (p2.z-p1.z)*(p3.x-p1.x)-(p2.x-p1.x)*(p3.z-p1.z) );
        double c = ( (p2.x-p1.x)*(p3.y-p1.y)-(p2.y-p1.y)*(p3.x-p1.x) );
        return Vect3d(a,b,c);
    }
    double getX() const{ return x;}
    double getY() const{ return y;}
    double getZ() const{ return z;}

};



class CAD {
private:
    vector<Shape*> shapes;
public:
    CAD(){}
    ~CAD(){
        for(int i=0; i<shapes.size(); i++)
            delete shapes[i];
    }
    CAD(const CAD &orig) = delete;
    CAD &operator = (const CAD &orig) = delete;

    void add(Shape *s){
        shapes.push_back(s);
    }

    void write(const string& filename){
        for (int i=0; i<shapes.size();i++) {
            vector<string> num ={"1","2","3","4","5","6","7","8"};
            double X = shapes[i]->getX();
            double Y = shapes[i]->getY();
            double Z = shapes[i]->getZ();
            if(shapes[i]->getR()>0) {
                string s1 = "cylinder_" + num[i] + "_";
                ofstream out(s1+filename);
                if(out.fail()){
                    cout<<"Open fail!";
                    exit(1);
                } //end if1
                out << "solid OpenSCAD_Model" <<'\n';
                double deg = 360.0/shapes[i]->getFacets()/180.0*M_PI;
                double R = shapes[i]->getR();
                double H = shapes[i]->getH();

                for(double j=0.0; j<=2.0*M_PI; j=j+deg){
                    //bot surface normals
                    Vect3d b1(X,Y,Z);
                    Vect3d b2(X+R*cos(j+deg),Y+R*sin(j+deg),Z);
                    Vect3d b3(X+R*cos(j),Y+R*sin(j),Z);
                    Vect3d b4;
                    Vect3d b = b4.getNormals(b1,b2,b3);

                    //top surface normals
                    Vect3d t1(X,Y,Z+H);
                    Vect3d t2(X+R*cos(j+deg),Y+R*sin(j+deg),Z+H);
                    Vect3d t3(X+R*cos(j),Y+R*sin(j),Z+H);
                    Vect3d t4;
                    Vect3d t = t4.getNormals(t1,t2,t3);

                    //side1 surface normals
                    Vect3d s11(X+R*cos(j),Y+R*sin(j),Z+H);
                    Vect3d s12(X+R*cos(j),Y+R*sin(j),Z);
                    Vect3d s13(X+R*cos(j+deg),Y+R*sin(j+deg),Z);
                    Vect3d s14;
                    Vect3d s1 = s14.getNormals(s11,s12,s13);

                    //side2 surface normals
                    Vect3d s21(X+R*cos(j+deg),Y+R*sin(j+deg),Z);
                    Vect3d s22(X+R*cos(j+deg),Y+R*sin(j+deg),Z+H);
                    Vect3d s23(X+R*cos(j),Y+R*sin(j),Z+H);
                    Vect3d s24;
                    Vect3d s2 = s24.getNormals(s21,s22,s23);

                    //bottom
                    out << "  facet normal "<<b.getX()<<' '<<b.getY()<<' '<<b.getZ() <<'\n';
                    out << "    outer loop" <<'\n';
                    out << "      vertex " <<X<<' '<<Y<<' '<<Z<<'\n';                                           //O
                    out << "      vertex " <<X+R*cos(j+deg)<<' '<<Y+R*sin(j+deg)<<' '<<Z<<'\n';                 //B
                    out << "      vertex " <<X+R*cos(j)<<' '<<Y+R*sin(j)<<' '<<Z<<'\n';                         //A
                    out << "    endloop" <<'\n';
                    out << "  endfacet" <<'\n';
                    //top
                    out << "  facet normal "<<t.getX()<<' '<<t.getY()<<' '<<t.getZ() <<'\n';
                    out << "    outer loop" <<'\n';
                    out << "      vertex " <<X<<' '<<Y<<' '<<Z+H<<'\n';                                         //O'
                    out << "      vertex " <<X+R*cos(j)<<' '<<Y+R*sin(j)<<' '<<Z+H<<'\n';                       //A'
                    out << "      vertex " <<X+R*cos(j+deg)<<' '<<Y+R*sin(j+deg)<<' '<<Z+H<<'\n';               //B'
                    out << "    endloop" <<'\n';
                    out << "  endfacet" <<'\n';
                    //side1
                    out << "  facet normal "<<s1.getX()<<' '<<s1.getY()<<' '<<s1.getZ() <<'\n';
                    out << "    outer loop" <<'\n';
                    out << "      vertex " <<X+R*cos(j)<<' '<<Y+R*sin(j)<<' '<<Z+H<<'\n';                       //A'
                    out << "      vertex " <<X+R*cos(j)<<' '<<Y+R*sin(j)<<' '<<Z<<'\n';                         //A
                    out << "      vertex " <<X+R*cos(j+deg)<<' '<<Y+R*sin(j+deg)<<' '<<Z<<'\n';                 //B
                    out << "    endloop" <<'\n';
                    out << "  endfacet" <<'\n';
                    //side2
                    out << "  facet normal "<<s2.getX()<<' '<<s2.getY()<<' '<<s2.getZ() <<'\n';
                    out << "    outer loop" <<'\n';
                    out << "      vertex " <<X+R*cos(j+deg)<<' '<<Y+R*sin(j+deg)<<' '<<Z<<'\n';                 //B
                    out << "      vertex " <<X+R*cos(j+deg)<<' '<<Y+R*sin(j+deg)<<' '<<Z+H<<'\n';               //B'
                    out << "      vertex " <<X+R*cos(j)<<' '<<Y+R*sin(j)<<' '<<Z+H<<'\n';                       //A'
                    out << "    endloop" <<'\n';
                    out << "  endfacet" <<'\n';
                }//end for
                out << "endsolid OpenSCAD_Model" <<'\n';

            }//end if2
            else {
                double size = shapes[i]->getAll();
                string s1 = "cube_" + num[i] + "_";
                ofstream out(s1+filename);
                if(out.fail()){
                    cout<<"Open fail!";
                    exit(1);
                }//end if
                //1
                out << "solid OpenSCAD_Model" <<'\n';
                out << "  facet normal -0 0 1" <<'\n';
                out << "    outer loop" <<'\n';
                out << "      vertex " <<X<<' '<<size+Y<<' '<<size+Z<<'\n';
                out << "      vertex " << X+size<<' '<<Y<<' '<<Z+size<<'\n';
                out << "      vertex " <<X+size<<' '<<Y+size<<' '<<Z+size<<'\n';
                out << "    endloop" <<'\n';
                out << "  endfacet" <<'\n';
                //2
                out << "  facet normal 0 0 1" <<'\n';
                out << "    outer loop" <<'\n';
                out << "      vertex " << X+size<<' '<<Y<<' '<<Z+size<<'\n';
                out << "      vertex " <<X<<' '<<size+Y<<' '<<size+Z<<'\n';
                out << "      vertex " <<X<<' '<<Y<<' '<<size+Z<<'\n';
                out << "    endloop" <<'\n';
                out << "  endfacet" <<'\n';
                //3
                out << "  facet normal 0 0 -1" <<'\n';
                out << "    outer loop" <<'\n';
                out << "      vertex " << X<<' '<<Y<<' '<<Z<<'\n';
                out << "      vertex " <<X+size<<' '<<size+Y<<' '<<Z<<'\n';
                out << "      vertex " <<X+size<<' '<<Y<<' '<<Z<<'\n';
                out << "    endloop" <<'\n';
                out << "  endfacet" <<'\n';
                //4
                out << "  facet normal -0 0 -1" <<'\n';
                out << "    outer loop" <<'\n';
                out << "      vertex " <<X+size<<' '<<size+Y<<' '<<Z<<'\n';
                out << "      vertex " <<X<<' '<<Y<<' '<<Z<<'\n';
                out << "      vertex " <<X<<' '<<size+Y<<' '<<Z<<'\n';
                out << "    endloop" <<'\n';
                out << "  endfacet" <<'\n';
                //5
                out << "  facet normal 0 -1 0" <<'\n';
                out << "    outer loop" <<'\n';
                out << "      vertex " <<X<<' '<<Y<<' '<<Z<<'\n';
                out << "      vertex " <<X+size<<' '<<Y<<' '<<Z+size<<'\n';
                out << "      vertex " <<X<<' '<<Y<<' '<<Z+size<<'\n';
                out << "    endloop" <<'\n';
                out << "  endfacet" <<'\n';
                //6
                out << "  facet normal 0 -1 0" <<'\n';
                out << "    outer loop" <<'\n';
                out << "      vertex " <<X+size<<' '<<Y<<' '<<Z+size<<'\n';
                out << "      vertex " <<X<<' '<<Y<<' '<<Z<<'\n';
                out << "      vertex " <<X+size<<' '<<Y<<' '<<Z<<'\n';
                out << "    endloop" <<'\n';
                out << "  endfacet" <<'\n';
                //7
                out << "  facet normal 1 -0 0" <<'\n';
                out << "    outer loop" <<'\n';
                out << "      vertex " <<X+size<<' '<<Y<<' '<<Z+size<<'\n';
                out << "      vertex " <<X+size<<' '<<size+Y<<' '<<Z<<'\n';
                out << "      vertex " <<X+size<<' '<<size+Y<<' '<<Z+size<<'\n';
                out << "    endloop" <<'\n';
                out << "  endfacet" <<'\n';
                //8
                out << "  facet normal 1 0 0" <<'\n';
                out << "    outer loop" <<'\n';
                out << "      vertex " <<X+size<<' '<<size+Y<<' '<<Z<<'\n';
                out << "      vertex " <<X+size<<' '<<Y<<' '<<Z+size<<'\n';
                out << "      vertex " <<X+size<<' '<<Y<<' '<<Z<<'\n';
                out << "    endloop" <<'\n';
                out << "  endfacet" <<'\n';
                //9
                out << "  facet normal 0 1 -0" <<'\n';
                out << "    outer loop" <<'\n';
                out << "      vertex " <<X+size<<' '<<size+Y<<' '<<Z<<'\n';
                out << "      vertex " <<X<<' '<<size+Y<<' '<<Z+size<<'\n';
                out << "      vertex " <<X+size<<' '<<size+Y<<' '<<Z+size<<'\n';
                out << "    endloop" <<'\n';
                out << "  endfacet" <<'\n';
                //10
                out << "  facet normal 0 1 0" <<'\n';
                out << "    outer loop" <<'\n';
                out << "      vertex " <<X<<' '<<size+Y<<' '<<Z+size<<'\n';
                out << "      vertex " <<X+size<<' '<<size+Y<<' '<<Z<<'\n';
                out << "      vertex " <<X<<' '<<size+Y<<' '<<Z<<'\n';
                out << "    endloop" <<'\n';
                out << "  endfacet" <<'\n';
                //11
                out << "  facet normal -1 0 0" <<'\n';
                out << "    outer loop" <<'\n';
                out << "      vertex " <<X<<' '<<Y<<' '<<Z<<'\n';
                out << "      vertex " <<X<<' '<<size+Y<<' '<<Z+size<<'\n';
                out << "      vertex " <<X<<' '<<size+Y<<' '<<Z<<'\n';
                out << "    endloop" <<'\n';
                out << "  endfacet" <<'\n';
                //12
                out << "  facet normal -1 -0 0" <<'\n';
                out << "    outer loop" <<'\n';
                out << "      vertex " <<X<<' '<<size+Y<<' '<<Z+size<<'\n';
                out << "      vertex " <<X<<' '<<Y<<' '<<Z<<'\n';
                out << "      vertex " <<X<<' '<<Y<<' '<<Z+size<<'\n';
                out << "    endloop" <<'\n';
                out << "  endfacet" <<'\n';
                out << "endsolid OpenSCAD_Model" <<'\n';
                out.close();
            }//end else
        }//emd outer for
    }//end write
};

//https://www.stratasysdirect.com/resources/how-to-prepare-stl-files/
//https://www.viewstl.com/
int main() {

    CAD c;
    c.add(new Cube(10,0,0, 5));
    c.add(new Cylinder(0,0,0, 10,10,10));
    c.add(new Cylinder(100,0,0, 3,10,100));
    c.write("test.stl");


}
