using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Media.Media3D;
using NLog;

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

        public Triple(params int[] sides): this(sides, (int)Math.Sqrt(sides.Select(s => s* s).Sum()))
        {
        }
        public Triple(int[] sides, int square)
        {
            Array.Sort(sides);
            Sides = sides;
            Square = square;
            Dimensions = Sides.Count(s => s > 0);
            Vector = new Vector3D(Sides[0], Sides[1], Sides[2]);

            IsPrimitive = CalcIsPrimitive();
        }

        private bool CalcIsPrimitive()
        {
            
            var nonZeroSides = Sides.Where(s => s != 0).ToArray();
            for (int f = 2; f <= (int)Math.Ceiling((double)Sides.Max()); f++)
            {
                if (nonZeroSides.All(s => Program.IsIntegral((double) s/f)))
                    return false;
            }
            return true;
        }

        public bool IsPrimitive { get;  }

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
            return $"{string.Join(",", Sides.Select(s => s.ToString()))} => {Square}";
        }

        public Cube GetCube(int n)
        {
            var possibleBasicCubes = GetCubeFromVector(n, Vector).ToArray();
            var basicCube = possibleBasicCubes.First();

            if (basicCube != null)
            {
                var vs = VectorVariantFinder.FindAllCombinationsOf(Vector);
                var extraCubes = vs.SelectMany(v => GetCubeFromVector(n, v)).Distinct().Where(c => !c.Equals(basicCube)).ToArray();

                if (possibleBasicCubes.Length > 1)
                {
                    var otherExtras = possibleBasicCubes.Skip(1).Where(c => !extraCubes.Contains(c)).ToArray();
                    if (otherExtras.Any())
                    {
                        LogManager.GetCurrentClassLogger().Warn($"{this} yields other unseen extras {string.Join(",   ", otherExtras.Select(c => c.ToString()))}");
                    }
                }
                basicCube.Variants = extraCubes;
            }
            return basicCube;
        }

        private static Cube[] GetCubeFromVector(int n, Vector3D vector)
        {
            var cubes = new List<Cube>();
            var o = new Vector3D(0, 0, 0);
            var vs = VectorVariantFinder.FindAllVariantsAtRightAnglesTo(vector);
            var bs = vs.Where(v => Math.Abs(v.LengthSquared - vector.LengthSquared) < 1e-9
                                   && Math.Abs(Math.Abs(Vector3D.AngleBetween(v, vector)) - 90) < 1e-9);
            foreach (var b in bs)
            {
                var c = Vector3D.CrossProduct(vector, b);
                c *= (vector.Length/c.Length);
                Cube cube;
                if (Cube.TryMakeCubeFrom(o, vector, b, c, out cube) && !cubes.Contains(cube))
                {
                    cubes.Add(cube);
                }
            }
            if(!cubes.Any()) throw new InvalidOperationException($"No cubes found for vector {vector}");
            cubes.RemoveAll(cube => !cube.Dimensions.All(d => d <= n));
            return cubes.Distinct().ToArray();

        }
    }
}