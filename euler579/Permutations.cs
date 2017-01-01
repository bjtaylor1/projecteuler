using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace euler579
{
    public class Permutations
    {
        private static readonly ConcurrentDictionary<Tuple<bool,int,int>, int[][]> indexPermutations= new ConcurrentDictionary<Tuple<bool,int,int>, int[][]>();

        public static T[][] Of<T>(T[] items, int length, bool allowDuplicates = false)
        {
            var indexPerms = indexPermutations.GetOrAdd(Tuple.Create(allowDuplicates, items.Length, length), GetIndexPermutations);
            var permutations = indexPerms.Select(indexPerm => indexPerm.Select(i => items[i]).ToArray()).ToArray();
            return permutations;
        }

        private static int[][] GetIndexPermutations(Tuple<bool,int,int> n)
        {
            var result = new List<int[]>();
            AddPermutation(n, new int[] { }, result);
            return result.ToArray();
        }
        
        static void AddPermutation(Tuple<bool, int,int> n, int[] p, List<int[]> result)
        {
            if (p.Length == n.Item3)
                result.Add(p);
            else
            {
                for (int i = 0; i < n.Item2; i++)
                    if (n.Item1 || !p.Contains(i)) AddPermutation(n, p.Concat(new[] { i }).ToArray(), result);
            }
        }
    }
}
