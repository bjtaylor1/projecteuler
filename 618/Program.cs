using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Collections.Immutable;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
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
            for (int k = 2; k <=24; k++)
            {
                f[k] = f[k - 2] + f[k - 1];
                var s = S.Calculate(f[k]);
                Console.WriteLine($"S({f[k]}) = {s.Total}");
                tot += s.Total;
            }
            Console.WriteLine(tot % 1e9);
        }
    }


    public class S
    {
        public override string ToString()
        {
            return Total.ToString();
        }

        private readonly Lazy<mpz_t> total;

        public S(ImmutableArray<Pfs> values)
        {
            Values = values;
            this.total = new Lazy<mpz_t>(() => values.Aggregate(new mpz_t(0), (s,i) => s+i.Total));
#if DEBUG
            Debug.WriteLine(this.total.Value);
#endif
        }

        public mpz_t Total => total.Value;

        public ImmutableArray<Pfs> Values { get; private set; }

        public static S Calculate(int n)
        {
            var fs = GetFactorsCached(n);
            S result = new S(fs);
            return result;
        }

        private static List<Pfs> GetFactors(int n)
        {
            var fs = new List<Pfs>();
            DoFactorize(n, fs, new Stack<int>(), 0);
            return fs;
        }

        private static readonly ConcurrentDictionary<int, ImmutableArray<Pfs>> pfsCache = new ConcurrentDictionary<int, ImmutableArray<Pfs>>();

        private static ImmutableArray<Pfs> GetFactorsCached(int n)
        {
            ImmutableArray<Pfs> result = pfsCache.GetOrAdd(n, CalcFactors);
            return result;
        }

        private static ImmutableArray<Pfs> CalcFactors(int n)
        {
            ImmutableArray<Pfs> result;
            //if(n <= 1)
            //{
            //    result = ImmutableArray.Create(new Pfs(ImmutableArray.Create<int>()));
            //}
            //else if (n == 2)
            //{
            //    result = ImmutableArray.Create( new Pfs(ImmutableArray.Create(2)) );
            //}
            //else
            {
                var pfss = new HashSet<Pfs>();
                foreach(var p in Primes.Values.Where(p => p <= n/2 ))
                {
                    var pfssLower = GetFactorsCached(n - p);
                    var pfssNew = pfssLower
                        .Select(pfs => new Pfs(pfs.Values.Concat(new[] { p }).OrderBy(v => v).ToImmutableArray()))
                        .ToArray();
                    foreach (var f in pfssNew) pfss.Add(f);
                }
                if (Primes.Values.Contains(n))
                {
                    pfss.Add(new Pfs(ImmutableArray.Create(n)));
                }

                result = pfss.ToImmutableArray();
            }
            return result;
        }

        static void DoFactorize(int n, List<Pfs> all, Stack<int> current, int depth)
        {
            if (Primes.Values.Contains(n))
            {
                all.Add(new Pfs(current.Concat(new[] { n }).OrderBy(i => i).ToImmutableArray()));
            }

            foreach (var p in Primes.Values.Where(p => p <= n / 2 && p >= current.FirstOrDefault()).ToArray())
            {
                current.Push(p);
                if (depth == 0)
                    Console.Write($"{(double)(p * 2) / n:P1}\r");
                DoFactorize(n - p, all, current, depth + 1);
                current.Pop();
            }
        }

        public static S operator+(S lhs, S rhs)
        {
            var pfss = new List<Pfs>();
            foreach(var lhspfs in lhs.Values)
            {
                foreach(var rhspfs in rhs.Values)
                {
                    var sum = lhspfs + rhspfs;
                    pfss.Add(sum);
                }
            }
            var tot = new S(pfss.ToImmutableArray());
            return tot;
        }
    }

    public class Pfs
    {
        private readonly Lazy<mpz_t> total;
        public mpz_t Total => total.Value;
        public Pfs(ImmutableArray<int> values, mpz_t knownTotal)
        {
            Values = values;
            total = new Lazy<mpz_t>(() => knownTotal);
#if DEBUG
            Debug.WriteLine(this.total.Value);
#endif
        }
        public Pfs(ImmutableArray<int> values)
        {
            Values = values;
            total = new Lazy<mpz_t>(() => values.Any() ? values.Where(i => i > 1).Aggregate(new mpz_t(1), (s,i) => s*i) : 0);
#if DEBUG
            Debug.WriteLine(this.total.Value);
#endif
        }
        public ImmutableArray<int> Values { get; }
        public override string ToString()
        {
            return $"{string.Join(",", Values)} = {Total}";
        }

        public override bool Equals(object obj)
        {
            return obj is Pfs pfs &&
                   Values.SequenceEqual(pfs.Values);
        }


        public override int GetHashCode()
        {
            unchecked
            {
                return 1291433875 + Values.Aggregate(397, (p, i) => (p * 397) + i);
            }
            
        }

        public static Pfs operator+(Pfs lhs, Pfs rhs)
        {
            var tot = lhs.Values.Concat(rhs.Values).OrderBy(i => i).ToImmutableArray();
            var result = new Pfs(tot, lhs.Total * rhs.Total);
            return result;
        }
    }
}
