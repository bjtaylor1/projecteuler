using System;
using System.Collections.Generic;
using System.Collections.Immutable;
using System.Linq;
using System.Numerics;
using C5;
using Mpir.NET;

namespace euler615n
{
    class Program
    {
        const int Limit = 1000000;

        static void Main(string[] args)
        {
            var cfs = new CompositeFactors();
            var cfHashes = new C5.HashSet<int>();
            var b = ImmutableSortedDictionary.CreateBuilder<int, int>();
            
            b.Add(0, Limit);
            CompositeFactor initial = new CompositeFactor(b);
            cfs.Add(initial);
            cfHashes.Add(initial.GetHashCode());

            for(int i = 0; i < Limit-1; i++)
            {
                if(i % 100 == 0) Console.Write($"{(double)i / Limit:P1}, count = {cfs.Count} / {cfs.HashCount}\r");
                var firstUnexpanded = cfs.DeleteMin();
                var expansions = firstUnexpanded.Expand();
                foreach(var e in expansions) cfs.Add(e);
                cfs.Add(new CompositeFactor(firstUnexpanded.Factors.ToBuilder().Increase(0)));
            }
            Console.Out.WriteLine($"\nFound {Limit}th factor");
            var answerFactor = cfs.DeleteMin();
            mpz_t result = answerFactor.Factors.Aggregate(new mpz_t(1), (res, f) => res * new mpz_t(Primes.Values[f.Key]).Power(f.Value)).Mod(123454321);
            Console.WriteLine(result);
        }
    }

    public class CompositeFactors
    {
        private readonly IntervalHeap<CompositeFactor> items = new IntervalHeap<CompositeFactor>(Comparer<CompositeFactor>.Create((cf1, cf2) => cf1.ApproxValue.CompareTo(cf2.ApproxValue)));
        private readonly C5.HashSet<long> hashes = new C5.HashSet<long>();
        public void Add(CompositeFactor cf)
        {
            if (hashes.Add(cf.GetLongHashCode()))
                items.Add(cf);
        }

        public int Count => items.Count;
        public int HashCount => hashes.Count;

        public CompositeFactor DeleteMin()
        {
            return items.DeleteMin();
        }

        public CompositeFactor Item(int i)
        {
            return items.Skip(i).First();
        }
    }

    public class CompositeFactor
    {
        public CompositeFactor(ImmutableSortedDictionary<int, int>.Builder factorsBuilder)
        {
            Factors = factorsBuilder.ToImmutable();
            ApproxValue = Factors.Sum(k => k.Value * Math.Log(Primes.Values[k.Key]));
        }

        public double ApproxValue { get; }
        public ImmutableSortedDictionary<int,int> Factors { get; }
        public bool Expanded { get; set; }

        public override bool Equals(object other)
        {
            var factor = other as CompositeFactor;
            return factor != null &&
                   Factors.SequenceEqual(factor.Factors);
        }

        public CompositeFactor[] Expand()
        {
            var expansions = Factors.Select(k => new CompositeFactor(
                Factors
                .ToBuilder()
                .Decrease(k.Key)
                .Increase(k.Key + 1))).ToArray();
            Expanded = true;
            return expansions;
        }

        public override string ToString()
        {
            return string.Join(", ", Factors.Select(f => $"[{f.Key}x{f.Value}]"));
        }

        public override int GetHashCode()
        {
            return GetLongHashCode().GetHashCode();
        }
        
        public long GetLongHashCode()
        {
            long ret = 0;
            foreach (var f in Factors)
            {
                ret = ret * 1000000009 + f.Key;
                ret = ret * 1000000009 + f.Value;
            }
            return ret;
        }
    }

    class Factor
    {
        public Factor(int primeIndex, int power)
        {
            PrimeIndex = primeIndex;
            Power = power;
        }

        public int PrimeIndex { get;  }
        public int Power { get; }
    }

    public static class CompositeFactorExtensions
    {
        public static ImmutableSortedDictionary<int, int>.Builder Increase(this ImmutableSortedDictionary<int, int>.Builder b, int f)
        {
            if (!b.TryGetValue(f, out var newCount)) newCount = 0;
            b[f] = ++newCount;
            return b;
        }

        public static ImmutableSortedDictionary<int, int>.Builder Decrease(this ImmutableSortedDictionary<int, int>.Builder b, int f)
        {
            if (--b[f] == 0) b.Remove(f);
            return b;
        }
    }
}
