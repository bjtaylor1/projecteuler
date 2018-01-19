using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Collections.Immutable;
using System.Diagnostics;
using System.Linq;
using Helpers;
using Mpir.NET;

namespace _618
{
    class Program
    {
        static void Main(string[] args)
        {
            var f = new int[25];
            f[0] = 0;
            f[1] = 1;
            mpz_t tot = 0;
            var s = new Dictionary<int, S>();
            for (int k = 2; k <= 24; k++)
            {
                f[k] = f[k - 2] + f[k - 1];
                s[f[k]] = S.Calculate(f[k]);
                if (k > 2)
                {
                    var sc = s[f[k - 2]] + s[f[k - 1]];
                    if (sc.Total != s[f[k]].Total)
                    {
                        Console.WriteLine($"Different, k={k}, n={f[k]}, s={s[f[k]]}, sc={sc}");
                    }
                }
                Console.WriteLine($"k={k}, S({f[k]})={s[f[k]]}");

            }
            Console.WriteLine(tot);
        }
    }


    public class S
    {
        public override string ToString()
        {
            return $"{N}: {Total.ToString()}";
        }

        private readonly Lazy<mpz_t> total;

        public S(ImmutableArray<Pfs> values, int n)
        {
            Values = values;
            N = n;
            this.total = new Lazy<mpz_t>(() => values.Aggregate(new mpz_t(0), (s, i) => (s + i.Total) % 1e9));
#if DEBUG
            Debug.WriteLine(this.total.Value);
#endif
        }

        public mpz_t Total => total.Value;

        public ImmutableArray<Pfs> Values { get; private set; }
        public int N { get; }

        public static S Calculate(int n)
        {
            var fs = GetFactorsCached(n);
            S result = new S(fs, n);
            return result;
        }

        private static readonly ConcurrentDictionary<int, ImmutableArray<Pfs>> PfsCache = new ConcurrentDictionary<int, ImmutableArray<Pfs>>();

        private static ImmutableArray<Pfs> GetFactorsCached(int n)
        {
            ImmutableArray<Pfs> result = PfsCache.GetOrAdd(n, CalcFactors);
            return result;
        }

        private static ImmutableArray<Pfs> CalcFactors(int n)
        {
            return CalcFactorsFrom(n, 0);
        }

        private static ImmutableArray<Pfs> CalcFactorsFrom(int n, int minPrimeExc)
        {
            ImmutableArray<Pfs> result;
            var pfss = new HashSet<Pfs>();
            foreach (var p in Primes.Values.Where(p => p <= n / 2 && p > minPrimeExc))
            {
                var pfssLower = GetFactorsCached(n - p);
                var pfssNew = pfssLower
                    .Select(pfs => pfs.Add(p))
                    .ToArray();
                foreach (var f in pfssNew)
                {
                    pfss.Add(f);
                }
            }
            if (Primes.Values.Contains(n))
            {
                pfss.Add(new Pfs(new[] { new KeyValuePair<int, int>(n, 1) }.ToImmutableDictionary()));
            }

            result = pfss.ToImmutableArray();
            return result;
        }

        public static S operator +(S lhs, S rhs)
        {
            var pfss = new List<Pfs>();
            foreach (var lhspfs in lhs.Values)
            {
                foreach (var rhspfs in rhs.Values)
                {
                    var sum = lhspfs + rhspfs;
                    pfss.Add(sum);
                }
            }
            pfss.AddRange(CalcFactorsFrom(lhs.N + rhs.N, new[] { lhs.N, rhs.N }.Min()));
            var tot = new S(pfss.Distinct().ToImmutableArray(), lhs.N + rhs.N);
            return tot;
        }
    }

    public class Pfs
    {
        private readonly Lazy<mpz_t> total;
        public mpz_t Total => total.Value;
        public Pfs(ImmutableDictionary<int, int> values, mpz_t knownTotal)
        {
            Values = values;
            total = new Lazy<mpz_t>(() => knownTotal);
#if DEBUG
            Debug.WriteLine(this.total.Value);
#endif
        }
        public Pfs(ImmutableDictionary<int, int> values)
        {
            Values = values;
            total = new Lazy<mpz_t>(() => values.Any() ? values.Where(i => i.Key > 1)
                .Aggregate(new mpz_t(1), (s, i) => s * new mpz_t(i.Key).Power(i.Value)) : 0);
#if DEBUG
            Debug.WriteLine(this.total.Value);
#endif
        }
        public ImmutableDictionary<int, int> Values { get; }
        public override string ToString()
        {
            return $"{string.Join(",", Values)} = {Total}";
        }

        public override bool Equals(object obj)
        {
            return obj is Pfs pfs &&
                   Values.SequenceEqual(pfs.Values);
        }

        public Pfs Add(int n)
        {
            var builder = Values.ToBuilder();
            if (builder.TryGetValue(n, out int val))
                val++;
            else val = 1;
            builder[n] = val;
            var pfs = new Pfs(builder.ToImmutable());
            return pfs;
        }

        public override int GetHashCode()
        {
            unchecked
            {
                return 1291433875 + Values
                    .SelectMany(k => new[] { k.Key, k.Value })
                    .Aggregate(397, (p, i) => p * 397 + i);
            }

        }

        public static Pfs operator +(Pfs lhs, Pfs rhs)
        {
            var tot = lhs.Values.Keys.Concat(rhs.Values.Keys).Distinct()
                .Select(k =>
                {
                    if (!lhs.Values.TryGetValue(k, out int val)) val = 0;
                    if (rhs.Values.TryGetValue(k, out int rval)) val += rval;
                    return new KeyValuePair<int, int>(k, val);
                }).ToImmutableDictionary();
            var result = new Pfs(tot, lhs.Total * rhs.Total);
            return result;
        }
    }
}
