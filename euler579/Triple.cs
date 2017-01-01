using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Media.Media3D;

namespace euler579
{
    public class Triple : IEquatable<Triple>
    {
        public bool Equals(Triple other)
        {
            if (ReferenceEquals(null, other)) return false;
            if (ReferenceEquals(this, other)) return true;
            return Sides.SequenceEqual(other.Sides);
        }

        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            if (ReferenceEquals(this, obj)) return true;
            if (obj.GetType() != this.GetType()) return false;
            return Equals((Triple) obj);
        }

        public override int GetHashCode()
        {
            return Sides?.Aggregate((i,s) => (i * 397)^s) ?? 0;
        }

        public static bool operator ==(Triple left, Triple right)
        {
            return Equals(left, right);
        }

        public static bool operator !=(Triple left, Triple right)
        {
            return !Equals(left, right);
        }

        public Triple(int[] sides, int square)
        {
            Array.Sort(sides);
            Sides = sides;
            Square = square;
            Dimensions = Sides.Count(s => s > 0);
            Vector = new Vector3D(Sides[0], Sides[1], Sides[2]);
        }

        public bool IsUnique { get; set; }

        public bool IsMultipleOf(Triple other)
        {
            var multiples = new List<double>();
            for(int i = 0; i < 3; i++)
            {
                if ((Sides[i] == 0) != (other.Sides[i] == 0))
                    return false;
                else if (Sides[i] != 0 && (other.Sides[i] != 0))
                    multiples.Add((double)Sides[i] / other.Sides[i]);
            }
            bool isMultiple = Math.Abs(multiples.Max() - multiples.Min()) < 2e-9;
            return isMultiple;
        }
        

        public Vector3D Vector { get;  }

        public int[] Sides { get; }
        public int Square { get; }
        public int Dimensions { get; }

        public override string ToString()
        {
            return $"{string.Join(",", Sides.Select(s => s.ToString()))} => {Square} ({Dimensions}D)";
        }

        public Cube[] GetCubes(int n)
        {
            var cubes = new List<Cube>();
            var o = new Vector3D(0, 0, 0);
            var vs = VectorVariantFinder.FindAllVariantsAtRightAnglesTo(Vector);
            var bs = vs.Where(v => Math.Abs(v.LengthSquared - Vector.LengthSquared) < 1e-9 
                        && Math.Abs(Math.Abs(Vector3D.AngleBetween(v, Vector)) - 90) < 1e-9);
            foreach (var b in bs)
            {
                var c = Vector3D.CrossProduct(Vector, b);
                c *= (Vector.Length / c.Length);
                Cube cube;
                if (Cube.TryMakeCubeFrom(o, Vector, b, c, out cube) && cube.Dimensions.All(d => d <= n) && !cubes.Contains(cube))
                {
                    cubes.Add(cube);
                }
            }
            return cubes.ToArray();
        }
    }
}