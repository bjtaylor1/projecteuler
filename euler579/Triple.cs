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
        private static List<int> extrasAndDimensions = new List<int>();
        public override string ToString()
        {
            return $"{string.Join(",", Sides.Select(s => s.ToString()))} => {Square} ({Dimensions}D)";
        }

        public Cube[] GetCubes(int n)
        {
/*
            var vs = VectorVariantFinder.FindAllCombinationsOf(Vector);
*/
            var basicCube = GetCubeFromVector(n, Vector);

            if (basicCube != null)
            {
                var vs = VectorVariantFinder.FindAllCombinationsOf(Vector);
                var extraCubes = vs.Select(v => GetCubeFromVector(n, v)).Distinct().Where(c => c != null && c != basicCube).ToArray();
                var cubes = new[] {basicCube}.Concat(extraCubes).ToArray();
                if(cubes.Select(c => c.GetRepeatability(n)).Distinct().Count() != 1)
                    throw new InvalidOperationException("Different repeatabilities!");

                var repeatabilityAndCombinations = cubes.Sum(c => c.GetRepeatability(n));
                basicCube.RepeatabilityCombinations = repeatabilityAndCombinations;
                var combinations = extraCubes.Length + 1;
                if (!extrasAndDimensions.Contains(combinations))
                {
                    extrasAndDimensions.Add(combinations);
                    LogManager.GetCurrentClassLogger().Info($"{basicCube.A}: {combinations}");
                }
                basicCube.SetCombinations(combinations);
                //LogManager.GetCurrentClassLogger().Info($"Cube.A = {basicCube.A}, Combinations = {basicCube.GetCombinations()}, totalIncExtras = {extraCubes.Length + 1}, extras A's: {string.Join("   ", extraCubes.Select(e => e.A))}");
            }
/*
            var extraCubes = vs.Select(v => GetCubeFromVector(n, v)).Distinct().Where(c => c != null && c != basicCube).ToArray();
            if (extraCubes.Any() && (basicCube?.GetCombinations() ?? 0) != extraCubes.Length + 1)
            {
                var combsBasic = basicCube?.GetCombinations().ToString() ?? "none";
                var basicDefinitions = basicCube == null ? "none" : string.Join(", ", basicCube?.Definitions.Select(d => d.ToString()));
                LogManager.GetCurrentClassLogger().Debug($"{Vector}: Basic combinations: {combsBasic}, Dimensions: {basicCube.NumDimensions}, Extras: {extraCubes.Length} ({string.Join(", ", extraCubes.Select(c => c.A.ToString()))}\nBasic:\n{basicDefinitions}\nExtras:\n{string.Join("\n", extraCubes.Select(c => string.Join(", ", c.Definitions.Select(d => d.ToString()))))})");
            }
*/
            var allCubes = new[] { basicCube}/*.Concat(extraCubes).Distinct()*/.Where(c => c != null).ToArray();
            return allCubes;
        }

        private static Cube GetCubeFromVector(int n, Vector3D vector)
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
                if (Cube.TryMakeCubeFrom(o, vector, b, c, out cube) && cube.Dimensions.All(d => d <= n) && !cubes.Contains(cube))
                {
                    cubes.Add(cube);
                }
            }

            if(cubes.Count > 1) throw new InvalidOperationException("More than one cube from a vector!");
            if (cubes.Any())
            {
                var cube = cubes.Single();
                if (cube.A != vector) throw new InvalidOperationException("Cube's A not equal to triple vector");
                return cube;
            }
            return null;
        }
    }
}