// Andrés Mejía
// Gives Wrong Answer on PKU. Experimenting with different things.
using namespace std;
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <fstream>
#include <cassert>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <string>
#include <cstdio>
#include <vector>
#include <cmath>
#include <queue>
#include <deque>
#include <stack>
#include <list>
#include <map>
#include <set>

////////////// Prewritten code follows. Look down for solution. ////////////////
#define foreach(x, v) for (typeof (v).begin() x=(v).begin(); x !=(v).end(); ++x)
#define For(i, a, b) for (int i=(a); i<(b); ++i)
#define D(x) cout << #x " is " << (x) << endl

const double EPS = 1e-9;
inline int cmp(double x, double y = 0, double tol = EPS) {
    return (x <= y + tol) ? (x + tol < y) ? -1 : 0 : 1;
}
////////////////////////// Solution starts below. //////////////////////////////

#define myAssert(what) { \
            if (!(what)) { \
                fprintf(stderr, "Assertion %s failed on line %d\n", #what, __LINE__); \
                fflush(stderr); \
                fflush(stdin); \
                int * p = NULL; printf("%d\n", *p); \
            } \
        }
     
struct Point {
    double x, y, z;
    Point(){} Point(double x, double y, double z) : x(x), y(y), z(z) {}
    
    static Point read() {
        int x, y, z;
        scanf("%d%d%d", &x, &y, &z);
        return Point(x, y, z);
    }

    Point operator + (const Point &t) const {
        return Point(x + t.x, y + t.y, z + t.z);
    }
    
    Point operator - (const Point &t) const {
        return Point(x - t.x, y - t.y, z - t.z);
    }
    Point operator * (const Point &t) const {
        double nx = +(y * t.z - t.y * z);
        double ny = -(x * t.z - t.x * z);
        double nz = +(x * t.y - t.x * y);
        return Point(nx, ny, nz);
    }
    bool operator == (const Point &other) const {
        return cmp(x, other.x) == 0 and cmp(y, other.y) == 0 and cmp(z, other.z) == 0;
    }
    bool operator != (const Point &other) const {
        return !(*this == other);
    }    
    double operator ^ (const Point &t) const {
        return x * t.x + y * t.y + z * t.z;
    }
    
    Point operator -() const {
        return Point(-x, -y, -z);
    }

    double length() const {
        return sqrt(x * x + y * y + z * z);
    }
    void normalize() {
        double m = length();
        x /= m, y /= m, z /= m;
    }

    friend ostream& operator << (ostream &out, Point p);
};

ostream& operator << (ostream &out, Point p) {
    out << "(" << p.x << ", " << p.y << ", " << p.z << ")";
    return out;
}

struct Plane {
    double a, b, c, d;
    Plane(){}
    Plane(const Point &p1, const Point &p2, const Point &p3) {
        myAssert(p1 != p2);
        myAssert(p1 != p3);
        myAssert(p2 != p3);
        Point normal = (p2 - p1) * (p3 - p1);
        normal.normalize();
        if (cmp(normal.x, 0) < 0 or
            cmp(normal.x, 0) == 0 and cmp(normal.y, 0) < 0 or
            cmp(normal.x, 0) == 0 and cmp(normal.y, 0) == 0 and cmp(normal.z, 0) < 0) {
                normal = -normal;
        }
        a = normal.x, b = normal.y, c = normal.z;
        d = -(normal ^ p1);
        myAssert(cmp(d, -(normal ^ p2)) == 0);
        myAssert(cmp(d, -(normal ^ p3)) == 0);
    }
    
    bool operator < (const Plane &t) const {
        int k = cmp(a, t.a);
        if (k != 0) return k < 0;
        k = cmp(b, t.b);
        if (k != 0) return k < 0;
        k = cmp(c, t.c);
        if (k != 0) return k < 0;
        k = cmp(d, t.d);
        return k < 0;
    }
    
    double distance(const Point &p) const {
        return (Point(a, b, c) ^ p) + d;
    }
    
    int side(const Point &p) const {
        return cmp(distance(p), 0);
    }
};

struct Face {
    vector<Point> v;
    Plane p;
    Face(const Plane &p) : p(p) {}
};

struct AngleComparator {
    double nxx;
    double nxy;
    double nxz;
    double nyx;
    double nyy;
    double nyz;
    double x0;
    double y0;

    AngleComparator(Plane p, Point origin) {
        if (abs(p.a) > EPS || abs(p.b) > EPS) {
            nxx = -p.b;
            nxy = p.a;
            nxz = 0;
        } else {
            nxx = 0;
            nxy = -p.c;
            nxz = p.b;
        }

        nyx = nxy * p.c - nxz * p.b;
        nyy = nxz * p.a - nxx * p.c;
        nyz = nxx * p.b - nxy * p.a;

        myAssert(abs(p.a * nxx + p.b * nxy + p.c * nxz) < EPS);
        myAssert(abs(p.a * nyx + p.b * nyy + p.c * nyz) < EPS);

        x0 = origin.x * nxx + origin.y * nxy + origin.z * nxz;
        y0 = origin.x * nyx + origin.y * nyy + origin.z * nyz;
    }

