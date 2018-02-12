using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using Mpir.NET;

namespace _0191
{
    class Program
    {
        int N;

        public Program(int n)
        {
            N = n;
        }

        readonly Dictionary<CacheKey, mpz_t> cache = new Dictionary<CacheKey, mpz_t>();
        mpz_t Solve(string s)
        {
            var ck = new CacheKey(s);
            if (ck.NumLate > 1 || ck.NumConsecutiveAbsent > 2) return 0;
            if (ck.Length == N) return 1;
            if (cache.TryGetValue(ck, out mpz_t val)) return val;
            var retval  = "LOA".Aggregate(new mpz_t(0), (st, c) => st + Solve(s + c));
            cache[ck] = retval;
            return retval;
        }
        static void Main(string[] args)
        {
            Console.WriteLine(new Program(30).Solve(""));
        }

        class CacheKey
        {
            public int Length { get; }
            public int NumLate { get; }
            public int NumConsecutiveAbsent { get; }
            public CacheKey(string s)
            {
                Length = s.Length;
                NumLate = s.Count(c => c == 'L');
                NumConsecutiveAbsent = Regex.Match(s, "A+$").Value.Length;
            }

            public override bool Equals(object obj)
            {
                return obj is CacheKey key &&
                       Length == key.Length &&
                       NumLate == key.NumLate &&
                       NumConsecutiveAbsent == key.NumConsecutiveAbsent;
            }

            public override int GetHashCode()
            {
                var hashCode = -1756240114;
                hashCode = hashCode * -1521134295 + Length.GetHashCode();
                hashCode = hashCode * -1521134295 + NumLate.GetHashCode();
                hashCode = hashCode * -1521134295 + NumConsecutiveAbsent.GetHashCode();
                return hashCode;
            }
        }
    }
}
