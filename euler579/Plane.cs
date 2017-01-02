using System;
using System.Windows.Media.Media3D;

namespace euler579
{
    class Plane
    {
        public Vector3D V1 { get; set; }
        public Vector3D V2 { get; set; }
        private readonly string name;
        public double A { get; }
        public double B { get; }
        public double C { get; }
        public double D { get; }

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
            A = (pointB.Y - pointA.Y)*(pointC.Z - pointA.Z) - (pointC.Y - pointA.Y)*(pointB.Z - pointA.Z);
            B = (pointB.Z - pointA.Z)*(pointC.X - pointA.X) - (pointC.Z - pointA.Z)*(pointB.X - pointA.X);
            C = (pointB.X - pointA.X)*(pointC.Y - pointA.Y) - (pointC.X - pointA.X)*(pointB.Y - pointA.Y);
            D = -(A*pointA.X + B*pointA.Y + C*pointA.Z);
            
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
            return Math.Sign(A*p.X + B*p.Y + C*p.Z + D);
        }

    }
}