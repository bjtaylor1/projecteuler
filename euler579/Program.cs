using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media.Animation;
using System.Windows.Media.Media3D;

namespace euler579
{
    class Cube : IEquatable<Cube>
    {
        private readonly Lazy<bool> isStraight;
        public bool IsStraight { get { return isStraight.Value; } }
        public Vector3D[] Vertices { get;  }
        public Vector3D[] OrderedVertices { get; }

        private static readonly Vector3D xAxis = new Vector3D(1, 0, 0);
        private static readonly Vector3D yAxis = new Vector3D(0, 1, 0);
        private static readonly Vector3D zAxis = new Vector3D(0, 0, 1);

        public Cube(Vector3D[] vertices)
        {
            if (vertices.Length != 8) throw new InvalidOperationException("Sanity check failed.");
            OrderedVertices = vertices.OrderBy(v => v.X).ThenBy(v => v.Y).ThenBy(v => v.Z).ToArray();
            Vertices = vertices.ToArray();
            isStraight = new Lazy<bool>(CalculateIsStraight);
        }

        private bool CalculateIsStraight()
        {
            var straight = Vertices.Skip(1).Take(3).All(p =>
            {
                var vector3D = p - Vertices[0];
                var result = IsParallelToAxis(vector3D, xAxis) || 
                    IsParallelToAxis(vector3D, yAxis) || 
                    IsParallelToAxis(vector3D, zAxis);
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
                cube = new Cube(points);
                return true;
            }
            else
            {
                cube = null;
                return false;
            }

        }

        static bool IsIntegral(Vector3D v)
        {
            return IsIntegral(v.X) &&
                IsIntegral(v.Y) &&
                IsIntegral(v.Z);
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
            for (int i = 1; i <= 5; i++)
            {
                var cubes = GetCubes(i);
                var straight = cubes.Count(c => c.IsStraight);
                var nonStraight = cubes.Length - straight;
                Console.Out.WriteLine($"C({i}) = {cubes.Length}, {straight}/{nonStraight}");

            }
        }
    }
}
