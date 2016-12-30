using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media.Animation;
using System.Windows.Media.Media3D;

namespace euler579
{
    class Plane
    {
        public Vector3D Origin { get; set; }
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
            Origin = origin;
            V1 = v1;
            V2 = v2;
            this.name = name;
            var normal = Vector3D.CrossProduct(v1, v2);
            A  = normal.X;
            B = normal.Y;
            C = normal.Z;
            D = -Vector3D.DotProduct(origin, normal);
        }

        // sing portion of http://forums.codeguru.com/showthread.php?378277-Distance-between-plane-and-a-point
        public int GetSide(Vector3D p)
        {
            return Math.Sign(A*p.X + B*p.Y + C*p.Z + D);
        }

    }

    class PlanePair
    {
        public Plane Plane1 { get; }
        public Plane Plane2 { get; }

        public PlanePair(Plane plane1, Plane plane2)
        {
            Plane1 = plane1;
            Plane2 = plane2;
        }

        public bool IsBetweenOrOn(Vector3D point)
        {
            var side1 = Plane1.GetSide(point);
            var side2 = Plane2.GetSide(point);
            var isBetweenOrOn = side1 == 0 || side2 == 0 || side1 != side2;
            return isBetweenOrOn;
        }
    }

    class Cube : IEquatable<Cube>
    {
        public Vector3D[] Definitions { get; }
        private readonly Lazy<bool> isStraight;
        public bool IsStraight { get { return isStraight.Value; } }
        public int LatticePoints { get { return latticePoints.Value; } }


        public Vector3D[] Vertices { get;  }
        public Vector3D[] OrderedVertices { get; }
        public Vector3D A { get { return Definitions[0]; } }
        public Vector3D B { get { return Definitions[1]; } }
        public Vector3D C { get { return Definitions[2]; } }
        public Vector3D O { get { return Vertices[0]; } }

        private static readonly Vector3D xAxis = new Vector3D(1, 0, 0);
        private static readonly Vector3D yAxis = new Vector3D(0, 1, 0);
        private static readonly Vector3D zAxis = new Vector3D(0, 0, 1);
        private Lazy<int> latticePoints;

        public Cube(Vector3D[] vertices, Vector3D[] definitions = null)
        {
            if (definitions == null) definitions = vertices.Skip(1).Take(3).Select(v => v - vertices[0]).ToArray();
            Definitions = definitions;
            if (vertices.Length != 8) throw new InvalidOperationException("Sanity check failed.");
            OrderedVertices = vertices.OrderBy(v => v.X).ThenBy(v => v.Y).ThenBy(v => v.Z).ToArray();
            Vertices = vertices.ToArray();
            isStraight = new Lazy<bool>(CalculateIsStraight);
            latticePoints = new Lazy<int>(CalculateLatticePoints);
        }

        private int CalculateLatticePoints()
        {
            var front = new Plane(O, A, B, "front");
            var back = new Plane(O+C, A, B, "back");
            var left = new Plane(O, B, C, "left");
            var right = new Plane(O + A, B, C, "right");
            var bottom = new Plane(O, A, C, "bottom");
            var top = new Plane(O + B, A, C, "top");

            var oppositeFaces = new[]
            {
                new PlanePair(front,back), 
                new PlanePair(left,right), 
                new PlanePair(bottom, top)
            };
            
            int result = 0;
            var minBounds = new Vector3D(Vertices.Min(v => v.X), Vertices.Min(v => v.Y), Vertices.Min(v => v.Z));
            var maxBounds = new Vector3D(Vertices.Max(v => v.X), Vertices.Max(v => v.Y), Vertices.Max(v => v.Z));

            for (int x = (int)Math.Round(minBounds.X,0); x <= (int)Math.Round(maxBounds.X,0); x++)
            {
                for (int y = (int) Math.Round(minBounds.Y,0); y <= (int)Math.Round(maxBounds.Y,0); y++)
                {
                    for(int z= (int)Math.Round(minBounds.Z, 0); z <= (int)Math.Round(maxBounds.Z,0); z++)
                    {
                        var v = new Vector3D(x,y,z);
                        if (oppositeFaces.All(pp => pp.IsBetweenOrOn(v)))
                        {
                            result++;
                        }
                    }
                }
            }
            return result;
        }

        private bool CalculateIsStraight()
        {
            var straight = Definitions.All(p =>
            {
                var result = 
                    IsParallelToAxis(p, xAxis) || 
                    IsParallelToAxis(p, yAxis) || 
                    IsParallelToAxis(p, zAxis);
                return result;
            });
            return straight;
        }

        private static bool IsParallelToAxis(Vector3D vector3D, Vector3D axis)
        {
            var angleBetweenXAxis = Vector3D.AngleBetween(vector3D, axis);
            var result = (int) Math.Round(Math.Abs(angleBetweenXAxis), 0)%90 == 0;
            return result;
        }

        public override string ToString()
        {
            return string.Join("   ", OrderedVertices.Select(o => $"{{{o.X:0},{o.Y:0},{o.Z:0}}}"));
        }

        public bool Equals(Cube other)
        {
            if (ReferenceEquals(null, other)) return false;
            if (ReferenceEquals(this, other)) return true;
            var sequenceEqual = OrderedVertices.SequenceEqual(other.OrderedVertices);
            return sequenceEqual;
        }

        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            if (ReferenceEquals(this, obj)) return true;
            if (obj.GetType() != this.GetType()) return false;
            return Equals((Cube) obj);
        }

        public override int GetHashCode()
        {
            unchecked
            {
                return OrderedVertices?.Aggregate(0, (i, v) => i*367 ^ ((int)v.X*419) ^ ((int)v.Y*293) ^ ((int)v.Z*137)) ?? 0;
            }
        }

        public static bool operator ==(Cube left, Cube right)
        {
            return Equals(left, right);
        }

        public static bool operator !=(Cube left, Cube right)
        {
            return !Equals(left, right);
        }
    }

    class Program
    {
        static void AddCubesFrom(int n, Vector3D[] vertices, List<Cube> cubes )
        {
            System.Windows.Media.Media3D.Geometry3D g;
            for (int x = 0; x <= n; x++)
            {
                for (int y = 0; y <= n; y++)
                {
                    for (int z = 0; z <= n; z++)
                    {
                        var v = new Vector3D(x, y, z);
                        if (!vertices.Contains(v))
                        {
                            var newVertices = vertices.Concat(new[] {v}).ToArray();
                            if (newVertices.Length == 3)
                            {
                                var v0 = newVertices[0];
                                var v1 = newVertices[1]; //the origin of the cube
                                var v2 = newVertices[2];
                                var sideA = v0 - v1;
                                var sideB = v2 - v1;
                                if (Math.Abs(sideA.LengthSquared - sideB.LengthSquared) < 1e-9 && Math.Abs(Vector3D.AngleBetween(sideA, sideB) - 90) < 1e-9)
                                {
                                    var crossProduct = Vector3D.CrossProduct(sideA, sideB);
                                    //make it the same length, and we have a choice of plus or minus
                                    var sideC1 = crossProduct*(sideA.Length/crossProduct.Length);
                                    var sideC2 = crossProduct*-(sideA.Length/crossProduct.Length);
                                    Cube cube;
                                    if (TryMakeCubeFrom(n, v1, sideA, sideB, sideC1, out cube) && !cubes.Contains(cube)) cubes.Add(cube);
                                    if (TryMakeCubeFrom(n, v1, sideA, sideB, sideC2, out cube) && !cubes.Contains(cube)) cubes.Add(cube);
                                }
                            }
                            else
                            {
                                AddCubesFrom(n, newVertices, cubes);
                            }
                        }
                    }
                }
            }
        }

        static bool TryMakeCubeFrom(int n, Vector3D origin, Vector3D a, Vector3D b, Vector3D c, out Cube cube)
        {
            var points = new[]
            {
                origin,
                origin + a,
                origin + b,
                origin + c,
                origin + a + b,
                origin + a + c,
                origin + b + c,
                origin + a + b + c
            };
            if (points.All(IsIntegral) && points.All(v => IsInBounds(n, v)))
            {
                cube = new Cube(points, new [] { a,b,c});
                return true;
            }
            else
            {
                cube = null;
                return false;
            }

        }

        public static bool IsIntegral(Vector3D v)
        {
            var isIntegral = IsIntegral(v.X) &&
                             IsIntegral(v.Y) &&
                             IsIntegral(v.Z);
            return isIntegral;
        }

        static bool IsIntegral(double d)
        {
            return Math.Abs(d - Math.Round(d, 0)) < 1e-9;
        }

        static bool IsInBounds(int n, Vector3D vector)
        {
            return IsInBounds(n, vector.X) &&
                IsInBounds(n, vector.Y) &&
                IsInBounds(n, vector.Z);
        }

        static bool IsInBounds(int n, double coord)
        {
            return coord <= n && coord >= 0;
        }

        
        static Cube[] GetCubes(int n)
        {
            var cubes = new List<Cube>();
            AddCubesFrom(n, new Vector3D[0], cubes);
            return cubes.Distinct().ToArray();
        }


        static void Main(string[] args)
        {
            var cube1 = new Cube(new[]
            {
                new Vector3D(0, 0, 0),
                new Vector3D(3, 0, 0),
                new Vector3D(0, 3, 0),
                new Vector3D(0, 0, 3),
                new Vector3D(0, 3, 3),
                new Vector3D(3, 0, 3),
                new Vector3D(3, 3, 0),
                new Vector3D(3, 3, 3)
            });
            Console.Out.WriteLine(cube1.LatticePoints);

            var cube2 = new Cube(new []
            {
                new Vector3D(0, 2, 2),
                new Vector3D(1, 4, 4),
                new Vector3D(2, 0, 3),
                new Vector3D(2, 3, 0),
                new Vector3D(3, 2, 5),
                new Vector3D(3, 5, 2),
                new Vector3D(4, 1, 1),
                new Vector3D(5, 3, 3)
            });
            Console.Out.WriteLine(cube2.LatticePoints);
            
            for (int i = 1; i <= 10; i++)
            {
                var cubes = GetCubes(i);
                var cubesByLatticePoints = cubes.GroupBy(c => c.LatticePoints);
                var latticePointsDist = string.Join(", ", cubesByLatticePoints.OrderBy(g => g.Key).Select(g => $"{g.Count()} x {g.Key:0000}"));
                var totalLatticePoints = cubes.Sum(c => c.LatticePoints);
                Console.Out.WriteLine($"C({i}) = {cubes.Length}, S = {latticePointsDist}, total: {totalLatticePoints}");
            }
        }
    }
}