    bool operator()(Point o1, Point o2) {
        double x1 = o1.x * nxx + o1.y * nxy + o1.z * nxz - x0;
        double y1 = o1.x * nyx + o1.y * nyy + o1.z * nyz - y0;
        double x2 = o2.x * nxx + o2.y * nxy + o2.z * nxz - x0;
        double y2 = o2.x * nyx + o2.y * nyy + o2.z * nyz - y0;
        double prod = x1 * y2 - x2 * y1;
        if (prod < -EPS) {
            return true; // -1
        } else if (prod > EPS) {
            return false; // +1
        } else {
            return false; // 0
        }
    }
};

bool sameLine(Point p1, Point p2, Point p3) {
    Point u = p2 - p1;
    Point v = p3 - p1;
    double determinant = (u * v).length();
    return cmp(determinant, 0) == 0;
}

struct Polyhedron {
    vector<Point> v; // vertices
        
    static Polyhedron read() {
        int n; scanf("%d", &n);
        Polyhedron p;
        p.v.resize(n);
        for (int i = 0; i < n; ++i) {
            p.v[i] = Point::read();
        }
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                for (int k = j + 1; k < n; ++k) {
                    myAssert(!sameLine(p.v[i], p.v[j], p.v[k]));
                }
            }
        }
        return p;
    }
    
    vector<Face> buildFaces() {
        set<Plane> seen;
        int n = v.size();
        
        vector<Face> ans;
        
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                for (int k = j + 1; k < n; ++k) {
                    Plane p(v[i], v[j], v[k]);
                    if (seen.count(p) > 0) continue;

                    seen.insert(p);
                    //printf("plane is (a=%.0lf, b=%.0lf, c=%.0lf, d=%.0lf)\n", p.a, p.b, p.c, p.d);
                    Face f(p);
                    bool positive = false, negative = false;
                    for (int l = 0; l < n; ++l) {
                        int side = p.side(v[l]);
                        positive |= (side > 0);
                        negative |= (side < 0);
                        if (side == 0) {
                            f.v.push_back( v[l] );
                        }
                    }
                    
                    if (not (positive and negative)) {
                        myAssert(f.v.size() >= 3);
                        //printf("face plane is (a=%.0lf, b=%.0lf, c=%.0lf, d=%.0lf)\n", f.p.a, f.p.b, f.p.c, f.p.d);                        
                        AngleComparator comp(f.p, f.v[0]);
                        sort(f.v.begin() + 1, f.v.end(), comp);
                        ans.push_back( f );
                    }
                }
            }
        }
        return ans;
    }
    
    double tetrahedronVolume(const Point &p1, const Point &p2, const Point &p3, const Point &p4) {
        Point u = p2 - p1;
        Point v = p3 - p1;
        Point w = p4 - p1;
        double vol = abs((w ^ (u * v))) / 6.0;
        myAssert(cmp(vol, 0) >= 0);
        return vol;
    }
    
    Point tetrahedronCentroid(const Point &p1, const Point &p2, const Point &p3, const Point &p4) {
        return Point((p1.x + p2.x + p3.x + p4.x) / 4.0, 
                     (p1.y + p2.y + p3.y + p4.y) / 4.0, 
                     (p1.z + p2.z + p3.z + p4.z) / 4.0);
    }
    
    Point centerOfMass(const vector<Face> &faces) {
        Point p1 = v[0];
        double cx = 0, cy = 0, cz = 0;
        double totalVolume = 0;
        
        int m = faces.size();
        for (int k = 0; k < m; ++k) {
            const Face &f = faces[k];
            Point p2 = f.v[0];
            
            for (int i = 1; i + 1 < f.v.size(); ++i) {
                Point p3 = f.v[i];
                Point p4 = f.v[i + 1];
                double v = tetrahedronVolume(p1, p2, p3, p4);
                Point c = tetrahedronCentroid(p1, p2, p3, p4);
                
                cx += c.x * v;
                cy += c.y * v;
                cz += c.z * v;
                totalVolume += v;
            }
        }
        
        myAssert(cmp(totalVolume, 0) > 0);
        cx /= totalVolume;
        cy /= totalVolume;
        cz /= totalVolume;
        
        return Point(cx, cy, cz);
    }
    
    bool leftTurn(const Plane &plane, const Point &p1, const Point &p2, const Point &p3) {
        Point u = p2 - p1;
        Point v = p3 - p1;
        int s = plane.side( p1 + (u * v) );
        // printf("plane is (a=%.0lf, b=%.0lf, c=%.0lf, d=%.0lf)\n", plane.a, plane.b, plane.c, plane.d);
        // D(p1);
        // D(p2);
        // D(p3);
        // D(s);
        AngleComparator comp(plane, p1);
        myAssert(comp(p2, p3));
        return s > 0;
    }
    
    double minDistance() {
        vector<Face> faces = buildFaces();
        myAssert(faces.size() > 2);
        Point c = centerOfMass(faces);
        // printf("Center of mass = (%.0lf, %.0lf, %.0lf)\n", c.x, c.y, c.z);
        double ans = 1e100;
        
        for (int k = 0; k < faces.size(); ++k) {
            const Face &f = faces[k];
            // printf("Points on this face are:\n");
            // foreach(p, f) cout << *p << " ";
            // puts("");
            
            for (int i = 0; i + 2 < f.v.size(); ++i) {
                myAssert(leftTurn(f.p, f.v[i], f.v[i+1], f.v[i+2]));
            }

            ans = min(ans, abs(f.p.distance(c)));
        }
        return ans;
    }
};

int main(){
    Polyhedron a = Polyhedron::read(), b = Polyhedron::read();
    printf("%.6lf\n", a.minDistance() + b.minDistance());
    return 0;
}