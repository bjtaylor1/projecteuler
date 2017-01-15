using System;
using System.Collections.Generic;
using System.Linq;

namespace Helpers
{
    public class Permutations
    {
        public static void Get<T>(T[] items, int numItemsInPermutation, bool allowDuplicates, bool combinationsOnly, Func<T[], bool> process)
        {
            var permutationKey = new PermutationKey(allowDuplicates, combinationsOnly, items.Length, numItemsInPermutation);
            AddPermutation(items, permutationKey, new int[] { }, 0, process);
        }

        public static T[][] Of<T>(T[] items, int numItemsInPermutation, bool allowDuplicates, bool combinationsOnly)
        {
            var result = new List<T[]>();
            Get(items, numItemsInPermutation, allowDuplicates, combinationsOnly, obj =>
            {
                result.Add(obj);
                return true;
            });
            return result.ToArray();
        }

        static bool AddPermutation<T>(T[] items, PermutationKey n, int[] p, int oldi, Func<T[], bool> process)
        {
            if (p.Length == n.NumItemsInPermutation)
                return  process(p.Select(i => items[i]).ToArray());
            else
            {
                int lowerLimit = n.CombinationsOnly ? oldi : 0;
                bool cont = true;
                for (int i = lowerLimit; i < n.NumItemsTotal && cont; i++)
                {
                    if (n.AllowDuplicates || !p.Contains(i)) cont = AddPermutation(items, n, p.Concat(new[] {i}).ToArray(), i, process);
                }
            }
            return true;
        }
    }

    internal class PermutationKey : IEquatable<PermutationKey>
    {
        public bool Equals(PermutationKey other)
        {
            if (ReferenceEquals(null, other)) return false;
            if (ReferenceEquals(this, other)) return true;
            return AllowDuplicates == other.AllowDuplicates && CombinationsOnly == other.CombinationsOnly && NumItemsTotal == other.NumItemsTotal && NumItemsInPermutation == other.NumItemsInPermutation;
        }

        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            if (ReferenceEquals(this, obj)) return true;
            if (obj.GetType() != this.GetType()) return false;
            return Equals((PermutationKey) obj);
        }

        public override int GetHashCode()
        {
            unchecked
            {
                var hashCode = AllowDuplicates.GetHashCode();
                hashCode = (hashCode*397) ^ CombinationsOnly.GetHashCode();
                hashCode = (hashCode*397) ^ NumItemsTotal;
                hashCode = (hashCode*397) ^ NumItemsInPermutation;
                return hashCode;
            }
        }

        public static bool operator ==(PermutationKey left, PermutationKey right)
        {
            return Equals(left, right);
        }

        public static bool operator !=(PermutationKey left, PermutationKey right)
        {
            return !Equals(left, right);
        }

        public bool AllowDuplicates { get; }
        public bool CombinationsOnly { get; }
        public int NumItemsTotal { get;  }
        public int NumItemsInPermutation { get; }

        public PermutationKey(bool allowDuplicates, bool combinationsOnly, int numItemsTotal, int numItemsInPermutation)
        {
            AllowDuplicates = allowDuplicates;
            CombinationsOnly = combinationsOnly;
            NumItemsTotal = numItemsTotal;
            NumItemsInPermutation = numItemsInPermutation;
        }

    }
}
