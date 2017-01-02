using System;
using System.Windows.Media.Media3D;

namespace euler579
{
    class Plane
    {
        public Vector3D V1 { get; set; }
        public Vector3D V2 { get; set; }
        private readonly string name;
        public int A { get; }
        public int B { get; }
        public int C { get; }
        public int D { get; }

        public override string ToString()
        {
            return $"{name}";
        }

        public Plane (Vector3D origin, Vector3D v1, Vector3D v2, string name)
        {
            this.name = name;

            //http://keisan.casio.com/has10/SpecExec.cgi?id=system/2006/1223596129
            var pointA = origin;
            var pointB = origin + v1;
            var pointC = origin + v2;
            A = ((int)pointB.Y - (int)pointA.Y)*((int)pointC.Z - (int)pointA.Z) - ((int)pointC.Y - (int)pointA.Y)*((int)pointB.Z - (int)pointA.Z);
            B = ((int)pointB.Z - (int)pointA.Z)*((int)pointC.X - (int)pointA.X) - ((int)pointC.Z - (int)pointA.Z)*((int)pointB.X - (int)pointA.X);
            C = ((int)pointB.X - (int)pointA.X)*((int)pointC.Y - (int)pointA.Y) - ((int)pointC.X - (int)pointA.X)*((int)pointB.Y - (int)pointA.Y);
            D = -(A* (int)pointA.X + B* (int)pointA.Y + C* (int)pointA.Z);
            
            //this is wrong!:
            /*Origin = origin;
            V1 = v1;
            V2 = v2;
            this.name = name;
            var normal = Vector3D.CrossProduct(v1, v2);
            A  = normal.X;
            B = normal.Y;
            C = normal.Z;
            D = -Vector3D.DotProduct(origin, normal);*/
        }

        // sing portion of http://forums.codeguru.com/showthread.php?378277-Distance-between-plane-and-a-point
        public int GetSide(Vector3D p)
        {
            var dist = A*p.X + B*p.Y + C*p.Z + D;
            
            var sign = Math.Sign(dist);
            if (sign != 0 && Math.Abs(dist) < 1e-6) throw new Exception("Point is very very close but sign is non-zero!");
            return sign;
        }
    }
}