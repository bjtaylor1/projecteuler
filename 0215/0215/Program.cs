using System;
using System.Collections.Generic;
using System.Collections.Concurrent;
using System.Linq;

namespace _0215
{
    public class C
    {
        public const int N = 32;
        public const int H = 10;
        public static int[] B = { 2, 3 };
    }
    class Program
    {
        static int[][] rows;


        static void Main(string[] args)
        {
            rows = MakeRows();
            var answer = W(C.H, null);
            Console.WriteLine(answer);
        }

        static void MakeRowsInternal(Stack<int> current, List<int[]> all)
        {
            var sum = current.Sum();
            if (sum == C.N) all.Add(current.ToArray());
            else if (C.N - sum >= C.B.Min())
            {
                foreach (int b in C.B)
                {
                    current.Push(b);
                    MakeRowsInternal(current, all);
                    current.Pop();
                }
            }
        }
        static int[][] MakeRows()
        {
            var current = new Stack<int>();
            var all = new List<int[]>();
            MakeRowsInternal(current, all);
            return all.ToArray();
        }

        static ConcurrentDictionary<int, int[]> compatibilityCache = new ConcurrentDictionary<int, int[]>();

        static int[] CalcCompatible(int i)
        {
            var compat = new List<int>();
            for (int r = 0; r < rows.Length; r++)
                if (r != i && IsCompatible(r, i)) compat.Add(r);
            return compat.ToArray();
        }
        static bool IsCompatible(int i, int j)
        {
            for (int sumi = 0, sumj = 0, ii = 0, ij = 0; sumi < C.N && sumj < C.N;)
            {
                if (sumi < sumj) sumi += rows[i][ii++];
                else sumj += rows[j][ij++];
                if (sumi == sumj) return false;
            }
            return true;
        }

        static ConcurrentDictionary<(int n, int? below), long> pCache = new ConcurrentDictionary<(int n, int? below), long>(
            EqualityComparers.ExpressionEqualityComparer<(int n, int? below)>.Create(KeysEquivalent, KeyHash));

        static ConcurrentDictionary<int?, int?> normCache = new ConcurrentDictionary<int?, int?>();
        static int? Norm(int? i) => normCache.GetOrAdd(i, CalcNorm);
        static int? CalcNorm(int? i)
        {
            if (i == null) return null;
            var rev = rows[i.Value].Reverse().ToArray();
            var mirr = Array.FindIndex(rows, r => r.SequenceEqual(rev));
            if (mirr == -1) throw new NotImplementedException($"Mirror of {i} not found");
            return new[] { mirr, i }.Min();
        }

        private static int KeyHash((int n, int? below) key)
        {
            unchecked
            {
                var hash = key.n.GetHashCode();
                if (key.below.HasValue) hash = (hash * 397) ^ Norm(key.below).GetHashCode();
                return hash;
            }
        }

        private static bool KeysEquivalent((int n, int? below) lhs, (int n, int? below) rhs)
        {
            var eq = Equals(lhs.n, rhs.n) && Equals(Norm(lhs.below), Norm(rhs.below));
            return eq;
        }

        static long W(int n, int? below) => pCache.GetOrAdd((n, below), W);
        static long W((int n, int? below) p)
        {
            long res;
            if (p.n == 0) res = 1;
            else
            {
                int[] compatible = p.below == null ?
                    Enumerable.Range(0, rows.Length).ToArray() :
                    compatibilityCache.GetOrAdd(p.below.Value, CalcCompatible);
                res = compatible.Sum(c => W(p.n - 1, c));
            }
            return res;
        }

    }
}