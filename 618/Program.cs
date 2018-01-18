using System;
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
            var s9real = S.Calculate(9);

            var s5 = S.Calculate(5);
            var s4 = S.Calculate(4);
            var s9calc = s5 + s4;

            Console.WriteLine(s9calc);
            

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
            var fs = new List<Pfs>();

            DoFactorize(n, fs, new Stack<int>(), 0);
            S result = new S(fs.ToImmutableArray());
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
            total = new Lazy<mpz_t>(() => values.Aggregate(new mpz_t(1), (s,i) => s*i));
#if DEBUG
            Debug.WriteLine(this.total.Value);
#endif
        }
        public ImmutableArray<int> Values { get; }
        public override string ToString()
        {
            return $"{string.Join(",", Values)} = {Total}";
        }

        public static Pfs operator+(Pfs lhs, Pfs rhs)
        {
            var tot = lhs.Values.Concat(rhs.Values).OrderBy(i => i).ToImmutableArray();
            var result = new Pfs(tot, lhs.Total * rhs.Total);
            return result;
        }
    }
}
